AUX_SOURCE_DIRECTORY(utils utilsSRC)
AUX_SOURCE_DIRECTORY(adt adtSRC)
AUX_SOURCE_DIRECTORY(nodes nodesSRC)

add_library(U3DC_LIB u3d.c
        ${nodesSRC}
        ${adtSRC}
        ${utilsSRC})