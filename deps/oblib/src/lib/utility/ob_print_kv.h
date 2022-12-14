/**
 * Copyright (c) 2021 OceanBase
 * OceanBase CE is licensed under Mulan PubL v2.
 * You can use this software according to the terms and conditions of the Mulan PubL v2.
 * You may obtain a copy of Mulan PubL v2 at:
 *          http://license.coscl.org.cn/MulanPubL-2.0
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PubL v2 for more details.
 */

#ifndef LIB_UTILITY_OB_PRINT_KV_
#define LIB_UTILITY_OB_PRINT_KV_

#include "lib/oblog/ob_log_module.h"
#include "lib/utility/ob_macro_utils.h"

#define TYPENAME_TN0
#define TYPENAME_TN1 typename T1
#define TYPENAME_TN2 TYPENAME_TN1, typename T2
#define TYPENAME_TN3 TYPENAME_TN2, typename T3
#define TYPENAME_TN4 TYPENAME_TN3, typename T4
#define TYPENAME_TN5 TYPENAME_TN4, typename T5
#define TYPENAME_TN6 TYPENAME_TN5, typename T6
#define TYPENAME_TN7 TYPENAME_TN6, typename T7
#define TYPENAME_TN8 TYPENAME_TN7, typename T8
#define TYPENAME_TN9 TYPENAME_TN8, typename T9
#define TYPENAME_TN10 TYPENAME_TN9, typename T10
#define TYPENAME_TN11 TYPENAME_TN10, typename T11
#define TYPENAME_TN12 TYPENAME_TN11, typename T12
#define TYPENAME_TN13 TYPENAME_TN12, typename T13
#define TYPENAME_TN14 TYPENAME_TN13, typename T14
#define TYPENAME_TN15 TYPENAME_TN14, typename T15
#define TYPENAME_TN16 TYPENAME_TN15, typename T16
#define TYPENAME_TN17 TYPENAME_TN16, typename T17
#define TYPENAME_TN18 TYPENAME_TN17, typename T18
#define TYPENAME_TN19 TYPENAME_TN18, typename T19
#define TYPENAME_TN20 TYPENAME_TN19, typename T20
#define TYPENAME_TN21 TYPENAME_TN20, typename T21
#define TYPENAME_TN22 TYPENAME_TN21, typename T22
#define TYPENAME_TN23 TYPENAME_TN22, typename T23
#define TYPENAME_TN24 TYPENAME_TN23, typename T24
#define TYPENAME_TN25 TYPENAME_TN24, typename T25
#define TYPENAME_TN26 TYPENAME_TN25, typename T26
#define TYPENAME_TN27 TYPENAME_TN26, typename T27
#define TYPENAME_TN28 TYPENAME_TN27, typename T28
#define TYPENAME_TN29 TYPENAME_TN28, typename T29
#define TYPENAME_TN30 TYPENAME_TN29, typename T30
#define TYPENAME_TN31 TYPENAME_TN30, typename T31
#define TYPENAME_TN32 TYPENAME_TN31, typename T32
#define TYPENAME_TN33 TYPENAME_TN32, typename T33
#define TYPENAME_TN34 TYPENAME_TN33, typename T34
#define TYPENAME_TN35 TYPENAME_TN34, typename T35
#define TYPENAME_TN36 TYPENAME_TN35, typename T36
#define TYPENAME_TN37 TYPENAME_TN36, typename T37
#define TYPENAME_TN38 TYPENAME_TN37, typename T38
#define TYPENAME_TN39 TYPENAME_TN38, typename T39
#define TYPENAME_TN40 TYPENAME_TN39, typename T40
#define TYPENAME_TN41 TYPENAME_TN40, typename T41
#define TYPENAME_TN42 TYPENAME_TN41, typename T42
#define TYPENAME_TN43 TYPENAME_TN42, typename T43
#define TYPENAME_TN44 TYPENAME_TN43, typename T44
#define TYPENAME_TN45 TYPENAME_TN44, typename T45
#define TYPENAME_TN46 TYPENAME_TN45, typename T46
#define TYPENAME_TN47 TYPENAME_TN46, typename T47
#define TYPENAME_TN48 TYPENAME_TN47, typename T48
#define TYPENAME_TN49 TYPENAME_TN48, typename T49
#define TYPENAME_TN50 TYPENAME_TN49, typename T50
#define TYPENAME_TN51 TYPENAME_TN50, typename T51
#define TYPENAME_TN52 TYPENAME_TN51, typename T52
#define TYPENAME_TN53 TYPENAME_TN52, typename T53
#define TYPENAME_TN54 TYPENAME_TN53, typename T54
#define TYPENAME_TN55 TYPENAME_TN54, typename T55
#define TYPENAME_TN56 TYPENAME_TN55, typename T56
#define TYPENAME_TN57 TYPENAME_TN56, typename T57
#define TYPENAME_TN58 TYPENAME_TN57, typename T58
#define TYPENAME_TN59 TYPENAME_TN58, typename T59
#define TYPENAME_TN60 TYPENAME_TN59, typename T60
#define TYPENAME_TN61 TYPENAME_TN60, typename T61
#define TYPENAME_TN62 TYPENAME_TN61, typename T62
#define TYPENAME_TN63 TYPENAME_TN62, typename T63
#define TYPENAME_TN64 TYPENAME_TN63, typename T64

#define FUNC_ARG(n) const char* key##n, const T##n &obj##n
#define ARGUMENT_KV0
#define ARGUMENT_KV1 FUNC_ARG(1)
#define ARGUMENT_KV2 ARGUMENT_KV1, FUNC_ARG(2)
#define ARGUMENT_KV3 ARGUMENT_KV2, FUNC_ARG(3)
#define ARGUMENT_KV4 ARGUMENT_KV3, FUNC_ARG(4)
#define ARGUMENT_KV5 ARGUMENT_KV4, FUNC_ARG(5)
#define ARGUMENT_KV6 ARGUMENT_KV5, FUNC_ARG(6)
#define ARGUMENT_KV7 ARGUMENT_KV6, FUNC_ARG(7)
#define ARGUMENT_KV8 ARGUMENT_KV7, FUNC_ARG(8)
#define ARGUMENT_KV9 ARGUMENT_KV8, FUNC_ARG(9)
#define ARGUMENT_KV10 ARGUMENT_KV9, FUNC_ARG(10)
#define ARGUMENT_KV11 ARGUMENT_KV10, FUNC_ARG(11)
#define ARGUMENT_KV12 ARGUMENT_KV11, FUNC_ARG(12)
#define ARGUMENT_KV13 ARGUMENT_KV12, FUNC_ARG(13)
#define ARGUMENT_KV14 ARGUMENT_KV13, FUNC_ARG(14)
#define ARGUMENT_KV15 ARGUMENT_KV14, FUNC_ARG(15)
#define ARGUMENT_KV16 ARGUMENT_KV15, FUNC_ARG(16)
#define ARGUMENT_KV17 ARGUMENT_KV16, FUNC_ARG(17)
#define ARGUMENT_KV18 ARGUMENT_KV17, FUNC_ARG(18)
#define ARGUMENT_KV19 ARGUMENT_KV18, FUNC_ARG(19)
#define ARGUMENT_KV20 ARGUMENT_KV19, FUNC_ARG(20)
#define ARGUMENT_KV21 ARGUMENT_KV20, FUNC_ARG(21)
#define ARGUMENT_KV22 ARGUMENT_KV21, FUNC_ARG(22)
#define ARGUMENT_KV23 ARGUMENT_KV22, FUNC_ARG(23)
#define ARGUMENT_KV24 ARGUMENT_KV23, FUNC_ARG(24)
#define ARGUMENT_KV25 ARGUMENT_KV24, FUNC_ARG(25)
#define ARGUMENT_KV26 ARGUMENT_KV25, FUNC_ARG(26)
#define ARGUMENT_KV27 ARGUMENT_KV26, FUNC_ARG(27)
#define ARGUMENT_KV28 ARGUMENT_KV27, FUNC_ARG(28)
#define ARGUMENT_KV29 ARGUMENT_KV28, FUNC_ARG(29)
#define ARGUMENT_KV30 ARGUMENT_KV29, FUNC_ARG(30)
#define ARGUMENT_KV31 ARGUMENT_KV30, FUNC_ARG(31)
#define ARGUMENT_KV32 ARGUMENT_KV31, FUNC_ARG(32)
#define ARGUMENT_KV33 ARGUMENT_KV32, FUNC_ARG(33)
#define ARGUMENT_KV34 ARGUMENT_KV33, FUNC_ARG(34)
#define ARGUMENT_KV35 ARGUMENT_KV34, FUNC_ARG(35)
#define ARGUMENT_KV36 ARGUMENT_KV35, FUNC_ARG(36)
#define ARGUMENT_KV37 ARGUMENT_KV36, FUNC_ARG(37)
#define ARGUMENT_KV38 ARGUMENT_KV37, FUNC_ARG(38)
#define ARGUMENT_KV39 ARGUMENT_KV38, FUNC_ARG(39)
#define ARGUMENT_KV40 ARGUMENT_KV39, FUNC_ARG(40)
#define ARGUMENT_KV41 ARGUMENT_KV40, FUNC_ARG(41)
#define ARGUMENT_KV42 ARGUMENT_KV41, FUNC_ARG(42)
#define ARGUMENT_KV43 ARGUMENT_KV42, FUNC_ARG(43)
#define ARGUMENT_KV44 ARGUMENT_KV43, FUNC_ARG(44)
#define ARGUMENT_KV45 ARGUMENT_KV44, FUNC_ARG(45)
#define ARGUMENT_KV46 ARGUMENT_KV45, FUNC_ARG(46)
#define ARGUMENT_KV47 ARGUMENT_KV46, FUNC_ARG(47)
#define ARGUMENT_KV48 ARGUMENT_KV47, FUNC_ARG(48)
#define ARGUMENT_KV49 ARGUMENT_KV48, FUNC_ARG(49)
#define ARGUMENT_KV50 ARGUMENT_KV49, FUNC_ARG(50)
#define ARGUMENT_KV51 ARGUMENT_KV50, FUNC_ARG(51)
#define ARGUMENT_KV52 ARGUMENT_KV51, FUNC_ARG(52)
#define ARGUMENT_KV53 ARGUMENT_KV52, FUNC_ARG(53)
#define ARGUMENT_KV54 ARGUMENT_KV53, FUNC_ARG(54)
#define ARGUMENT_KV55 ARGUMENT_KV54, FUNC_ARG(55)
#define ARGUMENT_KV56 ARGUMENT_KV55, FUNC_ARG(56)
#define ARGUMENT_KV57 ARGUMENT_KV56, FUNC_ARG(57)
#define ARGUMENT_KV58 ARGUMENT_KV57, FUNC_ARG(58)
#define ARGUMENT_KV59 ARGUMENT_KV58, FUNC_ARG(59)
#define ARGUMENT_KV60 ARGUMENT_KV59, FUNC_ARG(60)
#define ARGUMENT_KV61 ARGUMENT_KV60, FUNC_ARG(61)
#define ARGUMENT_KV62 ARGUMENT_KV61, FUNC_ARG(62)
#define ARGUMENT_KV63 ARGUMENT_KV62, FUNC_ARG(63)
#define ARGUMENT_KV64 ARGUMENT_KV63, FUNC_ARG(64)

#define J_COMMA_WITH_RET \
    if (OB_FAIL(ret)) { \
    } else if (OB_FAIL(J_COMMA())) { \
    } else {}

#define BODY(n) \
    if (OB_SUCC(ret)) { \
      if (1 == n) {\
        ret = common::databuff_print_json_kv(buf, buf_len, pos, key##n, obj##n);\
      } else {\
        ret = common::databuff_print_json_kv_comma(buf, buf_len, pos, key##n, obj##n);\
      }\
    }

#define FUNC_BODY_0
#define FUNC_BODY_1 BODY(1)
#define FUNC_BODY_2 FUNC_BODY_1 BODY(2)
#define FUNC_BODY_3 FUNC_BODY_2 BODY(3)
#define FUNC_BODY_4 FUNC_BODY_3 BODY(4)
#define FUNC_BODY_5 FUNC_BODY_4 BODY(5)
#define FUNC_BODY_6 FUNC_BODY_5 BODY(6)
#define FUNC_BODY_7 FUNC_BODY_6 BODY(7)
#define FUNC_BODY_8 FUNC_BODY_7 BODY(8)
#define FUNC_BODY_9 FUNC_BODY_8 BODY(9)
#define FUNC_BODY_10 FUNC_BODY_9 BODY(10)
#define FUNC_BODY_11 FUNC_BODY_10 BODY(11)
#define FUNC_BODY_12 FUNC_BODY_11 BODY(12)
#define FUNC_BODY_13 FUNC_BODY_12 BODY(13)
#define FUNC_BODY_14 FUNC_BODY_13 BODY(14)
#define FUNC_BODY_15 FUNC_BODY_14 BODY(15)
#define FUNC_BODY_16 FUNC_BODY_15 BODY(16)
#define FUNC_BODY_17 FUNC_BODY_16 BODY(17)
#define FUNC_BODY_18 FUNC_BODY_17 BODY(18)
#define FUNC_BODY_19 FUNC_BODY_18 BODY(19)
#define FUNC_BODY_20 FUNC_BODY_19 BODY(20)
#define FUNC_BODY_21 FUNC_BODY_20 BODY(21)
#define FUNC_BODY_22 FUNC_BODY_21 BODY(22)
#define FUNC_BODY_23 FUNC_BODY_22 BODY(23)
#define FUNC_BODY_24 FUNC_BODY_23 BODY(24)
#define FUNC_BODY_25 FUNC_BODY_24 BODY(25)
#define FUNC_BODY_26 FUNC_BODY_25 BODY(26)
#define FUNC_BODY_27 FUNC_BODY_26 BODY(27)
#define FUNC_BODY_28 FUNC_BODY_27 BODY(28)
#define FUNC_BODY_29 FUNC_BODY_28 BODY(29)
#define FUNC_BODY_30 FUNC_BODY_29 BODY(30)
#define FUNC_BODY_31 FUNC_BODY_30 BODY(31)
#define FUNC_BODY_32 FUNC_BODY_31 BODY(32)
#define FUNC_BODY_33 FUNC_BODY_32 BODY(33)
#define FUNC_BODY_34 FUNC_BODY_33 BODY(34)
#define FUNC_BODY_35 FUNC_BODY_34 BODY(35)
#define FUNC_BODY_36 FUNC_BODY_35 BODY(36)
#define FUNC_BODY_37 FUNC_BODY_36 BODY(37)
#define FUNC_BODY_38 FUNC_BODY_37 BODY(38)
#define FUNC_BODY_39 FUNC_BODY_38 BODY(39)
#define FUNC_BODY_40 FUNC_BODY_39 BODY(40)
#define FUNC_BODY_41 FUNC_BODY_40 BODY(41)
#define FUNC_BODY_42 FUNC_BODY_41 BODY(42)
#define FUNC_BODY_43 FUNC_BODY_42 BODY(43)
#define FUNC_BODY_44 FUNC_BODY_43 BODY(44)
#define FUNC_BODY_45 FUNC_BODY_44 BODY(45)
#define FUNC_BODY_46 FUNC_BODY_45 BODY(46)
#define FUNC_BODY_47 FUNC_BODY_46 BODY(47)
#define FUNC_BODY_48 FUNC_BODY_47 BODY(48)
#define FUNC_BODY_49 FUNC_BODY_48 BODY(49)
#define FUNC_BODY_50 FUNC_BODY_49 BODY(50)
#define FUNC_BODY_51 FUNC_BODY_50 BODY(51)
#define FUNC_BODY_52 FUNC_BODY_51 BODY(52)
#define FUNC_BODY_53 FUNC_BODY_52 BODY(53)
#define FUNC_BODY_54 FUNC_BODY_53 BODY(54)
#define FUNC_BODY_55 FUNC_BODY_54 BODY(55)
#define FUNC_BODY_56 FUNC_BODY_55 BODY(56)
#define FUNC_BODY_57 FUNC_BODY_56 BODY(57)
#define FUNC_BODY_58 FUNC_BODY_57 BODY(58)
#define FUNC_BODY_59 FUNC_BODY_58 BODY(59)
#define FUNC_BODY_60 FUNC_BODY_59 BODY(60)
#define FUNC_BODY_61 FUNC_BODY_60 BODY(61)
#define FUNC_BODY_62 FUNC_BODY_61 BODY(62)
#define FUNC_BODY_63 FUNC_BODY_62 BODY(63)
#define FUNC_BODY_64 FUNC_BODY_63 BODY(64)

#define DEFINE_PRINT_KV(n)\
  template <TYPENAME_TN##n>                                             \
  int databuff_print_kv(char *buf, const int64_t buf_len, int64_t& pos, ARGUMENT_KV##n)\
  {                                                                     \
    int ret = OB_SUCCESS;                                               \
    FUNC_BODY_##n                                                       \
    return ret;                                                         \
  }

namespace oceanbase
{
namespace common
{
DEFINE_PRINT_KV(1)
DEFINE_PRINT_KV(2)
DEFINE_PRINT_KV(3)
DEFINE_PRINT_KV(4)
DEFINE_PRINT_KV(5)
DEFINE_PRINT_KV(6)
DEFINE_PRINT_KV(7)
DEFINE_PRINT_KV(8)
DEFINE_PRINT_KV(9)
DEFINE_PRINT_KV(10)
DEFINE_PRINT_KV(11)
DEFINE_PRINT_KV(12)
DEFINE_PRINT_KV(13)
DEFINE_PRINT_KV(14)
DEFINE_PRINT_KV(15)
DEFINE_PRINT_KV(16)
DEFINE_PRINT_KV(17)
DEFINE_PRINT_KV(18)
DEFINE_PRINT_KV(19)
DEFINE_PRINT_KV(20)
DEFINE_PRINT_KV(21)
DEFINE_PRINT_KV(22)
DEFINE_PRINT_KV(23)
DEFINE_PRINT_KV(24)
DEFINE_PRINT_KV(25)
DEFINE_PRINT_KV(26)
DEFINE_PRINT_KV(27)
DEFINE_PRINT_KV(28)
DEFINE_PRINT_KV(29)
DEFINE_PRINT_KV(30)
DEFINE_PRINT_KV(31)
DEFINE_PRINT_KV(32)
DEFINE_PRINT_KV(33)
DEFINE_PRINT_KV(34)
DEFINE_PRINT_KV(35)
DEFINE_PRINT_KV(36)
DEFINE_PRINT_KV(37)
DEFINE_PRINT_KV(38)
DEFINE_PRINT_KV(39)
DEFINE_PRINT_KV(40)
DEFINE_PRINT_KV(41)
DEFINE_PRINT_KV(42)
DEFINE_PRINT_KV(43)
DEFINE_PRINT_KV(44)
DEFINE_PRINT_KV(45)
DEFINE_PRINT_KV(46)
DEFINE_PRINT_KV(47)
DEFINE_PRINT_KV(48)
DEFINE_PRINT_KV(49)
DEFINE_PRINT_KV(50)
DEFINE_PRINT_KV(51)
DEFINE_PRINT_KV(52)
DEFINE_PRINT_KV(53)
DEFINE_PRINT_KV(54)
DEFINE_PRINT_KV(55)
DEFINE_PRINT_KV(56)
DEFINE_PRINT_KV(57)
DEFINE_PRINT_KV(58)
DEFINE_PRINT_KV(59)
DEFINE_PRINT_KV(60)
DEFINE_PRINT_KV(61)
DEFINE_PRINT_KV(62)
DEFINE_PRINT_KV(63)
DEFINE_PRINT_KV(64)
} // end namespce common
} // end namespace oceanbase

#endif /* LIB_UTILITY_OB_PRINT_KV_ */