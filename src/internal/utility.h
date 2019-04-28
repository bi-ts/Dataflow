
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

#include <boost/graph/depth_first_search.hpp>

namespace dataflow
{
namespace internal
{
class color_map
{
public:
  typedef vertex_descriptor key_type;
  typedef boost::default_color_type value_type;
  typedef const boost::default_color_type& reference;
  typedef boost::read_write_property_map_tag category;

public:
  color_map(dependency_graph& g)
  : graph_(g)
  {
  }

  friend value_type get(const color_map& cm, vertex_descriptor v)
  {
    if (cm.graph_[v].vertex_color == vertex::color::white)
    {
      return boost::white_color;
    }
    else if (cm.graph_[v].vertex_color == vertex::color::gray)
    {
      return boost::gray_color;
    }

    return boost::black_color;
  }

  friend void
  put(color_map& cm, vertex_descriptor v, boost::default_color_type c)
  {
    if (c == boost::gray_color)
      cm.graph_[v].vertex_color = vertex::color::gray;
    else if (c == boost::black_color)
      cm.graph_[v].vertex_color = vertex::color::black;
    else
      cm.graph_[v].vertex_color = vertex::color::white;
  }

private:
  dependency_graph& graph_;
};
} // internal
} // dataflow
