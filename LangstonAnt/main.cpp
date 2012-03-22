#include "SDL.h"
#include <vector>

void handleInit();
void handleEvent();
void handleUpdate();
void handleDraw();
void handleDeinit();

void drawGrid();
void redrawScreen();

void FillRect(int x, int y, int w, int h, int color); // by patrick - http://pbeblog.wordpress.com/2009/06/24/drawing-rectangles-sdl/

bool quit=false;
bool stop=false;
int gridSize=9;
int gridScale=27*3;
std::vector< std::vector<int> > grid (gridSize,std::vector<int>(gridSize,0));
std::vector< std::vector<int> > gridnew (gridSize,std::vector<int>(gridSize,0));
SDL_Surface* screen;
Uint32* pixels;
int ant[2]={(gridSize-1)/2,(gridSize-1)/2};
int orientation = 1;
const int ORIENTATIONS[4][2] = {{1,0},{0,-1},{-1,0},{0,1}};
const int COLORS[4]={0xFFFFFF,0xFF0000,0x00FF00,0x0000FF};
const int DIRECTIONS[4]={-1,1,1,1};
const int COLORCHANGE[4]={1,2,3,0};

int main(int argc,char* args[])
{
	handleInit();

	while(!quit)
	{
		handleEvent();
		if(!stop)
		{
			handleUpdate();
		}
		handleDraw();
	}

	handleDeinit();
	return 0;
}

void handleInit()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	screen=SDL_SetVideoMode(gridSize*gridScale,gridSize*gridScale,32,SDL_SWSURFACE);
	pixels=(Uint32*)screen->pixels;

	redrawScreen();
}

void handleEvent()
{
	SDL_Event event;
	if(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_SPACE:
						quit=true;
					break;
				}
			break;
		}
	}
}

void handleUpdate()
{
	int gVal = grid[ant[0]][ant[1]];
	orientation=(orientation+DIRECTIONS[gVal]+4)%4;
	grid[ant[0]][ant[1]]=COLORCHANGE[gVal];
	drawGrid();
	
	//move
	ant[0]+=ORIENTATIONS[orientation][0];
	ant[1]+=ORIENTATIONS[orientation][1];

	//if out of range, enlarge grid
	if(ant[0]<0 || ant[0]>gridSize-1 || ant[1]<0 || ant[1]>gridSize-1)
	{
		if(gridScale<=1)
		{
			stop=true;
			ant[0]-=ORIENTATIONS[orientation][0];
			ant[1]-=ORIENTATIONS[orientation][1];
		}
		else
		{
			gridSize*=3;
			gridScale/=3;

			gridnew.clear();
			gridnew.resize(gridSize,std::vector<int>(gridSize,0));

			for(int x=0;x<gridSize/3;x++)
			{
				for(int y=0;y<gridSize/3;y++)
				{
					gridnew[gridSize/3+x][gridSize/3+y]=grid[x][y];
				}
			}
			gridnew.swap(grid);
	
			ant[0]+=gridSize/3;
			ant[1]+=gridSize/3;

			redrawScreen();
		}
	}
}

void drawGrid()
{
	int x=ant[0];
	int y=ant[1];
	FillRect(x*gridScale,y*gridScale,gridScale,gridScale,COLORS[grid[x][y]]);
}

void handleDraw()
{
	SDL_Flip(screen);
}

void handleDeinit()
{

}

void FillRect(int x, int y, int w, int h, int color)
{
	SDL_Rect rect = {x,y,w,h};
	SDL_FillRect(screen, &rect, color);
}

void redrawScreen()
{
	for(int x=0;x<gridSize;x++)
	{
		for(int y=0;y<gridSize;y++)
		{
			FillRect(x*gridScale,y*gridScale,gridScale,gridScale,COLORS[grid[x][y]]);
		}
	}
}