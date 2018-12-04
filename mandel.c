#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>

#define WIDTH 1200
#define HEIGHT 1000
#define MAX 255

SDL_Surface *surface;
SDL_Window *screen;
SDL_Renderer *renderer;
SDL_Texture *texture;

void setPixel(SDL_Surface *surface, unsigned x, unsigned y, Uint8 r, Uint8 g, Uint8 b);
Uint8 calcMandel(unsigned x, unsigned y);
void mandelbrot(SDL_Surface *surface);
void render(void);
void inputs(void);
int main(int argc, char * argv[]);

struct complex
{
	float r;
	float i;
};

void setPixel(SDL_Surface *surface, unsigned x, unsigned y, Uint8 r, Uint8 g, Uint8 b)
{
	Uint32 color;
	Uint8 *p;
	
	/* turn r, g, b into a color */
	color = SDL_MapRGB(surface->format, r % (255 + 1), g % (255 + 1), b % (255 + 1));
	
	/* set the pixel to a color */
	p = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;
	*(Uint32 *)p = color;
}

/*mandelbrot set: z = z*z + c*/
Uint8 calcMandel(unsigned x, unsigned y)
{
	struct complex z;
	struct complex c;
	
	Uint8 iterations = 0;
	float oldRe = 0, oldIm = 0;
	
	z.r = 0;
	z.i = 0;
	c.r = (((((float)x / (float)WIDTH) * 2.0f) - 1.5f) * 1.5);
	c.i = (((((float)y / (float)HEIGHT) * 2.0f) - 1.0f) * 1.5);
	
	while(sqrt(z.r * z.r + z.i * z.i) <= 4.0 && iterations != 100)
	{
		oldRe = z.r;
		oldIm = z.i;
		z.r = (oldRe * oldRe - oldIm * oldIm) + c.r;
		z.i = (2.0 * oldRe * oldIm) + c.i;
		
		iterations++;
	}

	return iterations;
}

void mandelbrot(SDL_Surface *surface)
{
	unsigned y;
	unsigned x;
	Uint8 manRed;
	
	for(y = 0; y <= HEIGHT; y++)
	{
		for(x = 0; x <= WIDTH; x++)
		{
			manRed = calcMandel(x, y);
			setPixel(surface, x, y, manRed, 0, 0);
		}
	}
	
	/* send the pixels to the texture buffer */
	SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
	
	/* copy the texture directly to the backbuffer */
	SDL_RenderCopy(renderer, texture, NULL, NULL);
}

void render(void)
{
	/* draw the shape on the screen */
	mandelbrot(surface);

	/* flip the back with the front buffer */
	SDL_RenderPresent(renderer);
}

void inputs(void)
{
	SDL_Event event;

	SDL_PollEvent(&event);
	switch(event.type)
	{
		case SDL_QUIT:
			exit(0);
			break;
			
		case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_ESCAPE)
			{
				exit(0);
			}
			break;
			
		default:
			break;
	}
}

int main(int argc, char * argv[])
{	
	(void)(argc);
	(void)(argv);
	
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Could not start SDL %s\n", SDL_GetError());
		exit(1);
	}
	
	atexit(SDL_Quit);
	
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &screen, &renderer);	
	
	surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32,
                                        0x00FF0000,
                                        0x0000FF00,
                                        0x000000FF,
                                        0xFF000000);
	if(surface == NULL)
	{
		fprintf(stderr, "Could not setup surface %s", SDL_GetError());
		exit(1);
	}
	
	/* enable the surface to be a renderable texture */
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if(texture == NULL)
	{
		fprintf(stderr, "Could not setup texture %s", SDL_GetError());
		exit(1);
	}	
	
	/* clear screen to black */
	SDL_SetRenderDrawColor(renderer, 1, 0, 0, 0);
	SDL_RenderClear(renderer);
	
	printf("Press escape to stop.");
	
	while(1)
	{
		render();
		inputs();
	}
	
	return 0;
}