# CMakeList.txt : CMake project for Rocket Launch Simulator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

Set(EXEC_NAME
	"Spring")
project (${EXEC_NAME})

# GLFW BUILD
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("Dependencies/GLFW/")

include_directories(
	"Dependencies/stbi_image/"
	"Dependencies/GLEW/include/"
	"Dependencies/GLFW/include/"
	"Dependencies/imgui/"
	"Dependencies/imgui/backends/"
	"Dependencies/ImGuizmo/"
	"Dependencies/GLM/"
	"Application/"
	"Engine/"
	"Input/"
	"LightAndShadow/"
	"Materials/"
	"Objects/"
	"Rendering/"
	"Shaders/"
	"Textures/"
	"Utility/"
	"Viewing/"
)

Set(SOURCE_FILES 
	"SpinningTriangleModified.cpp"

	# DEPENDENCIES
	"Dependencies/GLEW/include/GL/glew.h"
	"Dependencies/GLEW/src/glew.c"
	#"Dependencies/stbi_image/stbi_image.h"
	"Dependencies/ImGui/imgui.h"
	"Dependencies/ImGui/imconfig.h"
	"Dependencies/ImGui/imgui.cpp"
	"Dependencies/ImGui/imgui_draw.cpp"
	"Dependencies/ImGui/imgui_tables.cpp"
	"Dependencies/ImGui/imgui_widgets.cpp"
	"Dependencies/ImGui/imgui_internal.h"
	"Dependencies/ImGui/backends/imgui_impl_opengl3.h"
	"Dependencies/ImGui/backends/imgui_impl_opengl3_loader.h"
	"Dependencies/ImGui/backends/imgui_impl_opengl3.cpp"
	"Dependencies/ImGui/backends/imgui_impl_glfw.h"
	"Dependencies/ImGui/backends/imgui_impl_glfw.cpp"

	"Dependencies/ImGuizmo/ImGuizmo.h"
	"Dependencies/ImGuizmo/ImGuizmo.cpp"
	
	# CORE APPLICATION INCLUDES
	"Application/Application.h"
	"Application/Application.cpp"
	# ENGINE INCLUDES
	"Engine/BaseApplication.h"
	"Engine/BaseApplication.cpp"
	"Engine/Layer.h"
	"Engine/Layer.cpp"
	"Engine/TimeStep.h"
	"Engine/TimeStep.cpp"
	"Engine/UILayer.h"
	"Engine/UILayer.cpp"
	# INPUT
	"Input/InputDevice.h"
	"Input/InputDevice.cpp"
	"Input/Keyboard.h"
	"Input/Keyboard.cpp"
	"Input/Mouse.h"
	"Input/Mouse.cpp"
	# LIGHTS AND SHADOW
	"LightAndShadow/PointLight.h"
	"LightAndShadow/PointLight.cpp"

	# Materials
	"Materials/Material.h"
	"Materials/Material.cpp"
	"Materials/Shader.h"
	"Materials/Shader.cpp"
	"Materials/Texture.h"
	"Materials/Texture.cpp"

	# RENDERING
	"Rendering/FrameBuffer.h"
	"Rendering/FrameBuffer.cpp"
	"Rendering/Renderer.h"
	"Rendering/Renderer.cpp"
	"Rendering/Scene.h"
	"Rendering/Scene.cpp"
	"Rendering/VertexArray.h"
	"Rendering/VertexArray.cpp"

	# UTILITIES
	"Utility/ObjectLoader.h"
	"Utility/ObjectLoader.cpp"
	"Utility/Serializer.h"
	"Utility/Serializer.cpp"

	# VIEWING
	"Viewing/Camera.h"
	"Viewing/Camera.cpp"
	"Viewing/OrthographicCamera.h"
	"Viewing/OrthographicCamera.cpp"
	"Viewing/PerspectiveCamera.h"
	"Viewing/PerspectiveCamera.cpp"

	# APPLICATIONS
	"Application/Cube.h"
	"Application/Cube.cpp"
	"Application/Grid.h"
	"Application/Grid.cpp"
	"Application/Moon.h"
	"Application/Moon.cpp"
	"Application/Square.h"
	"Application/Square.cpp"
	"Application/Tank.h"
	"Application/Tank.cpp"
	"Application/Teaport.h"
	"Application/Teaport.cpp"

)
	
# Add source to this project's executable
add_executable (${EXEC_NAME} ${SOURCE_FILES} )

find_package(OpenGL REQUIRED)
target_link_libraries(${EXEC_NAME} 
	${OPENGL_gl_LIBRARY}
	glfw
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${EXEC_NAME} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
