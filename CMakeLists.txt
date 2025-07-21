cmake_minimum_required(VERSION 3.8)

# Enable hot reload for MSVC
if(POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT 
        "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>"
    )
endif()

# Project settings
set(EXEC_NAME "SpringEditor")
set(LIB_NAME "Spring")
project(${LIB_NAME})

# Compiler definitions
add_compile_definitions(
    -D__LEAN_AND_MEAN__
    -D_CRT_SECURE_NO_WARNINGS
    -DFBXSDK_SHARED
    -DSTB_IMAGE_IMPLEMENTATION
    #-DGLFW_INCLUDE_NONE
    -DGLEW_BUILD
)

# GLFW build options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add GLFW
add_subdirectory("Dependencies/GLFW/")

# Include directories
include_directories(
    "E:/FBX/include/"
    "E:/PhysX/physx/include/"
    "Dependencies/stbi_image/"
    "Dependencies/EnTT/"
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
    "Core/Application/"
    "Core/Common/"
    "Core/Engine/"
    "Core/Event/"
    "Core/FontMgr/"
    "Core/Input/"
    "Core/Materials/"
    "Core/Physics/"
    "Core/Project/"
    "Core/Rendering/"
    "Core/SceneMgr/"
    "Core/Scripting/"
    "Core/Viewing/"
    "Editor/"
    "Editor/Utility/"
)

# MSDF dependencies
file(GLOB MSDF_CR_INC "Dependencies/MSDF/msdfgen/core/*.h")
file(GLOB MSDF_CR_SRC "Dependencies/MSDF/msdfgen/core/*.cpp")
file(GLOB MSDF_XT_INC "Dependencies/MSDF/msdfgen/ext/*.h")
file(GLOB MSDF_XT_SRC "Dependencies/MSDF/msdfgen/ext/*.cpp")
file(GLOB MSDG_ATGN_INC "Dependencies/MSDF/msdf-atlas-gen/*.h")
file(GLOB MSDG_ATGN_SRC "Dependencies/MSDF/msdf-atlas-gen/*.cpp")

# Shared source files
set(SHARED_SOURCES
    "Dependencies/GLEW/include/GL/glew.h"
    "Dependencies/GLEW/src/glew.c"

    # ImGui
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

    ${MSDF_CR_INC} ${MSDF_CR_SRC}
    ${MSDF_XT_INC} ${MSDF_XT_SRC}
    ${MSDG_ATGN_INC} ${MSDG_ATGN_SRC}

    # Your engine and editor source files...
    "Core/Common/Common.h" "Core/Common/Common.cpp"
    "Core/Engine/TimeStep.h" "Core/Engine/TimeStep.cpp"
    "Core/Engine/BaseWin32Window.h" "Core/Engine/BaseWin32Window.cpp"
    "Core/Engine/Win32Window.h" "Core/Engine/Win32Window.cpp"
    #"Core/Engine/NGLFWwindow.h" "Core/Engine/NGLFWwindow.cpp"
    "Core/Event/Event.h" "Core/Event/Event.cpp"
    "Core/FontMgr/FontMgr.h" "Core/FontMgr/FontMgr.cpp"
    "Core/Text/TextField.h" "Core/Text/TextField.cpp"
    "Core/Text/Label.h" "Core/Text/Label.cpp"
    "Core/Input/InputDevice.h" "Core/Input/InputDevice.cpp"
    "Core/Input/Keyboard.h" "Core/Input/Keyboard.cpp"
    "Core/Input/Mouse.h" "Core/Input/Mouse.cpp"
    "Core/Rendering/LightAndShadow/PointLight.h" "Core/Rendering/LightAndShadow/PointLight.cpp"
    "Core/Materials/TextureBase.h" "Core/Materials/TextureBase.cpp"
    "Core/Materials/Material.h" "Core/Materials/Material.cpp"
    "Core/Materials/Shader.h" "Core/Materials/Shader.cpp"
    "Core/Materials/Texture.h" "Core/Materials/Texture.cpp"
    "Core/Physics/Collider.h" "Core/Physics/Collider.cpp"
    "Core/Physics/PhysicsEngine.h" "Core/Physics/PhysicsEngine.cpp"
    "Core/Physics/Ray.h" "Core/Physics/Ray.cpp"
    "Core/Physics/PhysicsState.h" "Core/Physics/PhysicsState.cpp"
    "Core/Project/Project.h" "Core/Project/Project.cpp"
    "Core/Rendering/Renderer.h" "Core/Rendering/Renderer.cpp"
    "Core/Rendering/VertexArray.h" "Core/Rendering/VertexArray.cpp"
    "Core/SceneMgr/Scene.h" "Core/SceneMgr/Scene.cpp"
    "Core/SceneMgr/SceneEvent.h" "Core/SceneMgr/SceneEvent.cpp"
    "Core/Scripting/ScriptBase.h" "Core/Scripting/ScriptBase.cpp"
    "Core/Scripting/ScriptMgr.h" "Core/Scripting/ScriptMgr.cpp"
    "Core/Scripting/Script.h" "Core/Scripting/Script.cpp"
    "Editor/Utility/ObjectLoader.h" "Editor/Utility/ObjectLoader.cpp"
    "Editor/Utility/Serializer.h" "Editor/Utility/Serializer.cpp"
    "Core/Viewing/Camera.h" "Core/Viewing/Camera.cpp"
    "Core/Application/Grid.h" "Core/Application/Grid.cpp"
    "Core/Engine/Mesh.h" "Core/Engine/Mesh.cpp"
    "Core/Application/Square.h" "Core/Application/Square.cpp"
)

# Executable-specific files
set(EDITOR_SOURCES
    "SpringEditor.cpp"
    "Core/Engine/BaseApplication.h"
    "Core/Engine/BaseApplication.cpp"
    "Editor/Layer.h" "Editor/Layer.cpp"
    "Editor/UILayer.h" "Editor/UILayer.cpp"
    "Editor/Panel.h"    "Editor/Panel.cpp"
)

# Library-specific sources
set(LIB_SOURCES
    ${SHARED_SOURCES}
    "Core/Engine/Character.h"
    "Core/Engine/Character.cpp"
    "Editor/Layer.h"
    "Editor/Layer.cpp"
)

# Executable target
add_executable(${EXEC_NAME}
    ${SHARED_SOURCES}
    ${EDITOR_SOURCES}
)

# Static library target
add_library(${LIB_NAME} STATIC
    ${LIB_SOURCES}
)

# External libraries
set(LUA_LIB_DIR "${PROJECT_SOURCE_DIR}/Dependencies/Lua/lua54.lib")
set(FBX_LIB_DIR "E:/FBX/lib/x64/debug/libfbxsdk.lib")
set(FT_LIB_DIR "${PROJECT_SOURCE_DIR}/Dependencies/FREETYPE/release static/vs2015-2022/win64/freetype.lib")
set(PHYSX_LIB_DIR "E:/PhysX/physx/bin/win.x86_64.vc143.mt/debug/PhysXDevice64.dll")

# Find OpenGL
find_package(OpenGL REQUIRED)

# Link libraries
target_link_libraries(${LIB_NAME}
    ${OPENGL_gl_LIBRARY}
    ${LUA_LIB_DIR}
    glfw
    ${FBX_LIB_DIR}
    ${FT_LIB_DIR}
)

target_link_libraries(${EXEC_NAME}
    ${OPENGL_gl_LIBRARY}
    ${LUA_LIB_DIR}
    glfw
    ${FBX_LIB_DIR}
    ${FT_LIB_DIR}
    ${LIB_NAME}  # Link static lib to executable
)

# Set C++ standard
if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD 20)
    set_property(TARGET ${EXEC_NAME} PROPERTY CXX_STANDARD 20)
endif()

# Optional: install targets or test targets here
