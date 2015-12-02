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

#ifndef OOMUSE_CORE_VALIDATORS_H
#define OOMUSE_CORE_VALIDATORS_H

#include <cassert>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "oomuse/core/Validator.h"
#include "oomuse/core/readability_macros.h"

namespace oomuse {


/**
 * Static factory functions for creating common Validator instances.
 * For example, these can be useful for validating command-line flags.
 */
template<typename T>
class Validators {
 public:
  /** Returns a Validator requiring all values to be < value. */
  static std::unique_ptr<Validator<T>> less(const T& value);

  /** Returns a Validator requiring all values to be <= value. */
  static std::unique_ptr<Validator<T>> lessOrEqual(const T& value);

  /** Returns a Validator requiring all values to be > value. */
  static std::unique_ptr<Validator<T>> greater(const T& value);

  /** Returns a Validator requiring all values to be >= value. */
  static std::unique_ptr<Validator<T>> greaterOrEqual(const T& value);

  /** Returns a Validator requiring all values to be == value. */
  static std::unique_ptr<Validator<T>> equal(const T& value);

  /** Returns a Validator requiring all values to be != value. */
  static std::unique_ptr<Validator<T>> notEqual(const T& value);

  /** Returns a Validator requiring all values to have size() <= size. */
  static std::unique_ptr<Validator<T>> sizeLessOrEqual(std::size_t size);

  /** Returns a Validator requiring all values to have size() >= size. */
  static std::unique_ptr<Validator<T>> sizeGreaterOrEqual(std::size_t size);

  /** Returns a Validator that is valid if any of the given validators pass. */
  static std::unique_ptr<Validator<T>> anyOf(
      std::unique_ptr<Validator<T>> validator1,
      std::unique_ptr<Validator<T>> validator2);

  /** Returns a Validator that is valid if any of the given validators pass. */
  static std::unique_ptr<Validator<T>> anyOf(
      std::unique_ptr<Validator<T>> validator1,
      std::unique_ptr<Validator<T>> validator2,
      std::unique_ptr<Validator<T>> validator3);

  /** Returns a Validator that is valid if all of the given validators pass. */
  static std::unique_ptr<Validator<T>> allOf(
      std::unique_ptr<Validator<T>> validator1,
      std::unique_ptr<Validator<T>> validator2);

  /** Returns a Validator that is valid if all of the given validators pass. */
  static std::unique_ptr<Validator<T>> allOf(
      std::unique_ptr<Validator<T>> validator1,
      std::unique_ptr<Validator<T>> validator2,
      std::unique_ptr<Validator<T>> validator3);

 private:
  CANT_INSTANTIATE(Validators);
};


/** Validates that a given value is < some fixed value. */
template<typename T>
class LessValidator : public Validator<T> {
 public:
  explicit LessValidator(const T& value) : value_(value) {}

  virtual std::string checkValidationErrors(const T& value) const {
    if (value < value_) {
      return "";
    }

    std::stringstream validationError;
    validationError << "Must be less than " << value_ << ".";
    return validationError.str();
  }

 private:
  const T value_;
};


/** Validates that a given value is <= some fixed value. */
template<typename T>
class LessOrEqualValidator : public Validator<T> {
 public:
  explicit LessOrEqualValidator(const T& value) : value_(value) {}

  virtual std::string checkValidationErrors(const T& value) const {
    if (value <= value_) {
      return "";
    }

    std::stringstream validationError;
    validationError << "Must be less than or equal to " << value_ << ".";
    return validationError.str();
  }

 private:
  const T value_;
};


/** Validates that a given value is > some fixed value. */
template<typename T>
class GreaterValidator : public Validator<T> {
 public:
  explicit GreaterValidator(const T& value) : value_(value) {}

  virtual std::string checkValidationErrors(const T& value) const {
    if (value > value_) {
      return "";
    }

    std::stringstream validationError;
    validationError << "Must be greater than " << value_ << ".";
    return validationError.str();
  }

 private:
  const T value_;
};


/** Validates that a given value is >= some fixed value. */
template<typename T>
class GreaterOrEqualValidator : public Validator<T> {
 public:
  explicit GreaterOrEqualValidator(const T& value) : value_(value) {}

  virtual std::string checkValidationErrors(const T& value) const {
    if (value >= value_) {
      return "";
    }

    std::stringstream validationError;
    validationError << "Must be greater than or equal to " << value_ << ".";
    return validationError.str();
  }

 private:
  const T value_;
};


/** Validates that a given value is == some fixed value. */
template<typename T>
class EqualValidator : public Validator<T> {
 public:
  explicit EqualValidator(const T& value) : value_(value) {}

  virtual std::string checkValidationErrors(const T& value) const {
    if (value == value_) {
      return "";
    }

    std::stringstream validationError;
    validationError << "Must be equal to " << value_ << ".";
    return validationError.str();
  }

 private:
  const T value_;
};


/** Validates that a given value is != some fixed value. */
template<typename T>
class NotEqualValidator : public Validator<T> {
 public:
  explicit NotEqualValidator(const T& value) : value_(value) {}

  virtual std::string checkValidationErrors(const T& value) const {
    if (value != value_) {
      return "";
    }

    std::stringstream validationError;
    validationError << "Must not be equal to " << value_ << ".";
    return validationError.str();
  }

 private:
  const T value_;
};


/** Validates that a given value has size() <= some fixed value. */
template<typename T>
class SizeLessOrEqualValidator : public Validator<T> {
 public:
  explicit SizeLessOrEqualValidator(std::size_t size) : size_(size) {}

  virtual std::string checkValidationErrors(const T& value) const {
    if (value.size() <= size_) {
      return "";
    }

    std::stringstream validationError;
    validationError << "Size/length must be less than or equal to "
                    << size_ << ".";
    return validationError.str();
  }

 private:
  const std::size_t size_;
};


/** Validates that a given value has size() >= some fixed value. */
template<typename T>
class SizeGreaterOrEqualValidator : public Validator<T> {
 public:
  explicit SizeGreaterOrEqualValidator(std::size_t size) : size_(size) {}

  virtual std::string checkValidationErrors(const T& value) const {
    if (value.size() >= size_) {
      return "";
    }

    std::stringstream validationError;
    validationError << "Size/length must be greater than or equal to "
                    << size_ << ".";
    return validationError.str();
  }

 private:
  const std::size_t size_;
};


/** Validates that any one of a fixed set of validators passes. */
template<typename T>
class OrValidator : public Validator<T> {
 public:
  using UniqueValidator = std::unique_ptr<Validator<T>>;

  explicit OrValidator(std::vector<UniqueValidator>&& validators)
      : validators_(std::forward<std::vector<UniqueValidator>>(validators)) {
    assert(!validators_.empty());
  }

  virtual std::string checkValidationErrors(const T& value) const {
    std::stringstream validationErrors;
    validationErrors << "Requires one of:" << std::endl;

    for (auto& validator : validators_) {
      std::string error = validator->checkValidationErrors(value);
      if (error.empty()) {
        return "";  // OK, at least one validator passed.
      }
      validationErrors << "  " << error << std::endl;
    }

    return validationErrors.str();
  }

 private:
  std::vector<UniqueValidator> validators_;
};


/** Validates that every one of a fixed set of validators passes. */
template<typename T>
class AndValidator : public Validator<T> {
 public:
  using UniqueValidator = std::unique_ptr<Validator<T>>;

  explicit AndValidator(std::vector<UniqueValidator>&& validators)
      : validators_(std::forward<std::vector<UniqueValidator>>(validators)) {
    assert(!validators_.empty());
  }

  virtual std::string checkValidationErrors(const T& value) const {
    std::stringstream validationErrors;

    for (auto& validator : validators_) {
      std::string error = validator->checkValidationErrors(value);
      if (!error.empty()) {
        validationErrors << error << std::endl;
      }
    }

    return validationErrors.str();
  }

 private:
  std::vector<UniqueValidator> validators_;
};


template<typename T>
std::unique_ptr<Validator<T>> Validators<T>::less(const T& value) {
  return std::make_unique<LessValidator<T>>(value);
}


template<typename T>
std::unique_ptr<Validator<T>> Validators<T>::lessOrEqual(const T& value) {
  return std::make_unique<LessOrEqualValidator<T>>(value);
}


template<typename T>
std::unique_ptr<Validator<T>> Validators<T>::greater(const T& value) {
  return std::make_unique<GreaterValidator<T>>(value);
}


template<typename T>
std::unique_ptr<Validator<T>> Validators<T>::greaterOrEqual(const T& value) {
  return std::make_unique<GreaterOrEqualValidator<T>>(value);
}


template<typename T>
std::unique_ptr<Validator<T>> Validators<T>::equal(const T& value) {
  return std::make_unique<EqualValidator<T>>(value);
}


template<typename T>
std::unique_ptr<Validator<T>> Validators<T>::notEqual(const T& value) {
  return std::make_unique<NotEqualValidator<T>>(value);
}


template<typename T>
std::unique_ptr<Validator<T>> Validators<T>::sizeLessOrEqual(std::size_t size) {
  return std::make_unique<SizeLessOrEqualValidator<T>>(size);
}


template<typename T>
std::unique_ptr<Validator<T>> Validators<T>::sizeGreaterOrEqual(
    std::size_t size) {
  return std::make_unique<SizeGreaterOrEqualValidator<T>>(size);
}


template<typename T>
std::unique_ptr<Validator<T>> Validators<T>::anyOf(
    std::unique_ptr<Validator<T>> validator1,
    std::unique_ptr<Validator<T>> validator2) {
  std::vector<std::unique_ptr<Validator<T>>> validators;
  validators.emplace_back(std::move(validator1));
  validators.emplace_back(std::move(validator2));
  return std::make_unique<OrValidator<T>>(std::move(validators));
}


template<typename T>
std::unique_ptr<Validator<T>> Validators<T>::anyOf(
    std::unique_ptr<Validator<T>> validator1,
    std::unique_ptr<Validator<T>> validator2,
    std::unique_ptr<Validator<T>> validator3) {
  std::vector<std::unique_ptr<Validator<T>>> validators;
  validators.emplace_back(std::move(validator1));
  validators.emplace_back(std::move(validator2));
  validators.emplace_back(std::move(validator3));
  return std::make_unique<OrValidator<T>>(std::move(validators));
}


template<typename T>
std::unique_ptr<Validator<T>> Validators<T>::allOf(
    std::unique_ptr<Validator<T>> validator1,
    std::unique_ptr<Validator<T>> validator2) {
  std::vector<std::unique_ptr<Validator<T>>> validators;
  validators.emplace_back(std::move(validator1));
  validators.emplace_back(std::move(validator2));
  return std::make_unique<AndValidator<T>>(std::move(validators));
}


template<typename T>
std::unique_ptr<Validator<T>> Validators<T>::allOf(
    std::unique_ptr<Validator<T>> validator1,
    std::unique_ptr<Validator<T>> validator2,
    std::unique_ptr<Validator<T>> validator3) {
  std::vector<std::unique_ptr<Validator<T>>> validators;
  validators.emplace_back(std::move(validator1));
  validators.emplace_back(std::move(validator2));
  validators.emplace_back(std::move(validator3));
  return std::make_unique<AndValidator<T>>(std::move(validators));
}


}  // namespace oomuse

#endif  // OOMUSE_CORE_VALIDATORS_H
