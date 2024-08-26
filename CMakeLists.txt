cmake_minimum_required(VERSION 3.8)
include($ENV{cpp_lib_build_scripts_path}/cmake-module/setup.cmake)
include(target_import_base)

set(ProjectName body-building)
project(${ProjectName})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)


# 编译为动态库
add_library(${ProjectName})
target_import_src(${ProjectName})
target_import_base(${ProjectName} PUBLIC)

set(CMAKE_INSTALL_PREFIX ${cpp_lib_build_scripts_path}/${platform}/.libs/${ProjectName}/)

# 添加测试程序
add_executable(test ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(test ${ProjectName})
install(TARGETS test
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)
