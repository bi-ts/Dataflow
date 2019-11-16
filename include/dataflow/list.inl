
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

#if !defined(DATAFLOW___LIST_H)
#error '.inl' file can't be included directly. Use 'list.h' instead
#endif

namespace dataflow
{
namespace list_detail
{
template <typename T, typename U>
core::ref_base make_list_element(std::true_type, const U& x)
{
  return static_cast<core::ref_base>(core::make_argument(x));
}

template <typename T, typename U>
T make_list_element(std::false_type, const U& x)
{
  return x;
}
}

template <typename T>
template <typename U, typename... Us>
list<T>::list(const U& x, const Us&... xs)
: data_({list_detail::make_list_element<T>(core::is_ref<T>(), x),
         list_detail::make_list_element<T>(core::is_ref<T>(), xs)...})
{
}

template <typename T> bool list<T>::operator==(const list& other) const
{
  return data_ == other.data_;
}

template <typename T> bool list<T>::operator!=(const list& other) const
{
  return !(*this == other);
}

template <typename T> integer list<T>::size() const
{
  return static_cast<integer>(data_.size());
}
} // dataflow

template <typename T>
std::ostream& dataflow::operator<<(std::ostream& out, const list<T>& value)
{
  out << "list";
  return out;
}

template <typename Arg, typename... Args, typename T>
dataflow::listA<T> dataflow::make_listA(const Arg& x, const Args&... xs)
{
  return listA<T>(x, xs...);
}

template <typename Arg, typename... Args, typename T>
dataflow::ref<dataflow::listA<T>> dataflow::ListA(const Arg& x,
                                                  const Args&... xs)
{
  return Const(make_listA(x, xs...));
}

template <typename T>
dataflow::ref<dataflow::integer> dataflow::Length(const ref<list<T>>& x)
{
  struct policy
  {
    static std::string label()
    {
      return "list-length";
    }
    static integer calculate(const list<T>& v)
    {
      return v.size();
    }
  };

  return core::Lift<policy>(x);
}
