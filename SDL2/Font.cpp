#include "Font.h"
#include <map>

using namespace std;

void Font::LoadFont(int _fontSize, string _fontName, const char* path) {
	font* newFont = new font();
	newFont->fontName = _fontName;
	//Check if fontSize already loaded
	for (font* f : fonts)
	{
		if (f->fontName == _fontName &&
			f->characterSizes.find(_fontSize) != f->characterSizes.end())
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
		newFont->characterSizes[_fontSize].emplace(c, character);
	}
	fonts.push_back(newFont);
	//Clean Up
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	for (font* f : fonts) {
		cout << f->fontName << endl;
	}
	cout << "Font Loaded!!" << endl;
}
font *Font::GetFont(int fontSize, string fontName, const char* filePath) {
	cout << "Getting Font: " << filePath << endl;
	for (font* f : fonts) {
		cout << f->fontName << endl;
		if (f->fontName == fontName && f->characterSizes.find(fontSize) != f->characterSizes.end()){
			cout << "Font Loaded: 1" << endl;
			return f;
		}
	}
	LoadFont(fontSize, fontName, filePath);
	for (font* f : fonts) {
		if (f->fontName == fontName && f->characterSizes.find(fontSize) != f->characterSizes.end()) {
			cout << "Font Loaded: 1" << endl;
			return f;
		}
	}
	cout << "Font Loaded: 3" << endl;
	return nullptr;
}