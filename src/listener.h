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


#ifndef __LISTENER_H__
#define __LISTENER_H__


#include <jni.h>
#include <mutex>
#include <vector>
#include <string>
#include <iostream>

#include "environment.h"
#include "exceptions.h"
#include "jtypes.h"

namespace jnipp {
inline namespace v1 {

template <typename T>
class Listener {
public:
  Listener(Environment& environ, T nativeObject, jobject listener):
    JVM(environ.getJVM()),
    NativeObject(nativeObject)
  {
    if (auto env = environ.getEnv()) ListenerObj = env->NewGlobalRef(listener);
  }

  ~Listener() {
    if (auto env = getEnv()) env->DeleteGlobalRef(ListenerObj);
  }

  JNIEnv*
  getAttachedEnv() {
    JNIEnv* env = NULL;

    int stat = JVM->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);

    //We are on a different thread, try to attach.
    if (stat == JNI_EDETACHED)
      JVM->AttachCurrentThread(reinterpret_cast<void **>(&env), NULL);

    //Can't attach to java, bail.
    if (env->ExceptionCheck() == JNI_TRUE)
      return NULL;

    return env;
  }

  JNIEnv* getEnv() { return getAttachedEnv(); }

  template <typename R, typename... A>
  void printMethodInfo(const std::string& method, A... args) {
    std::string method_sig = JMethod<R, A...>();
    std::cout << " Method Name: " << method << std::endl;
    std::cout << " Method Sig: " << method_sig << std::endl;
    std::cout << "  Arguments: ";
    printArgs(std::cout, args...);
    std::cout << std::endl;
  }

  template <typename R, typename... A>
  R call(const std::string& method_name, A... args) {
    std::string method_sig = JMethod<R, A...>();
    if (!JVM) return R();

    JNIEnv* env = getAttachedEnv();
    if (!env)                   throw InvalidListener("Invalid JNI Environment provided.");
    if (!ListenerObj)           throw InvalidListener("No object associated with this listener.");

    jclass klass = env->GetObjectClass(ListenerObj);
    if (env->ExceptionCheck())  throw InvalidListener("Unable to find listener object Java class.");

    jmethodID method = env->GetMethodID(klass, method_name.c_str(), method_sig.c_str());
    if (env->ExceptionCheck()) {
      std::string message = "Unable to find specified method:"
        "\n Method Name: " + method_name +
        "\n Method Sig : " + method_sig +
        "\n";
      throw InvalidListener(message);
    }

    return callMethod<R>(env, method, args...);
  }

  template <typename R, typename... A>
  struct JCallMethod {
    R operator()(JNIEnv* env, jobject listener, A... args) {
      throw UnhandledMethod(typeid(R).name());
    }
  };

#define JAVA_METHOD_CALLERS(method, jklass)                             \
template <typename... A>                                                \
struct JCallMethod<jklass, A...> {                                      \
  jklass operator()(JNIEnv* env, jobject listener, A... args) {         \
    return env->Call ## method ## Method(listener, args...);            \
  }                                                                     \
};

  JAVA_METHOD_CALLERS(Boolean, jboolean);
  JAVA_METHOD_CALLERS(Void,    void);
  JAVA_METHOD_CALLERS(Int,     jint);
  JAVA_METHOD_CALLERS(Long,    jlong);
  JAVA_METHOD_CALLERS(Object,  jstring);

  template <typename R, typename... A>
  R callMethod(JNIEnv* env, A... args) {
    return JCallMethod<R, A...>()(env, ListenerObj, args...);
  }

protected:
  JavaVM* JVM;
  jobject ListenerObj;

  T NativeObject;
};

}
}

#endif // __LISTENER_H__

