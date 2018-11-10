set( gui_sources
  mainWindow.cpp
  mainopenglwidget.cpp)


qt5_wrap_cpp( gui_moc_headers
   mainWindow.h
   mainopenglwidget.h)


qt5_wrap_ui( gui_ui
    ui/mainWindow.ui)


add_library(boolean_operator_gui ${gui_sources}  ${gui_ui}
                      ${gui_moc_headers})

target_include_directories(boolean_operator_gui PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

target_include_directories(boolean_operator_gui PUBLIC
  $<BUILD_INTERFACE:${includeDir}>
  $<INSTALL_INTERFACE:include)

target_link_libraries(boolean_operator_gui PUBLIC
  Qt5::Widgets
  Qt5::Gui
  Qt5::OpenGL
  GLEW::GLEW
  ${OPENGL_LIBRARIES}
  boolean_operator_core
  OpenGLShader
  OpenGLMeshRender)

set_target_properties(boolean_operator_gui PROPERTIES LINK_FLAGS
  "${OPENMESH_LDFLAGS}")
