install(
    TARGETS fractal-generator_exe
    RUNTIME COMPONENT fractal-generator_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
