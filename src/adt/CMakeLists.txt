add_library(ADT function.c function.h figure.c figure.h vector3.c vector3.h)
target_link_libraries(ADT Utils)
target_link_libraries(ADT m)