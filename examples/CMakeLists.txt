# Put every example program straight in build/examples, on every system.
# Visual Studio (the usual Windows setup) would otherwise put them in
# build/examples/Debug or build/examples/Release, and the README command
# .\build\examples\pipeline.exe would not find them.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
foreach(config Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER ${config} config_upper)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config_upper} ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

add_executable(scatter_example scatter_example.cpp)
target_link_libraries(scatter_example PRIVATE vizlib)

add_executable(histogram_example histogram_example.cpp)
target_link_libraries(histogram_example PRIVATE vizlib)

add_executable(pipeline pipeline.cpp)
target_link_libraries(pipeline PRIVATE vizlib)

add_executable(joint_plot_example joint_plot_example.cpp)
target_link_libraries(joint_plot_example PRIVATE vizlib)

add_executable(ecdf_example ecdf_example.cpp)
target_link_libraries(ecdf_example PRIVATE vizlib)

add_executable(data_examples data_examples.cpp)
target_link_libraries(data_examples PRIVATE vizlib)

add_executable(stats_example stats_example.cpp)
target_link_libraries(stats_example PRIVATE vizlib)

add_executable(log_axis_example log_axis_example.cpp)
target_link_libraries(log_axis_example PRIVATE vizlib)

add_executable(shapes_example shapes_example.cpp)
target_link_libraries(shapes_example PRIVATE vizlib)
