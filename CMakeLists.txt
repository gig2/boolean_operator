cmake_minimum_required(VERSION 3.5)
project(boolean_operator CXX)

set(CMAKE_CXX_STANDARD 17)

set(includeDir ${CMAKE_CURRENT_SOURCE_DIR}/src)

option(test "Build test with google test" OFF)
option(documentation "Generate documentation with doxygen" OFF)


# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

find_package(PkgConfig)

find_package(Qt5 COMPONENTS Core Widgets Gui OpenGL REQUIRED)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

pkg_check_modules(OPENMESH openmesh)


find_package(Git)

# fallback for OPENMESH
if (NOT OPENMESH_FOUND)
  if (NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/external/openmesh/CMakeLists.txt)
    if (NOT Git_FOUND)
      message(FATAL_ERROR "Git cannot be found and OPENMESH submodule is not init")
    else()
      message("Init OPENMESH subprojects")
      execute_process(COMMAND ${GIT_EXECUTABLE} submodule init external/openmesh
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
      execute_process(COMMAND ${GIT_EXECUTABLE} submodule update external/openmesh
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    endif()
  endif()

  add_subdirectory(external/openmesh)
endif()

find_package(glm CONFIG QUIET)

if (NOT glm_FOUND)

  if (NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/external/glm/CMakeLists.txt)
    if (NOT Git_FOUND)
      message(FATAL_ERROR "Git cannot be found and glm submodule is not init")
    else()
      message("Init glm subprojects")
      execute_process(COMMAND ${GIT_EXECUTABLE} submodule init external/glm
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

      execute_process(COMMAND ${GIT_EXECUTABLE} submodule update external/glm
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

  endif()

  add_subdirectory(external/glm)

endif()

# OpenMeshRender support
find_package(OpenGLMeshRender CONFIG QUIET)

if (NOT OpenGLMeshRender_FOUND)

  if (NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/external/openglmeshrender/CMakeLists.txt)
    if (NOT Git_FOUND)
      message(FATAL_ERROR "Git cannot be found and OpenGLMeshRender submodule is not init")
    else()
      message("Init OpenGLMeshRender subprojects")
      execute_process(COMMAND ${GIT_EXECUTABLE} submodule init external/openglmeshrender
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

      execute_process(COMMAND ${GIT_EXECUTABLE} submodule update external/openglmeshrender
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

  endif()

  add_subdirectory(external/openglmeshrender)

endif()

# OpenGL shader utility
find_package(OpenGLShader CONFIG QUIET)

if (NOT OpenGLShader_FOUND)

  if (NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/external/openglshader/CMakeLists.txt)
    if (NOT Git_FOUND)
      message(FATAL_ERROR "Git cannot be found and OpenGLShader submodule is not init")
    else()
      message("Init OpenGLShader subprojects")
      execute_process(COMMAND ${GIT_EXECUTABLE} submodule init external/openglshader
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

      execute_process(COMMAND ${GIT_EXECUTABLE} submodule update external/openglshader
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

  endif()

  add_subdirectory(external/openglshader)

endif()

add_subdirectory(src)



# test
if (test)

  if (NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/external/googletest/CMakeLists.txt)
    if (NOT Git_FOUND)
      message(FATAL_ERROR "Git cannot be found and googletest submodule is not init")
    else()
      message("Init googletest subprojects")

      execute_process(COMMAND ${GIT_EXECUTABLE} submodule init external/googletest
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

      execute_process(COMMAND ${GIT_EXECUTABLE} submodule update external/googletest
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    endif()
  endif()

  add_subdirectory(external/googletest)
  enable_testing()

  add_subdirectory(tests)

endif()

# documentation
if (documentation)
  find_package(Doxygen REQUIRED)

  set(DOXYGEN_GENERATE_XML YES)
  set(DOXYGEN_GENERATE_HTML YES)

  doxygen_add_docs(doxygen
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
    ${CMAKE_CURRENT_SOURCE_DIR}/external/openglmeshrender
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

endif()
