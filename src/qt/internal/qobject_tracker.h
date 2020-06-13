
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

#pragma once

#include <QtCore/QObject>

#include <ostream>
#include <unordered_map>

namespace dataflow
{
namespace qt
{
namespace internal
{
class qobject_tracker : QObject
{
  Q_OBJECT

public:
  static void track(QObject* p_qobject);

  static std::size_t counter();
  static void print_all(std::ostream& out);

private:
  qobject_tracker(QObject* p_qobject);
  ~qobject_tracker() override;

private:
  struct info
  {
    std::size_t index;
    QObject* p_qobject;
    std::string type_name;
  };

private:
  static std::size_t g_counter_;
  static std::unordered_map<qobject_tracker*, info> g_qobject_info_;
};
}
}
}
