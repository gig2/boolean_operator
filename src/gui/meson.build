gui_source_file = files(['mainWindow.cpp', 'mainopenglwidget.cpp'])
gui_include_file = files([])

gui_moc_headers = files(['mainWindow.h',
                         'mainopenglwidget.h'])

gui_ui_files = files(['ui/mainWindow.ui'])



gui_processed = qt5_mod.preprocess(
  moc_headers: [gui_moc_headers],
  ui_files: [gui_ui_files])

gui_include_dir = include_directories('./')

boolean_operator_gui_lib = library('boolean_operator_gui',
                                   sources: [gui_source_file,
                                             gui_include_file,
                                             gui_processed],
                                   dependencies: [qt5_widgets_dep,
                                                  qt5_gui_dep,
                                                  qt5_opengl_dep,
                                                  openglmeshrender_dep,
                                                  openglshader_dep,
                                                  boolean_operator_core_dep,
                                                 ],
                                  include_directories: gui_include_dir)

boolean_operator_gui_dep = declare_dependency(link_with: boolean_operator_gui_lib,
                                   dependencies: [qt5_widgets_dep,
                                                  qt5_gui_dep,
                                                  qt5_opengl_dep,
                                                  openglmeshrender_dep,
                                                  openglshader_dep,
                                                  boolean_operator_core_dep,
                                                 ],
                                   include_directories: gui_include_dir)
