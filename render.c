#include "render.h"
#include <stdio.h>

void InitGameRenderer(GameRenderer* g, const char* wtitle, unsigned int wwidth, unsigned int wheight)
{
		
	//Our window 
	g->window = NULL; 

	g->windowWidth = wwidth;
	g->windowHeight = wheight;

	g->gameWidth = wwidth;
	g->gameHeight = wheight;	

	g->zoom = 0;
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL did not initialize, SDL_GetError() says: %s\n", SDL_GetError() ); 
		return 1;
	}


	 //Create the window 
	g->window = SDL_CreateWindow( wtitle, SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, wwidth, wheight, SDL_WINDOW_SHOWN );
	if( g->window == NULL ) 
	{ 
		printf( "Window wasn't initialized, SDL_GetError() says: %s\n", 
			SDL_GetError() ); 
		return 1;
	}
 
	g->renderer = 0;
	g->renderer = SDL_CreateRenderer( g->window, -1, SDL_RENDERER_TARGETTEXTURE );
	if( g->renderer == NULL )
	{
		printf( "Renderer didn't initialize properly, SDL_GetError() says: %s\n", SDL_GetError() );

		return 1;
	}

	//We will need a function for updating game area size,
	//called on world resize, that reallocates this.
	g->gameTexture = SDL_CreateTexture(g->renderer, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET, wwidth, wheight);
}


void DrawGame(GameRenderer* g, World* world)
{

	//Prepare to render to texture.
	SDL_SetRenderTarget(g->renderer, g->gameTexture);
	
	//Clear screen
	SDL_SetRenderDrawColor( g->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( g->renderer );
	
	//Draw our grid
	DrawBackgroundGrid( g->renderer, g->gameWidth, g->gameHeight, g->gridProps );		
	DrawCells(g->renderer, g->gridProps, world);		
	SDL_RenderPresent( g->renderer );

	//Now draw the rest of the screen
	SDL_SetRenderTarget(g->renderer, NULL);
	SDL_SetRenderDrawColor( g->renderer, 180, 180, 180, 255 );
	SDL_RenderClear( g->renderer );

	//Dumb rectangle for testing
	SDL_Rect stretchRect; 
	stretchRect.x = 0; 
	stretchRect.y = 0; 
	stretchRect.w = 256; 
	stretchRect.h = 256; 
	SDL_RenderCopy(g->renderer, g->gameTexture, NULL, &stretchRect);
	SDL_RenderPresent(g->renderer);
}

void DrawCells(SDL_Renderer* render, DrawableGrid* grid, World* w)
{
	bool current = false;
	
	SDL_SetRenderDrawColor(render, 
		grid->color.r, grid->color.g, grid->color.b, grid->color.a);
	for(int x = 0; x < w->width; ++x)
	{
		for(int y = 0; y < w->height; ++y)
		{
			current = GetCell(w, x, y);
			if(current)
			{
				SDL_Rect rectCell =
				 	{x*(grid->cellSize), y*(grid->cellSize),
				 	grid->cellSize, grid->cellSize};
				
				SDL_RenderFillRect(render, &rectCell );
			}
		}
	}	
}

void DrawBackgroundGrid(SDL_Renderer* renderer, unsigned int width, unsigned int height, 
	DrawableGrid* grid)
{
	SDL_SetRenderDrawColor(renderer, grid->color.r, grid->color.g, grid->color.b, grid->color.a);
	//Draw vertical lines.
	for(int x = 0; x < width; x += grid->cellSize)
	{
		SDL_RenderDrawLine( renderer, x, 0, x, height);
	}
	//Draw horizontal lines.
	for(int y = 0; y < height; y += grid->cellSize)
	{
		SDL_RenderDrawLine( renderer, 0, y, width, y);
	}
}
void CleanupRendering(GameRenderer* g)
{
	SDL_DestroyTexture(g->gameTexture);
	SDL_DestroyRenderer(g->renderer);
	SDL_DestroyWindow(g->window);
	SDL_Quit();
}
