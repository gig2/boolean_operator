set (sourcesFiles
    main.cpp)

add_subdirectory(core)
add_subdirectory(gui)


add_executable(boolean_operator ${sourcesFiles} )

target_include_directories(boolean_operator PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

target_link_libraries(boolean_operator PUBLIC
   boolean_operator_gui
    )

set_target_properties(boolean_operator PROPERTIES LINK_FLAGS
  "${OPENMESH_LDFLAGS}")
