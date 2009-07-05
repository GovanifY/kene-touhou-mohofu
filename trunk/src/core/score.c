
/*---------------------------------------------------------
	�X�R�A�p�l��(�\��)�֘A
---------------------------------------------------------*/

#include "support.h"
#include "enemy.h"

static SDL_Surface *panel_base; 	// �p�l���x�[�X
static SDL_Surface *power_gauge;	// �p���[�Q�[�W�B���̊�����loadbmp2�ō��ƕ��ʂɕ\�������悤�ɂȂ����B
static SDL_Surface *boss_gauge; 	// [***090305		�ǉ�:�{�XHP�Q�[�W

/* �T�C�h�p�l���̉��\���ʒu pannel x offset */
#define PPP 380


/*---------------------------------------------------------
 * 10�i���\�����s���B
 * �������Ȃ���ʌ��� 0 �� �\�����Ȃ��B
 * ��ɍő包�����̕\���ɂȂ�B
 *
 * @param num [in] �\������l
 * @param size [in] �\������ő包��
 * @param sprite [in] �g�p����X�v���C�g���[�N�̐擪�ԍ�
 * @param x [in] �\������p�l�����ʒu X
 * @param y [in] �\������p�l�����ʒu Y
 * @return ����
 *
---------------------------------------------------------*/
static void dec_display(
		unsigned int num,
		int size,
//		int sprite,
//		int x,
//		int y
		char *my_str
)
{
	int s;
//	int j;
	//unsigned int n;
	unsigned int c;
	const unsigned int sub10[10/*8*/]=
	{
		1,
		10,
		100,
		1000,
		//
		10000,
		100000,
		1000000,
		10000000,
		//
		100000000,
		1000000000//,
	//	2147483647	== 0x7fffffff == signed int 32bit  max
	};
	s=0;
	num++;/*�Ō㌅�̒��덇�킹*/
//	j = 256* (x);
	while (size)
	{
		size--;
		c = 0;
		while (num > sub10[size])
		{
			num -= sub10[size];
			c++;
			s=1;
		}
		if (
			(0==s) //&& /* ��ʌ��� 0 �͕\�����Ȃ� */
			//(0!=size) 	/* 1���ڂ͕K���\������ */
		)
		{
	;// 	obj_01_panel[sprite+size].m_Use 	= FALSE;
		}
		else
		{
	//	//	obj_01_panel[sprite+size].x256			= 256* (x + ((size - 1 - i) * 16/DIV2));
	//		obj_01_panel[sprite+size].x256			= (j);
	//		obj_01_panel[sprite+size].y256			= 256* (y);
	//		obj_01_panel[sprite+size].w 			= TEX1_W_STR_09;
	//		obj_01_panel[sprite+size].h 			= TEX1_H_STR_09;
	//	//	obj_01_panel[sprite+size].m_Priority	= BS_DEPTH_PANEL_ITEM;
	//		obj_01_panel[sprite+size].alpha 		= 255;	/* ���l(255 �ŕs�����A0 �œ���) */
	//		obj_01_panel[sprite+size].texture_x 	= TEX1_X_STR_09+(/*9 **/16* c);
	//		obj_01_panel[sprite+size].texture_y 	= TEX1_Y_STR_09;
	//	//	obj_01_panel[sprite+size].m_TexW		= TEX1_W_STR_09;
	//	//	obj_01_panel[sprite+size].m_TexH		= TEX1_H_STR_09;
	//	//	obj_01_panel[sprite+size].m_RollX		= 0.0;
	//	//	obj_01_panel[sprite+size].m_RollY		= 0.0;
	//		obj_01_panel[sprite+size].m_RollZ		= 0/*.0*/;
	//		obj_01_panel[sprite+size].m_Use 		= TRUE;
	//	//	obj_01_panel[sprite+size].m_PosCenter	= FALSE;
			(*my_str)=('0'+c);
		}
		my_str++;
	//	j += ( 256* (16/DIV2) );
	}
}



/*---------------------------------------------------------
	�v���C���[�̐��̕\���̎q�֐�
---------------------------------------------------------*/

enum
{
	R_00_mizu_hosi_png = 0,
	R_01_aka_hosi_png,
};

static void draw_stars_status( int g_num, int num, int y_suf)
{
	SDL_Surface *star;
	/*const*/ char *img_name[2] =
	{
		"panel/mizu_hosi.png",
		"panel/aka_hosi.png",
	};
	SDL_Rect recAll,rec;
	star = loadbmp0((char *)img_name[g_num], 0, 1);
	SDL_SetColorKey(star,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	if (num<1)	{	num=0;}

	rec.x = 0;
	rec.y = 0;
	rec.w = (10*num);
	rec.h = 10;

	recAll.x = PSP_WIDTH480+3-7-(10*num);
	recAll.y = y_suf;
	recAll.w = 100;
	recAll.h = 11;
	SDL_BlitSurface(star, &rec, sdl_screen[SDL_00_SCREEN], &recAll);
}

/*---------------------------------------------------------
	�v���C���[�̃E�F�|���Q�[�W�̕\���̎q�֐�
---------------------------------------------------------*/

// ketm�ɍ������㏑������Ɖ��̂����ʂɕ\�������B
static void draw_power_gauge(int weapon/*, int dx, int dy*/)
{
	#define WP_GAUGE_X_OFS (PPP+7+9)
	#define WP_GAUGE_Y_OFS (124-2)
	SDL_Rect srec, drec;
	srec.x = 0;
	srec.y = 0;
	srec.h = 13;
	#if 0
//	srec.w = (int)((dou ble)weapon / 127/*128*/ * (dou ble)power_gauge->w); 	// [***090123		�ύX
	#else
	/* 1 dot ���炢�덷���邩������Ȃ����Ǌȗ���(������) */
	srec.w = ((int)(weapon * power_gauge->w)>>7);	// [***090123		�ύX
	#endif
	drec.w = power_gauge->w;
	drec.h = power_gauge->h;
	drec.x = WP_GAUGE_X_OFS/*dx*/;
	drec.y = WP_GAUGE_Y_OFS/*dy*/;
	SDL_BlitSurface(power_gauge, &srec, sdl_screen[SDL_00_SCREEN], &drec);
}

/*---------------------------------------------------------
	�{�X�� HP �\���̎q�֐�
---------------------------------------------------------*/

static void draw_boss_gauge(void/*int dx, int dy*/) 	// [***090305		�ύX
{
	SPRITE *s = ((PLAYER_DATA *)player->data)->boss;
	int boss_hp_value=(((BOSS_BASE *)s->data)->health);/*(???)141477*/
	if (0 > boss_hp_value)	return;/*�����̏ꍇ�͉������Ȃ�*/
	#if 1
	unsigned char boss_timer_value = ((((BOSS_BASE *)s->data)->boss_timer)>>6);/* */
	#endif
//
	/* boss hp �摜�̕������� / gauge stringth x width */
	#define HPGAUGE_X_OFS (50)
	#define HPGAUGE_Y_OFS (6)
	#define HP_FONT_X_OFS (50)/*(37+10) dx*/
	#define HP_FONT_Y_OFS (6)/*(0+0) dy*/
	//
	#define BOSS_TIMER_X_OFS (PPP-24)/* dx*/
	#define BOSS_TIMER_Y_OFS (6)/*(0+0) dy*/
	/* 1024�ȏ�̒l�͐����ŕ\�� */
	{	char buffer[4/*3*/];
	#if 1
	//	�X�y���c�莞�ԕ\��
		strcpy(buffer,"00");
		dec_display( (boss_timer_value), 2, (char *)buffer);
		font_print_screen_xy(buffer, FONT01/*FONT06*/, BOSS_TIMER_X_OFS, BOSS_TIMER_Y_OFS);
	#endif
	//	�c�胉�C�t�\��
	//	sp rintf(buffer,"%d", (boss_hp_value>>10));/*(???)141477*/
	//	sp rintf(buffer,"%d", (boss_hp_value>>10));/*(???)141477*/
		strcpy(buffer,"0");
		dec_display( (boss_hp_value>>10), 1, (char *)buffer);
		font_print_screen_xy(buffer, FONT01/*FONT06*/, HP_FONT_X_OFS, HP_FONT_Y_OFS);
	}
	/* 1024�����ׂ̍����l�̓O���t�\�� */
	{SDL_Rect srec, drec;
		srec.x = 0;
		srec.y = 0;
		srec.h = 10;
		srec.w = HPGAUGE_X_OFS+((boss_hp_value	& 0x03FF)>>2); /* 1023�l �� 255�h�b�g */
		drec.w = boss_gauge->w;
		drec.h = boss_gauge->h;
		drec.x = 10/*dx*/;
		drec.y =  0/*dy*/+HPGAUGE_Y_OFS;
		SDL_BlitSurface(boss_gauge, &srec, sdl_screen[SDL_00_SCREEN], &drec);
	}
}
#if 0
	#define STR_MAX 		"MAX"
	#define STR_MISSILE 	"MISSILE"
	#define STR_WEAPON_UP	"WEAPON_UP"
	#define STR_LASER		"LASER"
	#define STR_BOMB		"BOMB"
	#define STR_TIME_		"TIME :"
	#define STR_EXTRA_		"EXTRA  :"
#else
	//#define STR_MAX		"ABCD"			/*4��*/
	//#define STR_MISSILE	"EFGHIJ"		/*6��*/
	//#define STR_WEAPON_UP "KLMNOPQR"		/*8��*/
	//#define STR_LASER 	"STUVW" 		/*5��*/
	//#define STR_BOMB		"XYZab" 		/*5��*/
	//#define STR_TIME_ 	"cdefg" 		/*5��*/
	//#define STR_EXTRA_	"hijkl" 		/*5��*/
//
	#define STR_MAX 		"ABCD"			/*4��*/
	#define STR_EASY		"  EFGH"		/*4��*/
	#define STR_NORMAL		" IJKLM"		/*5��*/
	#define STR_HARD		"  NOPQ"		/*4��*/
	#define STR_LUNATIC 	"RSTUVW"		/*6��*/
	#define STR_EXTRA		"XYZab" 		/*5��*/
	#define STR_TIME_		"cdefg" 		/*5��*/
	#define STR_RANK_		"hijkl" 		/*5��*/
#endif



/*---------------------------------------------------------
	�X�R�A�p�l���̃X�e�[�^�X�\��
---------------------------------------------------------*/
#define USE_DEBUG	(0)
#if (1==USE_DEBUG)
int debug_num1;
int debug_num2;
#endif

static int top_score;
//static unsigned int psp_get_fps60(void);

/*---------------------------------------------------------
	60�t���[�����ƂɈ��ĂԂ��Ƃɂ��A
	�����Ԃ���Z�o����fps���擾����B
	fps �� 100 �{���������`���ŕԂ��B
	-------------------------------------------------------
	�p�r�Ffps�v���p�B
	���ӁF60�t���[�����ƂɕK���ĂԎ��B
	���ӁF���������ŌĂ΂�鎖�͍l������ĂȂ��̂ŁA�K���ꃖ������ĂԎ��B
	���ӁF�O�񂩂��72���ȓ��ɕK���ĂԎ��B(60�t���[����72���ȏ����...�ǂ�ȏ󋵂�˂�)
---------------------------------------------------------*/

/*static*/static unsigned int psp_get_fps60_00(void)
{
	static u64 ticks_alt;	/* �O��̎��� the clock time of alter ticks. */
	static u64 ticks_now;	/* ����̎��� the clock time of current ticks. */
	ticks_alt	= ticks_now;
	sceRtcGetCurrentTick(&ticks_now);
	/*	�O��v�����ԂƂ̍����� 1[nano sec] �P��  ( 1[nano sec] == 1/(1000*1000)[sec] )
	�� unsigned int == �����Ȃ�32bit�`���֕ϊ� */
	unsigned int ttt;
	ttt= ((unsigned int)(ticks_now - ticks_alt));/* �������� the clock time of differencial ticks. */
	/* fps �� 100 �{���������`���֕ϊ� */
	if (0 != ttt)/* �[�� 0 �Ŋ���ꍇ���������(Devision by zero �h�~) */
	{
	//	ttt = ( (unsigned int)(60*60*16666) / (ttt));					/*"60fps"*/
	//	ttt = ( (unsigned int)(60*60*166666) / (unsigned int)(ttt));	/*"60.0fps"*/
	//	ttt = ( (u64)(60*60*1666666) / (u64)(ttt)); 					/*"60.00fps"*/
	/*"60.00fps"(�����^�Ōv�Z����ɂ� 33bit �v��̂ŁAunsigned int (32bit) �ő���Ȃ�)*/
		ttt = ( (unsigned int)((((60*60)/16)*(1666666))) / (unsigned int)(ttt>>4));
		/* 60*60 == 3600 �́A 16 �Ŋ���؂��̂ŁA225 �ɂȂ�B */
	}
	return (ttt);
}

void score_display(void)
{
	/* [ �p�l���x�[�X��\�� ] */
	{
		SDL_Rect panel_base_r={GAME_WIDTH, 0, 0, 0};	// �f�[�^�E�B���h�E�prect->w,h,x,y
		SDL_BlitSurface(panel_base, NULL, sdl_screen[SDL_00_SCREEN], &panel_base_r);
	}
//
	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
	/* [ �{�X�̗͕̑\�� ] */
//	if (B01_BA TTLE == pd->bo ssmode)
//	if ((STATE_FLAG_05_IS_BOSS|0) == (pd->state_flag&(STATE_FLAG_05_IS_BOSS|STATE_FLAG_06_IS_SCRIPT)))
	if ((pd->state_flag&(STATE_FLAG_13_DRAW_BOSS_GAUGE)))
	{
		draw_boss_gauge(/*10, 6-6*/);
	}
//
	/* [ �v���C���[���\�� ] */
	draw_stars_status( R_00_mizu_hosi_png, (pd->zanki), 69+2);
//
	/* [ �{�����\�� ] */
	draw_stars_status( R_01_aka_hosi_png, (pd->bombs), 94+1);
//
	#if (1==USE_DEBUG)/* �e�D�揇�ʂ��Ƃǂꂭ�炢���邩���ׂĂ݂� */
	/* �p�l���̃X�R�A����debug_num1���A�O���C�Y����debug_num2��\��������B���Ă����������������Ⴄ�B */
	((PLAYER_DATA *)player->data)->score		= debug_num1;
	((PLAYER_DATA *)player->data)->graze_point	= debug_num2;
	#endif
//
	//{/*�����̂��X�R�[�v���Ȃ������ǂ�(�������X�R�[�v����Ȃ���,�R�[�h���ς��)*/
		char buffer[64/*100*/];
		/* [ �n�C�X�R�A�\�� ] */
		if (top_score < pd->my_score)
		{	top_score = pd->my_score;}
	//	sp rintf(buffer,"%09d0", top_score);
		strcpy(buffer,"0000000000");
		dec_display(top_score,		9/*8*/, (char *)buffer);		font_print_screen_xy(buffer,FONT01,PPP+1*8+3+5,15+2);
	//
		/* [ �X�R�A�\�� ] */
	//	sp rintf(buffer,"%09d0", pd->my_score);
		strcpy(buffer,"0000000000");
		dec_display(pd->my_score,	9/*8*/, (char *)buffer);		font_print_screen_xy(buffer,FONT01,PPP+1*8+3+5,40+2);
	//
		/* [ �p���[�Q�[�W�\�� ] */
		draw_power_gauge(pd->weapon_power /*,PPP+7,124*/);
		if (pd->weapon_power > (127-1) /*== 128*/)/*max==127==�u128�i�K�v*/
		{
			strcpy(buffer, STR_MAX);
		}
		else
		{
			#if 1
		//	sp rintf(buffer, "%d", (int)((dou ble)p->weapon / 128 * 100 )); 	// [***090123		�ύX
		//	sp rintf(buffer," %d", (((int)(pd->weapon_power) * 200) >>8) ); 	// [***090214		�ύX
			strcpy(buffer,"  0");
			dec_display( (((int)(pd->weapon_power) * 200) >>8), 2, (char *)&buffer[1]);
			#endif
		}
		font_print_screen_xy(buffer,FONT01,PPP+7*8+3,125-2);
		//font_print_screen_xy(buffer,FONT01,PPP+8*8+3,125/*+1*/-2);
	//
		/* [ �{���L�����ԕ\�� ] */
		//if (p->ex tra_type!=PLX_NONE)
		if (0 != pd->bomber_time)
		{
		//	sp rintf(buffer, STR_TIME_"%3d",(int)(((int)pd->bomber_time)/10));
		//	font_print_screen_xy(buffer,FONT01,PPP+3*8-6,160);
			strcpy(buffer, STR_TIME_"   ");
			dec_display( (int)(((int)pd->bomber_time) ), 3, (char *)&buffer[5]);
			buffer[7] = 0/*' '*/;/* 1���ڂ͕\�����Ȃ� */
			font_print_screen_xy(buffer,FONT01,PPP+4*8-6,160);
		}
	//
		/* [ �O���C�Y�X�R�A�\�� ] */
		//	sp rintf(buffer," %d", pd->graze_point);
			strcpy(buffer,"   0");
			dec_display( pd->graze_point, 4, (char *)&buffer[0]);
		//	font_print_screen_xy(buffer,FONT01,PPP+7*8+3,140);/*3��(����Ȃ�)*/
			font_print_screen_xy(buffer,FONT01,PPP+6*8+3,140);/*4��(�҂���)*/
	//
		/* [ ��Փx�\�� ] */
		{
			/*const*/ char *rank_name[4] =
			{
			/* 0==PLX_NONE*/		STR_EASY,
			/* 1==PLX_HOMING:*/ 	STR_NORMAL,
			/* 2==PLX_HLASER:*/ 	STR_HARD,
			/* 3==PLX_SHIELD:*/ 	STR_LUNATIC,
		//	/* 4==PLX_BOMB:*/		STR_EXTRA,
			//	/*default:*/		"UNKNOWN ???",
			};
			font_print_screen_xy( (char *)rank_name[(difficulty)&(4-1)], FONT01,PPP+7*8-4,256);
		}
	//
		#if 1
		/* [ fps �\��(60�t���[���P�ʂŕ\��) ] */
		/* (psp��1�b��59.�Ȃ񂽂�t���[���ŁA������60�łȂ��炵���ł��B ) */
		{
		//	static int ttt;
			static unsigned int ttt;
			static int fps_draw_wait_counter;
			fps_draw_wait_counter--;
			if (fps_draw_wait_counter < 1)
			{
				fps_draw_wait_counter = 60;
			//	[1/60sec]	0.016 666 [nsec] 6666666666666666666666666667
			//	16666.66666 / x == 60.0,  16666.666666/60.0 == x, x== 277.7777777777777
			//	16666.00(int) / 60.00(int) == 60.1660649819494584837545126353791 = 60.000 (int)
				ttt = psp_get_fps60_00();/* �O��v�����ԂƂ̍����� 1[nano sec] �P�ʂ� signed int == �����t32bit�`�ŕԂ��B */
			}
//
			/*"60fps"*/
	//	//	sp rintf(buffer, STR_TIME_"%3d",(int)(((int)ttt)));
	//		strcpy(buffer, STR_TIME_"00");
	//	//	dec_display(ttt,	3, (char *)&buffer[5]);
	//		dec_display(ttt,	2, (char *)&buffer[5]);
	//		font_print_screen_xy(buffer,FONT01,PPP/*+8*4*/,240);
//
	//		/*"60.0fps"*/
	//		strcpy(buffer, STR_TIME_"0000");
	//		dec_display(ttt,	3, (char *)&buffer[5]);
	//		buffer[8] = buffer[7];
	//		buffer[7] = ' ';/*'.'*/
	//		font_print_screen_xy(buffer,FONT01,PPP/*+8*4*/,240);
//
			/*"60.00fps"*/
			strcpy(buffer, STR_TIME_"00000");
			dec_display(ttt,	4, (char *)&buffer[5]);
			buffer[9] = buffer[8];
			buffer[8] = buffer[7];
			buffer[7] = ' ';/*'.'*/
			font_print_screen_xy(buffer,FONT01,PPP/*+8*4*/,240);
		}
		#endif
	//}
}

/*---------------------------------------------------------
	�p�l���\���A������
---------------------------------------------------------*/
#include "hiscore.h"/**/
extern int select_player;
void score_panel_init(void)
{
	top_score = high_score_table[select_player][0].score;	// ��ɕ\������n�C�R�A�̎擾=>score.c�ŗ��p
	panel_base			= loadbmp0("panel/panel_base.png", 0, 1);
	power_gauge 		= loadbmp0("panel/power_gauge.png", 0, 1);
	SDL_SetColorKey(power_gauge,SDL_SRCCOLORKEY|SDL_RLEACCEL,0x00000000);
	boss_gauge			= loadbmp0("panel/boss_gauge.png", 1, 1);/*2*/
}

/*---------------------------------------------------------
	�R���e�j���[�񐔂̕\��
---------------------------------------------------------*/
extern int now_max_continue;
void render_continue(void/*int now_max_continue*/)
{
	char buffer[64/*100*/];
//	/* ����n��R���e�B�j���[�ł��܂� */
//	sp rintf(buffer,  "TRY CHANCE STILL AT %2d", now_max_continue);
	strcpy(buffer,	"TRY CHANCE STILL AT  0");
	dec_display( now_max_continue, 2, (char *)&buffer[20]);

	font_print_screen_xy(buffer, FONT03, 10, 50);
//	/* �R���e�B�j���[�����ꍇ�A�X�R�A�����L���O����܂��� */
	font_print_screen_xy("IF YOU GOT CONTINUED,", FONT05, 10, 200);
	font_print_screen_xy("CAN NOT HAVE RANKING.", FONT05, 26, 220);
//
	/* ����܂蒷�����炵���p��ɂ��Ă�����[���Ȃ�����A(�����ƕ\���ł��Ȃ���)
		�Ӗ������Ȃ��͈͂œK���ɏȗ� */
}
/*---------------------------------------------------------
	�ȉ�����(�t�H���g�̌`�����ς�����̂ŕ\���ł��Ȃ���)
---------------------------------------------------------*/

	//	sp rintf(buffer, STR_EXTRA_);					font_print_screen_xy(buffer,FONT01,PPP+1*8-2,170);

	//sp rintf(buffer,"SHIPS : %d",p->zanki);		//	font_print_screen_xy(buffer,FONT01,0,10);
	//sp rintf(buffer,"SPEED : %d",p->player_speed); // font_print_screen_xy(buffer,FONT01,0,20);
	//sp rintf(buffer,"STAGE : %d",p->level);		//	font_print_screen_xy(buffer,FONT01,0,30);

	//sp rintf(buffer,"H_SCORE:");					//	font_print_screen_xy(buffer,FONT01,PPP+1*8,5);
	//sp rintf(buffer,"SCORE  :");					//	font_print_screen_xy(buffer,FONT01,PPP+1*8,30);
	//sp rintf(buffer,"GRAZE  :");					//	font_print_screen_xy(buffer,FONT01,PPP+1*8,140);
	//sp rintf(buffer,"POWER  :");					//	font_print_screen_xy(buffer,FONT01,PPP+1*8,110);
	//sp rintf(buffer,"PLAYER");					//	font_print_screen_xy(buffer,FONT01,PPP+1*8,60);
	//sp rintf(buffer,"BOMB");						//	font_print_screen_xy(buffer,FONT01,PPP+1*8,85);

	/*
	switch (weapon_List) {
	case WP_PLASMA: 			strcat(buffer,"REIFU-1");		break;
	case WP_DOUBLEPLASMA:		strcat(buffer,"REIFU-2");		break;
	case WP_QUADPLASMA: 		strcat(buffer,"REIFU-4");		break;
	case WP_FIREBALL:			strcat(buffer,"YUMEFU-1");		break;
	case WP_DOUBLEFIREBALL: 	strcat(buffer,"YUMEFU-2");		break;
	case WP_QUADFIREBALL:		strcat(buffer,"YUMEFU-4");		break;
	case WP_FIFTHFIREBALL:		strcat(buffer,"YUMEFU-5");		break;
	case WP_KILLRAY:			strcat(buffer,"OFUDA"); 		break;
	default:					strcat(buffer,"UNKNOWN ???");	break;
	}
	font_print_screen_xy(buffer,FONT01,PPP+3*8,120);
	*/

//void score_cleanup()
//{
//}

