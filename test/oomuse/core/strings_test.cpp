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

#include "oomuse/core/strings.h"

#include <string>

#include "gtest/gtest.h"

using std::locale;
using std::string;

namespace strings = oomuse::strings;

namespace {


TEST(strings, isEmptyOrWhitespace) {
  EXPECT_TRUE(strings::isEmptyOrWhitespace(""));
  EXPECT_TRUE(strings::isEmptyOrWhitespace(" "));
  EXPECT_TRUE(strings::isEmptyOrWhitespace("  "));
  EXPECT_TRUE(strings::isEmptyOrWhitespace("                      "));
  EXPECT_TRUE(strings::isEmptyOrWhitespace("\t"));
  EXPECT_TRUE(strings::isEmptyOrWhitespace(" \t "));
  EXPECT_TRUE(strings::isEmptyOrWhitespace("       \t"));
  EXPECT_TRUE(strings::isEmptyOrWhitespace("\n"));
  EXPECT_TRUE(strings::isEmptyOrWhitespace("\n\t\v\f\r\r\f\v\t\n   \n"));

  EXPECT_FALSE(strings::isEmptyOrWhitespace("a"));
  EXPECT_FALSE(strings::isEmptyOrWhitespace("ab"));
  EXPECT_FALSE(strings::isEmptyOrWhitespace("abc"));
  EXPECT_FALSE(strings::isEmptyOrWhitespace("            a"));
  EXPECT_FALSE(strings::isEmptyOrWhitespace("b            "));
  EXPECT_FALSE(strings::isEmptyOrWhitespace("      c      "));
}


TEST(strings, isEmptyOrWhitespace_locale) {
  locale loc;

  EXPECT_TRUE(strings::isEmptyOrWhitespace("", loc));
  EXPECT_TRUE(strings::isEmptyOrWhitespace(" ", loc));
  EXPECT_TRUE(strings::isEmptyOrWhitespace("  ", loc));
  EXPECT_TRUE(strings::isEmptyOrWhitespace("                      ", loc));
  EXPECT_TRUE(strings::isEmptyOrWhitespace("\t", loc));
  EXPECT_TRUE(strings::isEmptyOrWhitespace(" \t ", loc));
  EXPECT_TRUE(strings::isEmptyOrWhitespace("       \t", loc));
  EXPECT_TRUE(strings::isEmptyOrWhitespace("\n", loc));
  EXPECT_TRUE(strings::isEmptyOrWhitespace("\n\t\v\f\r\r\f\v\t\n   \n", loc));

  EXPECT_FALSE(strings::isEmptyOrWhitespace("a", loc));
  EXPECT_FALSE(strings::isEmptyOrWhitespace("ab", loc));
  EXPECT_FALSE(strings::isEmptyOrWhitespace("abc", loc));
  EXPECT_FALSE(strings::isEmptyOrWhitespace("            a", loc));
  EXPECT_FALSE(strings::isEmptyOrWhitespace("b            ", loc));
  EXPECT_FALSE(strings::isEmptyOrWhitespace("      c      ", loc));
}


TEST(strings, startsWith) {
  EXPECT_TRUE(strings::startsWith("Awesome", ""));
  EXPECT_TRUE(strings::startsWith("Awesome", "Awe"));
  EXPECT_FALSE(strings::startsWith("Awesome", "AweSo"));
  EXPECT_TRUE(strings::startsWith("Awesome", "Awesome"));
  EXPECT_FALSE(strings::startsWith("Awesome", "Awesome!"));
}


TEST(strings, endsWith) {
  EXPECT_TRUE(strings::endsWith("Awesome", ""));
  EXPECT_TRUE(strings::endsWith("Awesome", "some"));
  EXPECT_FALSE(strings::endsWith("Awesome", "wEsome"));
  EXPECT_TRUE(strings::endsWith("Awesome", "Awesome"));
  EXPECT_FALSE(strings::endsWith("Awesome", "#Awesome"));
}


TEST(strings, trimWhitespace) {
  EXPECT_EQ("abcdefghijklmnop",
            strings::trimWhitespace(" \t abcdefghijklmnop \t "));
  EXPECT_EQ("abcdefghijklmnop",
            strings::trimWhitespace("abcdefghijklmnop \t "));
  EXPECT_EQ("abcdefghijklmnop",
            strings::trimWhitespace(" \t abcdefghijklmnop"));
  EXPECT_EQ("a", strings::trimWhitespace("a"));
  EXPECT_EQ("ab", strings::trimWhitespace("ab"));
  EXPECT_EQ("abc", strings::trimWhitespace("abc"));
  EXPECT_EQ("a b c", strings::trimWhitespace("a b c"));
  EXPECT_EQ("a b c", strings::trimWhitespace(" \t a b c \t "));
  EXPECT_EQ("a \t b \t c", strings::trimWhitespace(" \t a \t b \t c \t "));
  EXPECT_EQ("", strings::trimWhitespace(" \t \n \r \v \f"));
  EXPECT_EQ("", strings::trimWhitespace(""));

  // Make sure original string isn't modified.
  string str = "  trim me  ";
  EXPECT_EQ("trim me", strings::trimWhitespace(str));
  EXPECT_EQ("  trim me  ", str);
}


TEST(strings, trimWhitespace_locale) {
  locale loc;

  EXPECT_EQ("abcdefghijklmnop",
            strings::trimWhitespace(" \t abcdefghijklmnop \t ", loc));
  EXPECT_EQ("abcdefghijklmnop",
            strings::trimWhitespace("abcdefghijklmnop \t ", loc));
  EXPECT_EQ("abcdefghijklmnop",
            strings::trimWhitespace(" \t abcdefghijklmnop", loc));
  EXPECT_EQ("a", strings::trimWhitespace("a", loc));
  EXPECT_EQ("ab", strings::trimWhitespace("ab", loc));
  EXPECT_EQ("abc", strings::trimWhitespace("abc", loc));
  EXPECT_EQ("a b c", strings::trimWhitespace("a b c", loc));
  EXPECT_EQ("a b c", strings::trimWhitespace(" \t a b c \t ", loc));
  EXPECT_EQ("a \t b \t c", strings::trimWhitespace(" \t a \t b \t c \t ", loc));
  EXPECT_EQ("", strings::trimWhitespace(" \t \n \r \v \f", loc));
  EXPECT_EQ("", strings::trimWhitespace("", loc));

  // Make sure original string isn't modified.
  string str = "  trim me  ";
  EXPECT_EQ("trim me", strings::trimWhitespace(str, loc));
  EXPECT_EQ("  trim me  ", str);
}


TEST(strings, toLowerCase) {
  string str = "tO lOWeRCase";
  EXPECT_EQ("to lowercase", strings::toLowerCase(str));
  EXPECT_EQ("tO lOWeRCase", str);  // Make sure original wasn't modified.
}


TEST(strings, toLowerCase_locale) {
  locale loc;
  string str = "tO lOWeRCase";
  EXPECT_EQ("to lowercase", strings::toLowerCase(str, loc));
  EXPECT_EQ("tO lOWeRCase", str);  // Make sure original wasn't modified.
}


TEST(strings, toUpperCase) {
  string str = "tO uPPeRCase";
  EXPECT_EQ("TO UPPERCASE", strings::toUpperCase(str));
  EXPECT_EQ("tO uPPeRCase", str);  // Make sure original wasn't modified.
}


TEST(strings, toUpperCase_locale) {
  locale loc;
  string str = "tO uPPeRCase";
  EXPECT_EQ("TO UPPERCASE", strings::toUpperCase(str, loc));
  EXPECT_EQ("tO uPPeRCase", str);  // Make sure original wasn't modified.
}


}  // namespace
