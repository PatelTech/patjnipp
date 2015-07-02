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


#include "listener.h"

namespace jnipp {
inline namespace v1 {

#define JAVA_BASE_TYPE_TEMPLATES_NA(jklass, label)                        \
  template <> std::string JType<jklass>() { return label; }

#define JAVA_BASE_TYPE_TEMPLATES(klass, jklass, label)                    \
  template <> std::string JType<jklass ## Array>() { return "[" label; }  \
  JAVA_BASE_TYPE_TEMPLATES_NA(klass, label)

// No need to respecify becuase base types are purely typedefs.
// template <> std::string JType<klass>() { return label; }

// Map all values to their respective letters...
JAVA_BASE_TYPE_TEMPLATES_NA(void,     "V");

// JAVA_BASE_TYPE_TEMPLATES(jchar,    jbyte,    "B");
JAVA_BASE_TYPE_TEMPLATES(jint,     jint,     "I");
JAVA_BASE_TYPE_TEMPLATES(jchar,    jchar,    "C");
JAVA_BASE_TYPE_TEMPLATES(jlong,    jlong,    "J");
JAVA_BASE_TYPE_TEMPLATES(jfloat,   jfloat,   "F");
JAVA_BASE_TYPE_TEMPLATES(jshort,   jshort,   "S");
JAVA_BASE_TYPE_TEMPLATES(jdouble,  jdouble,  "D");
JAVA_BASE_TYPE_TEMPLATES(jboolean, jboolean, "Z");
JAVA_BASE_TYPE_TEMPLATES(jstring,  jobject,  "Ljava/lang/String;");

}
}