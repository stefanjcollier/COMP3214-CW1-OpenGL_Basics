// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLFW
#include <GLFW/glfw3.h>

#include "Sphere.h"

class Cone {
public:
	Cone(GLuint nodes) {
		nodesInDemiCircle = nodes;
	}

	void populateArrayWithCone(GLfloat* arr, GLuint lineNo, GLfloat radius, GLfloat height) {
		GLuint offset = lineNo * vertIndxs;
		
		//Save the first circle point
		generatingNextCirclePoint(radius);
		GLfloat ox = x, oy = y, oz = z;

		//Create the top point
		GLfloat tx = 0.0f, ty = 0.0f, tz = height;

		// The index in the data array
		GLuint j = 0;

		//circle node n_0
		arr[offset + j*vertIndxs + 0] = ox;
		arr[offset + j*vertIndxs + 1] = oy;
		arr[offset + j*vertIndxs + 2] = oz;
		j++;

		//cone top node
		arr[offset + j*vertIndxs + 0] = tx;
		arr[offset + j*vertIndxs + 1] = ty;
		arr[offset + j*vertIndxs + 2] = tz;
		j++;


		//Create all intermediary triangles
		while (generatingNextCirclePoint(radius)) {

			//circle node n_i for the end of  the previous triangle
			arr[offset+j*vertIndxs + 0] = x;
			arr[offset+j*vertIndxs + 1] = y;
			arr[offset+j*vertIndxs + 2] = z;
			j++;

			// circle node n_i for the  start of this triangle
			arr[offset + j*vertIndxs + 0] = x;
			arr[offset + j*vertIndxs + 1] = y;
			arr[offset + j*vertIndxs + 2] = z;
			j++;


			//cone top node
			arr[offset+j*vertIndxs + 0] = tx;
			arr[offset+j*vertIndxs + 1] = ty;
			arr[offset+j*vertIndxs + 2] = tz;
			j++;
		}

		//Finishing of the very last triangle with the circle origin node
		arr[offset + j*vertIndxs + 0] = ox;
		arr[offset + j*vertIndxs + 1] = oy;
		arr[offset + j*vertIndxs + 2] = oz;
		j++;
	}

	void drawCone(GLuint offset) {
		GLuint count = 6 * nodesInDemiCircle;
		glDrawArrays(GL_LINES, offset, count);
	}

private:
	GLuint nodesInDemiCircle, i;
	GLfloat x, y, z;


	//Populates a given array (of length nodes*2) with a circle
	GLboolean generatingNextCirclePoint(GLfloat radius) {
		GLfloat rad_inc = glm::radians(360.0f / nodesInDemiCircle);
			x = radius*glm::cos(rad_inc*i);
			y = radius*glm::sin(rad_inc*i);
			z = 0;
			i++;
			return i > 2 * nodesInDemiCircle;
			//The other bits are not my concern atm
		}
};
