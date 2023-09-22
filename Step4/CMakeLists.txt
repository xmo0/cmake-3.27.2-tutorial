# TODO 1: Update the minimum required version to 3.15
# generator expression was introduced in CMake 3.15.
cmake_minimum_required(VERSION 3.15)

# ---------------------- Generator expressions test ----------------------
# "$<0:aaa>_$<1:bbb>"             _bbb
# "$<IF:1,aaa,bbb>"               aaa
# "$<IF:0,aaa,bbb>"               bbb
# "$<IF:0,aaa,>"                  
# "$<IF:0,aaa>"                   error
# "$<BOOL:false>" 0 false off n no ignore notfound -> 0
# "$<$<BOOL:${HAVE_SOME_FEATURE}>:-DENABLE_SOME_FEATURE>"
# "$<AND:1,1,1,0>"                0
# "$<AND:1,1,1,1>"                1
# "$<OR:1,1,0>"                   1
# "$<NOT:1>"                      0
# "$<STREQUAL:123,1234>"          0
# "$<STREQUAL:123,123>"           1
# "$<STREQUAL:$<UPPER_CASE:${foo}>,BAR>"
# "$<EQUAL:123,123>"              1
# $<VERSION_LESS:v1,v2>
# $<VERSION_GREATER:v1,v2>
# $<VERSION_EQUAL:v1,v2>
# $<VERSION_LESS_EQUAL:v1,v2>
# $<VERSION_GREATER_EQUAL:v1,v2>
# $<LOWER_CASE:string>
# $<UPPER_CASE:string>
# $<MAKE_C_IDENTIFIER:...>
# "$<IN_LIST:1,0;1;2;3>"          1
# "$<IN_LIST:Abc,ab;abc;abcd>"    0
# "$<JOIN:1;2;4;5,_>"             1_2_4_5
# "$<LIST:JOIN,1;2;4;5,_>"        1_2__4_5 #3.27

# ----- Toolchain And Language Expressions 
# $<CXX_COMPILER_VERSION>
# $<CXX_COMPILER_VERSION:version>
# $<COMPILE_LANG_AND_ID:language,compiler_ids>

# cmake --build . --target genexdebug
add_custom_target(genexdebug COMMAND ${CMAKE_COMMAND} -E echo "$<BUILD_INTERFACE:-Wall>")

# ---------------------- Generator expressions test end ------------------

# set the project name and version
project(Tutorial VERSION 1.0)

# specify the C++ standard
add_library(tutorial_compiler_flags INTERFACE)
target_compile_features(tutorial_compiler_flags INTERFACE cxx_std_11)

# TODO 2: Create helper variables to determine which compiler we are using:
# * Create a new variable gcc_like_cxx that is true if we are using CXX and
#   any of the following compilers: ARMClang, AppleClang, Clang, GNU, LCC
# * Create a new variable msvc_cxx that is true if we are using CXX and MSVC
# Hint: Use set() and COMPILE_LANG_AND_ID
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx     "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
message(STATUS ${gcc_like_cxx})
message(STATUS ${msvc_cxx})

# TODO 3: Add warning flag compile options to the interface library
# tutorial_compiler_flags.
# * For gcc_like_cxx, add flags -Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused
# * For msvc_cxx, add flags -W3
# Hint: Use target_compile_options()

# target_compile_options(tutorial_compiler_flags
#                        INTERFACE
#                        "$<${gcc_like_cxx}:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>"
#                        "$<${msvc_cxx}:-W3>"
#                      )

target_compile_options(tutorial_compiler_flags
                       INTERFACE
                       "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
                       "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
                      )

# TODO 4: With nested generator expressions, only use the flags for the
# build-tree
# Hint: Use BUILD_INTERFACE

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(TutorialConfig.h.in TutorialConfig.h)

# add the MathFunctions library
add_subdirectory(MathFunctions)

# add the executable
add_executable(Tutorial tutorial.cxx)

target_link_libraries(Tutorial PUBLIC MathFunctions tutorial_compiler_flags)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
