# Editor/CMakeLists.txt
cmake_minimum_required(VERSION 3.21) 
# Project settings
set(LIB_NAME "Editor")
project(${LIB_NAME})

# Include directories 
set(SPRING_INCLUDE_DIRS
	"${CMAKE_SOURCE_DIR}/Core/Audio"
	"${CMAKE_SOURCE_DIR}/Core/Common"
	"${CMAKE_SOURCE_DIR}/Core/Engine"
	"${CMAKE_SOURCE_DIR}/Core/Event"
	"${CMAKE_SOURCE_DIR}/Core/FontMgr"
	"${CMAKE_SOURCE_DIR}/Core/Input"
	"${CMAKE_SOURCE_DIR}/Core/IO"
	"${CMAKE_SOURCE_DIR}/Core/Materials"
	"${CMAKE_SOURCE_DIR}/Core/Primitives"
	"${CMAKE_SOURCE_DIR}/Core/Physics"
	"${CMAKE_SOURCE_DIR}/Core/Rendering"
	"${CMAKE_SOURCE_DIR}/Core/Resources"
	"${CMAKE_SOURCE_DIR}/Core/SceneMgr"
	"${CMAKE_SOURCE_DIR}/Core/Scripting"
	"${CMAKE_SOURCE_DIR}/Core/Text"
	"${CMAKE_SOURCE_DIR}/Core/UUID"
	"${CMAKE_SOURCE_DIR}/Core/Viewing"
)
# Executable-specific files 
set(EDITOR_SOURCES
	"BaseApplication.h"
	"BaseApplication.cpp"
	"UILayer.h"
	"UILayer.cpp"
	"Panel.h"
	"Panel.cpp"
	"Project/Project.h" 
	"Project/Project.cpp"
	"ParticleSystemPanel.h"
	"MenuBarPanel.h" 
	"ContentBrowserPanel.h" 
	"ComponentsPanel.h"
	"StatisticsPanel.h" 
	"RenderPanel.h"
) 

add_library(${LIB_NAME} 
	${EDITOR_SOURCES}
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
		"${SPRING_INCLUDE_DIRS}"
	PRIVATE
		"${PROJECT_SOURCE_DIR}/Editor"
		"${PROJECT_SOURCE_DIR}/Editor/Utility"
		"${CMAKE_SOURCE_DIR}/Dependencies/EnTT/"
		"${CMAKE_SOURCE_DIR}/Dependencies/imgui-docking/" 
		"${CMAKE_SOURCE_DIR}/Dependencies/imgui-docking/backends/"
		"${CMAKE_SOURCE_DIR}/Dependencies/GLEW/include/"
		"${CMAKE_SOURCE_DIR}/Dependencies/GLM/"
		"${CMAKE_SOURCE_DIR}/Dependencies/json-develop/include/"
		"${CMAKE_SOURCE_DIR}/Dependencies/Lua/include/"
		"${CMAKE_SOURCE_DIR}/Dependencies/MSDF/msdfgen/"
		"${CMAKE_SOURCE_DIR}/Dependencies/MSDF/msdfgen/core/"
		"${CMAKE_SOURCE_DIR}/Dependencies/MSDF/msdfgen/ext/"
		"${CMAKE_SOURCE_DIR}/Dependencies/MSDF/msdf-atlas-gen/"
)

target_link_libraries(${LIB_NAME}
	PUBLIC Spring
)
# Set C++ standard 
if (CMAKE_VERSION VERSION_GREATER 3.12) 
set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD 20) 
endif()
# Optional: install targets or test targets here