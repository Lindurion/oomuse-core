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
 */

#ifndef OOMUSE_CORE_VALIDATOR_H
#define OOMUSE_CORE_VALIDATOR_H

#include <string>

namespace oomuse {


/** Interface for a value validator. See Validators for common cases. */
template<typename T>
class Validator {
 public:
  virtual ~Validator() {}

  /** Returns non-empty validation error if invalid, empty string if ok. */
  virtual std::string checkValidationErrors(const T& value) const = 0;
};


}  // namespace oomuse

#endif  // OOMUSE_CORE_VALIDATOR_H
