// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLFW
#include <GLFW/glfw3.h>

//The number of places required in the array for a single vertex
const GLuint vertIndxs = 9; 


class Sphere
{
public:
	Sphere(GLuint nodes) {
		nodesPerDemiRing = nodes;
		noOfLines = nodes;
	}

	//TODO: Allow for a initial x,y,z0's and radius
	void populateArrayWithSphere(GLfloat* vertices, GLfloat radius, GLfloat x0, GLfloat y0, GLfloat z0) {
		//Populate verticies - Lattitude (Verticals)
		GLfloat delta_deg = 360.0f / (nodesPerDemiRing-1);
		for (GLuint line = 0; line < noOfLines; line++) {
			GLfloat phi = glm::radians(line*delta_deg);
			createLattitudeContours(vertices, line, radius, x0, y0, z0, phi);
		}
		//Populate verticies - Longitude (Horizontals)
		for (GLuint longLine = noOfLines; longLine < noOfLines * 2; longLine++) {
			GLfloat theta = glm::radians((longLine - noOfLines)*delta_deg);
			createLongitudeContours(vertices, longLine, radius, x0, y0, z0, theta);
		}
	}

private:
	const GLfloat pi = 3.14159265359f;

	GLuint nodesPerDemiRing;
	GLuint noOfLines;

	GLfloat gimmeX(GLfloat x0, GLfloat r, GLfloat theta, GLfloat phi) {
		return x0 + (r * glm::cos(theta) * glm::cos(phi));
	}
	GLfloat gimmeY(GLfloat y0, GLfloat r, GLfloat theta, GLfloat phi) {
		return y0 + (r * glm::cos(theta) * glm::sin(phi));
	}
	GLfloat gimmeZ(GLfloat z0, GLfloat r, GLfloat theta) {
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

			arr[offset + vertIndxs*i + 0] = x;
			arr[offset + vertIndxs*i + 1] = y;
			arr[offset + vertIndxs*i + 2] = z;

			//Include Normal Direction
			arr[offset + vertIndxs*i + 3] = x - x0;
			arr[offset + vertIndxs*i + 4] = y - y0;
			arr[offset + vertIndxs*i + 5] = z - z0;

			//Include null texture coords
			arr[offset + vertIndxs*i + 6] = 0.0f;
			arr[offset + vertIndxs*i + 7] = 0.0f;
			arr[offset + vertIndxs*i + 8] = 0.0f;
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
			phi = i*inc - pi / 2;
			x = gimmeX(x0, r, theta, phi);
			y = gimmeY(y0, r, theta, phi);
			z = gimmeZ(z0, r, theta);

			arr[offset + vertIndxs*i + 0] = x;
			arr[offset + vertIndxs*i + 1] = y;
			arr[offset + vertIndxs*i + 2] = z;

			//Include Normal Direction
			arr[offset + vertIndxs*i + 3] = x - x0;
			arr[offset + vertIndxs*i + 4] = y - y0;
			arr[offset + vertIndxs*i + 5] = z - z0;

			//Include null texture coords
			arr[offset + vertIndxs*i + 6] = 0.0f;
			arr[offset + vertIndxs*i + 7] = 0.0f;
			arr[offset + vertIndxs*i + 8] = 0.0f;
		}
	}


};

