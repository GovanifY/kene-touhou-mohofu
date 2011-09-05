
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
---------------------------------------------------------*/

/*
	SDL_mixer:	An audio mixer library based on the SDL library
	Copyright (C) 1997-2009 Sam Lantinga

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library; if not, write to the Free
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	Sam Lantinga
	slouken@libsdl.org
*/

#ifdef MP3_MAD_MUSIC

#include <mad.h>//#include "mad.h"
//#include <SD L/SDL_rwops.h>//#include "SDL_rwops.h"
//#include <SD L/SDL_audio.h>//#include "SDL_audio.h"
//#include <SD L/SD L_mixer.h>//#include "SD L_mixer.h"

/*
-----------------------
cygwin �̏ꍇ�A
C:/cygwin/pspdev/psp/include/mad.h
���K�v����B(�����ꍇ������Ώo�Ă���B)
-----------------------
Minimalist PSP homebrew SDK for Windows. �̏ꍇ�A
C:/pspsdk/psp/include/mad.h
���K�v����B(��������ĂȂ��ꍇ�́A�o�[�W������V��������Γ����Ă�B)
-----------------------

(r35����)�g�p���Ă���libmad�̃o�[�W�����́B

-----------------------
 * libmad - MPEG audio decoder library
 * Copyright (C) 2000-2004 Underbit Technologies, Inc.
-----------------------
# define MAD_VERSION_MAJOR	0
# define MAD_VERSION_MINOR	15
# define MAD_VERSION_PATCH	1
# define MAD_VERSION_EXTRA	" (beta)"

# define MAD_PUBLISHYEAR	"2000-2004"
# define MAD_AUTHOR		"Underbit Technologies, Inc."
# define MAD_EMAIL		"info@underbit.com"
-----------------------
�������B
-----------------------
version 0.15.1(beta) (C)2000-2004 Underbit Technologies, Inc.
-----------------------
libmad �� �\�[�X���~�����ꍇ�B(r35����)����͂ƂĂ��ȒP�����炮�����Ă���B

 */

//#define MAD_INPUT_BUFFER_SIZE 	(5*8192)
//#define MAD_INPUT_BUFFER_SIZE 	(8*8192)
//#define MAD_OUT PUT_BUFFER_SIZE	(8192)
#define MAD_INPUT_BUFFER_SIZE		(4*1024)
//#define MAD_OUT PUT_BUFFER_SIZE	(1024)

enum
{
	MS_input_eof		= 0x0001,
	MS_input_error		= 0x0001,
	MS_decode_eof		= 0x0002,
	MS_decode_error 	= 0x0004,
	MS_error_flags		= 0x000f,

	MS_playing			= 0x0100,
	MS_cvt_decoded		= 0x0200,
};

typedef struct
{
	SDL_RWops *rw_file_pos_dummy;
	SDL_bool			freerw;
	struct				mad_stream stream;
	struct				mad_frame frame;
	struct				mad_synth synth;
	int 				frames_read;
	mad_timer_t 		next_frame_start;
	unsigned int		volume;
	int 				status;
	int 				output_begin, output_end;
	PSPL_AUDIO_SPEC 		mixer;
	PSPL_AUDIO_CONVERT		cvt;
//
	unsigned int madfile_file_size;
	unsigned int madfile_file_seek;
	char *madfile_malloc_buf;
//
	unsigned char input_buffer[MAD_INPUT_BUFFER_SIZE + MAD_BUFFER_GUARD];
	unsigned char output_buffer[(8192)/*MAD_OUT PUT_BUFFER_SIZE*/];
} mad_data;

mad_data *mad_openFile(const char *filename, PSPL_AUDIO_SPEC *mixer);
//�͕핗�ł͔�Ή� mad_data *ma d_openFileRW(SDL_RWops *rw, PSPL_AUDIO_SPEC *mixer);
void mad_closeFile(mad_data *mp3_mad);

void mad_start(mad_data *mp3_mad);
void mad_stop(mad_data *mp3_mad);
int mad_isPlaying(mad_data *mp3_mad);

int mad_getSamples(mad_data *mp3_mad, u8 *stream, int len);
void mad_seek_int(mad_data *mp3_mad, /*dou ble*/int position);
void mad_setVolume(mad_data *mp3_mad, unsigned int volume);

#endif
