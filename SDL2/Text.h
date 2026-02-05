#pragma once
#include <glm.hpp>
#include <map>
#include <string>
#include "Font.h"
#include "Shader.h"
using namespace std;
class Text {
private:
	map<char, Character> Characters;
	unsigned int VAO, VBO;
	const char* path;
	int fontSize;
	ShaderProgram &shader;
	string text;
	string fontName;
	font *_font;
	float x;
	float y;
	float scale;
	glm::vec3 color;
public:

	/*@Brief: Initializes Text class calling all initalize funcs
	*
	* Generates and calls functions that generate all textures, 
	* VAO, VBO, and attribute data.
	* 
	* Parameters: const char* _path to Font file, int fontsize for setting size of, string &_fontName for finding font in Font class, Font& _font fontClass, ShaderProgram& shader,
	* string text actual text drawn, float x, float y for positioning, float scale, glm::vec3 color
	* generated bitmap
	* 
	* Returns: Set's all parameters in Text class
	*/
	Text(const char* _path, int _fontSize, string _fontName, Font& _Font, ShaderProgram& _shader, string _text, float x, float y, float scale, glm::vec3 color);

	/*@Brief: Set's all variables in class, Gets font from inpt
	*
	* Gets Font from Font class
	* Generates and calls functions that generate all textures,
	* VAO, VBO, and attribute data.
	*
	* Parameters: const char* _path to Font file, int fontsize for setting size of, string &_fontName for finding font in Font class, Font& _font fontClass, ShaderProgram& shader,
	* string text actual text drawn, float x, float y for positioning, float scale, glm::vec3 color
	* generated bitmap
	*
	* Returns: Set's all parameters in Text class
	*/
	void Init(const char* _path, int _fontSize, string _fontName, Font& _Font, ShaderProgram& _shader, string _text, float _x, float _y, float _scale, glm::vec3 _color);
	
	void SetString(string _text);
	
	/*@Brief: Set's drawn text
	* 
	* Paramaters: string _text 
	* Returns: draws new text
	*/
	font GetFont(Font& font) { return *font.GetFont(fontSize, fontName, path);}

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
	void setText(string _text) { text = _text; }
};