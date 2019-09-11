
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
  std::array<char, sizeof(T) + alignof(T)> raw_;

  struct
  {
    char engaged_;
    T value_;
  } storage_;

  static_assert(sizeof(decltype(raw_)) == sizeof(decltype(storage_)),
                "Incompatible sizes");

  explicit data()
  : raw_{}
  {
  }

  explicit data(const T& x)
  : storage_{true, x}
  {
  }

  data(const data& other)
  : data{}
  {
    if (other.storage_.engaged_)
    {
      data tmp{other.storage_.value_};
      std::swap(raw_, tmp.raw_);
    }
  }

  data& operator=(data other)
  {
    std::swap(raw_, other.raw_);

    return *this;
  }

  ~data()
  {
    if (storage_.engaged_)
      storage_.value_.~T();
  }
};

template <typename T> union maybe<ref<T>>::data {
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
maybe<T>::maybe(nothing_t)
: maybe{}
{
}

template <typename T>
maybe<T>::maybe()
: data_{}
{
}

template <typename T>
maybe<T>::maybe(const T& x)
: data_{x}
{
}

template <typename T> bool maybe<T>::engaged() const
{
  return data_.storage_.engaged_;
}

template <typename T> T maybe<T>::value_or(const T& def) const
{
  if (engaged())
    return data_.storage_.value_;
  else
    return def;
}

template <typename T> bool maybe<T>::operator==(const maybe& other) const
{
  return engaged() == other.engaged() &&
         data_.storage_.value_ == other.data_.storage_.value_;
}

template <typename T> bool maybe<T>::operator!=(const maybe& other) const
{
  return !(*this == other);
}

template <typename T>
maybe<ref<T>>::maybe(nothing_t)
: maybe{}
{
}

template <typename T>
maybe<ref<T>>::maybe()
: data_{}
{
}

template <typename T>
maybe<ref<T>>::maybe(const ref<T>& x)
: data_{x}
{
}

template <typename T> bool maybe<ref<T>>::engaged() const
{
  return data_.engaged_;
}

template <typename T> ref<T> maybe<ref<T>>::value_or(const ref<T>& def) const
{
  if (engaged())
    return data_.value_;
  else
    return def;
}

template <typename T> ref<T> maybe<ref<T>>::value_or() const
{
  if (engaged())
    return data_.value_;
  else
    return Const<T>();
}

template <typename T> bool maybe<ref<T>>::operator==(const maybe& other) const
{
  return data_.engaged_ == other.data_.engaged_;
}

template <typename T> bool maybe<ref<T>>::operator!=(const maybe& other) const
{
  return !(*this == other);
}
}

template <typename T>
std::ostream& dataflow::operator<<(std::ostream& out, const maybe<T>& value)
{
  if (value.engaged())
  {
    out << "just(" << core::to_string(value.value_or()) << ")";
  }
  else
  {
    out << "nothing";
  }
  return out;
}

template <typename T>
dataflow::ref<dataflow::maybe<T>> dataflow::JustE(const ref<T>& x)
{
  struct policy
  {
    static std::string label()
    {
      return "just";
    }
    maybe<T> calculate(const T& x)
    {
      return maybe<T>(x);
    }
  };

  return core::Lift<policy>(core::make_argument(x));
}

template <typename T>
dataflow::ref<T> dataflow::FromMaybe(const ref<maybe<ref<T>>>& x)
{
  return FromMaybe(x, Const<T>());
}

template <typename T, typename U, typename>
dataflow::ref<T> dataflow::FromMaybe(const ref<maybe<T>>& x, const U& def)
{
  struct policy
  {
    static std::string label()
    {
      return "from-maybe";
    }
    static T calculate(const maybe<T>& x, const T& def)
    {
      return x.value_or(def);
    }
  };

  return core::Lift<policy>(x, core::make_argument(def));
}

template <typename T>
dataflow::ref<T> dataflow::FromMaybe(const ref<maybe<T>>& x)
{
  return FromMaybe(x, Const<T>());
}

template <typename T> dataflow::ref<dataflow::maybe<T>> dataflow::Nothing()
{
  return Const<maybe<T>>();
}

template <typename T>
dataflow::ref<dataflow::maybe<dataflow::ref<T>>> dataflow::Just(const ref<T>& x)
{
  return Const<maybe<ref<T>>>(x);
}

template <typename T, typename U, typename>
dataflow::ref<T> dataflow::FromMaybe(const ref<maybe<ref<T>>>& x, const U& def)
{
  struct policy
  {
    static std::string label()
    {
      return "from-maybe";
    }
    ref<T> calculate(const maybe<ref<T>>& x)
    {
      return x.value_or(def_);
    }

    ref<T> def_;
  };

  return core::LiftSelector(policy{core::make_argument(def)}, x);
}
