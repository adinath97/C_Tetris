#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>

//additional header files
#include "Vector2.h"

struct shape {
    SDL_Color color;
    bool matrix[4][4];
    double x, y;
    int size;
};

bool Initialize();
void RunLoop();
void ShutDown();

void ProcessInput();
void UpdateGame();
void GenerateOutput();
void DisplayText();
void InitSDLText();
void StartingText();
void EndingText();
void ResetGame();

void Rotate();
void ReverseRotation();
void UpdateGrid();
void CheckForRows();
void RemoveFullRows(int i);

shape Transpose(shape input);
shape ReverseColumns(shape input);

class Game {
    public:
        Game():mWindow(nullptr), mIsRunning(true) {
            mTicksCount = 0;
        }

        ~Game() { }

        bool Initialize();
        void RunLoop();
        void ShutDown();

    private:
        //used to track state of snake
        enum State {
            EActive,
            EPaused,
            EDead
        };

        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;

        Uint32 mTicksCount;

        float thickness = 20.0f, deltaTime;
        
        bool mIsRunning, gameOver = false, startGame = false, up, down, left, right;

        float moveTimerCap = .5f, moveTimer = 0.0f;

        void ProcessInput();
        void UpdateGame();
        void GenerateOutput();
        void DisplayText();
        void InitSDLText();
        void StartingText();
        void EndingText();
        void ResetGame();

        void ReverseRotation();
        void Rotate();
        void UpdateGrid();
        void CheckForRows();
        void RemoveFullRows(int i);

        shape Transpose(shape input);
        shape ReverseColumns(shape input);

        shape active;

        int tetrisGrid[19][14] = {
            {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        };

        shape tetraminos[7] = {
            {
                {225,165,0}, //color
                {
                    {0,0,0,0},
                    {1,1,1,1},
                    {0,0,0,0},
                    {0,0,0,0},
                },
                4,
                -2,
                4,
            },
            {
                {225,165,0}, //color
                {
                    {0,0,0,0},
                    {1,1,0,0},
                    {0,1,1,0},
                    {0,0,0,0},
                },
                4,
                -2,
                4,
            },
            {
                {225,165,0}, //color
                {
                    {0,0,0,0},
                    {0,0,1,1},
                    {0,1,1,0},
                    {0,0,0,0},
                },
                4,
                -2,
                4,
            },
            {
                {225,165,0}, //color
                {
                    {0,0,0,0},
                    {0,1,0,0},
                    {1,1,1,0},
                    {0,0,0,0},
                },
                4,
                -2,
                4,
            },
            {
                {225,165,0}, //color
                {
                    {0,0,0,0},
                    {0,1,1,0},
                    {0,1,1,0},
                    {0,0,0,0},
                },
                4,
                -2,
                4,
            },
            {
                {225,165,0}, //color
                {
                    {0,0,0,0},
                    {1,0,0,0},
                    {1,1,1,0},
                    {0,0,0,0},
                },
                4,
                -2,
                4,
            },
            {
                {225,165,0}, //color
                {
                    {0,0,0,0},
                    {0,0,1,0},
                    {1,1,1,0},
                    {0,0,0,0},
                },
                4,
                -2,
                4,
            }
        };
};

shape Game::Transpose(shape input) {
    shape transpose = input;

    //generate the transpose
    for(int i = 0; i < input.size; i++) {
        for(int j = 0; j < input.size; j++) {
            transpose.matrix[i][j] = input.matrix[j][i];
        }
    }

    return transpose;
}

shape Game::ReverseColumns(shape input) {
    shape rev = input;

    //reverse rows
    for(int i = 0; i < input.size; i++) {
        for(int j = 0; j < input.size; j++) {
            bool val = input.matrix[i][j];
            rev.matrix[i][j] = input.matrix[i][input.size - 1 - j];
            rev.matrix[i][input.size - 1 - j] = val;
        }
    }

    return rev;
}

void Game::Rotate() {
    active = ReverseColumns(Transpose(active));
}

void Game::ReverseRotation() {
    active = Transpose(ReverseColumns(active));
}

bool Game::Initialize() {
    //random number generator seed
    srand (time(NULL));

    //get tetramino
    active = tetraminos[rand() % 7];

    //initialize SDL library
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);

    //Initialize text
    //InitSDLText();

    if(sdlResult != 0) {
        //initialization failed
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    //if initialization successful, create window
    mWindow = SDL_CreateWindow(
        "Tetris",
        100,
        100,
        550,
        800,
        0 // Flags
    );

    if(!mWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(
        mWindow,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC //initialization flags. use accelerated renderer and enable vsync
    );

    return true;
}

void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ShutDown() {
    //destroy SDL_Window
    SDL_DestroyWindow(mWindow);

    //destroy SDL_Renderer
    SDL_DestroyRenderer(mRenderer);

    //close SDL
    SDL_Quit();
}

void Game::ProcessInput() {
    up = right = down = left = false;

    SDL_Event event;

    //go through all events and respond as desired/appropriate
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                mIsRunning = false;
                break;
            case SDL_KEYDOWN:
                switch( event.key.keysym.sym ) {
                    case SDLK_UP:
                        up = true;
                        break;
                    case SDLK_RIGHT:
                        right = true;
                        break;
                    case SDLK_LEFT:
                        left = true;
                        break;
                    case SDLK_DOWN:
                        down = true;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }

    // Retrieve the state of all of the keys then scan each as desired
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_ESCAPE]){
        mIsRunning = false;
    }

}

void Game::UpdateGame() {
    bool hit = false;

    //update frame at fixed intervals (fixedDeltaTime)
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    //get deltaTime
    deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    mTicksCount = SDL_GetTicks();

    //clamp deltaTime
    if(deltaTime > .05f) {
        deltaTime = 0.05f;
    }

    if(up) { Rotate(); } 
    else if(right) { active.x++; } 
    else if(left) { active.x--; } 
    else if(down) { active.y++; }

    moveTimer += deltaTime;

    if(moveTimer >= moveTimerCap) {
        moveTimer = 0.0f;
        active.y++;
    }

    //bound movement
    //expected active tetramino position
    for(int i = 0; i < active.size; i++) {
        for(int j = 0; j < active.size; j++) {
            if(active.matrix[i][j]) {
                int proposedX = active.x + i;
                int proposedY = active.y + j;
                if(tetrisGrid[proposedY][proposedX] == 1) { hit = true; }
            }
        }
    }

    if(hit) {
        //reverse
        if(up) { ReverseRotation(); } 
        else if(right) { active.x--; } 
        else if(left) { active.x++; } 
        else if(down) { active.y--; }
        else {
            active.y--;
            //no room to go so...
            //update grid 
            UpdateGrid();
            //instantiate new active tetramino
            shape newTet = tetraminos[rand() % 7];
            active = newTet;
        }
    }

    CheckForRows();
}

void Game::CheckForRows() {
    int oneCount = 0;

    for(int i = 0; i < 18; i++) {
        oneCount = 0;
        for(int j = 1; j < 13; j++) {
            if(tetrisGrid[i][j]) {
                oneCount++;
                if(oneCount == 12) {
                    RemoveFullRows(i);
                }
            } else {
                continue;
            }
        }
    }
}

void Game::RemoveFullRows(int x) {
    for(int i = x; i > 0; i--) {
        for(int j = 1; j < 13; j++) {
            tetrisGrid[i][j] = tetrisGrid[i-1][j];
        }
    }
}

void Game::UpdateGrid() {
    for(int i = 0; i < active.size; i++) {
        for(int j = 0; j < active.size; j++) {
            if(active.matrix[i][j]) {
                int x = active.x + i;
                int y = active.y + j;
                tetrisGrid[y][x] = 1;
            }
        }
    }
}

void Game::GenerateOutput() {

    SDL_SetRenderDrawColor(mRenderer,0,0,0,0);

    SDL_RenderClear(mRenderer); //clear back buffer to current draw color

    SDL_SetRenderDrawColor(mRenderer,255,255,255,255);

    SDL_Rect topWall = {0,0,800,thickness};
    SDL_Rect bottomWall = {0,800 - thickness * 4,800,thickness * 4};
    SDL_Rect leftWall = {0,0,thickness * 1.8,1204};
    SDL_Rect rightWall = {520,0,thickness * 1.8,1204};

    //draw grid
    for(int i = 0; i < 19; i++) {
        for(int j = 0; j < 14; j++) {
            if(tetrisGrid[i][j]) {
                SDL_Rect block = {(j) * 40,(i) * 40,36,36};
                SDL_RenderFillRect(mRenderer,&block);
            }
        }
    }

    //draw active tetramino
    for(int i = 0; i < active.size; i++) {
        for(int j = 0; j < active.size; j++) {
            if(active.matrix[i][j]) {
                SDL_Rect block = {(active.x + i) * 40,(active.y + j) * 40,36,36};
                SDL_RenderFillRect(mRenderer,&block);
            }
        }
    }

    SDL_RenderFillRect(mRenderer,&topWall);
    SDL_RenderFillRect(mRenderer,&rightWall);
    SDL_RenderFillRect(mRenderer,&leftWall);
    SDL_RenderFillRect(mRenderer,&bottomWall);

    SDL_SetRenderDrawColor(mRenderer,255,0,0,255);

    SDL_RenderPresent(mRenderer); //swap front and back buffers

    //DisplayText();
} 


/* 
void Game::InitSDLText() {
    //initialize, else load error
    if(TTF_Init() == -1) {
        std::cout << "Failed to initialize text.Error: " << TTF_GetError() << std::endl;
    }
}

void Game::ResetGame() {

}

void Game::StartingText() {
    SDL_SetRenderDrawColor( //pointer, RGBA
            mRenderer,
            0,
            0,
            0,
            0
        );

        SDL_RenderClear(mRenderer);

        //draw game scene
        SDL_SetRenderDrawColor( //pointer, RGBA
            mRenderer,
            255,
            255,
            255,
            255
        );

        TTF_Font* ourFont;
        SDL_Surface * titleSurfaceText;	
        SDL_Surface * optionSurfaceText;
        SDL_Texture * titleTexture;
        SDL_Texture * optionTexture;

        SDL_Rect titleText = {340,100,300,200};
        SDL_Rect optionText = {250,400,500,100};

        //load font file and set size
        ourFont = TTF_OpenFont("AldotheApache.ttf",1000);

        //confirm font loaded
        if(ourFont == nullptr) {
            std::cout << "Could not load font" << std::endl;
            exit(1);
        }

        SDL_Color textColor = {255,255,255,255};

        //pixels from text
        titleSurfaceText = TTF_RenderText_Solid(ourFont,"JUMP!",textColor);
        optionSurfaceText = TTF_RenderText_Solid(ourFont,"HIT SPACEBAR TO PLAY",textColor);

        //set up texture
        titleTexture = SDL_CreateTextureFromSurface(mRenderer,titleSurfaceText);
        optionTexture = SDL_CreateTextureFromSurface(mRenderer,optionSurfaceText);

        SDL_FreeSurface(titleSurfaceText);
        SDL_FreeSurface(optionSurfaceText);

        SDL_RenderCopy(mRenderer,titleTexture,NULL,&titleText);
        SDL_RenderCopy(mRenderer,optionTexture,NULL,&optionText);

        //destroy texture
        SDL_DestroyTexture(titleTexture);
        SDL_DestroyTexture(optionTexture);

        //close font
        TTF_CloseFont(ourFont);

        SDL_RenderPresent(mRenderer); //swap front and back buffers
}

void Game::EndingText() {
        SDL_SetRenderDrawColor( //pointer, RGBA
            mRenderer,
            0,
            0,
            0,
            0
        );

        SDL_RenderClear(mRenderer);

        //draw game scene
        SDL_SetRenderDrawColor( //pointer, RGBA
            mRenderer,
            255,
            255,
            255,
            255
        );
        TTF_Font* ourFont;
        SDL_Surface * titleSurfaceText;	
        SDL_Surface * optionSurfaceText;
        SDL_Texture * titleTexture;
        SDL_Texture * optionTexture;

        SDL_Rect titleText = {340,100,300,200};
        SDL_Rect optionText = {250,400,500,100};

        //load font file and set size
        ourFont = TTF_OpenFont("AldotheApache.ttf",1000);

        //confirm font loaded
        if(ourFont == nullptr) {
            std::cout << "Could not load font" << std::endl;
            exit(1);
        }

        SDL_Color textColor = {255,255,255,255};

        //pixels from text
        titleSurfaceText = TTF_RenderText_Solid(ourFont,"JUMP!",textColor);
        optionSurfaceText = TTF_RenderText_Solid(ourFont,"EVERYONE WINS! A FOR EFFORT",textColor);

        //set up texture
        titleTexture = SDL_CreateTextureFromSurface(mRenderer,titleSurfaceText);
        optionTexture = SDL_CreateTextureFromSurface(mRenderer,optionSurfaceText);

        SDL_FreeSurface(titleSurfaceText);
        SDL_FreeSurface(optionSurfaceText);

        SDL_RenderCopy(mRenderer,titleTexture,NULL,&titleText);
        SDL_RenderCopy(mRenderer,optionTexture,NULL,&optionText);

        //destroy texture
        SDL_DestroyTexture(titleTexture);
        SDL_DestroyTexture(optionTexture);

        //close font
        TTF_CloseFont(ourFont);

        SDL_RenderPresent(mRenderer); //swap front and back buffers
        
}

void Game::DisplayText() {
    //add text
    TTF_Font* ourFont;
    SDL_Surface * playerSurfaceText;	
    SDL_Surface * AISurfaceText;
    SDL_Texture * playerTexture;
    SDL_Texture * AITexture;

    SDL_Rect playerScoreText = {950,25,50,50};

    //load font file and set size
    ourFont = TTF_OpenFont("AldotheApache.ttf",1000);

    //confirm font loaded
    if(ourFont == nullptr) {
        std::cout << "Could not load font" << std::endl;
        exit(1);
    }

    SDL_Color textColor = {255,255,255,255};

    playerScoreStr = std::to_string(playerScore);
    char const *pchar = playerScoreStr.c_str();  //use char const* as target type

    //pixels from text
    playerSurfaceText = TTF_RenderText_Solid(ourFont,pchar,textColor);

    //set up texture
    playerTexture = SDL_CreateTextureFromSurface(mRenderer,playerSurfaceText);

    SDL_FreeSurface(playerSurfaceText);

    SDL_RenderCopy(mRenderer,playerTexture,NULL,&playerScoreText);

    //destroy texture
    SDL_DestroyTexture(playerTexture);

    //close font
    TTF_CloseFont(ourFont);

    SDL_RenderPresent(mRenderer); //swap front and back buffers
}
 */