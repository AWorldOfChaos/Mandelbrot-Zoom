#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define x_centre exp(1)/7   //-e/7-e/20i, the point to zoom in on
#define y_centre exp(1)/20 

long double UPPER_x = 2;
long double LOWER_x = -2;
long double UPPER_y = 2;
long double LOWER_y = -2;

long MAX_ITERATION = 100;

using namespace std;

class Complex
{
    /** 
    This class stores a complex number inialised with a real and imaginary part
    */

    const int constant = 2;
    long double real,img;
    public:
    Complex(long double r, long double i)
    {
        real = r;
        img = i;
    }
    float getreal()
    {
        return real;
    }

    float getimg()
    {
        return img;
    }

    Complex add(Complex c)
    {
        return Complex(real+c.getreal(),img+c.getimg());
    }

    Complex multiply(Complex c)
    {
        return Complex(real*c.getreal()-img*c.getimg(), img*c.getreal()+real*c.getimg());
    }

    Complex square()
    {
        return multiply(Complex(real,img));
    }

    float magnitude()
    {
        return sqrt(real*real+img+img);
    }
};


long double map(long double x, long double in_min, long double in_max, long double out_min, long double out_max) 
{
    /**
    This function takes in value(PARAM 1) between values(PARAM 2 and 3)
    and returns a value between 2 values(PARAM 4 and 5)
    */

    return 1.0*(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


bool isMandelbrot (Complex* c)
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

    i=0;
    Re=0;
    Im=0;
    a=0;
    b=0;

    while (i < MAX_ITERATION) {

        Re=((a*a)-(b*b))+x;
        Im=(2 * a * b) + y;
        if(pow(Re,2)+pow(Im,2)>4)
        return false;

        a = Re;
        b = Im;

        i++;
    }

    finaldist = pow(Re,2)+pow(Im,2);

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

long iterations(Complex* c)
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

    i=0;
    Re=0;
    Im=0;
    a=0;
    b=0;

    while (i < MAX_ITERATION) {

        Re=((a*a)-(b*b))+x;
        Im=(2 * a * b) + y;
        if(pow(Re,2)+pow(Im,2)>4)
        return i;

        a = Re;
        b = Im;

        i++;
    }

    return MAX_ITERATION;
}



int main(int argc, char** argv){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("SLD test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(!window){
        printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    float count = 0.1;
    float scale = 0.9;
    int counter=0;
    bool running = true;
    while(running)
    {
        // SDL_RenderClear(renderer);
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;

                default:
                    break;
            }
        }

        for(int x=0; x<SCREEN_HEIGHT; x++)
        {
            for(int y=0; y<SCREEN_WIDTH; y++)
            {
                long double a = map(x, 0, SCREEN_HEIGHT, LOWER_x, UPPER_x);
                long double b = map(y, 0, SCREEN_WIDTH, LOWER_y, UPPER_y);

                Complex* c = new Complex(a,b);
                if(isMandelbrot(c))
                {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawPoint(renderer, x, y);
                }
                else
                {
                    int bright = map(iterations(c),0, MAX_ITERATION, 0,255);
                    int red = map(bright*bright, 0, 65025, 0, 255);
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

        int x_loc,y_loc;
        Uint32 buttons;

        SDL_PumpEvents();  // make sure we have the latest mouse state.

        buttons = SDL_GetMouseState(&x_loc, &y_loc);
        // count = (UPPER-LOWER)*0.05;
        // UPPER-=count;
        // LOWER+=count/2;

        UPPER_x = x_centre + scale*(UPPER_x-x_centre);
        LOWER_x = x_centre + scale*(LOWER_x-x_centre);
        UPPER_y = y_centre + scale*(UPPER_y-y_centre);
        LOWER_y = y_centre + scale*(LOWER_y-y_centre);
        counter++;

        // SDL_Surface *sshot = SDL_CreateRGBSurface(0, 750, 750, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
        // SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
        // string s = "screenshot";
        // s.append(to_string(counter));
        // s.append(".bmp");
        // SDL_SaveBMP(sshot, "screenshot.bmp");
        // SDL_FreeSurface(sshot);

        if(MAX_ITERATION>50)
        {
            MAX_ITERATION -= 10;
        }
        if(counter>100)
        {
            break;
        }
        
    }

    return 0;
}