SET(Exercise02_SRC
  Ex02.cpp
  MeshObj.cpp
  ObjLoader.cpp
  Trackball.cpp
)
ADD_EXECUTABLE(ex02 ${Exercise02_SRC})
TARGET_LINK_LIBRARIES(ex02 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES})
