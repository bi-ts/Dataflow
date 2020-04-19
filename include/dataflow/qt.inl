
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
struct add_rw_property
{
  template <typename T>
  void operator()(const std::string& name, var<T>& x) const
  {
    // TODO: avoid this shared var
    const auto p_x = std::make_shared<var<T>>(x);

    builder.add_property(name,
                         internal::convert_to_qml_type(T{}),
                         [p_x](const QVariant& value) mutable {
                           *p_x = internal::convert_from_qml_type<T>(value);
                         });
  }

  internal::context_builder& builder;
};

struct add_property
{
  template <std::size_t idx, typename T>
  void operator()(const std::string& name) const
  {
    builder.add_property(name, internal::convert_to_qml_type(T{}));
  }

  internal::context_builder& builder;
};

struct set_property
{
  template <typename T>
  void operator()(const std::string& name, var<T>& x) const
  {
    p_object->setProperty(name.c_str(), internal::convert_to_qml_type(*x));
  }

  std::shared_ptr<QObject> p_object;
};

template <typename... Refs> class rw_props_definition;

template <typename T, typename... Refs>
class rw_props_definition<std::reference_wrapper<var<T>>, Refs...>
: public rw_props_definition<Refs...>
{
public:
  rw_props_definition(
    std::pair<std::string, std::reference_wrapper<var<T>>> def,
    std::pair<std::string, Refs>... defs)
  : rw_props_definition<Refs...>(defs...)
  , name_(std::move(def.first))
  , x_(std::move(def.second))
  {
  }

  template <typename F> void for_each(const F& f)
  {
    f(name_, x_);

    rw_props_definition<Refs...>::for_each(f);
  }

private:
  std::string name_;
  var<T> x_;
};

template <typename T, typename... Refs>
class rw_props_definition<ref<T>, Refs...> : public rw_props_definition<Refs...>
{
public:
  rw_props_definition(std::pair<std::string, ref<T>> def,
                      std::pair<std::string, Refs>... defs)
  : rw_props_definition<Refs...>(defs...)
  {
  }

  template <typename F> void for_each(const F& f)
  {
    rw_props_definition<Refs...>::for_each(f);
  }
};

template <> class rw_props_definition<>
{
public:
  template <typename F> void for_each(const F& f)
  {
  }
};

template <typename... Refs> class props_definition;

template <typename T, typename... Refs>
class props_definition<std::reference_wrapper<var<T>>, Refs...>
: private props_definition<Refs...>
{
public:
  props_definition(std::pair<std::string, std::reference_wrapper<var<T>>> def,
                   std::pair<std::string, Refs>... defs)
  : props_definition<Refs...>(defs...)
  {
  }

  template <std::size_t Idx = 0, typename F> void for_each(const F& f) const
  {
    props_definition<Refs...>::template for_each<Idx>(f);
  }

  template <typename Ret, typename... Xs, typename F>
  Ret apply(F&& f, const Xs&... xs)
  {
    return props_definition<Refs...>::template apply<Ret>(std::forward<F>(f),
                                                          xs...);
  }
};

template <typename T, typename... Refs>
class props_definition<ref<T>, Refs...> : private props_definition<Refs...>
{
public:
  props_definition(std::pair<std::string, ref<T>> def,
                   std::pair<std::string, Refs>... defs)
  : props_definition<Refs...>(defs...)
  , name_(std::move(def.first))
  , x_(std::move(def.second))
  {
  }

  template <std::size_t Idx = 0, typename F> void for_each(const F& f) const
  {
    f.template operator()<Idx, T>(name_);

    props_definition<Refs...>::template for_each<Idx + 1>(f);
  }

  template <typename Ret, typename... Xs, typename F>
  Ret apply(F&& f, const Xs&... xs)
  {
    return props_definition<Refs...>::template apply<Ret>(
      std::forward<F>(f), xs..., x_);
  }

private:
  std::string name_;
  ref<T> x_;
};

template <> class props_definition<>
{
public:
  template <std::size_t Idx = 0, typename F> void for_each(const F& f) const
  {
  }

  template <typename Ret, typename... Xs, typename F>
  Ret apply(F&& f, const Xs&... xs)
  {
    return f(xs...);
  }
};

template <typename... Ts> struct set_property_from_value
{
  template <std::size_t Idx, typename>
  void operator()(const std::string& name) const
  {
    p_object->setProperty(
      name.c_str(), internal::convert_to_qml_type(std::get<Idx>(values_)));
  }

  std::tuple<Ts...> values_;
  std::shared_ptr<QObject> p_object;
};

template <typename... Ts, typename... Us, std::size_t... Is>
void set_properties(const std::shared_ptr<QObject>& p_context,
                    const props_definition<Us...>& props,
                    const std14::index_sequence<Is...>&,
                    const Ts&... values)
{
  props.for_each(
    set_property_from_value<Ts...>{std::make_tuple(values...), p_context});
}

template <typename... Refs> class qml_context_creator
{
public:
  qml_context_creator(rw_props_definition<Refs...> rw_props,
                      props_definition<Refs...> props)
  : rw_props_(std::move(rw_props))
  , props_(std::move(props))
  {
  }

  template <typename... Rs>
  ref<std::shared_ptr<QObject>> operator()(const Rs&... xs)
  {
    class policy
    {
    public:
      policy(rw_props_definition<Refs...> rw_props,
             const props_definition<Refs...>& props)
      : rw_props_(std::move(rw_props))
      , props_(props)
      {
      }

      std::string label() const
      {
        return "qml-context";
      }

      std::shared_ptr<QObject> calculate(const core::data_type_t<Rs>&... values)
      {
        internal::context_builder builder;

        rw_props_.for_each(add_rw_property{builder});
        props_.for_each(add_property{builder});

        const auto p_context = builder.build();

        p_context->blockSignals(true);

        rw_props_.for_each(set_property{p_context});

        set_properties(p_context,
                       props_,
                       std14::make_index_sequence<sizeof...(Rs)>(),
                       values...);

        p_context->blockSignals(false);

        return p_context;
      };

      std::shared_ptr<QObject> update(const std::shared_ptr<QObject>& p_context,
                                      const core::data_type_t<Rs>&... values)
      {
        set_properties(p_context,
                       props_,
                       std14::make_index_sequence<sizeof...(Rs)>(),
                       values...);

        return p_context;
      }

    private:
      rw_props_definition<Refs...> rw_props_;
      props_definition<Refs...> props_;
    };

    return core::LiftUpdater(policy{rw_props_, props_}, xs...);
  }

private:
  rw_props_definition<Refs...> rw_props_;
  props_definition<Refs...> props_;
};
}
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

template <typename... Refs>
ref<std::shared_ptr<QObject>>
qt::QmlContext(const std::pair<std::string, Refs>&... defs)
{
  detail::props_definition<Refs...> props{defs...};

  detail::qml_context_creator<Refs...> builder{{defs...}, props};

  return props.template apply<ref<std::shared_ptr<QObject>>>(builder);
}
} // dataflow
