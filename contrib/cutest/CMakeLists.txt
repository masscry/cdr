project (cutest)

set (PROJ_SOURCE cutest.c)

set (PROJ_HEADER cutest.h)

add_library(${PROJECT_NAME} STATIC ${PROJ_SOURCE} ${PROJ_HEADER})