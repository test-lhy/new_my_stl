set(LIB databases)
set(variable_for_father ${LIB} PARENT_SCOPE)
aux_source_directory(src SRC)
add_library(${LIB} SHARED ${SRC})

target_include_directories(
        ${LIB}
        PUBLIC /usr/include/mysql-cppconn
        PUBLIC include
)
target_link_libraries(
        ${LIB}
        PUBLIC /usr/lib/x86_64-linux-gnu/libmysqlcppconnx.so
)