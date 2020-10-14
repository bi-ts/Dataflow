
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

#pragma once

#include "config.h"

#include <dataflow/prelude/core/dtime.h>
#include <dataflow/prelude/core/internal/node_t.h>

namespace dataflow
{
namespace internal
{
class node_time final : public node_t<dtimestamp>
{
public:
  explicit node_time()
  : node_t<dtimestamp>(dtimestamp{std::numeric_limits<std::size_t>::max()})
  , next_value_(std::numeric_limits<std::size_t>::max())
  {
  }

  void increment()
  {
    ++next_value_;
  }

private:
  virtual update_status update_(node_id id,
                                bool initialized,
                                const node** p_args,
                                std::size_t args_count) override
  {
    return this->set_value_(dtimestamp{next_value_});
  }

  virtual std::string label_() const override
  {
    return "time";
  }

  virtual std::pair<std::size_t, std::size_t> mem_info_() const override final
  {
    return std::make_pair(sizeof(*this), alignof(decltype(*this)));
  }

private:
  std::size_t next_value_;
};
} // internal
} // dataflow
