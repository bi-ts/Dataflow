
//  Copyright (c) 2014 - 2020 Maksym V. Bilinets.
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

#include "qobject_tracker.h"

#include <algorithm>
#include <vector>

namespace dataflow
{
namespace qt
{
namespace internal
{
std::size_t qobject_tracker::g_counter_ = 0;
std::unordered_map<qobject_tracker*, qobject_tracker::info>
  qobject_tracker::g_qobject_info_{};

void qobject_tracker::track(QObject* p_qobject)
{
  new qobject_tracker{p_qobject};
}

std::size_t qobject_tracker::counter()
{
  return g_qobject_info_.size();
}

void qobject_tracker::print_all(std::ostream& out)
{
  std::vector<info> infos;
  infos.reserve(g_qobject_info_.size());

  std::transform(g_qobject_info_.begin(),
                 g_qobject_info_.end(),
                 std::back_inserter(infos),
                 [](const auto& key_value) { return key_value.second; });

  std::sort(infos.begin(), infos.end(), [](const info& lhs, const info& rhs) {
    return lhs.index < rhs.index;
  });

  for (const auto& info : infos)
  {
    out << info.p_qobject << " " << info.type_name << std::endl;
  }
}

qobject_tracker::qobject_tracker(QObject* p_qobject)
: QObject{p_qobject}
{
  g_qobject_info_[this] = {
    g_counter_++, p_qobject, p_qobject->metaObject()->className()};
}

qobject_tracker::~qobject_tracker()
{
  g_qobject_info_.erase(this);
}
}
}
}
