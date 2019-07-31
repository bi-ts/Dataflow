
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

#if !defined(DATAFLOW___INTERNAL_VD_HANDLE_H)
#error '.inl' file can't be included directly. Use 'vd_handle.h' instead
#endif

namespace dataflow
{
namespace internal
{
vd_handle::vd_handle(vertex_descriptor v)
: v_(v)
{
  engine::instance().add_ref(v_);
}

vd_handle::vd_handle(const vd_handle& other)
: vd_handle(other.v_)
{
}

vd_handle::~vd_handle()
{
  engine::instance().release(v_);
}

vd_handle::operator vertex_descriptor() const
{
  return v_;
}
} // internal
} // dataflow
