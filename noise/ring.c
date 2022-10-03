#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>


struct point
{
    int x;
    int y;
    double r;
};
typedef struct point point_t;


point_t* makeRandomRing(const int CENTER_X, const int CENTER_Y,
        const double R_1, const double R_2, const int COUNT)
{
    point_t* array = SDL_malloc(COUNT * sizeof(point_t));

    for (int i = 0; i < COUNT; ++i)
    {
        double angle  = ((double)rand() / (double)RAND_MAX);
        double radius = (double)rand() / (double)RAND_MAX;

        // Angle ranges from 0 to 2pi.
        angle = angle * M_PI * 2;
        // Inverse transform to fit the appropriate pdf.
        radius = sqrt((R_2*R_2 - R_1*R_1)*radius + R_1*R_1);

        // Converting to cartesian coordinates.
        array[i].x = (int)(radius * cos(angle)) + CENTER_X;
        array[i].y = (int)(radius * sin(angle)) + CENTER_Y;
        array[i].r = radius;
    }

    return array;
}


int main() {
    const int SCREEN_HEIGHT = 600;
    const int SCREEN_WIDTH = 800;

    if (TEST)
    {
        printf("test works!\n");
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("%s\n", "Initialization failed.");
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Random Points in Ring",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
            SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        SDL_Quit();
        return 2;
    }

    // Create a renderer with hardware acceleration, 
    // also present according with the vertical sync refresh.
    SDL_Renderer* gRenderer = SDL_CreateRenderer(window, 0, 
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    const int COUNT = 50000;
    const double R_1 = 100;
    const double R_2 = 250;

    point_t* rand_arr = makeRandomRing(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, R_1, R_2, COUNT); 

    bool quit = false;
    SDL_Event event;

    while (!quit) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_RenderClear(gRenderer);

        for (int i = 0; i<COUNT; ++i)
        {
            // Fun colors!
            unsigned char r = (unsigned char)(((rand_arr[i].r - R_1)/R_2)*0xFF);
            unsigned char b = (unsigned char)(((R_2 - rand_arr[i].r)/(R_2-R_1))*0xFF);

            SDL_SetRenderDrawColor(gRenderer, r, 0x00, b, 0xFF);

            // Render using point.
            SDL_RenderDrawPoint(gRenderer, rand_arr[i].x, rand_arr[i].y);
        }

        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
        
        // Present everything drawn. 
        SDL_RenderPresent(gRenderer);
    }

    // Clean-up.
    SDL_free(rand_arr);

    SDL_DestroyWindow(window);
    // We have to destroy the renderer, same as with the window.
    SDL_DestroyRenderer(gRenderer);
    SDL_Quit();
}

