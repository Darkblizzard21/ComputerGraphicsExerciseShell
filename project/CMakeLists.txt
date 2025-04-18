cmake_minimum_required(VERSION 3.10)
project(OpenGLViewer)

# Setze rekursive Quelle
file(GLOB_RECURSE SOURCES
    core/*.cpp
    render/*.cpp
)

# Executable mit allen Quellcodes
add_executable(OpenGLViewer ${SOURCES})

# Kopiere den gesamten assets-Ordner in das Ausgabeverzeichnis
add_custom_command(TARGET OpenGLViewer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:OpenGLViewer>/assets")

# Include‑Verzeichnisse
target_include_directories(OpenGLViewer PRIVATE
    ${CMAKE_SOURCE_DIR}/core
    ${CMAKE_SOURCE_DIR}/render
    ${CMAKE_SOURCE_DIR}/app

    ${CMAKE_SOURCE_DIR}/imported/glad/include
    ${CMAKE_SOURCE_DIR}/imported/glfw/include
    ${CMAKE_SOURCE_DIR}/imported/imgui        
    ${CMAKE_SOURCE_DIR}/imported/stb          
    ${CMAKE_SOURCE_DIR}/imported/assimp/include
)

# Linker‑Bibliotheken
target_link_libraries(OpenGLViewer PRIVATE
    glad
    glfw
    imgui
    assimp              
    glm::glm            
)
