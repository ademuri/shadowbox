#define GL_GLEXT_PROTOTYPES 1

#include "BasicHighlight.hpp"
#include "BasicTracer.hpp"
#include "Effect.hpp"
#include "FlickerShadow.hpp"
#include <GLES3/gl31.h>
#include <SDL2/SDL.h>
#include <ctime>
#include <cv.h>
#include <fstream>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

void logSdlError(const std::string &msg) {
  std::cout << msg << "error: " << SDL_GetError() << std::endl;
}

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

// Function turn a cv::Mat into a texture, and return the texture ID as a GLuint for use
GLuint matToTexture(cv::Mat &mat, GLenum wrapFilter)
{
	// Generate a number for our textureID's unique handle
	GLuint textureID;
	glGenTextures(1, &textureID);
 
	// Bind to our texture handle
	glBindTexture(GL_TEXTURE_2D, textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 
	// Set texture clamping method
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);
 
	// Set incoming texture format to:
	// GL_BGR       for CV_CAP_OPENNI_BGR_IMAGE,
	// GL_LUMINANCE for CV_CAP_OPENNI_DISPARITY_MAP,
	// Work out other mappings as required ( there's a list in comments in main() )
	GLenum inputColourFormat = GL_RGB;
	if (mat.channels() == 1)
	{
		inputColourFormat = GL_LUMINANCE;
	}
 
	// Create the texture
	glTexImage2D(GL_TEXTURE_2D,     // Type of texture
	             0,                 // Pyramid level (for mip-mapping) - 0 is the top level
	             GL_RGB,            // Internal colour format to convert to
	             mat.cols,          // Image width  i.e. 640 for Kinect in standard mode
	             mat.rows,          // Image height i.e. 480 for Kinect in standard mode
	             0,                 // Border width in pixels (can either be 1 or 0)
	             inputColourFormat, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
	             GL_UNSIGNED_BYTE,  // Image data type
	             mat.ptr());        // The actual image data itself
 
	return textureID;
}

int displaySdl() {
  VideoCapture cap;
  if (cap.open(0)) {
    std::cout << "Opened the camera." << std::endl;
  } else {
    std::cout << "Couldn't open the camera!" << std::endl;
    return 1;
  }

  if (!cap.set(CAP_PROP_FPS, 120)) {
    std::cout << "Couldn't set the FPS" << std::endl;
  }
  if (!cap.set(CAP_PROP_FRAME_WIDTH, 320)) {
    std::cout << "Couldn't set the width" << std::endl;
  }
  if (!cap.set(CAP_PROP_FRAME_HEIGHT, 240)) {
    std::cout << "Couldn't set the height" << std::endl;
  }
  if (!cap.set(CAP_PROP_AUTO_EXPOSURE, 0.25)) {
    std::cout << "couldn't set auto exposure" << std::endl;
  }
  // NOTE: this exposure value was chosen to make the hand light and the
  // background dark
  if (!cap.set(CAP_PROP_EXPOSURE, 0.005)) {
    std::cout << "couldn't set exposure" << std::endl;
  }

  // TODO: investigate capturing directly in grayscale
  /*if (!cap.set(CV_CAP_PROP_FORMAT, CV_8UC1)) {
    std::cout << "Couldn't set the format" << std::endl;
    }*/

  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    logSdlError("SDL_Init Error: ");
    return 1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  atexit(SDL_Quit);

  // Make a window
  SDL_Window *const win = SDL_CreateWindow(
      "Hello World!", 300, 100, 320, 240, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
  if (win == nullptr) {
    logSdlError("SDL_CreateWindow Error: ");
    return 1;
  }

  SDL_GLContext context = SDL_GL_CreateContext(win);
  if (context == nullptr) {
    logSdlError("SDL_GL_CreateContext: ");
    return 1;
  }
  const GLubyte *version = glGetString(GL_VERSION);
  if (version == 0) {
    logSdlError("GL Version: ");
    return 1;
  }
  std::cout << "Version: " << version << std::endl;

  // TODO: investigate what the best combination of these is for the screen
  if (SDL_GL_SetSwapInterval(0)) {
    logSdlError("Unable to disable VSync");
  }

  if (SDL_SetHint(SDL_HINT_RENDER_VSYNC, 0)) {
    logSdlError("Unable to disable VSync hint");
  }

  Mat image;

  //Effect *const effect = new BasicTracer(ren);
  
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // An array of 3 vectors which represents 3 vertices
  const GLfloat vertices[] = {
    -1, -1, 0,
    -1,  1, 0,
     1,  1, 0,
     1, -1, 0};

  const GLubyte indices[] = {
    0, 1, 2,
    0, 2, 3
  };

  const GLfloat textureVertices[] = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f,  1.0f,
        0.0f,  0.0f,
    };

  // This will identify our vertex buffer
  GLuint vertexbuffer;
  // Generate 1 buffer, put the resulting identifier in vertexbuffer
  glGenBuffers(1, &vertexbuffer);
  // The following commands will talk about our 'vertexbuffer' buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  // Give our vertices to OpenGL.
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Create and compile our GLSL program from the shaders
  GLuint programID = LoadShaders( "vertex_shader.glsl", "fragment_shader.glsl" );


  bool done = false;
  SDL_Event e;
  while (!done) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
      case SDL_KEYDOWN:
      case SDL_MOUSEBUTTONDOWN:
        done = true;
        break;
      default:
        // No-op
        break;
      }
    }
    if (done) {
      break;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader
    glUseProgram(programID);

    cap.read(image);
    GLuint imageTex = matToTexture(image, GL_CLAMP_TO_EDGE);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, imageTex);

    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );
    //glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, textureVertices);
    glEnableVertexAttribArray(1);
    // Draw the triangle !
    //glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);

    SDL_GL_SwapWindow(win);

    glDeleteTextures(1, &imageTex);

   // effect->render(image);
    //effect->calculateFramerate();
  }

  //SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();

  //cap.release();
  return 0;
}

int main(int argc, char **argv) { return displaySdl(); }
