# The MIT License (MIT)
#
# Copyright (c) 2017 Mateusz Pusz
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

from conans import ConanFile, CMake

class FsmVariantConan(ConanFile):
    name = "fsm-variant"
    version = "1.0.0"
    description = "Finite State Machine implementation using `std::variant` and `std::optional` under the hood"
    author = "Mateusz Pusz"
    license = "https://github.com/mpusz/fsm-variant/blob/master/LICENSE.md"
    url = "https://github.com/mpusz/fsm-variant"
    exports = ["LICENSE.md"]
    settings = "os", "compiler", "build_type", "arch"
    scm = {
        "type": "git",
        "url": "auto",
        "revision": "auto"
    }

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure(source_folder="src")
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        self.copy("license*", dst="licenses",  ignore_case=True, keep_path=False)
        cmake = self._configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["fsm"]

    def package_id(self):
        self.info.header_only()
