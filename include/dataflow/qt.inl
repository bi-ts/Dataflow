
//  Copyright (c) 2014 - 2020 Maksym V. Bilinets.
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

#include "qt/internal/context_builder.h"

namespace dataflow
{
namespace qt
{
namespace detail
{
template <typename T> struct qml_prop_definition_data_type : core::data_type<T>
{
};

template <typename T>
struct qml_prop_definition_data_type<std::reference_wrapper<T>>
: core::data_type<T>
{
};

template <typename T>
using qml_prop_definition_data_type_t =
  typename qml_prop_definition_data_type<T>::type;

int to_qml_basic(int v)
{
  return v;
}

int to_qml_basic(float v)
{
  return v;
}

QString to_qml_basic(std::string v)
{
  return QString::fromUtf8(v.c_str());
}

template <std::size_t I = 0, typename F, typename... Ts>
typename std::enable_if<I == sizeof...(Ts)>::type
for_each_tuple_element(const std::tuple<Ts...>&, const F&)
{
}

template <std::size_t I = 0, typename F, typename... Ts>
typename std::enable_if<!(I >= sizeof...(Ts))>::type
for_each_tuple_element(const std::tuple<Ts...>& t, const F& f)
{
  f(std::get<I>(t));

  for_each_tuple_element<I + 1>(t, f);
}

struct add_property
{
  template <typename T> void operator()(const T& def) const
  {
    builder.add(
      def.first,
      QVariant::fromValue(detail::to_qml_basic(
        detail::qml_prop_definition_data_type_t<decltype(def.second)>())));
  }

  internal::context_builder& builder;
};
}
}

template <typename... Ts>
std::tuple<std::pair<std::string, std::reference_wrapper<var<Ts>>>...>
qt::RW(const std::pair<std::string, std::reference_wrapper<var<Ts>>>&... props)
{
  return std::make_tuple(props...);
}

template <typename T>
std::pair<std::string, std::reference_wrapper<var<T>>>
qt::QmlPropertyRW(const std::string& name, var<T>& x)
{
  return std::make_pair(name, std::ref(x));
}

template <typename T>
std::pair<std::string, ref<T>> qt::QmlProperty(const std::string& name,
                                               const ref<T>& x)
{
  return std::make_pair(name, x);
}

template <typename... Ts, typename... Us>
ref<std::shared_ptr<QObject>> qt::QmlContext(
  const std::tuple<std::pair<std::string, std::reference_wrapper<var<Ts>>>...>&
    rw_props,
  const std::pair<std::string, ref<Us>>&... props)
{
  using all_defs_type =
    std::tuple<std::pair<std::string, std::reference_wrapper<var<Ts>>>...,
               std::pair<std::string, ref<Ts>>...>;

  class policy
  {
  public:
    policy(const all_defs_type& defs)
    : defs_(defs)
    {
    }

    std::string label() const
    {
      return "create-context";
    }

    std::shared_ptr<QObject> calculate(const Us&...)
    {
      internal::context_builder builder;

      detail::for_each_tuple_element(defs_, detail::add_property{builder});

      return builder.build();
    };

    std::shared_ptr<QObject> update(const std::shared_ptr<QObject>& p_context,
                                    const Us&...)
    {
      return p_context;
    }

  private:
    all_defs_type defs_;
  };

  return core::LiftUpdater(
    policy(std::tuple_cat(rw_props, std::make_tuple(props...))),
    props.second...);
}
} // dataflow
