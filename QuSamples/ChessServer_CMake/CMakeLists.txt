# CMakeList.txt : CMake project for ChessServer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

project ("ChessServer")

add_executable (ChessServer "../ChessServer/src/ChessServer.cpp")

target_compile_options(ChessServer PRIVATE -Wformat=0)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ChessServer PROPERTY CXX_STANDARD 20)
endif()