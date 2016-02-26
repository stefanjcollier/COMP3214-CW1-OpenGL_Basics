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
		GLuint j = 0;
		GLfloat tx = 0.0f, ty = 0.0f, tz = height;
		while (generatingNextCirclePoint(radius)) {
			arr[offset+j*vertIndxs + 0] + x;
			arr[offset+j*vertIndxs + 1] + y;
			arr[offset+j*vertIndxs + 2] + z;
			j++;

			arr[offset+j*vertIndxs + 0] + tx;
			arr[offset+j*vertIndxs + 1] + ty;
			arr[offset+j*vertIndxs + 2] + tz;
			j++;

			arr[offset+j*vertIndxs + 0] + x;
			arr[offset+j*vertIndxs + 1] + y;
			arr[offset+j*vertIndxs + 2] + z;
			j++;
		}
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
	}
};