#define PLAY_IMPLEMENTATION
#include "Play.h"
#include "Boid.h"
#include "Flock.h"

int DISPLAY_WIDTH = 640;
int DISPLAY_HEIGHT = 360;
int DISPLAY_SCALE = 2;

//Create a flock
Flock flockOne;

void MainGameEntry(PLAY_IGNORE_COMMAND_LINE)
{
	Play::CreateManager(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE);
	
	//Create and add boids to a flock
	for (int i = 0; i < 100; i++)
	{
		Boid boid(Vector2D(DISPLAY_WIDTH, DISPLAY_HEIGHT), Vector2D(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2), Vector2D(0, 0), Vector2D(0, 0));
		flockOne.AddBoid(boid);
	}
}

bool MainGameUpdate(float elapsedTime)
{
	Play::ClearDrawingBuffer(Play::cBlack);
	flockOne.UpdateFlock();
	Play::PresentDrawingBuffer();
	return Play::KeyDown(VK_ESCAPE);
}

int MainGameExit(void)
{
	Play::DestroyManager();
	return PLAY_OK;
}




