#pragma once
#include <glm.hpp>
#include <map>
#include <string>
#include "Font.h"
#include "Shader.h"
using namespace std;
class Text {
	map<char, Character> Characters;
	unsigned int VAO, VBO;
	const char* path;
	int fontSize;
public:
	ShaderProgram &shader;
	string text;
	string fontName;
	font _font;
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
	Text(const char* _path, int _fontSize, string _fontName, Font& _Font, ShaderProgram& _shader, string _text, float x, float y, float scale, glm::vec3 color);

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
	void Init(const char* _path, int _fontSize, string _fontName, Font& _Font, ShaderProgram& _shader, string _text, float _x, float _y, float _scale, glm::vec3 _color);
	
	void SetString(string _text);
	
	/*@Brief: Gets our map<char, Character> from the Font class
	* 
	* 
	* 
	* 
	*/
	font GetFont(Font& font) { return font.GetFont(fontSize, fontName, path);}

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