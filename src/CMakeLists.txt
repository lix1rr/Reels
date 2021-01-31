cmake_minimum_required(VERSION 3.9)
project(Reels)
include(FindPythonInterp)

add_library(libreels vm.c "stdfn.c" "resources.c")
add_executable(Reels main.c)
target_link_libraries(Reels PUBLIC libreels)

execute_process(
                  COMMAND ${PYTHON_EXECUTABLE} "compile_resources.py"
                  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                  RESULT_VARIABLE py_result
               )
message(STATUS "Python result: ${py_result}")