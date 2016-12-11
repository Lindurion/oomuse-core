/**
 * Copyright 2015 Eric W. Barndollar. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * =============================================================================
 * Macros to enhance the readability of the C++ language.
 */

#ifndef OOMUSE_CORE_READABILITY_MACROS_H
#define OOMUSE_CORE_READABILITY_MACROS_H


#ifndef CANT_INSTANTIATE
  /** Prevents instantiation of a class (place in private: section). */
  #define CANT_INSTANTIATE(ClassName)  ClassName() = delete
#endif


#ifndef CANT_REASSIGN
  /** Prevents copy and move assignment (place in private: section). */
  #define CANT_REASSIGN(ClassName)  \
      ClassName& operator=(const ClassName& other) = delete;  \
      ClassName& operator=(ClassName&& other) = delete
#endif


#ifndef CANT_COPY
  /** Prevents copy construction/assignment (place in private: section). */
  #define CANT_COPY(ClassName)  \
      ClassName(const ClassName& other) = delete;  \
      ClassName& operator=(const ClassName& other) = delete
#endif


#ifndef CANT_MOVE
  /** Prevents move construction/assignment (place in private: section). */
  #define CANT_MOVE(ClassName)  \
      ClassName(ClassName&& other) = delete;  \
      ClassName& operator=(ClassName&& other) = delete
#endif


#ifndef CALL_MEMBER_FN
  /**
   * Invokes pointer-to-member function, as recommended in Marshall Cline's
   * C++ FAQ (see http://www.parashift.com/c++-faq/).
   */
  #define CALL_MEMBER_FN(obj, memberPtr)  ((obj).*(memberPtr))
#endif


#ifndef UNREF_PARAM
  /**
   * Uses parameter x as a no-op, in order to suppress unused paramaeter
   * compiler warnings. This should usually only be used when a paramater
   * is used only in some circumstances, based on preprocessor definitions (such
   * as NDEBUG, in assertions).
   *
   * If the unused parameter is part of a CONSTEXPR_ASSERT() condition, for
   * example, can use this macro along with the comma operator in the result
   * argument:
   *
   * return CONSTEXPR_ASSERT(y > 0, "needs y > 0, even though it returns x",
   *                         (UNREF_PARAM(y), x));
   */
  #define UNREF_PARAM(x)  ((void) x)
#endif


#endif  // OOMUSE_CORE_READABILITY_MACROS_H
