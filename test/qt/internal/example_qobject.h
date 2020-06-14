
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

namespace dataflow_test
{
class example_qobject : QObject
{
  Q_OBJECT

public:
  explicit example_qobject(QObject* p_qobject = nullptr);

public:
  Q_PROPERTY(int myProperty1 MEMBER my_property_1_ NOTIFY myProperty1Changed);
  Q_PROPERTY(int myProperty2 MEMBER my_property_2_ NOTIFY myProperty2Changed);

signals:
  void myProperty1Changed();
  void myProperty2Changed();

public slots:
  void mySlot() const;

private:
  int my_property_1_;
  int my_property_2_;
};
}
