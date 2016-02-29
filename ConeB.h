#pragma once
#ifndef CONEB_H
#define CONEB_H
#include <iostream>

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

const GLfloat pi = 3.14159265359f;
const GLuint indexesPerPoint = 6;
const GLuint nodesPerCircle = 10;

class ConeB {
public:
	ConeB(GLfloat height, GLfloat radius) {
		rad = radius;
		hei = height;
	}

	//Cone has no texture, does include a normal
	void bindBuffersWithData(GLuint* VBO, GLuint* VAO, GLuint* EBO) {
		GLfloat vertices[2 + nodesPerCircle];
		GLuint indices[3*nodesPerCircle];
		
		//Mid Bot
		vertices[0] = 0.0f;
		vertices[1] = 0.0f;
		vertices[2] = 0.0f;

		//Mid Top
		vertices[indexesPerPoint + 0] = 0.0f;
		vertices[indexesPerPoint + 1] = 0.0f;
		vertices[indexesPerPoint + 2] = hei;

		//Draw Circle
		fillCircle(vertices, 2, rad);

		//Create The indices
		createIndices(indices);

		//Make the 
		bindBuffersToData(VBO, VAO, EBO, vertices, indices);
	}

private :
	GLfloat rad, hei;

	void fillCircle(GLfloat* arr, GLuint indexOffset, GLfloat radius){
		GLfloat rad_inc = glm::radians(360/(nodesPerCircle-1));

		GLfloat theta, x, y, z = 0.0f;
		for (int i = 0; i < nodesPerCircle; i++) {
			theta = i*rad_inc;
			x = radius*glm::cos(rad_inc*i);
			y = radius*glm::sin(rad_inc*i);

			arr[indexOffset + i*indexesPerPoint + 0] = x;
			arr[indexOffset + i*indexesPerPoint + 1] = y;
			arr[indexOffset + i*indexesPerPoint + 2] = z;
		}
	}

	void createIndices(GLuint* indices) {
		for (int i = 0; i < nodesPerCircle; i++){

		}
	}

	void bindBuffersToData(GLuint* VBO, GLuint* VAO, GLuint* EBO, GLfloat vertices[], GLuint indices[]) {
		glGenVertexArrays(1, VAO);
		glGenBuffers(1, VBO);

		glBindVertexArray(*VAO);

		glBindBuffer(GL_ARRAY_BUFFER, *VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, nodesPerCircle * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0); //Unbind VAO
	}

};


#endif