cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 20)

project(indopanelui LANGUAGES CXX)

find_package(SDL2_image REQUIRED SDL2_image>=2.0.0)
#find_package(ImGui REQUIRED)
include(${CMAKE_SOURCE_DIR}/cmake/imgui.cmake)

#include(${CMAKE_SOURCE_DIR}/cmake/imgui.cmake)

#add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

add_executable(indopanelui ${UI_SOURCES})

include_directories(indopanelui ${CMAKE_SOURCE_DIR}/src)

#message("uisrcs: ${UI_SOURCES}")

#target_sources(indopanelui ${UI_SOURCES})

target_link_libraries(indopanelui PRIVATE ${IMGUI_LIBRARIES} SDL2_image::SDL2_image)

install(TARGETS indopanelui RUNTIME DESTINATION bin)
