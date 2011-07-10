
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�^�C�g�����j���[
	-------------------------------------------------------
	���ہA���݂̋��ʃ��j���[�͎g���ɂ������A
	�߂�Ȃ��Ƃ����v���I�Ȍ��ׂ�����܂����A
	����ł��g���Ă�̂́A�u���̂��߂�ǂ������v���u��ɑ��ɂ�鎖���������񂠂�v
	����ł��B
	���̂����A�S�p���ĉ��ǂ���\��ł��B
	�������炢���āAGu���������Ă���̕������Ղ��ł��B
	�Ƃ�����SDL�ō���Ă��Q�x���(��蒼��)�ɂȂ�B
---------------------------------------------------------*/

#include "kanji_system.h"

#define USE_REPLAY_MODE 	(0)/*(���쐬)*/
/*---------------------------------------------------------

---------------------------------------------------------*/
#define USE_SDL_GAZO		(0)
enum
{
	MAIN_MENU_00_START = 0,
	MAIN_MENU_01_EXTRA_START,
	MAIN_MENU_02_PRACTICE_START,
	#if (1==USE_REPLAY_MODE)/*(���쐬)*/
	MAIN_MENU_03_REPLAY,
	#else
	MAIN_MENU_03_STORY,
	#endif
	MAIN_MENU_04_RESULT,
	MAIN_MENU_05_MUSIC_ROOM,
	MAIN_MENU_06_OPTION,
	MAIN_MENU_07_QUIT,
	MENU_ITEM_99_MAX,	/* �I�v�V�����̍��ڐ� */
};


#define MAX_MENU_ITEMS (8)
typedef struct
{
	int x256;
	int y256;
//
	int MENU_DATA_i2;		// �X�v���C�g�p(���j���[�I�����A�� x �̗h�ꕝ) */
	int MENU_DATA_i3;		// �X�v���C�g�p(�h�ꕝ�����X�ɖ߂�p) */
} MY_OBJ;


//typedef struct
//{
static MY_OBJ			menu_item_my_obj[MAX_MENU_ITEMS/*20*/]; 			/* �I�u�W�F�N�g */

static int				active_item;		/* ���݃��j���[��őI������Ă��鍀�ڔԍ� */

//
static int				menu_brite; 		/* ���j���[�̖��邳(���l) */
static int				time_out_flag;		/* -3==,  -2==, -1==���Ԑ؂�Ȃ�, 0==���Ԑ؂�, ���ԃJ�E���g��=1... */
//} MENU;
#define M1_NOT_TIME_OUT 	(-1)/* ���Ԑ؂�Ȃ��ɐݒ�(���Ԑ؂�Ȃ����[�h) */
//#define M2_STATE_TIME_OVER	(-2)/* ���Ԑ؂ꂠ�胂�[�h�Ŏ��Ԑ؂�ɂȂ������ */


//static MENU start_menu;


/*---------------------------------------------------------

---------------------------------------------------------*/
#define MAX_JI_5 (0)
//	#define TITLE_X_OFFSET	 (0)/* ���j���[�E�� */
//	#define TITLE_X_OFFSET (230)/* ���j���[���� */
typedef struct
{
	int x_offset[6];
	int y_offset[6];
	const char *str_name;
} MENU_RESOURCE;
//	int fade_out;
//	int time_out;
static MENU_RESOURCE my_menu_resource[/*8*/MAX_MENU_ITEMS+MAX_JI_5/*10*/] =
{
	//	RES00_MAIN_MENU 	//		/*const*/ char *start_menu_options[] =/*(char *)*/start_menu_options
	{	{	512,	360,	360,	360,	512,	360,  },
		{	 92,	 92,	 92,	 92,	 92,	 92,  }, // ((113) +((-1)*(21)))
		"Start",
	},
	{	{	512,	250,	250,	250,	512,	250,  },
		{	113,	113,	113,	113,	113,	113,  }, // ((113) +(0*(21)))
		"Extra Start", /* 330 */
	},
	{	{	512,	245,	245,	245,	512,	245,  },
		{	134,	134,	134,	134,	134,	134,  },//((PSP_HEIGHT272/2+40) -(((5+16))*((MENU_ITEM_99_MAX)>>1)) +(1*((5+16))))
		"Practice Start",	// "Practice Start"
	},
	#if (1==USE_REPLAY_MODE)/*(���쐬)*/
	{	{	512,	400,	400,	400,	512,	400,  },
		{	155,	155,	155,	155,	155,	155,  },//((PSP_HEIGHT272/2+40) -(((5+16))*((MENU_ITEM_99_MAX)>>1)) +(2*((5+16))))
		"Replay",			// "Replay"
	},
	#else
	{	{	512,	320,	320,	320,	512,	320,  },
		{	155,	155,	155,	155,	155,	155,  },//((PSP_HEIGHT272/2+40) -(((5+16))*((MENU_ITEM_99_MAX)>>1)) +(2*((5+16))))
		"Story",			// "Story"
	},
	#endif
	{	{	512,	285,	285,	285,	512,	285,  },
		{	176,	176,	176,	176,	176,	176,  },//((PSP_HEIGHT272/2+40) -(((5+16))*((MENU_ITEM_99_MAX)>>1)) +(3*((5+16))))
		"Result",			// "Result"
	},
	{	{	512,	310,	310,	310,	512,	310,  },
		{	197,	197,	197,	197,	197,	197,  },//((PSP_HEIGHT272/2+40) -(((5+16))*((MENU_ITEM_99_MAX)>>1)) +(4*((5+16))))
		"Music Room",		// "Music Room"
	},
	{	{	512,	335,	335,	335,	512,	335,  },
		{	218,	218,	218,	218,	218,	218,  },//((PSP_HEIGHT272/2+40) -(((5+16))*((MENU_ITEM_99_MAX)>>1)) +(5*((5+16))))
		"Option",			// "Option"
	},
	{	{	512,	310,	310,	310,	512,	310,  },
		{	239,	239,	239,	239,	239,	239,  },//((PSP_HEIGHT272/2+40) -(((5+16))*((MENU_ITEM_99_MAX)>>1)) +(6*((5+16))))
		"Quit", 			// "Quit"
	},
//	{	{	  0,	  0,	  0,	  0,	  0,	  0,  },
//		{	  0,	  0,	  0,	  0,	  0,	  0,  },//((PSP_HEIGHT272/2+40) -(((5+16))*((MENU_ITEM_99_MAX)>>1)) +(7*((5+16))))
//		NULL,	//	""
//	},
};

/*---------------------------------------------------------

---------------------------------------------------------*/

//#if 0
//	#define SPEED_04_FADE_IN	(8*fps_factor)
//	#define FPS_MENU_FACTOR30	(30*fps_factor)
//#else
//	#define SPEED_04_FADE_IN	(16)	/* 8*FPS_MENU_FACTOR */
//#endif
	#define FPS_MENU_FACTOR30	(6) 	/*4*/

/* �o�ꑬ�x�́A�������B */
	#define SPEED_04_FADE_IN	(4) 	/* 4*FPS_MENU_FACTOR */
/* �ޔ𑬓x�́A�����B */
	#define SPEED_06_FADE_OUT	(6) 	/* 6*FPS_MENU_FACTOR */

static int move_mode;
static int move_count;
/*---------------------------------------------------------
	���j���[�̕������`�悷��B�T�u���[�`���B
---------------------------------------------------------*/
static void move_menu_str(void)
{
//	move_count++;
	move_count = menu_brite;
	move_count = psp_min(move_count, 256);
	move_count = psp_max(move_count, 0);
	{
		static int angle1024;/*=0*/
		angle1024 += (FPS_MENU_FACTOR30);
		mask1024(angle1024);
		int i;
		for (i=0; i<MENU_ITEM_99_MAX; i++)
		{
			if (i==active_item) /* �I�����ꂽ */
			{
				menu_item_my_obj[i].MENU_DATA_i2 = 12;	/* ��ꕝ�� 12 �ɂ��� */
			}
			else	/* �I������ĂȂ� */
			{
				if ((menu_item_my_obj[i].MENU_DATA_i2) && (0==menu_item_my_obj[i].MENU_DATA_i3))
				{
					menu_item_my_obj[i].MENU_DATA_i2--;
					menu_item_my_obj[i].MENU_DATA_i3 = 3;
				}
			}
			if (menu_item_my_obj[i].MENU_DATA_i3)
			{	menu_item_my_obj[i].MENU_DATA_i3--;}
			int x_sa;
				x_sa  = (((my_menu_resource[i].x_offset[move_mode+move_mode  ])*(256-move_count))>>8);
				x_sa += (((my_menu_resource[i].x_offset[move_mode+move_mode+1])*(	 move_count))>>8);
				{
					int sin_value_t256; 		//	sin_value_t256 = 0;
					int cos_value_t256; 		//	cos_value_t256 = 0;
					int256_sincos1024( (((angle1024)/*&(1024-1)*/)), &sin_value_t256, &cos_value_t256);
					menu_item_my_obj[i].x256 = ((x_sa)<<8)+(((sin_value_t256))*menu_item_my_obj[i].MENU_DATA_i2);
				}
		//	menu_item_my_obj[i].y256 = (menu_item_my_obj[i].MENU_DATA_i1_256)+(si n1024((angle1024))*menu_item_my_obj[i].MENU_DATA_i2);
			menu_item_my_obj[i].y256 = ((my_menu_resource[i].y_offset[move_mode+move_mode])<<8);
		//	{
		//		menu_item_my_obj[i].menu_brite	= menu_brite;
		//	}
			#if 1
			ml_font[i].x = ((menu_item_my_obj[i].x256)>>8);/* X�ʒu */
			ml_font[i].y = ((menu_item_my_obj[i].y256)>>8);
			#endif
		}
	}
}
/*---------------------------------------------------------
	���j���[�̕������`�悷��B�T�u���[�`���B
---------------------------------------------------------*/
static void draw_menu_str(void)
{
	move_menu_str();
//	sp rite_work_SDL(SP_GROUP_PAUSE_S P_ME NU_TEXT);/*SP_GROUP_TEXTS*/
//	pause_sp rite_display();/*SP_GROUP_TEXTS*/
	/* �`�� */
	psp_pop_screen();
}

/*---------------------------------------------------------
	�^�C�g�����j���[	��������
	-------------------------------------------------------
---------------------------------------------------------*/

static void generic_menu_workMENU_STATE_03_FININSH(void)
{
	{
		SDL_SetAlpha(cb.sdl_screen[SDL_01_BACK_SCREEN], SDL_SRCALPHA, 255);
		psp_pop_screen();
	}
	{
		#if 0/*RESULT�ɂ�������v��Ȃ��Ȃ������A�����f���Ƃ��t����Ȃ炢��*/
		if (-3 == /*start_menu.*/time_out)
		{
			/* ���Ԑ؂�ŁA�����I�Ƀf���\��  / Timeout, go on and show HighScore List */
			cb.main_call_func = game_play_demonstration;
		}
		else
		#endif
		{
			void *aaa[8] =
			{
				&&case_MAIN_MENU_00_START,
				&&case_MAIN_MENU_01_EXTRA_START,
				&&case_MAIN_MENU_02_PRACTICE_START,
				#if (1==USE_REPLAY_MODE)/*(���쐬)*/
				&&case_MAIN_MENU_03_REPLAY, 		//	&&MAIN_MENU_break,
				#else
				&&case_MAIN_MENU_03_STORY,
				#endif
				&&case_MAIN_MENU_04_RESULT,
				&&case_MAIN_MENU_05_MUSIC_ROOM,
				&&case_MAIN_MENU_06_OPTION,
				&&case_MAIN_MENU_07_QUIT,
			};
			goto *aaa[(active_item&0x07)];
			{
			case_MAIN_MENU_00_START:
				cg.game_practice_mode	= 0;
				cg.game_continue_stage	= (1-1);			/*(0)*/ 	/* STAGE1 == 0 == (1-1)*/
				cb.main_call_func		= difficulty_select_menu_start; 	/* ��Փx�I�����j���[�� */
				goto MAIN_MENU_break; /* Start */
			case_MAIN_MENU_01_EXTRA_START:
				cg.game_practice_mode	= 0;
				cg.game_continue_stage	= (8-1);			/*(6)*/ 	/* STAGE8 == 7 == (8-1)*/
				if (psp_pad.pad_data & PSP_KEY_RIGHT)	{	cg.game_continue_stage++;	}/* PHANTASM DEBUG */
				cb.main_call_func		= difficulty_select_menu_start; 	/* ��Փx�I�����j���[�� */
				goto MAIN_MENU_break; /* Extra Start */
			case_MAIN_MENU_02_PRACTICE_START:
				cg.game_practice_mode	= 1;
				cb.main_call_func		= difficulty_select_menu_start; 	/* ��Փx�I�����j���[�� */
				goto MAIN_MENU_break; /* Practice Start */
			#if (1==USE_REPLAY_MODE)/*(���쐬)*/
			case_MAIN_MENU_03_REPLAY:		cb.main_call_func		= replay_mode_start;		goto MAIN_MENU_break; /* Replay */
			#else
			case_MAIN_MENU_03_STORY:		cb.main_call_func		= story_mode_start; 		goto MAIN_MENU_break; /* Story */
			#endif
			case_MAIN_MENU_04_RESULT:		cb.main_call_func		= yume_no_kiroku_start; 	goto MAIN_MENU_break; /* Result */
			case_MAIN_MENU_05_MUSIC_ROOM:	cb.main_call_func		= music_room_start; 		goto MAIN_MENU_break; /* Music Room */
			case_MAIN_MENU_06_OPTION:		cb.main_call_func		= option_menu_start;		goto MAIN_MENU_break; /* Option */
			case_MAIN_MENU_07_QUIT: 		cb.main_call_func		= NULL; 					goto MAIN_MENU_break; /* Quit */
			}
			MAIN_MENU_break:
				;
		}
	}
}


/*---------------------------------------------------------
	�^�C�g�����j���[	�t�F�[�h�A�E�g
	-------------------------------------------------------
	�I�������肵���̂ŁA���j���[���Â����Ȃ���t�F�[�h�A�E�g������B
---------------------------------------------------------*/

static void generic_menu_workMENU_STATE_02_FADE_OUT(void)
{
	menu_brite -= (SPEED_06_FADE_OUT);
	if (0 >= (menu_brite) )
	{
		cb.main_call_func = generic_menu_workMENU_STATE_03_FININSH;
		menu_brite	= 0;
	}
	draw_menu_str();
}


/*---------------------------------------------------------
	�^�C�g�����j���[	�I������
---------------------------------------------------------*/

static void generic_menu_workMENU_STATE_01_WORK_MENU(void)
{
	cg.msg_time = /*(�i���ɕ`��)*/byou60(5);	/* �� 5 �b */
	{
		if (0==psp_pad.pad_data_alter)/* ����������������ĂȂ������ꍇ�ɃL�[�`�F�b�N(���쏀��) */
		{
			if (psp_pad.pad_data & (PSP_KEY_DOWN|PSP_KEY_UP|PSP_KEY_PAUSE|PSP_KEY_RIGHT))
			{
				voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
			}
			if (psp_pad.pad_data & PSP_KEY_DOWN)
			{
				if (active_item == MENU_ITEM_99_MAX-1)
				{	active_item = 0;}
				else
				{
					/* �G�L�X�g�����j���[�I���֘A */
					{
						if ((MAIN_MENU_01_EXTRA_START-1)==active_item)
						{
							/* 0==						(					  1�ʂ܂ł̗��K�͑I�ׂ�) */
							/* 1==		  2�ʂ܂œ��B	(					  2�ʂ܂ł̗��K�͑I�ׂ�) */
							/* ... */
							/* 5==�ŏI��==6�ʂ܂œ��B	(EXTRA STAGE�I�ׂȂ��A6�ʂ܂ł̗��K�͑I�ׂ�) */
							/* 6==�ŏI��==6�ʃN���A 	(EXTRA STAGE�I�ׂ�) */
							/* 7==EXTRA��==EXTRA�ʃN���A(EXTRA STAGE�I�ׂ�) */
							if (6 > (option_config[OPTION_CONFIG_07_OPEN] & 0x07))
							{
								active_item++;
							}
						}
					}
					active_item++;
				}
				time_out_flag = M1_NOT_TIME_OUT;/* ���Ԑ؂�Ȃ� */
			}
			else if (psp_pad.pad_data & PSP_KEY_UP)
			{
				if (0 == active_item)
				{	active_item = MENU_ITEM_99_MAX-1;}
				else
				{
					/* �G�L�X�g�����j���[�I���֘A */
					{
						if ((MAIN_MENU_01_EXTRA_START+1)==active_item)
						{
							if (6 > (option_config[OPTION_CONFIG_07_OPEN] & 0x07))
							{
								active_item--;
							}
						}
					}
					active_item--;
				}
				time_out_flag = M1_NOT_TIME_OUT;/* ���Ԑ؂�Ȃ� */
			}
			if (psp_pad.pad_data & PSP_KEY_SHOT_OK)
			{
				voice_play(VOICE01_MENU_OK/*VOICE02_MENU_SELECT*/, TRACK01_EXPLODE);
				move_mode++;
				move_count = 0;
				cb.main_call_func = generic_menu_workMENU_STATE_02_FADE_OUT;	/* ���j���[�������� */
				time_out_flag	= M1_NOT_TIME_OUT;/* ���Ԑ؂�Ȃ� */
			}
		}
		/* ���Ԑ؂ꂠ�胂�[�h�p�A���Ԍo�� */
		if (0 < (time_out_flag) )
		{
			time_out_flag--;// -= FPS_MENU_FACTOR;
			//	time_out -= (1);
			if ((0) == time_out_flag)/* ���Ԑ؂� */
			{
				/* ���Ԑ؂ꂠ�胂�[�h�Ŏ��Ԑ؂�ɂȂ��� */
				move_mode++;
				move_count = 0;
				cb.main_call_func = generic_menu_workMENU_STATE_02_FADE_OUT;	/* ���j���[�������� */
			//	time_out_flag	= M2_STATE_TIME_OVER/*(-2)*/;/* ���Ԑ؂ꂠ�� */
			}
		}
	}
	draw_menu_str();
}

/*---------------------------------------------------------
	�^�C�g�����j���[	�t�F�[�h�C��
	-------------------------------------------------------
	���邭���Ȃ���A���j���[���t�F�[�h�C��������B
	Increasing menu_brite, fade menu in
---------------------------------------------------------*/

static void generic_menu_workMENU_STATE_00_FADE_IN_MENU(void)
{
	menu_brite += (SPEED_04_FADE_IN);
	if (255 <= (menu_brite) )
	{
		move_mode++;
		move_count = 0;
		cb.main_call_func = generic_menu_workMENU_STATE_01_WORK_MENU;
		menu_brite = (255);
	}
	draw_menu_str();
}

/*---------------------------------------------------------

---------------------------------------------------------*/
/*static*/ void title_logo_reset_position(void)
{
	//----[EFFECT]
	{	unsigned int i;
		for (i=0; i<(5/*+1*/); i++)
		{
			const u8 aaa_y[8] =
			{
				(8+  0),	// ��
				(8+ 51),	// ��
				(8+102),	// ��
				(8+153),	// ��
				(8+204),	// ��
				0,0,0		// dummy(.align)
			};
			{
				SPRITE *h;
				h					= obj_add_nn_direct(OBJ_HEAD_02_KOTEI+FIX_OBJ_08_EFFECT01+i);	/* �K���o�^�ł���B */
			//
				h					= &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_08_EFFECT01+i];
				h->jyumyou			= JYUMYOU_MUGEN;/* ���ԂŎ����������Ȃ� */
//				h->m_Hit256R		= JIKI_ATARI_ITEM_16;/*???*/
				h->flags			&= (~(SP_FLAG_COLISION_CHECK)); 	/* �����蔻�薳�� */
			//	h->type 			= S P_ZA KO/*S P_BO SS01*/;
				h->type 			= PANEL_STR_EASY+i;//(SPELL_SQUERE_);
			//	h->flags			|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
			//	h->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
//				h->flags			&= (~(SP_FLAG_VISIBLE));	/* ��\�� */
				//
			//	h->cx256			= t256(100);//src->cx256 + ((si n1024((ww_angle1024))*radius));
			//	h->cy256			= (i<<(8+5));//src->cy256 + ((co s1024((ww_angle1024))*radius));
				#if (0)
				h->cx256			= t256(128);//src->cx256 + ((si n1024((ww_angle1024))*radius));
				#else
				/* (r35�ł�)�Ή����Ԃɂ���Ȃ������B */
				h->cx256			= t256(480+1);//src->cx256 + ((si n1024((ww_angle1024))*radius));
				#endif
				h->cy256			= ((aaa_y[(i)])<<8);//src->cy256 + ((co s1024((ww_angle1024))*radius));
				h->m_zoom_x256		= t256(1.0);
				h->m_zoom_y256		= t256(1.0);
				#if 1
				/* �`��p�p�x(����0�x�ō����(�����v���)) */
				h->rotationCCW1024	= (0);
				#endif
			}
		}
	}
//	#define TEISOKU_EFFECT_00_OBJ (6)
//	#define TEISOKU_EFFECT_01_OBJ (7)
	#if 0
	// �ᑬeffect
	SPRITE *h;
	h					= &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_15_JIKI_TEISOKU_EFFECT];
	h->flags		&= (~(SP_FLAG_COLISION_CHECK)); 	/* �����蔻�薳�� */
	h->color32		= MAKE32RGBA(0xff, 0x22, 0x22, 0x80);	/* ���@�A������ */	/*	s1->alpha			= 0x50;*/
	#endif
}
/*---------------------------------------------------------

---------------------------------------------------------*/

/*static*/global void title_menu_start(void)
{
	cb.callback_gu_draw_haikei	= NULL;//dr aw_bg_screen = 0;	/* �w�i�E�B���h�E�\��off */
	set_music_volume(128);/*�Ƃ肠����*/
	load_SDL_bg(BG_TYPE_00_title_bg);
	psp_pop_screen();
	title_logo_reset_position();
//	int res_num,//		RES00_MAIN_MENU,
//	MENU *m =		&start_menu;

//
	cg.SDL_font_type	= FONT16W;
	//
	unsigned int yyy18;
	yyy18 = 0;
	//	kanji_window_clear_line(0); 			/* �����E�B���h�E��1�s��(==0)�̓��e�������B */
		kanji_window_all_clear();				/* ������ʂ�S�s�����B�����J�[�\�����z�[���|�W�V�����ֈړ��B */
	//
	int i;
	for (i=0; i<(MENU_ITEM_99_MAX); i++)
	{
		strcpy(my_font_text, (char *)my_menu_resource[i].str_name);
		//
		kanji_window_clear_line(i); 	/* �����E�B���h�E��n�s��(==0)�̓��e�������B */
		set_kanji_xy( (0), (yyy18) );
		kanji_color(7);
		#if (1)/*(????)*/
		kanji_draw();
		#else
		/*
			(r35)���̂��I�������Ȃ��B
			(�����͖�薳�����A���̕����� font surface ���������Ⴄ)
		 */
		kanji_ascii_font16_draw();
		#endif
		//
		{
			menu_item_my_obj[i].x256			= t256(0);
			menu_item_my_obj[i].y256			= t256(0);




			menu_item_my_obj[i].MENU_DATA_i2		= 0;
			menu_item_my_obj[i].MENU_DATA_i3		= 0;
		}
		ml_font[i].x = ((((my_menu_resource[i].x_offset[0]) )) );/* X�ʒu */
		ml_font[i].y = ((((my_menu_resource[i].y_offset[0]) )) );
		ml_font[i].timer	= ML_ON;
	//	ml_font[i].render	= NULL;
	//	ml_font[i].str		= my_font_text;/*(�f�o�b�O)*/
		yyy18 += (18);
	}
	for (; i<8; i++)
	{
		ml_font[i].x		= (32);/* X�ʒu */
		ml_font[i].y		= (32);
		ml_font[i].timer	= ML_OFF;
	//	ml_font[i].render	= NULL;
	//	ml_font[i].str		= NULL;
	}
	ml_font[(0)].haikei 		= (ML_HAIKEI_m1_OFF);/* ����Ӕw�ioff */
	cg.msg_time = (65536);	/* �� 18 �� */
//	/* �����܂�MENU�̏����� */
//
	#if 1
	/* �Ă��ƁA�\���I��� */
	//if (RES09_PAUSE_MENU==res_num)
	//	int set_fade_out_flag,
//	if (0!=/*pause_filter_flag*/0)
//	{
//		psp_pause_filter(); 	/* �|�[�Y�t�B���^�[�\�� */
//	}
	#endif
	{
		psp_push_screen();
	}
//	active_item 		= (0);/* ��mainmenu�̏ꍇ�͈ʒu�����������Ȃ� */
	menu_brite			= (0);

//	fade_out_flag	= (0)/*set_fade_out_flag*/;/*�\���I���*/
	time_out_flag	= M1_NOT_TIME_OUT/*���Ԑ؂�Ȃ�*/;
	/* ���Ԑ؂� == 500 6000 RESULT�ɂ�������v��Ȃ��Ȃ������A�����f���Ƃ��t����Ȃ炢�� */
	move_mode		= 0;
	move_count		= 0;
	cb.main_call_func = generic_menu_workMENU_STATE_00_FADE_IN_MENU;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

global void old_menu_system_init(void)
{
//	active_item 		= (0);
}
