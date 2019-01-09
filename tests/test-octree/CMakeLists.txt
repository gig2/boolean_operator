add_executable(test-octree
  main.cpp
  test_octree.cpp
  )

target_include_directories(test-octree PRIVATE
  $<BUILD_INTERFACE:${gtest_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${gmock_SOURCE_DIR}/include>)

target_link_libraries(test-octree PRIVATE
  boolean_operator_core
  gtest
  gmock)

add_test(NAME test-octree COMMAND test-octree
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
