# Core/CMakeLists.txt
cmake_minimum_required(VERSION 3.21) 
# Project settings
set(LIB_NAME "Spring") 
project(${LIB_NAME})

# Include directories 
set(SPRING_INCLUDE_DIRS
	"${CMAKE_CURRENT_SOURCE_DIR}/Audio"
	"${CMAKE_CURRENT_SOURCE_DIR}/Common"
	"${CMAKE_CURRENT_SOURCE_DIR}/Engine"
	"${CMAKE_CURRENT_SOURCE_DIR}/Event"
	"${CMAKE_CURRENT_SOURCE_DIR}/FontMgr"
	"${CMAKE_CURRENT_SOURCE_DIR}/Input"
	"${CMAKE_CURRENT_SOURCE_DIR}/IO"
	"${CMAKE_CURRENT_SOURCE_DIR}/Materials"
	"${CMAKE_CURRENT_SOURCE_DIR}/Primitives"
	"${CMAKE_CURRENT_SOURCE_DIR}/Physics"
	"${CMAKE_CURRENT_SOURCE_DIR}/Rendering"
	"${CMAKE_CURRENT_SOURCE_DIR}/Resources"
	"${CMAKE_CURRENT_SOURCE_DIR}/SceneMgr"
	"${CMAKE_CURRENT_SOURCE_DIR}/Scripting"
	"${CMAKE_CURRENT_SOURCE_DIR}/Text"
	"${CMAKE_CURRENT_SOURCE_DIR}/UUID"
	"${CMAKE_CURRENT_SOURCE_DIR}/Viewing"
)

# Shared source files 
set(SHARED_SOURCES 
	"Common/Common.h"
	"Common/Common.cpp"
	"Common/StringUtils.h"
	"Common/StringUtils.cpp"
	
	"Engine/Allocator.h"
	"Engine/Application.h" 
	"Engine/Application.cpp"
	"Engine/TimeStep.h" 
	"Engine/TimeStep.cpp"
	"Engine/BaseWin32Window.h"
	"Engine/BaseWin32Window.cpp" 
	"Engine/BoundingVolume.h"
	"Engine/BoundingVolume.cpp"
	"Engine/BVH.h"
	"Engine/BVH.cpp"
	"Engine/ComponentUtils.h"
	"Engine/ComponentUtils.cpp"
	"Engine/Layer.h"
	"Engine/Layer.cpp"
	"Engine/Morton.h"
	"Engine/Morton.cpp"
	"Engine/Win32Window.h"
	"Engine/Win32Window.cpp"
	
	"Event/Event.h"
	"Event/Event.cpp"
	"FontMgr/FontMgr.h" 
	"FontMgr/FontMgr.cpp"
	
	"Input/InputDevice.h" 
	"Input/InputDevice.cpp"
	"Input/Keyboard.h" 
	"Input/Keyboard.cpp" 
	"Input/Mouse.h"
	"Input/Mouse.cpp"
	
	"IO/ObjectLoader.h"
	"IO/ObjectLoader.cpp"

	"Materials/TextureBase.h"
	"Materials/TextureBase.cpp"
	"Materials/Material.h" 
	"Materials/Material.cpp"
	"Materials/Shader.h"
	"Materials/Shader.cpp"
	"Materials/Texture.h" 
	"Materials/Texture.cpp"
	
	"Physics/Collider.h"
	"Physics/Collider.cpp"
	"Physics/CollisionDescription.h" 
	"Physics/Ray.h"
	"Physics/Ray.cpp" 
	"Physics/PhysicsState.h" 
	"Physics/PhysicsState.cpp"
	"Physics/PhysicsSystem.h"
	"Physics/PhysicsSystem.cpp" 
	
	"Primitives/Bound.h" 
	"Primitives/Curve.h"
	"Primitives/Mesh.h" 
	"Primitives/Mesh.cpp"
	"Primitives/Grid.h" 
	"Primitives/Grid.cpp"
	"Primitives/ParticleSystem.h" 
	"Primitives/ParticleSystem.cpp"
	"Primitives/Range.h"
	"Primitives/Square.h"
	"Primitives/Square.cpp"
	"Primitives/Transform.h"
	"Primitives/Vertex.h"
	
	"Rendering/FrameBuffer.h" 
	"Rendering/FrameBuffer.cpp" 
	"Rendering/BufferLayout.h"
	"Rendering/BufferLayout.cpp"
	"Rendering/LightAndShadow/PointLight.h" 
	"Rendering/LightAndShadow/PointLight.cpp"
	"Rendering/Renderer.h" 
	"Rendering/Renderer.cpp"
	"Rendering/VertexArray.h"
	"Rendering/VertexArray.cpp" 
	"Rendering/VertexBuffer.h" 
	"Rendering/VertexBuffer.cpp"
	"Rendering/UniformBuffer.h"
	"Rendering/RenderCommand.h"
	"Rendering/RenderCommand.cpp"
	
	"Resources/Asset.h"
	"Resources/Asset.cpp"
	"Resources/ShaderResource.h"
	"Resources/ShaderResource.cpp"
	"Resources/AssetManager.h"
	"Resources/AssetManager.cpp"
	
	"SceneMgr/SceneState.h"
	"SceneMgr/Scene.h" 
	"SceneMgr/Scene.cpp"
	
	"Scripting/Script.h"
	"Scripting/Script.cpp"
	"Scripting/ScriptMgr.h" 
	"Scripting/ScriptMgr.cpp"
	"Scripting/ScriptSystem.h" 
	"Scripting/ScriptSystem.cpp" 
	
	"Text/TextField.h"
	"Text/TextField.cpp" 
	"Text/Label.h" 
	"Text/Label.cpp"
	
	"UUID/copyrt.h"
	"UUID/global.h"
	"UUID/md5.h"
	"UUID/md5.cpp"
	"UUID/sha1.h"
	"UUID/sha1.cpp"
	"UUID/sysdep.h"
	"UUID/sysdep.cpp"
	"UUID/UUID.h"
	"UUID/UUID.cpp"
	
	"Viewing/Camera.h"
	"Viewing/Camera.cpp"
	
	"Audio/WAV.h"
	"Audio/Sound.h"
)

add_library(${LIB_NAME} STATIC
	${SHARED_SOURCES}
	"Engine/Character.h"
	"Engine/Character.cpp"
)

target_compile_definitions(${LIB_NAME}
	PRIVATE
		-DSTB_IMAGE_IMPLEMENTATION
		-DGLEW_BUILD
)
if(MSVC)
	target_compile_definitions(${LIB_NAME}
	PRIVATE
		WIN32_LEAN_AND_MEAN 
		_CRT_SECURE_NO_WARNINGS 	
	)
endif()
target_include_directories(${LIB_NAME}
	PUBLIC	
		${SPRING_INCLUDE_DIRS}
	PRIVATE
		"${CMAKE_SOURCE_DIR}/Dependencies/EnTT/"
		"${CMAKE_SOURCE_DIR}/Dependencies/FREETYPE/include/"
		"${CMAKE_SOURCE_DIR}/Dependencies/GLEW/include/"
		"${CMAKE_SOURCE_DIR}/Dependencies/GLM/"
		"${CMAKE_SOURCE_DIR}/Dependencies/json-develop/include/"
		"${CMAKE_SOURCE_DIR}/Dependencies/Lua/include/"
		"${CMAKE_SOURCE_DIR}/Dependencies/MSDF/msdfgen/"
		"${CMAKE_SOURCE_DIR}/Dependencies/MSDF/msdfgen/core/"
		"${CMAKE_SOURCE_DIR}/Dependencies/MSDF/msdfgen/ext/"
		"${CMAKE_SOURCE_DIR}/Dependencies/MSDF/msdf-atlas-gen/"
		"${CMAKE_SOURCE_DIR}/Dependencies/stbi_image/"
)
# Link libraries 
target_link_libraries(${LIB_NAME}
	PUBLIC SpringVendor
) 
if(MSVC)
	target_link_libraries(${LIB_NAME}
	PUBLIC
		"Ws2_32.lib"
	)
endif()

# Set C++ standard 
if (CMAKE_VERSION VERSION_GREATER 3.12) 
set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD 20) 
endif()
# Optional: install targets or test targets here