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
	"Spring"
)
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
	"Dependencies/Lua/include/"
	"Core/Application/"
	"Core/Engine/"
	"Core/Input/"
	"Core/Materials/"
	"Core/Rendering/"
	"Core/Scripting/"
	"Core/Viewing/"
	"Editor/"
	"Editor/Utility/"
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
	"Core/Application/Application.h"
	"Core/Application/Application.cpp"
	# ENGINE INCLUDES
	"Core/Engine/BaseApplication.h"
	"Core/Engine/BaseApplication.cpp"
	"Editor/Layer.h"
	"Editor/Layer.cpp"
	"Core/Engine/TimeStep.h"
	"Core/Engine/TimeStep.cpp"

	# INPUT
	"Core/Input/InputDevice.h"
	"Core/Input/InputDevice.cpp"
	"Core/Input/Keyboard.h"
	"Core/Input/Keyboard.cpp"
	"Core/Input/Mouse.h"
	"Core/Input/Mouse.cpp"
	# LIGHTS AND SHADOW
	"Core/Rendering/LightAndShadow/PointLight.h"
	"Core/Rendering/LightAndShadow/PointLight.cpp"

	# Materials
	"Core/Materials/Material.h"
	"Core/Materials/Material.cpp"
	"Core/Materials/Shader.h"
	"Core/Materials/Shader.cpp"
	"Core/Materials/Texture.h"
	"Core/Materials/Texture.cpp"

	# RENDERING
	"Core/Rendering/FrameBuffer.h"
	"Core/Rendering/FrameBuffer.cpp"
	"Core/Rendering/Renderer.h"
	"Core/Rendering/Renderer.cpp"
	"Core/Rendering/Scene.h"
	"Core/Rendering/Scene.cpp"
	"Core/Rendering/VertexArray.h"
	"Core/Rendering/VertexArray.cpp"

	# SCRIPTING
	"Core/Scripting/ScriptingEngine.h"
	"Core/Scripting/ScriptingEngine.cpp"

	# UTILITIES
	"Editor/Utility/ObjectLoader.h"
	"Editor/Utility/ObjectLoader.cpp"
	"Editor/Utility/Serializer.h"
	"Editor/Utility/Serializer.cpp"

	# VIEWING
	"Core/Viewing/Camera.h"
	"Core/Viewing/Camera.cpp"
	"Core/Viewing/OrthographicCamera.h"
	"Core/Viewing/OrthographicCamera.cpp"
	"Core/Viewing/PerspectiveCamera.h"
	"Core/Viewing/PerspectiveCamera.cpp"

	# APPLICATIONS
	
	
	"Core/Application/Grid.h"
	"Core/Application/Grid.cpp"
	"Core/Engine/Mesh.h"
	"Core/Engine/Mesh.cpp"
	"Core/Application/Square.h"
	"Core/Application/Square.cpp"
	
	
	
	

	# EDITOR 

	"Editor/UILayer.h"
	"Editor/UILayer.cpp"

)


## BUILD LUA
set(LIB_NAME
	"lua54"
)
set(LUA_LIB_DIR
	"${PROJECT_SOURCE_DIR}/Dependencies/Lua/${LIB_NAME}.lib"
)
	
# Add source to this project's executable
add_executable (${EXEC_NAME} ${SOURCE_FILES} )

find_package(OpenGL REQUIRED)
target_link_libraries(${EXEC_NAME} 
	${OPENGL_gl_LIBRARY}
	${LUA_LIB_DIR}
	glfw
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${EXEC_NAME} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
