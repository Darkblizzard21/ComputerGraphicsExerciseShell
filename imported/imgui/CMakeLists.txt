project(imgui)
add_library(imgui STATIC)
TARGET_SOURCES(imgui 
    PRIVATE
    imgui/imconfig.h
    imgui/imgui.h
    imgui/imgui_internal.h
    imgui/backends/imgui_impl_glfw.h
    imgui/backends/imgui_impl_opengl3.h

    
    imgui/imgui.cpp    
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    )

    
target_include_directories(imgui PUBLIC imgui)

target_link_libraries(imgui
    PRIVATE
    glfw
)