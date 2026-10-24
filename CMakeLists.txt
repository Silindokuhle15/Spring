# Spring/CMakeLists.txt
cmake_minimum_required(VERSION 3.21) 
# Enable hot reload for MSVC 
if(POLICY CMP0141) 
	cmake_policy(SET CMP0141 NEW) 
	set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT 
	"$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>"
) 
endif() 
# Project settings
set(EXEC_NAME "SpringEditor")
project(${EXEC_NAME})

set(SPRING_INCLUDE_DIRS
	"${PROJECT_SOURCE_DIR}/Core/Engine"
)

add_subdirectory("Dependencies/")
add_subdirectory("Assets/Scripts")
add_subdirectory("Core/")
add_subdirectory("Editor/")

add_executable(${EXEC_NAME} 
	"SpringEditor.cpp"
)

if(MSVC)
	target_compile_definitions(${EXEC_NAME}
	PRIVATE
		WIN32_LEAN_AND_MEAN 
		-D_CRT_SECURE_NO_WARNINGS 
	)
endif()

target_include_directories(${EXEC_NAME}
	PUBLIC
		"${SPRING_INCLUDE_DIRS}"
	PRIVATE
		"${PROJECT_SOURCE_DIR}/Editor"
)

target_link_libraries(${EXEC_NAME} 
	PRIVATE SpringVendor
	PRIVATE Spring
	PRIVATE Editor
)
# Set C++ standard 
if (CMAKE_VERSION VERSION_GREATER 3.12) 
set_property(TARGET ${EXEC_NAME} PROPERTY CXX_STANDARD 20) 
endif()
# Optional: install targets or test targets here