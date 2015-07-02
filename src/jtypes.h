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


#ifndef __JTYPES_H__
#define __JTYPES_H__

#include <jni.h>
#include <string>

#include "exceptions.h"

namespace jnipp {
inline namespace v1 {

template <typename T>
std::string JType() {
  throw UnhandledType(typeid(T).name());
}

/********* JNI Method ID magic. *********\
  Automatically generate the
  required method signature
  on demand.
\****************************************/
template <typename... T> struct JMethodArgs;

template<>
struct JMethodArgs<> {
  std::string operator()() {
    return std::string();
  }
};

template <typename F, typename... T>
struct JMethodArgs<F, T...> {
  std::string operator()() {
    return JType<F>() + JMethodArgs<T...>()();
  }
};

/**
 * Generate the method signature for a return value and
 * the template parameters provided.
 */
template <typename R, typename... T>
std::string JMethod() {
  return std::string("(") + JMethodArgs<T...>()() + ")" + JType<R>();
}

/**** Template Variadic Print Methods ****/
template <typename... T> struct PrintArgs;

/**
 * Print the args provided to the given ostream.
 * @param st Ostream object to write arguments to.
 * @param args... Packed variadic template argument to be unpacked and printed.
 */
template <typename... S>
void printArgs(std::ostream& st, S... args) {
  PrintArgs<S...>()(st, args...); // This recurses with printArgs.
}

template<typename F>
struct PrintArgs<F> {
  void operator()(std::ostream& st, F f) {
    st << f << ' ';
  }
};

template <typename F, typename... S>
struct PrintArgs<F, S...> {
  void operator()(std::ostream& st, F f, S... args) {
    st << f << ' ';
    printArgs(st, args...);
  }
};

}
}

#endif // __JTYPES_H__