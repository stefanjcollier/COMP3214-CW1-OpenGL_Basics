#ifndef CONE_H
#define CONE_H

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

	void populateArrayWithCone(GLfloat* arr, GLuint numberOfPrecedingNodes, GLfloat radius, GLfloat height) {	
		//---------- Draw Spike
		//Create the top point
		GLfloat topx = 0.0f,
				topy = 0.0f,
				topz = height;

		GLuint offset = numberOfPrecedingNodes * vertIndxs;
		std::cout << "Offset:"<< offset << std::endl;

		midOffset = drawSurface(arr, offset, radius, topx, topy, topz);

		std::cout << "Offset:" << midOffset << std::endl;

		//---------- Draw Base Circle
		//Create the bottom point
		drawSurface(arr, midOffset, radius, 0.0f, 0.0f, 0.0f);
	}

	GLuint j = 0;
	GLuint drawSurface(GLfloat* arr, GLuint offset, GLfloat radius, 
		GLfloat tx, GLfloat ty, GLfloat tz)
	{
		// The index in the data array
		i = 0;

		//Save the first circle point (origin point)
		generatingNextCirclePoint(radius);
		GLfloat ox = myx,
				oy = myy,
				oz = myz;

		//circle node n_0
		arr[offset + j*vertIndxs + 0] = ox;
		arr[offset + j*vertIndxs + 1] = oy;
		arr[offset + j*vertIndxs + 2] = oz;

		arr[offset + j*vertIndxs + 3] = ox;
		arr[offset + j*vertIndxs + 4] = oy;
		arr[offset + j*vertIndxs + 5] = oz;

		arr[offset + j*vertIndxs + 6] = 0.0f;
		arr[offset + j*vertIndxs + 7] = 0.0f;
		j++;

		//cone top node
		arr[offset + j*vertIndxs + 0] = tx;
		arr[offset + j*vertIndxs + 1] = ty;
		arr[offset + j*vertIndxs + 2] = tz;

		arr[offset + j*vertIndxs + 3] = tx;
		arr[offset + j*vertIndxs + 4] = ty;
		arr[offset + j*vertIndxs + 5] = tz;

		arr[offset + j*vertIndxs + 6] = 0.5f;
		arr[offset + j*vertIndxs + 7] = 1.0f;
		j++;


		//Create all intermediary triangles
		while (generatingNextCirclePoint(radius)) {
			GLfloat percentThroughXaxis = i / (2 * nodesInDemiCircle + 1);

			//circle node n_i for the end of  the previous triangle
			arr[offset+j*vertIndxs + 0] = myx;
			arr[offset+j*vertIndxs + 1] = myy;
			arr[offset+j*vertIndxs + 2] = myz;

			arr[offset + j*vertIndxs + 3] = myx;
			arr[offset + j*vertIndxs + 4] = myy;
			arr[offset + j*vertIndxs + 5] = myz;

			arr[offset + j*vertIndxs + 6] = percentThroughXaxis;
			arr[offset + j*vertIndxs + 7] = 0.0f;
			j++;

			// circle node n_i for the  start of this triangle
			arr[offset + j*vertIndxs + 0] = myx;
			arr[offset + j*vertIndxs + 1] = myy;
			arr[offset + j*vertIndxs + 2] = myz;

			arr[offset + j*vertIndxs + 3] = myx;
			arr[offset + j*vertIndxs + 4] = myy;
			arr[offset + j*vertIndxs + 5] = myz;

			arr[offset + j*vertIndxs + 6] = percentThroughXaxis;
			arr[offset + j*vertIndxs + 7] = 0.0f;
			j++;


			//cone top node
			arr[offset+j*vertIndxs + 0] = tx;
			arr[offset+j*vertIndxs + 1] = ty;
			arr[offset+j*vertIndxs + 2] = tz;

			arr[offset + j*vertIndxs + 3] = tx;
			arr[offset + j*vertIndxs + 4] = ty;
			arr[offset + j*vertIndxs + 5] = tz;

			arr[offset + j*vertIndxs + 6] = 0.5f;
			arr[offset + j*vertIndxs + 7] = 1.0f;
			j++;
		}

		//Finishing of the very last triangle with the circle origin node
		arr[offset + j*vertIndxs + 0] = ox;
		arr[offset + j*vertIndxs + 1] = oy;
		arr[offset + j*vertIndxs + 2] = oz;

		arr[offset + j*vertIndxs + 3] = ox;
		arr[offset + j*vertIndxs + 4] = oy;
		arr[offset + j*vertIndxs + 5] = oz;

		arr[offset + j*vertIndxs + 6] = 1.0f;
		arr[offset + j*vertIndxs + 7] = 0.0f;
		j++;
		return offset + j*vertIndxs ;
	}


	void drawCone(GLuint offset) {
		//Draw surface one
		GLuint count = 2 * 3 * nodesInDemiCircle;
		glDrawArrays(GL_TRIANGLES, offset, count);
	}

private:
	GLuint nodesInDemiCircle, i;
	GLfloat myx, myy, myz = 0;
	GLuint midOffset;


	//Populates a given array (of length nodes*2) with a circle
	GLboolean generatingNextCirclePoint(GLfloat radius) {
		GLfloat rad_inc = glm::radians(360.0f / (nodesInDemiCircle-1));
		myx = radius*glm::cos(rad_inc*i);
		myy = radius*glm::sin(rad_inc*i);
		i++;
		return i < 2 * nodesInDemiCircle + 1;
		//The other bits are not my concern atm
	}
};

#endif