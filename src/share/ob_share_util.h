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

#ifndef OCEANBASE_SHARE_OB_SHARE_UTIL_H_
#define OCEANBASE_SHARE_OB_SHARE_UTIL_H_
#include "share/ob_define.h"
namespace oceanbase
{
namespace common
{
class ObTimeoutCtx;
}
namespace share
{
class ObShareUtil
{
public:
  // priority to set timeout_ctx: ctx > worker > default_timeout
  static int set_default_timeout_ctx(common::ObTimeoutCtx &ctx, const int64_t default_timeout);
  // priority to get timeout: ctx > worker > default_timeout
  static int get_abs_timeout(const int64_t default_timeout, int64_t &abs_timeout);
};
}//end namespace share
}//end namespace oceanbase
#endif //OCEANBASE_SHARE_OB_SHARE_UTIL_H_
