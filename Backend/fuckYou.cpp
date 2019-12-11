#include "GameWindow.h"
#include <SDL.h>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <ctime>
//#include <SDL_image.h>
//#include "Collidable.h"
using namespace std;

GameWindow::GameWindow(int screenWidth, int screenHeight){
    SDL_Init(SDL_INIT_VIDEO);
    m_gameWindow = SDL_CreateWindow("a window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000,SDL_WINDOW_OPENGL);
}

GameWindow::~GameWindow(){
    SDL_FreeSurface(this->GetWinSurface());
    m_gameWindow = NULL;
    SDL_DestroyWindow(m_gameWindow);
    m_gameWindow = NULL;
    SDL_Quit();
}
void GameWindow::RunGame(){
    bool quit = false;
    SDL_Event e;
    while(!quit){
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                quit = true;
            }
        }
    }
}

SDL_Surface* GameWindow::GetWinSurface(){
    return SDL_GetWindowSurface(m_gameWindow);
}

SDL_Window* GameWindow::GetWindow(){
    return m_gameWindow;
}

void GameWindow::BlitToWindow(SDL_Surface* sourceSurface, const SDL_Rect* srcRect, SDL_Surface* destSurface, SDL_Rect* destRect){
    SDL_BlitSurface(sourceSurface, srcRect, destSurface, destRect);
}

int main(int argc, char* argv[]){
    //double rate = atof(argv[1]);
    printf("Test");
    GameWindow window = GameWindow(1000,1000);
    SDL_Surface* windowSurface = SDL_GetWindowSurface(window.GetWindow());
    SDL_Surface* sourceSurface = SDL_CreateRGBSurface(0, 1000, 1000, 32,0,0,0,0);

    SDL_Surface* img = SDL_LoadBMP("TestImage.bmp");
    SDL_BlitSurface(img, NULL, windowSurface, NULL);
    SDL_UpdateWindowSurface(window.GetWindow());
    SDL_Delay(10000);
    /*SDL_Renderer* renderer = SDL_CreateRenderer(window.GetWindow(), -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, img);
    SDL_RenderCopyEx(renderer, tex, NULL, NULL, 0,NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(renderer);   */
    /*
    SDL_FreeSurface(img);
    SDL_Event e;
    SDL_Point* p;
    p->x = 500;
    p->y = 500;
    float angle = 0;
    bool quit = false;
    int increment = .35;
    printf("pre\n");
    float rate = atof(argv[1]);
    printf("post\n");
    while(!quit){
        if (SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT) quit = true;
        }
        
        SDL_RenderClear(renderer);
        SDL_RenderCopyEx(renderer, tex, NULL, NULL, angle,p, SDL_FLIP_NONE);
        angle+=rate;
        SDL_RenderPresent(renderer);
    }

*/



   /* SDL_FillRect(sourceSurface, NULL, SDL_MapRGB(sourceSurface->format,255,0,0));
    SDL_Rect sourceRect;
    sourceRect.x = 0;
    sourceRect.y = 0;
    sourceRect.h = 100;
    sourceRect.w = 200;
    SDL_Rect destRect;
    destRect.x = 0;
    destRect.y = 0;
    destRect.h = 100;
    destRect.w = 200;
    for(int i = 0; i < 100; i++){
        SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format,0,0,0));
        destRect.y = destRect.y + 5;
        SDL_BlitSurface(sourceSurface, &sourceRect, windowSurface, &destRect);
        SDL_UpdateWindowSurface(window.GetWindow());
        SDL_Delay(6);
    }
    SDL_UpdateWindowSurface(window.GetWindow());
    */
    //window.RunGame();

   /* Collidable A(CreateRect(1,1,2,2));
    Collidable B(CreateRect(2,2,2,2));
    if(A.CheckOverlap(B.GetBBox())){
        printf("True");
    }
*/
    return 0;
}
