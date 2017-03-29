#define GL_GLEXT_PROTOTYPES 1

#include "ThickHighlightEdge.hpp"
#include <GLES3/gl31.h>
#include <algorithm>
#include <cstdlib>
#include <cv.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

// TODO: clean up this class (extract OpenGL)
///
// Create a shader object, load the shader source, and
// compile the shader.
//
GLuint LoadShader(GLenum type, const char *shaderSrc) {
  GLuint shader;
  GLint compiled;

  // Create the shader object
  shader = glCreateShader(type);

  if (shader == 0) {
    return 0;
  }

  // Load the shader source
  glShaderSource(shader, 1, &shaderSrc, NULL);

  // Compile the shader
  glCompileShader(shader);

  // Check the compile status
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

  if (!compiled) {
    GLint infoLen = 0;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

    if (infoLen > 1) {
      char *infoLog = (char *)malloc(sizeof(char) * infoLen);

      glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
      cout << "Error compiling shader:\n" << infoLog << endl;

      free(infoLog);
    }

    glDeleteShader(shader);
    return 0;
  }

  return shader;
}

GLuint programObject;
GLint colorLoc;

///
// Initialize the shader and program object
//
int Init() {
  char vShaderStr[] =
      "#version 300 es                          \n"
      "layout(location = 0) in vec2 position;  \n"
      "void main()                              \n"
      "{                                        \n"
      "   gl_Position = vec4(position, 0.0, 1.0);              \n"
      "}                                        \n";

  char fShaderStr[] = "#version 300 es                              \n"
                      "precision mediump float;                     \n"
                      "uniform vec4 u_color;                     \n"
                      "out vec4 fragColor;                          \n"
                      "void main()                                  \n"
                      "{                                            \n"
                      "   fragColor = u_color;  \n"
                      "}                                            \n";

  GLuint vertexShader;
  GLuint fragmentShader;
  GLint linked;

  // Load the vertex/fragment shaders
  vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
  fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderStr);

  // Create the program object
  programObject = glCreateProgram();

  if (programObject == 0) {
    return 0;
  }

  glAttachShader(programObject, vertexShader);
  glAttachShader(programObject, fragmentShader);

  // Link the program
  glLinkProgram(programObject);

  // Check the link status
  glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

  if (!linked) {
    GLint infoLen = 0;

    glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

    if (infoLen > 1) {
      char *infoLog = (char *)malloc(sizeof(char) * infoLen);

      glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
      cout << "Error linking program:\n" << infoLog << endl;

      free(infoLog);
    }

    glDeleteProgram(programObject);
    return 0;
  }

  colorLoc = glGetUniformLocation(programObject, "u_color");

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  return 1;
}

ThickHighlightEdge::ThickHighlightEdge(SDL_Renderer *const renderer_,
                                       SDL_Window *const win_)
    : Effect(renderer_) {
  win = win_;
  output.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);
  Init();
}

struct GlPoint {
  float x;
  float y;
};

GlPoint pixelsToPoint(unsigned int x, unsigned int y) {
  return GlPoint{1.0 - x * 2.0 / 320, 1.0 - y * 2.0 / 240};
}

void ThickHighlightEdge::render(cv::Mat &frame) {
  // Highlight the hand in white, and make the rest of output black.
  findHand(frame, handMask, backMask);

  output = Scalar(0, 0, 0, 255);

  Canny(handMask, edges, 50, 150);
  vector<vector<Point>> contours;
  findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

  // Clear the color buffer
  glClear(GL_COLOR_BUFFER_BIT);

  // Use the program object
  glUseProgram(programObject);
  glLineWidth(5.0);
  glUniform4f(colorLoc, 1.0, 1.0, 1.0, 1.0);

  vector<GlPoint> points;
  for (auto i = contours.begin(); i != contours.end(); i++) {
    for (auto k = (*i).begin(); k != (*i).end(); k++) {
      points.push_back(pixelsToPoint(k->x, k->y));
    }
  }

  // Load the vertex data
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, points.data());
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_POINTS, 0, points.size());

  SDL_GL_SwapWindow(win);
}
