
/* The internal format for an audio chunk */
typedef struct
{
	int allocated;
	u8 *abuf;
	u32 alen;
	u8 volume;		/* Per-sample volume, 0-128 */
} Mix_Chunk;

/* The internal format for a music chunk interpreted via mikmod */
typedef struct the_Mix_Music_t Mix_Music;




/* Open the mixer with a certain audio format */
extern int /*SD LCALL*/ Mix_OpenAudio(int frequency, u16 format, int channels, int chunksize);

#if 00


/* Dynamically change the number of channels managed by the mixer.
   If decreasing the number of channels, the upper channels are
   stopped.
   This function returns the new number of allocated channels.
 */
/*�͕핗�Ŏg�p���Ă��Ȃ�*/ //extern int /*SD LCALL*/ Mix_AllocateChannels(int numchans);

/* Find out what the actual audio device parameters are.
   This function returns 1 if the audio has been opened, 0 otherwise.
 */
/*�͕핗�Ŏg�p���Ă��Ȃ�*/ //extern int /*SD LCALL*/ Mix_QuerySpec(int *frequency, u16 *format, int *channels);

#endif /*00*/
/* Load a wave file or a music (.mod .s3m .it .xm) file */
extern Mix_Chunk * /*SD LCALL*/ Mix_LoadWAV_RW(SDL_RWops *src, int freesrc);
//#define Mix_LoadWAV(file)	Mix_LoadWAV_RW(SDL_RWFromFile(file, "rb"), 1)
extern Mix_Music * /*SD LCALL*/ Mix_LoadMUS(const char *file);

#if 00
//#if 0 //def US E_RWOPS /* This hasn't been hooked into music.c yet */
//	/* Load a music file from an SDL_RWop object (MusicMod-specific currently)
//	   Matt Campbell (matt@campbellhome.dhs.org) April 2000 */
//extern Mix_Music * /*SD LCALL*/ Mix_LoadMUS_RW(SDL_RWops *rw);
//#endif

/* Load a wave file of the mixer format from a memory buffer */
/*�͕핗�Ŏg�p���Ă��Ȃ�*/ //extern Mix_Chunk * /*SD LCALL*/ Mix_QuickLoad_WAV(u8 *mem);

/* Load raw audio data of the mixer format from a memory buffer */
/*�͕핗�Ŏg�p���Ă��Ȃ�*/ //extern Mix_Chunk * /*SD LCALL*/ Mix_QuickLoad_RAW(u8 *mem, u32 len);

#endif /*00*/
/* Free an audio chunk previously loaded */
extern void /*SD LCALL*/ Mix_FreeChunk(Mix_Chunk *chunk);
extern void /*SD LCALL*/ Mix_FreeMusic(Mix_Music *music);

#if 00
/* Find out the music format of a mixer music, or the currently playing
   music, if 'music' is NULL.
*/
extern Mix_MusicType /*SD LCALL*/ Mix_GetMusicType(const Mix_Music *music);

/* Set a function that is called after all mixing is performed.
   This can be used to provide real-time visual display of the audio stream
   or add a custom mixer filter for the stream data.
*/
extern void /*SD LCALL*/ Mix_SetPostMix(void (*mix_func) (void *udata, u8 *stream, int len), void *arg);


/* Add your own music player or additional mixer function.
   If 'mix_func' is NULL, the default music player is re-enabled.
 */
/*[�p�~]�t�b�N���Ȃ�*/ //extern void /*SD LCALL*/ Mix_HookMusic(void (*mix_func) (void *udata, u8 *stream, int len), void *arg);

/* Add your own callback when the music has finished playing.
   This callback is only called if the music finishes naturally.
 */
/*[�p�~]�t�b�N���Ȃ�*/ //extern void /*SD LCALL*/ Mix_HookMusicFinished(void (*music_finished)(void));

/* Get a pointer to the user data for the current music hook */
/*[�p�~]�t�b�N���Ȃ�*/ //extern void * /*SD LCALL*/ Mix_GetMusicHookData(void);

/*
 * Add your own callback when a channel has finished playing. NULL
 *  to disable callback. The callback may be called from the mixer's audio
 *  callback or it could be called as a result of Mix_HaltChannel(), etc.
 *  do not call PSPL_LockAudio() from this callback; you will either be
 *  inside the audio callback, or SDL_mixer will explicitly lock the audio
 *  before calling your callback.
 */
/*�͕핗�Ŏg�p���Ă��Ȃ�*/ //extern void /*SD LCALL*/ Mix_ChannelFinished(void (*channel_finished)(int channel));


/* Special Effects API by ryan c. gordon. (icculus@linuxgames.com) */

///////////////////////

/* You may not need to call this explicitly, unless you need to stop all
 *  effects from processing in the middle of a chunk's playback. Note that
 *  this will also shut off some internal effect processing, since
 *  Mix_SetPanning() and others may use this API under the hood. This is
 *  called internally when a channel completes playback.
 * Posteffects are never implicitly unregistered as they are for channels,
 *  but they may be explicitly unregistered through this function by
 *  specifying MIX_CHANNEL_POST for a channel.
 * returns zero if error (no such channel), nonzero if all effects removed.
 *  Error messages can be retrieved from Mix_GetError().
 */
extern int /*SD LCALL*/ Mix_UnregisterAllEffects(int channel);


#define MIX_EFFECTSMAXSPEED  "MIX_EFFECTSMAXSPEED"

/*
 * These are the internally-defined mixing effects. They use the same API that
 *  effects defined in the application use, but are provided here as a
 *  convenience. Some effects can reduce their quality or use more memory in
 *  the name of speed; to enable this, make sure the environment variable
 *  MIX_EFFECTSMAXSPEED (see above) is defined before you call
 *  Mix_OpenAudio().
 */


/* Set the panning of a channel. The left and right channels are specified
 *  as integers between 0 and 255, quietest to loudest, respectively.
 *
 * Technically, this is just individual volume control for a sample with
 *  two (stereo) channels, so it can be used for more than just panning.
 *  If you want real panning, call it like this:
 *
 *   Mix_SetPanning(channel, left, 255 - left);
 *
 * ...which isn't so hard.
 *
 * Setting (channel) to MIX_CHANNEL_POST registers this as a posteffect, and
 *  the panning will be done to the final mixed stream before passing it on
 *  to the audio device.
 *
 * This uses the Mix_RegisterEffect() API internally, and returns without
 *  registering the effect function if the audio device is not configured
 *  for stereo output. Setting both (left) and (right) to 255 causes this
 *  effect to be unregistered, since that is the data's normal state.
 *
 * returns zero if error (no such channel or Mix_RegisterEffect() fails),
 *  nonzero if panning effect enabled. Note that an audio device in mono
 *  mode is a no-op, but this call will return successful in that case.
 *  Error messages can be retrieved from Mix_GetError().
 */
extern int /*SD LCALL*/ Mix_SetPanning(int channel, u8 left, u8 right);


/* Set the position of a channel. (angle) is an integer from 0 to 360, that
 *  specifies the location of the sound in relation to the listener. (angle)
 *  will be reduced as neccesary (540 becomes 180 degrees, -100 becomes 260).
 *  Angle 0 is due north, and rotates clockwise as the value increases.
 *  For efficiency, the precision of this effect may be limited (angles 1
 *  through 7 might all produce the same effect, 8 through 15 are equal, etc).
 *  (distance) is an integer between 0 and 255 that specifies the space
 *  between the sound and the listener. The larger the number, the further
 *  away the sound is. Using 255 does not guarantee that the channel will be
 *  culled from the mixing process or be completely silent. For efficiency,
 *  the precision of this effect may be limited (distance 0 through 5 might
 *  all produce the same effect, 6 through 10 are equal, etc). Setting (angle)
 *  and (distance) to 0 unregisters this effect, since the data would be
 *  unchanged.
 *
 * If you need more precise positional audio, consider using OpenAL for
 *  spatialized effects instead of SDL_mixer. This is only meant to be a
 *  basic effect for simple "3D" games.
 *
 * If the audio device is configured for mono output, then you won't get
 *  any effectiveness from the angle; however, distance attenuation on the
 *  channel will still occur. While this effect will function with stereo
 *  voices, it makes more sense to use voices with only one channel of sound,
 *  so when they are mixed through this effect, the positioning will sound
 *  correct. You can convert them to mono through SDL before giving them to
 *  the mixer in the first place if you like.
 *
 * Setting (channel) to MIX_CHANNEL_POST registers this as a posteffect, and
 *  the positioning will be done to the final mixed stream before passing it
 *  on to the audio device.
 *
 * This is a convenience wrapper over Mix_SetDistance() and Mix_SetPanning().
 *
 * returns zero if error (no such channel or Mix_RegisterEffect() fails),
 *  nonzero if position effect is enabled.
 *  Error messages can be retrieved from Mix_GetError().
 */
extern int /*SD LCALL*/ Mix_SetPosition(int channel, s16 angle, u8 distance);


/* Set the "distance" of a channel. (distance) is an integer from 0 to 255
 *  that specifies the location of the sound in relation to the listener.
 *  Distance 0 is overlapping the listener, and 255 is as far away as possible
 *  A distance of 255 does not guarantee silence; in such a case, you might
 *  want to try changing the chunk's volume, or just cull the sample from the
 *  mixing process with Mix_HaltChannel().
 * For efficiency, the precision of this effect may be limited (distances 1
 *  through 7 might all produce the same effect, 8 through 15 are equal, etc).
 *  (distance) is an integer between 0 and 255 that specifies the space
 *  between the sound and the listener. The larger the number, the further
 *  away the sound is.
 * Setting (distance) to 0 unregisters this effect, since the data would be
 *  unchanged.
 * If you need more precise positional audio, consider using OpenAL for
 *  spatialized effects instead of SDL_mixer. This is only meant to be a
 *  basic effect for simple "3D" games.
 *
 * Setting (channel) to MIX_CHANNEL_POST registers this as a posteffect, and
 *  the distance attenuation will be done to the final mixed stream before
 *  passing it on to the audio device.
 *
 * This uses the Mix_RegisterEffect() API internally.
 *
 * returns zero if error (no such channel or Mix_RegisterEffect() fails),
 *  nonzero if position effect is enabled.
 *  Error messages can be retrieved from Mix_GetError().
 */
extern int /*SD LCALL*/ Mix_SetDistance(int channel, u8 distance);


/*
 * !!! FIXME : Haven't implemented, since the effect goes past the
 *              end of the sound buffer. Will have to think about this.
 *               --ryan.
 */
#if 0
/* Causes an echo effect to be mixed into a sound. (echo) is the amount
 *  of echo to mix. 0 is no echo, 255 is infinite (and probably not
 *  what you want).
 *
 * Setting (channel) to MIX_CHANNEL_POST registers this as a posteffect, and
 *  the reverbing will be done to the final mixed stream before passing it on
 *  to the audio device.
 *
 * This uses the Mix_RegisterEffect() API internally. If you specify an echo
 *  of zero, the effect is unregistered, as the data is already in that state.
 *
 * returns zero if error (no such channel or Mix_RegisterEffect() fails),
 *  nonzero if reversing effect is enabled.
 *  Error messages can be retrieved from Mix_GetError().
 */
extern no_parse_DEC LSPEC int /*SD LCALL*/ Mix_SetReverb(int channel, u8 echo);
#endif

/* Causes a channel to reverse its stereo. This is handy if the user has his
 *  speakers hooked up backwards, or you would like to have a minor bit of
 *  psychedelia in your sound code.  :)  Calling this function with (flip)
 *  set to non-zero reverses the chunks's usual channels. If (flip) is zero,
 *  the effect is unregistered.
 *
 * This uses the Mix_RegisterEffect() API internally, and thus is probably
 *  more CPU intensive than having the user just plug in his speakers
 *  correctly. Mix_SetReverseStereo() returns without registering the effect
 *  function if the audio device is not configured for stereo output.
 *
 * If you specify MIX_CHANNEL_POST for (channel), then this the effect is used
 *  on the final mixed stream before sending it on to the audio device (a
 *  posteffect).
 *
 * returns zero if error (no such channel or Mix_RegisterEffect() fails),
 *  nonzero if reversing effect is enabled. Note that an audio device in mono
 *  mode is a no-op, but this call will return successful in that case.
 *  Error messages can be retrieved from Mix_GetError().
 */
extern int /*SD LCALL*/ Mix_SetReverseStereo(int channel, int flip);

/* end of effects API. --ryan. */


/* Reserve the first channels (0 -> n-1) for the application, i.e. don't allocate
   them dynamically to the next sample if requested with a -1 value below.
   Returns the number of reserved channels.
 */
/*�͕핗�Ŏg�p���Ă��Ȃ�*/ //extern int /*SD LCALL*/ Mix_ReserveChannels(int num);

/* Channel grouping functions */

/* Attach a tag to a channel. A tag can be assigned to several mixer
   channels, to form groups of channels.
   If 'tag' is -1, the tag is removed (actually -1 is the tag used to
   represent the group of all the channels).
   Returns true if everything was OK.
 */
extern int /*SD LCALL*/ Mix_GroupChannel(int which, int tag);
/* Assign several consecutive channels to a group */
extern int /*SD LCALL*/ Mix_GroupChannels(int from, int to, int tag);
/* Finds the first available channel in a group of channels,
   returning -1 if none are available.
 */
extern int /*SD LCALL*/ Mix_GroupAvailable(int tag);
/* Returns the number of channels in a group. This is also a subtle
   way to get the total number of channels when 'tag' is -1
 */
extern int /*SD LCALL*/ Mix_GroupCount(int tag);
/* Finds the "oldest" sample playing in a group of channels */
extern int /*SD LCALL*/ Mix_GroupOldest(int tag);
/* Finds the "most recent" (i.e. last) sample playing in a group of channels */
extern int /*SD LCALL*/ Mix_GroupNewer(int tag);

/* Play an audio chunk on a specific channel.
   If the specified channel is -1, play on the first free channel.
   If 'loops' is greater than zero, loop the sound that many times.
   If 'loops' is -1, loop inifinitely (~65000 times).
   Returns which channel was used to play the sound.
*/
#define Mix_PlayChannel(channel,chunk,loops) Mix_PlayChannelTimed(channel,chunk,loops,-1)
/* The same as above, but the sound is played at most 'ticks' milliseconds */
extern int /*SD LCALL*/ Mix_PlayChannelTimed(int channel, Mix_Chunk *chunk, int loops, int ticks);
extern int /*SD LCALL*/ Mix_PlayMusic(Mix_Music *music, int loops);

/* Fade in music or a channel over "ms" milliseconds, same semantics as the "Play" functions */
extern int /*SD LCALL*/ Mix_FadeInMusic(Mix_Music *music, int loops, int ms);
extern int /*SD LCALL*/ Mix_FadeInMusicPos(Mix_Music *music, int loops, int ms, double position);
/*�͕핗�Ŏg�p���Ă��Ȃ�*/ //extern int /*SD LCALL*/ Mix_FadeInChannelTimed(int channel, Mix_Chunk *chunk, int loops, int ms, int ticks);
/*�͕핗�Ŏg�p���Ă��Ȃ�*/ //#define Mix_FadeInChannel(channel,chunk,loops,ms) Mix_FadeInChannelTimed(channel,chunk,loops,ms,-1)

/* Set the volume in the range of 0-128 of a specific channel or chunk.
   If the specified channel is -1, set volume for all channels.
   Returns the original volume.
   If the specified volume is -1, just return the current volume.
*/
extern int /*SD LCALL*/ Mix_Volume(int channel, int volume);
/*�͕핗�Ŏg�p���Ă��Ȃ�*/ //extern int /*SD LCALL*/ Mix_VolumeChunk(Mix_Chunk *chunk, int volume);

#endif /*00*/
extern int /*SD LCALL*/ Mix_VolumeMusic(int volume);
#if 00

/* Halt playing of a particular channel */
extern int /*SD LCALL*/ Mix_HaltChannel(int channel);
/*�͕핗�Ŏg�p���Ă��Ȃ�*/ //extern int /*SD LCALL*/ Mix_HaltGroup(int tag);

#endif /*00*/
extern int /*SD LCALL*/ Mix_HaltMusic(void);
#if 00
/* Change the expiration delay for a particular channel.
   The sample will stop playing after the 'ticks' milliseconds have elapsed,
   or remove the expiration if 'ticks' is -1
*/
/*�͕핗�Ŏg�p���Ă��Ȃ�*/ //extern int /*SD LCALL*/ Mix_ExpireChannel(int channel, int ticks);

/* Halt a channel, fading it out progressively till it's silent
   The ms parameter indicates the number of milliseconds the fading
   will take.
 */
/*�͕핗�Ŏg�p���Ă��Ȃ�*/ //extern int /*SD LCALL*/ Mix_FadeOutChannel(int which, int ms);
/*�͕핗�Ŏg�p���Ă��Ȃ�*/ //extern int /*SD LCALL*/ Mix_FadeOutGroup(int tag, int ms);
extern int /*SD LCALL*/ Mix_FadeOutMusic(int ms);

/* Query the fading status of a channel */
extern Mix_Fading /*SD LCALL*/ Mix_FadingMusic(void);
/*�͕핗�Ŏg�p���Ă��Ȃ�*/ //extern Mix_Fading /*SD LCALL*/ Mix_FadingChannel(int which);

/* Pause/Resume a particular channel */
extern void /*SD LCALL*/ Mix_Pause(int channel);
extern void /*SD LCALL*/ Mix_Resume(int channel);
extern int /*SD LCALL*/ Mix_Paused(int channel);

/* Pause/Resume the music stream */
extern void /*SD LCALL*/ Mix_PauseMusic(void);
extern void /*SD LCALL*/ Mix_ResumeMusic(void);
extern void /*SD LCALL*/ Mix_RewindMusic(void);
extern int /*SD LCALL*/ Mix_PausedMusic(void);

/* Set the current position in the music stream.
   This returns 0 if successful, or -1 if it failed or isn't implemented.
   This function is only implemented for MOD music formats (set pattern
   order number) and for OGG music (set position in seconds), at the
   moment.
*/
extern int /*SD LCALL*/ Mix_SetMusicPosition(double position);

/* Check the status of a specific channel.
   If the specified channel is -1, check all channels.
*/
extern int /*SD LCALL*/ Mix_Playing(int channel);
extern int /*SD LCALL*/ Mix_PlayingMusic(void);

/* Stop music and set external music playback command */
extern int /*SD LCALL*/ Mix_SetMusicCMD(const char *command);

/* Synchro value is set by MusicMod from modules while playing */
extern int /*SD LCALL*/ Mix_SetSynchroValue(int value);
extern int /*SD LCALL*/ Mix_GetSynchroValue(void);

/* Get the Mix_Chunk currently associated with a mixer channel
    Returns NULL if it's an invalid channel, or there's no chunk associated.
*/
extern Mix_Chunk * /*SD LCALL*/ Mix_GetChunk(int channel);

#endif /*00*/

/* Close the mixer, halting all playing audio */
extern void /*SD LCALL*/ Mix_CloseAudio(void);

