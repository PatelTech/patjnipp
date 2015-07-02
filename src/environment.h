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


#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include <jni.h>
#include <mutex>
#include <vector>
#include <string>


namespace jnipp {
inline namespace v1 {

class Environment {
public:
  Environment(JNIEnv& env);
  Environment(JNIEnv* env);
  ~Environment();

  jfloatArray          floatArray(const std::vector<float>& vect);
  std::vector<float>   floatArray(jfloatArray vect);

  jintArray     intArray(const std::vector<int>& vect);
  jobjectArray  stringArray(const std::vector<std::string>& vect);

  jclass       klass(std::string str);

  jstring      string(const std::string& str);
  std::string  string(const jstring str);

  // Throwers!
  jint throwNoClassDefError(const std::string& message);
  jint throwNoSuchFieldError(const std::string& message);
  jint throwNullPointerException(const std::string& message);
  jint throwException(const std::string& classstr, const std::string& message);

  bool error();

  JavaVM* getJVM();
  JNIEnv* getEnv();

public: // Classes
  jclass  JStringClass();

public:
  template <typename T> jlong   toLong(std::shared_ptr<T> in) {
    auto ptr = in.get();
    in.reset();
    return toLong(ptr);
  }

  template <typename T> jlong   toLong(T* in) {
    return reinterpret_cast<jlong>(in);
  }


  template <typename T>
  T* toType(jlong handle) {
    if (!handle) {
      throwNullPointerException("Invalid handle provided.");
      return NULL;
    }
    return reinterpret_cast<T*>(handle);
  }
private:
  // Specifically prevent all implicit constructors we this object isn't kept around.
  Environment(const Environment&);
  Environment(Environment&&);

protected:
  JNIEnv& JEnv;

protected:
  // FindClass cache
  jclass StringClass;
};

/* Use when entering the JNI */
class EnvironmentLocked : public Environment {
public:
  EnvironmentLocked(JNIEnv& env);
  EnvironmentLocked(JNIEnv* env);
  ~EnvironmentLocked();

protected:
  static std::mutex GlobalMutex;
};

}
}

#endif // __ENVIRONMENT_H__