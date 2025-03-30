set(GLFW_BUILD_DOCS OFF)
# on linux turn on the backend you prefer/have installed
set(GLFW_BUILD_X11 OFF)
set(GLFW_BUILD_WAYLAND OFF)

add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(imgui)
add_subdirectory(nlohmannjson)
add_subdirectory(stbi)
add_subdirectory(tinygltf)


set_target_properties(glad PROPERTIES FOLDER "imported")
set_target_properties(glfw PROPERTIES FOLDER "imported")
set_target_properties(imgui PROPERTIES FOLDER "imported")
set_target_properties(uninstall PROPERTIES FOLDER "imported/GLFW3")
set_target_properties(update_mappings PROPERTIES FOLDER "imported/GLFW3")
set_target_properties(json PROPERTIES FOLDER "imported")
set_target_properties(stbi PROPERTIES FOLDER "imported")
set_target_properties(tinygltf PROPERTIES FOLDER "imported")