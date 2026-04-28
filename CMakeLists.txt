cmake_minimum_required(VERSION 3.10.0)
project(my_game_on_mac VERSION 1.0.0)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(my_game_on_mac 
    MyGameInOpenGL/main.cpp
    MyGameInOpenGL/glad.c
    MyGameInOpenGL/app.cpp
    MyGameInOpenGL/camera.cpp
    MyGameInOpenGL/cube.cpp
    MyGameInOpenGL/cube_mesh.cpp
    MyGameInOpenGL/entity.cpp
    MyGameInOpenGL/fault_formation_terrain.cpp
    MyGameInOpenGL/ground.cpp
    MyGameInOpenGL/ground_mesh.cpp
    MyGameInOpenGL/material.cpp
    MyGameInOpenGL/midpoint_disp_terrain.cpp
    MyGameInOpenGL/physics.cpp
    MyGameInOpenGL/player.cpp
    MyGameInOpenGL/read_files.cpp
    MyGameInOpenGL/stb.cpp
    MyGameInOpenGL/terrain.cpp
    MyGameInOpenGL/texture_generator.cpp
    MyGameInOpenGL/triangle_list.cpp
    MyGameInOpenGL/util.cpp
    dependencies/imgui/imgui.cpp
    dependencies/imgui/imgui_draw.cpp
    dependencies/imgui/imgui_tables.cpp
    dependencies/imgui/imgui_widgets.cpp
    dependencies/imgui/imgui_impl_glfw.cpp
    dependencies/imgui/imgui_impl_opengl3.cpp
)

target_include_directories(my_game_on_mac PRIVATE dependencies) # PRIVATE keyword = only my_game gets to include items from dependencies directory, it's not propogated to targets that link to my_game

target_link_libraries(my_game_on_mac glfw OpenGL::GL) # NOTE: installed glfw3 with 'pacman -S mingw-w64-x86_64-glfw' in msys terminal, so that install of glfw is being referenced