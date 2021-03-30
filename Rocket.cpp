// Include standard headers
#include <stdio.h>
#include <stdlib.h>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;
#include <common/controls.hpp>
#include <glut.h>
#include <common/shader.hpp>
#include <common/texture.hpp>
#define GL_PI 3.1415f

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Tutorial 04 - Colored Cube", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");
	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");



	
	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = { 
		1.0f,0.0f,0.0f,
		1.0f,2.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		 1.0f, 0.0f,1.0f,
		1.0f,2.0f,0.0f,
		1.0f, 2.0f,1.0f,
		 1.0f,2.0f, 0.0f,
		0.0f,2.0f,0.0f,
		 1.0f,2.0f,1.0f,
		 0.0f, 2.0f,0.0f,
		 0.0f,2.0f,1.0f,
		1.0f,2.0f,1.0f,
		0.0f,0.0f,0.0f,
		0.0f, 2.0f, 0.0f,
		0.0f, 0.0f,1.0f,
		 0.0f,0.0f, 1.0f,
		0.0f,2.0f, 0.0f,
		0.0f,2.0f,1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f,0.0f, 0.0f,
		 1.0f,0.0f, 1.0f,
		 0.0f, 0.0f, 0.0f,
		 0.0f,0.0f,1.0f,
		 1.0f, 0.0f,1.0f,
		 1.0f,0.0f,0.0f,
		 1.0f, 2.0f, 0.0f,
		 0.0f,2.0f, 0.0f,
		 1.0f, 0.0f, 0.0f,
		 0.0f, 2.0f,0.0f,
		0.0f, 0.0f,0.0f,
		 1.0f, 0.0f, 1.0f,
		1.0f, 2.0f,1.0f,
		0.0f, 2.0f, 1.0f,
		 1.0f, 0.0f, 1.0f,
		0.0f, 2.0f, 1.0f,
		 0.0f,0.0f, 1.0f
	};

	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = { 
		0.9,  0.9,  0.9,
		0.7,  0.7,  0.7,
		0.9,  0.9,  0.9,
		0.7,  0.7,  0.7,
		0.9,  0.9,  0.9,
		0.7,  0.7,  0.7,
		1,  1,  1,
		1,  1,  1,
		0.9,  0.9,  0.9,
		1,  1,  1,
		1,  1,  1,
		0.9,  0.9,  0.9,
		1,  1,  1,
		1,  1,  1,
		0.7,  0.7,  0.7,
		1,  1,  1,
		0.9,  0.9,  0.9,
		1,  1,  1,
		1,  1,  1,
		0.7,  0.7,  0.7,
		0.9,  0.9,  0.9,
		1,  1,  1,
		0.7,  0.7,  0.7,
		1,  1,  1,
		0.9,  0.9,  0.9,
		1,  1,  1,
		0.7,  0.7,  0.7,
		1,  1,  1,
		0.7,  0.7,  0.7,
		0.9,  0.9,  0.9,
		1,  1,  1,
		1,  1,  1,
		0.7,  0.7,  0.7,
		1,  1,  1,
		0.7,  0.7,  0.7,
		1,  1,  1
		/*0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f*/
	};
	static const GLfloat headcolor[] = { 
		0.0f,0.0f,0.3f,
		0.0f,0.1f,0.5f,
		0.0f,0.0f,0.3f,
		0.0f,0.1f,0.5f,
		0.0f,0.0f,0.3f,
		0.0f,0.1f,0.5f,
		0.0f,0.0f,0.3f,
		0.0f,0.1f,0.5f,
		0.0f,0.0f,0.3f,
		0.0f,0.1f,0.5f,
		0.0f,0.0f,0.3f,
		0.0f,0.1f,0.5f,
		0.0f,0.0f,0.3f,
		0.0f,0.1f,0.5f,
		0.0f,0.0f,0.3f,
		0.0f,0.1f,0.5f,
		0.0f,0.0f,0.3f,
		0.0f,0.1f,0.5f,
	
	
	};
	static const GLfloat wingcolor[] = {
		0.8f,0.0f,0.0f,
		0.6f,0.0f,0.0f,
		0.8f,0.0f,0.0f,
		0.6f,0.0f,0.0f,
		0.8f,0.0f,0.0f,
		0.6f,0.0f,0.0f,
		0.8f,0.0f,0.0f,
		0.6f,0.0f,0.0f,
		0.8f,0.0f,0.0f,
		0.6f,0.0f,0.0f,
		0.8f,0.0f,0.0f,
		0.6f,0.0f,0.0f,
		0.8f,0.0f,0.0f,
		0.6f,0.0f,0.0f

	
	
	};
	static const GLfloat floorcolor[] = {
		0.9f, 0.6f, 0.2f,
		0.9f, 0.6f, 0.2f,
		0.9f, 0.6f, 0.2f,
		0.9f, 0.6f, 0.2f,
		0.9f, 0.6f, 0.2f,
		0.9f, 0.6f, 0.2f,
	};
	static const GLfloat suit1color[] = {
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
	};
	static const GLfloat suit2color[] = {
		0.7f,0.3f,0.0f,
		0.7f,0.3f,0.0f,
		0.7f,0.3f,0.0f,
		0.7f,0.3f,0.0f,
		0.7f,0.3f,0.0f,
		0.7f,0.3f,0.0f
	};
	static const GLfloat suit3color[] = {
		0.7f,0.7f,0.0f,
		0.7f,0.7f,0.0f,
		0.7f,0.7f,0.0f,
		0.7f,0.7f,0.0f,
		0.7f,0.7f,0.0f,
		0.7f,0.7f,0.0f
	};
	static const GLfloat suit4color[] = {
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f
	};
	static const GLfloat suit5color[] = {
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f
	};
	static const GLfloat wing1[] = {
		1.5f,0.0f,0.5f,
		1.0f,0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		 1.5f, 0.0f,0.5f,
		1.0f,1.0f,0.5f,
		1.0f, 0.0f,1.0f,
		 1.0f,0.0f, 0.0f,
		1.0f,1.0f,0.5f,
		 1.5f,0.0f,0.5f,
		 1.0f,0.0f,0.0f,
		1.0f,0.0f,1.0f,
		1.0f,1.0f,0.5f

	};
	static const GLfloat wing2[] = {
		1.0f,0.0f,1.0f,
		0.5f,0.0f, 1.5f,
		0.0f, 0.0f, 1.0f,
		 1.0f, 0.0f,1.0f,
		0.5f,0.0f,1.5f,
		0.5f, 1.0f,1.0f,
		 0.0f,0.0f, 1.0f,
		0.5f,0.0f,1.5f,
		 0.5f,1.0f,1.0f,
		 1.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.5f,1.0f,1.0f

	};
	static const GLfloat wing3[] = {
		0.0f,0.0f,0.0f,
		0.0f,0.0f, 1.0f,
		-0.5f, 0.0f, 0.5f,
		 0.0f, 0.0f,1.0f,
		-0.5f,0.0f,0.5f,
		0.0f, 1.0f,0.5f,
		 0.0f,0.0f, 0.0f,
		-0.5f,0.0f,0.5f,
		 0.0f,1.0f,0.5f,
		 0.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,
		0.0f,1.0f,0.5f

	};
	static const GLfloat wing4[] = {
		1.0f,0.0f,0.0f,
		0.0f,0.0f, 0.0f,
		0.5f, 1.0f, 0.0f,
		 1.0f, 0.0f,0.0f,
		0.5f,1.0f,0.0f,
		0.5f, 0.0f,-0.5f,
		 0.0f,0.0f, 0.0f,
		0.5f,1.0f,0.0f,
		 0.5f,0.0f,-0.5f,
		 1.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.5f,0.0f,-0.5f

	};
	static const GLfloat head[] = {
		1.0f,2.0f,0.0f,
		0.0f,2.0f, 0.0f,
		1.0f, 2.0f, 1.0f,
		 0.0f, 2.0f,0.0f,
		0.0f,2.0f,1.0f,
		0.5f, 3.0f,0.5f,
		 1.0f,2.0f, 1.0f,
		0.0f,2.0f,1.0f,
		 0.5f,3.0f,0.5f,
		 1.0f,2.0f,1.0f,
		1.0f,2.0f,0.0f,
		0.5f,3.0f,0.5f,
		0.0f,2.0f,0.0f,
		0.0f,2.0f,1.0f,
		0.5f,3.0f,0.5f,
		1.0f,2.0f,0.0f,
		0.0f,2.0f,0.0f,
		0.5f,3.0f,0.5f
	};
	static const GLfloat floor[] = {
		100.0f, 0.0f, -100.0f,
		100.0f, 0.0f, 100.0f,
		-100.0f, 0.0f, -100.0f,
		-100.0f, 0.0f, -100.0f,
		-100.0f, 0.0f, 100.0f,
		100.0f, 0.0f, 100.0f
	};
	static const GLfloat suit3[] = {
		0.2f,4.0f,0.0f,
		0.6f,4.0f,1.0f,
		0.6f,4.0f,0.0f,
		0.2f,4.0f,0.0f,
		0.2f,4.0f,1.0f,
		0.6f,4.0f,1.0f
	};
	static const GLfloat suit4[] = {
		0.6f,4.0f, 0.0f,
		0.9f,3.8f,0.0f,
		0.9f,3.8f,1.0f,
		0.6f,4.0f,0.0f,
		0.6f,4.0f,1.0f,
		0.9f,3.8f,1.0f
	};
	static const GLfloat suit5[] = {
		0.9f,3.8f,0.0f,
		1.2f,3.5f,0.0f,
		1.2f,3.5f,1.0f,
		0.9f,3.8f,0.0f,
		0.9f,3.8f,1.0f,
		1.2f,3.5f,1.0f
	};
	static const GLfloat suit2[] = {
		0.2f,4.0f, 0.0f,
		-0.2f,3.8f,0.0f,
		0.2f,4.0f,1.0f,
		0.2f,4.0f,1.0f,
		-0.2f,3.8f,0.0f,
		-0.2f,3.8f,1.0f
	};
	static const GLfloat suit1[] = {
		-0.2f,3.8f,0.0f,
		-0.5f,3.5f,0.0f,
		-0.5f,3.5f,1.0f,
		-0.2f,3.8f,0.0f,
		-0.2f,3.8f,1.0f,
		-0.5f,3.5f,1.0f
	};
	static const GLfloat wall[] = {
		100.0f,30.0f,-5.0f,
		-100.0f,30.0f,-5.0f,
		100.0f,0.0f,-5.0f,
		100.0f,0.0f,-5.0f,
		-100.0f,0.0f,-5.0f,
		-100.0f,30.0f,-5.0f,

		50.0f,30.0f,-5.0f,
		50.0f,30.0f,100.0f,
		50.0f,0.0f,-5.0f,
		50.0f,30.0f,100.0f,
		50.0f,0.0f,100.0f,
		50.0f,0.0f,-5.0f,

		-30.0f,30.0f,-5.0f,
		-30.0f,30.0f,100.0f,
		-30.0f,0.0f,-5.0f,
		-30.0f,30.0f,100.0f,
		-30.0f,0.0f,100.0f,
		-30.0f,0.0f,-5.0f,

		

	};
	static const GLfloat wallcolor[] = {
		0.5f,0.5f,1.0f,
		0.5f,0.5f,1.0f,
		0.5f,0.5f,1.0f,
		0.5f,0.5f,1.0f,
		0.5f,0.5f,1.0f,
		0.5f,0.5f,1.0f,
		0.5f,0.5f,1.0f,
		0.5f,0.5f,1.0f,
		0.5f,0.5f,1.0f,
		0.5f,0.5f,1.0f,
		0.5f,0.5f,1.0f,
		0.5f,0.5f,1.0f,
		0.5f,0.5f,1.0f,
		0.5f,0.5f,1.0f,
		0.5f,0.5f,1.0f,
		0.5f,0.5f,1.0f,
		0.5f,0.5f,1.0f,
		0.5f,0.5f,1.0f,

	};
	static const GLfloat line[] = {
		1.2f,3.5f,1.0f,
		1.2f,3.5f,0.9f,
		1.0f,2.0f,1.0f,
		1.0f,2.0f,0.9f,
		1.0f,2.0f,1.0f,
		1.2f,3.5f,0.9f,

		1.2f,3.5f,0.1f,
		1.2f,3.5f,0.0f,
		1.0f,2.0f,0.1f,
		1.0f,2.0f,0.0f,
		1.0f,2.0f,0.1f,
		1.2f,3.5f,0.0f,

		-0.5f,3.5f,1.0f,
		-0.5f,3.5f,0.9f,
		0.0f,2.0f,1.0f,
		0.0f,2.0f,0.9f,
		0.0f,2.0f,1.0f,
		-0.5f,3.5f,0.9f,

		-0.5f,3.5f,0.1f,
		-0.5f,3.5f,0.0f,
		0.0f,2.0f,0.1f,
		0.0f,2.0f,0.0f,
		0.0f,2.0f,0.1f,
		-0.5f,3.5f,0.0f
	};
	static const GLfloat linecolor[] = {
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f
	};
	//몸통
	GLuint vertexbuffer;  //이것이 우리의 버텍스 버퍼
	glGenBuffers(1, &vertexbuffer);  //버퍼를 하나 생성한다.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); //우리의 버텍스 버퍼에 대해서 다룬다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW); //우리의 버텍스를 opengl로 넘겨줌
	//몸통색깔
	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
	//날개색깔
	GLuint colorbuffer2;
	glGenBuffers(1, &colorbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wingcolor), wingcolor, GL_STATIC_DRAW);
	//뚜껑색깔
	GLuint colorbuffer3;
	glGenBuffers(1, &colorbuffer3);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(headcolor), headcolor, GL_STATIC_DRAW);
	//바닥색깔
	GLuint colorbuffer4;
	glGenBuffers(1, &colorbuffer4);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorcolor), floorcolor, GL_STATIC_DRAW);
	//낙하산1 색깔
	GLuint colorbuffer5;
	glGenBuffers(1, &colorbuffer5);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(suit1color), suit1color, GL_STATIC_DRAW);
	//낙하산2 색깔
	GLuint colorbuffer6;
	glGenBuffers(1, &colorbuffer6);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(suit2color), suit2color, GL_STATIC_DRAW);
	//낙하산 3 색깔
	GLuint colorbuffer7;
	glGenBuffers(1, &colorbuffer7);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer7);
	glBufferData(GL_ARRAY_BUFFER, sizeof(suit3color), suit3color, GL_STATIC_DRAW);
	//낙하산 4 색깔
	GLuint colorbuffer8;
	glGenBuffers(1, &colorbuffer8);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer8);
	glBufferData(GL_ARRAY_BUFFER, sizeof(suit4color), suit4color, GL_STATIC_DRAW);
	//낙하산 5 색깔
	GLuint colorbuffer9;
	glGenBuffers(1, &colorbuffer9);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer9);
	glBufferData(GL_ARRAY_BUFFER, sizeof(suit5color), suit5color, GL_STATIC_DRAW);
	//벽색깔
	GLuint colorbuffer10;
	glGenBuffers(1, &colorbuffer10);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer10);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wallcolor), wallcolor, GL_STATIC_DRAW);
	//선색깔
	GLuint colorbuffer11;
	glGenBuffers(1, &colorbuffer11);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer11);
	glBufferData(GL_ARRAY_BUFFER, sizeof(linecolor), linecolor, GL_STATIC_DRAW);

	GLuint linebuffer;  //이것이 우리의 버텍스 버퍼2 날개용
	glGenBuffers(1, &linebuffer);  //버퍼를 하나 생성한다.
	glBindBuffer(GL_ARRAY_BUFFER, linebuffer); //우리의 버텍스 버퍼에 대해서 다룬다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW); //우리의 버텍스를 opengl로 넘겨줌
	GLuint vertexbuffer2;  //이것이 우리의 버텍스 버퍼2 날개용
	glGenBuffers(1, &vertexbuffer2);  //버퍼를 하나 생성한다.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2); //우리의 버텍스 버퍼에 대해서 다룬다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(wing1), wing1, GL_STATIC_DRAW); //우리의 버텍스를 opengl로 넘겨줌

	GLuint vertexbuffer3;  //날개2
	glGenBuffers(1, &vertexbuffer3);  //버퍼를 하나 생성한다.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3); //우리의 버텍스 버퍼에 대해서 다룬다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(wing2), wing2, GL_STATIC_DRAW); //우리의 버텍스를 opengl로 넘겨줌
	GLuint vertexbuffer4;  //날개3
	glGenBuffers(1, &vertexbuffer4);  //버퍼를 하나 생성한다.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer4); //우리의 버텍스 버퍼에 대해서 다룬다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(wing3), wing3, GL_STATIC_DRAW); //우리의 버텍스를 opengl로 넘겨줌
	GLuint vertexbuffer5;  //날개4
	glGenBuffers(1, &vertexbuffer5);  //버퍼를 하나 생성한다.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer5); //우리의 버텍스 버퍼에 대해서 다룬다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(wing4), wing4, GL_STATIC_DRAW); //우리의 버텍스를 opengl로 넘겨줌
	GLuint vertexbuffer6;  //뚜껑
	glGenBuffers(1, &vertexbuffer6);  //버퍼를 하나 생성한다.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer6); //우리의 버텍스 버퍼에 대해서 다룬다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(head), head, GL_STATIC_DRAW); //우리의 버텍스를 opengl로 넘겨줌
	GLuint vertexbuffer7;  //바닥
	glGenBuffers(1, &vertexbuffer7);  //버퍼를 하나 생성한다.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer7); //우리의 버텍스 버퍼에 대해서 다룬다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW); //우리의 버텍스를 opengl로 넘겨줌
	GLuint vertexbuffer8;  //낙하산1
	glGenBuffers(1, &vertexbuffer8);  //버퍼를 하나 생성한다.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer8); //우리의 버텍스 버퍼에 대해서 다룬다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(suit1), suit1, GL_STATIC_DRAW); //우리의 버텍스를 opengl로 넘겨줌
	GLuint vertexbuffer9;  //낙하산2
	glGenBuffers(1, &vertexbuffer9);  //버퍼를 하나 생성한다.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer9); //우리의 버텍스 버퍼에 대해서 다룬다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(suit2), suit2, GL_STATIC_DRAW); //우리의 버텍스를 opengl로 넘겨줌
	GLuint vertexbuffer10;  //낙하산3
	glGenBuffers(1, &vertexbuffer10);  //버퍼를 하나 생성한다.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer10); //우리의 버텍스 버퍼에 대해서 다룬다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(suit3), suit3, GL_STATIC_DRAW); //우리의 버텍스를 opengl로 넘겨줌
	GLuint vertexbuffer11;  //낙하산4
	glGenBuffers(1, &vertexbuffer11);  //버퍼를 하나 생성한다.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer11); //우리의 버텍스 버퍼에 대해서 다룬다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(suit4), suit4, GL_STATIC_DRAW); //우리의 버텍스를 opengl로 넘겨줌
	GLuint vertexbuffer12;  //낙하산5
	glGenBuffers(1, &vertexbuffer12);  //버퍼를 하나 생성한다.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer12); //우리의 버텍스 버퍼에 대해서 다룬다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(suit5), suit5, GL_STATIC_DRAW); //우리의 버텍스를 opengl로 넘겨줌
	GLuint vertexbuffer13;  //벽
	glGenBuffers(1, &vertexbuffer13);  //버퍼를 하나 생성한다.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer13); //우리의 버텍스 버퍼에 대해서 다룬다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(wall), wall, GL_STATIC_DRAW); //우리의 버텍스를 opengl로 넘겨줌

	// For speed computation
	double lastTime = glfwGetTime();
	double lastFrameTime = lastTime;
	float angle = 0.0f;
	vec3 gOrientation1;
	vec3 gro1, gro2;
	int start = 0;
	float main = 0.000215f;
	float gravity = 0.00000418f;
	float velocity = 0;
	float sky = 0;
	int up = 0;
	gro1.x = 0.0f;
	gro1.y = 0.0f;
	int suit = 0;
	int close = 0;
	do{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Use our shader
		glUseProgram(programID);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {  //spacebar 누르면출발
			start = 1;
			sky = 1;
		}
		//지속적으로 회전하기위해 deltaTime값을 구한다.
		double currentTime = glfwGetTime();
		float deltaTime = (float)(currentTime - lastFrameTime);
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
			suit = 1;
		}
		if (sky == 1)      //하늘에 떠있는 경우
		{
			if (suit == 0) {
				velocity += ((main - gravity)*deltaTime);  //가속도 붙여서 속력변화
				if (velocity < -0.03f)   //속도가 줄어 멈추게되는경우
				{
					start = 0;
				}
				if (velocity > 0.009f)  //속도가 일정이상 올라가는 경우 엔진 중지
				{
					main = 0.0f;
				}
				if (start == 1)
				{
					gro1.x += 0.015f;
					gro1.y += (4 * velocity*deltaTime);
				}
			}
			else {
				gro1.y -= 0.006f;
			}
		}
		if (gro1.y < 0)
		{
			start = 0;
			sky = 0;
		}
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
			if (close == 0) {
				close = 1;
			}
			else {
				close = 0;
			}
		}
		// Send our transformation to the currently bound shader, 
		computeMatricesFromInputs();
		glm::mat4 ViewMatrix;
		if (close == 0) {
			ViewMatrix = getViewMatrix();
		}
		else {
			ViewMatrix = glm::lookAt(
				glm::vec3(gro1.x+3, gro1.y+3, 10.0f), // Camera is at (4,3,-3), in World Space
				glm::vec3(gro1.x, gro1.y, 0.0f), // and looks at the origin
				glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
			);
		}
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 Model2 = glm::mat4(1.0f);
		//날개2
		glm::mat4 Model3 = glm::mat4(1.0f);
		//날개3
		glm::mat4 Model4 = glm::mat4(1.0f);
		//날개4
		glm::mat4 Model5 = glm::mat4(1.0f);
		//뚜껑
		glm::mat4 Model6 = glm::mat4(1.0f);
		//바닥
		glm::mat4 Model7 = glm::mat4(1.0f);
		glm::mat4 Model8 = glm::mat4(1.0f);
		glm::mat4 Model9 = glm::mat4(1.0f);
		glm::mat4 Model10 = glm::mat4(1.0f);
		glm::mat4 Model11 = glm::mat4(1.0f);
		glm::mat4 Model12 = glm::mat4(1.0f);
		glm::mat4 Model13 = glm::mat4(1.0f);
		glm::mat4 Model14 = glm::mat4(1.0f);
		glm::mat4 MVP;
		glm::mat4 MVP2;
		glm::mat4 MVP3;
		glm::mat4 MVP4;
		glm::mat4 MVP5;
		glm::mat4 MVP6;
		glm::mat4 MVP7;
		glm::mat4 MVP8;
		glm::mat4 MVP9;
		glm::mat4 MVP10;
		glm::mat4 MVP11;
		glm::mat4 MVP12;
		glm::mat4 MVP13;
		glm::mat4 MVP14;
		
		glm::mat4 TranslationMatrix1 = translate(mat4(), gro1); // A bit to the left
		//glm::mat4 RotationMatrix1 = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
		ModelMatrix = ModelMatrix*TranslationMatrix1;
		// Our ModelViewProjection : multiplication of our 3 matrices
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix; // Remember, matrix multiplication is the other way around
		// Our ModelViewProjection : multiplication of our 3 matrices
		MVP2 = ProjectionMatrix * ViewMatrix * ModelMatrix; // Remember, matrix multiplication is the other way around
		// Our ModelViewProjection : multiplication of our 3 matrices
		MVP3 = ProjectionMatrix * ViewMatrix * ModelMatrix;; // Remember, matrix multiplication is the other way around
		// Our ModelViewProjection : multiplication of our 3 matrices
		MVP4 = ProjectionMatrix * ViewMatrix * ModelMatrix; // Remember, matrix multiplication is the other way around
		// Our ModelViewProjection : multiplication of our 3 matrices
		MVP5 = ProjectionMatrix * ViewMatrix * ModelMatrix; // Remember, matrix multiplication is the other way around
		// Our ModelViewProjection : multiplication of our 3 matrices
		MVP6 = ProjectionMatrix * ViewMatrix * ModelMatrix; // Remember, matrix multiplication is the other way around
		// Our ModelViewProjection : multiplication of our 3 matrices
		MVP7 = ProjectionMatrix * ViewMatrix * Model7; // Remember, matrix multiplication is the other way around
		// in the "MVP" uniform

		//낙하산들
		MVP8 = ProjectionMatrix * ViewMatrix * ModelMatrix;
		MVP9 = ProjectionMatrix * ViewMatrix * ModelMatrix;
		MVP10 = ProjectionMatrix * ViewMatrix * ModelMatrix;
		MVP11 = ProjectionMatrix * ViewMatrix * ModelMatrix;
		MVP12 = ProjectionMatrix * ViewMatrix * ModelMatrix;
		MVP13 = ProjectionMatrix * ViewMatrix * Model13; // Remember, matrix multiplication is the other way around
		MVP14 = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		
		//버퍼의 첫번째 속성값 : 버텍스들
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset  배열 버퍼의 오프셋, 옮기는 값
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		//날개 1
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset  배열 버퍼의 오프셋, 옮기는 값
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 4 * 3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		//날개2
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP3[0][0]);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset  배열 버퍼의 오프셋, 옮기는 값
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 4 * 3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		//날개3
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP4[0][0]);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer4);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset  배열 버퍼의 오프셋, 옮기는 값
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 4 * 3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		//날개4
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP5[0][0]);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer5);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset  배열 버퍼의 오프셋, 옮기는 값
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer2);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 4 * 3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		//뚜껑
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP6[0][0]);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer6);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset  배열 버퍼의 오프셋, 옮기는 값
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer3);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 6 * 3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		//벽

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP13[0][0]);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer13);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset  배열 버퍼의 오프셋, 옮기는 값
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer10);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 6 * 3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		//바닥
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP7[0][0]);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer7);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset  배열 버퍼의 오프셋, 옮기는 값
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer4);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 2 * 3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		if (suit == 1) {
			//낙하산 선
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP14[0][0]);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, linebuffer);
			glVertexAttribPointer(
				0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset  배열 버퍼의 오프셋, 옮기는 값
			);
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, colorbuffer11);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);
			glDrawArrays(GL_TRIANGLES, 0, 8 * 3); // 12*3 indices starting at 0 -> 12 triangles
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

			//낙하산1
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP8[0][0]);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer8);
			glVertexAttribPointer(
				0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset  배열 버퍼의 오프셋, 옮기는 값
			);

			// 2nd attribute buffer : colors
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, colorbuffer5);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);
			glDrawArrays(GL_TRIANGLES, 0, 2 * 3); // 12*3 indices starting at 0 -> 12 triangles

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			//낙하산2
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP9[0][0]);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer9);
			glVertexAttribPointer(
				0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset  배열 버퍼의 오프셋, 옮기는 값
			);

			// 2nd attribute buffer : colors
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, colorbuffer6);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);
			glDrawArrays(GL_TRIANGLES, 0, 2 * 3); // 12*3 indices starting at 0 -> 12 triangles

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			//낙하산3
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP10[0][0]);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer10);
			glVertexAttribPointer(
				0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset  배열 버퍼의 오프셋, 옮기는 값
			);

			// 2nd attribute buffer : colors
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, colorbuffer7);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);
			glDrawArrays(GL_TRIANGLES, 0, 2 * 3); // 12*3 indices starting at 0 -> 12 triangles

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

			//낙하산4
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP11[0][0]);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer11);
			glVertexAttribPointer(
				0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset  배열 버퍼의 오프셋, 옮기는 값
			);

			// 2nd attribute buffer : colors
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, colorbuffer8);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);
			glDrawArrays(GL_TRIANGLES, 0, 2 * 3); // 12*3 indices starting at 0 -> 12 triangles

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

			//낙하산5
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP12[0][0]);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer12);
			glVertexAttribPointer(
				0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset  배열 버퍼의 오프셋, 옮기는 값
			);

			// 2nd attribute buffer : colors
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, colorbuffer9);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);
			glDrawArrays(GL_TRIANGLES, 0, 2 * 3); // 12*3 indices starting at 0 -> 12 triangles

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
		}
		
		// Draw the triangle !

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

