
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

#if !defined(DATAFLOW___INTROSPECT_H)
#error '.inl' file can't be included directly. Use 'introspect.h' instead
#endif

#include <cassert>
#include <iostream>

namespace dataflow
{
namespace introspect
{
namespace detail
{
template <typename T>
class end_iterator_impl final : public detail::iterator_impl<T>
{
  virtual void increment() override
  {
    throw std::logic_error("Iterator is not incrementable");
  }

  virtual std::unique_ptr<detail::iterator_impl<T>> clone() override
  {
    return std::unique_ptr<detail::iterator_impl<T>>(new end_iterator_impl());
  }

  virtual T& value() override
  {
    throw std::logic_error("Iterator is not dereferencable");
  }

  virtual bool equal(const detail::iterator_impl<T>& other) override
  {
    return nullptr != dynamic_cast<const end_iterator_impl*>(&other);
  }
};
} // detail

template <typename T>
graph_iterator<T>::graph_iterator()
: p_impl_(new detail::end_iterator_impl<T>())
{
  assert(p_impl_);
}

template <typename T>
graph_iterator<T>::graph_iterator(
  std::unique_ptr<detail::iterator_impl<T>>&& p_delegate)
: p_impl_(std::move(p_delegate))
{
  assert(p_impl_);
}

template <typename T>
graph_iterator<T>::graph_iterator(const graph_iterator& other)
: p_impl_(other.p_impl_->clone())
{
  assert(p_impl_);
}

template <typename T>
graph_iterator<T>& graph_iterator<T>::operator=(const graph_iterator& other)
{
  assert(p_impl_);
  assert(other.p_impl_);

  p_impl_ = other.p_impl_->clone();

  return *this;
}

template <typename T>
bool graph_iterator<T>::operator==(const graph_iterator& other) const
{
  assert(p_impl_);
  assert(other.p_impl_);

  return p_impl_->equal(*other.p_impl_);
}

template <typename T>
bool graph_iterator<T>::operator!=(const graph_iterator& other) const
{
  return !(*this == other);
}

template <typename T> graph_iterator<T>& graph_iterator<T>::operator++()
{
  assert(p_impl_);

  p_impl_->increment();

  return *this;
}

template <typename T> graph_iterator<T> graph_iterator<T>::operator++(int)
{
  const auto prev = *this;
  ++(*this);
  return prev;
}

template <typename T>
typename graph_iterator<T>::reference graph_iterator<T>::operator*() const
{
  assert(p_impl_);

  return p_impl_->value();
}

inline dependency_graph::vertex_descriptor::vertex_descriptor()
: id_()
{
}

inline dependency_graph::vertex_descriptor::vertex_descriptor(
  const internal::ref& x)
: id_(x.id())
{
}

inline dependency_graph::vertex_descriptor::vertex_descriptor(
  internal::node_id id)
: id_(id)
{
}

inline bool dependency_graph::vertex_descriptor::
operator==(vertex_descriptor other) const
{
  return id_ == other.id_;
}

inline bool dependency_graph::vertex_descriptor::
operator!=(vertex_descriptor other) const
{
  return !(*this == other);
}

inline bool dependency_graph::vertex_descriptor::
operator<(const vertex_descriptor& other) const
{
  return id_ < other.id_;
}
} // introspect

inline bool introspect::dependency_graph::edge_descriptor::
operator==(const dependency_graph::edge_descriptor& other) const
{
  return u == other.u && idx == other.idx;
}

inline bool introspect::dependency_graph::edge_descriptor::
operator!=(const dependency_graph::edge_descriptor& other) const
{
  return !(*this == other);
}

inline bool introspect::activator_node(dependency_graph::vertex_descriptor v)
{
  return label(v) == "activator";
}

inline bool introspect::data_dependency(dependency_graph::edge_descriptor e)
{
  return !logical_dependency(e);
}

template <typename T>
ref<T> introspect::Log(const ref<T>& x, const std::string& l)
{
  struct policy
  {
    static std::string label()
    {
      return "introspect-log";
    }
    T calculate(const T& v)
    {
      std::clog << prefix_ << " = " << value(x_) << std::endl;
      return v;
    }

    const dependency_graph::vertex_descriptor x_;
    const std::string prefix_;
  };
  return core::Lift(policy{x, (l != "" ? l : label(x))}, x);
}
} // dataflow
