
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�R���e�B�j���[�H���j���[
	-------------------------------------------------------
---------------------------------------------------------*/

#include "kanji_system.h"

typedef struct
{
	int x256;
	int y256;
//
	int MENU_DATA_i0_256;	// �\���p x �I�t�Z�b�g */
	int MENU_DATA_i1_256;	// �\���p y �I�t�Z�b�g */
	int MENU_DATA_i2;		// �X�v���C�g�p(���j���[�I�����A�� x �̗h�ꕝ) */
	int MENU_DATA_i3;		// �X�v���C�g�p(�h�ꕝ�����X�ɖ߂�p) */
} MY_OBJ;

enum
{
	MENU_ITEM_00_CONTINUE_YES	= 0,
	MENU_ITEM_01_CONTINUE_NO,
	MENU_ITEM_99_MAX	/* �I�u�W�F�N�g�̐� */
};

static MY_OBJ			menu_item_my_obj[MENU_ITEM_99_MAX]; 	/* �I�u�W�F�N�g */
static SDL_Surface		*menu_item_surface[MENU_ITEM_99_MAX];	/* �����̉摜 */
static int				active_item;	/* ���݃��j���[��őI������Ă��鍀�ڔԍ� */
//
static int				menu_brite; 	/* ���j���[�̖��邳(���l) */


#define M1_NOT_TIME_OUT 		(-1)/* ���Ԑ؂�Ȃ��ɐݒ�(���Ԑ؂�Ȃ����[�h) */
//#define M2_STATE_TIME_OVER	(-2)/* ���Ԑ؂ꂠ�胂�[�h�Ŏ��Ԑ؂�ɂȂ������ */


/*---------------------------------------------------------

---------------------------------------------------------*/

typedef struct
{
	int x_offset;
	const char *str_name;
} MENU_RESOURCE;
//	int fade_out;
//	int time_out;
static MENU_RESOURCE my_menu_resource[MENU_ITEM_99_MAX] =
{
	//	RES08_ASK_CONTINUE_MENU 	//	/*const*/ char *ask_continue_menu_options[] =/*(char *)*/ask_continue_menu_options
	{	80, 		"CONTINUE YES", 	},
	{	90, 		"CONTINUE NO",		},
//	{	0,NULL, 	},
};


/*---------------------------------------------------------

---------------------------------------------------------*/

//#if 0
//	#define FPS_MENU_FACTOR 	(fps_factor)
//	#define FPS_MENU_FACTOR10	(10/fps_factor)/*���ǂ����s����(Division by Zero ???)*/
//	#define FPS_MENU_FACTOR8	(8*fps_factor)
//	#define FPS_MENU_FACTOR15	(15*fps_factor)
//#else
	#define FPS_MENU_FACTOR 	(1)
	#define FPS_MENU_FACTOR10	(16/*64*/)/* 10/FPS_MENU_FACTOR */
	#define FPS_MENU_FACTOR8	(16)/* 8*FPS_MENU_FACTOR */
	#define FPS_MENU_FACTOR15	(3/*2*/)
//#endif

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

//	int res_num,//		RES08_ASK_CONTINUE_MENU,
//	MENU *m,//		&ask_continue_menu,

//static u32 pause_start_time = 0;
extern void render_continue(void/*int now_max_continue*/);


/*---------------------------------------------------------

---------------------------------------------------------*/

//extern void adjust_start_time(u32 pause_time);
extern void player_continue_value(void);

static u8 my_ppp_loop;
extern int now_max_continue;
static void ask_continue_menu_local_work(void)
{
	{
	//	static int www=10;
		switch ((u8)(my_ppp_loop))
		{
		case MENU_STATE_00_FADE_IN_MENU:		/* Increasing menu_brite, fade menu in */
			menu_brite += FPS_MENU_FACTOR8;
			if (255 <= (menu_brite) )
			{
				menu_brite = 255;
				my_ppp_loop++;// MENU_STATE_01_WORK_MENU;
			}
			break;
		case MENU_STATE_01_WORK_MENU:			/* fadein complete */
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
					{	active_item = 0;	}
					else
					{
						active_item++;
					}
				//	www=FPS_MENU_FACTOR10;
				}
				else if (my_pad & PSP_KEY_UP)
				{
					if (0 == active_item)
					{	active_item = MENU_ITEM_99_MAX-1;	}
					else
					{
						active_item--;
					}
				//	www=FPS_MENU_FACTOR10;
				}
				/* �Z���N�g�L�[���������ꍇ�A�K���R���e�B�j���[���Ȃ��B */
				if (my_pad & PSP_KEY_SELECT)					/* [select]�{�^���ŕK���R���e�B�j���[���Ȃ� */
				{
					/*pause_menu.*/active_item	= MENU_ITEM_01_CONTINUE_NO;
					my_ppp_loop++;// MENU_STATE_02_FADE_OUT;
				}
				/* �|�[�Y�L�[���������ꍇ�A�K���R���e�B�j���[����B */
				if (my_pad & PSP_KEY_PAUSE) 				/* [start]�{�^���ŕK���R���e�B�j���[���� */
				{
					/*pause_menu.*/active_item	= MENU_ITEM_00_CONTINUE_YES;
					my_ppp_loop++;// MENU_STATE_02_FADE_OUT;
				}
				if (my_pad & PSP_KEY_SHOT_OK)
				{
					voice_play(VOICE01_MENU_OK/*VOICE02_MENU_SELECT*/, TRACK01_EXPLODE);
					my_ppp_loop++;// MENU_STATE_02_FADE_OUT;/* ���j���[�������� */
				}
			}
		}
			break;
		case MENU_STATE_02_FADE_OUT:	/* selection done, decreasing menu_brite, fade menu out */
			menu_brite -= FPS_MENU_FACTOR8;
			if (0 >= (menu_brite) )
			{
				menu_brite	= 0;
				my_ppp_loop++;// MENU_STATE_03_FININSH;
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
			if (MENU_ITEM_00_CONTINUE_YES == active_item)
			{
		//	case 0: /* Continue Game */
				player_continue_value();
				#if 1/*Gu������������v��Ȃ��Ȃ�*/
				{
					psp_clear_screen(); /* [PAUSE] ���A����SDL��ʂ������B */
				}
				#endif
				main_call_func = shooting_game_core_work;
				//adjust_start_time(pause_start_time);
			}
			else
			{
//			case 1: /* Game over /Quit Game */
			//	gamecore_term();
			//	main_call_func = title_menu_start;	/* �^�C�g�����j���[�ֈړ� */
				main_call_func = gameover_start;
			}
			//	www=FPS_MENU_FACTOR10;
			return;
		}
//
		{
			static int angle1024/*=0*/;
			angle1024 += (FPS_MENU_FACTOR15+FPS_MENU_FACTOR15);
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

/*static*/global void ask_continue_menu_start(void)
{
	render_continue(/*now_max_continue*/);
	#if 1
	/* �Ă��ƁA�\���I��� */
	//if (RES09_PAUSE_MENU==res_num)
//	// int set_fade_out_flag,
//	if (0!=/*pause_filter_flag*/(1)/*1 �\���I���*/)
	{
		/* (???)render_continue���������Ⴄ�B�`�揇���̖��(???)�B */
//		psp_pause_filter(); 	/* �|�[�Y�t�B���^�[�\�� */
	}
	#endif
	{
		psp_push_screen();
	}
//
	int i;
	for (i=0; i<MENU_ITEM_99_MAX; i++)
	{
		menu_item_surface[i] = NULL;
		menu_item_surface[i] = font_render( (char *)my_menu_resource[i].str_name, FONT16R);
//		SDL_SetColorKey(menu_item_surface[i], (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
//		/* �J���[�L�[(�����F�A�����F)�͍� */
		{
			menu_item_my_obj[i].x256			= t256(0);
			menu_item_my_obj[i].y256			= t256(0);
			menu_item_my_obj[i].MENU_DATA_i0_256	= ((my_menu_resource[i].x_offset)<<8);
			menu_item_my_obj[i].MENU_DATA_i1_256	= (((PSP_HEIGHT272/2+40) -(((5+16))*((MENU_ITEM_99_MAX)>>1)) +(i*((5+16))))<<8);
			menu_item_my_obj[i].MENU_DATA_i2		= 0;
			menu_item_my_obj[i].MENU_DATA_i3		= 0;
		}
	}
//
	active_item 		= MENU_ITEM_00_CONTINUE_YES;/*"CONTINUE YES"*/
	menu_brite			= (0);
//	fade_out_flag		= (0)/*set_fade_out_flag*/;/*�\���I���*/
//	pause_start_time	= psp_get_uint32_ticks();
	my_ppp_loop = MENU_STATE_00_FADE_IN_MENU;
	main_call_func = ask_continue_menu_local_work;
}
