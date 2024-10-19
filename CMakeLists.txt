cmake_minimum_required(VERSION 3.0)

project(indopanelui LANGUAGES C)

add_executable(indopanelui main.c)

install(TARGETS indopanelui RUNTIME DESTINATION bin)
