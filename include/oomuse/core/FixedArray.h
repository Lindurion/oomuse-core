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

#ifndef OOMUSE_CORE_FIXEDARRAY_H
#define OOMUSE_CORE_FIXEDARRAY_H

#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <type_traits>

#include "oomuse/core/int_types.h"
#include "oomuse/core/readability_macros.h"

namespace oomuse {


/**
 * A fixed-length (runtime determined) array. Memory is new[] and delete[]
 * managed by default, but the allocator can be customized supplying another
 * template argument.
 */
template<typename T, typename Allocator = std::allocator<T>>
class FixedArray {
 public:
  /** Type of element this holds. */
  using value_type = T;

  /**
   * Constructs a new FixedArray of the given length. Elements of plain-old-data
   * (POD) types will be initialized to default 0/0.0/false/nullptr values.
   */
  explicit FixedArray(std::size_t length) : length_(length) {
    assert(length >= 0);
    data_ = std::allocator_traits<Allocator>::allocate(
        allocator_, length_, this /* hint for memory locality */);
    for (std::size_t i = 0; i < length_; ++i) {
      std::allocator_traits<Allocator>::construct(allocator_, &data_[i]);
    }
  }

  /** See SKIP_DEFAULT_INIT constant below. */
  enum class SkipDefaultInit {YES};

  /**
   * Pass this constant in as a second constructor param to skip default
   * initialization for POD types.
   */
  static const SkipDefaultInit SKIP_DEFAULT_INIT = SkipDefaultInit::YES;

  /**
   * Constructs a new FixedArray of the given length. Use this for elements of
   * plain-old-data (POD) types to skip initialization to default
   * 0/0.0/false/nullptr values.
   */
  FixedArray(std::size_t length, SkipDefaultInit) : length_(length) {
    assert(length >= 0);
    assert(std::is_pod<T>::value);  // Shouldn't skip constructors unless POD.
    data_ = std::allocator_traits<Allocator>::allocate(
        allocator_, length_, this /* hint for memory locality */);
    // Skip default initialization of elements.
  }

  /** Constructs a new FixedArray containing the given elements. */
  FixedArray(std::initializer_list<T> initElements)
      : length_(initElements.size()) {
    data_ = std::allocator_traits<Allocator>::allocate(
        allocator_, length_, this /* hint for memory locality */);
    if (length_ > 0) {
      std::uninitialized_copy(initElements.begin(), initElements.end(),
                              &data_[0]);
    }
  }

  /** Moves another FixedArray into this newly constructed one. */
  FixedArray(FixedArray<T>&& other)
      : data_(other.data_), length_(other.length_) {
    other.data_ = nullptr;
    other.length_ = 0;
  }

  /** Cleans up this object and moves another FixedArray into it. */
  FixedArray<T>& operator=(FixedArray<T>&& other) {
    // Clean up this existing array.
    cleanUp();

    // Move.
    data_ = other.data_;
    length_ = other.length_;

    // Clean up other FixedArray.
    other.data_ = nullptr;
    other.length_ = 0;
  }

  /**
   * Destructs this FixedArray, cleaning up allocated memory and calling
   * individual element destructors.
   */
  ~FixedArray() { cleanUp(); }

  /** Returns the raw data pointer to the underlying T[] array. */
  T* data() { return data_; }

  /** Returns const pointer to raw data in underlying T[] array. */
  const T* data() const { return data_; }

  /** Returns the number of elements in this FixedArray. */
  std::size_t length() const { return length_; }

  /** Returns a reference to the element at the given index. */
  T& operator[](std::size_t index) { return data_[index]; }

  /** Returns a const reference to the element at the given index. */
  const T& operator[](std::size_t index) const { return data_[index]; }

  /** Returns pointer to the first element, for iteration. */
  T* begin() { return &data_[0]; }

  /** Returns const pointer to the first element, for iteration. */
  const T* begin() const { return &data_[0]; }

  /** Returns pointer to one past the last element, for iteration. */
  T* end() { return &data_[length_]; }

  /** Returns const pointer to one past the last element, for iteration. */
  const T* end() const { return &data_[length_]; }

 private:
  CANT_COPY(FixedArray);

  void cleanUp() {
    // Call destructors in reverse order (to be consistent with delete[]).
    const T* beyondBegin = &data_[0] - 1;
    for (T* element = &data_[length_ - 1]; element != beyondBegin; --element) {
      std::allocator_traits<Allocator>::destroy(allocator_, element);
    }

    // Free memory.
    std::allocator_traits<Allocator>::deallocate(allocator_, data_, length_);
  }

  T* data_;
  std::size_t length_;

  Allocator allocator_;
};


/** Considers two FixedArrays equal if they are element-wise ==. */
template<typename U, typename V>
bool operator==(const FixedArray<U>& fixedArray1,
                const FixedArray<V>& fixedArray2) {
  std::size_t length = fixedArray1.length();
  if (length != fixedArray2.length()) {
    return false;
  }

  for (std::size_t i = 0; i < length; ++i) {
    if (!(fixedArray1[i] == fixedArray2[i])) {
      return false;
    }
  }

  return true;
}


/** Considers two FixedArrays non-equal if they aren't element-wise ==. */
template<typename U, typename V>
bool operator!=(const FixedArray<U>& fixedArray1,
                const FixedArray<V>& fixedArray2) {
  return !(fixedArray1 == fixedArray2);
}


}  // namespace oomuse

#endif  // OOMUSE_CORE_FIXEDARRAY_H
