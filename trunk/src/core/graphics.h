#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <psptypes.h>

//#define	PSP_LINE_SIZE 512
//#define SCREEN_WIDTH 480
//#define SCREEN_HEIGHT 272

//typedef u32 Color;
//#define A(color) ((u8)(color >> 24 & 0xFF))
//#define B(color) ((u8)(color >> 16 & 0xFF))
//#define G(color) ((u8)(color >> 8 & 0xFF))
//#define R(color) ((u8)(color & 0xFF))

typedef struct
{
	int textureWidth;	// the real width of data, 2^n with n>=0
	int textureHeight;	// the real height of data, 2^n with n>=0
	int imageWidth; 	// the image width
	int imageHeight;
	/*Color*/u32* pixels;
} Image;

/**
 * Load a PNG image.
 *
 * @pre filename != NULL
 * @param filename - filename of the PNG image to load
 * @return pointer to a new allocated Image struct, or NULL on failure
 */
extern Image* my_png_loadImage(const char *filename);

/**
 * Frees an allocated image.
 *
 * @pre image != null
 * @param image a pointer to an image struct
 */
extern void my_png_freeImage(Image *image);

#endif
