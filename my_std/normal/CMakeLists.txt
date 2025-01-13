set(LIB normal)
set(variable_for_father ${LIB} PARENT_SCOPE)
aux_source_directory(src SRC)
# 遍历源文件列表并为每个源文件创建一个共享库
add_library(${LIB} SHARED ${SRC})
target_include_directories(
        ${LIB}
        PUBLIC include
)