
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

#include "tuple_size.h"

#define DATAFLOW___TUPLE_FOR_EACH_I(macro, data, tuple)                        \
  DATAFLOW___CAT(DATAFLOW___TUPLE_FOR_EACH_I_, DATAFLOW___TUPLE_SIZE(tuple))   \
  (macro, data, tuple, DATAFLOW___TUPLE_SIZE(tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_64(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_63(macro, data, tuple, size)                     \
  macro(_, data, 63, DATAFLOW___TUPLE_ELEM(63, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_63(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_62(macro, data, tuple, size)                     \
  macro(_, data, 62, DATAFLOW___TUPLE_ELEM(62, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_62(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_61(macro, data, tuple, size)                     \
  macro(_, data, 61, DATAFLOW___TUPLE_ELEM(61, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_61(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_60(macro, data, tuple, size)                     \
  macro(_, data, 60, DATAFLOW___TUPLE_ELEM(60, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_60(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_59(macro, data, tuple, size)                     \
  macro(_, data, 59, DATAFLOW___TUPLE_ELEM(59, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_59(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_58(macro, data, tuple, size)                     \
  macro(_, data, 58, DATAFLOW___TUPLE_ELEM(58, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_58(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_57(macro, data, tuple, size)                     \
  macro(_, data, 57, DATAFLOW___TUPLE_ELEM(57, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_57(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_56(macro, data, tuple, size)                     \
  macro(_, data, 56, DATAFLOW___TUPLE_ELEM(56, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_56(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_55(macro, data, tuple, size)                     \
  macro(_, data, 55, DATAFLOW___TUPLE_ELEM(55, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_55(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_54(macro, data, tuple, size)                     \
  macro(_, data, 54, DATAFLOW___TUPLE_ELEM(54, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_54(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_53(macro, data, tuple, size)                     \
  macro(_, data, 53, DATAFLOW___TUPLE_ELEM(53, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_53(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_52(macro, data, tuple, size)                     \
  macro(_, data, 52, DATAFLOW___TUPLE_ELEM(52, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_52(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_51(macro, data, tuple, size)                     \
  macro(_, data, 51, DATAFLOW___TUPLE_ELEM(51, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_51(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_50(macro, data, tuple, size)                     \
  macro(_, data, 50, DATAFLOW___TUPLE_ELEM(50, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_50(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_49(macro, data, tuple, size)                     \
  macro(_, data, 49, DATAFLOW___TUPLE_ELEM(49, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_49(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_48(macro, data, tuple, size)                     \
  macro(_, data, 48, DATAFLOW___TUPLE_ELEM(48, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_48(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_47(macro, data, tuple, size)                     \
  macro(_, data, 47, DATAFLOW___TUPLE_ELEM(47, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_47(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_46(macro, data, tuple, size)                     \
  macro(_, data, 46, DATAFLOW___TUPLE_ELEM(46, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_46(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_45(macro, data, tuple, size)                     \
  macro(_, data, 45, DATAFLOW___TUPLE_ELEM(45, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_45(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_44(macro, data, tuple, size)                     \
  macro(_, data, 44, DATAFLOW___TUPLE_ELEM(44, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_44(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_43(macro, data, tuple, size)                     \
  macro(_, data, 43, DATAFLOW___TUPLE_ELEM(43, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_43(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_42(macro, data, tuple, size)                     \
  macro(_, data, 42, DATAFLOW___TUPLE_ELEM(42, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_42(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_41(macro, data, tuple, size)                     \
  macro(_, data, 41, DATAFLOW___TUPLE_ELEM(41, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_41(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_40(macro, data, tuple, size)                     \
  macro(_, data, 40, DATAFLOW___TUPLE_ELEM(40, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_40(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_39(macro, data, tuple, size)                     \
  macro(_, data, 39, DATAFLOW___TUPLE_ELEM(39, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_39(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_38(macro, data, tuple, size)                     \
  macro(_, data, 38, DATAFLOW___TUPLE_ELEM(38, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_38(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_37(macro, data, tuple, size)                     \
  macro(_, data, 37, DATAFLOW___TUPLE_ELEM(37, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_37(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_36(macro, data, tuple, size)                     \
  macro(_, data, 36, DATAFLOW___TUPLE_ELEM(36, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_36(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_35(macro, data, tuple, size)                     \
  macro(_, data, 35, DATAFLOW___TUPLE_ELEM(35, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_35(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_34(macro, data, tuple, size)                     \
  macro(_, data, 34, DATAFLOW___TUPLE_ELEM(34, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_34(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_33(macro, data, tuple, size)                     \
  macro(_, data, 33, DATAFLOW___TUPLE_ELEM(33, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_33(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_32(macro, data, tuple, size)                     \
  macro(_, data, 32, DATAFLOW___TUPLE_ELEM(32, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_32(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_31(macro, data, tuple, size)                     \
  macro(_, data, 31, DATAFLOW___TUPLE_ELEM(31, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_31(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_30(macro, data, tuple, size)                     \
  macro(_, data, 30, DATAFLOW___TUPLE_ELEM(30, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_30(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_29(macro, data, tuple, size)                     \
  macro(_, data, 29, DATAFLOW___TUPLE_ELEM(29, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_29(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_28(macro, data, tuple, size)                     \
  macro(_, data, 28, DATAFLOW___TUPLE_ELEM(28, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_28(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_27(macro, data, tuple, size)                     \
  macro(_, data, 27, DATAFLOW___TUPLE_ELEM(27, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_27(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_26(macro, data, tuple, size)                     \
  macro(_, data, 26, DATAFLOW___TUPLE_ELEM(26, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_26(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_25(macro, data, tuple, size)                     \
  macro(_, data, 25, DATAFLOW___TUPLE_ELEM(25, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_25(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_24(macro, data, tuple, size)                     \
  macro(_, data, 24, DATAFLOW___TUPLE_ELEM(24, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_24(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_23(macro, data, tuple, size)                     \
  macro(_, data, 23, DATAFLOW___TUPLE_ELEM(23, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_23(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_22(macro, data, tuple, size)                     \
  macro(_, data, 22, DATAFLOW___TUPLE_ELEM(22, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_22(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_21(macro, data, tuple, size)                     \
  macro(_, data, 21, DATAFLOW___TUPLE_ELEM(21, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_21(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_20(macro, data, tuple, size)                     \
  macro(_, data, 20, DATAFLOW___TUPLE_ELEM(20, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_20(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_19(macro, data, tuple, size)                     \
  macro(_, data, 19, DATAFLOW___TUPLE_ELEM(19, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_19(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_18(macro, data, tuple, size)                     \
  macro(_, data, 18, DATAFLOW___TUPLE_ELEM(18, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_18(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_17(macro, data, tuple, size)                     \
  macro(_, data, 17, DATAFLOW___TUPLE_ELEM(17, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_17(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_16(macro, data, tuple, size)                     \
  macro(_, data, 16, DATAFLOW___TUPLE_ELEM(16, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_16(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_15(macro, data, tuple, size)                     \
  macro(_, data, 15, DATAFLOW___TUPLE_ELEM(15, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_15(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_14(macro, data, tuple, size)                     \
  macro(_, data, 14, DATAFLOW___TUPLE_ELEM(14, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_14(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_13(macro, data, tuple, size)                     \
  macro(_, data, 13, DATAFLOW___TUPLE_ELEM(13, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_13(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_12(macro, data, tuple, size)                     \
  macro(_, data, 12, DATAFLOW___TUPLE_ELEM(12, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_12(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_11(macro, data, tuple, size)                     \
  macro(_, data, 11, DATAFLOW___TUPLE_ELEM(11, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_11(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_10(macro, data, tuple, size)                     \
  macro(_, data, 10, DATAFLOW___TUPLE_ELEM(10, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_10(macro, data, tuple, size)               \
  DATAFLOW___TUPLE_FOR_EACH_I_9(macro, data, tuple, size)                      \
  macro(_, data, 9, DATAFLOW___TUPLE_ELEM(9, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_9(macro, data, tuple, size)                \
  DATAFLOW___TUPLE_FOR_EACH_I_8(macro, data, tuple, size)                      \
  macro(_, data, 8, DATAFLOW___TUPLE_ELEM(8, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_8(macro, data, tuple, size)                \
  DATAFLOW___TUPLE_FOR_EACH_I_7(macro, data, tuple, size)                      \
  macro(_, data, 7, DATAFLOW___TUPLE_ELEM(7, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_7(macro, data, tuple, size)                \
  DATAFLOW___TUPLE_FOR_EACH_I_6(macro, data, tuple, size)                      \
  macro(_, data, 6, DATAFLOW___TUPLE_ELEM(6, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_6(macro, data, tuple, size)                \
  DATAFLOW___TUPLE_FOR_EACH_I_5(macro, data, tuple, size)                      \
  macro(_, data, 5, DATAFLOW___TUPLE_ELEM(5, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_5(macro, data, tuple, size)                \
  DATAFLOW___TUPLE_FOR_EACH_I_4(macro, data, tuple, size)                      \
  macro(_, data, 4, DATAFLOW___TUPLE_ELEM(4, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_4(macro, data, tuple, size)                \
  DATAFLOW___TUPLE_FOR_EACH_I_3(macro, data, tuple, size)                      \
  macro(_, data, 3, DATAFLOW___TUPLE_ELEM(3, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_3(macro, data, tuple, size)                \
  DATAFLOW___TUPLE_FOR_EACH_I_2(macro, data, tuple, size)                      \
  macro(_, data, 2, DATAFLOW___TUPLE_ELEM(2, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_2(macro, data, tuple, size)                \
  DATAFLOW___TUPLE_FOR_EACH_I_1(macro, data, tuple, size)                      \
  macro(_, data, 1, DATAFLOW___TUPLE_ELEM(1, tuple))

#define DATAFLOW___TUPLE_FOR_EACH_I_1(macro, data, tuple, size)                \
  macro(_, data, 0, DATAFLOW___TUPLE_ELEM(0, tuple))
