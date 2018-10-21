gui_source_file = files(['simpleWindow.cpp','mainWindow.cpp'])
gui_include_file = files([])

gui_moc_headers = files(['simpleWindow.h',
                         'mainWindow.h',
                        'mainopenglwidget.h'])

gui_ui_files = files(['ui/mainWindow.ui'])


gui_processed = qt5_mod.preprocess(
  moc_headers: [gui_moc_headers],
  ui_files: [gui_ui_files])

boolean_operator_gui_lib = library('boolean_operator_gui',
                                   sources: [gui_source_file,
                                             gui_include_file,
                                             gui_processed],
                                   dependencies: [qt5_widgets_dep,
                                                  qt5_gui_dep,
                                                  qt5_opengl_dep,
                                                  boolean_operator_tools_dep,
                                                  boolean_operator_mesh_dep,
                                                 ],
                                  include_directories: includeDir)

boolean_operator_gui_dep = declare_dependency(link_with: boolean_operator_gui_lib,
                                   dependencies: [qt5_widgets_dep,
                                                  qt5_gui_dep,
                                                  qt5_opengl_dep,
                                                  boolean_operator_tools_dep,
                                                  boolean_operator_mesh_dep,
                                                 ])
