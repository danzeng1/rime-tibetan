cmake_minimum_required(VERSION 3.10)
project(rime_tibetan)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. 手动指定 Rime 的安装路径（关键修正！）
# ------------------------------------------------
# 假设 Rime 安装在 C:/librime/install
set(RIME_ROOT "C:\Users\1\Downloads\rime")  # 根据你的实际路径修改

# 设置 Rime 的 CMake 模块路径
set(Rime_DIR "${RIME_ROOT}/share/rime/cmake")
message(STATUS "Rime_DIR set to: ${Rime_DIR}")

# 设置头文件和库文件路径
set(Rime_INCLUDE_DIR "${RIME_ROOT}/include")
set(Rime_LIBRARY "${RIME_ROOT}/lib/rime.lib")

# 查找 Rime 包
find_package(Rime REQUIRED)
if (Rime_FOUND)
    message(STATUS "Found Rime: ${Rime_INCLUDE_DIRS}, ${Rime_LIBRARIES}")
else()
    message(FATAL_ERROR "Rime not found! Please check RIME_ROOT path.")
endif()

# 查找 GTest
find_package(GTest REQUIRED)

# 2. 主插件库
# ------------------------------------------------
add_library(tibetan SHARED
    src/tibetan.cc
    src/tibetan_engine.cc
)

# 包含目录
target_include_directories(tibetan PRIVATE
    ${Rime_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS}
)

# 链接库
target_link_libraries(tibetan PRIVATE
    ${Rime_LIBRARIES}
    # Windows 必需的系统库
    kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib
)

# 3. 安装规则
# ------------------------------------------------
install(TARGETS tibetan
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/rime
)

install(FILES
    data/tibetan.schema.yaml
    data/tibetan.dict.yaml
    data/tibetan.yaml
    DESTINATION ${CMAKE_INSTALL_DATADIR}/rime-data
)

# 4. 测试程序
# ------------------------------------------------
add_executable(tibetan_test
    src/tibetan_test.cc
)

target_link_libraries(tibetan_test
    tibetan
    GTest::GTest
    GTest::Main
)

enable_testing()
add_test(NAME tibetan_test COMMAND tibetan_test)