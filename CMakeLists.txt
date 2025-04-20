cmake_minimum_required(VERSION 3.31)

project(jsh C)
set(SRC src/main.c src/shell_command.c)
add_executable(${PROJECT_NAME} ${SRC})
