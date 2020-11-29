AUX_SOURCE_DIRECTORY(../nodes nodesRC)

add_library(U3D_Core U3D_Functions.c U3D_Functions.h ../u3d_context.c ../u3d_context.h ${nodesRC} ../u3d.c)
target_link_libraries(U3D_Core Utils)
target_link_libraries(U3D_Core ADT)