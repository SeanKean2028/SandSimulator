#pragma once
#include <ft2build.h>
#include "Shader.h"
#include FT_FREETYPE_H
#include <GL/glew.h>
#include<map>
using namespace std;
struct Character {
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advanced;
};
struct font {
	string fontName;
	map<int, map<char, Character>> characterSizes;
};
class Font {
	vector<font*> fonts;
	const char* path;
public: 
	Font() {};
	/*@Brief: Initializes Freetype, and Fotn
	*
	* Initializes FreeType directory, and Font structure.
	* Creates a texture for each char from char 0 => 128.
	* Creates the Character struct for our map.
	* Sets our map of chars, and Charcters in text class.
	*
	* @Params: int _fontSize, string _fontName, const char *path
	* @Return: Sets map<char, Character> Characters in Text class
	*/
	void LoadFont(int _fontSize, string _fontName, const char* path);
	/*@Brief: Get's Font for collection of font struct
	* 
	* The font struct has map<fontsize<map<char, Character>>, and a string fontName as an identifier
	* If can't find calls loadFont
	* 
	* @Params: int fontSize, string fontName, const char* filepath
	* @Return: font struct
	*/
	font *GetFont(int fontSize, string fontName, const char* filePath);
};
