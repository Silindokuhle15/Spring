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
add_compile_definitions(-DCMAKE_TOOLCHAIN_FILE=VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake )
add_compile_definitions(-D__LEAN_AND_MEAN__)
add_compile_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_compile_definitions(-DFBXSDK_SHARED)
#add_compile_definitions(-DSTB_IMAGE_IMPLEMENTATION)
add_compile_definitions(-DGLFW_INCLUDE_NONE)

include_directories(
	"E:/FBX/include/"
	"E:/PhysX/physx/include/"
	"Dependencies/stbi_image/"
	"Dependencies/GLEW/include/"
	"Dependencies/GLFW/include/"
	"Dependencies/imgui/"
	"Dependencies/imgui/backends/"
	"Dependencies/ImGuizmo/"
	"Dependencies/GLM/"
	"Dependencies/Lua/include/"
	"Dependencies/FREETYPE/include/"
	"Dependencies/MSDF/msdfgen/"
	"Dependencies/MSDF/msdfgen/core/"
	"Dependencies/MSDF/msdfgen/ext/"
	"Dependencies/MSDF/msdf-atlas-gen/"
	"Dependencies/tiny/"
	"Core/Application/"
	"Core/Common/"
	"Core/Engine/"
	"Core/Event"
	"Core/Input/"
	"Core/Materials/"
	"Core/Physics/"
	"Core/Project"
	"Core/Rendering/"
	"Core/SceneMgr/"
	"Core/Scripting/"
	"Core/Viewing/"
	"Editor/"
	"Editor/Utility/"
)

file(GLOB MSDF_CR_INC "Dependencies/MSDF/msdfgen/core/*.h")
file(GLOB MSDF_CR_SRC "Dependencies/MSDF/msdfgen/core/*.cpp")
file(GLOB MSDF_XT_INC "Dependencies/MSDF/msdfgen/ext/*.h")
file(GLOB MSDF_XT_SRC "Dependencies/MSDF/msdfgen/ext/*.cpp")
file(GLOB MSDG_ATGN_INC "Dependencies/MSDF/msdfgen-atlas-gen/*.h")
file(GLOB MSDG_ATGN_SRC "Dependencies/MSDF/msdfgen-atlas-gen/*.cpp")

Set(SOURCE_FILES 
	"SpringEditor.cpp"

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
	"Dependencies/ImGui/backends/imgui_impl_win32.h"
	"Dependencies/ImGui/backends/imgui_impl_win32.cpp"

	"Dependencies/ImGuizmo/ImGuizmo.h"
	"Dependencies/ImGuizmo/ImGuizmo.cpp"

	${MSDF_CR_INC}
	${MSDF_CR_SRC}
	${MSDF_XT_INC}
	${MSDF_XT_SRC}
	${MSDG_ATGN_INC}
	${MSDG_ATGN_SRC}
	
	# CORE APPLICATION INCLUDES
	
	# COMMON
	"Core/Common/Common.h"
	"Core/Common/Common.cpp"
	# ENGINE INCLUDES
	"Core/Engine/BaseApplication.h"
	"Core/Engine/BaseApplication.cpp"

	"Core/Engine/TimeStep.h"
	"Core/Engine/TimeStep.cpp"
	"Core/Engine/Window.h" 
	"Core/Engine/Window.cpp"
	"Core/Engine/NGLFWwindow.h"
	"Core/Engine/NGLFWwindow.cpp"
	
	# EVENTS
	"Core/Event/Event.h" 
	"Core/Event/Event.cpp"

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
	"Core/Materials/TextureBase.h"
	"Core/Materials/TextureBase.cpp"
	"Core/Materials/Material.h"
	"Core/Materials/Material.cpp"
	"Core/Materials/Shader.h"
	"Core/Materials/Shader.cpp"
	"Core/Materials/Texture.h"
	"Core/Materials/Texture.cpp"

	# PHYSICS SIMULATION
	 "Core/Physics/Collider.h" 
	 "Core/Physics/Collider.cpp"
	 "Core/Physics/PhysicsEngine.h" 
	 "Core/Physics/PhysicsEngine.cpp"

	# PROJECTS
	"Core/Project/Project.h" 
	"Core/Project/Project.cpp"

	# RENDERING
	"Core/Rendering/OpenGLFrameBuffer.h"
	"Core/Rendering/OpenGLFrameBuffer.cpp"
	"Core/Rendering/Renderer.h"
	"Core/Rendering/Renderer.cpp"
	"Core/Rendering/VertexArray.h"
	"Core/Rendering/VertexArray.cpp"

	# SCENE MANAGER
	"Core/SceneMgr/Scene.h"
	"Core/SceneMgr/Scene.cpp"
	"Core/SceneMgr/SceneEvent.h" 
	"Core/SceneMgr/SceneEvent.cpp" 

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
	
	
	
	

	# APPLICATIONS
	"Core/Application/Grid.h"
	"Core/Application/Grid.cpp"
	"Core/Engine/Mesh.h"
	"Core/Engine/Mesh.cpp"
	"Core/Application/Square.h"
	"Core/Application/Square.cpp"
	
	# EDITOR 
	"Editor/Layer.h"
	"Editor/Layer.cpp"
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

SET(FBX_LIB_DIR
	"E:/FBX/lib/x64/debug/libfbxsdk.lib"
)

SET(FT_LIB_DIR
	"${PROJECT_SOURCE_DIR}/Dependencies/FREETYPE/release static/vs2015-2022/win64/freetype.lib"
)

SET(PHYSX_LIB_DIR
	"E:/PhysX/physx/bin/win.x86_64.vc143.mt/debug/PhysXDevice64.dll"
)
	
# Add source to this project's executable
add_executable (${EXEC_NAME} ${SOURCE_FILES} )

find_package(OpenGL REQUIRED)
target_link_libraries(${EXEC_NAME} 
	${OPENGL_gl_LIBRARY}
	${LUA_LIB_DIR}
	glfw
	${FBX_LIB_DIR}
	${FT_LIB_DIR}
	#${PHYSX_LIB_DIR}
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${EXEC_NAME} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
