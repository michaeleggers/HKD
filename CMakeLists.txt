cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(HKD VERSION 1.0)

macro(ADD_SRC_HEADER_FILES )
	file(GLOB_RECURSE SRC_FILES LIST_DIRECTORIES false RELATIVE
		${CMAKE_CURRENT_SOURCE_DIR} *.c??)
	file(GLOB_RECURSE HEADER_FILES LIST_DIRECTORIES false RELATIVE
		${CMAKE_CURRENT_SOURCE_DIR} *.h)    
endmacro()

# ADD_SRC_HEADER_FILES()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

if (WIN32)
	# res/app.rc
	SET(RESOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/app.rc)
endif()

if (MSVC)
    add_compile_options(/W4)
endif()

add_subdirectory(dependencies/SDL2.28.4)

# add_executable(Game
# 	${SRC_FILES}
# 	${HEADER_FILES}
# )	

add_executable(Game

	main.cpp
	platform.h
	platform.cpp
	iqm_loader.h
	iqm_loader.cpp
	irender.h
	r_gl.h
	r_gl.cpp
	r_model.h
	r_model.cpp
	r_common.h

	shaders/basic.vert
	shaders/basic.frag

    dependencies/stb_image.h
	dependencies/stb_image_write.h

	dependencies/imgui/imgui.h
	dependencies/imgui/imconfig.h
	dependencies/imgui/imgui_internal.h
	dependencies/imgui/imstb_rectpack.h
	dependencies/imgui/imstb_textedit.h
	dependencies/imgui/imstb_truetype.h

	dependencies/imgui/misc/cpp/imgui_stdlib.h
	dependencies/imgui/misc/cpp/imgui_stdlib.cpp

	dependencies/imgui/imgui.cpp
	dependencies/imgui/imgui_draw.cpp
	dependencies/imgui/imgui_tables.cpp
	dependencies/imgui/imgui_widgets.cpp
	dependencies/imgui/imgui_demo.cpp

	dependencies/imgui/backends/imgui_impl_opengl3.cpp
	dependencies/imgui/backends/imgui_impl_opengl3.h
	dependencies/imgui/backends/imgui_impl_sdl2.cpp
	dependencies/imgui/backends/imgui_impl_sdl2.h

	dependencies/glad/src/glad.c

	${RESOURCE_FILES}
)

target_include_directories(Game
    PUBLIC dependencies
	PUBLIC dependencies/glad/include
	PUBLIC dependencies/imgui
	PUBLIC dependencies/SDL2.28.4/include
)

target_link_libraries(Game PUBLIC
    PUBLIC SDL2
    PUBLIC SDL2main
)

# if (WIN32)
# 	set(ASSETS_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/assets")
# 	set(SHADERS_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/shaders")

# 	set(DEST_ASSETS_DIR_DEBUG "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/assets")
# 	set(DEST_ASSETS_DIR_RELEASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/assets")
# 	set(DEST_ASSETS_DIR_RELDEBUG "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo/assets")
# 	set(DEST_SHADERS_DIR_DEBUG "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/shaders")
# 	set(DEST_SHADERS_DIR_RELEASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/shaders")
# 	set(DEST_SHADERS_DIR_RELDEBUG "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo/shaders")

# 	add_custom_command(TARGET Game POST_BUILD
# 			COMMAND ${CMAKE_COMMAND} -E copy_directory
# 					${ASSETS_DIR}
# 					${DEST_ASSETS_DIR_DEBUG}
# 	)
# 	add_custom_command(TARGET Game POST_BUILD
# 			COMMAND ${CMAKE_COMMAND} -E copy_directory
# 					${ASSETS_DIR}
# 					${DEST_ASSETS_DIR_RELEASE}
# 	)

# 	add_custom_command(TARGET Game POST_BUILD
# 	COMMAND ${CMAKE_COMMAND} -E copy_directory
# 			${SHADERS_DIR}
# 			${DEST_SHADERS_DIR_DEBUG}
# 	)
# 	add_custom_command(TARGET Game POST_BUILD
# 		COMMAND ${CMAKE_COMMAND} -E copy_directory
# 				${SHADERS_DIR}
# 				${DEST_SHADERS_DIR_RELEASE}
# 	)

# 	add_custom_command(TARGET Game POST_BUILD
# 			COMMAND ${CMAKE_COMMAND} -E copy_directory
# 					${ASSETS_DIR}
# 					${DEST_ASSETS_DIR_RELDEBUG}
# 	)
# 	add_custom_command(TARGET Game POST_BUILD
# 	COMMAND ${CMAKE_COMMAND} -E copy_directory
# 			${SHADERS_DIR}
# 			${DEST_SHADERS_DIR_RELDEBUG}
# 	)

# endif()

# Is this needed???
# add_custom_target(copy_directory_target ALL DEPENDS ${DEST_ASSETS_DIR})


# file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
# file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})


