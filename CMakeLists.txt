cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME Pong)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/render/ShaderProgram.cpp
    src/render/ShaderProgram.h
    src/resources/ResourceManager.cpp
    src/resources/ResourceManager.h
    src/resources/stb_image.h 
    "src/render/Texture2D.h"
    "src/render/Texture2D.cpp" 
    "src/render/Sprite.h" 
    "src/render/Sprite.cpp"
    "src/objects/GameObject.h" 
    "src/objects/GameObject.cpp" 
    "src/objects/Ball.h" 
    "src/objects/Ball.cpp")

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} glm)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)

