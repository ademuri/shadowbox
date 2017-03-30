#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#define GL_GLEXT_PROTOTYPES 1
#include <GLES3/gl31.h>

// Defines a 2D point that can be handed to OpenGL.
struct GlPoint {
  float x;
  float y;
};

/** Deals with OpenGL. Owns the shaders.
 * TODO: should this be a namespace instead of a class?
 */
class Renderer {
public:
  // Create the shaders. Returns 0 (and prints to the console) if an error
  // occurs.
  static int Init();

  // Create a shader object, load the shader source, and
  // compile the shader.
  static GLuint LoadShader(GLenum type, const char *shaderSrc);

  static void useSolidShader();

  static void setSolidShaderColor(float r, float g, float b, float a);

  static GlPoint pixelsToPoint(unsigned int x, unsigned int y);

private:
  // This is a 'static' class, since it is logically a singleton, and returns a
  // status when initializing (since creating the shaders may fail).
  Renderer() {}
};

#endif
