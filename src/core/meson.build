core_sources = files(['mesh.cpp',])

core_headers = files(['mesh.h',])


boolean_operator_core_lib = library('boolean_operator_core',
                        sources: [core_sources,
                                  core_headers],

        dependencies: [ openmesh_dep]
        , include_directories: [includeDir]
                       )


boolean_operator_core_dep = declare_dependency(
                              link_with: boolean_operator_core_lib,
                              dependencies: openmesh_dep,
                              include_directories: [includeDir])
