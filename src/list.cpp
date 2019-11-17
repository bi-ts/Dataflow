
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

#include <dataflow/list.h>

#include <immer/vector.hpp>
#include <immer/vector_transient.hpp>

namespace dataflow
{
namespace list_internal
{
class ref_wrapper
{
public:
  ref_wrapper(const core::ref_base& x)
  : x_(x)
  {
  }

  bool operator==(const ref_wrapper& other) const
  {
    return x_.id() == other.x_.id();
  }

  bool operator!=(const ref_wrapper& other) const
  {
    return !(*this == other);
  }

  core::ref_base x_;
};

class list_data_impl
{
private:
  using immer_policy = immer::memory_policy<immer::heap_policy<immer::cpp_heap>,
                                            immer::default_refcount_policy>;
  using data_container = immer::vector<ref_wrapper, immer_policy>;

public:
  explicit list_data_impl(const std::initializer_list<core::ref_base>& init)
  : data_([&]() {
    data_container::transient_type tmp;

    for (const auto x : init)
      tmp.push_back(x);

    return tmp.persistent();
  }())
  , ref_counter_()
  {
  }

  void retain()
  {
    ++ref_counter_;
  }

  void release()
  {
    if (--ref_counter_ == 0)
      delete this;
  }

  data_container data_;

private:
  std::size_t ref_counter_;
};

listA_data::listA_data()
: p_impl_(new list_data_impl({}))
{
  p_impl_->retain();
}

listA_data::listA_data(const std::initializer_list<core::ref_base>& init)
: p_impl_(new list_data_impl(init))
{
  DATAFLOW___CHECK_PRECONDITION(p_impl_ != nullptr);

  p_impl_->retain();
}

listA_data::listA_data(const listA_data& other)
: p_impl_(other.p_impl_)
{
  DATAFLOW___CHECK_PRECONDITION(p_impl_ != nullptr);

  p_impl_->retain();
}

listA_data::~listA_data()
{
  DATAFLOW___CHECK_PRECONDITION(p_impl_ != nullptr);

  p_impl_->release();
}

listA_data& listA_data::operator=(const listA_data& other)
{
  DATAFLOW___CHECK_PRECONDITION(p_impl_ != nullptr);
  DATAFLOW___CHECK_PRECONDITION(other.p_impl_ != nullptr);

  p_impl_->release();

  p_impl_ = other.p_impl_;

  p_impl_->retain();

  return *this;
}

bool listA_data::operator==(const listA_data& other) const
{
  DATAFLOW___CHECK_PRECONDITION(p_impl_ != nullptr);
  DATAFLOW___CHECK_PRECONDITION(other.p_impl_ != nullptr);

  return p_impl_->data_ == other.p_impl_->data_;
}

bool listA_data::operator!=(const listA_data& other) const
{
  return !(*this == other);
}

std::size_t listA_data::size() const
{
  DATAFLOW___CHECK_PRECONDITION(p_impl_ != nullptr);

  return p_impl_->data_.size();
}
}
}
