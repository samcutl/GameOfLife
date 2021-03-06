#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>

typedef struct World World;
typedef struct WorldCoord WorldCoord;

//A datatype to tell us things like "which buffer are
//we currently writing to?" 
typedef enum {
	RED,
	BLUE
} bufferid;

struct World
{

	unsigned int height;
	unsigned int width;

	/* This implementation of Conway's Game of Life
	 * works by reading one buffer (the previous /
	 * current one) in order to produce the next
	 * generation (the results of this buffer) in
	 * the other one, and then swapping the two.
	 *
	 * So, let's say you've got buffer Red and buffer
	 * Blue. Red is read-only and has nothing in it,
	 * as it was initialized that way. The player
	 * clicks somewhere, flipping some cells buffer
	 * blue, and then the game updates a tick.
	 * This tick update switches the roles of the
	 * two buffers, such that blue buffer is the one
	 * from which we read, and the results of that
	 * are written into the red buffer.*/

	//TODO: More concise explanation. :P
	bufferid writeBuf;

	bool* redBuffer;
	bool* blueBuffer;

	/*Incremented every time the world is resized,
	used to inform the renderer that this has happened.*/
	int resizeRevision;
};

struct WorldCoord
{
	int x, y;
};
void Update(World *const);

void FlipBuffers(World *const);
/* 
 * First int pair is new size, second is
 * offset by which we blit the old map
 * onto the new one.
 */
void ResizeWorld(World *const, unsigned int, unsigned int, int, int);

void SetCell(World *const, int, int, bool);
void FlipCell(World *const, int, int);

void InitializeWorld(World *const, unsigned int, unsigned int);
void DestroyWorld(World *const);
/* For rendering and other such output functions.
 * Wraps/hides buffering and such. */
bool GetCell(World *const, int, int);

//Basically a convenience function over InitializeWorld.
World* MakeWorld(unsigned int, unsigned int);

#endif
