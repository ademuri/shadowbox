#include "Renderer.hpp"
#include <iostream>

using namespace std;

// Identifier for the solid color shader.
static GLuint solidShaderProgram;

// Index for the color property on the shader.
static GLint solidShaderColorLoc;

static GLuint textureShaderProgram;

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
      "layout(location = 0) in vec2 position;   \n"
      "in vec2 a_texCoord;               \n"
      "out vec2 v_texCoord;                 \n"
      "void main()                              \n"
      "{                                        \n"
      "   gl_Position = vec4(position, 0.0, 1.0);              \n"
      "   v_texCoord = a_texCoord;              \n"
      "}                                        \n";

  char fSolidShaderStr[] = "#version 300 es                              \n"
                           "precision mediump float;                     \n"
                           "uniform vec4 u_color;                     \n"
                           "out vec4 fragColor;                          \n"
                           "void main()                                  \n"
                           "{                                            \n"
                           "   fragColor = u_color;  \n"
                           "}                                            \n";

  char fTextureShaderStr[] =
      "#version 300 es                              \n"
      "precision mediump float;                     \n"
      "uniform sampler2D u_texture;                     \n"
      "in vec2 v_texCoord;                     \n"
      "out vec4 fragColor;                          \n"
      "void main()                                  \n"
      "{                                            \n"
      "   vec4 abgr = texture(u_texture, v_texCoord);          \n"
      "   fragColor.r = abgr.b;          \n"
      "   fragColor.b = abgr.r;          \n"
      "   fragColor.g = abgr.g;          \n"
      "   fragColor.a = 1.0;          \n"
      "}                                            \n";

  solidShaderProgram = createShaderProgram(vShaderStr, fSolidShaderStr);
  textureShaderProgram = createShaderProgram(vShaderStr, fTextureShaderStr);

  solidShaderColorLoc = glGetUniformLocation(solidShaderProgram, "u_color");

  return solidShaderProgram && textureShaderProgram;
}

GLuint Renderer::createShaderProgram(const char *vShaderStr,
                                     const char *fShaderStr) {
  GLint linked;

  // Load the vertex/fragment shaders
  const GLuint vertexShader =
      Renderer::LoadShader(GL_VERTEX_SHADER, vShaderStr);
  const GLuint fragmentShader =
      Renderer::LoadShader(GL_FRAGMENT_SHADER, fShaderStr);

  // Create the program object
  const GLuint shaderProgram = glCreateProgram();

  if (shaderProgram == 0) {
    return 0;
  }

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  // Link the program
  glLinkProgram(shaderProgram);

  // Check the link status
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);

  if (!linked) {
    GLint infoLen = 0;

    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLen);

    if (infoLen > 1) {
      char *infoLog = (char *)malloc(sizeof(char) * infoLen);

      glGetProgramInfoLog(shaderProgram, infoLen, NULL, infoLog);
      cout << "Error linking program:\n" << infoLog << endl;

      free(infoLog);
    }

    glDeleteProgram(shaderProgram);
    return 0;
  }

  return shaderProgram;
}

void Renderer::useSolidShader() { glUseProgram(solidShaderProgram); }
void Renderer::useTextureShader() { glUseProgram(textureShaderProgram); }

void Renderer::setSolidShaderColor(float r, float g, float b, float a) {
  glUniform4f(solidShaderColorLoc, r, g, b, a);
}

GlPoint Renderer::pixelsToPoint(unsigned int x, unsigned int y) {
  return GlPoint{1.0f - x * 2.0 / 320, 1.0f - y * 2.0 / 240};
}
