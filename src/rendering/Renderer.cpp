#include "Renderer.hpp"
#include <iostream>

using namespace std;

// Identifier for the solid color shader.
static GLuint solidShaderProgram;

// Index for the color property on the shader.
static GLint solidShaderColorLoc;

GLuint Renderer::LoadShader(GLenum type, const char *shaderSrc) {
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

int Renderer::Init() {
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
  vertexShader = Renderer::LoadShader(GL_VERTEX_SHADER, vShaderStr);
  fragmentShader = Renderer::LoadShader(GL_FRAGMENT_SHADER, fShaderStr);

  // Create the program object
  solidShaderProgram = glCreateProgram();

  if (solidShaderProgram == 0) {
    return 0;
  }

  glAttachShader(solidShaderProgram, vertexShader);
  glAttachShader(solidShaderProgram, fragmentShader);

  // Link the program
  glLinkProgram(solidShaderProgram);

  // Check the link status
  glGetProgramiv(solidShaderProgram, GL_LINK_STATUS, &linked);

  if (!linked) {
    GLint infoLen = 0;

    glGetProgramiv(solidShaderProgram, GL_INFO_LOG_LENGTH, &infoLen);

    if (infoLen > 1) {
      char *infoLog = (char *)malloc(sizeof(char) * infoLen);

      glGetProgramInfoLog(solidShaderProgram, infoLen, NULL, infoLog);
      cout << "Error linking program:\n" << infoLog << endl;

      free(infoLog);
    }

    glDeleteProgram(solidShaderProgram);
    return 0;
  }

  solidShaderColorLoc = glGetUniformLocation(solidShaderProgram, "u_color");

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  return 1;
}

void Renderer::useSolidShader() { glUseProgram(solidShaderProgram); }

void Renderer::setSolidShaderColor(float r, float g, float b, float a) {
  glUniform4f(solidShaderColorLoc, r, g, b, a);
}

GlPoint Renderer::pixelsToPoint(unsigned int x, unsigned int y) {
  return GlPoint{1.0 - x * 2.0 / 320, 1.0 - y * 2.0 / 240};
}
