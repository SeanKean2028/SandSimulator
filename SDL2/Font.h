#pragma once
#include "Text.h"
#include<ft2build.h>
#include "Shader.h"
#include FT_FREETYPE_H
#include <GL/glew.h>
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
	vector<font> fonts;
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
	* @Params: Class data must be initialized
	* @Return: Sets map<char, Character> Characters in Text class
	*/
	void LoadFont(int _fontSize, string _fontName, const char* path) {
		font newFont = font();
		newFont.fontName = _fontName;
		//Check if fontSize already loaded
		for (font& f : fonts)
		{
			if (f.fontName == _fontName &&
				f.characterSizes.find(_fontSize) != f.characterSizes.end())
			{
				cout << "Font already loaded: " << path << " " << _fontSize << endl;
				return;
			}
		}
		cout << "Loading Font at: " << path << endl;
		//Initialize FreeType Library
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
			cout << "Error::Freetype: Could not init FreeType Library \n";
		//Initialize Font
		FT_Face face;
		if (FT_New_Face(ft, path, 0, &face))
			cout << "Error::Freetype: Failed to load font \n";
		//Width is dynamically set if width = 0 based on the height
		FT_Set_Pixel_Sizes(face, 0, _fontSize);
		//Initialize each Char
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		for (unsigned char c = 0; c < 128; c++) {
			//Load char glyph
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				cout << "Error::Freetype: Failed to load char: " << c << endl;
				continue;
			}
			//Generate Texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			//Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//Store the char
			Character character =
			{
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};
			newFont.characterSizes[_fontSize][c] = character;
		}
		fonts.push_back(newFont);
		//Clean Up
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}
	font GetFont(int fontSize, string fontName, const char* filePath)
	{
		for (font _font : fonts) {
			if(_font.fontName == fontName)
				return _font;
		}
		LoadFont(fontSize, fontName, filePath);
		for (font _font : fonts) {
			if (_font.fontName == fontName)
				return _font;
		}
	}
	map<char, Character> GetFontBySize(font _font, int _fontSize) {
	}
};
