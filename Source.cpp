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
#include "Sphere.h"
#include "Cone.h"
//#include "ConeB.h"

/**************************************************************
********************[  Outside Def's   ]***********************
***************************************************************/
//This method will change the scene between A-E
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

const GLuint nodes = 21;

//Indication of what we intend to see
GLuint gamemode = 'A';

void populateWithNormal(GLfloat* arr, GLuint lineNo, GLuint noOfNormals);

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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertIndxs * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Texture Coord attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertIndxs * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
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
		GLfloat camX = sin((GLfloat)glfwGetTime()) * radius;
		GLfloat camZ = cos((GLfloat)glfwGetTime()) * radius;
		view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 model;
		glm::mat4 projection;
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
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


		/**************************************************************
		********************[  Drawing Time! ]*******************
		***************************************************************/
		glBindVertexArray(VAO);
		if (gamemode == 'A' || gamemode == 'B' )
		{
			// Draw DA COUNTOURS
			for (GLuint line = 0; line < 2 * nodes; line++)
			{
				glDrawArrays(GL_TRIANGLE_FAN, nodes*line, nodes);
			}

			if (gamemode == 'B') {
				//Draw the normals
				glDrawArrays(GL_LINES, nodes*nodes * 2, nodes*nodes * 2);
			}

		}
		if (gamemode == 'C') {
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
