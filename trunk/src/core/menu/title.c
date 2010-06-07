
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���ʃ��j���[
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

global int continue_stage;
global int practice_mode;

/*---------------------------------------------------------

---------------------------------------------------------*/

enum
{
	MAIN_MENU_START = 0,
	MAIN_MENU_EXTRA_START,
	MAIN_MENU_PRACTICE_START,
//	MAIN_MENU_REPLAY,
	MAIN_MENU_STORY,
	MAIN_MENU_RESULT,
//	MAIN_MENU_MUSIC_ROOM,
	MAIN_MENU_OPTION,
	MAIN_MENU_QUIT,
	MENU_ITEM_99_MAX,	/* �I�v�V�����̍��ڐ� */
};


#define MAX_MENU_ITEMS (8)
typedef struct
{
	int x256;
	int y256;
//
	int MENU_DATA_i0_256;	// �\���p x �I�t�Z�b�g */
	int MENU_DATA_i1_256;	// �\���p y �I�t�Z�b�g */
	int MENU_DATA_i2;	// �X�v���C�g�p(���j���[�I�����A�� x �̗h�ꕝ) */
	int MENU_DATA_i3;	// �X�v���C�g�p(�h�ꕝ�����X�ɖ߂�p) */
} MY_OBJ;


//typedef struct
//{
static MY_OBJ			menu_item_my_obj[MAX_MENU_ITEMS/*20*/]; 	/* �I�u�W�F�N�g */
static SDL_Surface		*menu_item_surface[MAX_MENU_ITEMS/*20*/];			/* �����̉摜 */
static int				active_item;	/* ���݃��j���[��őI������Ă��鍀�ڔԍ� */

//
static int				menu_brite; 	/* ���j���[�̖��邳(���l) */
static int				time_out_flag;	/* -3==,  -2==, -1==���Ԑ؂�Ȃ�, 0==���Ԑ؂�, ���ԃJ�E���g��=1... */
//} MENU;
#define M1_NOT_TIME_OUT 	(-1)/* ���Ԑ؂�Ȃ��ɐݒ�(���Ԑ؂�Ȃ����[�h) */
//#define M2_STATE_TIME_OVER	(-2)/* ���Ԑ؂ꂠ�胂�[�h�Ŏ��Ԑ؂�ɂȂ������ */

static int generic_menu_done_flag;	/* ���ʃ��j���[�g�p�����ۂ��̃t���O */

//static MENU start_menu;


/*---------------------------------------------------------

---------------------------------------------------------*/


typedef struct
{
	int x_offset;
	const char *str_name;
} MENU_RESOURCE;
//	int fade_out;
//	int time_out;
static MENU_RESOURCE my_menu_resource[/*8*/MAX_MENU_ITEMS/*10*/] =
{
	//	RES00_MAIN_MENU 	//		/*const*/ char *start_menu_options[] =/*(char *)*/start_menu_options
	{	360,		"START",			},
	{	250,		"EXTRA START",		},/* 330 */
	{	245,		"PRACTICE START",	},
//	{	400,		"REPLAY",			},
	{	320,		"STORY",			},		// ������ƕύX */
	{	285,		"RESULT",			},
//	{	310,		"MUSIC ROOM",		},
	{	335,		"OPTION",			},
	{	310,		"QUIT", 			},
	{	0,			NULL,				},
};

/*---------------------------------------------------------

---------------------------------------------------------*/

//#if 0
//	#define FPS_MENU_FACTOR 	(fps_factor)
//	#define FPS_MENU_FACTOR10	(10/fps_factor)/*���ǂ����s����(Division by Zero ???)*/
//	#define FPS_MENU_FACTOR8	(8*fps_factor)
//	#define FPS_MENU_FACTOR30	(30*fps_factor)
//#else
	#define FPS_MENU_FACTOR 	(1)
	#define FPS_MENU_FACTOR10	(16)	/*64*/		/* 10/FPS_MENU_FACTOR */
	#define FPS_MENU_FACTOR8	(16)	/* 8*FPS_MENU_FACTOR */
	#define FPS_MENU_FACTOR30	(6) 	/*4*/
//#endif

/*---------------------------------------------------------

---------------------------------------------------------*/

/*---------------------------------------------------------

---------------------------------------------------------*/
extern void (*callback_gu_draw_haikei)(void);//unsigned int dr aw_bg_screen;				/* �w�i�E�B���h�E�\���t���O */


/* ���j���[�̏�ԁB MENU_STATE */
enum
{
	MENU_STATE_00_FADE_IN_MENU = 0,
	MENU_STATE_01_WORK_MENU,
	MENU_STATE_02_FADE_OUT,
	MENU_STATE_03_FININSH,
	MENU_STATE_04_MAX
};


/*---------------------------------------------------------

---------------------------------------------------------*/
extern int now_max_continue;

static u8 my_ppp_loop;
static void generic_menu_work(void/*MENU *m*/)
{
//	if (/*!*/0==generic_menu_done_flag)
	{
	//	if (fade_out_flag)
	//	{
	//		psp_clear_screen();
	//		SDL_SetAlpha(sdl_screen[SDL_01_BACK_SCREEN],SDL_SRCALPHA, 255-((menu_brite)>>1) );
	//	}
	//	static int www=10;
		switch ((u8)(my_ppp_loop))
		{
		case MENU_STATE_00_FADE_IN_MENU:	/* Increasing menu_brite, fade menu in */
			menu_brite += FPS_MENU_FACTOR8;
			if (255 <= (menu_brite) )
			{
				my_ppp_loop++;// MENU_STATE_01_WORK_MENU;
				menu_brite = 255;
			}
			break;
		case MENU_STATE_01_WORK_MENU: /* fadein complete */
		{
			if (0==my_pad_alter)/* ����������������ĂȂ������ꍇ�ɃL�[�`�F�b�N(���쏀��) */
			{
				if (my_pad & (PSP_KEY_DOWN|PSP_KEY_UP|PSP_KEY_PAUSE|PSP_KEY_RIGHT))
				{
					voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
				}
				if (my_pad & PSP_KEY_DOWN)
				{
					if (active_item == MENU_ITEM_99_MAX-1)
					{	active_item = 0;}
					else
					{
						/* �G�L�X�g�����j���[�I���֘A */
						{
							if ((MAIN_MENU_EXTRA_START-1)==active_item)
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
				//	www=FPS_MENU_FACTOR10;
					time_out_flag = M1_NOT_TIME_OUT;/* ���Ԑ؂�Ȃ� */
				}
				else if (my_pad & PSP_KEY_UP)
				{
					if (0 == active_item)
					{	active_item = MENU_ITEM_99_MAX-1;}
					else
					{
						/* �G�L�X�g�����j���[�I���֘A */
						{
							if ((MAIN_MENU_EXTRA_START+1)==active_item)
							{
								if (6 > (option_config[OPTION_CONFIG_07_OPEN] & 0x07))
								{
									active_item--;
								}
							}
						}
						active_item--;
					}
				//	www=FPS_MENU_FACTOR10;
					time_out_flag = M1_NOT_TIME_OUT;/* ���Ԑ؂�Ȃ� */
				}
				if (my_pad & PSP_KEY_SHOT_OK)
				{
					voice_play(VOICE01_MENU_OK/*VOICE02_MENU_SELECT*/, TRACK01_EXPLODE);
					my_ppp_loop++;// MENU_STATE_02_FADE_OUT;/* ���j���[�������� */
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
					my_ppp_loop++;// MENU_STATE_02_FADE_OUT;/* ���j���[�������� */
				//	time_out_flag	= M2_STATE_TIME_OVER/*(-2)*/;/* ���Ԑ؂ꂠ�� */
				}
			}
		}
			break;
		case MENU_STATE_02_FADE_OUT:	/* selection done, decreasing menu_brite, fade menu out */
			menu_brite -= FPS_MENU_FACTOR8;
			if (0 >= (menu_brite) )
			{
				my_ppp_loop++;// MENU_STATE_03_FININSH;
				menu_brite	= 0;
			}
			break;
		case MENU_STATE_03_FININSH: 	/* fadout fininshed, menu done */
			SDL_SetAlpha(sdl_screen[SDL_01_BACK_SCREEN],SDL_SRCALPHA,255);
			psp_pop_screen();

//			{	int i;
//				for (i=0; i<MENU_ITEM_99_MAX; i++)
//				{
//					//SDL_FreeSurface(menu_item_surface[i]);
//				}
//			}
			#if 0
//			if ((-2) == time_out_flag)/*(???)*/
//			{
//				time_out_flag = (-3);	/*(???)*/
//			}
			#endif
			/**/generic_menu_done_flag = 1;
		//	www=FPS_MENU_FACTOR10;
			return;
		}
//
		{
			static int angle1024/*=0*/;
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
				menu_item_my_obj[i].x256 = (menu_item_my_obj[i].MENU_DATA_i0_256)+(cos1024((angle1024))*menu_item_my_obj[i].MENU_DATA_i2);
			//	menu_item_my_obj[i].y256 = (menu_item_my_obj[i].MENU_DATA_i1_256)+(sin1024((angle1024))*menu_item_my_obj[i].MENU_DATA_i2);
				menu_item_my_obj[i].y256 = (menu_item_my_obj[i].MENU_DATA_i1_256);
			//	{
			//		menu_item_my_obj[i].menu_brite	= menu_brite;
			//	}
			}
		}
//		sp rite_work_SDL(SP_GROUP_PAUSE_S P_ME NU_TEXT);/*SP_GROUP_TEXTS*/
//		pause_sp rite_display();/*SP_GROUP_TEXTS*/
		/* �`�� */
		psp_pop_screen();
		{			/*static*/ SDL_Rect rect_locate_offset; 	/* �\���ʒu */
			int i;
			for (i=0; i<MENU_ITEM_99_MAX; i++)
			{
				rect_locate_offset.x = ((menu_item_my_obj[i].x256)>>8);
				rect_locate_offset.y = ((menu_item_my_obj[i].y256)>>8);
			//	SDL_SetAlpha(	menu_item_surface[i], SDL_SRCALPHA, /*bg_alpha_aaa*/(255));
				SDL_SetAlpha(	menu_item_surface[i], SDL_SRCALPHA, /*bg_alpha_aaa*/(menu_brite));
				SDL_BlitSurface(menu_item_surface[i], NULL,sdl_screen[SDL_00_VIEW_SCREEN],&rect_locate_offset);
			}
		}
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/
//extern void start_menu_work(void);
/*static*/static /*global*/ void start_menu_work(void)
{
	if (0==generic_menu_done_flag)
	{
		generic_menu_work(/*&start_menu*/);
	}
	else
	{
		#if 0/*RESULT�ɂ�������v��Ȃ��Ȃ������A�����f���Ƃ��t����Ȃ炢��*/
		if (-3 == /*start_menu.*/time_out)
		{
			/* ���Ԑ؂�ŁA�����I�Ƀf���\��  / Timeout, go on and show HighScore List */
			main_call_func = game_play_demonstration;
		}
		else
		#endif
		{
//			generic_menu_done_flag = 0;
			switch (/*start_menu.*/active_item)
			{
			case MAIN_MENU_START:			practice_mode = 0;	continue_stage = (1-1); 				/*(0)*/ 	/* STAGE1 == 0 == (1-1)*/
											main_call_func = difficulty_select_menu_start;			break; /* Start */				/* ��Փx�I�����j���[�� */
			case MAIN_MENU_EXTRA_START: 	practice_mode = 0;	continue_stage = (8-1); 				/*(6)*/ 	/* STAGE8 == 7 == (8-1)*/
											if (my_pad & PSP_KEY_RIGHT) 	{	continue_stage++;	}/* PHANTASM DEBUG */
											main_call_func = difficulty_select_menu_start;			break; /* Extra Start */		/* ��Փx�I�����j���[�� */
			case MAIN_MENU_PRACTICE_START:	practice_mode = 1;
											main_call_func = stage_select_menu_start;				break; /* Practice Start */
		//	case MAIN_MENU_REPLAY:																	break; /* Replay */
			case MAIN_MENU_STORY:			main_call_func = story_script_start;					break; /* Story */
			case MAIN_MENU_RESULT:			main_call_func = yume_no_kiroku_start;					break; /* Result */
		//	case MAIN_MENU_MUSIC_ROOM:																break; /* Music Room */
			case MAIN_MENU_OPTION:			main_call_func = option_menu_start; 					break; /* Option */
			case MAIN_MENU_QUIT:			main_call_func = NULL;									break; /* Quit */
			}
		}
	}
}

/*static*/global void title_menu_start(void)
{
	callback_gu_draw_haikei = NULL;//dr aw_bg_screen = 0;	/* �w�i�E�B���h�E�\��off */
	set_music_volume(128);/*�Ƃ肠����*/
	load_SDL_bg(BG_TYPE_00_title_bg);
	psp_pop_screen();


//	int res_num,//		RES00_MAIN_MENU,
//	MENU *m=		&start_menu;

//
	int i;
	for (i=0; i<(MENU_ITEM_99_MAX); i++)
	{
		menu_item_surface[i] = NULL;
		menu_item_surface[i] = font_render( (char *)my_menu_resource[i].str_name, FONT16R);
//		SDL_SetColorKey(menu_item_surface[i], (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
//		/* �J���[�L�[(�����F�A�����F)�͍� */
		{
			menu_item_my_obj[i].x256			= t256(0);
			menu_item_my_obj[i].y256			= t256(0);

			menu_item_my_obj[i].MENU_DATA_i0_256	= ((my_menu_resource[i].x_offset)<<8);
//			int jjj = ((menu_item_my_obj[i]->SDL_h128)>>7)+5;
			int jjj = 5+16;
			menu_item_my_obj[i].MENU_DATA_i1_256	= (((PSP_HEIGHT272/2+40) -((jjj)*((MENU_ITEM_99_MAX)>>1)) +(i*(jjj)))<<8);
			menu_item_my_obj[i].MENU_DATA_i2		= 0;
			menu_item_my_obj[i].MENU_DATA_i3		= 0;
		}
	}
	/**/generic_menu_done_flag	= 0;	/* �����܂�MENU�̏����� */
//
//1874216

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
//	active_item 		= 0;/* ��mainmenu�̏ꍇ�͈ʒu�����������Ȃ� */
	menu_brite			= (0);

//	fade_out_flag	= (0)/*set_fade_out_flag*/;/*�\���I���*/
	time_out_flag	= M1_NOT_TIME_OUT/*���Ԑ؂�Ȃ�*/;
	/* ���Ԑ؂� == 500 6000 RESULT�ɂ�������v��Ȃ��Ȃ������A�����f���Ƃ��t����Ȃ炢�� */
	my_ppp_loop = MENU_STATE_00_FADE_IN_MENU;
	main_call_func = start_menu_work;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

global void old_menu_system_init(void)
{
//	active_item 		= 0;
//	stage_select_menu.active_item	= (1-1);	/* (0) */ /*continue_stage*/
}
