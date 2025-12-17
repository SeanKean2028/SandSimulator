#version 330 core
in vec2 v_uv;
out vec4 FragColor;
uniform vec4 params; // x=scale, y=resolution
uniform vec4 color;
float grid(vec2 st, float res)
{
    vec2 g = fract(st * res);
    float lineWidth = 0.05;

    float xLine1 = step(1.0 - lineWidth, g.x);
    float yLine1 = step(1.0 - lineWidth, g.y);

    float xLine2 = step(g.x, lineWidth);
    float yLine2 = step(g.y, lineWidth);

    return max(max(xLine1, xLine2), max(yLine1, yLine2));
}

void main()
{
    vec2 grid_uv = v_uv * params.x;
    float x = grid(grid_uv, params.y);
    FragColor = vec4(color.xyz* x, color.w);
}