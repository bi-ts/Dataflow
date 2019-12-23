
//  Copyright (c) 2014 - 2019 Maksym V. Bilinets.
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

template <typename Patch> struct patch_metadata : metadata
{
  patch_metadata(const Patch& other)
  : patch(other)
  {
  }

  Patch patch;
};

template <typename Policy, typename OutPatch, typename... InDiffs>
class node_patcher_n_ary final : public node_t<typename OutPatch::data_type>,
                                 public Policy
{
  friend class nodes_factory;

public:
  static ref create(const Policy& policy,
                    bool eager,
                    ref_t<typename InDiffs::data_type>... xs)
  {
    DATAFLOW___CHECK_PRECONDITION(
      check_all_of(xs.template is_of_type<typename InDiffs::data_type>()...));

    const std::array<node_id, sizeof...(InDiffs)> args = {{xs.id()...}};

    return nodes_factory::create<
      node_patcher_n_ary<Policy, OutPatch, InDiffs...>>(
      &args[0],
      args.size(),
      eager ? node_flags::eager : node_flags::none,
      policy);
  }

private:
  explicit node_patcher_n_ary(const Policy& policy)
  : Policy(policy)
  {
  }

  template <typename... Args, std::size_t... Is>
  typename OutPatch::data_type initialize_(const node** p_args,
                                           const std14::index_sequence<Is...>&)
  {
    prev_args_ = std::tuple<typename InDiffs::data_type...>{
      extract_node_value<Args>(p_args[Is])...};

    return Policy::calculate(extract_node_value<Args>(p_args[Is])...);
  }

  template <typename Diff, std::size_t Idx>
  Diff get_argument_diff_(const node* p_node)
  {
    const auto& p_metadata = node::get_metadata(p_node).get();

    const auto& curr = extract_node_value<typename Diff::data_type>(p_node);
    const auto& prev = std::get<Idx>(prev_args_);

    if (!p_metadata)
      return Diff(curr, prev);

    DATAFLOW___CHECK_CONDITION(
      dynamic_cast<const patch_metadata<typename Diff::patch_type>*>(
        p_metadata));

    const auto p_patch_metadata =
      static_cast<const patch_metadata<typename Diff::patch_type>*>(p_metadata);

    return Diff(curr, prev, p_patch_metadata->patch);
  }

  template <std::size_t... Is>
  update_status update_(bool initialized,
                        const node** p_args,
                        const std14::index_sequence<Is...>&)
  {
    if (initialized)
    {
      const auto patch =
        Policy::prepare_patch(get_argument_diff_<InDiffs, Is>(p_args[Is])...);

      prev_args_ = std::tuple<typename InDiffs::data_type...>{
        extract_node_value<typename InDiffs::data_type>(p_args[Is])...};

      node::set_metadata(this,
                         std::unique_ptr<patch_metadata<OutPatch>>{
                           new patch_metadata<OutPatch>{patch}});

      const auto v = patch.apply(this->value());

      return this->set_value_(v);
    }

    return this->set_value_(initialize_<typename InDiffs::data_type...>(
      p_args, std14::make_index_sequence<sizeof...(InDiffs)>()));
  }

  virtual update_status update_(node_id id,
                                bool initialized,
                                const node** p_args,
                                std::size_t args_count) override
  {
    DATAFLOW___CHECK_PRECONDITION(p_args != nullptr);
    DATAFLOW___CHECK_PRECONDITION(args_count == sizeof...(InDiffs));

    return update_(
      initialized, p_args, std14::make_index_sequence<sizeof...(InDiffs)>());
  }

  virtual std::string label_() const override
  {
    return Policy::label();
  }

  virtual std::pair<std::size_t, std::size_t> mem_info_() const override final
  {
    return std::make_pair(sizeof(*this), alignof(decltype(*this)));
  }

  std::tuple<typename InDiffs::data_type...> prev_args_;
};
} // internal
} // dataflow
