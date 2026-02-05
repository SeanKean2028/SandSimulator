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
	Shader(string _filePath, GLenum _type);
	GLuint compileShader();
	//Delete our shader
	void Delete();
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
private :
	GLuint ID = 0;
public : 
	ShaderProgram();
	ShaderProgram(VertexShader& vertexShader, FragmentShader& fragmentShader);
	void use();
	GLuint GetId() { return ID; }
	//Sets bool uniform with name and value
	void setBool(const std::string& name, bool value) const;
	//Sets int uniform with name and value
	void setInt(const std::string& name, int value) const;
	//Sets float uniform with name and value
	void setFloat(const std::string& name, float value) const;
	//Sets Matrix 4 uniform with name and value
	void setMat4(const std::string& name, const glm::mat4& mat) const;
	//Sets Vec3 uniform with name and values
	void setVec3(const std::string& name, float x, float y, float z) const;

	//Sets Vec3 uniform with name and vec3
	void setVec3(const std::string& name, const glm::vec3& value) const;
	//Sets Vec3 uniform with name and vec3
	void setVec4(const std::string& name, const glm::vec4& value) const;
	//Disposes program
	void Delete();
};
