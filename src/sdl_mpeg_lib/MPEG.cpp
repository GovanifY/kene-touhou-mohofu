#include <SDL/SDL.h>//#include "SD L.h"

#include "MPEG.h"

#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif


#ifndef smpeg_SetError_bbb
	#define smpeg_SetError_bbb(aaa)
#endif


MPEG::MPEG(const char * name, bool SDLaudio) :
	MPEGerror()
{
	mpeg_mem = 0;
//
	SDL_RWops *source;
	source = SDL_RWFromFile(name, "rb");
	if (!source)
	{
		InitErrorState();
		smpeg_SetError_bbb(SDL_GetError());
		return;
	}
	Init(source, SDLaudio);
}

MPEG::MPEG(int Mpeg_FD, bool SDLaudio) :
	MPEGerror()
{
	mpeg_mem = 0;
//
	// *** FIXME we're leaking a bit of memory for the FILE *
	// best solution would be to have SDL_RWFromFD
	FILE *file = fdopen(Mpeg_FD, "rb");
	if (!file)
	{
		InitErrorState();
		SetError(strerror(errno));
		return;
	}
//
	SDL_RWops *source;
	source = SDL_RWFromFP(file, (SDL_bool) false);
	if (!source)
	{
		InitErrorState();
		smpeg_SetError_bbb(SDL_GetError());
		return;
	}
	Init(source, SDLaudio);
}

MPEG::MPEG(void *data, int size, bool SDLaudio) :
	MPEGerror()
{
	SDL_RWops *source;

	// The semantics are that the data passed in should be copied
	// (?)
	mpeg_mem = new char[size];
	memcpy(mpeg_mem, data, size);

	source = SDL_RWFromMem(mpeg_mem, size);
	if (!source)
	{
		InitErrorState();
		smpeg_SetError_bbb(SDL_GetError());
		return;
	}
	Init(source, SDLaudio);
}

MPEG::MPEG(SDL_RWops *mpeg_source, bool SDLaudio) :
	MPEGerror()
{
	mpeg_mem = 0;
	Init(mpeg_source, SDLaudio);
}

void MPEG::Init(SDL_RWops *mpeg_source, bool SDLaudio)
{
	source = mpeg_source;
	sdlaudio = SDLaudio;

	/* Create the system that will parse the MPEG stream */
	system = new MPEGsystem(source);

	/* Initialize everything to invalid values for cleanup */
	error = NULL;

	audiostream= NULL;
	audioaction = NULL;
	audio = NULL;
	audioaction_enabled = SDLaudio;

	#if (1==USE_MP3_VIDEO)
	videostream = NULL;
	videoaction = NULL;
	video = NULL;
	videoaction_enabled = false;
	#endif

	loop = false;
	pause = false;

	parse_stream_list();

	EnableAudio(audioaction_enabled);

	#if (1==USE_MP3_VIDEO)
	EnableVideo(videoaction_enabled);
	#endif

	if ( ! audiostream
	#if (1==USE_MP3_VIDEO)
		&& ! videostream
	#endif
	)
	{
		SetError("No audio/video stream found in MPEG");
	}

	if ( system && system->WasError() )
	{
		SetError(system->TheError());
	}

	if ( audio && audio->WasError() )
	{
		SetError(audio->TheError());
	}
	#if (1==USE_MP3_VIDEO)
	if ( video && video->WasError() )
	{
		SetError(video->TheError());
	}
	#endif
	if ( WasError() )
	{
		SetError(TheError());
	}
}

void MPEG::InitErrorState()
{
	system		= NULL;
	error		= NULL;
	source		= NULL;
//
	audio		= NULL;
	audiostream = NULL;
	audioaction = NULL;
	audioaction_enabled = false;
//
	#if (1==USE_MP3_VIDEO)
	video		= NULL;
	videostream = NULL;
	videoaction = NULL;
	videoaction_enabled = false;
	#endif
//
	loop		= false;
	pause		= false;
}

MPEG::~MPEG()
{
	Stop();
	#if (1==USE_MP3_VIDEO)
	if (video) delete video;
	#endif
	if (audio) delete audio;
	if (system) delete system;

	if (source) SDL_RWclose(source);
	if ( mpeg_mem )
	{	delete[] mpeg_mem;}
}

bool MPEG::AudioEnabled(void)
{
	return (audioaction_enabled);
}
void MPEG::EnableAudio(bool enabled)
{
	if ( enabled && ! audioaction )
	{
		enabled = false;
	}
	audioaction_enabled = enabled;

	/* Stop currently playing stream, if necessary */
	if ( audioaction && ! audioaction_enabled )
	{
		audioaction->Stop();
	}
	#if (1==USE_MP3_VIDEO)
	/* Set the video time source */
	if ( videoaction )
	{
		if ( audioaction_enabled )
		{
			videoaction->SetTimeSource(audioaction);
		}
		else
		{
			videoaction->SetTimeSource(NULL);
		}
	}
	#endif
	if (audiostream)
	{	audiostream->enable(enabled);}
}

#if (1==USE_MP3_VIDEO)
bool MPEG::VideoEnabled(void)
{
	return (videoaction_enabled);
}
void MPEG::EnableVideo(bool enabled)
{
	if ( enabled && ! videoaction )
	{
		enabled = false;
	}
	videoaction_enabled = enabled;

	/* Stop currently playing stream, if necessary */
	if ( videoaction && ! videoaction_enabled )
	{
		videoaction->Stop();
	}
	if (videostream)
	{	videostream->enable(enabled);}
}
#endif

/* MPEG actions */
void MPEG::Loop(bool toggle)
{
	loop = toggle;
}
void MPEG::Play(void)
{
	if ( AudioEnabled() )
	{
		audioaction->Play();
	}
	#if (1==USE_MP3_VIDEO)
	if ( VideoEnabled() )
	{
		videoaction->Play();
	}
	#endif
}
void MPEG::Stop(void)
{
	#if (1==USE_MP3_VIDEO)
	if ( VideoEnabled() )
	{
		videoaction->Stop();
	}
	#endif
	if ( AudioEnabled() )
	{
		audioaction->Stop();
	}
}

void MPEG::Rewind(void)
{
	seekIntoStream(0);
}

void MPEG::Pause(void)
{
	pause = !pause;
	#if (1==USE_MP3_VIDEO)
	if ( VideoEnabled() )
	{
		videoaction->Pause();
	}
	#endif
	if ( AudioEnabled() )
	{
		audioaction->Pause();
	}
}

/* Michel Darricau from eProcess <mdarricau@eprocess.fr> conflict name with popcorn */
MPEGstatus MPEG::GetStatus(void)
{
	MPEGstatus status;

	status = MPEG_STOPPED;
	#if (1==USE_MP3_VIDEO)
	if ( VideoEnabled() )
	{
		/* Michel Darricau from eProcess <mdarricau@eprocess.fr> conflict name with popcorn */
		switch (videoaction->GetStatus())
		{
		case MPEG_PLAYING:
			status = MPEG_PLAYING;
			break;
		default:
			break;
		}
	}
	#endif
	if ( AudioEnabled() )
	{
		/* Michel Darricau from eProcess <mdarricau@eprocess.fr> conflict name with popcorn */
		switch (audioaction->GetStatus())
		{
		case MPEG_PLAYING:
			status = MPEG_PLAYING;
			break;
		default:
			break;
		}
	}

	if (status == MPEG_STOPPED && loop && !pause)
	{
		/* Here we go again */
		Rewind();
		Play();
		#if (1==USE_MP3_VIDEO)
		if ( VideoEnabled() )
		{
		/* Michel Darricau from eProcess <mdarricau@eprocess.fr> conflict name with popcorn */
			switch (videoaction->GetStatus())
			{
			case MPEG_PLAYING:
				status = MPEG_PLAYING;
				break;
			default:
				break;
			}
		}
		#endif
		if ( AudioEnabled() )
		{
		/* Michel Darricau from eProcess <mdarricau@eprocess.fr> conflict name with popcorn */
			switch (audioaction->GetStatus())
			{
			case MPEG_PLAYING:
				status = MPEG_PLAYING;
				break;
			default:
				break;
			}
		}
	}
	return (status);
}


/* MPEG audio actions */
bool MPEG::GetAudioInfo(MPEG_AudioInfo *info)
{
	if ( AudioEnabled() )
	{
		return (audioaction->GetAudioInfo(info));
	}
	return (false);
}
//void MPEG::Volume(int vol)
//{
//	if ( AudioEnabled() )
//	{
//		audioaction->Volume(vol);
//	}
//}
bool MPEG::WantedSpec(PSPL_AUDIO_SPEC *wanted)
{
	if ( audiostream )
	{
		return (GetAudio()->WantedSpec(wanted));
	}
	return (false);
}
void MPEG::ActualSpec(const PSPL_AUDIO_SPEC *actual)
{
	if ( audiostream )
	{
		GetAudio()->ActualSpec(actual);
	}
}
MPEGaudio *MPEG::GetAudio(void) 	// Simple accessor used in the C interface
{
	return (audio);
}

#if (1==USE_MP3_VIDEO)
/* MPEG video actions */
bool MPEG::GetVideoInfo(MPEG_VideoInfo *info)
{
	if ( VideoEnabled() )
	{
		return (videoaction->GetVideoInfo(info));
	}
	return (false);
}
bool MPEG::SetDisplay(SDL_Surface *dst, SDL_mutex *lock,
		MPEG_DisplayCallback callback)
{
	if ( VideoEnabled() )
	{
		return (videoaction->SetDisplay(dst, lock, callback));
	}
	return (false);
}
void MPEG::MoveDisplay(int x, int y)
{
	if ( VideoEnabled() )
	{
		videoaction->MoveDisplay(x, y);
	}
}
void MPEG::ScaleDisplayXY(int w, int h)
{
	if ( VideoEnabled() )
	{
		videoaction->ScaleDisplayXY(w, h);
	}
}
void MPEG::SetDisplayRegion(int x, int y, int w, int h)
{
	if ( VideoEnabled() )
	{
		videoaction->SetDisplayRegion(x, y, w, h);
	}
}
void MPEG::RenderFrame(int frame)
{
	if ( VideoEnabled() )
	{
		videoaction->RenderFrame(frame);
	}
}
void MPEG::RenderFinal(SDL_Surface *dst, int x, int y)
{
	Stop();
	if ( VideoEnabled() )
	{
		videoaction->RenderFinal(dst, x, y);
	}
	Rewind();
}

SMPEG_Filter * MPEG::Filter(SMPEG_Filter * filter)
{
	if ( VideoEnabled() )
	{
		return (videoaction->Filter(filter));
	}
	return (0);
}
#endif

void MPEG::Seek(int position)
{
	int was_playing = 0;

	/* Cannot seek past end of file */
	if ((u32)position > system->TotalSize()) return;

	/* Michel Darricau from eProcess <mdarricau@eprocess.fr> conflict name with popcorn */
	/* get info whrether we need to restart playing at the end */
	if ( GetStatus() == MPEG_PLAYING )
		was_playing = 1;

	if (!seekIntoStream(position)) return;

	/* If we were playing and not rewind then play again */
	if (was_playing)
	{	Play();}
	#if (1==USE_MP3_VIDEO)
	if (VideoEnabled() && !was_playing)
	{	videoaction->RenderFrame(0);}

	if ( pause && VideoEnabled() )
	{
		videoaction->Pause();
	}
	#endif
	if ( pause && AudioEnabled() )
	{
		audioaction->Pause();
	}
}

bool MPEG::seekIntoStream(int position)
{
	/* First we stop everything */
	Stop();

	/* Go to the desired position into file */
	if (!system->Seek(position)) return (false);

	/* Seek first aligned data */
	if (audiostream && audioaction_enabled)
		while (audiostream->time() == -1)
			if ( ! audiostream->next_packet() ) return (false);
	#if (1==USE_MP3_VIDEO)
	 if (videostream && videoaction_enabled)
		while (videostream->time() == -1)
			if ( ! videostream->next_packet() ) return (false);
	#endif

	/* Calculating current play time on audio only makes sense when there
		 is no video */
	if ( audioaction
	#if (1==USE_MP3_VIDEO)
	&& !videoaction
	#endif
	)
	{
		audioaction->Rewind();
		audioaction->ResetSynchro(((/*dou ble*/float)(system->TimeElapsedAudio(position))));
	}
	/* And forget what we previouly buffered */
	else if ( audioaction )
	{
		audioaction->Rewind();
		audioaction->ResetSynchro((/*dou ble*/float)audiostream->time());
	}
	#if (1==USE_MP3_VIDEO)
	if ( videoaction )
	{
		videoaction->Rewind();
		videoaction->ResetSynchro((/*dou ble*/float)videostream->time());
	}
	#endif
	return (true);
}

void MPEG::Skip(float seconds)
{
	if (system->get_stream(SYSTEM_STREAMID))
	{
		system->Skip(seconds);
	}
	else
	{
		/* No system information in MPEG */
		#if (1==USE_MP3_VIDEO)
		if ( VideoEnabled() ) videoaction->Skip(seconds);
		#endif
		if ( AudioEnabled() ) audioaction->Skip(seconds);
	}
}

void MPEG::GetSystemInfo(MPEG_SystemInfo * sinfo)
{
	sinfo->total_size		= system->TotalSize();
	sinfo->current_offset	= system->Tell();
	sinfo->total_time		= system->TotalTime();

	/* Get current time from audio or video decoder */
	/* TODO: move timing reference in MPEGsystem	*/
	sinfo->current_time = 0;
	#if (1==USE_MP3_VIDEO)
	if ( videoaction )
	{	sinfo->current_time = videoaction->Time();}
	#endif
	if ( audioaction )
	{	sinfo->current_time = audioaction->Time();}
}

void MPEG::parse_stream_list()
{
	/* A new thread is created for each video and audio */
	/* stream											*/
	/* TODO: support MPEG systems containing more than	*/
	/*		 one audio or video stream					*/
	MPEGstream ** stream_list;
	register int i;
	i = 0;
	do
	{
		/* Retreive the list of streams */
		stream_list = system->GetStreamList();

		switch(stream_list[i]->streamid)
		{
		case SYSTEM_STREAMID:
			break;

		case AUDIO_STREAMID:
			audiostream = stream_list[i];
			audioaction_enabled = true;
			audiostream->next_packet();
			audio = new MPEGaudio(audiostream, sdlaudio);
			audioaction = audio;
			break;
		#if (1==USE_MP3_VIDEO)
		case VIDEO_STREAMID:
			videostream = stream_list[i];
			videoaction_enabled = true;
			videostream->next_packet();
			video = new MPEGvideo(videostream);
			videoaction = video;
			break;
		#endif
		}
		i++;
	}
	while (stream_list[i]);
}
