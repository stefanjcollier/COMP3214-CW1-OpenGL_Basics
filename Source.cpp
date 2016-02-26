#include <iostream>

/**************************************************************
********************[  Imports   ]*****************************
***************************************************************/
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include <SOIL.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"

/**************************************************************
********************[  Outside Def's   ]***********************
***************************************************************/
//This method will change the scene between A-E
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

//Circle constants
const GLfloat pi = 3.14159265359f;

const GLuint vertIndxs = 5; //The number of places required in the array for a single vertex
const GLuint nodesPerDemiRing = 15;
const GLuint noOfLines = 2 * nodesPerDemiRing; //TODO later remove this if it = nodesPerRing stil

GLfloat gimmeX(GLfloat x0, GLfloat r, GLfloat theta, GLfloat phi);
GLfloat gimmeY(GLfloat y0, GLfloat r, GLfloat theta, GLfloat phi);
GLfloat gimmeZ(GLfloat z0, GLfloat r, GLfloat theta);

//Fills given array 'arr' with the points for a sphere, where the points are organised by longitude rings, ring by ring.
void createLongitudeContours(GLfloat* arr, GLuint lineNo, GLfloat r, GLfloat x0, GLfloat y0, GLfloat z0, GLfloat phi);

//Fills given array 'arr' with the points for a sphere, where the points are organised by lattitude rings, ring by ring.
void createLattitudeContours(GLfloat* arr, GLuint lineNo, GLfloat r, GLfloat x0, GLfloat y0, GLfloat z0, GLfloat theta);

//TODO: Implement me
void seedCircle(GLfloat* arr, GLfloat radius);

//TODO eventually delete me
void printArr(GLfloat* arr, GLuint n) {
	for (GLuint i = 0; i < n; i++) {
		std::cout << "METHOD:printArr:"<<i<<":"<<arr[i] << std::endl;
	}
}

/**************************************************************
********************[  The Action Code!   ]********************
***************************************************************/
int main()
{
	/**************************************************************
	********************[  Boring Set up   ]********************
	***************************************************************/
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "This is Stefan's Amazing Coursework", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// This instructs it to obey our keyboard!
	glfwSetKeyCallback(window, key_callback);

	// Set this to  true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Build and compile our shader program
	Shader ourShader("shader.vs", "shader.frag");


	/**************************************************************
	********************[  Graphics Objs Setup ]*******************
	***************************************************************/
	GLfloat vertices[ 2 * noOfLines * nodesPerDemiRing * vertIndxs];

	//Populate verticies - Lattitude (Verticals)
	GLfloat delta_deg = 180.0f / (nodesPerDemiRing);
	for (GLuint line = 0; line < noOfLines; line++) {
		GLfloat phi = glm::radians(line*delta_deg); 
		createLattitudeContours(vertices, line, 1.0f, 0.0f, 0.0f, 0.0f, phi);
	}
	//Populate verticies - Longitude (Horizontals)
	for (GLuint longLine = noOfLines; longLine < noOfLines * 2; longLine++) {
		GLfloat theta = glm::radians((longLine-noOfLines)*delta_deg);
		createLongitudeContours(vertices, longLine, 1.0f, 0.0f, 0.0f, 0.0f, theta);
	}

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertIndxs * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Texture Coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertIndxs * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO


	/**************************************************************
	********************[  GAME LOOP ]*******************
	***************************************************************/	
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) 
		//    and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Activate shader
		ourShader.Use();

		/**************************************************************
		********************[  ACTION TIME! ]*******************
		***************************************************************/

		// Camera/View transformation
		glm::mat4 view;
		GLfloat radius = 10.0f;
		GLfloat camX = sin(glfwGetTime()) * radius;
		GLfloat camZ = cos(glfwGetTime()) * radius;
		view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 model;
		glm::mat4 projection;
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		// Get their uniform location
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");

		// Pass them to the shaders
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		// Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw Da COUNTOURS
		glBindVertexArray(VAO);
		for (GLuint line = 0; line < 2*noOfLines; line++)
		{
			glDrawArrays(GL_LINE_STRIP, nodesPerDemiRing*line, nodesPerDemiRing);
		}
		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

//Called whenever the keyboard is used! Will controll our scenes (A-E).
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (64 < key && key < 70)
	{
		std::cout << "This key [" << key << "] will do something " << std::endl;
	}
	else
	{
		std::cout << "This key [" << key << "] does nada, sozza ma bro-a-reeno " << std::endl;
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


GLfloat gimmeX(GLfloat x0, GLfloat r, GLfloat theta, GLfloat phi) {
	return x0 + (r * glm::cos(theta) * glm::cos(phi));
}
GLfloat gimmeY(GLfloat y0, GLfloat r, GLfloat theta, GLfloat phi) {
	return y0 + (r * glm::cos(theta) * glm::sin(phi));
}
GLfloat gimmeZ(GLfloat z0, GLfloat r, GLfloat theta){
	return z0 + (r * glm::sin(theta));
}

//Completes a longitude circle 
void createLongitudeContours(GLfloat* arr, GLuint lineNo, GLfloat r, GLfloat x0, GLfloat y0, GLfloat z0, GLfloat phi) {
	GLuint offset = lineNo * vertIndxs*nodesPerDemiRing;
	//How much we will increment 
	GLfloat inc = 2 * pi / (nodesPerDemiRing - 1); // "pi/(n-1)" allows for 0 <= theta <= 2*pi
	GLfloat theta, x, y, z;
	for (GLuint i = 0; i < nodesPerDemiRing; i++)
	{
		theta = i*inc;
		x = gimmeX(x0, r, theta, phi);
		y = gimmeY(y0, r, theta, phi);
		z = gimmeZ(z0, r, theta);

		arr[offset + vertIndxs*i]   = x;
		arr[offset + vertIndxs*i+1] = y;
		arr[offset + vertIndxs*i+2] = z;

		//Include null texture coords
		arr[offset + vertIndxs*i + 3] = 0.0f;
		arr[offset + vertIndxs*i + 4] = 0.0f;
	}
}

//Completes a lattitude circle 
void createLattitudeContours(GLfloat* arr, GLuint lineNo, GLfloat r, GLfloat x0, GLfloat y0, GLfloat z0, GLfloat theta) {
	GLuint offset = lineNo * vertIndxs*nodesPerDemiRing;

	//How much we will increment 
	GLfloat inc = 2 * pi / (nodesPerDemiRing - 1); // "pi/(n-1)" allows for 0 <= phi <= 2*pi
	GLfloat phi, x, y, z;
	for (GLuint i = 0; i < nodesPerDemiRing; i++)
	{
		phi = i*inc - pi/2;
		x = gimmeX(x0, r, theta, phi);
		y = gimmeY(y0, r, theta, phi);
		z = gimmeZ(z0, r, theta);

		arr[offset+vertIndxs*i] = x;
		arr[offset + vertIndxs*i + 1] = y;
		arr[offset + vertIndxs*i + 2] = z;

		//Include null texture coords
		arr[offset + vertIndxs*i + 3] = 0.0f;
		arr[offset + vertIndxs*i + 4] = 0.0f;
	}
}

void seedCircle(GLfloat* arr, GLfloat radius) {
	std::cout << "INCOMPLETE::seedCircle(arr,radius) unimplented!" << std::endl;

}


