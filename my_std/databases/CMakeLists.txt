set(LIB databases)
set(variable_for_father ${LIB} PARENT_SCOPE)
include(${CMAKE_SOURCE_DIR}/findpackage/findmycppconn8.cmake)
aux_source_directory(src SRC)
add_library(${LIB} SHARED ${SRC})

target_include_directories(
        ${LIB}
        PUBLIC MySQL::conncpp
        PUBLIC include
)
target_link_libraries(
        ${LIB}
        PUBLIC MySQL::conncpp -lmysqlcppconn8
)