
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�X�e�[�W�I�����j���[
	-------------------------------------------------------
---------------------------------------------------------*/

#include "kanji_system.h"

#define MAX_MENU_ITEMS (8)
typedef struct
{
	int x256;
	int y256;
//
	int MENU_DATA_i0;	// �\���p x �I�t�Z�b�g */
	int MENU_DATA_i1;	// �\���p y �I�t�Z�b�g */
	int MENU_DATA_i2;	// �X�v���C�g�p(���j���[�I�����A�� x �̗h�ꕝ) */
	int MENU_DATA_i3;	// �X�v���C�g�p(�h�ꕝ�����X�ɖ߂�p) */
} MY_OBJ;


typedef struct
{
	MY_OBJ			menu_item_my_obj[MAX_MENU_ITEMS];			/* �I�u�W�F�N�g */
	SDL_Surface 	*menu_item_surface[MAX_MENU_ITEMS]; 		/* �����̉摜 */
	int 			active_item;	/* ���݃��j���[��őI������Ă��鍀�ڔԍ� */
	int 			max_items;		/* �I�v�V�����̍��ڐ� */
//
	int 			menu_brite; 	/* ���j���[�̖��邳(���l) */
} MENU;
#define M1_NOT_TIME_OUT 		(-1)/* ���Ԑ؂�Ȃ��ɐݒ�(���Ԑ؂�Ȃ����[�h) */
//#define M2_STATE_TIME_OVER	(-2)/* ���Ԑ؂ꂠ�胂�[�h�Ŏ��Ԑ؂�ɂȂ������ */

static MENU aaa;
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
		//	RES01_STAGE_SELECT	/*const*/ char *stage_select_menu_options[] =/*(char *)*/stage_select_menu_options
		{	360,		"1",			},	// stage1 */
		{	350,		"2",			},	// stage2 */
		{	340,		"3",			},	// stage3 */
		{	330,		"4",			},	// stage4 */
		{	320,		"5",			},	// stage5 */
		{	310,		"6",			},	// stage6 */
	//	{	275,		"ENDING",		},	// ending */
		{	0,NULL, 	},
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


/*---------------------------------------------------------

---------------------------------------------------------*/
static void stage_select_menu_draw_all(void)
{
	{
		static int angle1024/*=0*/;
		angle1024 += (FPS_MENU_FACTOR15+FPS_MENU_FACTOR15);
		mask1024(angle1024);
		int i;
		for (i=0; i<aaa.max_items; i++)
		{
			if (i==aaa.active_item) 	/* �I�����ꂽ */
			{
				aaa.menu_item_my_obj[i].MENU_DATA_i2 = 12;	/* ��ꕝ�� 12 �ɂ��� */
			}
			else	/* �I������ĂȂ� */
			{
				if ((aaa.menu_item_my_obj[i].MENU_DATA_i2) && (0==aaa.menu_item_my_obj[i].MENU_DATA_i3))
				{
					aaa.menu_item_my_obj[i].MENU_DATA_i2--;
					aaa.menu_item_my_obj[i].MENU_DATA_i3 = 3;
				}
			}
			if (aaa.menu_item_my_obj[i].MENU_DATA_i3)
			{	aaa.menu_item_my_obj[i].MENU_DATA_i3--;}
			aaa.menu_item_my_obj[i].x256 = (aaa.menu_item_my_obj[i].MENU_DATA_i0*256)+(cos1024((angle1024))*aaa.menu_item_my_obj[i].MENU_DATA_i2);
		//	aaa.menu_item_my_obj[i].y256 = (aaa.menu_item_my_obj[i].MENU_DATA_i1*256)+(sin1024((angle1024))*aaa.menu_item_my_obj[i].MENU_DATA_i2);
			aaa.menu_item_my_obj[i].y256 = (aaa.menu_item_my_obj[i].MENU_DATA_i1*256);
		//	{
		//		aaa.menu_item_my_obj[i].aaa.menu_brite	= aaa.menu_brite;
		//	}
		}
	}
//	sp rite_work_SDL(SP_GROUP_PAUSE_S P_ME NU_TEXT);/*SP_GROUP_TEXTS*/
//	pause_sp rite_display();/*SP_GROUP_TEXTS*/
	/* �`�� */
	psp_pop_screen();
	{			/*static*/ SDL_Rect rect_locate_offset; 	/* �\���ʒu */
		int i;
		for (i=0; i<aaa.max_items; i++)
		{
			rect_locate_offset.x = ((aaa.menu_item_my_obj[i].x256)>>8);
			rect_locate_offset.y = ((aaa.menu_item_my_obj[i].y256)>>8);
		//	SDL_SetAlpha(	aaa.menu_item_surface[i], SDL_SRCALPHA, /*bg_alpha_aaa*/(255));
			SDL_SetAlpha(	aaa.menu_item_surface[i], SDL_SRCALPHA, /*bg_alpha_aaa*/(aaa.menu_brite));
			SDL_BlitSurface(aaa.menu_item_surface[i], NULL,sdl_screen[SDL_00_VIEW_SCREEN],&rect_locate_offset);
		}
	}
}
/*---------------------------------------------------------
	fadout fininshed, menu done
---------------------------------------------------------*/


static void stage_select_menu_work_MENU_STATE_03_FININSH(void)
{
	SDL_SetAlpha(sdl_screen[SDL_01_BACK_SCREEN],SDL_SRCALPHA,255);
	psp_pop_screen();
//	{	int i;
//		for (i=0; i<aaa.max_items; i++)
//		{
//			//SDL_FreeSurface(aaa.menu_item_surface[i]);
//		}
//	}
	cg.game_continue_stage = aaa.active_item;
	/* �v���N�e�B�X �Q�[���J�n */
	main_call_func = difficulty_select_menu_start;	/* ��Փx�I�����j���[�� */
//	www=FPS_MENU_FACTOR10;
//	return;
//not stage_select_menu_draw_all();
}


/*---------------------------------------------------------
	selection done, decreasing aaa.menu_brite, fade menu out
---------------------------------------------------------*/

static void stage_select_menu_work_MENU_STATE_02_FADE_OUT(void)
{
	aaa.menu_brite -= FPS_MENU_FACTOR8;
	if (0 >= (aaa.menu_brite) )
	{
		aaa.menu_brite	= 0;
		main_call_func = stage_select_menu_work_MENU_STATE_03_FININSH;	/* */
	}
	stage_select_menu_draw_all();
}
/*---------------------------------------------------------
	fadein complete
---------------------------------------------------------*/

static void stage_select_menu_work_MENU_STATE_01_WORK_MENU(void)
{
	if (0==cg_my_pad_alter)/* ����������������ĂȂ������ꍇ�ɃL�[�`�F�b�N(���쏀��) */
	{
		if (cg_my_pad & (PSP_KEY_DOWN|PSP_KEY_UP|PSP_KEY_PAUSE|PSP_KEY_RIGHT))
		{
			voice_play(VOICE02_MENU_SELECT, TRACK01_EXPLODE);
		}
		if (cg_my_pad & PSP_KEY_DOWN)
		{
			if (aaa.active_item == aaa.max_items-1)
			{	aaa.active_item = 0;}
			else
			{
				aaa.active_item++;
			}
		//	www=FPS_MENU_FACTOR10;
		}
		else if (cg_my_pad & PSP_KEY_UP)
		{
			if (0 == aaa.active_item)
			{	aaa.active_item = aaa.max_items-1;}
			else
			{
				aaa.active_item--;
			}
		//	www=FPS_MENU_FACTOR10;
		}
		if (cg_my_pad & PSP_KEY_SHOT_OK)
		{
			voice_play(VOICE01_MENU_OK/*VOICE02_MENU_SELECT*/, TRACK01_EXPLODE);
			main_call_func = stage_select_menu_work_MENU_STATE_02_FADE_OUT; 	/* ���j���[�������� */
		}
	}
	stage_select_menu_draw_all();
}


/*---------------------------------------------------------
	Increasing aaa.menu_brite, fade menu in
---------------------------------------------------------*/

static void stage_select_menu_work_MENU_STATE_00_FADE_IN_MENU(void)
{
//	static int www=10;
	aaa.menu_brite += FPS_MENU_FACTOR8;
	if (255 <= (aaa.menu_brite) )
	{
		aaa.menu_brite = 255;
		main_call_func = stage_select_menu_work_MENU_STATE_01_WORK_MENU;	/* */
	}
	stage_select_menu_draw_all();
}

/*---------------------------------------------------------

---------------------------------------------------------*/

/*static*/global void stage_select_menu_start(void)
{
	int i;
	for (i=0; i<MAX_MENU_ITEMS/*20*/; i++)
	{
		aaa.menu_item_surface[i] = NULL;
		{
			aaa.menu_item_my_obj[i].x256			= 0;
			aaa.menu_item_my_obj[i].y256			= 0;

			aaa.menu_item_my_obj[i].MENU_DATA_i0	= 0;
			aaa.menu_item_my_obj[i].MENU_DATA_i1	= 0;
			aaa.menu_item_my_obj[i].MENU_DATA_i2	= 0;
			aaa.menu_item_my_obj[i].MENU_DATA_i3	= 0;
		}
	}
	aaa.max_items		= 0;
//
	{
loop:
	{
		if (aaa.max_items>(option_config[OPTION_CONFIG_07_OPEN]&0x07))	{	goto loop_quit;}
	}
		/*while*/if (NULL == my_menu_resource[aaa.max_items].str_name)
		{	goto loop_quit;}
		aaa.menu_item_surface[aaa.max_items] = font_render( (char *)my_menu_resource[aaa.max_items].str_name, FONT16R);
//		SDL_SetColorKey(aaa.menu_item_surface[aaa.max_items], (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
//		/* �J���[�L�[(�����F�A�����F)�͍� */

		aaa.menu_item_my_obj[aaa.max_items].x256	= t256(0);
		aaa.menu_item_my_obj[aaa.max_items].y256	= t256(0);

		aaa.max_items++;
		goto loop;
	}
loop_quit:
	for (i=0; i<aaa.max_items; i++)
	{
		aaa.menu_item_my_obj[i].MENU_DATA_i0	= my_menu_resource[i/*aaa.max_items*/].x_offset;
		aaa.menu_item_my_obj[i].MENU_DATA_i1	= (PSP_HEIGHT272/2+40) -(((5+16))*((aaa.max_items)>>1)) +(i*((5+16)));
		aaa.menu_item_my_obj[i].MENU_DATA_i2	= 0;
		aaa.menu_item_my_obj[i].MENU_DATA_i3	= 0;
	}
//
	#if 1
	//	psp_pause_filter(); 	/* �|�[�Y�t�B���^�[�\�� */
	#endif
	{
		psp_push_screen();
	}
	aaa.menu_brite		= (0);
//	fade_out_flag	= (0)/*set_fade_out_flag*/;/*�\���I���*/
	//keybo ard_clear();
	#if 1
	/*
		�G�L�X�g���X�e�[�W���v���C������ŁA
		�u�G�L�X�g���X�e�[�W�A�v���N�e�B�X�v��
		�u�G���f�B���O�A�X�^�b�t���[���A�v���N�e�B�X�v��
		�I�ׂ�o�O������̂ŁA�C������B
	*/
	if ( (6-1) < cg.game_continue_stage)	{cg.game_continue_stage = (6-1);	/* (6-1) */}
	#endif
	aaa.active_item = cg.game_continue_stage;
	main_call_func = stage_select_menu_work_MENU_STATE_00_FADE_IN_MENU;
}
