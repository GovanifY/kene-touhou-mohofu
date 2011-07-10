
#define USE_PSP_READ (1)
#if (1==USE_PSP_READ)
extern int sceIoRead(int fd, void *data, int size);
#endif


#if !defined(PNG_NO_STDIO)
/* This is the function that does the actual reading of data.  If you are
	not reading from a standard C stream, you should create a replacement
	read_data function and use it at run time with png_set_read_fn(), rather
	than changing the library. */
#ifndef USE_FAR_KEYWORD

void PNGAPI aa_png_default_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
	png_size_t check;

	/* f read() returns 0 on error, so it is OK to store this in a png_size_t
	 * instead of an int, which is what f read() actually returns.
	 */
#if defined(_WIN32_WCE)
	if ( !ReadFile((HANDLE)(png_ptr->io_ptr), data, length, &check, NULL) )
	{	check = 0;}
#else
	#if (1==USE_PSP_READ)
	check = sceIoRead(fileno((png_FILE_p)(png_ptr->io_ptr)), data, length);
	#else
	check = (png_size_t)f read(data, (png_size_t)1, length, (png_FILE_p)png_ptr->io_ptr);
	#endif
#endif

	if (check != length)
	{	png_error(png_ptr, "Read Error");}
}
#else
/* this is the model-independent version. Since the standard I/O library
	can't handle far buffers in the medium and small models, we have to copy
	the data.
*/

#define NEAR_BUF_SIZE 1024
#define MIN(a,b) (a <= b ? a : b)

static void /* PRIVATE */ bb_png_default_read_data(png_structp png_ptr, png_bytep dest_data, png_size_t length)
{
	int check;
	png_byte *n_data;
	png_FILE_p io_ptr;

	/* Check if dest_data really is near. If so, use usual code. */
	n_data = (png_byte *)CVT_PTR_NOCHECK(dest_data);
	io_ptr = (png_FILE_p)CVT_PTR(png_ptr->io_ptr);
	if ((png_bytep)n_data == dest_data)
	{
//#if defined(_WIN32_WCE)
//		if ( !ReadFile((HANDLE)(png_ptr->io_ptr), dest_data, length, &check, NULL) )
//		{	check = 0;}
//#else
		#if (1==USE_PSP_READ)
		check = sceIoRead(fileno((png_FILE_p)(io_ptr)), n_data, length);
		#else
		check = f read(n_data, 1, length, io_ptr);
		#endif
//#endif
	}
	else
	{
		png_byte buf[NEAR_BUF_SIZE];
		png_size_t read, remaining, err;
		check		= 0;
		remaining	= length;
		do
		{
			read = MIN(NEAR_BUF_SIZE, remaining);
//#if defined(_WIN32_WCE)
//			if ( !ReadFile((HANDLE)(io_ptr), buf, read, &err, NULL) )
//			{	err = 0;}
//#else
			#if (1==USE_PSP_READ)
			err = sceIoRead(fileno((png_FILE_p)(io_ptr)), buf, read);
			#else
			err = f read(buf, (png_size_t)1, read, io_ptr);
			#endif
//#endif
			png_memcpy(dest_data, buf, read); /* copy far buffer to near buffer */
			if (err != read)
			{	break;}
			else
			{	check += err;}
			dest_data += read;
			remaining -= read;
		}
		while (remaining != 0);
	}
	if ((png_uint_32)check != (png_uint_32)length)
	{	png_error(png_ptr, "read Error");}
}
#endif
#endif
