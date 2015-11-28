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

#include "oomuse/core/Optional.h"

#include "gtest/gtest.h"

using oomuse::Optional;

namespace {


int numInstances;

class InstanceCounter {
 public:
  InstanceCounter() { ++numInstances; }
  InstanceCounter(const InstanceCounter& other) { ++numInstances; }
  ~InstanceCounter() { --numInstances; }
};


TEST(Optional, basicOperations) {
  Optional<int> optVal;
  EXPECT_FALSE(optVal.hasValue());

  optVal = -7;
  ASSERT_TRUE(optVal.hasValue());
  EXPECT_EQ(-7, optVal.value());

  optVal.clear();
  EXPECT_FALSE(optVal.hasValue());

  optVal = 42;
  ASSERT_TRUE(optVal.hasValue());
  EXPECT_EQ(42, optVal.value());
}


TEST(Optional, valueConstructor) {
  Optional<int> optVal(3);
  ASSERT_TRUE(optVal.hasValue());
  EXPECT_EQ(3, optVal.value());

  optVal.clear();
  EXPECT_FALSE(optVal.hasValue());

  optVal = 8675309;
  ASSERT_TRUE(optVal.hasValue());
  EXPECT_EQ(8675309, optVal.value());
}


TEST(Optional, constOperations) {
  Optional<char> optVal('A');

  const Optional<char>& constOptVal = optVal;
  ASSERT_TRUE(constOptVal.hasValue());
  EXPECT_EQ('A', constOptVal.value());
}


TEST(Optional, equals) {
  Optional<int> optVal1;
  Optional<int> optVal2;
  EXPECT_TRUE(optVal1 == optVal2);
  EXPECT_TRUE(optVal2 == optVal1);

  optVal1 = 3;
  EXPECT_FALSE(optVal1 == optVal2);
  EXPECT_FALSE(optVal2 == optVal1);

  optVal2 = 3;
  EXPECT_TRUE(optVal1 == optVal2);
  EXPECT_TRUE(optVal2 == optVal1);

  optVal1 = 5;
  EXPECT_FALSE(optVal1 == optVal2);
  EXPECT_FALSE(optVal2 == optVal1);

  optVal2 = 5;
  EXPECT_TRUE(optVal1 == optVal2);
  EXPECT_TRUE(optVal2 == optVal1);

  optVal1.clear();
  EXPECT_FALSE(optVal1 == optVal2);
  EXPECT_FALSE(optVal2 == optVal1);

  optVal2.clear();
  EXPECT_TRUE(optVal1 == optVal2);
  EXPECT_TRUE(optVal2 == optVal1);
}


TEST(Optional, lessThan) {
  Optional<int> optVal1 = 4;
  Optional<int> optVal2 = 7;
  Optional<int> optVal3;
  Optional<int> optVal4;

  // Both values set: compare values.
  EXPECT_TRUE(optVal1 < optVal2);
  EXPECT_FALSE(optVal2 < optVal1);

  // One set, one not set: unset considered less.
  EXPECT_FALSE(optVal2 < optVal3);
  EXPECT_TRUE(optVal3 < optVal2);

  // Do stuff with values to prevent compiler optimizing away storage...
  optVal3 = 0;
  optVal4 = 0;
  int* val3Ptr = &optVal3.value();
  int* val4Ptr = &optVal4.value();
  *val3Ptr = 11;
  *val4Ptr = 23;
  EXPECT_EQ(11, optVal3.value());
  EXPECT_EQ(23, optVal4.value());

  // Both unset: compare memory addresses for stability.
  optVal3.clear();
  optVal4.clear();
  EXPECT_TRUE((optVal3 < optVal4) != (optVal4 < optVal3));
}


TEST(Optional, destructorCalls) {
  numInstances = 0;

  /* Open scope */ {
    Optional<InstanceCounter> optObj;
    EXPECT_EQ(0, numInstances);

    optObj = InstanceCounter();
    EXPECT_EQ(1, numInstances);
    optObj.clear();
    EXPECT_EQ(0, numInstances);

    optObj = InstanceCounter();
    EXPECT_EQ(1, numInstances);
    optObj = InstanceCounter();
    EXPECT_EQ(1, numInstances);

    // Close scope should cause destruction...
  }

  EXPECT_EQ(0, numInstances);

  /* Open scope */ {
    Optional<InstanceCounter> optObj;
    // Close scope should cause destruction, but no value set, so no instances.
  }

  EXPECT_EQ(0, numInstances);
}


}  // namespace
