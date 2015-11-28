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

#include <algorithm>
#include <cctype>
#include <functional>

using std::all_of;
using std::find_if;
using std::isspace;
using std::isspace;
using std::locale;
using std::string;
using std::tolower;
using std::toupper;
using std::transform;

namespace oomuse {
namespace strings {


bool isEmptyOrWhitespace(const string& str) {
  return all_of(str.begin(), str.end(), [](char c) { return isspace(c); });
}


bool isEmptyOrWhitespace(const string& str, const locale& loc) {
  return all_of(str.begin(), str.end(),
                [&loc](char c) { return isspace(c, loc); });
}


bool startsWith(const string& str, const string& prefix) {
  return (str.compare(0, prefix.length(), prefix) == 0);
}


bool endsWith(const string& str, const string& suffix) {
  return (str.length() >= suffix.length())
      && (str.compare(str.length() - suffix.length(),
                      suffix.length(), suffix) == 0);
}


string trimWhitespace(const string& str) {
  auto firstRealChar = find_if(str.begin(), str.end(), [](char c) {
    return !isspace(c);
  });
  if (firstRealChar == str.end()) {
    return "";
  }

  auto lastRealChar = find_if(str.rbegin(), str.rend(), [](char c) {
    return !isspace(c);
  });

  auto startIndex = firstRealChar - str.begin();
  auto endIndexExclusive = str.rend() - lastRealChar;
  return str.substr(startIndex, endIndexExclusive - startIndex);
}


string trimWhitespace(const string& str, const locale& loc) {
  auto firstRealChar = find_if(str.begin(), str.end(), [&loc](char c) {
    return !isspace(c, loc);
  });
  if (firstRealChar == str.end()) {
    return "";
  }

  auto lastRealChar = find_if(str.rbegin(), str.rend(), [&loc](char c) {
    return !isspace(c, loc);
  });

  auto startIndex = firstRealChar - str.begin();
  auto endIndexExclusive = str.rend() - lastRealChar;
  return str.substr(startIndex, endIndexExclusive - startIndex);
}


string toLowerCase(const string& str) {
  string lowerStr = str;
  transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
            [](char c) { return tolower(c); });
  return lowerStr;
}


string toLowerCase(const string& str, const locale& loc) {
  string lowerStr = str;
  transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
            [&loc](char c) { return tolower(c, loc); });
  return lowerStr;
}


string toUpperCase(const string& str) {
  string upperStr = str;
  transform(upperStr.begin(), upperStr.end(), upperStr.begin(),
            [](char c) { return toupper(c); });
  return upperStr;
}


string toUpperCase(const string& str, const locale& loc) {
  string upperStr = str;
  transform(upperStr.begin(), upperStr.end(), upperStr.begin(),
            [&loc](char c) { return toupper(c, loc); });
  return upperStr;
}


}  // namespace strings
}  // namespace oomuse
