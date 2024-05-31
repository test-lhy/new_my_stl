set(LIB normal)
set(variable_for_father ${LIB} PARENT_SCOPE)
aux_source_directory(src SRC)
add_library(${LIB} SHARED ${SRC})
target_include_directories(
        ${LIB}
        PUBLIC include
)