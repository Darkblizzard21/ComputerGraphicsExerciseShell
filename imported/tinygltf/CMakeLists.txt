project(tinygltf)

add_library(tinygltf STATIC)
target_sources(tinygltf
    PRIVATE
    include/tiny_gltf.h	

    tiny_gltf_build.cpp
)

target_include_directories(tinygltf PUBLIC include)
target_link_libraries(tinygltf 
    PUBLIC
    stbi 
    json
)

# disable external image loading
target_compile_definitions(tinygltf PUBLIC TINYGLTF_NO_STB_IMAGE TINYGLTF_NO_EXTERNAL_IMAGE)