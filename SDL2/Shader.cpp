#pragma once
#include <GL/glew.h>
#include <gtc/type_ptr.hpp>
#include <glm.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Shader.h"
	// Load shader code from file. Just insert type: either GL_VERTEX_SHADER, or GL_FRAGMENT_SHADER
	Shader::Shader(string _filePath, GLenum _type) {
		filePath = _filePath;
		type = _type;
	}
	GLuint Shader::compileShader() {
		//Open File
		std::ifstream file(filePath);
		if (!file.is_open()) {
			std::cerr << "ERROR: Cannot open shader file: " << filePath << std::endl;
			return 0;
		}
		//Read file
		std::stringstream ss;
		ss << file.rdbuf();
		std::string codeStr = ss.str();
		const char* code = codeStr.c_str();
		// Create and compile shader
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &code, nullptr);
		glCompileShader(shader);

		// Check compilation
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			std::cerr << "ERROR: Shader compilation failed ("
				<< filePath << "):\n" << infoLog << std::endl;
		}
		return shader;
	}
	//Delete our shader
	void Shader::Delete() {
		glDeleteShader(id);
	}

	ShaderProgram::ShaderProgram() {
	}
	ShaderProgram::ShaderProgram(VertexShader& vertexShader, FragmentShader& fragmentShader) {
		// Compiles both shaders and sets their GLuint ids
		vertexShader.id = vertexShader.compileShader();
		fragmentShader.id = fragmentShader.compileShader();

		// Create shader program
		ID = glCreateProgram();
		glAttachShader(ID, vertexShader.id);
		glAttachShader(ID, fragmentShader.id);
		glBindFragDataLocation(ID, 0, "outColor");
		glLinkProgram(ID);

		//Check if linking worked
		GLint success;
		char infoLog[512];
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}
	void ShaderProgram::use() {
		glUseProgram(ID);
	}
	//Sets bool uniform with name and value
	void ShaderProgram::setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	//Sets int uniform with name and value
	void ShaderProgram::setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	//Sets float uniform with name and value
	void ShaderProgram::setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	//Sets Matrix 4 uniform with name and value
	void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}

	//Sets Vec3 uniform with name and values
	void ShaderProgram::setVec3(const std::string& name, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}

	//Sets Vec3 uniform with name and vec3
	void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const {
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
	}
	//Sets Vec3 uniform with name and vec3
	void ShaderProgram::setVec4(const std::string& name, const glm::vec4& value) const {
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
	}
	//Disposes program
	void ShaderProgram::Delete() {
		glDeleteProgram(ID);
	}

