
// ���̃t�@�C���ɂ�2���j�b�g������܂��B
// 1. �n�C�X�R�A�\���f�����(���̋L�^)
// 2. �n�C�X�R�A���O���́A�o�^���

//////////////////////////////////////////////////////////////////

#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�n�C�X�R�A�\���f�����(���̋L�^)
---------------------------------------------------------*/
#include "111_my_file.h"/*(bg�ǂ݂��݁B)*/

#include "kanji_system.h"

#define MAX_7_LINES (7/*6*/)

global SCORE_FORMAT high_score_table[MAX_8_SAVE_PLAYERS][MAX_5_RANKING/*5*/];


/*---------------------------------------------------------
	���ʃ^�C�g���`��T�u
	�T�[�t�F�C�X���쐬���邪�A�J���͂��Ȃ��̂Œ��ӁB
	-------------------------------------------------------
	���̃T�u���[�`���́A���O���͉�ʂł��g���܂��B
---------------------------------------------------------*/
/*static*/static/*global*/ void high_score_render_sub(int show_player_num, unsigned int iii)
{
	sprintf(my_font_text,
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
	int font_color;
} RR_OBJ;
enum
{
	FLAG_00_MOVING_NOW = 0, 	/* �ړ����B */
	FLAG_01_MOVE_DONE,			/* �ړ������B */
};
enum
{
	FLAG_00_DIRECTION_TO_LEFT = 0,		/* [��]���ֈړ��B/ slide-in */
	FLAG_01_DIRECTION_TO_RIGHT, 		/* [��]�E�ֈړ��B/ slide-out */
};
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
		if (FLAG_00_MOVING_NOW==result_my_obj[i].move_done_flag)	/* �ړ��͕K�v�H */
		{
			result_my_obj[i].x256 = ((result_my_obj[i].xg256) );
			result_my_obj[i].y256 = ((result_my_obj[i].yg256) );
			if (FLAG_00_DIRECTION_TO_LEFT==result_my_obj[i].direction)		/* �ړ�������� */
			{	/* [��]���ֈړ��B/ slide-in */
				result_my_obj[i].amplifier256 -= result_my_obj[i].amplifier_speed256;
				if (result_my_obj[i].amplifier256 <= t256(0.3) )
				{
					result_my_obj[i].move_done_flag = (FLAG_01_MOVE_DONE);	/* �ړ����� */
					move_done_lines++;
				}
				else
				{
					result_my_obj[i].phase256 += result_my_obj[i].phase_speed256;/*fps_factor*/
					result_my_obj[i].x256 += ((result_my_obj[i].phase256*result_my_obj[i].amplifier256)>>8);
				}
			}
			else
		//	if (FLAG_01_DIRECTION_TO_RIGHT==result_my_obj[i].direction) 	/* �ړ�������� */
			{	/* [��]�E�ֈړ��B/ slide-out */
				result_my_obj[i].phaseout256 += result_my_obj[i].phase_speed256;/*fps_factor*/
				result_my_obj[i].x256 += ((result_my_obj[i].phaseout256*result_my_obj[i].amplifier256)>>8);
				/* slide-out */
				result_my_obj[i].amplifier256 += result_my_obj[i].amplifier_speed256;
				if (
						(t256(PSP_WIDTH480)  < result_my_obj[i].x256)	/* ��ʊO */
					)
				{
					result_my_obj[i].move_done_flag = (FLAG_01_MOVE_DONE);	/* �ړ����� */
					move_done_lines++;
				}
			}
		}
	}
}


/*---------------------------------------------------------
	�n�C�X�R�A�\���f�����(���̋L�^)
---------------------------------------------------------*/
	static char result_chars[MAX_7_LINES/*6*/][64];

static void result_font16_render_new_surface(void)
{
	unsigned int i;
	for (i=0; i<MAX_7_LINES; i++)
	{
		if (i<5)
		{
			high_score_render_sub(show_player_num, i);/*FONT16W*/	/*��*/
			result_my_obj[i].font_color 	= (7);
		}
		else
		if (5==i)
		{
			strcpy(my_font_text, (char*)"���̋L�^");
			result_my_obj[i].font_color 	= (9);
		}
		else
		if (6==i)
		{
			static const/*const*/ char *score_name[MAX_08_PLAYER] =
			{
			/* 0==*/	"���� �얲 A (�아)",
			/* 1==*/	"���� �얲 B (����)",
			/* 2==*/	"���J ������ A (����)",
			/* 3==*/	"���J ������ B (����)",
			/* 4==*/	"���~���A �X�J�[���b�g",
			/* 5==*/	"���s�� �H�X�q",
			/* 6==*/	"�`���m (�X��)",
			/* 7==*/	"�`���m (�H��)",
			};
			strcpy(my_font_text, (char *)score_name[(show_player_num)]);
			result_my_obj[i].font_color 	= (8);
		}
		strcpy(&result_chars[i][0], 	my_font_text);
		//
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
		result_my_obj[i].direction			= (FLAG_00_DIRECTION_TO_LEFT);	/* [��]���ֈړ��B/ slide-in */
		result_my_obj[i].move_done_flag 	= (FLAG_00_MOVING_NOW);
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
		{
			unsigned int yyy18;
			yyy18 = (0);/* Y�ʒu */
			unsigned int i;
			for (i=0; i<MAX_7_LINES; i++)
			{
				kanji_window_clear_line(i); 	/* �����E�B���h�E��4�s��(==3)�̓��e�������B */
				set_kanji_xy((0)*(KANJI_FONT_08_HARF_WIDTH), (yyy18));	/* �J�[�\����4�s��(==3)�ֈړ� */
				ml_font[i].x		= ((result_my_obj[i].x256)>>8);
				ml_font[i].y		= ((result_my_obj[i].y256)>>8);
				ml_font[i].timer	= ML_ON;
				strcpy(my_font_text, (char *)&result_chars[i][0] );
				kanji_color(result_my_obj[i].font_color/*9*/);
				kanji_draw();
				yyy18 += (18);
			}
		}
	}
}


/*---------------------------------------------------------
	do slide out now.
	�ޏꒆ�B
	�I������番�򂷂�
---------------------------------------------------------*/

static MAIN_CALL_FUNC(result_05_do_slide_out_now)
{
	if (move_done_lines==MAX_7_LINES)
	{
		cb.main_call_func = cb.return_call_func;	/* �u�܂������v&�u���������܂��v�ړ��̏�Ԃ𔽉f������B */
		/* ���ӁF���Ƀt�H���g������Ă邩��`��ł��Ȃ��� */
		return;/* �`�悷��ƃn���O�A�b�v����̂ŁA�����ŋ������^�[���B */
	}
	else
	{
		yume_no_kiroku_draw();//�v��(�ޏ�`��)
	}
}


/*---------------------------------------------------------
	�ޏꏀ���B
---------------------------------------------------------*/

static MAIN_CALL_FUNC(result_04_set_slide_out)
{
	{
		move_done_lines = 0;
		int i;
		for (i=0; i<MAX_7_LINES; i++)
		{
			result_my_obj[i].direction		= (FLAG_01_DIRECTION_TO_RIGHT); 	/* [��]�E�ֈړ��B/ slide-out */
			result_my_obj[i].move_done_flag = (FLAG_00_MOVING_NOW);
		}
		cb.main_call_func = result_05_do_slide_out_now;
	}
//	yume_no_kiroku_draw();//�v��Ȃ�
}


/*---------------------------------------------------------
	�L�[�`�F�b�N�B
---------------------------------------------------------*/
static MAIN_CALL_FUNC(result_01_set_location);/*(�錾���K�v)*/
static MAIN_CALL_FUNC(result_03_pad_check)
{
	if (0==psp_pad.pad_data_alter)/* ����������������ĂȂ������ꍇ�ɃL�[�`�F�b�N(���쏀��) */
	{
		/* ���̐l�̐�����܂����� */
		if (psp_pad.pad_data & (PSP_KEY_RIGHT|PSP_KEY_LEFT|PSP_KEY_UP|PSP_KEY_DOWN))	/* PSP_KEY_SHOT_OK�������ꂽ */
		{
			show_player_num++;										/* ���̐l�ɂ��� */
		//	if (/*4*/7<show_player_num) 	{show_player_num=0;}	/* �Ō�܂Ō�����n�߂��猩�� */
			show_player_num &= (0x07);								/* �Ō�܂Ō�����n�߂��猩�� */
			cb.return_call_func 	= result_01_set_location;	/* �܂�����̏ꍇ�̈ړ����ݒ�(���f����̂́A�܂���) */
			cb.main_call_func		= result_04_set_slide_out;
		}
		/* ����̂�߂ă��j���[�ɖ߂� */
		else
		if (psp_pad.pad_data & (PSP_KEY_SHOT_OK|PSP_KEY_BOMB_CANCEL|PSP_KEY_OPTION|PSP_KEY_PAUSE|PSP_KEY_SELECT))
		{
			cb.return_call_func 	= title_menu_start; 	/* �^�C�g�����j���[�ֈړ����ݒ�(���f����̂́A�܂���) */
			cb.main_call_func		= result_04_set_slide_out;
		}
	}
//	yume_no_kiroku_draw();//�v��Ȃ�
}


/*---------------------------------------------------------
	�o�ꒆ�B
---------------------------------------------------------*/

static MAIN_CALL_FUNC(result_02_do_slide_in_now)
{
	if (move_done_lines==MAX_7_LINES)
	{
		cb.main_call_func		= result_03_pad_check;
	}
	yume_no_kiroku_draw();//�v��(�o��`��)
}


/*---------------------------------------------------------
	�o�ꏀ���B�V������т̕����������_�����O����B
---------------------------------------------------------*/

static MAIN_CALL_FUNC(result_01_set_location)
{
	move_done_lines 		= 0;
	result_font16_render_new_surface();
	cb.main_call_func		= result_02_do_slide_in_now;
}


/*---------------------------------------------------------
	�n�C�X�R�A�\���f�����(���̋L�^)(�J�n�A�����ݒ�)
---------------------------------------------------------*/

global MAIN_CALL_FUNC(yume_no_kiroku_start)
{
	//void result_init(void)
	show_player_num 		= ((cg_game_select_player) & 0x07);/*0*/	/* ���ݑI������Ă���v���C���[����L�^��\���J�n����B */
	psp_push_screen();	/* ���݂̕\����ʂ𗠉�ʂɕۑ� */
	cg.msg_time 			= (65536);	/* �� 18 �� */
	cb.main_call_func		= result_01_set_location;/* ����� */
//	return_call_func		= result_01_set_location;/* �߂蓮���A�u�܂������v�ɐݒ� */
}


//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////

//#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�n�C�X�R�A���O���́A�o�^���
---------------------------------------------------------*/

//	/*static*/extern void high_score_render_sub(int show_player_num, int iii, int bbb);

/*---------------------------------------------------------
	�n�C�X�R�A���O���́A�o�^���
---------------------------------------------------------*/

#define KEYBOARD_W10	(16)
#define KEYBOARD_H04	(5)
#define KEYBOARD_M40	((KEYBOARD_W10)*(KEYBOARD_H04))

#define MAX_40_LETTER	(KEYBOARD_M40)
static SDL_Surface *letter_surface[MAX_40_LETTER];

typedef struct
{
	int xpos;
	int ypos;
	u8 ascii;
	u8 dummy_scale256;
} LETTER;
static LETTER letter[MAX_40_LETTER];

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
static char *ranking_enter_name_string;

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
		/*unsigned*/ int i;
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
					{
						int sin_value_t256; 		//	sin_value_t256 = 0;
						int cos_value_t256; 		//	cos_value_t256 = 0;
						int256_sincos1024( (((angle1024)/*&(1024-1)*/)), &sin_value_t256, &cos_value_t256);
						r.x 	+= ((sin_value_t256)>>(5));
						r.y 	+= ((sin_value_t256)>>(6));
					}
				}
			}
//			else/* �I�𒆕��� != �L�[�{�[�h���� */
//			{
//				letter[i].scale256 = t256(1.0);
//			}
			/* �`�� */
							SDL_SetAlpha(	letter_surface[i], SDL_SRCALPHA, /*bg_alpha_aaa*/(255));
			/*blit_scaled*/ PSPL_UpperBlit(letter_surface[i], &s, cb.sdl_screen[SDL_00_VIEW_SCREEN], &r);
		}
	}
	/* ���蕶����`�� */
/*	if (0 <= sel_aaa)*/
	{
		strcpy(my_font_text, ranking_enter_name_string);
		SDL_Surface *tmp_surface;
		tmp_surface 	= font16_render_new_surface();
	//	s.x 	= 0;
	//	s.y 	= 0;
		r.w 	=	s.w 	= tmp_surface->w;
		r.h 	=	s.h 	= tmp_surface->h;
		r.x 	= plate_x;
		r.y 	= plate_y;
							SDL_SetAlpha(	tmp_surface, SDL_SRCALPHA, /*bg_alpha_aaa*/(255));
		/*blit_scaled*/ 	PSPL_UpperBlit(tmp_surface, &s, cb.sdl_screen[SDL_00_VIEW_SCREEN], &r);
		SDL_FreeSurface(tmp_surface);
	}
}


/*---------------------------------------------------------
	�n�C�X�R�A���O���́A�o�^��ʁA����[�L�[���́���ԕύX����]
---------------------------------------------------------*/


//static MAIN_CALL_FUNC(name_entry_local_final)
//{
//}
#define MY_CODE_DEL_KEY 	(0x7e)
#define MY_CODE_OK_KEY		(0x7f)

static MAIN_CALL_FUNC(name_entry_local_work)
{
	if (0 == psp_pad.pad_data_alter)/* ����������������ĂȂ������ꍇ�ɃL�[�`�F�b�N(���쏀��) */
	{
		/* �΂߈ړ��͂��Ȃ� */
			 if (psp_pad.pad_data & PSP_KEY_LEFT )	{	sel_aaa--;				if (sel_aaa <  (0)) 				{	sel_aaa  = (KEYBOARD_M40-1);}	}
		else if (psp_pad.pad_data & PSP_KEY_RIGHT)	{	sel_aaa++;				if (sel_aaa == (KEYBOARD_M40))		{	sel_aaa  = (0); 			}	}
		else if (psp_pad.pad_data & PSP_KEY_UP	 )	{	sel_aaa-=KEYBOARD_W10;	if (sel_aaa <  (0)) 				{	sel_aaa += (KEYBOARD_M40);	}	}
		else if (psp_pad.pad_data & PSP_KEY_DOWN )	{	sel_aaa+=KEYBOARD_W10;	if (sel_aaa >  (KEYBOARD_M40-1))	{	sel_aaa -= (KEYBOARD_M40);	}	}
		//
		if (psp_pad.pad_data & PSP_KEY_SHOT_OK) /* ���͌��� == ���͌���{�^���������ꂽ�B */
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
				if (8 > now_select_name_chr) /* 8[3]�����ȉ����肵���ꍇ�͕������́B  3 chrs, name input entry. */
				{
					ranking_enter_name_string[now_select_name_chr] = letter[sel_aaa].ascii;
					now_select_name_chr++;
				}
				else /* 8[3]�����ȏ�̌��肵���ꍇ�͎����I�ɓ��͏I���L�[�����������Ƃɂ���B */
				{
					sel_aaa = (KEYBOARD_M40-1);/* [���͏I���L�[](OK)���������̂Ɠ����B force set [OK] */
				}
			}
		}
		else if (psp_pad.pad_data & PSP_KEY_BOMB_CANCEL/*PSP_KEY_OPTION*/)	/* �L�����Z���{�^���������ꂽ�B */
		{
		delete_last_character:
			if (0 < now_select_name_chr) /* ���O���͂̓��͕���������ꍇ�ŁB  at first chr? */
			{
				now_select_name_chr--;				/* [�폜�L�[](DEL) �����͂���Ă���̂ŁA���̕��߂�B */
				ranking_enter_name_string[now_select_name_chr] = ' ';	/* ���� */
			}
		}
		else if (psp_pad.pad_data & PSP_KEY_PAUSE)		/* �I��(��������)�{�^���������ꂽ�B */
		{
		agree_entry:	/* ���͏I������ */
			if (0 < now_select_name_chr)	// ���O���͂̓��͕���������ꍇ�ŁB�������͂���Ă���ꍇ�ŁB
			{
			//	cb.main_call_func = name_entry_local_final;/*(�������)*/
				cb.main_call_func = title_menu_start;	/* �^�C�g�����j���[�ֈړ� */
				/*(�L���ɂ��Ă����v�ȋC�����邯�ǁA���������n�C�X�R�A�o���ăn���O�A�b�v����ƁA�������肷�邵�B)*/
				/*(psp��malloc�����₵�[�̂Ńn���O�A�b�v����\��������B�͕핗�̏ꍇBGM��mp3(libmad)�Ƃ��g���ƍ��m���Ńn���O�A�b�v����B)*/
				#if (0)/*(�������)*/
				/* �L�[�{�[�h�̕������J������ */
				{	int i;
					for (i=0; i<KEYBOARD_M40; i++)
					{
						#if (1)
					//	if (letter_surface[i])
						{
							/* �����Ńn���O�A�b�v */
							SDL_FreeSurface(letter_surface[i]);
						}
						#endif
					}
				}
				/*(���ӁF���Ƀt�H���g������Ă邩��`��ł��Ȃ���)*/
				return;/*(�`�悷��ƃn���O�A�b�v����̂ŁA�����ŋ������^�[���B)*/
				#endif
			}
		}
	}
	name_entry_draw();/*�`�敔��*/
}


//static MAIN_CALL_FUNC(name_entry_init)
global MAIN_CALL_FUNC(name_entry_start)
{
	unsigned int i;
	unsigned int j;
	/* �L�[�{�[�h(�̕���)�̏����� */
	{
		my_font_text[1] = '\0';/* 0 == EOS ������̏I��� / End of String */
		for (i=0; i<(KEYBOARD_M40); i++)
		{
				/* ���ݒ�`����Ă���t�H���g�����Ȃ��� */
			//	"ABCDEFGHIJKLMNOP"
			//	"QRSTUVWXYZ..:://"		/* ����:�X�R�A�L�^�`����ς���܂� ','�����͎g���Ȃ��B */
			//	"ABCDEFGHIJKLMNOP"
			//	"QRSTUVWXYZ--//--"
			//	"0123456789[]!?" /*("DEL")*/"/"  /*("OK")*/"!"
			const char str_aaa[(KEYBOARD_M40)] = /* 80==16x5  8*5==40==38(chars)+1(dummy)+1(0) */
			{
'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '.', '$', ':', ';', '|', '@', 	/* ����:�X�R�A�L�^�`����ς���܂� ','�����͎g���Ȃ��B */
'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '+', '-', '/', '*', '=', '%',
'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '(', ')', '!', '?', MY_CODE_DEL_KEY, MY_CODE_OK_KEY,
			};
			unsigned char c;
			c = str_aaa[i];
			my_font_text[0] = c;
			letter[i].ascii = c;
			letter_surface[i] = font16_render_new_surface(); /* */
		}
	}
	{
		unsigned int k;
		k = 0;
		for (j=0; j<(KEYBOARD_H04*20);/*(4*36)*/ j+=(20))	 /*(36)*/ /*25*/
		{
			for (i=0; i<(KEYBOARD_W10*20);/*(10*30)*/ i+=(20))/*(30)*/ /*25*/
			{
				letter[k].xpos		= (i) + (KEYBORD_LOCATE_OFS_X_064);
				letter[k].ypos		= (j) + (KEYBORD_LOCATE_OFS_Y_168);
			//	letter[k].scale256	= t256(1.0);/*0.0*/ 	/* �L�[�{�[�h(�̕���)�̊g�嗦�������� */
				k++;
			}
		}
	}

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
	ranking_enter_name_string = high_score_table[(cg_game_select_player)][my_rank].name;
	ranking_enter_name_string[0] = ' ';
	ranking_enter_name_string[1] = ' ';
	ranking_enter_name_string[2] = ' ';
	ranking_enter_name_string[3] = ' ';
//
	ranking_enter_name_string[4] = ' ';
	ranking_enter_name_string[5] = ' ';
	ranking_enter_name_string[6] = ' ';
	ranking_enter_name_string[7] = ' ';
	ranking_enter_name_string[8] = 0;
	now_select_name_chr = 0;
//
	/** �`�� **/
	{
		set_kanji_hide_line(ML_LINE_99_MAX);/*(�S�s�\������B)*/
		kanji_window_all_clear();				/* ������ʂ�S�s�����B�����J�[�\�����z�[���|�W�V�����ֈړ��B */
	}
	/* PLAY RESULT���t��������A��ʂ�����������������Ƃɂ����B */
	{
	//	psp_clear_screen(); /* �\��ʂ����� */
		my_file_common_name[0] = BG_TYPE_01_name_regist;psp_load_bg_file_name();/* ����ʂɃ��[�h */
		psp_pop_screen();/* ����ʂ�\�ɃR�s�[ */
	}
	/* ���ʃ^�C�g���`�� */
	{
		for (j=0; j<5; j++)
		{
			high_score_render_sub((cg_game_select_player), j);/* FONT16W ��  FONT16R �g(��) */
			cg.PSPL_font_x = (RANK_LOCATE_OFS_X_32);
			cg.PSPL_font_y = (RANK_LOCATE_OFS_Y_40)+(j*20);
			font_render_view_screen();
		}
	}
	#if 1
	/* ���蕶���̈ʒu */
	plate_x = (RANK_LOCATE_OFS_X_32)+((16/*FONT16RW*/)*(2));
	plate_y = (RANK_LOCATE_OFS_Y_40)+(my_rank*20);
	#endif
	psp_push_screen();	/* ���݂̕\����ʂ𗠉�ʂɕۑ� */
	sel_aaa = 0;
	cb.main_call_func = name_entry_local_work;
	//
//	set_music_volume(128);/*�Ƃ肠����*/
	play_music_num(BGM_31_menu05);/*(th06_16.IT �g���R���i��)*/
}
