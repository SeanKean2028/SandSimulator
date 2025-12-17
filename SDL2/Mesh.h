#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <vector>
#include <string>
#include "Shader.h"
using namespace std;
struct Texture {
	unsigned int id;
	string type;
	string path;
};
class Mesh {
	//Holds vertex floats pos 3, normals 3, and texcoords 2
	float* vertices;
	//count of vertices
	int vertexCount = 0;
	//floats contained in each vertex
	int floatsPerVertex = 8;
	//ID
	GLuint VAO,VBO,EBO = 0;
	int* indices;
	int indicesCount;

	Texture texture;
	//Shader used to draw object
	ShaderProgram shaderProgram;
public: 
	Mesh() {

	}
	Mesh(float* _vertices, int _vertexCount, int* _indices, int _indicesCount, ShaderProgram _shaderProgram){
		vertices = _vertices;
		vertexCount = _vertexCount;
		
		indices = _indices;
		indicesCount = _indicesCount;

		shaderProgram = _shaderProgram;
		GenerateElementBufferObject();
	}

	Mesh(float* _vertices, int _vertexCount, ShaderProgram _shaderProgram) {
		vertices = _vertices;
		vertexCount = _vertexCount;
		
		shaderProgram = _shaderProgram;
		GenerateArrayObject();
	}
private: 
	//Indices Used
	void GenerateElementBufferObject() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * floatsPerVertex * sizeof(float), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
		SetAttributePointers();
	}
	
	//No Indices
	void GenerateArrayObject() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);
		SetAttributePointers();
	}
	void SetAttributePointers() {
		// Position attribute (location = 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Normal attribute (location = 1)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// TexCoord attribute (location = 2)
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}
public : 
	void DrawMesh() {
		shaderProgram.use();
		//Set Up Textures
		if(texture.id != 0)
			glBindTexture(GL_TEXTURE_2D, texture.id);

		glBindVertexArray(VAO);
		if (indicesCount > 0) {
			glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
		}
		else {
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		}
		glBindVertexArray(0);
	}
	void Delete() {
		if (EBO) glDeleteBuffers(1, &EBO);
		if (VBO) glDeleteBuffers(1, &VBO);
		if (VAO) glDeleteVertexArrays(1, &VAO);
	}
};