mesh_sources = files(['meshnode.h'])
mesh_headers = files(['meshnode.cpp'])

boolean_operator_mesh_lib = library('boolean_operator_mesh',
                                    sources: [mesh_sources, mesh_headers],
                                    dependencies: [glew_dep, openmesh_dep],
                                   include_directories: includeDir)

boolean_operator_mesh_dep = declare_dependency(link_with: boolean_operator_mesh_lib,
                                              dependencies: [glew_dep, openmesh_dep])


