
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
#include "qt/internal/conversion.h"

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

struct add_rw_property
{
  template <typename T>
  void operator()(
    const std::pair<std::string, std::reference_wrapper<var<T>>>& def) const
  {
    const auto p_x = std::make_shared<var<T>>(def.second.get());

    builder.add_property(
      def.first,
      internal::convert_to_qml_type(
        detail::qml_prop_definition_data_type_t<decltype(def.second)>()),
      [p_x](const QVariant& value) mutable {
        *p_x = internal::convert_from_qml_type<T>(value);
      });
  }

  internal::context_builder& builder;
};

struct add_property
{
  template <typename T> void operator()(const T& def) const
  {
    builder.add_property(
      def.first,
      internal::convert_to_qml_type(
        detail::qml_prop_definition_data_type_t<decltype(def.second)>()));
  }

  internal::context_builder& builder;
};

struct set_property
{
  template <typename T> void operator()(const T& def) const
  {
    p_object->setProperty(def.first.c_str(),
                          internal::convert_to_qml_type(*(def.second.get())));
  }

  std::shared_ptr<QObject> p_object;
};

template <typename... Ts, std::size_t... Is>
std::shared_ptr<QObject>
set_properties(const std::shared_ptr<QObject>& p_context,
               const std::tuple<std::pair<std::string, ref<Ts>>...>& props,
               const std14::index_sequence<Is...>&,
               const Ts&... values)
{
  const auto values_tuple = std::make_tuple(values...);

  const std::vector<std::pair<std::string, QVariant>> name_value_pairs = {
    std::make_pair(
      std::get<Is>(props).first,
      internal::convert_to_qml_type(std::get<Is>(values_tuple)))...};

  for (const auto& name_value : name_value_pairs)
  {
    p_context->setProperty(name_value.first.c_str(), name_value.second);
  }

  return p_context;
}
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
  using rw_props_definition_type =
    std::tuple<std::pair<std::string, std::reference_wrapper<var<Ts>>>...>;
  using props_definition_type = std::tuple<std::pair<std::string, ref<Ts>>...>;

  class policy
  {
  public:
    policy(const rw_props_definition_type& rw_props,
           const props_definition_type& props)
    : rw_props_(rw_props)
    , props_(props)
    {
    }

    std::string label() const
    {
      return "create-context";
    }

    std::shared_ptr<QObject> calculate(const Us&... values)
    {
      internal::context_builder builder;

      detail::for_each_tuple_element(rw_props_,
                                     detail::add_rw_property{builder});

      detail::for_each_tuple_element(props_, detail::add_property{builder});

      const auto p_context = builder.build();

      p_context->blockSignals(true);

      detail::for_each_tuple_element(rw_props_,
                                     detail::set_property{p_context});

      detail::set_properties(p_context,
                             props_,
                             std14::make_index_sequence<sizeof...(Us)>(),
                             values...);

      p_context->blockSignals(false);

      return p_context;
    };

    std::shared_ptr<QObject> update(const std::shared_ptr<QObject>& p_context,
                                    const Us&... values)
    {
      p_context->blockSignals(true);

      detail::set_properties(p_context,
                             props_,
                             std14::make_index_sequence<sizeof...(Us)>(),
                             values...);

      p_context->blockSignals(false);

      return p_context;
    }

  private:
    rw_props_definition_type rw_props_;
    props_definition_type props_;
  };

  return core::LiftUpdater(policy(rw_props, std::make_tuple(props...)),
                           props.second...);
}
} // dataflow
