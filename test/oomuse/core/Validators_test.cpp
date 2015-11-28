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

#include "oomuse/core/Validators.h"

#include <string>

#include "gtest/gtest.h"
#include "oomuse/core/int_types.h"

using oomuse::Validators;
using std::string;

namespace {


void EXPECT_CONTAINS(const string& substr, const string& str) {
  EXPECT_TRUE(str.find(substr) != string::npos)
      << "Expected: \"" << str << "\" to contain \"" << substr
      << "\", but it does not.";
}


TEST(Validators, less) {
  auto lessThanFive = Validators<int32>::less(5);
  EXPECT_EQ("", lessThanFive->checkValidationErrors(-1));
  EXPECT_EQ("", lessThanFive->checkValidationErrors(0));
  EXPECT_EQ("", lessThanFive->checkValidationErrors(4));
  EXPECT_EQ("Must be less than 5.", lessThanFive->checkValidationErrors(5));
  EXPECT_EQ("Must be less than 5.", lessThanFive->checkValidationErrors(6));
}


TEST(Validators, lessOrEqual) {
  auto lessOrEqualToTwo = Validators<int64>::lessOrEqual(2);
  EXPECT_EQ("", lessOrEqualToTwo->checkValidationErrors(-1));
  EXPECT_EQ("", lessOrEqualToTwo->checkValidationErrors(0));
  EXPECT_EQ("", lessOrEqualToTwo->checkValidationErrors(1));
  EXPECT_EQ("", lessOrEqualToTwo->checkValidationErrors(2));
  EXPECT_EQ("Must be less than or equal to 2.",
            lessOrEqualToTwo->checkValidationErrors(3));
}


TEST(Validators, greater) {
  auto positive = Validators<float>::greater(0.0F);
  EXPECT_CONTAINS("Must be greater than 0",
                  positive->checkValidationErrors(-1.0F));
  EXPECT_CONTAINS("Must be greater than 0",
                  positive->checkValidationErrors(0.0F));
  EXPECT_EQ("", positive->checkValidationErrors(0.01F));
}


TEST(Validators, greaterOrEqual) {
  auto nonNegative = Validators<double>::greaterOrEqual(0.0);
  EXPECT_CONTAINS("Must be greater than or equal to 0",
                  nonNegative->checkValidationErrors(-1.0));
  EXPECT_EQ("", nonNegative->checkValidationErrors(0.0));
  EXPECT_EQ("", nonNegative->checkValidationErrors(0.01));
}


TEST(Validators, equal) {
  auto theAnswer = Validators<int32>::equal(42);
  EXPECT_EQ("Must be equal to 42.", theAnswer->checkValidationErrors(41));
  EXPECT_EQ("", theAnswer->checkValidationErrors(42));
  EXPECT_EQ("Must be equal to 42.", theAnswer->checkValidationErrors(43));
}


TEST(Validators, notEqual) {
  auto correctWord = Validators<string>::notEqual("irregardless");
  EXPECT_EQ("", correctWord->checkValidationErrors("regardless"));
  EXPECT_EQ("Must not be equal to irregardless.",
            correctWord->checkValidationErrors("irregardless"));
  EXPECT_EQ("", correctWord->checkValidationErrors("irrespective"));
}


TEST(Validators, sizeLessOrEqual) {
  auto sizeLessOrEqualToTwo = Validators<string>::sizeLessOrEqual(2);
  EXPECT_EQ("", sizeLessOrEqualToTwo->checkValidationErrors(""));
  EXPECT_EQ("", sizeLessOrEqualToTwo->checkValidationErrors("1"));
  EXPECT_EQ("", sizeLessOrEqualToTwo->checkValidationErrors("12"));
  EXPECT_EQ("Size/length must be less than or equal to 2.",
            sizeLessOrEqualToTwo->checkValidationErrors("123"));
}


TEST(Validators, sizeGreaterOrEqual) {
  auto sizeGreaterOrEqualTwo = Validators<string>::sizeGreaterOrEqual(2);
  EXPECT_EQ("Size/length must be greater than or equal to 2.",
            sizeGreaterOrEqualTwo->checkValidationErrors(""));
  EXPECT_EQ("Size/length must be greater than or equal to 2.",
            sizeGreaterOrEqualTwo->checkValidationErrors("1"));
  EXPECT_EQ("", sizeGreaterOrEqualTwo->checkValidationErrors("12"));
  EXPECT_EQ("", sizeGreaterOrEqualTwo->checkValidationErrors("123"));
}


TEST(Validators, anyOf_pair) {
  auto luckyNumbers = Validators<int32>::anyOf(
      Validators<int32>::equal(7),
      Validators<int32>::equal(13));
  EXPECT_EQ("", luckyNumbers->checkValidationErrors(7));
  EXPECT_EQ("", luckyNumbers->checkValidationErrors(13));
  EXPECT_EQ("Requires one of:\n  Must be equal to 7.\n  Must be equal to 13.\n",
            luckyNumbers->checkValidationErrors(21));
}


TEST(Validators, anyOf_triple) {
  auto luckyNumbers = Validators<int32>::anyOf(
      Validators<int32>::equal(7),
      Validators<int32>::equal(13),
      Validators<int32>::equal(21));
  EXPECT_EQ("", luckyNumbers->checkValidationErrors(7));
  EXPECT_EQ("", luckyNumbers->checkValidationErrors(13));
  EXPECT_EQ("", luckyNumbers->checkValidationErrors(21));
  EXPECT_EQ(
      "Requires one of:\n  Must be equal to 7.\n  Must be equal to 13.\n"
          "  Must be equal to 21.\n",
      luckyNumbers->checkValidationErrors(42));
}


TEST(Validators, allOf_pair) {
  auto isWeekdayIndex = Validators<int32>::allOf(
      Validators<int32>::greaterOrEqual(0),
      Validators<int32>::less(7));
  EXPECT_EQ("Must be greater than or equal to 0.\n",
            isWeekdayIndex->checkValidationErrors(-1));
  EXPECT_EQ("", isWeekdayIndex->checkValidationErrors(0));
  EXPECT_EQ("", isWeekdayIndex->checkValidationErrors(1));
  EXPECT_EQ("", isWeekdayIndex->checkValidationErrors(4));
  EXPECT_EQ("", isWeekdayIndex->checkValidationErrors(6));
  EXPECT_EQ("Must be less than 7.\n",
            isWeekdayIndex->checkValidationErrors(7));
}


TEST(Validators, allOf_triple) {
  auto isNonFridayWeekdayIndex = Validators<int32>::allOf(
      Validators<int32>::greaterOrEqual(0),
      Validators<int32>::notEqual(4),
      Validators<int32>::less(7));
  EXPECT_EQ("Must be greater than or equal to 0.\n",
            isNonFridayWeekdayIndex->checkValidationErrors(-1));
  EXPECT_EQ("", isNonFridayWeekdayIndex->checkValidationErrors(0));
  EXPECT_EQ("", isNonFridayWeekdayIndex->checkValidationErrors(1));
  EXPECT_EQ("Must not be equal to 4.\n",
            isNonFridayWeekdayIndex->checkValidationErrors(4));
  EXPECT_EQ("", isNonFridayWeekdayIndex->checkValidationErrors(6));
  EXPECT_EQ("Must be less than 7.\n",
            isNonFridayWeekdayIndex->checkValidationErrors(7));
}


}  // namespace
