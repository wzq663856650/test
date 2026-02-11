cmake_minimum_required(VERSION 3.16)
project(MyApp VERSION 1.0.0 LANGUAGES CXX)

# C++ 标准设置
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译选项
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# 收集源文件
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 生成可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 安装规则
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
