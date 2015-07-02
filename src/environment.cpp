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


#include "environment.h"

namespace jnipp {
inline namespace v1 {

EnvironmentLocked::EnvironmentLocked(JNIEnv& env):
  Environment(env)
{
  GlobalMutex.lock();
}

EnvironmentLocked::EnvironmentLocked(JNIEnv* env):
  Environment(env)
{
  GlobalMutex.lock();
}

EnvironmentLocked::~EnvironmentLocked() {
  GlobalMutex.unlock();
}


/** PRIVATE **/
Environment::Environment(const Environment& e):
  JEnv(e.JEnv),
  StringClass(NULL)
{}

Environment::Environment(Environment&& e):
  JEnv(e.JEnv),
  StringClass(NULL)
{}

Environment::~Environment()
{}
/*************/

/**
 * RAII: Grab the Environment::GlobalMutex.
 */
Environment::Environment(JNIEnv& env):
  JEnv(env),
  StringClass(NULL)
{}

/**
 * RAII: Grab the Environment::GlobalMutex.
 */
Environment::Environment(JNIEnv* env):
  JEnv(*env),
  StringClass(NULL)
{}

/**
 * Get the current JVM.
 */
JavaVM*
Environment::getJVM() {
  JavaVM* vm = NULL;

  JEnv.GetJavaVM(&vm);
  if (error()) return NULL;

  return vm;
}

JNIEnv*
Environment::getEnv() {
  return &JEnv;
}

/**
 * Generate a jfloatArray from a std::vector<float>.
 * @param vect Float vector convert to a jfloatArray.
 */
jfloatArray
Environment::floatArray(const std::vector<float>& vect) {

  jfloatArray result = JEnv.NewFloatArray(vect.size());
  if (result == NULL) return NULL;

  // move from the temp structure to the java structure
  JEnv.SetFloatArrayRegion(result, 0, vect.size(), &vect[0]);
  if (error()) return NULL;
  return result;
}

/**
 * Generate a jfloatArray from a std::vector<float>.
 * @param vect Float vector convert to a jfloatArray.
 */
std::vector<float>
Environment::floatArray(jfloatArray fa) {
  if (fa == NULL) return std::vector<float>();

  int len = JEnv.GetArrayLength(fa);
  std::vector<float> result(len);

  // This shouldn't be an issue: float == jfloat (typedef), but
  // if for some reason that changes, the size of the float may
  // result in errors.
  JEnv.GetFloatArrayRegion(fa, 0, len, &result[0]);
  return result;
}

/**
 * Generate a jintArray from a std::vector<int>.
 * @param vect Int vector convert to a jintArray.
 */
jintArray
Environment::intArray(const std::vector<int>& vect) {

  jintArray result = JEnv.NewIntArray(vect.size());
  if (!result) return NULL;

  // move from the temp structure to the java structure
  JEnv.SetIntArrayRegion(result, 0, vect.size(), &vect[0]);
  if (error()) return NULL;
  return result;
}

/**
 * Generate a jobjectArray from a std::vector<std::string>.
 * @param vect String vector convert to a jobjectArray.
 */
jobjectArray
Environment::stringArray(const std::vector<std::string>& vect) {

  jobjectArray result = JEnv.NewObjectArray(vect.size(), JStringClass(), string(""));
  if (!result) return NULL;

  int idx = 0;
  for (const std::string& str: vect) {
    JEnv.SetObjectArrayElement(result, idx++, string(str));
    if (error())
      return NULL;
  }

  return result;
}

bool
Environment::error() {
  return (JEnv.ExceptionCheck() == JNI_TRUE);
}

jclass
Environment::klass(std::string str) {
  std::string prefix = "java/lang/";

  if (str.substr(0, 10) != prefix)
    str.insert(0, prefix);

  auto kls = JEnv.FindClass(str.c_str());

  if (error()) return NULL;
  return kls;
}

jstring
Environment::string(const std::string& str) {
  return JEnv.NewStringUTF(str.c_str());
}

std::string
Environment::string(const jstring javaString) {
  const char *nativeString = JEnv.GetStringUTFChars(javaString, 0);

  std::string result(nativeString);

  JEnv.ReleaseStringUTFChars(javaString, nativeString);
  return result;
}

// Get and store the Java String class.
jclass
Environment::JStringClass() {
  if (StringClass) return StringClass;

  StringClass = klass("String");
  return StringClass;
}

jint
Environment::throwNullPointerException(const std::string& message) {
  return throwException("NullPointerException", message.c_str());
}

jint
Environment::throwNoClassDefError(const std::string& message) {
  return throwException("NoClassDefFoundError", message.c_str());
}

jint
Environment::throwException(const std::string& classstr, const std::string& message) {
  auto exception = klass(classstr);
  if (!exception) return -1;
  return JEnv.ThrowNew(exception, message.c_str());
}

jint
Environment::throwNoSuchFieldError(const std::string& message) {
  return throwException("NoSuchFieldError", message.c_str());
}


// We'll need to tighten this down as the management constructs fall into place...
std::mutex EnvironmentLocked::GlobalMutex;

};
};