
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�^�C�g�����j���[
	-------------------------------------------------------
	Replay �ɑΉ�������B
	���ԂŃA�h�o�^�C�Y(�I�[�g�v���C�f��)�ɂȂ����肷��\���
	���邯�ǁA��(r36)�̏�����ł��B(�����Ԑ悩��)
---------------------------------------------------------*/
#include "111_my_file.h"/*(bg�ǂ݂��݁B)*/

#include "kanji_system.h"

#include "../menu/kaiwa_sprite.h"

/*---------------------------------------------------------

---------------------------------------------------------*/
enum
{
	MAIN_MENU_00_START = 0,
	MAIN_MENU_01_EXTRA_START,
	MAIN_MENU_02_PRACTICE_START,
	MAIN_MENU_03_REPLAY,
	MAIN_MENU_04_RESULT,
	MAIN_MENU_05_MUSIC_ROOM,
	MAIN_MENU_06_OPTION,
	MAIN_MENU_07_QUIT,
	MENU_ITEM_99_MAX,	/* �I�v�V�����̍��ڐ� */
};

typedef struct
{
	int x256;
	int y256;
//
	int x_yurehaba; 	// �h�ꕝ(���j���[�I�����A�� x �̗h�ꕝ[pixel]) */
	int x_okureti;		// �x��l(�h�ꕝ�����X�ɖ߂�p�̃f�B���C�t���[����ݒ�) 3[frame] */
} MY_OBJ;

//typedef struct
//{
static MY_OBJ	menu_item_my_obj[MENU_ITEM_99_MAX]; 		/* �I�u�W�F�N�g */
static int		active_item;		/* ���݃��j���[��őI������Ă��鍀�ڔԍ��B(�I�𒆃J�[�\���ʒu) */
static int		toutatu_wariai; 	/* ���j���[�̓��B����(0 ... 255)�B */
static int		time_out_flag;		/* -3==,  -2==, -1==���Ԑ؂�Ȃ�, 0==���Ԑ؂�, ���ԃJ�E���g��=1... */
//} MENU;
	#define M1_NOT_TIME_OUT 	(-1)/* ���Ԑ؂�Ȃ��ɐݒ�(���Ԑ؂�Ȃ����[�h) */
//	#define M2_STATE_TIME_OVER	(-2)/* ���Ԑ؂ꂠ�胂�[�h�Ŏ��Ԑ؂�ɂȂ������ */
//static MENU start_menu;

/*---------------------------------------------------------

---------------------------------------------------------*/
#define USE_REPLAY_MODE 	(0)/*(���쐬)*/
#if (1==USE_REPLAY_MODE)/*(���쐬)*/
	#define STR_REPLAY	"Replay"
#else
	#define STR_REPLAY	"Story"
#endif
/*(�v4���==3���+1���)*/

enum
{
	MODE_00_TOUJYOU = 0,	/*(move_mode[0]: ���j���[�����o�ꒆ)*/
	MODE_01_MENU_SELECT,	/*(move_mode[1]: ���j���[�I��)*/
	MODE_02_TAIJYOU,		/*(move_mode[2]: ���j���[�����ޔ�)*/
	MODE_03_TAIHI_ITI,		/*(+�ޔ��ʒu�̐�ǂݕ�)*/
	MODE_04_MAX_JYOUTAI/*(�ő�l)*/
};
typedef struct
{
	s16 x_offset[MODE_04_MAX_JYOUTAI];
	s16 y_offset[MODE_04_MAX_JYOUTAI];
	const char *str_name;
} MENU_RESOURCE;
static const MENU_RESOURCE my_menu_resource[MENU_ITEM_99_MAX] =
{
// mode:	[0] 	[1] 	[2] 	[2+1]
	{	{	100,	360,	360,	400,  },
		{	-16,	 92,	 92,	-16,  }, "Start",			},
	{	{	200,	250,	250,	300,  },
		{	-16,	113,	113,	-16,  }, "Extra Start", 	},
	{	{	300,	245,	245,	200,  },
		{	-16,	134,	134,	-16,  }, "Practice Start",	},
	{	{	512,	320,	320,	512,  },// (x_offset: 400)
		{	155,	155,	155,	155,  }, STR_REPLAY,		},
	{	{	100,	285,	285,	200,  },
		{	272,	176,	176,	272,  }, "Result",			},
	{	{	200,	310,	310,	100,  },
		{	272,	197,	197,	272,  }, "Music Room",		},
	{	{	300,	335,	335,	512,  },
		{	272,	218,	218,	100,  }, "Option",			},
	{	{	400,	310,	310,	512,  },
		{	272,	239,	239,	200,  }, "Quit",			},
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
/*---------------------------------------------------------
	���j���[�̕������`�悷��B�T�u���[�`���B
---------------------------------------------------------*/
static void move_menu_str(void)
{
	int move_rate256;
	move_rate256 = toutatu_wariai;
	move_rate256 = psp_min(move_rate256, 256);
	move_rate256 = psp_max(move_rate256, 0);
	#if (1)/* �񓙕��l�̌v�Z */
	{
		u32 i_rate65536;
		i_rate65536 = (move_rate256 << 8);
	//	i_rate65536 = (int)(vfpu_ease_in_out(t)*256.0);
		i_rate65536 = (int)(vfpu_ease_in_out65536(i_rate65536));	/* t65536�̓����J�E���^�l���g���A�񓙕��ɂȂ�悤�l���C������B
		�񓙕��̕����́uease_in_out�v����́A�n�߂ƍŌオ�������ŊԂ������Ƃ����z�������B */
		move_rate256 = (i_rate65536 >> 8);
	}
	#endif
	{
		static int angle1024;
		angle1024 += (FPS_MENU_FACTOR30);
		mask1024(angle1024);
		/*unsigned*/ int i;
		for (i=0; i<MENU_ITEM_99_MAX; i++)/*(�S�s��������)*/
		{
			if (i==active_item) /*(�I�����ꂽ)*/
			{
				menu_item_my_obj[i].x_yurehaba = (12);	/*(�h�ꕝ�� 12[pixel] �ɐݒ�B)*/
			}
			else	/*(�I������ĂȂ�)*/
			{
				if ((menu_item_my_obj[i].x_yurehaba)/*(�h�ꕝ������)*/
					 && (0==menu_item_my_obj[i].x_okureti)/*(�x��l�������ς݂̏ꍇ)*/
				)
				{
					menu_item_my_obj[i].x_yurehaba--;	/*(�h�ꕝ�����炷�B)*/
					menu_item_my_obj[i].x_okureti = (3);/*(�x��l�� 3[frame]�ɐݒ�B)*/
				}
			}
			/*(�x��l���v�Z)*/
			if (menu_item_my_obj[i].x_okureti)/*(�x��l������ꍇ)*/
			{
				menu_item_my_obj[i].x_okureti--;	/*(�x��l�����炷�B)*/
			}
			//------------------
			HATSUDAN_01_speed256	= ((menu_item_my_obj[i].x_yurehaba)<<8);
			HATSUDAN_03_angle65536	= deg1024to65536(angle1024);
			sincos256();/*(�j�󃌃W�X�^�����̂Œ���)*/
			REG_08_REG0 = REG_02_DEST_X;//cos_value_t256	// �������h�炷�B
		//	REG_09_REG1 = REG_03_DEST_Y;//sin_value_t256	// �c�͗h�炳�Ȃ��B(== y offset �͎g��Ȃ��B)
			//------------------
			#if (1)/*(���B�������l�����āA��������B)*/
			REG_00_SRC_X	= ((my_menu_resource[i].x_offset[move_mode	])<<8); 	/*(�����ʒu[A]t256()�`��)*/
			REG_01_SRC_Y	= ((my_menu_resource[i].y_offset[move_mode	])<<8); 	/*(�����ʒu[A]t256()�`��)*/
			REG_02_DEST_X	= ((my_menu_resource[i].x_offset[move_mode+1])<<8); 	/*(�����ʒu[B]t256()�`��)*/
			REG_03_DEST_Y	= ((my_menu_resource[i].y_offset[move_mode+1])<<8); 	/*(�����ʒu[B]t256()�`��)*/
			REG_11_GOUSEI_WARIAI256 	= move_rate256;/*(��������t256()�`��)*/
			multiprex_rate_vector();/*(�j�󃌃W�X�^�����̂Œ���)*/
			#endif
			// ���̎��_�ŁA
			// REG_02_DEST_X: �����l
			// REG_03_DEST_Y: �����l
			//-----------------------
			menu_item_my_obj[i].x256 = (REG_02_DEST_X)+(REG_08_REG0);		// �������h�炷�B
			menu_item_my_obj[i].y256 = (REG_03_DEST_Y);//+(REG_09_REG1);	// �c�͗h�炳�Ȃ��B(== y offset �͎g��Ȃ��B)
			#if 1
			ml_font[i].x = ((menu_item_my_obj[i].x256)>>8);/*(X�ʒu)*/
			ml_font[i].y = ((menu_item_my_obj[i].y256)>>8);/*(Y�ʒu)*/
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
	/* �`�� */
	psp_pop_screen();
}


/*---------------------------------------------------------
	[��Փx�I�����j���[#01]�̊J�n�B
---------------------------------------------------------*/
static void title_menu_05_start_wait(void)
{
//	psp_clear_screen();
	/* [init] */
	if (0==psp_pad.pad_data_alter)/* ����������������ĂȂ������ꍇ */
	{
		psp_push_screen();/*(�V�K�̏ꍇ�̂݁A�o�b�N�o�b�t�@�Ƀ^�C�g���w�i�摜��ۑ�����B)*/
		cb.main_call_func = rank_select_menu_start;/* ��Փx�I�����j���[��(�ړ�) */
	}
}


/*---------------------------------------------------------
	�^�C�g�����j���[	��������
	-------------------------------------------------------
---------------------------------------------------------*/

static void title_menu_04_osimai(void)
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
			static const void *jump_menu_type[(8)] =
			{
				&&case_MAIN_MENU_00_START,
				&&case_MAIN_MENU_01_EXTRA_START,
				&&case_MAIN_MENU_02_PRACTICE_START,
				&&case_MAIN_MENU_03_REPLAY,
				&&case_MAIN_MENU_04_RESULT,
				&&case_MAIN_MENU_05_MUSIC_ROOM,
				&&case_MAIN_MENU_06_OPTION,
				&&case_MAIN_MENU_07_QUIT,
			};
			goto *jump_menu_type[(active_item&0x07)];
			{
			case_MAIN_MENU_00_START:
				cg.game_practice_mode	= 0;
				cg.game_continue_stage	= (1-1);			/*(0)*/ 	/* STAGE1 == 0 == (1-1)*/
				cb.main_call_func		= title_menu_05_start_wait; 	/* ��Փx�I�����j���[�� */
				goto MAIN_MENU_break; /* Start */
			case_MAIN_MENU_01_EXTRA_START:
				cg.game_practice_mode	= 0;
				cg.game_continue_stage	= (8-1);			/*(6)*/ 	/* STAGE8 == 7 == (8-1)*/
				if (psp_pad.pad_data & PSP_KEY_RIGHT)	{	cg.game_continue_stage++;	}/* PHANTASM DEBUG */
				cb.main_call_func		= title_menu_05_start_wait; 	/* ��Փx�I�����j���[�� */
				goto MAIN_MENU_break; /* Extra Start */
			case_MAIN_MENU_02_PRACTICE_START:
				cg.game_practice_mode	= 1;
				cb.main_call_func		= title_menu_05_start_wait; 	/* ��Փx�I�����j���[�� */
				goto MAIN_MENU_break; /* Practice Start */
			#if (1==USE_REPLAY_MODE)/*(���쐬)*/
			case_MAIN_MENU_03_REPLAY:		cb.main_call_func		= replay_mode_start;		goto MAIN_MENU_break; /* Replay */
			#else
			case_MAIN_MENU_03_REPLAY:		cb.main_call_func		= story_mode_start; 		goto MAIN_MENU_break; /* Story */
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
	�^�C�g�����j���[#03:	�������U��΂�B
	-------------------------------------------------------
	�I�����m��(�܂��͎��Ԑ؂�)�����̂ŁA������ޔ�������B
---------------------------------------------------------*/
static void title_menu_03_moji_taihi(void)
{
	toutatu_wariai += (SPEED_06_FADE_OUT);
	if (255 <= (toutatu_wariai) )
	{
	//	toutatu_wariai	= (0);//(�`��̓s���㖳��)
		cb.main_call_func = title_menu_04_osimai;
	}
	draw_menu_str();
}

/*---------------------------------------------------------
	�^�C�g�����j���[#01:	�������W�܂�B
---------------------------------------------------------*/

static void title_menu_02_select_menu(void);
static void title_menu_01_move_atumaru(void)
{
	toutatu_wariai += (SPEED_04_FADE_IN);
	if (255 <= (toutatu_wariai) )
	{
		toutatu_wariai = (0);
		move_mode++;//(1)�ɂȂ�B
		cb.main_call_func = title_menu_02_select_menu;
	}
	draw_menu_str();
}


/*---------------------------------------------------------
	�^�C�g�����j���[#02:	�I������
---------------------------------------------------------*/

static void title_menu_02_select_menu(void)
{
	cg.msg_time = /*(�i���ɕ`��)*/byou60(5);	/* �� 5 �b */
	{
		if (0==psp_pad.pad_data_alter)/* ����������������ĂȂ������ꍇ�ɃL�[�`�F�b�N(���쏀��) */
		{
			if (psp_pad.pad_data & (PSP_KEY_DOWN|PSP_KEY_UP|PSP_KEY_PAUSE|PSP_KEY_RIGHT))
			{
				voice_play_menu(VOICE02_MENU_SELECT, TRACK01_MENU01);
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
				voice_play_menu(VOICE01_MENU_OK/*VOICE02_MENU_SELECT*/, TRACK01_MENU01);
				move_mode++;//(2)�ɂȂ�B
				cb.main_call_func = title_menu_03_moji_taihi;	/* ���j���[�����ޔ��ړ� */
				time_out_flag	= M1_NOT_TIME_OUT;/* ���Ԑ؂�Ȃ� */
			}
		}
		/* ���Ԑ؂ꂠ�胂�[�h�p�A���Ԍo�� */
		if (0 < (time_out_flag) )
		{
			time_out_flag--;//FPS_MENU_FACTOR;
			if ((0) == time_out_flag)/* ���Ԑ؂� */
			{
				/* ���Ԑ؂ꂠ�胂�[�h�Ŏ��Ԑ؂�ɂȂ��� */
				move_mode++;//(2)�ɂȂ�B
				cb.main_call_func = title_menu_03_moji_taihi;	/* ���j���[�����ޔ��ړ� */
			//	time_out_flag	= M2_STATE_TIME_OVER/*(-2)*/;/* ���Ԑ؂ꂠ�� */
			}
		}
	}
	draw_menu_str();
}


#if 0
/*---------------------------------------------------------

---------------------------------------------------------*/
/*static*/ void title_logo_reset_position(void)
{
	#if 0
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
				OBJ *h;
				h					= obj_regist_direct_number(OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_08_EFFECT01+i); 	/* �K���o�^�ł���B */
			//
				h					= &obj99[OBJ_HEAD_03_0x0a00_KOTEI+FIX_OBJ_08_EFFECT01+i];
				h->jyumyou			= JYUMYOU_MUGEN;/* ���ԂŎ����������Ȃ� */
//				h->m_Hit256R		= JIKI_ATARI_ITEM_16;/*???*/
				h->atari_hantei 	= ATARI_HANTEI_OFF; 		/* �����蔻���OFF(���G) */
			//	h->type 			= AAA;
				h->obj_type_set 			= PANEL_STR_EASY+i;//(SPELL_SQUERE_);
				//
			//	h->center.x256			= t256(100);//src->center.x256 + ((si n1024((ww_angle1024))*radius));
			//	h->center.y256			= (i<<(8+5));//src->center.y256 + ((co s1024((ww_angle1024))*radius));
				#if (0)
				h->center.x256			= t256(128);//src->center.x256 + ((si n1024((ww_angle1024))*radius));
				#else
				/* (r35�ł�)�Ή����Ԃɂ���Ȃ������B */
				h->center.x256			= t256(480+1);//src->center.x256 + ((si n1024((ww_angle1024))*radius));
				#endif
				h->center.y256			= ((aaa_y[(i)])<<8);//src->center.y256 + ((co s1024((ww_angle1024))*radius));
				h->m_zoom.x256		= t256(1.0);
				h->m_zoom.y256		= t256(1.0);
				#if 1
				/* �`��p�p�x(����0�x�ō����(�����v���)) */
				h->rotationCCW1024	= (0);
				#endif
			}
		}
	}
	#endif

}
	#endif
/*---------------------------------------------------------

---------------------------------------------------------*/

/*static*/extern void kaiwa_init_obj_position(void);
/*static*/global MAIN_CALL_FUNC(title_menu_start)
{
	set_music_volume(128);/*�Ƃ肠����*/
	play_music_num(BGM_27_menu01);
	//
	cb.callback_gu_draw_haikei	= NULL;//dr aw_bg_screen = 0;	/* �w�i�E�B���h�E�\��off */
	my_file_common_name[0] = BG_TYPE_00_title_bg;psp_load_bg_file_name();
	psp_pop_screen();
//	title_logo_reset_position();
//	int res_num,//		RES00_MAIN_MENU,
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
			menu_item_my_obj[i].x_yurehaba		= (0);	/*(�h�ꕝ�� 0[pixel] �ɏ���������B)*/
			menu_item_my_obj[i].x_okureti		= (0);	/*(�x��l�� 0[frame] �ɏ���������B)*/
		}
		ml_font[i].x = ((((my_menu_resource[i].x_offset[0]) )) );/*(X�ʒu)*/
		ml_font[i].y = ((((my_menu_resource[i].y_offset[0]) )) );/*(Y�ʒu)*/
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
//	active_item 			= (0);/* ��main menu�̏ꍇ�͈ʒu�����������Ȃ� */
	toutatu_wariai			= (0);

//	fade_out_flag	= (0)/*set_fade_out_flag*/;/*�\���I���*/
	time_out_flag	= M1_NOT_TIME_OUT/*���Ԑ؂�Ȃ�*/;
	/* ���Ԑ؂� == 500 6000 RESULT�ɂ�������v��Ȃ��Ȃ������A�����f���Ƃ��t����Ȃ炢�� */
	move_mode		= (0);
	{	/*(�t�����=="Story" ���[�h��p�̗����G�ړ�)*/
		cg.side_panel_draw_flag 	= (0);	/* �p�l���\��off */
	//	ml_font[(0)].haikei 		= (ML_HAIKEI_m1_OFF);/* ����Ӕw�ioff */
		kaiwa_init_obj_position();
		kaiwa_all_obj_draw_on_off(0);	/* �����G��`�悵�Ȃ��B */
	}
	cb.main_call_func = title_menu_01_move_atumaru;
}
