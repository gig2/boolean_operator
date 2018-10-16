project('testOpenMesh','cpp', default_options : ['cpp_std=c++17'])

glm_dep = dependency('glm',required:true)
glew_dep = dependency('glew', required:true)

qt5_mod = import('qt5')
qt5widgets = dependency('qt5', modules : 'Widgets')
qt5opengl = dependency('qt5' , modules : 'OpenGL')


openmesh_dep = dependency('openmesh', required:true)


subdir('src')

includeDir = include_directories('src')


processed = qt5_mod.preprocess(
  moc_headers : [src_moc_headers],
  ui_files : [src_ui_files],
)

executable('testOpenMesh', sources: [source_file,
                                     gui_source_file,
                                     gui_include_file
                                     , processed],
           dependencies: [
                          qt5widgets,
                          qt5opengl,
                          glm_dep,
                          openmesh_dep,
                          glew_dep,
                          ],
            include_directories: includeDir
                  )

