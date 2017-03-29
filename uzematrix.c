#include <stdbool.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <uzebox.h>
 
// graphics
#include "gfx/gfx_symbols.c"
 
// random number generation
// http://www.dragonsgate.net/pipermail/icc-avr/2005-January/004853.html
long seed = 1234;
#define M 0x7FFFFFFF	// 2^31-1, the modulus used by the psuedo-random
						// number generator prng().
// a good random number generator; call with 1 <= x <= M-1
unsigned char prng()
{
	seed = (seed >> 16) + ((seed << 15) & M) - (seed >> 21) - ((seed << 10) & M);
	if (seed < 0) seed += M;
	return (unsigned char) seed;
}
 
uint8_t tiles[28][30];
uint8_t starts[30];
uint8_t lengths[30];
uint8_t i;
uint8_t n;
 
int main()
{
	// init the graphics
	SetTileTable(tiles_symbols);
 
	// init the first row
	for(n = 0; n < 30; n++)
	{
		tiles[0][n] = (prng() % 30) + 1;
		lengths[n] = (prng() % 8) + 2;
	}
 
	// now the loop
	while(1)
	{
		// wait for v sync
		WaitVsync(1);
 
		// update 3 columns at a time
		for(n = 0; n < 3; n++)
		{
			// choose a random column
			i = prng() % 30;
 
			// I failed to document this
			// and cannot remember what it
			// is doing! D:
			tiles[starts[i]][i] += 30;
			SetTile(i, starts[i], tiles[starts[i]][i]);
 
			starts[i]++;
 
			if(starts[i] < (26-lengths[i]) && tiles[starts[i]+lengths[i]+2][i] != 0)
			{
				tiles[starts[i]+lengths[i]+2][i] += 30;
				SetTile(i, starts[i]+lengths[i]+2, tiles[starts[i]+lengths[i]+2][i]);
			}
			if(starts[i] < (27-lengths[i]) && tiles[starts[i]+lengths[i]+1][i] != 0)
			{
				tiles[starts[i]+lengths[i]+1][i] += 30;
				SetTile(i, starts[i]+lengths[i]+1, tiles[starts[i]+lengths[i]+1][i]);
			}
			if(starts[i] < (28-lengths[i]) && tiles[starts[i]+lengths[i]][i] != 0)
			{
				tiles[starts[i]+lengths[i]][i] = 0;
				SetTile(i, starts[i]+lengths[i], tiles[starts[i]+lengths[i]][i]);
			}
 
			if(starts[i] >= 28)
			{
				starts[i] = 0;
				lengths[i] = (prng() % 8) + 2;
			}
 
			tiles[starts[i]][i] = (prng() % 30) + 1;
			SetTile(i, starts[i], tiles[starts[i]][i]);
		}
	}
}
