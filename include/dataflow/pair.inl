
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

#if !defined(DATAFLOW___PAIR_H)
#error '.inl' file can't be included directly. Use 'pair.h' instead
#endif

namespace dataflow
{

template <typename T, typename U>
pair<T, U>::pair(const ref<T>& first, const ref<U>& second)
: p_impl_(std::make_shared<impl>(std::make_pair(first, second)))
{
}

template <typename T, typename U>
bool pair<T, U>::operator==(const pair& other) const
{
  return p_impl_ == other.p_impl_;
}

template <typename T, typename U>
bool pair<T, U>::operator!=(const pair& other) const
{
  return !(*this == other);
}

} // dataflow

template <typename T, typename U>
std::ostream& dataflow::operator<<(std::ostream& out, const pair<T, U>& value)
{
  out << "pair";
  return out;
}

template <typename T, typename U>
dataflow::pair<T, U> dataflow::make_pair(const ref<T>& first,
                                         const ref<U>& second)
{
  return pair<T, U>(first, second);
}

template <typename T, typename U>
dataflow::ref<dataflow::pair<T, U>> dataflow::Pair(const ref<T>& first,
                                                   const ref<U>& second)
{
  return Const(make_pair(first, second));
}

template <typename T, typename U, typename FwT>
dataflow::ref<dataflow::pair<FwT, U>> dataflow::Pair(const T& first,
                                                     const ref<U>& second)
{
  return Pair(Const(first), second);
}

template <typename T, typename U, typename FwU>
dataflow::ref<dataflow::pair<T, FwU>> dataflow::Pair(const ref<T>& first,
                                                     const U& second)
{
  return Pair(first, Const(second));
}

template <typename T, typename U, typename FwT, typename FwU>
dataflow::ref<dataflow::pair<FwT, FwU>> dataflow::Pair(const T& first,
                                                       const U& second)
{
  return Pair(Const(first), Const(second));
}
