


add_subdirectory(fpng)


add_library(external SHARED $<TARGET_OBJECTS:fpng>)



