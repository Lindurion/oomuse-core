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

#include "oomuse/core/FixedArray.h"

#include <algorithm>
#include <unordered_map>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

using oomuse::FixedArray;
using std::fill;
using std::forward;
using std::move;
using std::unordered_map;
using std::vector;

namespace {


class CountMyDestruction {
 public:
  void setDestructorCountsMap(
      unordered_map<CountMyDestruction*, int>* countsMap) {
    countsMap_ = countsMap;
  }

  ~CountMyDestruction() { ++((*countsMap_)[this]); }

 private:
  unordered_map<CountMyDestruction*, int>* countsMap_;
};


TEST(FixedArray, ensureDestruction) {
  unordered_map<CountMyDestruction*, int> destructorCounts;

  /* Open a new scope */ {
    FixedArray<CountMyDestruction> fixedArray(100);
    for (int i = 0; i < 100; ++i) {
      fixedArray[i].setDestructorCountsMap(&destructorCounts);
    }

    // Elements in FixedArray should be destructed when it goes out of scope.
  }

  // Make sure 100 objects were destroyed.
  EXPECT_EQ(100, destructorCounts.size());

  // Make sure each destructor was invoked exactly once.
  for (auto& entry : destructorCounts) {
    EXPECT_EQ(1, entry.second);
  }
}


TEST(FixedArray, length) {
  FixedArray<int> fixedArray0(0);
  EXPECT_EQ(0, fixedArray0.length());

  FixedArray<double> fixedArray1(1);
  EXPECT_EQ(1, fixedArray1.length());

  FixedArray<const char*> fixedArray7(7);
  EXPECT_EQ(7, fixedArray7.length());

  FixedArray<bool> fixedArray10000(10000);
  EXPECT_EQ(10000, fixedArray10000.length());
}


/** Allocator that sets all allocated elements to 42 for testing. */
struct SetTo42Allocator {
  using value_type = int;

  int* allocate(std::size_t n) {
    int* data = allocator_.allocate(n);
    fill(&data[0], &data[n], 42);
    return data;
  }

  void deallocate(int* data, std::size_t n) {
    allocator_.deallocate(data, n);
  }

 private:
  std::allocator<int> allocator_;
};


using FixedArray42Alloc = FixedArray<int, SetTo42Allocator>;


TEST(FixedArray, defaultInit) {
  FixedArray42Alloc fixedArray(4);

  // Should have still been default initialized to 0 after allocation.
  EXPECT_EQ(0, fixedArray[0]);
  EXPECT_EQ(0, fixedArray[1]);
  EXPECT_EQ(0, fixedArray[2]);
  EXPECT_EQ(0, fixedArray[3]);
}


TEST(FixedArray, skipDefaultInit) {
  FixedArray42Alloc fixedArray(4, FixedArray42Alloc::SKIP_DEFAULT_INIT);

  // Should NOT have been default initialized after allocation.
  EXPECT_EQ(42, fixedArray[0]);
  EXPECT_EQ(42, fixedArray[1]);
  EXPECT_EQ(42, fixedArray[2]);
  EXPECT_EQ(42, fixedArray[3]);
}


TEST(FixedArray, emptyInitializerList) {
  FixedArray<int> fixedArray = {};
  EXPECT_EQ(0, fixedArray.length());
}


TEST(FixedArray, primitiveInitializerList) {
  FixedArray<int> fixedArray = {4, 5, 6};
  ASSERT_EQ(3, fixedArray.length());

  EXPECT_EQ(4, fixedArray[0]);
  EXPECT_EQ(5, fixedArray[1]);
  EXPECT_EQ(6, fixedArray[2]);
}


class CheckMyConstruction {
 public:
  CheckMyConstruction(
      char letter,
      unordered_map<CheckMyConstruction*, int>* destructorCountsMap)
      : letter_(letter), wasDirectlyConstructed_(true), wasCopied_(false),
        countsMap_(destructorCountsMap) {
  }

  CheckMyConstruction(const CheckMyConstruction& other)
      : letter_(other.letter_), wasDirectlyConstructed_(false),
        wasCopied_(true), countsMap_(other.countsMap_) {
  }

  ~CheckMyConstruction() { ++((*countsMap_)[this]); }

  char letter() { return letter_; }
  bool wasDirectlyConstructed() { return wasDirectlyConstructed_; }
  bool wasCopied() { return wasCopied_; }

 private:
  char letter_;
  bool wasDirectlyConstructed_;
  bool wasCopied_;
  unordered_map<CheckMyConstruction*, int>* countsMap_;
};


TEST(FixedArray, objectInitializerList) {
  unordered_map<CheckMyConstruction*, int> destructorCounts;

  /* Open a new scope */ {
    FixedArray<CheckMyConstruction> fixedArray = {
      {'A', &destructorCounts},
      {'B', &destructorCounts},
      {'C', &destructorCounts}
    };

    // Temporary objects from initializer list should have been destructed.
    EXPECT_EQ(3, destructorCounts.size());
    for (auto& entry : destructorCounts) {
      EXPECT_EQ(1, entry.second);
    }

    ASSERT_EQ(3, fixedArray.length());
    EXPECT_EQ('A', fixedArray[0].letter());
    EXPECT_EQ('B', fixedArray[1].letter());
    EXPECT_EQ('C', fixedArray[2].letter());

    // Since initializer lists may come from static memory, the elements have
    // to be copied, not moved or directly constructed.
    EXPECT_FALSE(fixedArray[0].wasDirectlyConstructed());
    EXPECT_FALSE(fixedArray[1].wasDirectlyConstructed());
    EXPECT_FALSE(fixedArray[2].wasDirectlyConstructed());

    EXPECT_TRUE(fixedArray[0].wasCopied());
    EXPECT_TRUE(fixedArray[1].wasCopied());
    EXPECT_TRUE(fixedArray[2].wasCopied());
  }

  // Now the 3 additional FixedArray objects should have been destructed.
  EXPECT_EQ(6, destructorCounts.size());
  for (auto& entry : destructorCounts) {
    EXPECT_EQ(1, entry.second);
  }
}


TEST(FixedArray, getAndSet) {
  FixedArray<int> fixedArray(3);

  // Elements should be the default constructed ints (0).
  EXPECT_EQ(0, fixedArray[0]);
  EXPECT_EQ(0, fixedArray[1]);
  EXPECT_EQ(0, fixedArray[2]);

  // Set some values.
  fixedArray[0] = 42;
  fixedArray[1] = 7;
  fixedArray[2] = -9;

  // Test those values.
  EXPECT_EQ(42, fixedArray[0]);
  EXPECT_EQ(7, fixedArray[1]);
  EXPECT_EQ(-9, fixedArray[2]);

  // Change some values and test the new ones.
  fixedArray[1] = 0;
  fixedArray[0] = -3;
  EXPECT_EQ(-3, fixedArray[0]);
  EXPECT_EQ(0, fixedArray[1]);
  EXPECT_EQ(-9, fixedArray[2]);
}


TEST(FixedArray, moveCtor) {
  FixedArray<int> fixedArray1(3);
  fixedArray1[0] = 1;
  fixedArray1[1] = 2;
  fixedArray1[2] = 3;

  FixedArray<int> fixedArray2(move(fixedArray1));
  ASSERT_EQ(3, fixedArray2.length());
  EXPECT_EQ(1, fixedArray2[0]);
  EXPECT_EQ(2, fixedArray2[1]);
  EXPECT_EQ(3, fixedArray2[2]);

  EXPECT_EQ(0, fixedArray1.length());
  // It would be an error now to access any fixedArray1 elements.
}


TEST(FixedArray, data) {
  FixedArray<char> fixedArray(4);
  fixedArray[0] = 'h';
  fixedArray[1] = 'e';
  fixedArray[2] = 'y';
  fixedArray[3] = '\0';
  EXPECT_STREQ("hey", fixedArray.data());

  fixedArray.data()[1] = 'o';
  EXPECT_STREQ("hoy", fixedArray.data());
}


class ConstTester {
 public:
  const char* get() { return "mutable"; }
  const char* get() const { return "const"; }
};


/** Makes sure const FixedArrays return const element references. */
TEST(FixedArray, constRefs) {
  FixedArray<ConstTester> fixedArray(2);
  const FixedArray<ConstTester>& constFixedArray = fixedArray;

  EXPECT_STREQ("mutable", fixedArray[0].get());
  EXPECT_STREQ("const", constFixedArray[0].get());
}


TEST(FixedArray, mutableRangeBasedForLoop) {
  FixedArray<int> fixedArray = {7, 8, 9};

  vector<int> seenValues;
  for (int& value : fixedArray) {
    seenValues.push_back(value);
    if (value == 8) {
      value = 3;
    }
  }

  EXPECT_EQ(vector<int>({7, 8, 9}), seenValues);
  EXPECT_EQ(FixedArray<int>({7, 3, 9}), fixedArray);
}


TEST(FixedArray, constRangeBasedForLoop) {
  FixedArray<int> fixedArray = {7, 8, 9};
  const FixedArray<int>& constFixedArray = fixedArray;

  vector<int> seenValues;
  for (const int& value : constFixedArray) {
    seenValues.push_back(value);
  }

  EXPECT_EQ(vector<int>({7, 8, 9}), seenValues);
}


TEST(FixedArray, equals_emptyArrays) {
  FixedArray<int> empty1 = {};
  FixedArray<int> empty2 = {};
  EXPECT_EQ(empty1, empty2);
  EXPECT_EQ(empty2, empty1);

  FixedArray<int> nonEmpty = {0};
  EXPECT_NE(empty1, nonEmpty);
  EXPECT_NE(nonEmpty, empty1);
  EXPECT_NE(empty2, nonEmpty);
  EXPECT_NE(nonEmpty, empty2);
}


TEST(FixedArray, equals_shortSequences) {
  FixedArray<char> abcd1 = {'a', 'b', 'c', 'd'};
  FixedArray<char> abcd2 = {'a', 'b', 'c', 'd'};
  EXPECT_EQ(abcd1, abcd2);
  EXPECT_EQ(abcd2, abcd1);

  FixedArray<char> abc = {'a', 'b', 'c'};
  EXPECT_NE(abcd1, abc);
  EXPECT_NE(abc, abcd1);
  EXPECT_NE(abcd2, abc);
  EXPECT_NE(abc, abcd2);

  FixedArray<char> abcdUppercase = {'A', 'B', 'C', 'D'};
  EXPECT_NE(abcd1, abcdUppercase);
  EXPECT_NE(abcdUppercase, abcd1);
  EXPECT_NE(abcd2, abcdUppercase);
  EXPECT_NE(abcdUppercase, abcd2);

  FixedArray<char> abcde = {'a', 'b', 'c', 'd', 'e'};
  EXPECT_NE(abcd1, abcde);
  EXPECT_NE(abcde, abcd1);
  EXPECT_NE(abcd2, abcde);
  EXPECT_NE(abcde, abcd2);
}


}  // namespace
