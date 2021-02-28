
//  Copyright (c) 2014 - 2021 Maksym V. Bilinets.
//
//  This file is part of Dataflow++.
//
//  Dataflow++ is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Dataflow++ is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with Dataflow++. If not, see <http://www.gnu.org/licenses/>.

#if !defined(DATAFLOW___QT_H)
#error '.inl' file can't be included directly. Use 'qt.h' instead
#endif

#include "qt/internal/qobject_builder.h"

#include <dataflow/utility/std_future.h>
#include <dataflow/utility/utility.h>

namespace dataflow
{
namespace qt
{
namespace internal
{
namespace detail
{
template <std::size_t N, std::size_t I, bool finished, std::size_t... Vs>
struct filtered_index_sequence_builder;

template <std::size_t N, std::size_t I, std::size_t... Vs>
struct filtered_index_sequence_builder<N, I, false, 1, Vs...>
: filtered_index_sequence_builder<N, I + 1, I + 1 == N, Vs..., I>
{
};

template <std::size_t N, std::size_t I, std::size_t... Vs>
struct filtered_index_sequence_builder<N, I, false, 0, Vs...>
: filtered_index_sequence_builder<N, I + 1, I + 1 == N, Vs...>
{
};

template <std::size_t N, std::size_t... Vs>
struct filtered_index_sequence_builder<N, N, true, Vs...>
{
  using type = std14::index_sequence<Vs...>;
};
}

template <std::size_t... Ps>
using make_filtered_index_sequence =
  typename detail::filtered_index_sequence_builder<sizeof...(Ps),
                                                   0,
                                                   sizeof...(Ps) == 0,
                                                   Ps...>::type;

class qobject_patch
{
public:
  explicit qobject_patch(const qobject& curr, const qobject& prev)
  : global_signal_indices_{}
  {
  }

  explicit qobject_patch(std::vector<int> global_signal_indices)
  : global_signal_indices_{std::move(global_signal_indices)}
  {
  }

  qobject apply(const qobject& obj) const
  {
    for (const auto& idx : global_signal_indices_)
    {
      const QMetaMethod method = obj.get()->metaObject()->method(idx);
      method.invoke(obj.get());
    }

    return obj;
  }

private:
  std::vector<int> global_signal_indices_;
};
}
}
}

namespace dataflow
{
namespace core
{
template <> struct patch_type<qt::qobject>
{
  using type = dataflow::qt::internal::qobject_patch;
};
}
}

namespace dataflow
{
namespace qt
{
namespace internal
{
template <typename... Refs,
          std::size_t... ReadOnlyIndices,
          std::size_t... ReadWriteIndices,
          std::size_t... SignalsIndices>
ref<qobject>
make_qml_context(const std::tuple<std::pair<std::string, Refs>...>& defs,
                 const std14::index_sequence<ReadOnlyIndices...>&,
                 const std14::index_sequence<ReadWriteIndices...>&,
                 const std14::index_sequence<SignalsIndices...>&)
{
  const auto read_only_props =
    std::make_tuple(std::get<ReadOnlyIndices>(defs)...);

  const auto read_write_props =
    std::make_tuple(std::get<ReadWriteIndices>(defs)...);

  const auto signal_definitions =
    std::make_tuple(std::get<SignalsIndices>(defs)...);

  class policy
  {
  public:
    std::string label() const
    {
      return "qml-context";
    }

    qobject calculate(
      const core::data_type_t<
        typename std::tuple_element<ReadOnlyIndices,
                                    std::tuple<Refs...>>::type>&... values)
    {
      internal::qobject_builder builder;

      utility::for_each_tuple_element(
        read_only_props_, [&builder](const auto& def) {
          builder.add_property(def.first, [x = def.second]() {
            using type = core::data_type_t<decltype(x)>;
            return convert_to_qml_type(x.template value<type>());
          });
        });

      utility::for_each_tuple_element(
        read_write_props_, [&builder](const auto& def) {
          builder.add_property(
            def.first,
            [x = def.second]() { return convert_to_qml_type(*x); },
            [x = def.second](const QVariant& v) {
              using type = core::data_type_t<decltype(x)>;

              const auto& value = converter<type>::from_qml_type(v);
              if (*x != value)
              {
                x = value;
                return true;
              }
              return false;
            });
        });

      utility::for_each_tuple_element(
        signal_definitions_, [&builder](const auto& def) {
          builder.add_slot(def.first, [x = def.second]() { return x(); });
        });

      return builder.build();
    };

    qobject_patch prepare_patch(
      const core::diff_type_t<core::data_type_t<
        typename std::tuple_element<ReadOnlyIndices,
                                    std::tuple<Refs...>>::type>>&... diffs)
    {
      std::vector<bool> flags{(diffs.curr() != diffs.prev())...};

      std::vector<int> global_signal_indices;
      for (int i = 0; i < flags.size(); ++i)
      {
        if (flags[i])
        {
          global_signal_indices.push_back(
            QObject::staticMetaObject.methodCount() + i);
        }
      }

      return qobject_patch{global_signal_indices};
    }

    decltype(read_only_props) read_only_props_;
    decltype(read_write_props) read_write_props_;
    decltype(signal_definitions) signal_definitions_;
  };

  return core::LiftPatcher(
    policy{read_only_props, read_write_props, signal_definitions},
    std::get<ReadOnlyIndices>(defs).second...);
}

inline ref<qobject> make_qml_context(const std::tuple<>&,
                                     const std14::index_sequence<>&,
                                     const std14::index_sequence<>&,
                                     const std14::index_sequence<>&)
{
  internal::qobject_builder builder;
  return Const(builder.build());
}

}
}

template <typename T>
std::pair<std::string, var<T>> qt::QmlPropertyRW(const std::string& name,
                                                 var<T>& x)
{
  return std::make_pair(name, x);
}

inline std::pair<std::string, sig> qt::QmlFunction(const std::string& name,
                                                   const sig& x)
{
  return std::make_pair(name, x);
}

template <typename T>
std::pair<std::string, ref<T>> qt::QmlProperty(const std::string& name,
                                               const ref<T>& x)
{
  return std::make_pair(name, x);
}

template <typename... Refs>
ref<qt::qobject> qt::QmlContext(const std::pair<std::string, Refs>&... defs)
{
  const auto read_only_props_indices = internal::make_filtered_index_sequence<(
    core::is_ref<Refs>::value && !core::is_var<Refs>::value &&
    !core::is_sig<Refs>::value)...>{};

  const auto read_write_props_indices =
    internal::make_filtered_index_sequence<core::is_var<Refs>::value...>{};

  const auto signals_indices =
    internal::make_filtered_index_sequence<core::is_sig<Refs>::value...>{};

  return internal::make_qml_context(std::make_tuple(defs...),
                                    read_only_props_indices,
                                    read_write_props_indices,
                                    signals_indices);
}

namespace qt
{
namespace internal
{
DATAFLOW_QT_EXPORT ref<qobject>
create_qml_data_list(const ref<listC<qml_data>>& xs);
}
}

template <typename T> ref<qt::qobject> qt::QmlData(const ref<listC<T>>& xs)
{
  const auto ys = Map(xs, [](const T& x) { return qml_data{x}; });

  return internal::create_qml_data_list(ys);
}
} // dataflow
