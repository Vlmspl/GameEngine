# CMakeList.txt : CMake project for GameEngine, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (GameEngine "GameEngine.cpp" "GameEngine.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GameEngine PROPERTY CXX_STANDARD 20)
endif()

find_package(OpenGL REQUIRED)

# TODO: Add tests and install targets if needed.
add_subdirectory(external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)

target_link_libraries(GameEngine PRIVATE glfw ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} glu32)