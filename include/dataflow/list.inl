
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
: data_({static_cast<core::ref_base>(core::make_argument(x)),
         static_cast<core::ref_base>(core::make_argument(xs))...})
{
}

template <typename T> bool list<T>::operator==(const list& other) const
{
  return data_ == other.data_;
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

template <typename Arg, typename... Args, typename T>
dataflow::listA<T> dataflow::make_listA(const Arg& x, const Args&... xs)
{
  return listA<T>(x, xs...);
}

template <typename Arg, typename... Args, typename T>
dataflow::ref<dataflow::listA<T>> dataflow::ListA(const Arg& x,
                                                  const Args&... xs)
{
  return Const(make_listA(x, xs...));
}
