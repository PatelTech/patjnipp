/*****************************PROPRIETARY*********************************
 * Copyright (C) 2015 - Patel Technologies Corporation
 * All Rights Reserved.
 *************************************************************************
 * Authors: Terry Meacham (terry.meacham@pateltechnologies.com)
 *          Patel Technologies Corporation
 *************************************************************************
 * NOTICE:  All information contained herein is, and remains
 * the property of Patel Technologies Incorporated and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to Patel Technologies Incorporated
 * and its suppliers and may be covered by U.S. and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information, reproduction of this material, or
 * modification of this notice in any way, to include removal of contributor
 * information, is strictly forbidden unless prior written permission is
 * obtained from Patel Technologies.
 *************************************************************************/


#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include <string>

namespace jnipp {
inline namespace v1 {

struct BaseException {
  BaseException(const std::string& msg): Msg(msg) {};
  std::string getMessage() { return Msg; }
protected:
  std::string Msg;
};

#define DEFINE_EXCEPTION(name) \
  struct name : public BaseException { name(const std::string& msg): BaseException(msg) {}; };

DEFINE_EXCEPTION(UnhandledType);
DEFINE_EXCEPTION(UnhandledMethod);
DEFINE_EXCEPTION(InvalidListener);

}
}

#endif // __EXCEPTIONS_H__