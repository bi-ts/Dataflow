
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

#if !defined(DATAFLOW___LIST_H)
#error '.inl' file can't be included directly. Use 'list.h' instead
#endif

namespace dataflow
{
template <typename T>
template <typename U, typename... Us>
list<T>::list(const U& x, const Us&... xs)
: p_data_(std::make_shared<data>(
    data{{core::make_argument(x), core::make_argument(xs)...}}))
{
}

template <typename T> bool list<T>::operator==(const list& other) const
{
  return p_data_ == other.p_data_;
}

template <typename T> bool list<T>::operator!=(const list& other) const
{
  return !(*this == other);
}

} // dataflow

template <typename T>
std::ostream& dataflow::operator<<(std::ostream& out, const list<T>& value)
{
  out << "list";
  return out;
}

template <typename U, typename... Us, typename T>
dataflow::list<T> dataflow::make_list(const U& x, const Us&... xs)
{
  return list<T>(x, xs...);
}

template <typename U, typename... Us, typename T>
dataflow::ref<dataflow::list<T>> dataflow::List(const U& x, const Us&... xs)
{
  return Const(make_list(x, xs...));
}
