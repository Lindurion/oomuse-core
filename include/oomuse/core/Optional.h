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

#ifndef OOMUSE_CORE_OPTIONAL_H
#define OOMUSE_CORE_OPTIONAL_H

#include <cassert>

#include "oomuse/core/int_types.h"

namespace oomuse {


/** Base template class for Optional (don't use directly). */
template<uint64 numBytes>
class OptionalStorage {
 private:
  // Set Optional as a friend so that only it can construct this type.
  template<typename T>
  friend class Optional;

  OptionalStorage() : hasValue_(false) {}

  uint8 data_[numBytes];
  bool hasValue_;
};


/**
 * Represents an optional value, which may not be present. The value is stored
 * within this Optional object (via placement new).
 *
 * This class is inspired by boost's optional type and the optional type
 * discussed in Mike McShaffry's Game Coding Complete, 3rd Edition.
 *
 * Note that this class will be deprecated in the future in favor of
 * std::optional, once that gets fully incorporated into the C++ Standard
 * Library.
 */
template<typename T>
class Optional : private OptionalStorage<sizeof(T)> {
 public:
  /** Type of element this holds. */
  using value_type = T;

  /** Constructs a new Optional with no value set. */
  Optional() {}

  /** Constructs a new Optional with the given value set. */
  Optional(const T& value) { setValue(value); }

  ~Optional() { clear(); }

  /** Returns true if this has a value set. */
  bool hasValue() const { return this->hasValue_; }

  /** Returns constant value. Should only be called if hasValue(). */
  const T& value() const {
    assert(this->hasValue_);
    return *getValuePtr();
  }

  /** Returns value. Should only be called if hasValue(). */
  T& value() {
    assert(this->hasValue_);
    return *getValuePtr();
  }

  /** Clears the existing value, if any. */
  void clear() {
    if (this->hasValue_) {
      // Placement new requires calling destructor explicitly.
      getValuePtr()->~T();
      this->hasValue_ = false;
    }
  }

  /** Assigns a new value. */
  Optional<T>& operator=(const T& value) {
    clear();
    setValue(value);
    return *this;
  }

 private:
  void setValue(const T& value) {
    // Use placement new to construct value in data_[].
    new (getValuePtr()) T(value);
    this->hasValue_ = true;
  }

  const T* getValuePtr() const {
    return reinterpret_cast<const T*>(this->data_);
  }

  T* getValuePtr() { return reinterpret_cast<T*>(this->data_); }
};


/** Returns true if the given Optional values are equal. */
template<typename T, typename U>
bool operator==(const Optional<T>& a, const Optional<U>& b) {
  return (a.hasValue() == b.hasValue())
      && (!a.hasValue() || (a.value() == b.value()));
}


/** Returns true if the first Optional value is less than the second. */
template<typename T, typename U>
bool operator<(const Optional<T>& a, const Optional<U>& b) {
  // If both have values, compare them.
  if (a.hasValue() && b.hasValue()) {
    return (a.value() < b.value());
  }

  // If both are empty, use memory addresses for stable comparison.
  if (!a.hasValue() && !b.hasValue()) {
    return (&a < &b);
  }

  // Otherwise, treat the missing value as less than the present one.
  return b.hasValue();
}


}  // namespace oomuse

#endif  // OOMUSE_CORE_OPTIONAL_H
