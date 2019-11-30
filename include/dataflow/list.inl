
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

#include <algorithm>

namespace dataflow
{
namespace list_internal
{
template <typename T> class element_wrapper
{
public:
  element_wrapper(const ref<T>& x)
  : x_(x)
  {
  }

  bool operator==(const element_wrapper& other) const
  {
    return x_.id() == other.x_.id();
  }

  bool operator!=(const element_wrapper& other) const
  {
    return !(*this == other);
  }

  operator const ref<T>&() const
  {
    return x_;
  }

private:
  ref<T> x_;
};
}

namespace list_detail
{
template <typename T, typename U>
T make_list_element(std::true_type, const U& x)
{
  return core::make_argument(x);
}

template <typename T, typename U>
T make_list_element(std::false_type, const U& x)
{
  return x;
}

template <typename T> integer erase_index(const list<T>& lst, integer idx)
{
  // TODO: std17::clamp?
  return std::max(std::min(idx, static_cast<integer>(lst.size() - 1)), 0);
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

template <typename T> list<T> list<T>::insert(integer idx, const T& v) const
{
  // TODO: std17::clamp?
  return data_.insert(static_cast<std::size_t>(std::max(
                        std::min(idx, static_cast<integer>(data_.size())), 0)),
                      v);
}

template <typename T> list<T> list<T>::erase(integer idx) const
{
  if (data_.empty())
    return *this;

  return data_.erase(
    static_cast<std::size_t>(list_detail::erase_index(*this, idx)));
}

template <typename T> T list<T>::operator[](integer idx) const
{
  return data_.at(static_cast<std::size_t>(idx));
}

template <typename T> integer list<T>::size() const
{
  return static_cast<integer>(data_.size());
}

template <typename T> class list_patch<T>::change_data
{
public:
  change_type type;
  integer idx;
  std::vector<T> args;
};

template <typename T> list_patch<T>::list_patch()
{
}

template <typename T>
list_patch<T>::list_patch(const list<T>& curr, const list<T>& prev)
{
  const auto& m = curr.size();
  const auto& n = prev.size();

  std::vector<int> table_data((m + 1) * (n + 1));

  const auto table = [&](int i, int j) -> int& {
    return table_data.at(i * (n + 1) + j);
  };

  for (int i = 0; i <= m; ++i)
    table(i, 0) = 0;

  for (int j = 0; j <= n; ++j)
    table(0, j) = 0;

  for (int i = 1; i <= m; ++i)
  {
    for (int j = 1; j <= n; ++j)
    {
      if (curr[i - 1] == prev[j - 1])
        table(i, j) = table(i - 1, j - 1) + 1;
      else
        table(i, j) = std::max(table(i, j - 1), table(i - 1, j));
    }
  }

  for (int i = m, j = n; i > 0 || j > 0;)
  {
    if (i > 0 && j > 0 && curr[i - 1] == prev[j - 1])
    {
      --i, --j;
    }
    else if (j > 0 && (i == 0 || table(i, j - 1) >= table(i - 1, j)))
    {
      changes_.push_back({change_type::erase, j - 1, {}});

      --j;
    }
    else if (i > 0 && (j == 0 || table(i, j - 1) < table(i - 1, j)))
    {
      changes_.push_back({change_type::insert, j, {curr[i - 1]}});

      --i;
    }
  }
}

template <typename T> void list_patch<T>::insert(integer idx, const T& v)
{
  changes_.push_back({change_type::insert, idx, {v}});
}

template <typename T> void list_patch<T>::erase(integer idx)
{
  changes_.push_back({change_type::erase, idx, {}});
}

template <typename T>
template <typename Insert, typename Erase>
void list_patch<T>::apply(const Insert& insert, const Erase& erase) const
{
  for (const auto& act : changes_)
  {
    if (act.type == change_type::insert)
    {
      insert(act.idx, act.args.front());
    }
    else if (act.type == change_type::erase)
    {
      erase(act.idx);
    }
  }
}

template <typename T> list<T> list_patch<T>::apply(list<T> v) const
{
  apply([&](const integer& idx, const T& x) { v = v.insert(idx, x); },
        [&](const integer& idx) { v = v.erase(idx); });
  return v;
}

template <typename T> bool list_patch<T>::empty() const
{
  return changes_.empty();
}

template <typename T>
ref<list<T>>::ref(core::ref_base<list<T>> base)
: core::ref_base<list<T>>(base)
{
}

template <typename T> ref<T> ref<list<T>>::operator()(const Time& t) const
{
  return this->snapshot_(t);
}

template <typename T>
ref<maybe<T>> ref<list<T>>::operator[](const ref<integer>& idx) const
{
  return Get(static_cast<const ref<list<T>>&>(*this), idx);
}

template <typename T> var<list<T>>& var<list<T>>::operator=(const list<T>& v)
{
  var_base<list<T>>::set_value_(v);

  return *this;
}

template <typename T>
var<list<T>>::var(var_base<list<T>> base)
: var_base<list<T>>(std::move(base))
{
}

template <typename T> void var<list<T>>::insert(integer idx, const T& v)
{
  list_patch<T> patch;

  patch.insert(idx, v);

  this->set_patch_(patch);
}

template <typename T> void var<list<T>>::erase(integer idx)
{
  list_patch<T> patch;

  patch.erase(idx);

  this->set_patch_(patch);
}

} // dataflow

template <typename T>
std::ostream& dataflow::operator<<(std::ostream& out, const list<T>& value)
{
  out << "list(";

  if (value.size() > 0)
    out << core::to_string(value[0]);

  for (integer i = 1; i < value.size(); ++i)
  {
    out << " " << core::to_string(value[i]);
  }

  out << ")";

  return out;
}

template <typename Arg, typename... Args, typename T>
dataflow::listA<T> dataflow::make_listA(const Arg& x, const Args&... xs)
{
  return listA<T>(x, xs...);
}

template <typename Arg, typename... Args, typename T>
dataflow::listC<T> dataflow::make_listC(const Arg& x, const Args&... xs)
{
  return listC<T>(x, xs...);
}

template <typename Arg, typename... Args, typename T>
dataflow::ref<dataflow::listA<T>> dataflow::ListA(const Arg& x,
                                                  const Args&... xs)
{
  return Const(make_listA(x, xs...));
}

template <typename Arg, typename... Args, typename T>
dataflow::ref<dataflow::listC<T>> dataflow::ListC(const Arg& x,
                                                  const Args&... xs)
{
  struct policy
  {
    static std::string label()
    {
      return "listC";
    }

    static list<T> calculate(const core::argument_data_type_t<Arg>& x,
                             const core::argument_data_type_t<Args>&... xs)
    {
      return make_listC(x, xs...);
    }
  };

  return core::Lift<policy>(core::make_argument(x), core::make_argument(xs)...);
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

template <typename ArgL, typename ArgI, typename T, typename>
dataflow::ref<dataflow::maybe<T>> dataflow::Get(const ArgL& lst,
                                                const ArgI& idx)
{
  struct policy
  {
    static std::string label()
    {
      return "list-get";
    }

    static maybe<T> calculate(const list<T>& x, integer idx)
    {
      return idx < static_cast<integer>(x.size()) ? just(x[idx]) : nothing();
    }
  };

  return core::Lift<policy>(core::make_argument(lst), core::make_argument(idx));
}

template <typename ArgL,
          typename ArgI,
          typename ArgX,
          typename T,
          typename,
          typename>
dataflow::ref<dataflow::list<T>>
dataflow::Insert(const ArgL& l, const ArgI& idx, const ArgX& x)
{
  struct policy
  {
    static std::string label()
    {
      return "list-insert";
    }

    list<T> calculate(const list<T>& l, integer idx, const T& x)
    {
      return l.insert(idx, x);
    }

    list_patch<T> prepare_patch(const core::diff_type_t<list<T>>& diff_l,
                                const core::diff_type_t<integer>& diff_idx,
                                const core::diff_type_t<T>& diff_x)
    {
      DATAFLOW___CHECK_CONDITION((diff_l.curr() == diff_l.prev()) ==
                                 diff_l.patch().empty());

      list_patch<T> patch;
      patch.erase(diff_idx.prev());

      diff_l.patch().apply(
        [&](const integer& idx, const T& x) { patch.insert(idx, x); },
        [&](const integer& idx) { patch.erase(idx); });

      patch.insert(diff_idx.curr(), diff_x.curr());

      return patch;
    }
  };

  return core::LiftPatcher<policy>(
    core::make_argument(l), core::make_argument(idx), core::make_argument(x));
}

template <typename ArgL, typename ArgI, typename T, typename>
dataflow::ref<dataflow::list<T>> dataflow::Erase(const ArgL& l, const ArgI& idx)
{
  struct policy
  {
    static std::string label()
    {
      return "list-erase";
    }

    list<T> calculate(const list<T>& l, integer idx)
    {
      return l.erase(idx);
    }

    list_patch<T> prepare_patch(const core::diff_type_t<list<T>>& diff_l,
                                const core::diff_type_t<integer>& diff_idx)
    {
      DATAFLOW___CHECK_CONDITION((diff_l.curr() == diff_l.prev()) ==
                                 diff_l.patch().empty());

      const auto& erased_element =
        diff_l.prev()[list_detail::erase_index(diff_l.prev(), diff_idx.prev())];

      list_patch<T> patch;

      patch.insert(diff_idx.prev(), erased_element);

      diff_l.patch().apply(
        [&](const integer& idx, const T& x) { patch.insert(idx, x); },
        [&](const integer& idx) { patch.erase(idx); });

      patch.erase(diff_idx.curr());

      return patch;
    }
  };

  return core::LiftPatcher<policy>(core::make_argument(l),
                                   core::make_argument(idx));
}
