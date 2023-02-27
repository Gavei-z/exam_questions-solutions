CMAKE_MINIMUM_REQUIRED(VERSION 3.23)

project(solution)

set(CMAKE_C_STANDARD 11)

add_executable(main main.c parse.c ./third_party/cJSON/cJSON.c)