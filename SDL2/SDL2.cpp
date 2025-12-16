#include <iostream>
#include<SDL3/SDL.h>
#include<Gl/glew.h>

//Graphic Library Maths vectors, matrices, etc.
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//Importing Textures
#include <SOIL.h>
int main(int argc, char* argv[])
{
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
    SDL_Window* window = SDL_CreateWindow(
        "SDL3 + GLEW + OpenGL",
        800, 600,
        SDL_WINDOW_OPENGL
    );

    if (!window) {
        std::cerr << "Window creation failed: "
            << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Create GL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
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
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        glViewport(0, 0, 800, 600);
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(window);
    }
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}