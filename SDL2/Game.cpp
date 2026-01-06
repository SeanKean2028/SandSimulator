#include <iostream>
#include <GL/glew.h>
#include <SDL3/SDL.h>

#include "Game.h"
#include "InputHandler.h"
#include "Grid.h"
#include "Circle.h"

using namespace std;

int SetUpWindow(SDL_Window* window, SDL_GLContext* glContext){
    if (!window) {
        cerr << "Window creation failed: " << SDL_GetError() << endl;
        return -1;
    }

    if (*glContext == nullptr) {
        cerr << "GL context creation failed: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        return -1;
    }

    SDL_GL_MakeCurrent(window, *glContext);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        cerr << "GLEW init failed: "
            << glewGetErrorString(err) << endl;
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
    return 0;
}

bool Game::init(GameInitArgs initArgs){
    std::cout << "Initilizing Game \n";
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL init failed: " << SDL_GetError() << endl;
        return false;
    }

    gameArgs = initArgs;

    /* ---- SET ATTRIBUTES BEFORE WINDOW CREATION ---- */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_pWindow = SDL_CreateWindow(
        initArgs.title,
        initArgs.width,
        initArgs.height,
        initArgs.flags | SDL_WINDOW_OPENGL
    );

    m_pContext = SDL_GL_CreateContext(m_pWindow);
    std::cout << "Window Created \n";
    if (SetUpWindow(m_pWindow, &m_pContext) != 0)
        return false;

    m_bRunning = true;
    float rad = 0.1f, steps = 100;
    circleInitArgs _circleInitArgs{
        rad,
        steps
    };
    m_MouseCircle.init(_circleInitArgs);
    int cellAmount = 100;
    GridInitArgs gridInitArgs{
        gameArgs.width, gameArgs.height, cellAmount
    };
    m_Grid.init(gridInitArgs);

    m_Grid.CreateCell(glm::vec2(2, 1), sand);
    m_Grid.CreateCell(glm::vec2(2, 2), sand);
    m_Grid.CreateCell(glm::vec2(2, 3), sand);
    std::cout << "Creating Cells \n";
    return true;
}
void Game::handleEvents(){
    InputHandler::Instance()->update();
}
void Game::render(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, gameArgs.width, gameArgs.height);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_CULL_FACE);

    float x, y;
    SDL_GetMouseState(&x, &y);

    m_Grid.Draw(m_deltaTime);
    m_MouseCircle.DrawCircle(
        glm::vec2((float)x, (float)y),
        glm::vec2(gameArgs.width, gameArgs.height)
    );

    SDL_GL_SwapWindow(m_pWindow);
}

void Game::update(){
    while (m_bRunning) {
        if (m_bRunning == false)
            return;
        Uint64 now = SDL_GetPerformanceCounter();
        float deltaTime =
            (now - m_lastCounter) /
            (float)SDL_GetPerformanceFrequency();

        m_lastCounter = now;
        m_deltaTime = deltaTime;

        handleEvents();
        render();
    }
    clean();
}
void Game::clean(){
    m_bRunning = false;
    m_Grid.Delete();
    m_MouseCircle.Delete();
    
    SDL_GL_DestroyContext(m_pContext);
    SDL_DestroyWindow(m_pWindow);
    SDL_Quit();
}
