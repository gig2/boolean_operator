project('boolean_operator','cpp', default_options : ['cpp_std=c++17'])

glm_dep = dependency('glm',required:true)
glew_dep = dependency('glew', required:true)

qt5_mod = import('qt5')
qt5_widgets_dep = dependency('qt5', modules : 'Widgets', required:true)
qt5_opengl_dep = dependency('qt5' , modules : 'OpenGL', required:true)
qt5_gui_dep = dependency('qt5', modules: 'Gui', required:true)


openmesh_dep = dependency('openmesh', required:true)


openglmeshrender_dep = dependency('OpenGLMeshRender', fallback: ['openglmeshrender', 'openglmeshrender_dep'])

openglshader_dep = dependency('OpenGLShader', fallback: ['openglshader', 'openglshader_dep'])

test_option = get_option('test')
if (test_option)
  gtest_dep = dependency('gtest', required:false,fallback:['gtest','gtest_dep'])
  gmock_dep = dependency('gmock', required:false,fallback:['gtest','gmock_dep'])
endif

includeDir = include_directories('src')

subdir('src')

if (test_option)
  subdir('tests')
endif

documentation_option = get_option('documentation')
# doxygen support
if (documentation_option)
  doxygen = find_program('doxygen' , required: true)
  doxyfile_conf = configuration_data()
  doxyfile_conf.set('PACKAGE_NAME', meson.project_name())
  doxyfile_conf.set('PACKAGE_VERSION', meson.project_version())
  doxyfile_conf.set('top_srcdir', meson.source_root())
  doxyfile_conf.set('top_builddir', meson.build_root())

  dot = find_program('dot', required: false)

  if dot.found()
      doxyfile_conf.set('HAVE_DOT', 'YES')
  else
      doxyfile_conf.set('HAVE_DOT', 'NO')
  endif

  doxyfile = configure_file(input: 'Doxyfile.in',
                            output: 'Doxyfile',
                            configuration: doxyfile_conf,
                            install: false)

  doxygen_target = custom_target('doxygen',
                                 input: [doxyfile],
                                 output: ['docs'],
                                 command: [ doxygen, doxyfile ],
                                 install: false)
endif
