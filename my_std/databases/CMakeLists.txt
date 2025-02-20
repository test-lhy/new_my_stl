#set(LIB databases)
#set(variable_for_father ${LIB} PARENT_SCOPE)
#aux_source_directory(src SRC)
#add_library(${LIB} SHARED ${SRC})
#
#target_include_directories(
#        ${LIB}
#        PUBLIC /usr/include/mysql-cppconn
#        PUBLIC include
#)
#target_link_libraries(
#        ${LIB}
#        PUBLIC /usr/lib/x86_64-linux-gnu/libmysqlcppconnx.so
#)
set(LIB databases)
set(variable_for_father ${LIB} PARENT_SCOPE)
set(FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR /usr/local/mysql/connector-c++-9.1.0)
aux_source_directory(src SRC)
add_library(${LIB} SHARED ${SRC})

target_include_directories(
        ${LIB}
        PUBLIC include
        ${FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR}/include
)
target_link_libraries(
        ${LIB}
        ${FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR}/lib64/debug/libmysqlcppconnx.2.9.1.0.dylib
)