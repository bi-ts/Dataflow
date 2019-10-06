
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

#if !defined(DATAFLOW___LOGICAL_H)
#error '.inl' file can't be included directly. Use 'logical.h' instead
#endif

inline dataflow::ref<bool> dataflow::operator!(const ref<bool>& x)
{
  return Not(x);
}

inline dataflow::ref<bool> dataflow::operator&&(const ref<bool>& x,
                                                const ref<bool>& y)
{
  return And(x, y);
}

inline dataflow::ref<bool> dataflow::operator||(const ref<bool>& x,
                                                const ref<bool>& y)
{
  return Or(x, y);
}

inline dataflow::ref<bool> dataflow::operator&(const ref<bool>& x,
                                               const ref<bool>& y)
{
  return AndE(x, y);
}

inline dataflow::ref<bool> dataflow::operator|(const ref<bool>& x,
                                               const ref<bool>& y)
{
  return OrE(x, y);
}
