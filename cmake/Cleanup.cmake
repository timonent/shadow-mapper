cmake_minimum_required(VERSION 3.17)

#----------------------------------------------------------------------------

file(GLOB files "*")
list(FILTER files EXCLUDE REGEX "glew|glfw|glm")
foreach(f ${files})
    message("Removing ${f}")
    file(REMOVE_RECURSE "${f}")
endforeach()

#----------------------------------------------------------------------------