
add_executable(vizlib_tests
    test_main.cpp
    test_version.cpp
    test_image_export.cpp
    test_canvas_export.cpp
    test_scatter.cpp
    test_histogram.cpp
    test_joint_plot.cpp
    test_text.cpp
    test_annotations.cpp
    test_legend.cpp
    test_drawable.cpp
    test_figure_labels.cpp
)

target_link_libraries(vizlib_tests PRIVATE vizlib)
target_include_directories(vizlib_tests PRIVATE ${PROJECT_SOURCE_DIR}/third_party)

add_test(NAME vizlib_tests COMMAND vizlib_tests)

set_tests_properties(vizlib_tests PROPERTIES
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
