
file(GLOB_RECURSE CPP_SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.h)
file(GLOB_RECURSE CUDA_SOURCES src/*.cu)

add_library(raytracing ${CPP_SOURCES} ${CUDA_SOURCES})
target_include_directories(raytracing PRIVATE include)
target_link_libraries(raytracing)

