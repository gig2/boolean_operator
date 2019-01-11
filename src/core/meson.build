core_sources = files(['intersect_mesh.cpp',
                      'mesh.cpp',])

core_headers = files(['bbox.h',
                      'compute_octree.h',
                      'intersect_mesh.h',
                      'mesh.h',
                      'mesh_face_bbox.h',
                      'octree.h',
                      ])

core_include_dir = include_directories('./')


boolean_operator_core_lib = library('boolean_operator_core',
                        sources: [core_sources,
                                  core_headers],

        dependencies: [ openmesh_dep]
        , include_directories: [core_include_dir]
                       )


boolean_operator_core_dep = declare_dependency(
                              link_with: boolean_operator_core_lib,
                              dependencies: openmesh_dep,
                              include_directories: [core_include_dir])
