
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

#if !defined(DATAFLOW___RANDOM_H)
#error "'.inl' file can't be included directly. Use 'random.h' instead"
#endif

namespace dataflow
{
template <typename T,
          typename...,
          typename ArgA,
          typename ArgB,
          typename ArgV,
          typename,
          typename,
          typename,
          typename>
ref<T> random::FromRandomNumber(const ArgA& a, const ArgB& b, const ArgV& v)
{
  return Cast<T>(If(a <= b, a, b) + v % (1 + Abs(b - a)));
}
} // namespace dataflow
