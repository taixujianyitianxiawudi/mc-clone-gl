cmake_minimum_required(VERSION 3.28)
project(untitled3)

set(CMAKE_CXX_STANDARD 20)
set(SFML_DIR "E:/SFML-2.6.1/lib/cmake/SFML")
# Find SFML
find_package(SFML 2.5 COMPONENTS system window graphics network REQUIRED)
add_definitions(-DSFML_STATIC)
# Add the executable
add_executable(untitled3 main.cpp
        voxel_engine.cpp
        voxel_engine.hpp
        #Shader.cpp
        #Shader.hpp
        settings.hpp
        settings.cpp
        Camera.cpp
        Camera.hpp
        Player.cpp
        Player.hpp
        meshes/quad_mesh.cpp
        meshes/quad_mesh.hpp
        Scene.cpp
        Scene.hpp
        Shader.cpp
        Shader.hpp
        glad/glad.h
        meshes/chunk_mesh.cpp
        meshes/chunk_mesh.hpp
        world_objects/chunk.cpp
        world_objects/chunk.hpp
        meshes/chunk_mesh_builder.cpp
        meshes/chunk_mesh_builder.hpp
        Textures.cpp
        Textures.hpp
        World.cpp
        World.hpp
        assets/stb_image.h
)
target_include_directories(untitled3 PUBLIC "E:/SFML-2.6.1/include")
# Link SFML libraries
target_link_libraries(untitled3 sfml-system sfml-window sfml-graphics sfml-network)


set(SFML_DLL_DIR "E:/SFML-2.6.1/bin")

# Post-build step to copy the DLLs to the output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SFML_DLL_DIR}"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)


target_include_directories(untitled3 PUBLIC "E:/glfw-3.4.bin.WIN64/include")
target_include_directories(untitled3 PUBLIC "E:/glm")
target_include_directories(untitled3 PUBLIC src)
target_include_directories(untitled3 PUBLIC shaders)
target_include_directories (untitled3 PUBLIC "E:/glew-2.2.0/include")


#(untitled3 Vulkan::Vulkan "E:/glfw-3.4.bin.WIN64/lib-mingw-w64/libglfw3.a")
target_link_libraries(untitled3 "E:/glfw-3.4.bin.WIN64/lib-mingw-w64/glfw3.dll")
target_link_libraries(untitled3 "E:/glfw-3.4.bin.WIN64/lib-mingw-w64/libglfw3.a")
target_link_libraries(untitled3 "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22000.0/um/x64/OpenGL32.Lib")

#E:\glew-2.2.0\lib\Release\x64


target_link_libraries(untitled3 "E:/glew-2.2.0/lib/Release/x64/glew32.lib")
target_link_libraries(untitled3 "C:/Users/zzk/ClionProjects/untitled3/glad/glad.c")
