cmake_minimum_required(VERSION 3.10)
#project(OpenGLViewer)

add_executable(project
    main.cpp
    ...
)
# Setze rekursive Quelle
file(GLOB_RECURSE SOURCES
    core/*.cpp
    render/*.cpp
)

# copy assets
add_custom_command(TARGET project POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:project>/assets
)

# include directories
target_include_directories(project
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}   
    ${CMAKE_SOURCE_DIR}/imported/imgui/imgui
    ${CMAKE_SOURCE_DIR}/imported/imgui/imgui/backends
    ${CMAKE_SOURCE_DIR}/project/core
    ${CMAKE_SOURCE_DIR}/project/render
    ${CMAKE_SOURCE_DIR}/project/shaders
    ${CMAKE_SOURCE_DIR}/imported/glad/include
    ${CMAKE_SOURCE_DIR}/imported/glfw/include
    ${CMAKE_SOURCE_DIR}/imported/tinygltf/include      
    ${CMAKE_SOURCE_DIR}/imported/stbi/include       
    ${CMAKE_SOURCE_DIR}/imported/assimp/include
)

target_link_libraries(project
    PRIVATE
    glad
    glfw
    glm::glm
    imgui
    tinygltf
    stbi
)

