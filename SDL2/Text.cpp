#include "Text.h"
#include<ft2build.h>
#include "Shader.h"
#include FT_FREETYPE_H
#include <iostream>
#include <GL/glew.h>
using namespace std;
Text::Text(const char* _path, int _fontSize, ShaderProgram &_shader, string _text, float x, float y, float scale, glm::vec3 color): shader(_shader) {
	shader = _shader;
	Init(_path, _fontSize, _shader, _text, x, y, scale, color);
}
void Text::Init(const char* _path, int _fontSize, ShaderProgram &_shader, string _text, float _x, float _y, float _scale, glm::vec3 _color) {
	path = _path;
	fontSize = _fontSize;
	text = _text;
	x = _x;
	y = _y;
	scale = _scale;
	color = _color;
	LoadFont();
	GenerateMesh();
}
void Text::SetString(string _text) {
	text = _text;
}
void Text::LoadFont() {
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
	FT_Set_Pixel_Sizes(face, 0, fontSize);
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
		Characters.insert(pair<char, Character>(c, character));
	}
	//Clean Up
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}
void Text::GenerateMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Text::RenderText() {
	shader.use();
	glUniform3f(glGetUniformLocation(shader.ID, "textColor"),
		color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// iterate through all characters
	string::const_iterator c;
	float xposCursor = x;
	float yposCursor = y;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = Characters[*c];

		float xpos = xposCursor + ch.Bearing.x * scale;
		float ypos = yposCursor - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		float vertices[6][4] = {
			{xpos,     ypos + h, 0.0f, 0.0f},
			{xpos,     ypos,     0.0f, 1.0f},
			{xpos + w, ypos,     1.0f, 1.0f},

			{xpos,     ypos + h, 0.0f, 0.0f},
			{xpos + w, ypos,     1.0f, 1.0f},
			{xpos + w, ypos + h, 1.0f, 0.0f}
		};

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		xposCursor += (ch.Advanced >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}