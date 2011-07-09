
// ���̃t�@�C���ɂ�2���j�b�g������܂��B
// 1. �n�C�X�R�A�\���f�����(���̋L�^)
// 2. �n�C�X�R�A���O���́A�o�^���

//////////////////////////////////////////////////////////////////

#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�n�C�X�R�A�\���f�����(���̋L�^)
---------------------------------------------------------*/

#define MAX_7_LINES (7/*6*/)

global SCORE_FORMAT high_score_table[MAX_8_SAVE_PLAYERS][MAX_5_RANKING/*5*/];

	/*static*/static/*global*/ SDL_Surface *result_surfaces[MAX_7_LINES/*6*/];


/*---------------------------------------------------------
	���ʃ^�C�g���`��T�u
	�T�[�t�F�C�X���쐬���邪�A�J���͂��Ȃ��̂Œ��ӁB
	-------------------------------------------------------
	���̃T�u���[�`���́A���O���͉�ʂł��g���܂��B
---------------------------------------------------------*/
/*static*/static/*global*/ void high_score_render_sub(int show_player_num, unsigned int iii, int bbb)
{
	char ttmp[64/*50*/];
	sprintf(ttmp,
		"%1d "		/* ���� */
		"%-8s " 	/* �v���C���[�� */
		"%09d0" 	/* �X�R�A */
		"[%1d]",	/* ���B�X�e�[�W */
	//
		(iii+1),												/* ���� */
		high_score_table[(show_player_num)][iii].name,			/* �v���C���[�� */
		(int)high_score_table[(show_player_num)][iii].score,	/* �X�R�A */	/* gcc 4.3.5 */
		high_score_table[(show_player_num)][iii].final_stage	/* ���B�X�e�[�W */
	);

	result_surfaces[iii]		= font_render( ttmp, (bbb)?(FONT16R/*��*/):(FONT16W/*�g(��)*/) );
}
/*
	warning: format '%09d' expects type 'int', but argument 5 has type 'u32'
	����: �T�Ԗڂ̈����� signed int(�����t32bit) �Ȃ񂾂��ǁA�n���̂� unsigned int(��������32bit) ����B
*/
/*
	4.3.3 �ł� u32 -> s32 �̈Öٕϊ��́Awarning�ɂȂ�Ȃ������B(��{�I�� -std=gnu99 �� -Werror �̏ꍇ)
	4.3.5 �ł� u32 -> s32 �̈Öٕϊ��́Awarning�ɂȂ���ۂ��B
	�R���p�C���I�v�V�����ŗ}���ł���Ǝv�����Ǘǂ��킩��Ȃ����B
	���� psp �� signed �� unsigned �ł� �œK���ł���R�[�h���啪�Ⴄ����A���̕����s���͗ǂ��񂾂��ǁB
*/

/*---------------------------------------------------------
	�n�C�X�R�A�\���f�����(���̋L�^)
---------------------------------------------------------*/

/*local static*/

typedef struct
{
	int x256;
	int y256;
//
	int amplifier256;		/* ���̃Y���ʂ̊����B amplifier x offset */
	int phase256;			/* ���̃Y���ʁB phase x offset */
	int phaseout256;		/* ���̃Y���ʁB phase x offset */
	int direction;			/* �Y������� */
	int move_done_flag; 	/* �ړ������������A���ĂȂ����̏�� */
//	�萔
	int xg256;
	int yg256;
	int amplifier_speed256;
	int phase_speed256;
} RR_OBJ;

static RR_OBJ result_my_obj[MAX_7_LINES/*6*/];

static int move_done_lines; 			/* �ړ����������s�̐� */
static int show_player_num; 			/* �X�R�A���\�����̃v���C���[�ԍ� */


/*---------------------------------------------------------
	�n�C�X�R�A�\���f�����(���̋L�^)(�����ړ��p)
---------------------------------------------------------*/

static void move_result(void)
{
	unsigned int i;
	for (i=0; i<MAX_7_LINES; i++)
	{
		if (0==result_my_obj[i].move_done_flag) 	/* �ړ��͕K�v�H */
		{
			result_my_obj[i].x256 = ((result_my_obj[i].xg256) );
			result_my_obj[i].y256 = ((result_my_obj[i].yg256) );
			if (0==result_my_obj[i].direction)		/* �ړ�������� */
			{
				/* slide-in */
				result_my_obj[i].amplifier256 -= result_my_obj[i].amplifier_speed256;
				if (result_my_obj[i].amplifier256 <= t256(0.3) )
				{
					result_my_obj[i].move_done_flag=1;	/* �ړ����� */
					move_done_lines++;
				}
				else
				{
					result_my_obj[i].phase256 += result_my_obj[i].phase_speed256;/*fps_factor*/
					result_my_obj[i].x256 += ((result_my_obj[i].phase256*result_my_obj[i].amplifier256)>>8);
				}
			}
			else
			{
				result_my_obj[i].phaseout256 += result_my_obj[i].phase_speed256;/*fps_factor*/
				result_my_obj[i].x256 += ((result_my_obj[i].phaseout256*result_my_obj[i].amplifier256)>>8);
				/* slide-out */
				result_my_obj[i].amplifier256 += result_my_obj[i].amplifier_speed256;
				if (
						(t256(PSP_WIDTH480)  < result_my_obj[i].x256)	/* ��ʊO */
					)
				{
					result_my_obj[i].move_done_flag=1;	/* �ړ����� */
					move_done_lines++;
				}
			}
		}
	}
}


/*---------------------------------------------------------
	�n�C�X�R�A�\���f�����(���̋L�^)
---------------------------------------------------------*/

static void result_font_render(void)
{
	unsigned int i;
	for (i=0; i<MAX_7_LINES; i++)
	{
		if (i<5)
		{
			high_score_render_sub(show_player_num, i, (1));/*FONT16R*/	/*��*/
		}
		else
		if (5==i)
		{
			result_surfaces[5]		= font_render( (char*)"RESULT OF SCORE", FONT16W);/*"TOP FIVE FIGHTERS"*/
		}
		else
		if (6==i)
		{
			static const/*const*/ char *score_name[MAX_08_PLAYER] =
			{
			/* 0==*/	"REIMU_A",
			/* 1==*/	"REIMU_B",
			/* 2==*/	"MARISA_A",
			/* 3==*/	"MARISA_B",
			/* 4==*/	"REMILIA",
			/* 5==*/	"YUYUKO",
			/* 6==*/	"CHIRNO_A",
			/* 7==*/	"CHIRNO_Q",
			};
			result_surfaces[6]				= font_render( (char *)score_name[(show_player_num)], FONT16R);
		}
		{
			enum
			{
				RESULT_DATA_00_XG256 = 0,
				RESULT_DATA_01_YG256,
				RESULT_DATA_02_PHASE_SPEED256,
				RESULT_DATA_03_MAX		/* �ő吔 */
			};
			static const signed int result_const_status[RESULT_DATA_03_MAX][MAX_7_LINES] =
			{
				{/* RESULT_DATA_00_XG256 */
					/*(t256( 4)*0) +*/ t256(64),	/*1ST*/ 	/* 20*/
					/*(t256( 4)*1) +*/ t256(64),	/*2ND*/ 	/* 20*/
					/*(t256( 4)*2) +*/ t256(64),	/*3RD*/ 	/* 20*/
					/*(t256( 4)*3) +*/ t256(64),	/*4TH*/ 	/* 20*/
					/*(t256( 4)*4) +*/ t256(64),	/*5TH*/ 	/* 20*/
					t256( (64+32)/*30*/ ),					/*"RESULT OF SCORE"*/
					t256( (64-32)/*30*/ ),					/*"REIMU"*/
				},
				{/* RESULT_DATA_01_YG256 */
					(t256( (20*0)+(144+4)) ),		/*1ST*/ 	//(25*0)+(110+25+5)/*150*/
					(t256( (20*1)+(144+4)) ),		/*2ND*/ 	//(25*1)+(110+25+5)/*150*/
					(t256( (20*2)+(144+4)) ),		/*3RD*/ 	//(25*2)+(110+25+5)/*150*/
					(t256( (20*3)+(144+4)) ),		/*4TH*/ 	//(25*3)+(110+25+5)/*150*/
					(t256( (20*4)+(144+4)) ),		/*5TH*/ 	//(25*4)+(110+25+5)/*150*/
					t256(110),								/*"RESULT OF SCORE"*/
					t256(128-2),							/*"REIMU"*/
				},
				{/* RESULT_DATA_02_PHASE_SPEED256 */
					4,		/*1ST*/
					4,		/*2ND*/
					4,		/*3RD*/
					4,		/*4TH*/
					4,		/*5TH*/
					6,		/*"RESULT OF SCORE"*/
					5,		/*"REIMU"*/
				},
			};
			result_my_obj[i].xg256							= result_const_status[RESULT_DATA_00_XG256][i];
			result_my_obj[i].yg256							= result_const_status[RESULT_DATA_01_YG256][i];
			result_my_obj[i].phase_speed256 				= result_const_status[RESULT_DATA_02_PHASE_SPEED256][i];
		}
		result_my_obj[i].amplifier_speed256 			= t256(10.0);//t256(1.0);
		result_my_obj[i].x256			= 0;					//	result_my_obj[5]->x 	= 0/*30*/;
		result_my_obj[i].y256			= 0/*(i*25+110)*/;		//	result_my_obj[5]->y 	= 0/*50*/;
	}
	for (i=0; i<MAX_7_LINES/*5*/; i++)
	{
		result_my_obj[i].phase256			= (i<<6);/*(0)*/
		result_my_obj[i].phaseout256		= ((4-i)<<6);/*(5<<6)*/
		result_my_obj[i].amplifier256		= t256(100);/*300*/
		result_my_obj[i].direction			= 0;
		result_my_obj[i].move_done_flag 	= 0;
	}
}





/*---------------------------------------------------------
	�n�C�X�R�A�\���f�����(���̋L�^)(�{��)
---------------------------------------------------------*/

/*---------------------------------------------------------
	SDL�T�[�t�F�C�X�ɕ`�悷��B
---------------------------------------------------------*/

static void yume_no_kiroku_draw(void)
{
	psp_pop_screen();	/* ����Ēu�����w�i�ŉ�ʂ�`�� */
	{
		/* �ړ� */
		move_result();
		/* �`�� */
		{	/*static*/ SDL_Rect rect_locate_offset; 	/* �\���ʒu */
			unsigned int i;
			for (i=0; i<MAX_7_LINES; i++)
			{
				rect_locate_offset.x = ((result_my_obj[i].x256)>>8);
				rect_locate_offset.y = ((result_my_obj[i].y256)>>8);
				SDL_SetAlpha(	result_surfaces[i], SDL_SRCALPHA, /*bg_alpha_aaa*/(255));
				SDL_BlitSurface(result_surfaces[i], NULL, sdl_screen[SDL_00_VIEW_SCREEN], &rect_locate_offset);
			}
		}
	}
}


/*---------------------------------------------------------
	do slide out now.
	�ޏꒆ�B
	�I������番�򂷂�
---------------------------------------------------------*/

static void result_05_do_slide_out_now(void)
{
	if (move_done_lines==MAX_7_LINES)
	{
	//	result_font_free();/* �����̕\���p�摜���J������ */
	//	static void result_font_free(void)
		{
			unsigned int i;
			for (i=0; i<MAX_7_LINES; i++)
			{
				#if 1/* �����̕\���p�摜���J������ */
				/* �����Ńn���O�A�b�v */
			//	if (result_surfaces[i])
				{
					SDL_FreeSurface(result_surfaces[i]);
				}
				#endif
			}
		}
		main_call_func = return_call_func;	/* �u�܂������v&�u���������܂��v�ړ��̏�Ԃ𔽉f������B */
		/* ���ӁF���Ƀt�H���g������Ă邩��`��ł��Ȃ��� */
		return;/* �`�悷��ƃn���O�A�b�v����̂ŁA�����ŋ������^�[���B */
	}
	yume_no_kiroku_draw();//�v��(�ޏ�`��)
}


/*---------------------------------------------------------
	�ޏꏀ���B
---------------------------------------------------------*/

static void result_04_set_slide_out(void)
{
	{
		move_done_lines = 0;
		int i;
		for (i=0; i<MAX_7_LINES; i++)
		{
			result_my_obj[i].direction = 1;
			result_my_obj[i].move_done_flag = 0;
		}
		main_call_func = result_05_do_slide_out_now;
	}
//	yume_no_kiroku_draw();//�v��Ȃ�
}


/*---------------------------------------------------------
	�L�[�`�F�b�N�B
---------------------------------------------------------*/
static void result_01_set_location(void);/*(�錾���K�v)*/
static void result_03_pad_check(void)
{
	if (0==psp_pad.pad_data_alter)/* ����������������ĂȂ������ꍇ�ɃL�[�`�F�b�N(���쏀��) */
	{
		/* ���̐l�̐�����܂����� */
		if (psp_pad.pad_data & (PSP_KEY_RIGHT|PSP_KEY_LEFT|PSP_KEY_UP|PSP_KEY_DOWN))	/* PSP_KEY_SHOT_OK�������ꂽ */
		{
			show_player_num++;										/* ���̐l�ɂ��� */
		//	if (/*4*/7<show_player_num) 	{show_player_num=0;}	/* �Ō�܂Ō�����n�߂��猩�� */
			show_player_num &= (0x07);								/* �Ō�܂Ō�����n�߂��猩�� */
			return_call_func = result_01_set_location;	/* �܂�����̏ꍇ�̈ړ����ݒ�(���f����̂́A�܂���) */
			main_call_func = result_04_set_slide_out;
		}
		/* ����̂�߂ă��j���[�ɖ߂� */
		else
		if (psp_pad.pad_data & (PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL|PSP_KEY_OPTION|PSP_KEY_PAUSE|PSP_KEY_SELECT))
		{
			return_call_func = title_menu_start;	/* �^�C�g�����j���[�ֈړ����ݒ�(���f����̂́A�܂���) */
			main_call_func = result_04_set_slide_out;
		}
	}
//	yume_no_kiroku_draw();//�v��Ȃ�
}


/*---------------------------------------------------------
	�o�ꒆ�B
---------------------------------------------------------*/

static void result_02_do_slide_in_now(void)
{
	if (move_done_lines==MAX_7_LINES)
	{
		main_call_func		= result_03_pad_check;
	}
	yume_no_kiroku_draw();//�v��(�o��`��)
}


/*---------------------------------------------------------
	�o�ꏀ���B�V������т̕����������_�����O����B
---------------------------------------------------------*/

static void result_01_set_location(void)
{
	move_done_lines = 0;
	result_font_render();
	main_call_func		= result_02_do_slide_in_now;
}


/*---------------------------------------------------------
	�n�C�X�R�A�\���f�����(���̋L�^)(�J�n�A�����ݒ�)
---------------------------------------------------------*/

global void yume_no_kiroku_start(void)
{
	//void result_init(void)
	show_player_num = ((cg_game_select_player) & 0x07);/*0*/	/* ���ݑI������Ă���v���C���[����L�^��\���J�n����B */
	psp_push_screen();	/* ���݂̕\����ʂ𗠉�ʂɕۑ� */
	main_call_func		= result_01_set_location;/* ����� */
//	return_call_func	= result_01_set_location;/* �߂蓮���A�u�܂������v�ɐݒ� */
}


//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////

//#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�n�C�X�R�A���O���́A�o�^���
---------------------------------------------------------*/

//	/*static*/extern void high_score_render_sub(int show_player_num, int iii, int bbb);
//	/*static*/extern SDL_Surface *result_surfaces[];

/*---------------------------------------------------------
	�n�C�X�R�A���O���́A�o�^���
---------------------------------------------------------*/

static char *entry;

#define KEYBOARD_W10	(16)
#define KEYBOARD_H04	(5)
#define KEYBOARD_M40	((KEYBOARD_W10)*(KEYBOARD_H04))

#define MAX_40_LETTER	(KEYBOARD_M40)
static SDL_Surface *letter_surface[MAX_40_LETTER];

typedef struct
{
	int xpos;
	int ypos;
	int dummy_scale256;
	char ascii;
} LETTER;
static LETTER letter[MAX_40_LETTER];

#define MY_CODE_DEL_KEY (-1)
#define MY_CODE_OK_KEY	(-2)

static int now_select_name_chr;

static /*un*/signed int sel_aaa;

static int plate_x;
static int plate_y;


/* �����L���O�\���ʒu */
#define RANK_LOCATE_OFS_X_32		(32)		/*	 8 24 */
#define RANK_LOCATE_OFS_Y_40		(40)

#define KEYBORD_LOCATE_OFS_X_064	(64+16) 	/*	32 48  30 */
#define KEYBORD_LOCATE_OFS_Y_168	(168)		/* 160 84 100 */


/*---------------------------------------------------------
	�n�C�X�R�A���O���́A�o�^��ʁA����(�̎q�֐�)[�\������]
---------------------------------------------------------*/

static void name_entry_draw(void)
{
	psp_pop_screen();	/* ����Ēu�����w�i�ŉ�ʂ�`�� */
//
	SDL_Rect s;
	SDL_Rect r;
	s.x = 0;
	s.y = 0;
	/* �L�[�{�[�h������`�� */
	{
		int i;
		for (i=0; i<KEYBOARD_M40; i++)
		{
			r.w 	= s.w	= (letter_surface[i]->w);
			r.h 	= s.h	= (letter_surface[i]->h);
			r.x = (letter[i].xpos);
			r.y = (letter[i].ypos);

			if (i == sel_aaa)/* �I�𒆕���==�L�[�{�[�h���� */
			{
//				letter[i].scale256 = t256(1.6);
				{
					static int angle1024;		/*=0*/
					angle1024 += cv1024r((5));	/*fps_factor*/
					mask1024(angle1024);
					r.x 	+= ((sin1024((angle1024)))>>5);
					r.y 	+= ((sin1024((angle1024)))>>6);
				}
			}
//			else/* �I�𒆕��� != �L�[�{�[�h���� */
//			{
//				letter[i].scale256 = t256(1.0);
//			}
			/* �`�� */
							SDL_SetAlpha(	letter_surface[i], SDL_SRCALPHA, /*bg_alpha_aaa*/(255));
			/*blit_scaled*/ SDL_BlitSurface(letter_surface[i],&s,sdl_screen[SDL_00_VIEW_SCREEN],&r);
		}
	}
	/* ���蕶����`�� */
/*	if (0 <= sel_aaa)*/
	{
		SDL_Surface *tmp_surface;
		tmp_surface 	= font_render( entry, FONT16R);
	//	s.x 	= 0;
	//	s.y 	= 0;
		r.w 	=	s.w 	= tmp_surface->w;
		r.h 	=	s.h 	= tmp_surface->h;
		r.x 	= plate_x;
		r.y 	= plate_y;
							SDL_SetAlpha(	tmp_surface, SDL_SRCALPHA, /*bg_alpha_aaa*/(255));
		/*blit_scaled*/ 	SDL_BlitSurface(tmp_surface, &s, sdl_screen[SDL_00_VIEW_SCREEN], &r);
		SDL_FreeSurface(tmp_surface);
	}
}


/*---------------------------------------------------------
	�n�C�X�R�A���O���́A�o�^��ʁA����[�L�[���́���ԕύX����]
---------------------------------------------------------*/

static void name_entry_local_work(void)
{
	{
		if (0 == psp_pad.pad_data_alter)/* ����������������ĂȂ������ꍇ�ɃL�[�`�F�b�N(���쏀��) */
		{
			/* �΂߈ړ��͂��Ȃ� */
				 if (psp_pad.pad_data & PSP_KEY_LEFT )	{	sel_aaa--;				if (sel_aaa <  (0)) 				sel_aaa  = (KEYBOARD_M40-1);	}
			else if (psp_pad.pad_data & PSP_KEY_RIGHT)	{	sel_aaa++;				if (sel_aaa == (KEYBOARD_M40))		sel_aaa  = (0); 				}
			else if (psp_pad.pad_data & PSP_KEY_UP	 )	{	sel_aaa-=KEYBOARD_W10;	if (sel_aaa <  (0)) 				sel_aaa += (KEYBOARD_M40);		}
			else if (psp_pad.pad_data & PSP_KEY_DOWN )	{	sel_aaa+=KEYBOARD_W10;	if (sel_aaa >  (KEYBOARD_M40-1))	sel_aaa -= (KEYBOARD_M40);		}
			//
			if (psp_pad.pad_data & PSP_KEY_SHOT_OK) /* ���͌��� == (������)���͌���{�^���������ꂽ�B */
			{
				if (MY_CODE_DEL_KEY==(letter[sel_aaa].ascii))
				{/* [�폜�L�[](DEL)�����肵���ꍇ�B Delete last character */
					goto delete_last_character;
				}
				else
				if (MY_CODE_OK_KEY==(letter[sel_aaa].ascii))
				{/* [���͏I���L�[](OK)�����肵���ꍇ�B Input completed. ���͏I���B Eingabe abgeschlossen. */
					goto agree_entry;
				}
				else
				{
					if (now_select_name_chr < 8) /* 8[3]�����ȉ����肵���ꍇ�͕������́B  3 chrs, name input entry. */
					{
						entry[now_select_name_chr] = letter[sel_aaa].ascii;
						now_select_name_chr++;
					}
					else /* 8[3]�����ȏ�̌��肵���ꍇ�͎����I�ɓ��͏I���L�[�����������Ƃɂ���B */
					{
						sel_aaa = (KEYBOARD_M40-1);/* [���͏I���L�[](OK)���������̂Ɠ����B force set [OK] */
					}
				}
			}
			else if (psp_pad.pad_data & PSP_KEY_BOMB_CANCEL/*PSP_KEY_OPTION*/)	/* (������)�L�����Z���{�^���������ꂽ�B */
			{
			delete_last_character:
				if (0 < now_select_name_chr) /* ���O���͂̓��͕���������ꍇ�ŁB  at first chr? */
				{
					now_select_name_chr--;				/* [�폜�L�[](DEL) �����͂���Ă���̂ŁA���̕��߂�B */
					entry[now_select_name_chr] = ' ';	/* ���� */
				}
			}
			else if (psp_pad.pad_data & PSP_KEY_PAUSE)		/* (������)�I��(��������)�{�^���������ꂽ�B */
			{
			agree_entry:	/* ���͏I������ */
				if (0 < now_select_name_chr)	// ���O���͂̓��͕���������ꍇ�ŁB�������͂���Ă���ꍇ�ŁB
				{
					#if 0
					/* �L�[�{�[�h�̕������J������ */
					{	int i;
						for (i=0; i<KEYBOARD_M40; i++)
						{
							#if 0
						//	if (letter_surface[i])
							{
								/* �����Ńn���O�A�b�v */
								SDL_FreeSurface(letter_surface[i]);
							}
							#endif
						}
					}
					#endif
					main_call_func = title_menu_start;	/* �^�C�g�����j���[�ֈړ� */
					/* ���ӁF���Ƀt�H���g������Ă邩��`��ł��Ȃ��� */
					return;/* �`�悷��ƃn���O�A�b�v����̂ŁA�����ŋ������^�[���B */
				}
			}
		}
		name_entry_draw();/*�`�敔��*/
	}
}

//static void name_entry_init(void)
global void name_entry_start(void)
{
	unsigned int i;
	unsigned int j;
	/* �L�[�{�[�h(�̕���)�̏����� */
	{
		char tmp_str[2];/*64*/ /* 96 == 3 x [32] */
		tmp_str[1] = '\0';/* 0 == EOS ������̏I��� / End of String */
		for (i=0; i<(KEYBOARD_M40-2); )
		{
			const char *str_aaa = /* 8*5==40==38(chars)+1(dummy)+1(0) */
		//	{ "ABCDEFG123" "HIJKLMN456" "OPQRSTU789" "VWXYZ.-0" /*.align*/" " };
			{
				/* ���ݒ�`����Ă���t�H���g�����Ȃ��� */
				"ABCDEFGHIJKLMNOP"
				"QRSTUVWXYZ..:://"	/* ����:�X�R�A�L�^�`����ς���܂� ','�����͎g���Ȃ��B */
				"ABCDEFGHIJKLMNOP"
				"QRSTUVWXYZ--//--"
				"0123456789[]!?" /*.align*/" "/*.align*/" "
			};
			unsigned char c;
			c = str_aaa[i];
			tmp_str[0] = c;
			letter[i].ascii = c;
			letter_surface[i] = font_render( tmp_str, FONT16R); /* */
			i++;
		}
	}
	letter[i].ascii = MY_CODE_DEL_KEY;	letter_surface[i++] = font_render( (char*)/*"DEL"*/"/", FONT16R); /* 39 */
	letter[i].ascii = MY_CODE_OK_KEY;	letter_surface[i++] = font_render( (char*)/*"OK"*/"!",	FONT16R); /* 40 */
	{
		unsigned int k;
		k = 0;
		for (j=0; j<(KEYBOARD_H04*20);/*(4*36)*/ j+=(20))	 /*(36)*/ /*25*/
		{
			for (i=0; i<(KEYBOARD_W10*20);/*(10*30)*/ i+=(20))/*(30)*/ /*25*/
			{
				letter[k].xpos = (i) + (KEYBORD_LOCATE_OFS_X_064);
				letter[k].ypos = (j) + (KEYBORD_LOCATE_OFS_Y_168);
				k++;
			}
		}
	}
	/* �L�[�{�[�h(�̕���)�̊g�嗦�������� */
//	for (i=0; i<KEYBOARD_M40; i++)
//	{
//		letter[i].scale256 = t256(1.0);/*0.0*/
//	}
	/* �V���Ƀ����N�C�������X�R�A�ʒu�𒲂ׂ� */
	for (i=0; i<5; i++)
	{
		if (cg.game_score > high_score_table[(cg_game_select_player)][i].score)
		{
			break;
		}
	}
	/* �ȍ~ i �������N�C�������ʒu�B����ɂ����̂ŁA�ʖ��Œ�`���Ƃ��B */
	#define 	my_rank 	i
	/* �X�R�A�ʒu��艺���ړ������� */
	for (j=4; j>my_rank; j--)
	{
		high_score_table[(cg_game_select_player)][j] = high_score_table[(cg_game_select_player)][j-1];
	}
	/* �V���Ƀ����N�C�������X�R�A�ʒu�֑}�� */
	high_score_table[(cg_game_select_player)][my_rank].score			= cg.game_score;
	high_score_table[(cg_game_select_player)][my_rank].final_stage		= cg.game_now_stage;/**/
//
	entry = high_score_table[(cg_game_select_player)][my_rank].name;
	entry[0] = ' ';
	entry[1] = ' ';
	entry[2] = ' ';
	entry[3] = ' ';
//
	entry[4] = ' ';
	entry[5] = ' ';
	entry[6] = ' ';
	entry[7] = ' ';
	entry[8] = 0;
	now_select_name_chr = 0;

	/** �`�� **/

	/* PLAY RESULT���t��������A��ʂ�����������������Ƃɂ����B */
	{
		//psp_clear_screen();	/* �\��ʂ����� */
		load_SDL_bg(BG_TYPE_01_name_regist);/* ����ʂɃ��[�h */
		psp_pop_screen();/* ����ʂ�\�ɃR�s�[ */
	}
	/* ���ʃ^�C�g���`�� */
	{
		for (j=0; j<5; j++)
		{
			high_score_render_sub((cg_game_select_player), j, ((my_rank==j)));/* FONT16R ��  FONT16W �g(��) */
		//
			SDL_Rect r;
			r.x = (RANK_LOCATE_OFS_X_32);
			r.y = (RANK_LOCATE_OFS_Y_40)+(j*20);
			r.w = result_surfaces[j]->w;
			r.h = result_surfaces[j]->h;
			SDL_SetAlpha(	result_surfaces[j], SDL_SRCALPHA, /*bg_alpha_aaa*/(255));
			SDL_BlitSurface(result_surfaces[j], NULL, sdl_screen[SDL_00_VIEW_SCREEN], &r);
			SDL_FreeSurface(result_surfaces[j]);
		}
	}
	#if 1
	/* ���蕶���̈ʒu */
	plate_x = (RANK_LOCATE_OFS_X_32)+((16/*FONT16WW*/)*(2));
	plate_y = (RANK_LOCATE_OFS_Y_40)+(my_rank*20);
	#endif
	psp_push_screen();	/* ���݂̕\����ʂ𗠉�ʂɕۑ� */
	sel_aaa = 0;
	main_call_func = name_entry_local_work;
}
