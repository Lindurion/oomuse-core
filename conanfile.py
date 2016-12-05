## Copyright 2015 Eric W. Barndollar. All rights reserved.
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
##
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.

import conans


class OOMuseCoreConan(conans.ConanFile):
  name = "oomuse-core"
  version = "0.1"

  options = {
    "include_pdbs": [False, True],
    "testing": [False, True],
  }
  default_options = "include_pdbs=False", "testing=False"

  author = "Eric W. Barndollar (eric@EricWBarndollar.com)"
  build_policy = "missing"
  exports = "*"
  generators = "cmake"
  license = "Apache 2.0"
  settings = "os", "compiler", "build_type", "arch"
  url = "https://github.com/lindurion/oomuse-core"

  def requirements(self):
    if self.options.testing:
      self.requires("gtest/1.8.0@lasote/stable")

  def imports(self):
    self.copy("*.dll", dst="bin", src="bin")
    self.copy("*.dylib*", dst="bin", src="lib")

  def build(self):
    cmake_test_def = "-DOOMUSE_CORE_TESTING=1" if self.options.testing else ""

    cmake = conans.CMake(self.settings)
    self.run("cmake %s %s %s" % (self.conanfile_directory,
                                 cmake.command_line,
                                 cmake_test_def))
    self.run("cmake --build . %s" % cmake.build_config)

    # If testing, run unit tests to make sure library works before packaging.
    if self.options.testing:
      self.run("ctest -C %s --output-on-failure" % self.settings.build_type)

  def package(self):
    # Include headers:
    self.copy(pattern="*.h", dst="include", src="include", keep_path=True)

    # Static libs:
    self.copy(pattern="*.a", dst="lib", src=".", keep_path=False)
    self.copy(pattern="*.lib", dst="lib", src=".", keep_path=False)

    # Debug symbols:
    if self.settings.compiler == "Visual Studio" and self.options.include_pdbs:
      self.copy(pattern="*.pdb", dst="lib", src=".", keep_path=False)

  def package_info(self):
    # Projects that use oomuse-core must link the right static library name.
    if self.settings.build_type == "Debug":
      self.cpp_info.libs.append("oomuse-core-debug")
    else:
      self.cpp_info.libs.append("oomuse-core")
