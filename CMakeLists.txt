# Minimal CMake for Wizard's Academy
cmake_minimum_required(VERSION 3.10)

project(WizardsAcademy LANGUAGES C CXX)

# Use a reasonably modern standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Where to look for headers bundled with the repo
include_directories("${CMAKE_SOURCE_DIR}/libs/include"
					"${CMAKE_SOURCE_DIR}/Wizard Academy/Header Files")

# Collect sources
file(GLOB_RECURSE PROJECT_SOURCES
	"${CMAKE_SOURCE_DIR}/Wizard Academy/Source Files/*.cpp"
	"${CMAKE_SOURCE_DIR}/glad.c"
)

# Add executable
add_executable(WizardAcademy ${PROJECT_SOURCES})

# Ensure GLAD C sources are explicitly added (handles spaces and glob quirks)
target_sources(WizardAcademy PRIVATE
	"${CMAKE_SOURCE_DIR}/libs/src/glad.c"
)

# On macOS we need to link the OpenGL framework. For other platforms, prefer OpenGL::GL
if(APPLE)
	find_library(OpenGL_FRAMEWORK OpenGL)
	if(OpenGL_FRAMEWORK)
		target_link_libraries(WizardAcademy PRIVATE ${OpenGL_FRAMEWORK})
	else()
		find_package(OpenGL REQUIRED)
		target_link_libraries(WizardAcademy PRIVATE ${OPENGL_gl_LIBRARY})
	endif()
else()
	find_package(OpenGL REQUIRED)
	target_link_libraries(WizardAcademy PRIVATE ${OPENGL_gl_LIBRARY})
endif()

# Use GLFW from our libs directory
set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libs/include")
set(GLFW_LIBRARY "${CMAKE_SOURCE_DIR}/libs/lib/libglfw.3.dylib")

message(STATUS "Using bundled GLFW: ${GLFW_LIBRARY}")
target_link_libraries(WizardAcademy PRIVATE ${GLFW_LIBRARY})

# Link Assimp static library
set(ASSIMP_LIBRARY "${CMAKE_SOURCE_DIR}/libs/lib/libassimp.a")
message(STATUS "Using bundled Assimp: ${ASSIMP_LIBRARY}")
target_link_libraries(WizardAcademy PRIVATE ${ASSIMP_LIBRARY})

# Add some useful compile flags for macOS to pick proper contexts
if(APPLE)
	target_compile_definitions(WizardAcademy PRIVATE GL_SILENCE_DEPRECATION)
	# Cocoa/IOKit/CoreVideo frameworks are required for OpenGL windowing on macOS
	target_link_libraries(WizardAcademy PRIVATE "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()

# Ensure the runtime linker can find our bundled libs at runtime on macOS by
# adding an rpath relative to the executable. This makes the binary load
# `libs/lib/libglfw.3.dylib` at runtime without extra environment variables.
set(BUNDLED_LIB_DIR "${CMAKE_SOURCE_DIR}/libs/lib")
set_target_properties(WizardAcademy PROPERTIES
	BUILD_RPATH "@executable_path/../libs/lib"
	INSTALL_RPATH "@executable_path/../libs/lib"
	BUILD_WITH_INSTALL_RPATH TRUE
)

# Enable experimental GLM extensions (some glm/gtx headers require this)
target_compile_definitions(WizardAcademy PRIVATE GLM_ENABLE_EXPERIMENTAL)

# GLM is header-only and already provided under libs/include/glm, nothing to link.

# Include folders for target (again for target-aware include paths)
target_include_directories(WizardAcademy PRIVATE
	"${CMAKE_SOURCE_DIR}/libs/include"
    "${CMAKE_SOURCE_DIR}/libs/src"
    "${CMAKE_SOURCE_DIR}/libs/lib"
	"${CMAKE_SOURCE_DIR}/Wizard Academy/Header Files"
)

# Compiler warnings (optional)
if(MSVC)
	target_compile_options(WizardAcademy PRIVATE /W3)
else()
	target_compile_options(WizardAcademy PRIVATE -Wall -Wextra -Wpedantic)
endif()

message(STATUS "Sources: ${PROJECT_SOURCES}")

# Copy shader source files into the build directory so the runtime can find
# shaders referenced by relative names like "basic.vs" and "basic.fs".
file(GLOB PROJECT_SHADERS
	"${CMAKE_SOURCE_DIR}/Wizard Academy/shaders/*"
)
if(PROJECT_SHADERS)
	message(STATUS "Installing shader files into build dir")
	file(COPY ${PROJECT_SHADERS} DESTINATION ${CMAKE_BINARY_DIR})
endif()

