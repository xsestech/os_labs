#include(dsym)
function(add_lib_auto)
    file(GLOB sources "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/*.c"
            "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/*.h"
            "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/*/*.h"
            "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/*/*.c")
    list(FILTER sources EXCLUDE REGEX ".*test\\.c$")

    add_library(${PROJECT_NAME} SHARED ${sources})
    target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
endfunction()