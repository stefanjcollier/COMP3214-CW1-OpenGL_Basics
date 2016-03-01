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
#include "Camera.h"

#include "Sphere.h"
#include "Cone.h"
//#include "ConeB.h"

/**************************************************************
********************[  Outside Def's   ]***********************
***************************************************************/
//This method will change the scene between A-E
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];

// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame



//Indication of what we intend to see
GLuint gamemode = 'A';
//Control of precision of circle curve
const GLuint nodes = 51;


void populateWithNormal(GLfloat* arr, GLuint lineNo, GLuint noOfNormals);

//TODO eventually delete me
void printArr(GLfloat* arr, GLuint n) {
	for (GLuint i = 0; i < n; i++) {
		std::cout << "METHOD:printArr:"<<i<<":"<<arr[i] << std::endl;
	}
}

GLfloat indices[2 * nodes*nodes];
GLuint i = 0;
void setIndxA(GLuint value) {
	indices[i] = value;
	i++;
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

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to  true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Build and compile our shader program
	Shader fancyLightShader("shader.vs", "lightShader.frag");

	Sphere genericSphere(nodes);
	Cone cone1(nodes);

	/**************************************************************
	********************[  Graphics Objs Setup ]*******************
	***************************************************************/
	//It's 4 because 2+2 * (etc) for both (longitude AND latitude coords) +
	//  Cone: (nodes: half nodes in circle => 2 demi circles = 1 circle)
	GLfloat vertices[ 4 * (nodes * nodes * vertIndxs) + (2*(vertIndxs * nodes * 2 * 3)) ];

	genericSphere.populateArrayWithSphere(vertices, 2.0f, 0.0f, 0.0f, 0.0f);

	populateWithNormal(vertices, 2*nodes*nodes, nodes*nodes);

	cone1.populateArrayWithCone(vertices, 4 * nodes*nodes, 0.5f, 1.5f);

	for (GLuint row = 1; row < nodes; row++) {
		for (GLuint column = 1; column < nodes; column++) {
			GLuint xny = column * vertIndxs + row;
			GLuint xn1 = 2 + 1;


		}
	}


	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertIndxs * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertIndxs * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Texture Coord attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertIndxs * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
	/**************************************************************
	********************[  GAME LOOP ]*******************
	***************************************************************/	
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) 
		//    and call corresponding response functions
		glfwPollEvents();
		do_movement();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/**************************************************************
		********************[  Lighting Magic! ]**************************
		***************************************************************/
		// Activate shader
		fancyLightShader.Use();
		GLint objectColorLoc = glGetUniformLocation(fancyLightShader.Program, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(fancyLightShader.Program, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(fancyLightShader.Program, "lightPos");
		GLint viewPosLoc = glGetUniformLocation(fancyLightShader.Program, "viewPos");
		if ('A' <= gamemode && gamemode <= 'B') {
			glUniform3f(objectColorLoc, 0.0f, 0.0f, 0.0f);
		}
		else {
			glUniform3f(objectColorLoc, 0.5f, 0.0f, 0.5f);
		}
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

		/**************************************************************
		********************[  Camera Magic! ]**************************
		***************************************************************/
		// Camera/View transformation
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(fancyLightShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(fancyLightShader.Program, "view");
		GLint projLoc = glGetUniformLocation(fancyLightShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glm::mat4 model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		/**************************************************************
		********************[  Drawing Time! ]*******************
		***************************************************************/
		glBindVertexArray(VAO);

		GLuint drawType = GL_LINE_LOOP;
		if (gamemode == 'C')
			drawType = GL_TRIANGLE_FAN;

		if ('A' <= gamemode && gamemode <= 'C' )
		{
			// Draw DA COUNTOURS
			for (GLuint line = 0; line < 2 * nodes; line++)
			{
				glDrawArrays(drawType, nodes*line, nodes);
			}
			if (gamemode == 'B') {
				//Draw the normals
				glDrawArrays(GL_LINES, nodes*nodes * 2, nodes*nodes * 2);
			}

		}
		if (gamemode == 'D') {
			cone1.drawCone(nodes*nodes * 4);
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
		gamemode = key;
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void do_movement()
{
	// Camera controls
	if (keys[GLFW_KEY_UP])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_DOWN])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_LEFT])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_RIGHT])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void populateWithNormal(GLfloat* arr, GLuint lineNo, GLuint noOfNormals) {
	GLuint offset = lineNo * vertIndxs;

	//Multiplier
	GLfloat m = 0.333f;
	GLfloat dx, dy, dz, x, y, z,  nx, ny, nz;
	GLuint j = 0;
	for (GLuint i = 0; i < noOfNormals*vertIndxs; i+= vertIndxs) {
		x = arr[i + 0];
		y = arr[i + 1];
		z = arr[i + 2];

		dx = arr[i + 3];
		dy = arr[i + 4];
		dz = arr[i + 5];

		nx = m*dx + x;
		ny = m*dy + y;
		nz = m*dz + z;

		//Set the positions
		arr[offset + j + 0] = x;
		arr[offset + j + 1] = y;
		arr[offset + j + 2] = z;
		j+= vertIndxs;

		arr[offset + j + 0] = nx;
		arr[offset + j + 1] = ny;
		arr[offset + j + 2] = nz;
		j += vertIndxs;

		//We simply ignore the other values as we are only going to draw lines
	}

}
