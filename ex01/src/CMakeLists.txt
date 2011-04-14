# create the executable 'ex01' from the source files
#  - 'Ex01.cpp'
#  - 'ObjLoader.cpp'

# link the executable 'ex01' with all necessary libraries
#  - openGLTools (in ex01/libs/)
#  - OpenGL libraries -> use ${OpenGL_LIBRARIES}
#  - GLUT libraries -> use ${GLUT_LIBRARIES}
#  - GLEW libraries -> use ${GLEW_LIBRARIES}
add_executable(ex01 Ex01 ObjLoader)

target_link_libraries(ex01
        ${OPENGL_LIBRARIES}
        ${GLUT_LIBRARIES}
        ${GLEW_LIBRARIES}
    )
