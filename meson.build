project('testOpenMesh','cpp', default_options : ['cpp_std=c++17'])

sdl2_dep = dependency('sdl2',required:true)
sdl2image_dep = dependency('SDL2_image',required:true)
glew_dep = dependency('glew',required:true)
glm_dep = dependency('glm',required:true)

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
                          glew_dep,
                          glm_dep,
                          openmesh_dep],
            include_directories: includeDir
                  )

