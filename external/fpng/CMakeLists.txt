
file(GLOB_RECURSE CPP_SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.h)


add_library(fpng ${CPP_SOURCES} )
target_include_directories(fpng PRIVATE include)
target_compile_definitions(fpng PUBLIC FPNG_NO_SSE=1)
target_link_libraries(fpng)

