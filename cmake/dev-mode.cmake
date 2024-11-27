include(cmake/folders.cmake)

include(CTest)
add_subdirectory(test)
add_subdirectory(benchmark)

add_custom_target(
    run-exe
	COMMAND fractal-generator-exe 
    VERBATIM
)
add_dependencies(run-exe fractal-generator_exe)

include(cmake/lint-targets.cmake)
include(cmake/spell-targets.cmake)

add_folders(Project)
