#include "gui.h"
#include "board.h"
#include "solution.h"
#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_ttf.h>

const int SCREEN_WIDTH  = 600;
const int SCREEN_HEIGHT = 600;
const int N             =   3;
const std::string WINDOW_TITLE = "Sliding Puzzle";

TTF_Font* font;
SDL_Window *window;
SDL_Texture *texture;
SDL_Renderer *renderer;
int start;

void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

SDL_Rect grid(int p)
{
    int y = (p / N) * (SCREEN_HEIGHT / N), x = (p % N) * (SCREEN_WIDTH / N);
    return SDL_Rect{x, y, SCREEN_WIDTH / N, SCREEN_HEIGHT / N};
}

void texting();
void gui(board B)
{
    SDL_RenderClear(renderer);
    //texting();
    SDL_Rect sources, destination;
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++){
            if (B.e[i][j] == 1) continue;
            sources = grid(B.e[i][j] - 1);
            destination = grid(i * N + j);
            SDL_RenderCopy(renderer, texture, &sources, &destination);
        }
    SDL_RenderPresent(renderer);
}
void gui_move(board A, board B, SDL_Rect moving)
{

    SDL_RenderClear(renderer);
//    texting();
    SDL_Rect sources, destination;
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++){
            sources = grid(A.e[i][j] - 1);
            destination = grid(i * N + j);
            if (A.e[i][j] == 1) continue;
            if (B.e[i][j] == 1) {
                destination = moving;
            };
            SDL_RenderCopy(renderer, texture, &sources, &destination);
        }
    SDL_RenderPresent(renderer);
}
void moveBoard(board &A, board B)
{
    int step = 20, delay = 4;
    SDL_Rect sour = grid(B.pivot), dest = grid(A.pivot);

    while(abs(sour.x - dest.x) >= step){
        int delta = (sour.x - dest.x)/ abs(sour.x - dest.x);
        sour.x-= delta * step;
        gui_move(A, B, sour);
        SDL_Delay(delay);
    }


    while(abs(sour.y - dest.y) >= step){
        int delta = (sour.y - dest.y)/ abs(sour.y - dest.y);
        sour.y-= delta * step;
        gui_move(A, B, sour);
        SDL_Delay(delay);
    }

    A = B;
}
void instruct();
void score_board();
void update_score();
void game(int level)
{
    Bfs();
    instruct();

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(4000);
    start = SDL_GetTicks();
    board B = board(rand(level));
    gui(B);
    SDL_Event event;

    bool sol = 0;

    while(SDL_WaitEvent(&event)){
        switch (event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_UP:
                        moveBoard(B, B.moveDown());
                        break;
                    case SDLK_DOWN:
                        moveBoard(B, B.moveUp());
                        break;
                    case SDLK_LEFT:
                        moveBoard(B, B.moveRigh());
                        break;
                    case SDLK_RIGHT:
                        moveBoard(B, B.moveLeft());
                        break;
                    case SDLK_s:{
                        while(B.getID() != 123456789){
                            int direct = trace(B.getID());
                            if (direct == 1) moveBoard(B, B.moveDown());
                            if (direct == 2) moveBoard(B, B.moveUp());
                            if (direct == 3) moveBoard(B, B.moveRigh());
                            if (direct == 4) moveBoard(B, B.moveLeft());
                            SDL_Delay(100);
                        }
                        SDL_Delay(3500);
                        sol = 1;
                        break;
                    }

                    case SDLK_r:{
                        B = board(rand(level));
                        gui(B);
                        start = SDL_GetTicks();
                        break;
                    }
                    case SDLK_ESCAPE:
                        return;
                        break;
                    case SDLK_b:
                        score_board();
                        gui(B);
                        break;
                }
            break;
        }
        if (B.getID() == 123456789){
                std::cerr << "GAME OVER!!!\n";
                if (!sol)
                    update_score();
                return;
        }
    }
}
void puts(const char *t, const SDL_Color &color, SDL_Rect *pos)
{
    SDL_Surface *message = TTF_RenderText_Solid(font, t, color);
	pos->w = message->w;
	pos->h = message->h;
	SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, message);
	SDL_RenderCopy(renderer, text, NULL, pos);
	SDL_DestroyTexture(text);
	SDL_FreeSurface(message);
}
void score_board()
{
    SDL_Event event;

    while(SDL_WaitEvent(&event))
        switch (event.type){
            case SDL_KEYUP:
                switch(event.key.keysym.sym){
                    case SDLK_b:
                    {

                        return;
                        break;
                    }
                }
            default :
            {
                SDL_RenderClear(renderer);
                int time = SDL_GetTicks() - start;
                int s = time/1000, m = s/60;
                s = s % 60;
                SDL_Color color = {0, 200, 120, 255};
                SDL_Rect pos = {180, 100, 0, 0};
                std::string t = "Current time:  " + std::to_string(m) + " min " + std::to_string(s) + " sec.";
                puts(t.c_str(), color, &pos);

                color = {50, 250, 0, 255};
                pos = {200, 175, 0, 0};
                t = "*SCOREBOARD*";
                puts(t.c_str(), color, &pos);

                color = {250, 180, 0, 255};

                std::ifstream sfile ( "score.txt" );
                for(int i = 1; i <= 3; i++){
                    sfile >> m >> s;
                    pos.x = 185;
                    pos.y+= 50;
                    t = "TOP  " + std::to_string (i) + " :   " + std::to_string(m) + " min " + std::to_string(s) + " sec.";
                    puts(t.c_str(), color, &pos);
                }
                sfile.close();
                SDL_RenderPresent(renderer);
                break;
            }
        }
}

void update_score()
{
    int time = SDL_GetTicks() - start;
    int s = time/1000, m = s/60;
    std::vector< std::pair< int, int > > v;

    std::ifstream sfile ( "score.txt" );
    for(int i = 0;i < 3; i++){
        int cur_m, cur_s;
        sfile >> cur_m >> cur_s;
        v.push_back(std::make_pair(cur_m, cur_s));
    }
    sfile.close();
    std::ofstream _sfile ( "score.txt" );
    v.push_back(std::make_pair(m, s));
    sort(v.begin(), v.end());
    for(int i = 0;i < 3; i++)
        _sfile << v[i].first <<' '<< v[i].second <<'\n';
    _sfile.close();
}
void instruct()
{
    SDL_Event event;

    while(SDL_WaitEvent(&event)){
        SDL_RenderClear(renderer);
        std::string t ;
        SDL_Color color = {0, 125, 125, 255};
        SDL_Rect pos = {195, 100, 0, 0};
        t = "->INSTRUCTION<-";
        puts(t.c_str(), color, &pos);
        pos = {180, 150, 0, 0};
        t = "~[S] to get solution!";
        puts(t.c_str(), color, &pos);
        pos = {180, 200, 0, 0};
        t = "~[R] to reset!";
        puts(t.c_str(), color, &pos);
        pos = {180, 250, 0, 0};
        t = "~[B] to see score!";
        puts(t.c_str(), color, &pos);
        pos = {180, 300, 0, 0};
        t = "~[ESC] to quit!";
        puts(t.c_str(), color, &pos);
        color = {0, 250, 125, 255};
        pos = {155, 450, 0, 0};
        t = "Press [SPACE] to continue...";
        puts(t.c_str(), color, &pos);

        SDL_SetRenderDrawColor(renderer, 250, 25, 50, 100);
        pos = {170, 80, 240, 280};
        SDL_RenderDrawRect(renderer,&pos);
        SDL_RenderPresent(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        switch (event.type)
        {

            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_SPACE: return; break;
                    case SDLK_ESCAPE: exit(0); break;
                }
            break;
        }
    }
}
int texting_leveling()
{
    SDL_Event event;
    int x = 0;
    std::string t = "CHOOSE LEVEL FROM [1] TO [5]";
    SDL_Color color = {255, 165, 0, 255};
    SDL_Rect pos = {130, 200, 0, 0};

    SDL_RenderClear(renderer);
    puts(t.c_str(), color, &pos);

    SDL_SetRenderDrawColor(renderer, 200, 50, 50, 100);
    pos = {120, 190, 385, 53};
    SDL_RenderDrawRect(renderer,&pos);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);

    while(SDL_WaitEvent(&event)){
        switch (event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_1: x = 1; break;
                    case SDLK_2: x = 2; break;
                    case SDLK_3: x = 3; break;
                    case SDLK_4: x = 4; break;
                    case SDLK_5: x = 5; break;
                }
            break;
        };
        if (x) break;
    }
    pos = {143, 200, 0, 0};
    t = "";
    t += "OKAY , LEVEL ";
    t += char(x + '0');
    t += " IS LOADING...";

    SDL_RenderClear(renderer);
    puts(t.c_str(), color, &pos);
    SDL_RenderPresent(renderer);
    return x;
}

int gui_main()
{
    srand(time(0));

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,  SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    int seed = rand()  * rand()% 4 + 1;
    SDL_Surface *image = SDL_LoadBMP(std::string(std::string("picture") + char(seed + '0') + ".bmp").c_str());
    texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);


    if(TTF_Init() == -1) logSDLError(std::cout, "Init TTF", true);

    font = TTF_OpenFont("font.ttf", 24);

    //SDL_SetWindowSize(window, SCREEN_WIDTH + 150, SCREEN_HEIGHT);

    int level = texting_leveling();
    //std::cin >> level;
    std::cerr << "LOADING... \n";


    game(level);

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
	TTF_Quit();
    SDL_Quit();

    return 0;
}
