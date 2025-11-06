cmake_minimum_required(VERSION 3.20)

project(computer_club
        VERSION 1.0.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(LIB_SOURCES
        # Core Logic
        src/core/computer_club_manager_impl.cpp

        # Entities
        src/entities/client.cpp
        src/entities/club_configuration.cpp
        src/entities/event.cpp
        src/entities/table.cpp
        src/entities/time.cpp

        # Services
        src/services/client_repository.cpp
        src/services/event_repository.cpp
        src/services/table_repository.cpp

        # Utilities
        src/utils/exceptions.cpp
        src/utils/file_parser.cpp
        src/utils/result_printer.cpp)

add_library(computer_club_lib ${LIB_SOURCES})

target_include_directories(computer_club_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(computer_club_lib PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Werror>
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
)

add_executable(computer_club src/main.cpp)

target_link_libraries(computer_club PRIVATE computer_club_lib)

target_compile_options(computer_club PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Werror>
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
)

enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0)


FetchContent_MakeAvailable(googletest)
include(GoogleTest)

set(TEST_SOURCES
        tests/test_main.cpp
        tests/test_computer_club.cpp
        tests/entities/test_time.cpp
        tests/entities/test_table.cpp

        tests/services/test_client_repository.cpp
        tests/services/test_table_repository.cpp

        tests/utils/test_file_parser.cpp
)

add_executable(computer_club_tests ${TEST_SOURCES})

target_link_libraries(computer_club_tests PRIVATE
        computer_club_lib
        GTest::gtest
        GTest::gmock
)

target_compile_options(computer_club_tests PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Werror>
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
)

gtest_discover_tests(computer_club_tests)