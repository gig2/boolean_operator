tools_headers = files(['shader.h'])

tools_sources = files(['shader.cpp'])

boolean_operator_tools_lib = library('boolean_operator_tools',
                                     sources: [tools_headers, tools_sources],
                                     dependencies: [glew_dep],
                                    include_directories: includeDir)

boolean_operator_tools_dep = declare_dependency(link_with: boolean_operator_tools_lib,
                                               dependencies: [glew_dep])

