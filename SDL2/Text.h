#pragma once
#include <glm.hpp>
#include <map>
#include <string>
#include "Shader.h"
using namespace std;
struct Character {
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advanced;
};

class Text {
	map<char, Character> Characters;
	unsigned int VAO, VBO;
	const char* path;
	int fontSize;
public:
	ShaderProgram &shader;
	string text;
	float x;
	float y;
	float scale;
	glm::vec3 color;
	/*@Brief: Initializes Text class calling all initalize funcs
	*
	* Generates and calls functions that generate all textures, 
	* VAO, VBO, and attribute data.
	* 
	* Parameters: const char* _path to sont file, int fontsize for setting size of 
	* generated bitmap
	* 
	* Returns: Set's all parameters in Text class
	*/
	Text(const char* _path, int _fontSize, ShaderProgram &_shader, string text, float x, float y, float scale, glm::vec3 color);

	/*@Brief: Calls Generate Font, and Generate Mesh. Use if needing to set param data
	*
	* Set's path, and fontsize
	* Generates and calls functions that generate all textures,
	* VAO, VBO, and attribute data.
	*
	* Parameters: path, and size must be set if using!
	*
	* Returns: Set's class data
	*/
	void Init(const char* _path, int _fontSize, ShaderProgram &_shader, string text, float x, float y, float scale, glm::vec3 color);
	
	void SetString(string _text);

	/*@Brief: Initializes Freetype, and Fotn
	* 
	* Initializes FreeType directory, and Font structure.
	* Creates a texture for each char from char 0 => 128.
	* Creates the Character struct for our map.
	* Sets our map of chars, and Charcters in text class. 
	* 
	* @Params: Class data must be initialized
	* @Return: Sets map<char, Character> Characters in Text class
	*/
	void LoadFont();

	/*@Brief: Initializes our Mesh(VAO, VBO, Attribute Pointers)
	*   
	* Generates, and Binds: VAO, VBO. Set's Attribute pointer to a 6*4   
	* vertices data arr for Character Quads.
	* 
	* Parameters: None
	* Returns: Sets our Text classes VAO, and VBO binding to GPU
	*/
	void GenerateMesh();

	/*@Brief: Draws the inputed Text Argument
	* 
	* Binds: VAO, VBO, Textures. Generates vertex data of string of text.
	* 
	* Parameters: Text Args _textArgs
	* Returns: None
	*/
	void RenderText();
};