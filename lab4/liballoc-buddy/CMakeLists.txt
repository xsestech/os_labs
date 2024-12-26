project(liballoc-buddy C)

set(LIBNAME liballoc)
include(lib)
file(GLOB sources "${PROJECT_SOURCE_DIR}/${LIBNAME}/*.c"
        "${PROJECT_SOURCE_DIR}/${LIBNAME}/*.h"
        "${PROJECT_SOURCE_DIR}/${LIBNAME}/*/*.h"
        "${PROJECT_SOURCE_DIR}/${LIBNAME}/*/*.c")
list(FILTER sources EXCLUDE REGEX ".*test\\.c$")

add_library(${PROJECT_NAME} SHARED ${sources})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})