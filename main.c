#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <time.h>

#define ROWS 60
#define COLS 80

#define DEAD 0
#define ALIVE 1

#define RECT_SIZE 10

#define SCREEN_WIDTH COLS*RECT_SIZE
#define SCREEN_HEIGHT ROWS*RECT_SIZE
#define RENDER_DELAY 150

SDL_Rect rects[ROWS*COLS];

int table[ROWS][COLS];


int cell_neighbour_count(int row, int col) {
    int sum = 0, t_r = row-1, t_c = col-1;
    for(int r = 0; r <= 2; ++r) {
        if(t_r + r < 0) continue;
        for(int c = 0; c <= 2; ++c) {
            if(t_c + c < 0) continue;
            if(t_r+r == row && t_c+c == col) continue;
            sum += table[t_r+r][t_c+c];
        }
    }
    return sum;
}


void update_table() {
    int tmp_table[ROWS][COLS] = {};
    for(int r = 0;  r < ROWS; ++r) {
        for(int c = 0; c < COLS; ++c) {
            bool is_cell_dead = table[r][c] == DEAD;
            int neighbours = cell_neighbour_count(r,c);
            if(is_cell_dead && neighbours == 3) {
                tmp_table[r][c] = ALIVE;
            }else if(!is_cell_dead && (neighbours == 2 || neighbours == 3)){
                tmp_table[r][c] = ALIVE;
            }else{
                tmp_table[r][c] = DEAD;
            }

        }
    }

    //make tmp_table the current table
    for(int r = 0; r < ROWS; ++r) {
        for(int c = 0; c < COLS; ++c) {
            table[r][c] = tmp_table[r][c];
        }
    }
}

void generate_table() {
    srand(time(NULL));
    for(int r = 0; r < ROWS; ++r) {
        for(int c = 0; c < COLS; c++) {
            table[r][c] = rand() % 2;
        }
    }

}

int main(void) {
    generate_table();
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window *window = SDL_CreateWindow("John Conways Game of Life",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if(!window)
    {
        printf("Window could not be created!\n"
               "SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer)
    {
        printf("Renderer could not be created!\n"
               "SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    while (true) {
        int alive=0;

        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        for(int r = 0; r < ROWS; ++r) {
            for(int c = 0; c < COLS; ++c) {
                if(table[r][c] == ALIVE) {
                    rects[alive].x = c * RECT_SIZE;
                    rects[alive].y = r * RECT_SIZE;
                    rects[alive].w = RECT_SIZE;
                    rects[alive].h = RECT_SIZE;
                    alive++;
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        SDL_RenderFillRects(renderer, rects, alive);
        SDL_RenderPresent(renderer);
        update_table();
        SDL_Delay(RENDER_DELAY);
    }
    SDL_RenderPresent(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
