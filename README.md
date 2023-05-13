# It works

Adding

```
    def package_info(self):
        ...
        self.cpp_info.requires = ["boost::program_options", "boost::thread"]
```

Into `project1` Conan recipe fixed the problem.


# Description

The issue is as follows: I have a project named Project1 that depends on Boost (a few libraries) and Catch2, and it consists of a library and an executable with unit tests. I have another project, named Project2, which depends on Project1 and Catch2; it too is a library and an executable with unit tests.

When building Project1, everything is correct; the unit testing executable (called project1_test) links against the appropriate libraries.

However, when building Project2, an issue arises. The unit testing executable (called project2_test) links against all of the Boost libraries, despite not having a target_link_libraries command for Boost in the Cmake file. The problem here is that it should have linked against the same Boost libraries as were linked in Project1. Instead, it links against all the Boost libraries.

Here, I provide code and instructions to reproduce this issue. It was tested on macOS with apple-clang.



# How to reproduce

## Create the lock files, just for the 1st time.

```
cd project1
conan lock create conanfile.py --version=1.0.0 --update

cd ../project2
conan lock create conanfile.py --version=1.0.0 --update
```

## Build Project 1

```
cd project1
conan lock create conanfile.py --version 1.0.0 --lockfile=conan.lock --lockfile-out=build/conan.lock
conan create conanfile.py --version 1.0.0 --lockfile=build/conan.lock  --build=missing
cd ..
```

## Build Project 2

```
cd project2
conan lock create conanfile.py --version 1.0.0 --lockfile=conan.lock --lockfile-out=build/conan.lock
conan create conanfile.py --version 1.0.0 --lockfile=build/conan.lock  --build=missing
```

# When linking project1_test

```
[100%] Linking CXX executable project1_test
/usr/local/Cellar/cmake/3.24.0/bin/cmake -E cmake_link_script CMakeFiles/project1_test.dir/link.txt --verbose=1
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ -m64 -stdlib=libc++ -O3 -DNDEBUG -arch x86_64 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.3.sdk -Wl,-search_paths_first -Wl,-headerpad_max_install_names -m64 CMakeFiles/project1_test.dir/test/dummy.cpp.o -o project1_test
-L/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib
-L/Users/fernando/.conan2/p/catch1014972ea6d56/p/lib
-Wl,-rpath,/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib
-Wl,-rpath,/Users/fernando/.conan2/p/catch1014972ea6d56/p/lib
libproject1.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_program_options.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_thread.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_atomic.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_chrono.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_container.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_date_time.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_exception.a
/Users/fernando/.conan2/p/catch1014972ea6d56/p/lib/libCatch2Main.a
/Users/fernando/.conan2/p/catch1014972ea6d56/p/lib/libCatch2.a
```

**Ok: it link the proper libs.**

# When linking project2_test

**Linking error:**

```
Undefined symbols for architecture x86_64:
  "test_main(int, char**)", referenced from:
      test_main_caller::operator()() in libboost_test_exec_monitor.a(test_main.o)
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
make[2]: *** [project2_test] Error 1
make[1]: *** [CMakeFiles/project2_test.dir/all] Error 2
make: *** [all] Error 2
```

Why? Because it is linking the whole Boost:

```
[100%] Linking CXX executable project2_test
/usr/local/Cellar/cmake/3.24.0/bin/cmake -E cmake_link_script CMakeFiles/project2_test.dir/link.txt --verbose=1
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ -m64 -stdlib=libc++ -O3 -DNDEBUG -arch x86_64 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.3.sdk -Wl,-search_paths_first -Wl,-headerpad_max_install_names -m64 CMakeFiles/project2_test.dir/test/dummy.cpp.o -o project2_test
-L/Users/fernando/.conan2/p/proje816c863d067cc/p/lib
-L/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib
-L/Users/fernando/.conan2/p/libic656535a467ea4/p/lib
-L/Users/fernando/.conan2/p/bzip2a0dd331867b14/p/lib
-L/Users/fernando/.conan2/p/zlibf0b61bbd04bd8/p/lib
-L/Users/fernando/.conan2/p/libba6fe3b4e0d9321/p/lib
-L/Users/fernando/.conan2/p/catch1014972ea6d56/p/lib
-Wl,-rpath,/Users/fernando/.conan2/p/proje816c863d067cc/p/lib
-Wl,-rpath,/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib
-Wl,-rpath,/Users/fernando/.conan2/p/libic656535a467ea4/p/lib
-Wl,-rpath,/Users/fernando/.conan2/p/bzip2a0dd331867b14/p/lib
-Wl,-rpath,/Users/fernando/.conan2/p/zlibf0b61bbd04bd8/p/lib
-Wl,-rpath,/Users/fernando/.conan2/p/libba6fe3b4e0d9321/p/lib
-Wl,-rpath,/Users/fernando/.conan2/p/catch1014972ea6d56/p/lib
libproject2.a
/Users/fernando/.conan2/p/proje816c863d067cc/p/lib/libproject1.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_log_setup.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_log.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_locale.a
/Users/fernando/.conan2/p/libic656535a467ea4/p/lib/libiconv.a
/Users/fernando/.conan2/p/libic656535a467ea4/p/lib/libcharset.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_iostreams.a
/Users/fernando/.conan2/p/bzip2a0dd331867b14/p/lib/libbz2.a
/Users/fernando/.conan2/p/zlibf0b61bbd04bd8/p/lib/libz.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_graph.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_fiber_numa.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_fiber.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_contract.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_wave.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_unit_test_framework.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_type_erasure.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_timer.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_thread.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_random.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_prg_exec_monitor.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_nowide.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_json.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_filesystem.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_coroutine.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_chrono.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_wserialization.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_url.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_test_exec_monitor.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_stacktrace_noop.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_stacktrace_basic.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_stacktrace_backtrace.a
/Users/fernando/.conan2/p/libba6fe3b4e0d9321/p/lib/libbacktrace.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_stacktrace_addr2line.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_serialization.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_regex.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_program_options.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_math_tr1l.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_math_tr1f.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_math_tr1.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_math_c99l.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_math_c99f.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_math_c99.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_exception.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_date_time.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_context.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_container.a
/Users/fernando/.conan2/p/boostfa968bfe4c99d/p/lib/libboost_atomic.a
/Users/fernando/.conan2/p/catch1014972ea6d56/p/lib/libCatch2Main.a
/Users/fernando/.conan2/p/catch1014972ea6d56/p/lib/libCatch2.a

```