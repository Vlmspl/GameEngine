# CMakeList.txt : CMake project for GameEngine, include source and define
# project specific logic here.
#

# Add source to this project's executable.
file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

set(GLAD_SRC glad/src/glad.c)


add_executable (GameEngine ${SOURCES} ${HEADERS} ${GLAD_SRC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GameEngine PROPERTY CXX_STANDARD 20)
endif()

find_package(OpenGL REQUIRED)

# TODO: Add tests and install targets if needed.
add_subdirectory(external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)

target_link_libraries(GameEngine PRIVATE glfw ${OPENGL_gl_LIBRARY}) #${OPENGL_glu_LIBRARY} glu32)
target_include_directories(GameEngine PUBLIC ${PROJECT_SOURCE_DIR}/include PUBLIC glad/include)

# Define the path to the Shaders directory within the GameEngine folder
set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/GameEngine/Assets")

# Define the path to the build directory
set(BUILD_DIR "${CMAKE_BINARY_DIR}")

# Debugging output to check paths
message(STATUS "Source directory for assets: ${ASSETS_DIR}")
message(STATUS "Build directory: ${BUILD_DIR}")

# Copy the Shaders directory directly to the build directory
file(COPY ${ASSETS_DIR} DESTINATION ${BUILD_DIR}/GameEngine)