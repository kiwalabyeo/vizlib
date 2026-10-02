add_executable(vizlib_tests
    test_main.cpp
    test_version.cpp
)

target_link_libraries(vizlib_tests PRIVATE vizlib)
target_include_directories(vizlib_tests PRIVATE ${PROJECT_SOURCE_DIR}/third_party)

add_test(NAME vizlib_tests COMMAND vizlib_tests)