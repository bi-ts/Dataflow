
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

#include <dataflow/prelude/core.h>

#include "core/internal/engine.h"

#include <dataflow/prelude/core/internal/node_signal.h>

namespace dataflow
{
bool unit::operator==(const unit&) const
{
  return true;
}

bool unit::operator!=(const unit&) const
{
  return false;
}
}

std::ostream& dataflow::operator<<(std::ostream& out, const unit&)
{
  return (out << "()");
}

namespace dataflow
{
Engine::Engine(engine_options options)
{
  internal::engine::start(this, options);
}

Engine::~Engine()
{
  internal::engine::stop();
}

Engine* Engine::engine_()
{
  return static_cast<Engine*>(internal::engine::data());
}

ref<bool> Engine::timeout_(const ref<integer>& interval_msec, dtime t0)
{
  struct active_policy
  {
    static std::string label()
    {
      return "active";
    }
    bool calculate(const int& x)
    {
      return x == 1;
    }
  };

  struct final_policy
  {
    static std::string label()
    {
      return "final";
    }
    bool calculate(const int& x)
    {
      return x == 2;
    }
  };

  struct incr_policy
  {
    static std::string label()
    {
      return "incr";
    }
    int calculate(int x)
    {
      return ++x;
    }
  };

  const auto s = Recursion(
    0,
    [](const ref<int>& prev) {
      const auto final_state = core::Lift<final_policy>(prev);
      return If(final_state, prev, core::Lift<incr_policy>(prev));
    },
    t0);

  return core::Lift<active_policy>(s);
}

const ref<bool>& sig::as_ref() const
{
  return *this;
}

void sig::operator()() const
{
  this->schedule_();
}

sig::sig(const internal::ref& r, internal::ref::ctor_guard_t)
: ref<bool>(core::ref_base<bool>(r, internal::ref::ctor_guard))
{
}

} // dataflow

dataflow::ref<dataflow::dtimestamp> dataflow::CurrentTime()
{
  return core::ref_base<dtimestamp>(internal::nodes_factory::get_time(),
                                    internal::ref::ctor_guard);
}

dataflow::ref<dataflow::dtimestamp> dataflow::Timestamp(dtime t)
{
  return CurrentTime()(t);
}

dataflow::sig dataflow::Signal()
{
  return sig(internal::node_signal::create(), internal::ref::ctor_guard);
}

dataflow::ref<bool> dataflow::Timeout(const arg<integer>& interval_msec,
                                      dtime t0)
{
  DATAFLOW___CHECK_PRECONDITION(Engine::engine_() != nullptr);

  return Engine::engine_()->timeout_(interval_msec, t0);
}

dataflow::init_function<bool>
dataflow::Timeout(const arg<integer>& interval_msec)
{
  return [=](dtime t0) { return Timeout(interval_msec, t0); };
}
