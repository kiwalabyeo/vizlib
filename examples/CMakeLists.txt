add_executable(scatter_example scatter_example.cpp)
target_link_libraries(scatter_example PRIVATE vizlib)

add_executable(histogram_example histogram_example.cpp)
target_link_libraries(histogram_example PRIVATE vizlib)

add_executable(pipeline pipeline.cpp)
target_link_libraries(pipeline PRIVATE vizlib)
