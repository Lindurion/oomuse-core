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
 * Provides CONSTEXPR_ASSERT() macro to work around difficulty of using assert()
 * in C++11 constexpr functions. This macro will be obsolete once the N3652
 * "Relaxing requirements on constexpr functions" C++14 language feature is
 * supported by all major compilers.
 *
 * This workaround is based on a technique from
 * http://ericniebler.com/2014/09/27/assert-and-constexpr-in-cxx11/.
 */

#ifndef OOMUSE_CORE_CONSTEXPR_ASSERT_H
#define OOMUSE_CORE_CONSTEXPR_ASSERT_H

#include <cassert>

namespace oomuse {
  /** Helper error class that invokes a given function in its constructor. */
  struct ConstexprAssertFailure {
    template<typename Fn>
    explicit ConstexprAssertFailure(Fn errorFn) {
      errorFn();
    }
  };
}


#ifndef CONSTEXPR_ASSERT

#ifdef NDEBUG
  /**
   * Allows use of assert() within a C++11 constexpr function, outputting the
   * correct file and line number where the assertion occurred. Sample usage
   * that returns x / y after asserting that y > 0:
   *
   * constexpr int divByPositive(int x, int y) {
   *   return CONSTEXPR_ASSERT(y > 0, "y must be positive", x / y);
   * }
   */
  #define CONSTEXPR_ASSERT(condition, failureMessage, result) (result)
#else
  /**
   * Allows use of assert() within a C++11 constexpr function, outputting the
   * correct file and line number where the assertion occurred. Sample usage
   * that returns x / y after asserting that y > 0:
   *
   * constexpr int divByPositive(int x, int y) {
   *   return CONSTEXPR_ASSERT(y > 0, "y must be positive", x / y);
   * }
   */
  #define CONSTEXPR_ASSERT(condition, failureMessage, result)  \
      (condition) ? (result)                                   \
                  : throw oomuse::ConstexprAssertFailure([] {  \
                      assert(!(failureMessage));               \
                    })
#endif  // NDEBUG

#endif  // CONSTEXPR_ASSERT


#endif  // OOMUSE_CORE_CONSTEXPR_ASSERT_H
