set (core_sources
     mesh.cpp)
set (core_headers
     mesh.h )

add_library(boolean_operator_core ${core_sources} ${core_headers})

target_include_directories(boolean_operator_core PUBLIC
  $<BUILD_INTERFACE:${OPENMESH_INCLUDE_DIRS}>
  $<BUILD_INTERFACE:${includeDir}>
  $<INSTALL_INTERFACE:include>)

target_link_libraries(boolean_operator_core PUBLIC
  ${OPENMESH_LIBRARIES})

set_target_properties(boolean_operator_core PROPERTIES LINK_FLAGS
  "${OPENMESH_LDFLAGS}")
