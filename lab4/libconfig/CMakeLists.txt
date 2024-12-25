project(libconfig C)

file(GLOB sources "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/*.c"
        "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/*.h"
        "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/*/*.h"
        "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/*/*.c")
list(FILTER sources EXCLUDE REGEX ".*test\\.c$")

add_library(${PROJECT_NAME} INTERFACE ${sources})
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR})
set(ENV{CHILD_EXEC_PATH} "${CMAKE_BINARY_DIR}/child/child")