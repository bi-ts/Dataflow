
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

#if !defined(DATAFLOW_QT_H)
#error "'.inl' file can't be included directly. Use 'qt.h' instead"
#endif

#include "qt/conversion.h"
#include "qt/internal/qobject_builder.h"

#include <dataflow/utility/std_future.h>

namespace dataflow2qt
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
  using type = dataflow::std14::index_sequence<Vs...>;
};
}

template <std::size_t... Ps>
using make_filtered_index_sequence =
  typename detail::filtered_index_sequence_builder<sizeof...(Ps),
                                                   0,
                                                   sizeof...(Ps) == 0,
                                                   Ps...>::type;

template <std::size_t... Is, typename Tuple, typename F>
void for_each_tuple_element(const Tuple& t,
                            dataflow::std14::index_sequence<Is...> indices,
                            F f)
{
  (void)std::make_tuple([&f, &t]() {
    f(std::get<Is>(t));
    return 0;
  }()...);
}

template <typename Tuple, typename F>
void for_each_tuple_element(const Tuple& t, F f)
{
  for_each_tuple_element(
    t,
    dataflow::std14::make_index_sequence<std::tuple_size<Tuple>::value>{},
    std::move(f));
}

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

namespace dataflow
{
namespace core
{
template <> struct patch_type<dataflow2qt::qobject>
{
  using type = dataflow2qt::internal::qobject_patch;
};
}
}

namespace dataflow2qt
{
namespace internal
{
template <typename... Refs,
          std::size_t... ReadOnlyIndices,
          std::size_t... ReadWriteIndices,
          std::size_t... SignalsIndices>
dataflow::ref<qobject>
make_qml_context(const std::tuple<std::pair<std::string, Refs>...>& defs,
                 const dataflow::std14::index_sequence<ReadOnlyIndices...>&,
                 const dataflow::std14::index_sequence<ReadWriteIndices...>&,
                 const dataflow::std14::index_sequence<SignalsIndices...>&)
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
      const dataflow::core::data_type_t<
        typename std::tuple_element<ReadOnlyIndices,
                                    std::tuple<Refs...>>::type>&... values)
    {
      internal::qobject_builder builder;

      for_each_tuple_element(read_only_props_, [&builder](const auto& def) {
        builder.add_property(def.first, [x = def.second]() {
          using type = dataflow::core::data_type_t<decltype(x)>;
          return cast_to_qml_type(x.template value<type>());
        });
      });

      for_each_tuple_element(read_write_props_, [&builder](const auto& def) {
        using type = dataflow::core::data_type_t<decltype(def.second)>;
        using qml_type = decltype(cast_to_qml_type(*def.second));
        builder.add_property(
          def.first,
          [x = def.second]() { return cast_to_qml_type(*x); },
          [x = def.second](const qml_type& qml_v) {
            const auto& v = cast_from_qml_type<type>(qml_v);
            if (*x != v)
            {
              x = v;
              return true;
            }
            return false;
          });
      });

      for_each_tuple_element(signal_definitions_, [&builder](const auto& def) {
        builder.add_slot(def.first, [x = def.second]() { return x(); });
      });

      return builder.build();
    };

    qobject_patch prepare_patch(
      const dataflow::core::diff_type_t<dataflow::core::data_type_t<
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

  return dataflow::core::LiftPatcher(
    policy{read_only_props, read_write_props, signal_definitions},
    std::get<ReadOnlyIndices>(defs).second...);
}

inline dataflow::ref<qobject>
make_qml_context(const std::tuple<>&,
                 const dataflow::std14::index_sequence<>&,
                 const dataflow::std14::index_sequence<>&,
                 const dataflow::std14::index_sequence<>&)
{
  internal::qobject_builder builder;
  return dataflow::Const(builder.build());
}
}
}

template <typename T>
std::pair<std::string, dataflow::var<T>>
dataflow2qt::QmlPropertyRW(const std::string& name, dataflow::var<T>& x)
{
  return std::make_pair(name, x);
}

inline std::pair<std::string, dataflow::sig>
dataflow2qt::QmlFunction(const std::string& name, const dataflow::sig& x)
{
  return std::make_pair(name, x);
}

template <typename T, typename..., typename>
std::pair<std::string, dataflow::ref<T>>
dataflow2qt::QmlProperty(const std::string& name, const dataflow::ref<T>& x)
{
  return std::make_pair(name, x);
}

template <typename T>
std::pair<std::string, dataflow::ref<dataflow2qt::qobject>>
dataflow2qt::QmlProperty(const std::string& name,
                         const dataflow::ref<dataflow::listC<T>>& xs)
{
  return QmlProperty(name,
                     Map(xs, [](const T& x) { return cast_to_qvariant(x); }));
}

template <typename... Refs>
dataflow::ref<dataflow2qt::qobject>
dataflow2qt::QmlContext(const std::pair<std::string, Refs>&... defs)
{
  const auto read_only_props_indices = internal::make_filtered_index_sequence<(
    dataflow::core::is_ref<Refs>::value &&
    !dataflow::core::is_var<Refs>::value &&
    !dataflow::core::is_sig<Refs>::value)...>{};

  const auto read_write_props_indices = internal::make_filtered_index_sequence<
    dataflow::core::is_var<Refs>::value...>{};

  const auto signals_indices = internal::make_filtered_index_sequence<
    dataflow::core::is_sig<Refs>::value...>{};

  return internal::make_qml_context(std::make_tuple(defs...),
                                    read_only_props_indices,
                                    read_write_props_indices,
                                    signals_indices);
}
