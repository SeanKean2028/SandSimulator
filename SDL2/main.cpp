#include <iostream>
#include<SDL3/SDL.h>
#include<Gl/glew.h>

//Graphic Library Maths vectors, matrices, etc.
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//Importing Textures
#include <SOIL.h>

//Sean Made Headers
#include "Mesh.h"
#include "Shader.h"
#include "Grid.h"
#include "Circle.h"
int SetUpWindow(SDL_Window* window, SDL_GLContext glContext) {

    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        std::cerr << SDL_GetError() << std::endl;
    }
    std::cout << "Video drivers:\n";
    int n = SDL_GetNumVideoDrivers();
    for (int i = 0; i < n; i++)
        std::cout << "  " << SDL_GetVideoDriver(i) << "\n";

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL init failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    // ---- OpenGL 3.3 Core ----
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // SDL3 window creation (flags are last!)


    if (!window) {
        std::cerr << "Window creation failed: "
            << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }


    if (!glContext) {
        std::cerr << "GL context creation failed: "
            << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Make context current (important in SDL3)
    SDL_GL_MakeCurrent(window, glContext);

    // Init GLEW AFTER context creation
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW init failed\n";
        return -1;
    }

    std::cout << "OpenGL Version: "
        << glGetString(GL_VERSION) << std::endl;
}
float Box[] = {
    // bottom-left       Pos, Normals, TextureCoords
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    // bottom-right
     0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
     // top-right
      0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
      // top-left
      -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f
};
int BoxIndices[] = {
    0, 1, 2,
    2, 3, 0
};

int main(int argc, char* argv[])
{
    //--- SetUp Window
    int resX = 1000; int resY = 1000;
    SDL_Window* window;
    window = SDL_CreateWindow(
        "SDL3 + GLEW + OpenGL",
        resX, resY,
        SDL_WINDOW_OPENGL
    );
    // ---
    // 
    // Create GL context
    SDL_GLContext context = {};
    context = SDL_GL_CreateContext(window);

    int errorId = SetUpWindow(window, context);
    if (errorId == -1) {
        return -1;
    }
    // ---

    //--- Shaders
    VertexShader boxVertex("Shaders/boxVertex.glsl", GL_VERTEX_SHADER);
    FragmentShader boxFragment("Shaders/boxFragment.glsl", GL_FRAGMENT_SHADER);
    
    ShaderProgram boxShader (boxVertex, boxFragment);
    // ---
    
    //--- Meshes
    boxShader.use();
    glm::mat4 model = glm::mat4(1.0f);
    boxShader.setMat4("model", model);
    boxShader.setVec3("color", glm::vec3(1, 0, 0));
    Mesh boxMesh(Box, sizeof(Box), BoxIndices, sizeof(BoxIndices), boxShader);

    Grid grid(resX, resY, 100);
    grid.CreateCell(glm::vec2(2,1), sand);
    grid.CreateCell(glm::vec2(2, 2), sand);
    grid.CreateCell(glm::vec2(2, 3), sand);

    Circle circle(glm::vec2(0,0), 0.25f, 55);
    // ---
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }
        float x, y;
        SDL_GetMouseState(&x, &y);
        cout << "Mouse at: " << x << "," << y << endl;
        //Enable Blending for grid
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glViewport(0, 0, resX, resY);
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_CULL_FACE);
        grid.Draw();
        circle.DrawCircle(glm::vec2(x, y), glm::vec2(resX, resY));
        //boxMesh.DrawMesh();

        SDL_GL_SwapWindow(window);
    }

    boxShader.Delete();
    boxMesh.Delete();
    grid.Delete();

    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}