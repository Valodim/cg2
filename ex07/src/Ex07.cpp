#include <GL/glew.h>
#include <GL/glut.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "MeshObj.h"
#include "ObjLoader.h"
#include "Trackball.h"

void initGL();
void initShader();
void initUniforms();
void enableShader();
void disableShader();

void initTextures();

void resizeGL(int w, int h);
void updateGL();
void idle();
void keyboardEvent(unsigned char key, int x, int y);
void mouseEvent(int button, int state, int x, int y);
void mouseMoveEvent(int x, int y);

void invertRotTransMat(float *mat, float *inv);
void invertPerspectiveMat(float *mat, float *inv);

void renderScene();

GLint windowWidth, windowHeight;
GLfloat zNear, zFar;
GLfloat fov;

Trackball trackball(0, 0.6, 1);
ObjLoader objLoader;

bool shadersInitialized = false;
GLuint shaderProgram;

struct Texture {
  unsigned char *data;
  unsigned int width, height;
  GLuint glTextureLocation;
  GLint uniformLocation;
};
enum TextureLayer {DIFFUSE = 0, NORMAL_MAP, LAYER_COUNT};
Texture texture[LAYER_COUNT];

GLint att_position, att_normal, att_texcoord, att_tangent, att_bitangent;

void loadTextureData(const char *fileName, Texture &texture);

int main (int argc, char **argv) {
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  
  windowWidth = 512;
  windowHeight = 512;
  
  zNear = 0.1f;
  zFar = 10000.0f;
  fov = 45.0f;

  glutInitWindowSize (windowWidth, windowHeight);
  glutInitWindowPosition (100, 100);
  glutCreateWindow("Exercise 07");
  
  glutReshapeFunc(resizeGL);
  glutDisplayFunc(updateGL);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboardEvent);
  glutMouseFunc(mouseEvent);
  glutMotionFunc(mouseMoveEvent);
  
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
  }
  fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
  
  // load obj file here //
  objLoader.loadObjFile("./meshes/sphere.obj", "planet");
  
  initGL();
  initShader();
  
  initTextures();
  
  glutMainLoop();
  
  return 0;
}

void initGL() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  
  // set projectionmatrix
  glMatrixMode(GL_PROJECTION);
  gluPerspective(fov, 1.0, zNear, zFar);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

char* loadShaderSource(const char* fileName) {
  std::ifstream file(fileName, std::ios::in);
  if (file.good()) {
    unsigned long srcLength = 0;
    file.tellg();
    file.seekg(0, std::ios::end);
    srcLength = file.tellg();
    file.seekg(0, std::ios::beg);
    
    char *srcData = new char[srcLength + 1];
    srcData[srcLength] = 0;
    
    unsigned long i = 0;
    while (file.good()) {
      srcData[i] = file.get();
      if (!file.eof()) {
        ++i;
      }
    }
    file.close();
    srcData[i] = 0;
    
    // return shader source //
    return srcData;
  } else {
    std::cout << "(loadShaderSource) - Could not open file \"" << fileName << "\"." << std::endl;
    return NULL;
  }
}

GLuint loadShaderFile(const char* fileName, GLenum shaderType) {
  GLuint shader = glCreateShader(shaderType);
  if (shader == 0) {
    return 0;
  }
  
  const char* shaderSrc = loadShaderSource(fileName);
  if (shaderSrc == NULL) return 0;
  glShaderSource(shader, 1, (const char**)&shaderSrc, NULL);
  
  glCompileShader(shader);
  
  int logMaxLength;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logMaxLength);
  char log[logMaxLength];
  int logLength = 0;
  glGetShaderInfoLog(shader, logMaxLength, &logLength, log);
  if (logLength > 0) {
    std::cout << "(loadShaderFile) - Compiler log:\n------------------\n" << log << "\n------------------" << std::endl;
  }
  
  return shader;
}

void initShader() {
  shadersInitialized = false;
  
  shaderProgram = glCreateProgram();
  
  GLuint vertexShader = loadShaderFile("shader/normalMapShader.vert", GL_VERTEX_SHADER);
  if (vertexShader == 0) {
    std::cout << "(initShader) - Could not create vertex shader." << std::endl;
    glDeleteProgram(shaderProgram);
    return;
  }
  GLuint fragmentShader = loadShaderFile("shader/normalMapShader.frag", GL_FRAGMENT_SHADER);
  if (fragmentShader == 0) {
    std::cout << "(initShader) - Could not create vertex shader." << std::endl;
    glDeleteProgram(shaderProgram);
    return;
  }
  
  // loaded shaders seem to work -> attach them to program //
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  
  // link final program //
  glLinkProgram(shaderProgram);
  
  // get log //
  int logMaxLength;
  glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logMaxLength);
  char log[logMaxLength];
  int logLength = 0;
  glGetShaderInfoLog(shaderProgram, logMaxLength, &logLength, log);
  if (logLength > 0) {
    std::cout << "(initShader) - Linker log:\n------------------\n" << log << "\n------------------" << std::endl;
  }
  
  initUniforms();
}

void initUniforms(void) {
  enableShader();
  
  att_position = glGetAttribLocation(shaderProgram, "att_position");
  att_normal = glGetAttribLocation(shaderProgram, "att_normal");
  att_texcoord = glGetAttribLocation(shaderProgram, "att_texcoord");
  att_tangent = glGetAttribLocation(shaderProgram, "att_tangent");
  att_bitangent = glGetAttribLocation(shaderProgram, "att_bitangent");

  // XXX: assign the used texture uniforms here //
  texture[DIFFUSE].uniformLocation = glGetUniformLocation(shaderProgram, "tex_diffuse");
  texture[NORMAL_MAP].uniformLocation = glGetUniformLocation(shaderProgram, "tex_normal");
  
  disableShader();
}

void initTextures (void) {
  // XXX: init your textures here //
  loadTextureData("textures/mars.png", texture[DIFFUSE]);
  glGenTextures(1, &texture[DIFFUSE].glTextureLocation);

  glBindTexture(GL_TEXTURE_2D, texture[DIFFUSE].glTextureLocation);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture[DIFFUSE].width, texture[DIFFUSE].height, 0, GL_BGR, GL_UNSIGNED_BYTE, texture[DIFFUSE].data);
  glGenerateMipmap(GL_TEXTURE_2D);

  loadTextureData("textures/mars_normal.png", texture[NORMAL_MAP]);
  glGenTextures(1, &texture[NORMAL_MAP].glTextureLocation);

  glBindTexture(GL_TEXTURE_2D, texture[NORMAL_MAP].glTextureLocation);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture[NORMAL_MAP].width, texture[NORMAL_MAP].height, 0, GL_BGR, GL_UNSIGNED_BYTE, texture[NORMAL_MAP].data);
  glGenerateMipmap(GL_TEXTURE_2D);
}

void loadTextureData(const char *textureFile, Texture &texture) {
  IplImage *image = cvLoadImage(textureFile, CV_LOAD_IMAGE_COLOR);
  if (image != NULL) {
    texture.width = image->width;
    texture.height = image->height;
    texture.data = new unsigned char[image->imageSize];
    if (image->origin == 0) {
      // flip rows of the image from top to bottom //
      cvFlip(image);
    }
    memcpy(texture.data, image->imageData, image->imageSize);
    std::cout << "(loadTextureData) - imported \"" << textureFile << "\" (" << texture.width << ", " << texture.width << ")" << std::endl;
  }
  cvReleaseImage(&image);
}

void enableShader() {
  glUseProgram(shaderProgram);
}

void disableShader() {
  glUseProgram(0);
}

void updateGL() {
  GLfloat aspectRatio = (GLfloat)windowWidth / windowHeight;
  
  // clear renderbuffer //
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glViewport(0, 0, windowWidth, windowHeight);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fov, aspectRatio, zNear, zFar);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  trackball.rotateView();
    
  // render planet //
  enableShader();
  
  // XXX: enable and upload textures //
  glEnable(GL_TEXTURE_2D);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture[DIFFUSE].glTextureLocation);
  glUniform1i(texture[DIFFUSE].uniformLocation, 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture[NORMAL_MAP].glTextureLocation);
  glUniform1i(texture[NORMAL_MAP].uniformLocation, 1);
  
  // render object //
  objLoader.getMeshObj("planet")->render(att_position, att_normal, att_texcoord, att_tangent, att_bitangent);
  
  disableShader();
  
  // swap render and screen buffer //
  glutSwapBuffers();
}

void idle() {
  glutPostRedisplay();
}

void resizeGL(int w, int h) {
  windowWidth = w;
  windowHeight = h;
  glutPostRedisplay();
}

void keyboardEvent(unsigned char key, int x, int y) {
  switch (key) {
    case 'x':
    case 27 : {
      exit(0);
      break;
    }
    case 'w': {
      // move forward //
      trackball.updateOffset(Trackball::MOVE_FORWARD);
      break;
    }
    case 's': {
      // move backward //
      trackball.updateOffset(Trackball::MOVE_BACKWARD);
      break;
    }
    case 'a': {
      // move left //
      trackball.updateOffset(Trackball::MOVE_LEFT);
      break;
    }
    case 'd': {
      // move right //
      trackball.updateOffset(Trackball::MOVE_RIGHT);
      break;
    }
    default : {
      break;
    }
  }
  glutPostRedisplay();
}

void mouseEvent(int button, int state, int x, int y) {
 Trackball::MouseState mouseState;
  if (state == GLUT_DOWN) {
    switch (button) {
      case GLUT_LEFT_BUTTON : {
        mouseState = Trackball::LEFT_BTN;
        break;
      }
      case GLUT_RIGHT_BUTTON : {
        mouseState = Trackball::RIGHT_BTN;
        break;
      }
      default : break;
    }
  } else {
    mouseState = Trackball::NO_BTN;
  }
  trackball.updateMouseBtn(mouseState, x, y);
}

void mouseMoveEvent(int x, int y) {
  trackball.updateMousePos(x, y);
}
