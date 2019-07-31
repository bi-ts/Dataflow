
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

#ifndef DATAFLOW___INTERNAL_VD_HANDLE_H
#define DATAFLOW___INTERNAL_VD_HANDLE_H

#include "engine.h"

namespace dataflow
{
namespace internal
{
class vd_handle
{
public:
  vd_handle(vertex_descriptor v);

  vd_handle(const vd_handle& other);

  ~vd_handle();

  operator vertex_descriptor() const;

  vd_handle& operator=(const vd_handle&) = delete;

private:
  vertex_descriptor v_;
};
} // internal
} // dataflow

#include "vd_handle.inl"

#endif // DATAFLOW___INTERNAL_VD_HANDLE_H
