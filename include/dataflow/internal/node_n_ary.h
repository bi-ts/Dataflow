
//  Copyright (c) 2014 - 2017 Maksym V. Bilinets.
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
#include "node_t.h"
#include "nodes_factory.h"
#include "ref.h"
#include "std_future.h"

#include <array>
#include <utility>

namespace dataflow
{
namespace internal
{

template <typename Policy, typename T, typename... Xs>
class node_n_ary final : public node_t<T>, public Policy
{
  friend class nodes_factory;

private:
  template <typename> using ref_t = ref;

  struct helper
  {
    static bool check_all(bool b)
    {
      return b;
    }

    template <typename B1, typename B2, typename... Bs>
    static bool check_all(B1 b1, B2 b2, Bs... bs)
    {
      return b1 && check_all(b2, bs...);
    }
  };

public:
  static ref create(const Policy& policy, ref_t<Xs>... xs)
  {
    DATAFLOW___CHECK_PRECONDITION(
      helper::check_all(xs.template is_of_type<Xs>()...));

    const std::array<node_id, sizeof...(Xs)> args = {{xs.id()...}};

    return nodes_factory::create<node_n_ary<Policy, T, Xs...>>(
      &args[0], args.size(), false, policy);
  }

private:
  explicit node_n_ary(const Policy& policy)
  : Policy(policy)
  {
  }

  template <typename... Args, std::size_t... Is>
  T calculate_(const node** p_args, const std14::index_sequence<Is...>&)
  {
    return Policy::calculate(extract_node_value<Args>(p_args[Is])...);
  }

  virtual bool update_(node_id id,
                       bool initialized,
                       const node** p_args,
                       std::size_t args_count) override
  {
    DATAFLOW___CHECK_PRECONDITION(p_args != nullptr);
    DATAFLOW___CHECK_PRECONDITION(args_count == sizeof...(Xs));

    return this->set_value_(
      calculate_<Xs...>(p_args, std14::make_index_sequence<sizeof...(Xs)>()));

    return false;
  }

  virtual std::string label_() const override
  {
    return Policy::label();
  }

  virtual std::pair<std::size_t, std::size_t> mem_info_() const override final
  {
    return std::make_pair(sizeof(*this), alignof(decltype(*this)));
  }
};
} // internal
} // dataflow
