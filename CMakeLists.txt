cmake_minimum_required (VERSION 3.8)

project(ham-conceptual)

add_library(${PROJECT_NAME} INTERFACE)
add_library(ham::conceptual ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE include/)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

if (MSVC)
	target_compile_options(${PROJECT_NAME} INTERFACE /Zc:preprocessor)
endif()

# Needed for intellisense to function properly during development
# feel free to remove this line when cloning this project.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.dev/dummy.cpp")
	add_executable(vs_dummy_executable ".dev/dummy.cpp")
	target_link_libraries(vs_dummy_executable PUBLIC ${PROJECT_NAME})
endif()