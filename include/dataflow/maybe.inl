
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

#if !defined(DATAFLOW___MAYBE_H)
#error '.inl' file can't be included directly. Use 'maybe.h' instead
#endif

namespace dataflow
{
template <typename T> union maybe<T>::data {
  std::intptr_t engaged_;
  ref<T> value_;

  static_assert(sizeof(decltype(engaged_)) == sizeof(decltype(value_)),
                "Incompatible sizes");

  explicit data()
  : engaged_{0x0}
  {
  }

  explicit data(const ref<T>& x)
  : value_{x}
  {
  }

  data(const data& other)
  : engaged_{0x0}
  {
    if (other.engaged_)
    {
      data tmp{other.value_};
      std::swap(engaged_, tmp.engaged_);
    }
  }

  data& operator=(data other)
  {
    std::swap(engaged_, other.engaged_);

    return *this;
  }

  ~data()
  {
    if (engaged_)
      value_.~ref();

    engaged_ = 0x0;
  }
};

template <typename T>
maybe<T>::maybe()
: data_{}
{
}

template <typename T>
maybe<T>::maybe(const ref<T>& x)
: data_{x}
{
}

template <typename T> bool maybe<T>::engaged() const
{
  return data_.engaged_;
}

template <typename T> ref<T> maybe<T>::value_or(const ref<T>& def) const
{
  if (engaged())
    return data_.value_;
  else
    return def;
}

template <typename T> bool maybe<T>::operator==(const maybe<T>& other) const
{
  return data_.engaged_ == other.data_.engaged_;
}

template <typename T> bool maybe<T>::operator!=(const maybe<T>& other) const
{
  return !(*this == other);
}
}
