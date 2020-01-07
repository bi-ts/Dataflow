
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

#include <dataflow/prelude/core.h>

#include <QtCore/QObject>
#include <QtCore/QPointF>

inline std::ostream& operator<<(std::ostream& out, QPointF value)
{
  out << "QPointF(" << value.x() << "; " << value.y() << ")";
  return out;
}

class Input : public QObject
{
  Q_OBJECT

public:
  Input()
  : left_mouse_button_pressed(dataflow::Var<int>())
  , mouse_pos(dataflow::Var<QPointF>())
  {
  }

public:
  Q_PROPERTY(
    bool leftMouseButtonPressed READ get_leftMouseButtonPressed_ WRITE
      set_leftMouseButtonPressed_ NOTIFY leftMouseButtonPressedChanged);
  Q_PROPERTY(QPointF mousePos READ get_mousePos_ WRITE set_mousePos_ NOTIFY
               mousePosChanged);

public:
  dataflow::var<int> left_mouse_button_pressed;
  dataflow::var<QPointF> mouse_pos;

signals:
  void leftMouseButtonPressedChanged();

private:
  bool get_leftMouseButtonPressed_()
  {
    return *left_mouse_button_pressed;
  }

  void set_leftMouseButtonPressed_(bool value)
  {
    left_mouse_button_pressed = value;
    emit leftMouseButtonPressedChanged();
  }

signals:
  void mousePosChanged();

public:
  void set_mousePos_(const QPointF& value)
  {
    mouse_pos = value;
    emit mousePosChanged();
  }

  QPointF get_mousePos_() const
  {
    return *mouse_pos;
  }
};
