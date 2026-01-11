cmake_minimum_required(VERSION 3.28)



project(magnifying_glass VERSION 1.0
		DESCRIPTION "magnifying-glass"
		LANGUAGES CXX
)


if(NOT MSVC)
	set_target_properties(magnifying_glass PROPERTIES CXX_STANDARD 26)
elseif(MSVC)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
	set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} /DWIN32")
endif()


set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES 
	${CMAKE_SOURCE_DIR}/src/main.cpp
	${CMAKE_SOURCE_DIR}/src/Zoomer.cpp
	${CMAKE_SOURCE_DIR}/src/Global.cpp
	${CMAKE_SOURCE_DIR}/src/MenuWindow.cpp
	${CMAKE_SOURCE_DIR}/src/RenderWindow.cpp
	${CMAKE_SOURCE_DIR}/src/resource.h
	${CMAKE_SOURCE_DIR}/src/ZoomContext.cpp
)


set(DEPENDENCIES
)

add_executable(
		${PROJECT_NAME} WIN32
		${SOURCES}
		${DEPENDENCIES}
)
# Add resource.rc to application
set(RESOURCE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources.rc)
target_sources(${PROJECT_NAME} PRIVATE ${RESOURCE_FILE})

message(STATUS "Project name: ${PROJECT_NAME}")

target_include_directories(
		${PROJECT_NAME}
		PUBLIC
		${CMAKE_SOURCE_DIR}/include
)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Выберите тип сборки (Debug или Release)" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")


# Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
	kernel32.lib
	user32.lib
	gdi32.lib
	winspool.lib
	comdlg32.lib
	advapi32.lib
	shell32.lib
	ole32.lib
	oleaut32.lib
	uuid.lib
	odbc32.lib
	odbccp32.lib
	Winmm.lib
)


# Hot load for MSVC
if(MSVC)
	# Activate Edit and Continue policy
	cmake_policy(SET CMP0141 NEW)

	target_compile_options(${PROJECT_NAME} PRIVATE /std:c++latest)

	set_property(TARGET ${PROJECT_NAME} PROPERTY
		MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug>:EditAndContinue>"
	)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		LINK_FLAGS_DEBUG "/INCREMENTAL"
	)
endif()
# Drop down optimization flags for programming literally
if(MSVC)
	# /Od for MSVC
	target_compile_options(${PROJECT_NAME} PRIVATE
		"$<$<CONFIG:Debug>:/Od>"
	)
else()
	# -O0 for GCC/Clang
	target_compile_options(${PROJECT_NAME} PRIVATE -O0)
endif()

# for #define _DEBUG
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(_DEBUG)
endif()