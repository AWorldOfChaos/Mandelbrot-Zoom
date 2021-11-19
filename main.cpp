#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Complex.hpp>
#include <qtree.hpp>
#include <iostream>
#include <string>
// #include <quadmath.h>

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 400
#define SIZE 100
// double x_centre = -exp(1)/7;   //-e/7-e/20i, the point to zoom in on
// double y_centre = exp(1)/20;

double x_centre = -0.775683;
double y_centre = 0.136467;

long double UPPER_x = 2;
long double LOWER_x = -2;
long double UPPER_y = 2;
long double LOWER_y = -2;

long MAX_ITERATION = 200;

using namespace std;

long double map(long double x, long double in_min, long double in_max, long double out_min, long double out_max)
{
    /**
    This function takes in value(PARAM 1) between values(PARAM 2 and 3)
    and returns a value between 2 values(PARAM 4 and 5)
    */

   long double conv1 = x-in_min;
   long double diff1 = out_max-out_min;
   long double diff2 = in_max-in_min;
    return 1.0 * conv1 * diff1 / diff2 + out_min;
}

bool isMandelbrot(Computation::Complex *c)
{
    long double x = c->getreal();
    long double y = c->getimg();
    int i;
    double Re;
    double Im;
    double a;
    double b;
    double dist;
    double finaldist;
    bool check;

    i = 0;
    Re = 0;
    Im = 0;
    a = 0;
    b = 0;

    while (i < MAX_ITERATION)
    {

        Re = ((a * a) - (b * b)) + x;
        Im = (2 * a * b) + y;
        if (pow(Re, 2) + pow(Im, 2) > 4)
            return false;

        a = Re;
        b = Im;

        i++;
    }

    finaldist = pow(Re, 2) + pow(Im, 2);

    if (finaldist > 4)
    {
        check = false;
    }
    else
    {
        check = true;
    }

    return check;
}

long iterations(Computation::Complex *c)
{
    long double x = c->getreal();
    long double y = c->getimg();
    int i;
    double Re;
    double Im;
    double a;
    double b;
    double dist;
    double finaldist;
    bool check;

    i = 0;
    Re = 0;
    Im = 0;
    a = 0;
    b = 0;

    while (i < MAX_ITERATION)
    {

        Re = ((a * a) - (b * b)) + x;
        Im = (2 * a * b) + y;
        if (pow(Re, 2) + pow(Im, 2) > 4)
            return i;

        a = Re;
        b = Im;

        i++;
    }

    return MAX_ITERATION;
}

class Zoom
{
public:
    Zoom()
    {
        // default constructor
    }

    int play_zoom(SDL_Window *window, SDL_Renderer *renderer)
    {

        float count = 0.1;
        float scale = 0.9;
        int counter = 0;
        bool running = true;
        while (running)
        {
            // SDL_RenderClear(renderer);
            SDL_Event event;

            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    running = false;
                    break;

                default:
                    break;
                }
            }

            for (int x = 0; x < SCREEN_HEIGHT; x++)
            {
                for (int y = 0; y < SCREEN_WIDTH; y++)
                {
                    long double a = map(x, 0, SCREEN_HEIGHT, LOWER_x, UPPER_x);
                    long double b = map(y, 0, SCREEN_WIDTH, LOWER_y, UPPER_y);

                    Computation::Complex *c = new Computation::Complex(a, b);
                    if (isMandelbrot(c))
                    {
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderDrawPoint(renderer, x, y);
                    }
                    else
                    {
                        int bright = map(iterations(c), 0, MAX_ITERATION, 0, 255);
                        int red = map(bright * bright, 0, 65025, 0, 255);
                        int green = bright;
                        int blue = map(sqrt(bright), 0, 16, 0, 255);
                        SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
                        SDL_RenderDrawPoint(renderer, x, y);
                    }
                }
            }

            // SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
            // SDL_RenderClear(renderer);

            SDL_RenderPresent(renderer);

            int x_loc, y_loc;
            Uint32 buttons;

            SDL_PumpEvents(); // make sure we have the latest mouse state.

            buttons = SDL_GetMouseState(&x_loc, &y_loc);
            // count = (UPPER-LOWER)*0.05;
            // UPPER-=count;
            // LOWER+=count/2;

            x_centre = map(x_loc, 0, SCREEN_WIDTH, LOWER_x, UPPER_x);
            y_centre = map(y_loc, 0, SCREEN_HEIGHT, LOWER_y, UPPER_y);
            // SDL_Log("Mouse cursor is at %d, %d", x_loc, y_loc);
            cout << x_loc << " " << y_loc << "\n";

            UPPER_x = x_centre + scale * (UPPER_x - x_centre);
            LOWER_x = x_centre + scale * (LOWER_x - x_centre);
            UPPER_y = y_centre + scale * (UPPER_y - y_centre);
            LOWER_y = y_centre + scale * (LOWER_y - y_centre);
            counter++;

            // SDL_Surface *sshot = SDL_CreateRGBSurface(0, 750, 750, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
            // SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
            // string s = "screenshot";
            // s.append(to_string(counter));
            // s.append(".bmp");
            // SDL_SaveBMP(sshot, "screenshot.bmp");
            // SDL_FreeSurface(sshot);

            // if(MAX_ITERATION>50)
            // {
            //     MAX_ITERATION -= 10;
            // }
            // if(counter>100)
            // {
            //     break;
            // }
        }

        return 0;
    }

    int store_zoom(int red[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT],
                   int green[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT],
                   int blue[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT])
    {
        float count = 0.1;
        float scale = 0.9;
        int counter = 0;
        bool running = true;
        while (running)
        {
            // SDL_RenderClear(renderer);
            SDL_Event event;
            cout << counter << "\n";

            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    running = false;
                    break;

                default:
                    break;
                }
            }

            if (counter >= SIZE)
                break;

            for (int x = 0; x < SCREEN_HEIGHT; x++)
            {
                for (int y = 0; y < SCREEN_WIDTH; y++)
                {
                    long double a = map(x, 0, SCREEN_HEIGHT, LOWER_x, UPPER_x);
                    long double b = map(y, 0, SCREEN_WIDTH, LOWER_y, UPPER_y);

                    Computation::Complex *c = new Computation::Complex(a, b);
                    if (isMandelbrot(c))
                    {
                        red[counter][x][y] = 0;
                        blue[counter][x][y] = 0;
                        green[counter][x][y] = 0;
                    }
                    else
                    {
                        int bright = map(iterations(c), 0, MAX_ITERATION, 0, 255);
                        int r = map(bright * bright, 0, 65025, 0, 255);
                        int g = bright;
                        int b = map(sqrt(bright), 0, 16, 0, 255);
                        red[counter][x][y] = r;
                        blue[counter][x][y] = b;
                        green[counter][x][y] = g;
                    }
                }
            }

            UPPER_x = x_centre + scale * (UPPER_x - x_centre);
            LOWER_x = x_centre + scale * (LOWER_x - x_centre);
            UPPER_y = y_centre + scale * (UPPER_y - y_centre);
            LOWER_y = y_centre + scale * (LOWER_y - y_centre);
            counter++;
            
            if(counter%10==0)
            MAX_ITERATION+=5;
        }
        return 0;
    }

    int render_zoom(SDL_Window *window, SDL_Renderer *renderer,
                    int red[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT],
                    int green[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT],
                    int blue[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT])
    {
        for (int i = 0; i < SIZE; i++)
        {
            SDL_Event event;

            bool running = true;

            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    running = false;
                    break;

                default:
                    break;
                }
            }

            if (!running)
            {
                break;
            }

            for (int x = 0; x < SCREEN_WIDTH; x++)
            {
                for (int y = 0; y < SCREEN_HEIGHT; y++)
                {

                    SDL_SetRenderDrawColor(renderer, red[i][x][y], green[i][x][y], blue[i][x][y], 255);
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
            SDL_RenderPresent(renderer);

        }
        return 0;
    }



    void reset()
    {
        MAX_ITERATION = 200;
        UPPER_x = 2;
        LOWER_x = -2;
        UPPER_y = 2;
        LOWER_y = -2;
    }



};



class UndoZoom
{
public:
    UndoZoom()
    {
        // default constructor
    }

    int play_zoom(SDL_Window *window, SDL_Renderer *renderer)
    {

        float count = 0.1;
        float scale = 0.9;
        int counter = 0;
        bool running = true;
        bool undo = false;

        while (running)
        {
            // SDL_RenderClear(renderer);
            SDL_Event event;

            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_KEYDOWN:
                    undo = !undo;
                break;

                default:
                    break;
                }
            }

            for (int x = 0; x < SCREEN_HEIGHT; x++)
            {
                for (int y = 0; y < SCREEN_WIDTH; y++)
                {
                    long double a = map(x, 0, SCREEN_HEIGHT, LOWER_x, UPPER_x);
                    long double b = map(y, 0, SCREEN_WIDTH, LOWER_y, UPPER_y);

                    Computation::Complex *c = new Computation::Complex(a, b);
                    if (isMandelbrot(c))
                    {
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderDrawPoint(renderer, x, y);
                    }
                    else
                    {
                        int bright = map(iterations(c), 0, MAX_ITERATION, 0, 255);
                        int red = map(bright * bright, 0, 65025, 0, 255);
                        int green = bright;
                        int blue = map(sqrt(bright), 0, 16, 0, 255);
                        SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
                        SDL_RenderDrawPoint(renderer, x, y);
                    }
                }
            }

            // SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
            // SDL_RenderClear(renderer);

            SDL_RenderPresent(renderer);

            int x_loc, y_loc;
            Uint32 buttons;

            SDL_PumpEvents(); // make sure we have the latest mouse state.

            buttons = SDL_GetMouseState(&x_loc, &y_loc);
            // count = (UPPER-LOWER)*0.05;
            // UPPER-=count;
            // LOWER+=count/2;

            x_centre = map(x_loc, 0, SCREEN_WIDTH, LOWER_x, UPPER_x);
            y_centre = map(y_loc, 0, SCREEN_HEIGHT, LOWER_y, UPPER_y);
            // SDL_Log("Mouse cursor is at %d, %d", x_loc, y_loc);
            cout << x_loc << " " << y_loc << "\n";

            if(!undo)
            {
                UPPER_x = x_centre + scale * (UPPER_x - x_centre);
                LOWER_x = x_centre + scale * (LOWER_x - x_centre);
                UPPER_y = y_centre + scale * (UPPER_y - y_centre);
                LOWER_y = y_centre + scale * (LOWER_y - y_centre);
                counter++;
            }
            else
            {
                UPPER_x = x_centre + (1.0/scale) * (UPPER_x - x_centre);
                LOWER_x = x_centre + (1.0/scale) * (LOWER_x - x_centre);
                UPPER_y = y_centre + (1.0/scale) * (UPPER_y - y_centre);
                LOWER_y = y_centre + (1.0/scale) * (LOWER_y - y_centre);
                counter--;
            }

            // SDL_Surface *sshot = SDL_CreateRGBSurface(0, 750, 750, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
            // SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
            // string s = "screenshot";
            // s.append(to_string(counter));
            // s.append(".bmp");
            // SDL_SaveBMP(sshot, "screenshot.bmp");
            // SDL_FreeSurface(sshot);

            // if(MAX_ITERATION>50)
            // {
            //     MAX_ITERATION -= 10;
            // }
            // if(counter>100)
            // {
            //     break;
            // }
        }

        return 0;
    }

    int store_zoom(int red[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT],
                   int green[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT],
                   int blue[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT])
    {
        float count = 0.1;
        float scale = 0.9;
        int counter = 0;
        bool running = true;
        while (running)
        {
            // SDL_RenderClear(renderer);
            SDL_Event event;
            cout << counter << "\n";

            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    running = false;
                    break;

                default:
                    break;
                }
            }

            if (counter >= SIZE)
                break;

            for (int x = 0; x < SCREEN_HEIGHT; x++)
            {
                for (int y = 0; y < SCREEN_WIDTH; y++)
                {
                    long double a = map(x, 0, SCREEN_HEIGHT, LOWER_x, UPPER_x);
                    long double b = map(y, 0, SCREEN_WIDTH, LOWER_y, UPPER_y);

                    Computation::Complex *c = new Computation::Complex(a, b);
                    if (isMandelbrot(c))
                    {
                        red[counter][x][y] = 0;
                        blue[counter][x][y] = 0;
                        green[counter][x][y] = 0;
                    }
                    else
                    {
                        int bright = map(iterations(c), 0, MAX_ITERATION, 0, 255);
                        int r = map(bright * bright, 0, 65025, 0, 255);
                        int g = bright;
                        int b = map(sqrt(bright), 0, 16, 0, 255);
                        red[counter][x][y] = r;
                        blue[counter][x][y] = b;
                        green[counter][x][y] = g;
                    }
                }
            }

            UPPER_x = x_centre + scale * (UPPER_x - x_centre);
            LOWER_x = x_centre + scale * (LOWER_x - x_centre);
            UPPER_y = y_centre + scale * (UPPER_y - y_centre);
            LOWER_y = y_centre + scale * (LOWER_y - y_centre);
            counter++;
            
            if(counter%10==0)
            MAX_ITERATION+=5;
        }
        return 0;
    }

    int render_zoom(SDL_Window *window, SDL_Renderer *renderer,
                    int red[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT],
                    int green[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT],
                    int blue[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT])
    {
        for (int i = 0; i < SIZE; i++)
        {
            SDL_Event event;

            bool running = true;

            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    running = false;
                    break;

                default:
                    break;
                }
            }

            if (!running)
            {
                break;
            }

            for (int x = 0; x < SCREEN_WIDTH; x++)
            {
                for (int y = 0; y < SCREEN_HEIGHT; y++)
                {

                    SDL_SetRenderDrawColor(renderer, red[i][x][y], green[i][x][y], blue[i][x][y], 255);
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
            SDL_RenderPresent(renderer);

        }
        return 0;
    }



    void reset()
    {
        MAX_ITERATION = 200;
        UPPER_x = 2;
        LOWER_x = -2;
        UPPER_y = 2;
        LOWER_y = -2;
    }



};


class QuadZoom
{
public:
    QuadZoom()
    {
        // default constructor
    }

    int play_zoom(SDL_Window *window, SDL_Renderer *renderer)
    {

        float count = 0.1;
        float scale = 0.9;
        int counter = 0;
        bool running = true;
        bool undo = false;

        while (running)
        {
            // SDL_RenderClear(renderer);
            SDL_Event event;

            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_KEYDOWN:
                    undo = !undo;
                break;

                default:
                    break;
                }
            }

            auto red1 = new int[SCREEN_WIDTH][SCREEN_HEIGHT];
            auto green1 = new int[SCREEN_WIDTH][SCREEN_HEIGHT];
            auto blue1 = new int[SCREEN_WIDTH][SCREEN_HEIGHT];

            for (int x = 0; x < SCREEN_HEIGHT; x++)
            {
                for (int y = 0; y < SCREEN_WIDTH; y++)
                {
                    long double a = map(x, 0, SCREEN_HEIGHT, LOWER_x, UPPER_x);
                    long double b = map(y, 0, SCREEN_WIDTH, LOWER_y, UPPER_y);

                    Computation::Complex *c = new Computation::Complex(a, b);
                    if (isMandelbrot(c))
                    {
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderDrawPoint(renderer, x, y);
                        red1[x][y] = 0;
                        blue1[x][y] = 0;
                        green1[x][y] = 0;
                    }
                    else
                    {
                        int bright = map(iterations(c), 0, MAX_ITERATION, 0, 255);
                        int red = map(bright * bright, 0, 65025, 0, 255);
                        int green = bright;
                        int blue = map(sqrt(bright), 0, 16, 0, 255);
                        SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
                        SDL_RenderDrawPoint(renderer, x, y);
                        red1[x][y] = red;
                        blue1[x][y] = blue;
                        green1[x][y] = green;
                    }
                }
            }

            QuadTree::Point a(0,0);
            QuadTree::Point c(SCREEN_WIDTH, SCREEN_WIDTH);
            QuadTree::Quad* qt = new QuadTree::Quad(a,c);
            qt->make(red1, green1, blue1, SCREEN_HEIGHT); // Image compression for use later on and storage

            // SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
            // SDL_RenderClear(renderer);

            SDL_RenderPresent(renderer);

            int x_loc, y_loc;
            Uint32 buttons;

            SDL_PumpEvents(); // make sure we have the latest mouse state.

            buttons = SDL_GetMouseState(&x_loc, &y_loc);
            // count = (UPPER-LOWER)*0.05;
            // UPPER-=count;
            // LOWER+=count/2;

            x_centre = map(x_loc, 0, SCREEN_WIDTH, LOWER_x, UPPER_x);
            y_centre = map(y_loc, 0, SCREEN_HEIGHT, LOWER_y, UPPER_y);
            // SDL_Log("Mouse cursor is at %d, %d", x_loc, y_loc);
            cout << x_loc << " " << y_loc << "\n";

            if(!undo)
            {
                UPPER_x = x_centre + scale * (UPPER_x - x_centre);
                LOWER_x = x_centre + scale * (LOWER_x - x_centre);
                UPPER_y = y_centre + scale * (UPPER_y - y_centre);
                LOWER_y = y_centre + scale * (LOWER_y - y_centre);
                counter++;
            }
            else
            {
                UPPER_x = x_centre + (1.0/scale) * (UPPER_x - x_centre);
                LOWER_x = x_centre + (1.0/scale) * (LOWER_x - x_centre);
                UPPER_y = y_centre + (1.0/scale) * (UPPER_y - y_centre);
                LOWER_y = y_centre + (1.0/scale) * (LOWER_y - y_centre);
                counter--;
            }

            // SDL_Surface *sshot = SDL_CreateRGBSurface(0, 750, 750, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
            // SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
            // string s = "screenshot";
            // s.append(to_string(counter));
            // s.append(".bmp");
            // SDL_SaveBMP(sshot, "screenshot.bmp");
            // SDL_FreeSurface(sshot);

            // if(MAX_ITERATION>50)
            // {
            //     MAX_ITERATION -= 10;
            // }
            // if(counter>100)
            // {
            //     break;
            // }
        }

        return 0;
    }

    int store_zoom(int red[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT],
                   int green[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT],
                   int blue[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT])
    {
        float count = 0.1;
        float scale = 0.9;
        int counter = 0;
        bool running = true;
        while (running)
        {
            // SDL_RenderClear(renderer);
            SDL_Event event;
            cout << counter << "\n";

            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    running = false;
                    break;

                default:
                    break;
                }
            }

            if (counter >= SIZE)
                break;

            for (int x = 0; x < SCREEN_HEIGHT; x++)
            {
                for (int y = 0; y < SCREEN_WIDTH; y++)
                {
                    long double a = map(x, 0, SCREEN_HEIGHT, LOWER_x, UPPER_x);
                    long double b = map(y, 0, SCREEN_WIDTH, LOWER_y, UPPER_y);

                    Computation::Complex *c = new Computation::Complex(a, b);
                    if (isMandelbrot(c))
                    {
                        red[counter][x][y] = 0;
                        blue[counter][x][y] = 0;
                        green[counter][x][y] = 0;
                    }
                    else
                    {
                        int bright = map(iterations(c), 0, MAX_ITERATION, 0, 255);
                        int r = map(bright * bright, 0, 65025, 0, 255);
                        int g = bright;
                        int b = map(sqrt(bright), 0, 16, 0, 255);
                        red[counter][x][y] = r;
                        blue[counter][x][y] = b;
                        green[counter][x][y] = g;
                    }
                }
            }

            UPPER_x = x_centre + scale * (UPPER_x - x_centre);
            LOWER_x = x_centre + scale * (LOWER_x - x_centre);
            UPPER_y = y_centre + scale * (UPPER_y - y_centre);
            LOWER_y = y_centre + scale * (LOWER_y - y_centre);
            counter++;
            
            if(counter%10==0)
            MAX_ITERATION+=5;
        }
        return 0;
    }

    int render_zoom(SDL_Window *window, SDL_Renderer *renderer,
                    int red[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT],
                    int green[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT],
                    int blue[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT])
    {
        for (int i = 0; i < SIZE; i++)
        {
            SDL_Event event;

            bool running = true;

            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    running = false;
                    break;

                default:
                    break;
                }
            }

            if (!running)
            {
                break;
            }

            for (int x = 0; x < SCREEN_WIDTH; x++)
            {
                for (int y = 0; y < SCREEN_HEIGHT; y++)
                {

                    SDL_SetRenderDrawColor(renderer, red[i][x][y], green[i][x][y], blue[i][x][y], 255);
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
            SDL_RenderPresent(renderer);

        }
        return 0;
    }



    void reset()
    {
        MAX_ITERATION = 200;
        UPPER_x = 2;
        LOWER_x = -2;
        UPPER_y = 2;
        LOWER_y = -2;
    }



};




int main(int argc, char **argv)
{
    int type;

    cout<<"Enter zoom type, 1 is for normal, 2 is for undoable and rest is for Compressed \n";
    cin>>type;

    
    if(type==1)
    {
    
        
        int choice = 0;
        
        cout<<"Enter 1 for live computing, 0 for precomputed and any other integer quits \n";

        Zoom z;

        while (true)
        {
            cin >> choice;
            if (SDL_Init(SDL_INIT_VIDEO) < 0)
            {
                printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
                return 1;
            }

            SDL_Window *window = SDL_CreateWindow("SLD test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
            if (!window)
            {
                printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
                return 1;
            }

            SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (!renderer)
            {
                printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
                return 1;
            }

            SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

            
            if (choice == 1)
            {
                z.play_zoom(window, renderer);
            }
            else if(choice==0)
            {
                auto red = new int[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT];
                auto green = new int[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT];
                auto blue = new int[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT];

                z.store_zoom(red, green, blue);
                z.render_zoom(window, renderer, red, green, blue);
                z.reset();

                delete red;
                delete green;
                delete blue;
            }
            else
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                break;
            }
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
        }
    }

    else if(type==2)
    {

        int choice = 0;
        
        cout<<"Enter 1 for live computing, 0 for precomputed and any other integer quits \n";

        UndoZoom z;

        while (true)
        {
            cin >> choice;
            if (SDL_Init(SDL_INIT_VIDEO) < 0)
            {
                printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
                return 1;
            }

            SDL_Window *window = SDL_CreateWindow("SLD test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
            if (!window)
            {
                printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
                return 1;
            }

            SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (!renderer)
            {
                printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
                return 1;
            }

            SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

            
            if (choice == 1)
            {
                z.play_zoom(window, renderer);
            }
            else if(choice==0)
            {
                auto red = new int[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT];
                auto green = new int[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT];
                auto blue = new int[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT];

                z.store_zoom(red, green, blue);
                z.render_zoom(window, renderer, red, green, blue);
                z.reset();

                delete red;
                delete green;
                delete blue;
            }
            else
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                break;
            }
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
        }
    }


    else
    {
        int choice = 0;
        
        cout<<"Enter 1 for live computing, 0 for precomputed and any other integer quits \n";

        QuadZoom z;

        while (true)
        {
            cin >> choice;
            if (SDL_Init(SDL_INIT_VIDEO) < 0)
            {
                printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
                return 1;
            }

            SDL_Window *window = SDL_CreateWindow("SLD test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
            if (!window)
            {
                printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
                return 1;
            }

            SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (!renderer)
            {
                printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
                return 1;
            }

            SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

            
            if (choice == 1)
            {
                z.play_zoom(window, renderer);
            }
            else if(choice==0)
            {
                auto red = new int[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT];
                auto green = new int[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT];
                auto blue = new int[SIZE][SCREEN_WIDTH][SCREEN_HEIGHT];

                z.store_zoom(red, green, blue);
                z.render_zoom(window, renderer, red, green, blue);
                z.reset();

                delete red;
                delete green;
                delete blue;
            }
            else
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                break;
            }
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
        }
    }

    return 0;
}