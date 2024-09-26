set(LIB network)
set(variable_for_father ${LIB} PARENT_SCOPE)
aux_source_directory(src SRC)
# 遍历源文件列表并为每个源文件创建一个共享库
foreach (src ${SRC})
    get_filename_component(src_name ${src} NAME_WE)
    add_library(${src_name} SHARED ${src})
    target_include_directories(${src_name} PUBLIC include)
endforeach ()

add_library(${LIB} INTERFACE)

foreach (src ${SRC})
    get_filename_component(src_name ${src} NAME_WE)
    target_link_libraries(${LIB} INTERFACE ${src_name})
endforeach ()
target_include_directories(
        ${LIB}
        INTERFACE include
)