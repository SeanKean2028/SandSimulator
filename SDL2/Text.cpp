#include "Text.h"
#include<ft2build.h>
#include "Shader.h"
#include FT_FREETYPE_H
#include <iostream>
#include <GL/glew.h>
using namespace std;
Text::Text(const char* _path, int _fontSize, string _fontName,Font &_Font, ShaderProgram &_shader, string _text, float x, float y, float scale, glm::vec3 color): shader(_shader) {
	Init(_path, _fontSize, _fontName, _Font, _shader, _text, x, y, scale, color);
}
void Text::Init(const char* _path, int _fontSize,string _fontName, Font& _Font, ShaderProgram &_shader, string _text, float _x, float _y, float _scale, glm::vec3 _color) {
	path = _path;
	fontSize = _fontSize;
	text = _text;
	x = _x;
	y = _y;
	fontName = _fontName;
	scale = _scale;
	color = _color;
	_font = _Font.GetFont(fontSize, fontName, path);
	if (_font == nullptr)
		cout << "Failed Getting Font \n";
	GenerateMesh();
}
void Text::SetString(string _text) {
	text = _text;
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
    if (!_font) return; // safety check

    shader.use();
    glUniform3f(
        glGetUniformLocation(shader.ID, "textColor"),
        color.x, color.y, color.z
    );

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    float xposCursor = x;
    float yposCursor = y;

    // get glyph map ONCE
    auto& sizeMap = _font->characterSizes.at(fontSize);

    for (char c : text) {
        unsigned char glyph = static_cast<unsigned char>(c);

        auto it = sizeMap.find(glyph);
        if (it == sizeMap.end())
            continue; // skip unsupported glyphs safely

        const Character& ch = it->second;

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
