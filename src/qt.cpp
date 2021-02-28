
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

#include "qt/internal/qobject_factory.h"
#include "qt/internal/qvariant_list_model.h"

#include <dataflow/qt.h>
#include <dataflow/tuple.h>

#include <QtCore/QTimer>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlContext>

namespace dataflow
{
EngineQml::EngineQml(const QCoreApplication& app)
: app_{app}
{
}

EngineQml::~EngineQml()
{
  QTimer::singleShot(0, &app_, QCoreApplication::quit);

  app_.exec();

  qt::internal::qobject_factory::on_shutdown();
}

EngineQml& EngineQml::instance()
{
  if (!Engine::engine_())
    throw std::logic_error("No Engine instance available");

  if (!dynamic_cast<EngineQml*>(Engine::engine_()))
    throw std::logic_error("The engine is not EngineQml");

  return *static_cast<EngineQml*>(Engine::engine_());
}

QQmlEngine& EngineQml::GetQmlEngine()
{
  return qml_engine_;
}

ref<bool> EngineQml::timeout_(const ref<integer>& interval_msec, dtime t0)
{
  const auto p_tick = std::make_shared<sig>(Signal());

  struct policy
  {
    std::string label() const
    {
      return "timeout";
    }

    unit calculate(const int& msec)
    {
      QTimer::singleShot(msec, [p_tick = std::weak_ptr<sig>(this->p_tick)]() {
        if (const auto ptr = p_tick.lock())
        {
          (*ptr)();
        }
      });

      return {};
    }

    const std::shared_ptr<sig> p_tick;
  };

  const auto timer = core::LiftPuller(policy{p_tick}, interval_msec(t0));

  return Second(TupleC(timer, *p_tick));
}

ref<qt::qobject> qt::QmlComponent(const arg<std::string>& qml_url,
                                  const arg<qobject>& context)
{
  using component_data = tupleC<qhandle<QQmlContext>, qobject>;

  class policy
  {
  public:
    std::string label() const
    {
      return "qml-component";
    }

    component_data calculate(const std::string& qml_url, const qobject& context)
    {
      QQmlComponent qml_component_factory{
        &EngineQml::instance().GetQmlEngine(),
        QUrl(QString::fromUtf8(qml_url.c_str()))};

      if (qml_component_factory.isError())
      {
        throw std::runtime_error(
          "Can't create qml component: " +
          qml_component_factory.errorString().toStdString());
      }

      const auto qml_context = internal::qobject_factory::create<QQmlContext>(
        EngineQml::instance().GetQmlEngine().rootContext());

      qml_context.get()->setContextProperty("view_context", context.get());

      const auto qml_component = internal::qobject_factory::make_shared(
        qml_component_factory.create(qml_context.get()));

      return component_data{qml_context, qml_component};
    };
  };

  return Second(core::Lift<policy>(qml_url, context));
}

namespace qt
{
namespace internal
{
class qvariant_list_model_patch
{
public:
  explicit qvariant_list_model_patch(const qhandle<qvariant_list_model>& curr,
                                     const qhandle<qvariant_list_model>& prev)
  : patch_{curr.get()->list_data(), prev.get()->list_data()}
  {
  }

  explicit qvariant_list_model_patch(const list_patch<qml_data>& patch)
  : patch_{patch}
  {
  }

  qhandle<qvariant_list_model>
  apply(const qhandle<qvariant_list_model>& model) const
  {
    patch_.apply([&](const integer& idx,
                     const qml_data& x) { model.get()->insert(idx, x); },
                 [&](const integer& idx) { model.get()->erase(idx); });

    return model;
  }

private:
  list_patch<qml_data> patch_;
};
}
}

namespace core
{
template <> struct patch_type<qt::qhandle<qt::internal::qvariant_list_model>>
{
  using type = qt::internal::qvariant_list_model_patch;
};
}

ref<qt::qobject>
qt::internal::create_qml_data_list(const ref<listC<qml_data>>& xs)
{
  struct policy
  {
    std::string label() const
    {
      return "qml-data-list";
    }

    static qhandle<qvariant_list_model> calculate(const listC<qml_data>& xs)
    {
      return qobject_factory::create<qvariant_list_model>(xs, nullptr);
    }

    qvariant_list_model_patch
    prepare_patch(const core::diff_type_t<listC<qml_data>>& d)
    {
      return qvariant_list_model_patch{d.patch()};
    }
  };

  return Cast<qobject>(core::LiftPatcher<policy>(xs));
}
}
