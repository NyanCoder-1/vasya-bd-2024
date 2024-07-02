cmake_minimum_required (VERSION 3.8)

set(PROJECT "vasya-bd-2024")
set(TARGET "game")
project(${PROJECT})

set(PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
cmake_path(NORMAL_PATH PROJECT_DIR)
set(SOURCE_DIR "${PROJECT_DIR}/src")
set(INCLUDE_DIR "${PROJECT_DIR}/include")
set(THIRDPARTY_DIR "${PROJECT_DIR}/thirdparty")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_DIR}/bin)

file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.c")
if (INCLUDE_THIRDPARTY)
	file(GLOB_RECURSE HEADERS "${SOURCE_DIR}/*.hpp" "${SOURCE_DIR}/*.h" "${INCLUDE_DIR}/*.hpp" "${INCLUDE_DIR}/*.h" "${THIRDPARTY_DIR}/*.hpp" "${THIRDPARTY_DIR}/*.h")
else ()
	file(GLOB_RECURSE HEADERS "${SOURCE_DIR}/*.hpp" "${SOURCE_DIR}/*.h" "${INCLUDE_DIR}/*.hpp" "${INCLUDE_DIR}/*.h")
endif ()

# Set their directories
foreach (SOURCE_FILE ${SOURCES};${HEADERS})

	# Get relative path
	cmake_path(RELATIVE_PATH SOURCE_FILE BASE_DIRECTORY ${PROJECT_DIR})

	# Extract directory
	get_filename_component(SOURCE_FILE_DIR ${SOURCE_FILE} DIRECTORY)
	
	# Set source group
	string(REPLACE "/" "\\" SOURCE_FILE_DIR ${SOURCE_FILE_DIR})
	source_group("${SOURCE_FILE_DIR}" FILES ${SOURCE_FILE})

endforeach ()

if (INCLUDE_THIRDPARTY)
	set(INCLUDE_DIR "${SOURCE_DIR}/" "${INCLUDE_DIR}/" "${THIRDPARTY_DIR}/")
else ()
	set(INCLUDE_DIR "${SOURCE_DIR}/" "${INCLUDE_DIR}/")
endif ()

add_executable(${TARGET} ${SOURCES} ${HEADERS})

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")

	set(FLAGS
		-sUSE_SDL=2
		-sUSE_SDL_TTF=2
		-sUSE_SDL_IMAGE=2
		-sSDL2_IMAGE_FORMATS='["png"]'
		-sUSE_ZLIB=1
		-sUSE_LIBPNG=1
		-sNO_DISABLE_EXCEPTION_CATCHING=1
		--preload-file ../assets@assets)
	string(REPLACE ";" " " FLAGS "${FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${FLAGS}")
	#set(FLAGS
	#	-sWASM=1
	#	-sUSE_SDL=2
	#	-sFULL_ES3=1
	#	-sALLOW_MEMORY_GROWTH=1
	#	-sUSE_ZLIB=1
	#	-sUSE_LIBPNG=1
	#	-sNO_DISABLE_EXCEPTION_CATCHING=1
	#	-sRETAIN_COMPILER_SETTINGS=1
	#	-lfreetype
	#	-L${SUBMODULES_DIR}/msdfgen/embuild
	#	-lmsdfgen-core
	#	-lmsdfgen-ext
	#	--preload-file assets.tar.gz)
	#string(REPLACE ";" " " FLAGS "${FLAGS}")
	#set_target_properties(${TARGET} PROPERTIES LINK_FLAGS ${FLAGS})
	#set(CMAKE_EXECUTABLE_SUFFIX ".js")

else ()

	find_package(SDL2 REQUIRED)
	find_package(SDL2_ttf REQUIRED)
	find_package(SDL2_image REQUIRED)
	target_link_libraries(${TARGET} SDL2 SDL2_ttf SDL2_image)

endif ()

include_directories(${INCLUDE_DIR})
#set_target_properties(${TARGET} PROPERTIES CXX_STANDARD 20 DEBUG_POSTFIX d)
add_definitions(-DTARGET="${TARGET}")
