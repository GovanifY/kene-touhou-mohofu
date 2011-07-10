
#include "mohou_mikmod_internals.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	-------------------------------------------------------
	MikMod sound library (c) 1998, 1999, 2000 Miodrag Vallat and others.
	-------------------------------------------------------
	$Id: mmio.c,v 1.4 2000/05/08 14:39:59 hercules Exp $
	Portable file I/O routines
	-------------------------------------------------------
	The way this module works:

	- mod_music_fopen will call the errorhandler [see mmerror.c] in addition to
	  setting mod_music_error_number on exit.
	- mod_music_iobase is for internal use.  It is used by Player_LoadFP to
	  ensure that it works properly with wad files.
	- mod_music_file_read_..._LE and
	  mod_music_file_read_..._BE differ :
	  the first is for reading data written by
	  a little endian (Intel(R), RISC, psp) machine(LE), and
	  the second is for reading big endian (Motrola(R), Mac, RISC, Alpha) machine data(BE).
	- mod_music_write functions work the same as the mod_music_read functions.
	- mod_music_file_read_string is for reading binary strings.
	  It is basically the same as an fread of bytes.
	-------------------------------------------------------
	FIXME:
		the mod_music_iobase variable ought to be MF_READER-specific.
		It will eventually become a private field of the MF_READER structure,
		but this will require a soname version bump.
		In the meantime, the drawback is that if you use the xxx_LoadFP functions,
		you can't have several MF_READER objects with different iobase values.
---------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#define COPY_BUFSIZE	(1024)

static int mod_music_iobase 	= 0;
static int temp_iobase			= 0;


#if (MYMOD_stdio_FILE_system==USE_MYMOD_FILE_MODE)
	/* FILE *fp を使う */
	//#define MYMOD_FILE	FILE
	//#define s_MYMOD_Player_LoadFP s_Player_LoadFP
//	#define mo d_music_MYMOD_FILE_Reader_Eof	mo d_music_FileReader_Eof
//	#define mo d_music_MYMOD_FILE_Reader_Read	mo d_music_FileReader_Read
//	#define mo d_music_MYMOD_FILE_Reader_Get	mo d_music_FileReader_Get
//	#define mo d_music_MYMOD_FILE_Reader_Seek	mo d_music_FileReader_Seek
//	#define mo d_music_MYMOD_FILE_Reader_Tell	mo d_music_FileReader_Tell
#endif

//#if 0//def US E_RWOPS
#if (MYMOD_SDL_rw_FILE_system==USE_MYMOD_FILE_MODE)
	/* SDL の RW を使う */
	//#define MYMOD_FILE	SDL_RWops
	//#define s_MYMOD_Player_LoadFP s_Player_LoadRW
//	#define mo d_music_MYMOD_FILE_Reader_Eof	mo d_music_SDL_RWopsReader_Eof
//	#define mo d_music_MYMOD_FILE_Reader_Read	mo d_music_SDL_RWopsReader_Read
//	#define mo d_music_MYMOD_FILE_Reader_Get	mo d_music_SDL_RWopsReader_Get
//	#define mo d_music_MYMOD_FILE_Reader_Seek	mo d_music_SDL_RWopsReader_Seek
//	#define mo d_music_MYMOD_FILE_Reader_Tell	mo d_music_SDL_RWopsReader_Tell
#endif



typedef struct MYMOD_FILE_READER
{
	MF_READER	core;
	MYMOD_FILE_ptr fp_rw;		// FILEで言えば FILE *fp 相当。(但しSDL側で抽象化した構造体のポインタなのでFILEと互換は無い)
	#if (MYMOD_SDL_rw_FILE_system==USE_MYMOD_FILE_MODE)
	int end;						// FILEと仕様が違うので互換用
	#endif
} MYMOD_FILE_READER;



//FILE* mod_music_fopen(MM_CHAR *fname, MM_CHAR *attrib)
//FILE *mod_music_fopen_FILE(MM_CHAR *fname)
MYMOD_FILE_ptr mod_music_fopen_FILE(MM_CHAR *fname)
{
//	FILE *fp;
	MYMOD_FILE_ptr fp;
//	fp = fopen(fname, "rb"/*attrib*/);
	fp = MYMOD_fopen(fname, MYMOD_attrib);
	if (!fp)
	{
		mod_music_error_number = MOD_MUSIC_ERROR_OPENING_FILE;
		if (mod_music_error_handler) mod_music_error_handler();
	}
	return (fp);
}

#if 0/*(未使用)*/
MM_BOOL mod_music_MYMOD_FILE_Exists(MM_CHAR *fname)
{
	FILE *fp;
	fp = MYMOD_fopen(fname, "r");
	if (!fp)	{	return (0); 	}
	MYMOD_fclose(fp);
	return (1);
}
#endif

/* Sets the current file-position as the new mod_music_iobase */
void mod_music_iobase_setcur(MF_READER *reader)
{
	temp_iobase = mod_music_iobase;  /* store old value in case of revert */
	mod_music_iobase = reader->Tell(reader);
}

/* Reverts to the last known mod_music_iobase value. */
void mod_music_iobase_revert(void)
{
	mod_music_iobase = temp_iobase;
}

#if 0 //def US E_RWOPS
/*
  This section is added to use SDL_rwops for IO.
  -Matt Campbell (matt@campbellhome.dhs.org) April 2000.
*/
/*
  End SDL_rwops section
*/
#endif /* US E_RWOPS */

#if (1)
/* support FILE read functions */

/*---------- File Reader */

static MM_BOOL mod_music_MYMOD_FILE_Reader_Eof(MF_READER *reader)
{
	#if (MYMOD_psp_FILE_system==USE_MYMOD_FILE_MODE)
	#endif
	#if (MYMOD_stdio_FILE_system==USE_MYMOD_FILE_MODE)
	return feof(((MYMOD_FILE_READER*)reader)->fp_rw);
//	return feof(((MYMOD_FILE_READER*)reader)->fp_rw);
	#endif
	#if (MYMOD_SDL_rw_FILE_system==USE_MYMOD_FILE_MODE)
	/* SDL用、feof()互換(FILEと仕様が違う) */
	if ( ((MYMOD_FILE_READER*)reader)->end == SDL_RWtell(((MYMOD_FILE_READER*)reader)->rw) )
			{return (1);}
	else	{return (0);}
	#endif
}

static MM_BOOL mod_music_MYMOD_FILE_Reader_Read(MF_READER *reader, void* ptr, size_t size)
{
	#if (MYMOD_psp_FILE_system==USE_MYMOD_FILE_MODE)
	#endif
	#if (MYMOD_stdio_FILE_system==USE_MYMOD_FILE_MODE)
	return fread(ptr, size, 1, ((MYMOD_FILE_READER*)reader)->fp_rw);
	#endif
	#if (MYMOD_SDL_rw_FILE_system==USE_MYMOD_FILE_MODE)
	/* SDL用、fread()互換(FILEと引数の順序が違う。寧ろfread()が変) */
	return (SDL_RWread(((MYMOD_FILE_READER*)reader)->fp_rw, ptr, size, 1));
	#endif
}

static int mod_music_MYMOD_FILE_Reader_Get(MF_READER *reader)
{
	#if (MYMOD_psp_FILE_system==USE_MYMOD_FILE_MODE)
	#endif
	#if (MYMOD_stdio_FILE_system==USE_MYMOD_FILE_MODE)
	return fgetc(((MYMOD_FILE_READER*)reader)->fp_rw);
	#endif
	#if (MYMOD_SDL_rw_FILE_system==USE_MYMOD_FILE_MODE)
	/* SDL用、fgetc()互換(FILEと仕様が違う) */
	char buf;
	if ( SDL_RWread(((MYMOD_FILE_READER*)reader)->fp_rw, &buf, 1, 1) != 1 )
			{return (EOF);}
	else	{return ((int)buf);}
	#endif
}

static MM_BOOL mod_music_MYMOD_FILE_Reader_Seek(MF_READER *reader, int offset, int whence)
{
	#if (MYMOD_psp_FILE_system==USE_MYMOD_FILE_MODE)
	return (sceIoLseek32( ((MYMOD_FILE_READER*)reader)->fp_rw, (whence==SEEK_SET)?offset+mod_music_iobase:offset, whence));
	#endif
	#if (MYMOD_stdio_FILE_system==USE_MYMOD_FILE_MODE)
	return (fseek(((MYMOD_FILE_READER*)reader)->fp_rw, (whence==SEEK_SET)?offset+mod_music_iobase:offset, whence));
	#endif
	#if (MYMOD_SDL_rw_FILE_system==USE_MYMOD_FILE_MODE)
	/* SDL用、fseek()互換(FILEと仕様が同じ) */
	return (SDL_RWseek(((MYMOD_FILE_READER*)reader)->fp_rw, (whence==SEEK_SET)?offset+mod_music_iobase:offset, whence));
	#endif
}

static int mod_music_MYMOD_FILE_Reader_Tell(MF_READER *reader)
{
	#if (MYMOD_psp_FILE_system==USE_MYMOD_FILE_MODE)
	#endif
	#if (MYMOD_stdio_FILE_system==USE_MYMOD_FILE_MODE)
	return (ftell(((MYMOD_FILE_READER*)reader)->fp_rw) - mod_music_iobase);
	#endif
	#if (MYMOD_SDL_rw_FILE_system==USE_MYMOD_FILE_MODE)
	/* SDL用、ftell()互換(FILEと仕様が同じ) */
	return (SDL_RWtell(((MYMOD_FILE_READER*)reader)->fp_rw) - mod_music_iobase);
	#endif
}

#if (MYMOD_psp_FILE_system==USE_MYMOD_FILE_MODE)
MF_READER *mod_music_new_psp_fd_reader(MYMOD_FILE_ptr fp_rw)
#endif
#if (MYMOD_stdio_FILE_system==USE_MYMOD_FILE_MODE)
//MF_READER *mod_music_new_file_reader(MYMOD_FILE_ptr fp_rw)
MF_READER *mod_music_new_file_reader(MYMOD_FILE_ptr fp_rw)
#endif
#if (MYMOD_SDL_rw_FILE_system==USE_MYMOD_FILE_MODE)
MF_READER *mod_music_new_rwops_reader(MYMOD_FILE_ptr fp_rw)
#endif
{
	MYMOD_FILE_READER* reader = (MYMOD_FILE_READER*)mod_music_malloc(sizeof(MYMOD_FILE_READER));
	if (reader)
	{
		/* ファイルアクセスはクラス化して隠蔽 */
		reader->core.Eof	= &mod_music_MYMOD_FILE_Reader_Eof;
		reader->core.Read	= &mod_music_MYMOD_FILE_Reader_Read;
		reader->core.Get	= &mod_music_MYMOD_FILE_Reader_Get;
		reader->core.Seek	= &mod_music_MYMOD_FILE_Reader_Seek;
		reader->core.Tell	= &mod_music_MYMOD_FILE_Reader_Tell;
		reader->fp_rw		= fp_rw;
		#if (MYMOD_SDL_rw_FILE_system==USE_MYMOD_FILE_MODE)
		/* RWops does not explicitly support an eof check, so we shall find
		   the end manually - this requires seek support for the RWop */
		{
			int here;
			here				= SDL_RWtell(fp_rw);
			reader->end 		= SDL_RWseek(fp_rw, 0, SEEK_END);
			SDL_RWseek(fp_rw, here, SEEK_SET);		/* Move back */
		}
		#endif
	}
	return ((MF_READER*)reader);
}


#if (MYMOD_psp_FILE_system==USE_MYMOD_FILE_MODE)
void mod_music_delete_psp_fd_reader(MF_READER *reader)
#endif
#if (MYMOD_stdio_FILE_system==USE_MYMOD_FILE_MODE)
void mod_music_delete_file_reader(MF_READER *reader)
#endif
#if (MYMOD_SDL_rw_FILE_system==USE_MYMOD_FILE_MODE)
void mod_music_delete_rwops_reader(MF_READER *reader)
#endif
{
	if (reader) 	{	free(reader);}
}


/*---------- Read functions */










#endif/*(1)*/

#if 0//def USE_MIKMOD_ALONE
/* not support FILE write functions */

/*---------- File Writer */

typedef struct MFILEWRITER
{
	MF_WRITER core;
	FILE*	file;
} MFILEWRITER;

static MM_BOOL mod_music_MYMOD_FILE_Writer_Seek(MF_WRITER* writer, long offset, int whence)
{
	return fseek(((MFILEWRITER*)writer)->file, offset, whence);
}

static long mod_music_MYMOD_FILE_Writer_Tell(MF_WRITER* writer)
{
	return ftell(((MFILEWRITER*)writer)->file);
}

static MM_BOOL mod_music_MYMOD_FILE_Writer_Write(MF_WRITER* writer, void* ptr, size_t size)
{
	return (fwrite(ptr,size,1,((MFILEWRITER*)writer)->file)==size);
}

static MM_BOOL mod_music_MYMOD_FILE_Writer_Put(MF_WRITER* writer, int value)
{
	return fputc(value,((MFILEWRITER*)writer)->file);
}

MF_WRITER *mod_music_new_file_writer(FILE* fp)
{
	MFILEWRITER* writer=(MFILEWRITER*)mod_music_malloc(sizeof(MFILEWRITER));
	if (writer)
	{
		writer->core.Seek	= &mod_music_MYMOD_FILE_Writer_Seek;
		writer->core.Tell	= &mod_music_MYMOD_FILE_Writer_Tell;
		writer->core.Write	= &mod_music_MYMOD_FILE_Writer_Write;
		writer->core.Put	= &mod_music_MYMOD_FILE_Writer_Put;
		writer->file		= fp;
	}
	return ((MF_WRITER*) writer);
}

void mod_music_delete_file_writer(MF_WRITER* writer)
{
	if (writer) 	{free (writer);}
}

/*---------- Write functions */

void mod_music_write_string(MF_WRITER* writer, MM_CHAR *data )
{
	if (data)
	{	mod_music_write_multi_u8(writer, data, strlen(data) );}
}

void mod_music_write_u16BE(MF_WRITER* writer, u16 data )
{
	mod_music_write_u8(writer, data>>8 );
	mod_music_write_u8(writer, data&0xff );
}

void mod_music_write_u16LE(MF_WRITER* writer, u16 data )
{
	mod_music_write_u8(writer, data&0xff );
	mod_music_write_u8(writer, data>>8 );
}

void mod_music_write_u32BE(MF_WRITER* writer, u32 data )
{
	mod_music_write_u16BE(writer, data>>16 );
	mod_music_write_u16BE(writer, data&0xffff );
}

void mod_music_write_u32LE(MF_WRITER* writer, u32 data )
{
	mod_music_write_u16LE(writer, data&0xffff );
	mod_music_write_u16LE(writer, data>>16 );
}

void mod_music_write_s16BE(MF_WRITER* writer, s16 data )
{
	mod_music_write_u16BE(writer, (u16)data );
}

void mod_music_write_s16LE(MF_WRITER* writer, s16 data )
{
	mod_music_write_u16LE(writer, (u16)data );
}

void mod_music_write_s32BE(MF_WRITER* writer, s32 data )
{
	mod_music_write_u32BE(writer, (u32)data );
}

void mod_music_write_s32LE(MF_WRITER* writer, s32 data)
{
	mod_music_write_u32LE(writer, (u32)data );
}

#ifdef __STDC__
#define DEFINE_MULTIPLE_WRITE_FUNCTION(type_name,type)							\
void mod_music_write_multi_##type_name(MF_WRITER* writer, type *buffer, int number) 	\
{																				\
	while (number-->0)															\
	{	mod_music_write_##type_name(writer, *(buffer++));		}						\
}
#else
#define DEFINE_MULTIPLE_WRITE_FUNCTION(type_name,type)							\
void mod_music_write_multi_/**/type_name(MF_WRITER* writer, type *buffer, int number)	\
{																				\
	while (number-->0)															\
	{	mod_music_write_/**/type_name(writer, *(buffer++)); 	}					\
}
#endif

DEFINE_MULTIPLE_WRITE_FUNCTION(s16BE, s16)
DEFINE_MULTIPLE_WRITE_FUNCTION(u16BE, u16)
DEFINE_MULTIPLE_WRITE_FUNCTION(s16LE, s16)
DEFINE_MULTIPLE_WRITE_FUNCTION(u16LE, u16)

DEFINE_MULTIPLE_WRITE_FUNCTION(s32BE, s32)
DEFINE_MULTIPLE_WRITE_FUNCTION(u32BE, u32)
DEFINE_MULTIPLE_WRITE_FUNCTION(s32LE, s32)
DEFINE_MULTIPLE_WRITE_FUNCTION(u32LE, u32)
#endif /* USE_MIKMOD_ALONE */

/* ex:set ts=4: */
