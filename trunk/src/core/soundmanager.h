/*
 * $Id: soundmanager.h,v 1.1.1.1 2002/11/03 11:08:24 kenta Exp $
 *
 * Copyright 2002 Kenta Cho. All rights reserved.
 */

/**
 * BGM/SE manager header file.
 *
 * @version $Revision: 1.1.1.1 $
 */
#ifndef _BGM_VOICE_H_
#define _BGM_VOICE_H_

extern void closeSound(void);
extern void initSound(void);
//extern void playMusic(char *play_music_file_name);
extern void fadeMusic(void);
//extern void stopMusic(void);
extern void pauseMusic(void);
extern void resumeMusic(void);
extern void playChunk(int req_idx);
extern void setChunkVolume(int volume);
//
extern void voice_play_vbl(void);
extern void play_music(int num);
void voice_play(int req, int play_track );
enum
{
	BGM_00_intro = 0,
	BGM_01_stage1,
	BGM_02_stage2,
	BGM_03_stage3,
	BGM_04_stage4,
	BGM_05_stage5,
	BGM_06_stage6,
	BGM_07_stage7,
	BGM_08_menu2,
	BGM_09_boss1,
	BGM_10_boss2,
	BGM_11_boss3,
	BGM_12_boss4,
	BGM_13_boss5,
	BGM_14_boss6,
	BGM_15_boss7,
};
enum
{
	TRACK00_BULLETS = 0,
	TRACK01_EXPLODE,
	TRACK02_ALEART_IVENT,
	TRACK03_SHORT_MUSIC,
	TRACK04,
	TRACK05,
	TRACK06,
	TRACK07
};
#endif
