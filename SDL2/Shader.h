#pragma once
#include <GL/glew.h>
#include <gtc/type_ptr.hpp>
#include <glm.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
class Shader {
public:
	GLuint id;
	string filePath = "";
	GLenum type;
	// Load shader code from file. Just insert type: either GL_VERTEX_SHADER, or GL_FRAGMENT_SHADER
	Shader(string _filePath, GLenum _type) {
		filePath = _filePath;
		type = _type;
	}
	GLuint compileShader() {
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
	void Delete() {
		glDeleteShader(id);
	}
};
class VertexShader : public Shader {
public:
	using Shader::Shader;
};
class FragmentShader : public Shader {
public:
	using Shader::Shader;
};

class ShaderProgram {
public : 
	GLuint ID = 0;
	ShaderProgram() {
	}
	ShaderProgram(VertexShader& vertexShader, FragmentShader& fragmentShader) {
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
	void use() {
		glUseProgram(ID);
	}
	   	//Sets bool uniform with name and value
	void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	//Sets int uniform with name and value
	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	//Sets float uniform with name and value
	void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	//Sets Matrix 4 uniform with name and value
	void setMat4(const std::string& name, const glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}

	//Sets Vec3 uniform with name and values
	void setVec3(const std::string& name, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}

	//Sets Vec3 uniform with name and vec3
	void setVec3(const std::string& name, const glm::vec3& value) const {
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
	}
	//Sets Vec3 uniform with name and vec3
	void setVec4(const std::string& name, const glm::vec4& value) const {
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
	}
	//Disposes program
	void Delete() {
		glDeleteProgram(ID);
	}
};
