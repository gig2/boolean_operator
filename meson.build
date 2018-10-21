project('boolean_operator','cpp', default_options : ['cpp_std=c++17'])

glm_dep = dependency('glm',required:true)
glew_dep = dependency('glew', required:true)

qt5_mod = import('qt5')
qt5_widgets_dep = dependency('qt5', modules : 'Widgets', required:true)
qt5_opengl_dep = dependency('qt5' , modules : 'OpenGL', required:true)
qt5_gui_dep = dependency('qt5', modules: 'Gui', required:true)


openmesh_dep = dependency('openmesh', required:true)

includeDir = include_directories('src')

subdir('src')



# processed = qt5_mod.preprocess(
#   moc_headers : [src_moc_headers],
#   ui_files : [src_ui_files],
# )

# executable('testOpenMesh', sources: [source_file,
#                                      source_header,
#                                      gui_source_file,
#                                      gui_include_file
#                                      , processed],
#            dependencies: [
#                           qt5widgets,
#                           qt5opengl,
#                           glm_dep,
#                           openmesh_dep,
#                           glew_dep,
#                           ],
#             include_directories: includeDir
#                   )

