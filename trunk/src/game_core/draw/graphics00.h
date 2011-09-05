
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

#ifndef _GRAPHICS_00_H_
#define _GRAPHICS_00_H_

//#include <psptypes.h>

//#define PSP_LINE_SIZE 512
//#define SCREEN_WIDTH	480
//#define SCREEN_HEIGHT 272

//typedef u32 Color;
//#define A(color) ((u8)(color >> 24 & 0xff))
//#define B(color) ((u8)(color >> 16 & 0xff))
//#define G(color) ((u8)(color >>  8 & 0xff))
//#define R(color) ((u8)(color		 & 0xff))

typedef struct
{
	u32 	*pixels;				/*Color*/
	int 	texture_width;		// the real width of data, 2^n with n>=0
	int 	texture_height; 	// the real height of data, 2^n with n>=0
	int 	texture_format; 	//
//	int 	imageWidth; 		// the image width
//	int 	imageHeight;
} my_image;

/**
 * Load a PNG image.
 *
 * @pre filename != NULL
 * @param filename - filename of the PNG image to load
 * @return pointer to a new allocated Image struct, or NULL on failure
 */
extern my_image 	*png_load_my_image(const char *filename);

/**
 * Frees an allocated image.
 *
 * @pre image != null
 * @param image a pointer to an image struct
 */
extern void png_free_my_image(my_image *image);

/*---------------------------------------------------------
	�w���p�[�֐��͓������ׂ��H
---------------------------------------------------------*/
extern void trans_format8888to5650(u32 *pixsrc, u16 *pixdst, int trans_size);


#endif /* _GRAPHICS_00_H_ */
