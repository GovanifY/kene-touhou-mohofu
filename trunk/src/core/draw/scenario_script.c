
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�X�N���v�g����
	-------------------------------------------------------
	���݂̖��_:
	�� ���x���x������B�Q�[���{�̂� 60fps �߂��œ����Ă�̂ɁA
	   �V�i���I������� SDL �̔��������� ���x������ 30-40fps ���x�ɒቺ����B
	�� SDL��� �� Gu ��ʂ̉摜����������ɏo���Ȃ��B
	   SDL �̔��������� �� Gu ��ʂƍ����ł��Ȃ��ׁA
		�������� ����� CPU�p���[�������� �\�t�g�E�F�A�[���������� �����Ă邪�A
		�P�ɈÂ��Ȃ邾���B
	(���̂��������Ӗ��A Gu ������� ������������ psp �̃n�[�h�E�F�A�[�ŊȒP�ɏo����̂�...)�B
	-------------------------------------------------------
	�u�Q�[�����̃V�i���I�����v��
	�u�G���f�B���O���̃V�i���I�����v��
	�F�X�l�������� psp �ł͑��x�I�ɑ����������B
	�����͕�������\��ł��B
	-------------------------------------------------------
	���s���낵�����ǁu�Q�[�����̃V�i���I�����v�� SDL �Ƃ�����
	�����������������Ēx���Ęb�ɂȂ�Ȃ����B
	�u�G���f�B���O���̃V�i���I�����v�́A�ʂɑ��x�͗v��Ȃ��̂ŁA
	(�ꉞGu������) SDL �̑O�̕������ۂ��߂��������ǂ������m��Ȃ��B
	-------------------------------------------------------
	�{�Ƃ����̂Q�͑S�R�Ⴄ�����ɂȂ��Ă���ۂ��ł��B
---------------------------------------------------------*/

#include "scenario_script.h"

#include "kanji_system.h"

#include "111_my_file.h"


/*---------------------------------------------------------

---------------------------------------------------------*/

//extern int errno;

// _script_command

// [�e�L�X�g�̃J�[�\������]

// [�e�L�X�g�\��/�N���b�N�҂�]
//		CURSOR		/*	CURSOR	�J�[�\���̏����� */
//					/*	CURSOR [�p�~]	�L�������J�[�\���ʒu�̌Ăяo�� */
//					/*	CURSOR [�p�~]	�J�[�\���ʒu�̋L�� */
//		CLICK		/* �N���b�N�҂� */
//		TEXT		/* ���b�Z�[�W�E�B���h�E�ւ̏������� */

//		BG_LOAD 	/* LO ADBG		�w�i�t�@�C���̕ύX */
//		OBJ_LOAD	/* LO ADSP		�ėp�X�v���C�g�̃��[�h */
//		OBJ_SWAP	/* RE LO ADSP	�ėp�X�v���C�g�̍ēǂݍ��� */

//		OBJ_MOVE	/* MO VESP		�X�v���C�g�̈ړ� */
//		OBJ_SPEED	/* MO VESP		�X�v���C�g�̑��x */



//----------- [�p�~]
//		SUBG		/* [�p�~] [SCREEN]	SU BG�w�i�̕\��/��\�� */
//		SUTWINDOW	/* [�p�~] [SCREEN]	SU TWINDOW ���������G�E�B���h�E�̕\��/��\�� para2�̓X�R�A�p�l����on/off */

//----------- [�p�~]
//		CUR_HOME	/* [�p�~] [CURSOR]	CL CURSOR	�J�[�\���̏����� */
//		CUR_POP 	/* [�p�~] [CURSOR]	LO ADCUR	�L�������J�[�\���ʒu�̌Ăяo�� */
//		CUR_PUSH	/* [�p�~] [CURSOR]	SA VECUR	�J�[�\���ʒu�̋L�� */

//		SUSPRITE	/* [�p�~] [�p�~    ] SU SPRITE	�ėp�X�v���C�g�̕\��/��\�� */
//		OBJ_SET 	/* [�p�~] [�p�~    ] PA RAMSP	�ėp�X�v���C�g�̃A���t�@�Z�b�g/�A�j���[�V����	*/


//		BGTEXT		/*					 BG TEXT	�w�i�Ƀe�L�X�g��ł����� */
//		FILTER		/* [�p�~] [�p�~    ] FI LTER */
//		SUFILTER	/* [�p�~] [�p�~    ] SU FILTER	�t�B���^�[�\��/��\��&���ߓx */

//		SUWINDOW	/* [�p�~] [�p�~    ] SU WINDOW	���b�Z�[�W�E�B���h�E�̕\��/��\�� para2��bpp���x�B*/
//		SUL 		/* [�p�~] [SU SPRITE] SU L		�����G���X�v���C�g�̕\��/��\�� */
//		SUR 		/* [�p�~] [SU SPRITE] SU R		�����G�E�X�v���C�g�̕\��/��\�� */
//		JUMP		/* [�p�~] [�p�~    ] JU MP		pause�{�^��������������jump��B */ /*�p�~*/
//		LOADL		/* [�p�~] [LO ADSP ] LO ADL 	�����G�� �X�v���C�g�̃��[�h */
//		LOADR		/* [�p�~] [LO ADSP ] LO ADR 	�����G�E �X�v���C�g�̃��[�h */
//		WAIT		/* [�p�~] [�p�~    ]  wait */

//		TWINDOW 	/* TW INDOW[�p�~] [�p�~    ]  �����G�����E�B���h�E�̏����� */

enum
{
	I_CODE_DONE = 0,	/* �����ς�(���̃R�}���h�͏I�����) */
// [�e�L�X�g�̃J�[�\������]
//	I_CODE_CUR_POP,
//	I_CODE_CUR_PUSH,
	I_CODE_CUR_HOME,
// [�e�L�X�g�\��/�N���b�N�҂�]
	I_CODE_CLICK,
	I_CODE_BG_WRITE_TEXT,
	I_CODE_TEXT_WRITE_TEXT,
	I_CODE_TEXT,
//
	I_CODE_BGM,
	I_CODE_BGM_VOLUME,
//
//	I_CODE_FILTER,/*�p�~*/
//	I_CODE_JUMP,/*�p�~*/
	I_CODE_BG_LOAD,
//	I_CODE_LOAD_LR,/*�p�~*/
	I_CODE_OBJ_LOAD,
	I_CODE_OBJ_MOVE,
//	I_CODE_OBJ_SPEED,/*�p�~*/
//	I_CODE_OBJ_SET,/*�p�~*/
	I_CODE_OBJ_SWAP,

	I_CODE_SCREEN_BG,
	I_CODE_SCREEN_TEXT,
	I_CODE_SCREEN_PANEL,

	I_CODE_BOSS_LOAD,
	I_CODE_BOSS_START,

//	I_CODE_SUFILTER,/*�p�~*/
//	I_CODE_SUL,/*�p�~*/
//	I_CODE_SUR,/*�p�~*/
//	I_CODE_SUSPRITE,/*�p�~*/
//	I_CODE_SUWINDOW,/*�p�~*/
//	I_CODE_TWINDOW,/*�p�~*/
//	I_CODE_WAIT/*�p�~*/
};

typedef struct _scenario_script
{
	int i_code; 		/* Interprited Code. / Intermediate Language, Command ���Ԍ��� / �I��������ǂ��� */	//	int owattayo;
	int douji_chain;	// �������s���ǂ���
						// 0=�Ⴄ�� 1=1�� 2=2��
	int time_out;		/* 0:����, ����ȊO����ł͂Ȃ�  �e�X�N���v�g�R�}���h���Ƃ̏��������� */
//
	int para1;
	int para2;
	int para3;
//	int para4;
	struct _scenario_script *next;
//
	char para0[(7*32)/*200*/];/* 32�̔{�� */
} S_SCRIPT;/* == 256bytes */


typedef struct /*_script_sprite*/
{
	int cx256;			/* obj�̒��S���W, ���x�m�ۗp(256�Œ菬���_�`��) */ 	//	int/*int*/ x;		/* x���W���� */
	int cy256;			/* obj�̒��S���W, ���x�m�ۗp(256�Œ菬���_�`��) */ 	//	int/*int*/ y;		/* y���W���� */
//[4]
	int w;				/* obj�̕� */			//	int cw; 	/* ���S���W(��) */
	int h;				/* obj�̍��� */			//	int ch; 	/* ���S���W(����) */
//
	int target_x256;	/* �ڕW���W */
	int target_y256;	/* �ڕW���W */
//[8]
	int set_speed;		/* �ړ����x */		//	int move_wait;		/* �������������Ƃ��̐���p */
	int angle1024;		/* ���m�ȕ��� */	//	int r_course;	/* ��̂̕��� */
//[12]
	SDL_Surface *img;	/* Images */
//
//	u8 alpha255;				/* alpha�l */
//	u8 flags;					/* 0:��\��, 1:�\��, 2:tachie_window(2nd screen)�ɕ\��	�����ł͕\�������邩�ǂ������� */
} SCRIPT_SPRITE;

/*---------------------------------------------------------

---------------------------------------------------------*/

/*************�O���[�o���ϐ�**************/
/* zenkaku.c */

/*****************************************/

/*	psp�ł� �����ŏ��������Ă����퓮�삵�Ȃ��ꍇ������B
	�N�����ɏ��������Ȃ��ƃ_���B(PC�̃\�[�X��psp�ŃR���p�C�����ē����Ȃ������̂ЂƂ�) */



//
static S_SCRIPT *memory_script_list;			/* ���ߕێ��p */	/*=NULL*/
static S_SCRIPT *search_script_list;			/* ���ߎ��s�p */	/*=NULL*/
static S_SCRIPT *loading_script_list;			/* ���ߎ��W�p */
//
#define SPRITE_tachie_l (29&1)/*-3*/
#define SPRITE_tachie_r (30&1)/*-2*/
//#define SPRITE_tachie_m (31)/*-1*/
#define SCRIPT_TACHIE_OBJ_MAX		(2/*32*/)
static SCRIPT_SPRITE *std_obj[SCRIPT_TACHIE_OBJ_MAX/*32*/ /*20*/];		/* �ėp�X�v���C�g */
//static SCRIPT_SPRITE *tachie_r;
//static SCRIPT_SPRITE *tachie_l;
//


//

static int is_bg;	/*=0*/					/* �w�i�\��/��\���t���O */

//static int 380/*scenario_width*/; 			/* ���� */

/*---------------------------------------------------------
	�q�֐�
---------------------------------------------------------*/

/*---------------------------------------------------------

---------------------------------------------------------*/

static SDL_Surface *load_local01(char *filename, SDL_Surface *s2)// /*result*/, int alpha)
{
	char file_name[128];
	strcpy(file_name, DIRECTRY_NAME_DATA_STR "/" );
	strcat(file_name, filename);
//
	if (NULL != s2)
	{
		SDL_FreeSurface(s2);
		s2 = NULL;
	}
//
	SDL_Surface *s1;/*temporaly*/
//	s1 = NULL;
	s1 = IMG_Load(file_name);/*�����Ń��[�h����Ɗm���ɏ������������*/
	if ( NULL == s1 )
	{
		CHECKPOINT;
		error(ERR_FATAL, (char*)"script:cant load image %s:", file_name );
//		return (NULL);
	}
#if 0
	if (	(-1==alpha) /* �X�N���v�g���ŏȗ������ꍇ(�f�t�H���g) */
		||
			(0==alpha)	/* ���������ꍇ */
	)
#endif
	{
		/* �A���t�@�g��Ȃ� */
		s2 = SDL_DisplayFormat(s1);/*�T�[�t�F�X��\���t�H�[�}�b�g�ɕϊ�����B*/
		#if 1/* ����SDL�w�i������A story �ŗv��B */
		SDL_SetColorKey(s2, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);	/* �J���[�L�[(�����F�A�����F)�͍� */
		#endif
	}
#if 0
	else
	{
		/* �A���t�@�g�� */
		s2 = SDL_DisplayFormatAlpha(s1);/*�T�[�t�F�X��\���t�H�[�}�b�g�ɕϊ�����B*/
	}
#endif
	#if 0
	if ( NULL == s2 )
	{
		CHECKPOINT;
		error(ERR_FATAL, (char*)"cant convert image %s to display format:", file_name );
//		return (NULL);
	}
	#endif
	SDL_FreeSurface(s1);
	s1 = NULL;
	return (s2);
}

/*---------------------------------------------------------

---------------------------------------------------------*/
#include <malloc.h>/* free(); */

static void remove_script_sprite(SCRIPT_SPRITE *src)
{
	if (NULL != src)
	{
		SDL_FreeSurface(src->img);
		free(src);
		src = NULL;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void swap_my_sprite(S_SCRIPT *ssc)
{
	char *filename;
	int num;
//	int dummy_frames;
//	int alpha;
//
	filename		= ssc->para0;
	num 			= ssc->para1;
//	dummy_frames	= ssc->para2;
//	alpha			= ssc->para3;
//
	SCRIPT_SPRITE *tmp;
	if (-1==num)		{	return; 		}
//	else if (-2==num)	{	tmp=std_obj[SPRITE_tachie_r/*32-2*/];	}	/* right */
//	else if (-3==num)	{	tmp=std_obj[SPRITE_tachie_l/*32-3*/];	}	/* left */
	else				{	tmp=std_obj[(num)&(SCRIPT_TACHIE_OBJ_MAX-1)];	}	/* �ėp�X�v���C�g */
	if (NULL==tmp)		{	return; 		}

	SDL_FreeSurface(tmp->img);

	SDL_Surface *suf = NULL;
	suf 		= load_local01(filename, suf);//, 0/*alpha*/);
	tmp->img	= suf;
//	tmp->frames = frames;
	tmp->w		= (suf->w/*/frames*/);	//tmp->cw		= ((tmp->w)>>1);
	tmp->h		= (suf->h); 			//tmp->ch		= ((tmp->h)>>1);
}

/*---------------------------------------------------------

---------------------------------------------------------*/

	/*int dummy_frames, int dummy_alpha, SDL_Rect *rect*/
static /*SCRIPT_SPRITE **/void load_my_sprite(S_SCRIPT *ssc)
{
	char *filename;
	int sc_sp_num;
	int xx;
	int yy;
//
	filename	= ssc->para0;
	sc_sp_num	= ssc->para1;
	xx			= ssc->para2;
	yy			= ssc->para3;
//
	sc_sp_num &= ((SCRIPT_TACHIE_OBJ_MAX-1));
	if (NULL != std_obj[sc_sp_num]) 	{	remove_script_sprite(std_obj[sc_sp_num]); }
	SCRIPT_SPRITE *tmp;
	tmp 				= my_calloc(sizeof(SCRIPT_SPRITE));
	SDL_Surface *suf	= NULL;
//	suf 				= load_local02(filename, 0/*alpha*/);
	suf 				= load_local01(filename, suf);//, 0/*alpha*/);
	if (NULL==suf)		{ return /*(NULL)*/;			}
	std_obj[sc_sp_num]	= tmp;
	tmp->img			= suf;
//	if (frames==-1) 	{	tmp->frames = 1;		}
//	else				{	tmp->frames = frames;	}
	tmp->w				= (suf->w/*/frames*/);	//tmp->cw			= ((tmp->w)>>1);
	tmp->h				= (suf->h); 		//tmp->ch			= ((tmp->h)>>1);
//
//	tmp->ak tframe		= 0;
//	tmp->an im_speed	= 0;
//	tmp->an im_type 	= 0;
//	tmp->an im_time 	= 0;
//	tmp->an im_count	= 0;
	/*tmp->alpha255 	= 255;*/	/*tmp->color32		= MAKE32RGBA(0xff, 0xff, 0xff, 0xff);*/
//	tmp->flags			= 0;
//	tmp->move_wait		= 0;
	tmp->cx256			= (xx<<8);	/*rect->x*/
	tmp->cy256			= (yy<<8);	/*rect->y*/
//	tmp->x				= rect->x-(rect->w*tmp->w);
//	tmp->y				= rect->y-(tmp->h);
//	tmp->x				= xx-(0*tmp->w);
//	tmp->y				= yy-(tmp->h);

//	return (tmp);
}


/*---------------------------------------------------------

---------------------------------------------------------*/

//static int inits; 							/* �e�X�N���v�g�R�}���h���Ƃ̏��������� */

/*---------------------------------------------------------
	I_CODE_OBJ_MOVE �X�v���C�g�̈ړ�
---------------------------------------------------------*/
//		int n = parth_str_right_or_str_left_or_number(l_or_r);
//		if (n==STR_ERROR/*-1*/) 		{	return (2/*-1*/);	}/*�X�N���v�g�I��*/
	//	else if (n==STR_RIGHT/*-2*/)	{	sc = tachie_r;	}	/* right */
	//	else if (n==STR_LEFT/*-3*/) 	{	sc = tachie_l;	}	/* left */
	//	else

//static void set_sprite_speed( int num, int speed_aaa)
//{
//	SCRIPT_SPRITE *tmp;
//	tmp=std_obj[(num)&(SCRIPT_TACHIE_OBJ_MAX-1)];		/* �ėp�X�v���C�g */
//	//std_obj[((num)&(SCRIPT_TACHIE_OBJ_MAX-1))].set_speed = speed_aaa;
//	tmp->set_speed = speed_aaa;
//}
static int do_move_script_sprite(S_SCRIPT *ssc)/*, int speed_aaa, int alpha255*/	/*,ssc->para4*/
{
	char *color32_str;
	int num;
	int x;
	int y;
//
	color32_str = ssc->para0;
	num = ssc->para1;
	x = ssc->para2;
	y = ssc->para3;
//
	SCRIPT_SPRITE *my_std_obj;
	my_std_obj = std_obj[((num)&(SCRIPT_TACHIE_OBJ_MAX-1))];	/* �ėp�X�v���C�g */
//
	if (0 == ssc->time_out/*inits*/)	/* ����̂ݍs�� */
	{
		ssc->time_out = 1;/*����ł͂Ȃ�*/
		/* ���炩���߈ړ������ʒu���Z�o���Ă��� */
		my_std_obj->target_x256 = my_std_obj->cx256 + (x<<8);		/* �ړ��������W */
		my_std_obj->target_y256 = my_std_obj->cy256 + (y<<8);		/* �ړ��������W */
//		my_std_obj->cx256		= ((my_std_obj->cx256)/*<<8*/);		/* ���x�m�ۗp */
//		my_std_obj->cy256		= ((my_std_obj->cy256)/*<<8*/);		/* ���x�m�ۗp */
		/* �ړ����������߂� */
	//	my_std_obj->angle1024	= at an_1024(y,x);
		my_std_obj->angle1024	= atan_65536(y,x);
		my_std_obj->angle1024	>>= (6);
	//	my_std_obj->color32 	= ((alpha255)<<8);	/* �F */
	//	my_std_obj->alpha		= ((alpha255)<<8);	/* �F */
		if ('f'==color32_str[2])
		{
		//	my_std_obj->alpha255 = (255);	/* �F */
			SDL_SetAlpha(my_std_obj->img, SDL_SRCALPHA, (255) );
		}
		else
		{
		//	my_std_obj->alpha255 = (180);	/* �F */
			SDL_SetAlpha(my_std_obj->img, SDL_SRCALPHA, (180) );
		}
//		my_std_obj->set_speed = ((40)<<(8-3));	/* �����x (x32) */
//		my_std_obj->set_speed = ((20)<<(8-3));	/* �����x (x32) */
		my_std_obj->set_speed = (0);			/* �����x (x32) */
	}
//
	{
		/* 10�Ŋ���̂͒x�߂���̂ŁA3�r�b�g�V�t�g����(1/8)�̒l�ɂ��� */
		int speed256;
		speed256 = (/*speed_aaa*/(my_std_obj->set_speed));		/* ���x�m�ۗp */
	//	my_std_obj->set_speed += (5*32);			/* 160==5x32(������) �����x(�����̗����G�͏�ɉ������Ĉړ�����) (x32) */
	//	my_std_obj->set_speed += (4*32);			/* 128==4x32() �����x(�����̗����G�͏�ɉ������Ĉړ�����) (x32) */
		my_std_obj->set_speed += (3*32);			/*	96==3x32() �����x(�����̗����G�͏�ɉ������Ĉړ�����) (x32) */
		if (t256(6.0)<speed256) speed256 = t256(8.0);		/* �ő呬�x8[dots/flame] t256(6.0) */

		#if 0
		/* CW�̏ꍇ */
	//	my_std_obj->x += ((cos512(my_std_obj->angle512)*speed)/10);
	//	my_std_obj->y += ((sin512(my_std_obj->angle512)*speed)/10);
	//	my_std_obj->cx256 += ((cos512(my_std_obj->angle512)*speed256)>>8);	/* ���x�m�ۗp */
	//	my_std_obj->cy256 += ((sin512(my_std_obj->angle512)*speed256)>>8);	/* ���x�m�ۗp */
		#else
		/* CCW�̏ꍇ */
		my_std_obj->cx256 += ((sin1024(my_std_obj->angle1024)*speed256)>>8); /* ���x�m�ۗp */
		my_std_obj->cy256 += ((cos1024(my_std_obj->angle1024)*speed256)>>8); /* ���x�m�ۗp */
		#endif
	}
//
//	my_std_obj->cx256	= (/*t256_floor*/(my_std_obj->cx256));		/* ���x�m�ۗp */
//	my_std_obj->cy256	= (/*t256_floor*/(my_std_obj->cy256));		/* ���x�m�ۗp */
	/* ��̂̕���: �Ȃ̂� �S�T�x��]���Ă��炵�āA��ʂQ�r�b�g����܂��ȕ��� */
	#if 0
	/* CW�̏ꍇ */
	switch ((((my_std_obj->angle512+64/*deg_360_to_512(45)*/) & 0x180 )))/*my_std_obj->r_course*/
	{	/* �ړ��������W�ɓ��������͂ݏo������A�����Ƃ���B */
	case (0<<7)/* 1:��(512_0) */:	if (my_std_obj->cx256 >= my_std_obj->target_x256)	{	goto move_complete; 	}	break;
	case (1<<7)/* 2:��(512_1) */:	if (my_std_obj->cy256 <= my_std_obj->target_y256)	{	goto move_complete; 	}	break;
	case (2<<7)/* 3:��(512_2) */:	if (my_std_obj->cx256 <= my_std_obj->target_x256)	{	goto move_complete; 	}	break;
	case (3<<7)/* 0:��(512_3) */:	if (my_std_obj->cy256 >= my_std_obj->target_y256)	{	goto move_complete; 	}	break;
	}
	#else
	/* CCW�̏ꍇ */
//	switch ((((my_std_obj->angle512-64/*deg_360_to_512(45)*/) & 0x180 )))/*my_std_obj->r_course*/
	switch ((((my_std_obj->angle1024-128/*deg_360_to_512(45)*/) & 0x300 )))/*my_std_obj->r_course*/
	{	/* �ړ��������W�ɓ��������͂ݏo������A�����Ƃ���B */
//	case (0<<7)/* 2:��(512_1) */:	if (my_std_obj->y <= my_std_obj->target_y)	{	goto move_complete; 	}	break;
//	case (1<<7)/* 1:��(512_0) */:	if (my_std_obj->x >= my_std_obj->target_x)	{	goto move_complete; 	}	break;
//	case (2<<7)/* 0:��(512_3) */:	if (my_std_obj->y >= my_std_obj->target_y)	{	goto move_complete; 	}	break;
//	case (3<<7)/* 3:��(512_2) */:	if (my_std_obj->x <= my_std_obj->target_x)	{	goto move_complete; 	}	break;
	case (1<<8)/* 2:��(1024_1) */:	if (my_std_obj->cy256 <= my_std_obj->target_y256)	{	goto move_complete; 	}	break;
	case (0<<8)/* 1:��(1024_0) */:	if (my_std_obj->cx256 >= my_std_obj->target_x256)	{	goto move_complete; 	}	break;
	case (3<<8)/* 0:��(1024_3) */:	if (my_std_obj->cy256 >= my_std_obj->target_y256)	{	goto move_complete; 	}	break;
	case (2<<8)/* 3:��(1024_2) */:	if (my_std_obj->cx256 <= my_std_obj->target_x256)	{	goto move_complete; 	}	break;
	}
	#endif
	return (0);/*�ړ���*/
move_complete:
	/* �ړ����������ꍇ�́A���m�ȍ��W�ɏC������B */
	my_std_obj->cx256 = my_std_obj->target_x256;
	my_std_obj->cy256 = my_std_obj->target_y256;
	return (1);/*�ړ�����*/
}

/*---------------------------------------------------------

---------------------------------------------------------*/

global /*static*/ int draw_script_screen/*=0*/; 				/* ����ӃE�B���h�E�\���t���O */

/*---------------------------------------------------------
	�V�X�e������(???)
---------------------------------------------------------*/

/*---------------------------------------------------------

---------------------------------------------------------*/

static void load_script_free(void)
{
	S_SCRIPT *l = memory_script_list;
	S_SCRIPT *n;
	while (NULL != l)
	{
		n = l->next;
		free (l);
		l = n;
	}
	memory_script_list = (S_SCRIPT *)NULL;
	loading_script_list = (S_SCRIPT *)NULL;
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static char *load_command(char *c, char *buffer, int *end_arg)
{
	int i = 0;
	while ((*c != ' ') && (*c != 13))
	{
		i++;
		if (i >= 10)
		{	return ((char *)NULL);}
		*buffer++ = *c++;
	}
	if (*c == 13)	/* ���s�R�[�h�� OD OA */
	{
		*end_arg = 1;
	}
	*buffer = 0;		//NULL
	return (c);
}


/*---------------------------------------------------------

---------------------------------------------------------*/

static char *load_int(char *ccc, int *number, int *line_terminate_end_flag)
{
	char buffer[32/*20*/];
	char *ddd = buffer;
	int i = 0;
	while ((isdigit(*ccc))||('-'==(*ccc)))		/* �����ɂ��Ή� / isdigit : �������ǂ����̔��� */
	{
		i++;
		if (i >= 32/*20*/)
		{	goto ne222;}
		*ddd++ = *ccc++;
	}
/* ',' �܂��� '\n' ��������I�� */

//
	if (13==(*ccc)) /* ���s�R�[�h�� OD OA */
	{
		*line_terminate_end_flag = 1;
	}
	*ddd = 0;
	if (((','==(*ccc)) || (13==(*ccc))) && (','==(*(ccc-1))))	{	*number = -1; }
	else														{	*number = atoi(buffer); }
	return (ccc);
/*error*/
ne222:
	return ((char *)NULL);
}

/*---------------------------------------------------------

---------------------------------------------------------*/
enum
{
	PARAM_01 = 0,
	PARAM_02,
	PARAM_03,
//	PARAM_04,/*���g�p*/
//	PARAM_05,/*���g�p*/
//	PARAM_06,/*���g�p*/
//	PARAM_07,/*���g�p*/
//	PARAM_08,/*���g�p*/
	PARAM_MAX/*�ő吔*/
};

static void regist_script(
	char *command,
	char *c_p0,
//	int c_p1,	//c_pn[0]
//	int c_p2,	//c_pn[1]
//	int c_p3,	//c_pn[2]
//	int c_p4,	//c_pn[3]
//	int c_p5,	//c_pn[4]
//	int c_p6,	//c_pn[5]
//	int c_p7,	//c_pn[6]
	const int *c_pn,
	int chain)
{
/*
	��{�`
	command para0,para1,para2,para3
*/
	S_SCRIPT *new_script;
	new_script				= my_calloc(sizeof(S_SCRIPT));

	strcpy(new_script->para0, c_p0);
	new_script->para1		= /*c_p1*/c_pn[PARAM_01];	/* �f�t�H���g */
	new_script->para2		= /*c_p2*/c_pn[PARAM_02];	/* �f�t�H���g */
	new_script->para3		= /*c_p3*/c_pn[PARAM_03];	/* �f�t�H���g */
//	new_script->para4		= /*c_p3*/c_pn[PARAM_04];	/* �f�t�H���g */
//
//	new_script->i_code		= I_CODE_DONE;/*owattayo	= 0;*/
	new_script->douji_chain = chain;
	new_script->next		= NULL;
	new_script->time_out	= 0;


	switch (*command)
	{
	case 'B':
					if (0==tiny_strcmp(command, "BG_LOAD")) 	{	new_script->i_code = I_CODE_BG_LOAD;			}	/* �w�i�t�@�C���̕ύX */
			else	if (0==tiny_strcmp(command, "BGM")) 		{	new_script->i_code = I_CODE_BGM;				}	/* BGM�ύX */
			else	if (0==tiny_strcmp(command, "BGM_VOLUME"))	{	new_script->i_code = I_CODE_BGM_VOLUME; 		}	/* BGM ���� �ύX */
		if (0==tiny_strcmp(command, 			"BOSS"))
		{
					if (0==tiny_strcmp(c_p0,	"load"))		{	new_script->i_code = I_CODE_BOSS_LOAD;			}	/* �{�X�ǂݍ��� */
			else	if (0==tiny_strcmp(c_p0,	"start"))		{	new_script->i_code = I_CODE_BOSS_START; 		}	/* �{�X�U���J�n�B */
		}
		break;

		break;
	case 'C':
		if (0==tiny_strcmp(command, 			"CURSOR"))
		{
					if (0==tiny_strcmp(c_p0,	"home"))		{	new_script->i_code = I_CODE_CUR_HOME;			}	/* �J�[�\���̏����� */
//			else	if (0==tiny_strcmp(c_p0,	"load"))		{	new_script->i_code = I_CODE_CUR_POP;			}	/* �L�������J�[�\���ʒu�̌Ăяo�� */
//			else	if (0==tiny_strcmp(c_p0,	"save"))		{	new_script->i_code = I_CODE_CUR_PUSH;			}	/* �J�[�\���ʒu�̋L�� */
			else	if (0==tiny_strcmp(c_p0,	"click"))		{	new_script->i_code = I_CODE_CLICK;				}	/* �N���b�N�҂� */
			else	if (0==tiny_strcmp(c_p0,	"text"))		{	new_script->i_code = I_CODE_TEXT_WRITE_TEXT;	}	/* ���b�Z�[�W�E�B���h�E�Ɋ����̕�����`���l�ɐݒ肷��B */
			else	if (0==tiny_strcmp(c_p0,	"bg"))			{	new_script->i_code = I_CODE_BG_WRITE_TEXT;		}	/* BG�E�B���h�E�Ɋ����̕�����`���l�ɐݒ肷��B */
		}
		break;
	case 'O':
					if (0==tiny_strcmp(command, "OBJ_MOVE"))	{	new_script->i_code = I_CODE_OBJ_MOVE;			}	/* �X�v���C�g�̈ړ� */
			else	if (0==tiny_strcmp(command, "OBJ_LOAD"))	{	new_script->i_code = I_CODE_OBJ_LOAD;			}	/* �ėp�X�v���C�g�̃��[�h */
			else	if (0==tiny_strcmp(command, "OBJ_SWAP"))	{	new_script->i_code = I_CODE_OBJ_SWAP;			}	/* �X�v���C�g�̍ēǂݍ��� */
//			else	if (0==tiny_strcmp(command, "OBJ_SPEED"))	{	new_script->i_code = I_CODE_OBJ_SPEED;			}	/* �X�v���C�g�̑��x�ݒ� */
		break;
	case 'S':
		if (0==tiny_strcmp(command, 			"SCREEN"))
		{
					if (0==tiny_strcmp(c_p0,	"panel"))		{	new_script->i_code = I_CODE_SCREEN_PANEL;		}	/* �X�R�A�p�l���̕\��/��\�� */
			else	if (0==tiny_strcmp(c_p0,	"text"))		{	new_script->i_code = I_CODE_SCREEN_TEXT;		}	/* �e�L�X�g��ʂ̕\��/��\�� */
			else	if (0==tiny_strcmp(c_p0,	"bg"))			{	new_script->i_code = I_CODE_SCREEN_BG;			}	/* �w�i�̕\��/��\�� */
		}
		break;
	case 'T':
					if (0==tiny_strcmp(command, "TEXT"))		{	new_script->i_code = I_CODE_TEXT;				}	/* ���b�Z�[�W�E�B���h�E�ւ̏������� */
		break;
	}

	if (NULL==loading_script_list)/* �擪�Ȃ� */
	{
		loading_script_list 		= new_script;
	}
	else
	{
		loading_script_list->next	= new_script;
		loading_script_list 		= loading_script_list->next;
	}

	if (NULL==memory_script_list)
	{	memory_script_list			= loading_script_list;	}
}



/*---------------------------------------------------------
	�X�N���v�g�ǂݍ���
	-------------------------------------------------------
	���̊֐��́A�d�l��u�ƂĂ������������܂��v

---------------------------------------------------------*/

static int s_load_scenario(void)
{
/*
	��{�`
	command para0,para1,para2,para3
*/

//	FILE *fp;
	if (NULL == (/*fp =*/my_fopen(/*my_fopen_file_name*/ /*,"r"*/)))		/* �J���Ȃ������Ƃ� */
	{
		return (0);
	}

	int opecode_entrys		= 0;	/* ���߂����������܂�Ă��邩�ǂ����B */
	int line_num			= 0;	/* �s�ԍ� */
	int opecode_chains		= 0;	/* �A���������� */
	while (/*NULL*/0 != my_file_fgets(/*buffer_text_1_line,255,fp*/))
	{
		/****************** script_data �p ******************/
		char char_command[16/*15*/];		/* ��{�R�}���h */
		char c_p0[256/*200*/];
		int c_pn[PARAM_MAX/*6*/];
		{
			int kk;
			for (kk=0; kk<PARAM_MAX/*6*/; kk++)
			{
				c_pn[kk] = (-1);	/* ���w��̏ꍇ �𔻕ʂ���� */
			}
		}
		/****************** script_search �p ****************/
		int end_arg;	end_arg = 0;		/* �s�� == �����̎擾�̒��~ */
		line_num++;
//
		char *ch;					/* �����ʒu */
		ch = buffer_text_1_line;
		/* skiped lines. */
		#if 0
		/* '\n'�������̂��I�������Ȃ�(???) */
		if ('\n'==(*ch))		{	continue;	}	/* ���s�݂̂̍s�͋�s�Ȃ̂ł��Ȃ��łƂ΂� */
		while (isspace(*ch))	{	ch++;		}	/* �󔒂�TAB������ */
		#else
		{my_isspace:;
			if (' '>=(*ch))
			{
				ch++;
				if (0x0a==(*ch))
				{	/*goto loop;*/continue; }	/* skiped null line. */
				else
				{	goto my_isspace;	}
			}
		}
		#endif
		/* ';'�Ŏn�܂�s�̓R�����g�s�Ȃ̂ŁA���̍s�܂Ŕ�΂��B */
//		if (';'==(*ch)) 	{	continue;	}	/* ';'�Ŏn�܂�s�̓R�����g�Ȃ̂ł��Ȃ��łƂ΂� */
		/* '-'�Ŏn�܂�s�̓X�N���v�g�`�F�C���@�\(�����X�N���v�g�s�A�������s�@�\)�B */
		if ('-'==(*ch)) 	{	ch++;	opecode_chains++;	opecode_chains &= 0x0f; }	/* ���[�N��16�܂ł����Ȃ��̂ōő�16���� */
		else				{	opecode_chains = 0; }
//
	#if (1==USE_PARTH_DEBUG)
		#define GOTO_ERR_WARN goto err_warn
	#else
		#define GOTO_ERR_WARN continue
	#endif
		/* parth start */
		ch = load_command(ch, char_command, &end_arg);		/* ��{�R�}���h����(�I�y�R�[�h)�ǂݍ���  */
		if (NULL==ch)										{	GOTO_ERR_WARN;	}	/* �ǂ߂Ȃ���΃G���[ */
		if (!end_arg)/* �s�� */
		{
			if (' ' != (*ch))								{	GOTO_ERR_WARN;	}	/* ��؂� */
			ch++;
			ch = load_my_file_get_str(ch, c_p0, &end_arg/*, ','*/); 						/* ������R�}���h(�I�y�����h)�ǂݍ���  */
			if (NULL==ch)									{	GOTO_ERR_WARN;	}	/* �ǂ߂Ȃ���΃G���[ */
		}
		{
			int kk;
			for (kk=0; kk<PARAM_MAX/*(6)*/; kk++)
			{
				if (!end_arg)
				{
					if (',' != (*ch))						{	GOTO_ERR_WARN;	}	/* ��؂� */
					ch++;
					ch = load_int(ch, &c_pn[kk], &end_arg); 						/* ���l�R�}���h(�I�y�����h)�ǂݍ���  */
					if (NULL==ch)							{	GOTO_ERR_WARN;	}	/* �ǂ߂Ȃ���΃G���[ */
				}
			}
		}
	//	regist_script(char_command, c_p0, c_pn[0],c_pn[1],c_pn[2],c_pn[3],c_pn[4],c_pn[5],c_pn[6], opecode_chains);
		regist_script(char_command, c_p0, c_pn, opecode_chains);
		opecode_entrys++;
		continue;
	#if (1==USE_PARTH_DEBUG)
	err_warn:
		error(ERR_WARN, (char*)"syntax error in scriptfile '%s', line no: %d", my_fopen_file_name, line_num);
		continue;
	#endif
	}
	my_fclose (/*fp*/);
	//return (opecode_entrys);
	if (0 == opecode_entrys)
	{
		#if 0
		//ps pDebugScreenPrintf("can't entrys from this file %s", my_fopen_file_name);
		//sc eKernelDelayThread(3000000);
		#endif
		error(ERR_WARN, (char*)"can't entrys from this file %s", my_fopen_file_name);
		return (0);
	}
	return (1);
}

/*---------------------------------------------------------
	�X�N���v�g�V�X�e���p
	�J�[�\���ʒu�L���b�V��
---------------------------------------------------------*/

static int cursor_x_chached;			/* �J�[�\���ʒu �ۑ��p */
static int cursor_y_chached;			/* �J�[�\���ʒu �ۑ��p */

/*---------------------------------------------------------
	�X�N���v�g�ɂ�����w�i
---------------------------------------------------------*/

#if (1)
static UINT16 *bg_story_window_surface_image;
static int bg_story_window_surface_pitch;
static int bg_story_window_surface_width;
#endif

static SDL_Surface *bg_story_window_surface;			/* �X�N���v�g�ɂ�����w�i */

/*---------------------------------------------------------

---------------------------------------------------------*/

//static extern int my_string_offset;
/*static*/ extern void clear_my_string_offset(void);
static void aaa_script_reset(void)
{
	int i;
	for (i=0; i<SCRIPT_TACHIE_OBJ_MAX/*20*/; i++)
	{
		remove_script_sprite(std_obj[i]);
	}
//	remove_script_sprite(std_obj[SPRITE_tachie_r]);
//	remove_script_sprite(std_obj[SPRITE_tachie_l]);
	if (NULL != bg_story_window_surface)
	{
		SDL_FreeSurface(bg_story_window_surface);
		bg_story_window_surface 	= NULL;
	}
	is_bg = 0;/* �J�������񂾂��� bg �\���ł��Ȃ��A���������ċ����\�� off */
	//

	home_cursor();			/* �J�[�\�����z�[���|�W�V�����ֈړ� */	/* �J�[�\���ʒu */
	cursor_x_chached = 0;	/* �J�[�\���ʒu �ۑ��p */
	cursor_y_chached = 0;	/* �J�[�\���ʒu �ۑ��p */

	clear_my_string_offset();	//	my_string_offset=0;
}

/*---------------------------------------------------------
	is_bg �� 1 �ɂȂ�\���̂���ꏊ�́Abg_story_window_surface ��������
	�`��Ńn���O�A�b�v���邩��A�`�F�b�N����B
	bg_story_window_surface �������ꍇ�͋���off�B
---------------------------------------------------------*/

static void check_is_bg(void)
{
	if (NULL == bg_story_window_surface)
	{
		is_bg = 0;/* �J�������񂾂��� bg �\���ł��Ȃ��A���������ċ����\�� off */
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
static int bg_alpha;		/* �w�i���l�p */
static void load_bg_aaa(char *filename, int alpha)
{
	bg_story_window_surface = load_local01(filename, bg_story_window_surface);//, alpha);
	{
		bg_story_window_surface_image	= (UINT16 *)bg_story_window_surface->pixels;
		bg_story_window_surface_pitch	= bg_story_window_surface->pitch;
		bg_story_window_surface_width	= bg_story_window_surface->w;
	}
	#if 0/* ����A�v��Ȃ��H�B */
	SDL_SetColorKey(bg_story_window_surface, (SDL_SRCCOLORKEY|SDL_RLEACCEL), 0x00000000);
	#endif
}
static int load_my_bg(S_SCRIPT *ssc)
{
	char *filename;
	int alpha;
	int fade_command;
	int set_alpha_speed;
//
	filename		= ssc->para0;
	alpha			= ssc->para1;
	fade_command	= ssc->para2;
	set_alpha_speed = ssc->para3;
//
	static int bg_alpha_speed;		/* �w�i���l�p */
	switch (fade_command)
	{
	case (-1):
		load_bg_aaa(filename, alpha);
	//	ssc->owattayo = 1;
		return (1); /* �������� */
//		break;
	case 1:/*fade_in_command*/
		if (0 == ssc->time_out/*inits*/)	/* ����̂ݍs�� */
		{
			ssc->time_out = 1;/*����ł͂Ȃ�*/
		//if (inits)/*fade_in_command_start*/
		//{
		//	if (filename)
			{
				load_bg_aaa(filename, 0/*alpha*/);
			}
		//	ssc->owattayo = 1;
			//
			bg_alpha = 0;
		//	if (-1 == set_alpha_speed)/*�p�����[�^�[�ȗ���*/
			{
				set_alpha_speed = 5;
			}
			bg_alpha_speed = set_alpha_speed;
		//	inits=0;
			is_bg				= 1;	check_is_bg();
		}
		else
		{
//			psp_clear_screen();
//			SD L_SetAlpha(bg_story_window_surface, SD L_SRCALPHA, bg_alpha);
//			SD L_BlitSurface(bg_story_window_surface, NULL, sdl_screen[SD L_00_SCREEN], NULL);
			bg_alpha += bg_alpha_speed/*5*/;
			if (250 < bg_alpha)
			{
				bg_alpha = 255;
				#if 0/*???*/
				/*	�����ǂ��킩��Ȃ����ǁA�����͂�������???�B */
				if (NULL != bg_story_window_surface)
				{
					unloadbmp_by_surface(bg_story_window_surface);	//�L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bbg_story_window_surface = NULL;
				}
				#endif
				return (1); 	/* �������� */
			}
		}
		break;
//	case 2:/*fade_out_command_start*/
//		if (inits)
//		{
//			inits=0;
//		}
//		else
//		{
//		//	if ()
//			{
//			//	return (1); /* �������� */
//			}
//		}
//		break;
	}
	return (0); 	/* ������ */
}
#define USE_SDL_DRAW (1)
#if (1==USE_SDL_DRAW)

/*---------------------------------------------------------
	�X�v���C�g�I�u�W�F�N�g�̕`��/����
---------------------------------------------------------*/

static void draw_my_sprite(SCRIPT_SPRITE *std_obj_nnn_/*int nnn*/)
{
//	int nnn;
//	for (nnn=start; nnn<=end; nnn++)
	{
		if (NULL != std_obj_nnn_)
		{
		//	if (std_obj_nnn_->flags)	/* �ėp�X�v���C�g�̕\��/��\�� */
			{
				SDL_Rect src_r;
				SDL_Rect dst_r;
				src_r.x = (0)/*std_obj_nnn_->w*std_obj_nnn_->aktframe*/;
				src_r.y = (0);
				src_r.w = std_obj_nnn_->w;
				src_r.h = std_obj_nnn_->h;
				dst_r.x = (t256_floor(std_obj_nnn_->cx256));
				dst_r.y = (t256_floor(std_obj_nnn_->cy256));
				SDL_BlitSurface(std_obj_nnn_->img, &src_r, sdl_screen[SDL_00_VIEW_SCREEN], &dst_r);
			}
			/* ����/�A�j���[�V���� */
		}
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void s_draw_the_script(void)
{
	/* ----- �`��n ----- */
	/* 1. �܂��w�i��`�� */
	if (is_bg)
	{
		if (255 > bg_alpha)
		{
			SDL_SetAlpha(bg_story_window_surface, SDL_SRCALPHA, bg_alpha);
		}
		SDL_BlitSurface(bg_story_window_surface, NULL, sdl_screen[SDL_00_VIEW_SCREEN], NULL);
	}
	/* 2. ���ɗ����G��`�� */
	draw_my_sprite(std_obj[SPRITE_tachie_l]);/* �����G 1�� */
	draw_my_sprite(std_obj[SPRITE_tachie_r]);/* �����G 1�� */
}
#endif /* (1==USE_SDL_DRAW) */


/*---------------------------------------------------------
	��蒼���K�v������B
	���ׂ�����̂ŕ`���ɂ����o���Ȃ��B
---- ��肽������ ----
	1�t���[���ł͏I���Ȃ��������������Ƃ��̂��߂ɂ��̃R�}���h����̖��߂ɂ͑S��
	�Y���R�}���h�I���̏ꍇ�A���ԃR�[�h sc->i_code = I_CODE_DONE;�ɑ���ŏI���B
	���chain���m�F���A0�ȊO�̒l�������Ă����玟�̊֐������s����(next��H��)�B
---------------------------------------------------------*/

static int script_skip_mode;

enum
{
	SCRIPT_00_CONTINUE = 0,
	SCRIPT_01_TERMINATE,
};
static int script_terminate_flag;
static /*int*/void work_the_script(void)
{
	S_SCRIPT *ssc;
	ssc = search_script_list;/* �����J�n�ʒu */
//
	int shot_ositenai;
	shot_ositenai = 1;	/* �V���b�g�����ĂȂ� */
	if (0 == my_pad)/* ���͉����ĂȂ� */
	{
		if (my_pad_alter & PSP_KEY_BOMB_CANCEL) 	/* �L�����Z������ */
		{
			script_skip_mode = 1;/*on*/
			#if 0
			while ((NULL != ssc->next)
			//	&& (I_CODE_JUMP != ssc->i_code)/*�p�~*/
				)
			{
				ssc->i_code = I_CODE_DONE;
				ssc = ssc->next;
			}
			if (NULL == ssc->next)
			{
				script_terminate_flag = SCRIPT_01_TERMINATE;/*(1)*/
				return;
			}
			#endif
		}
		if (my_pad_alter & PSP_KEY_SHOT_OK) 		/* �n�j���� */
		{
			shot_ositenai = 0;	/* �V���b�g�������� */
		}
	}
//
	if (0!=script_skip_mode)
	{
		shot_ositenai = 0;	/* �V���b�g�������� */
	}
//
	/* ----- �X�N���v�g�R�}���h����(���s)�n ----- */
//	/*static*/ int n9;			/* ���̖��߂ɐi�߂邩�̔��� */
//	n9 = 0; 					/* ���̖��߂ɐi�߂邩�̔��� */
	while (1)
	{
	//	if (0==ssc->owattayo)
		{
			switch (ssc->i_code)
			{
			// [�e�L�X�g�̃J�[�\������]
//			case I_CODE_CUR_POP:		/* �J�[�\���ʒu�A���� */
//				cursor_x = cursor_x_chached;
//				cursor_y = cursor_y_chached;
//				ssc->i_code = I_CODE_DONE;
//				break;
//			case I_CODE_CUR_PUSH:		/* �J�[�\���ʒu�A�L�� */
//				cursor_x_chached = cursor_x;
//				cursor_y_chached = cursor_y;
//				ssc->i_code = I_CODE_DONE;
//				break;
			case I_CODE_CUR_HOME:		/* �J�[�\���̏����� */
				home_cursor();				/* �J�[�\�����z�[���|�W�V�����ֈړ� */
				ssc->i_code = I_CODE_DONE;
				break;
			// [�e�L�X�g�\��/�N���b�N�҂�]
			case I_CODE_CLICK:
				if (0 == shot_ositenai)
				{	ssc->i_code = I_CODE_DONE;	}
				break;
			case I_CODE_BG_WRITE_TEXT:
				//set_write_text(1);		/* BG�E�B���h�E�Ɋ����̕�����`���l�ɐݒ肷��B */
				ssc->i_code = I_CODE_DONE;
				break;
			case I_CODE_TEXT_WRITE_TEXT:
				//set_write_text(0);		/* ���b�Z�[�W�E�B���h�E�Ɋ����̕�����`���l�ɐݒ肷��B */
				ssc->i_code = I_CODE_DONE;
				break;
			case I_CODE_TEXT:
			{	static int tmp_all[16/*15*/];	/* �����R�ɂ��g�����������B */
				if (0 == ssc->time_out/*inits*/)	/* ����̂ݍs�� */
				{
					ssc->time_out = 1;/*����ł͂Ȃ�*/
				//if (inits)
				//{
					int i;
					for (i=0; i<16/*15*/; i++)
					{
						tmp_all[i] = 0;
					}
				}

				if (0 == tmp_all[(ssc->douji_chain)])
				{
					if (0 == shot_ositenai) 	{	ssc->para2=0;	}	/* �V���b�g��������A�c��͑S������ */
					/*sdl_lock_surface,*/
					tmp_all[(ssc->douji_chain)] = print_kanji000(ssc->para0, ssc->para1, ssc->para2);
				}
				else	/*if (1==tmp_all[(ssc->chain)])*/
				{
					if (0x00 == (ssc->para3 & 0xf8))	/* 0���W�X�^�Ɣ�r */
					{
						if ( 0x00 == ( (ssc->para3 & shot_ositenai) & 0x01) )	/* 0���W�X�^�Ɣ�r */
						{
							if (0x00 != (ssc->para3 & 0x02))	/* 0���W�X�^�Ɣ�r */
							{
								home_cursor();				/* �J�[�\�����z�[���|�W�V�����ֈړ� */
							}
						//			//cursor_continue = 1;	/* �J�[�\���p�� */
							if (0x00 != (ssc->para3 & 0x04))	/* 0���W�X�^�Ɣ�r */
							{
								kanji_window_clear();	/* ���b�Z�[�W�E�B���h�E�̓��e�������B */
							//	msg_window_init();/*???*/		/* �E�B���h�E������ */
							}
						//
							ssc->i_code = I_CODE_DONE;
							tmp_all[(ssc->douji_chain)] = 0;
						}
					}
				}
			}
				break;
#if 0
/*
	switch (ssc->para3)
	w: 0:���Ȃ� 	   1:�E�B���h�E������
	c: 0:�J�[�\���p��  1:�J�[�\��������
	b: 0:���s�B 	   1:�{�^��������������s�B
		   wcb
	case 0:000: ����������		 ���̖��߂ցA�J�[�\���p��
	case 1:001: �{�^������������ ���̖��߂ցA�J�[�\���p��
	case 2:010: ����������		 ���̖��߂ցA�J�[�\��������
	case 3:011: �{�^������������ ���̖��߂ցA�J�[�\��������
	case 4:100: ����������		 ���̖��߂ցA�J�[�\���p��	 �E�B���h�E������
	case 5:101: �{�^������������ ���̖��߂ցA�J�[�\���p���A  �E�B���h�E������
	case 6:110: ����������		 ���̖��߂ցA�J�[�\���������A�E�B���h�E������
	case 7:111: �{�^������������ ���̖��߂ցA�J�[�\���������A�E�B���h�E������
*/
#endif

			case I_CODE_BG_LOAD:
			//	bg_story_window_surface = load_local01(ssc->para0, bg_story_window_surface);//, 0 ssc->para1);
			//	SD L_SetColorKey(bg_story_window_surface, SD L_SRCCOLORKEY|SD L_RLEACCEL, 0x00000000);
			//	ssc->i_code = I_CODE_DONE;
				{	int aaa;
					aaa = load_my_bg(ssc);
					if (0!=aaa)
					{
						ssc->i_code = I_CODE_DONE;
					}
				}
				break;
			case I_CODE_OBJ_LOAD:	/* �ėp�G  �����GL	�����GR */
				load_my_sprite(ssc);
				ssc->i_code = I_CODE_DONE;
				break;
			case I_CODE_OBJ_MOVE:
				{	int aaa;
					aaa = do_move_script_sprite(ssc);
					if (0!=aaa)
					{
						ssc->i_code = I_CODE_DONE;
					}
				}
				break;
//			case I_CODE_OBJ_SPEED:
//				set_sprite_speed(/*ssc->para0,*/ssc->para1/*num*/, ssc->para2/*setspeed*/  /*,ssc->para3,ssc->para4*/);
//				ssc->i_code = I_CODE_DONE;
//				break;
			case I_CODE_OBJ_SWAP:
				swap_my_sprite(ssc);
				ssc->i_code = I_CODE_DONE;
				break;
			case I_CODE_SCREEN_BG:			is_bg					= ssc->para1;	check_is_bg();	ssc->i_code = I_CODE_DONE;	break;
			case I_CODE_SCREEN_TEXT:		draw_script_screen		= ssc->para1;					ssc->i_code = I_CODE_DONE;	break;
			case I_CODE_SCREEN_PANEL:		draw_side_panel 		= ssc->para1;					ssc->i_code = I_CODE_DONE;
				#if 1
				/* �{���͗v��Ȃ����A����ł͎g���ɂ����̂Ńp�l���؂�ւ����ɍ׍H����B */
				/* �X�L�b�v���[�h�A�����I�ɖ���(�X�L�b�v���Ȃ�)�B */
				script_skip_mode = 0;/*off*/
				#endif
																												break;
			case I_CODE_BGM:				play_music_num( (ssc->para1) ); 		ssc->i_code = I_CODE_DONE;	break;
			case I_CODE_BGM_VOLUME: 		set_music_volume( (ssc->para1) );		ssc->i_code = I_CODE_DONE;	break;
			case I_CODE_BOSS_LOAD:			script_boss_load( (ssc->para1) );		ssc->i_code = I_CODE_DONE;	break;
			case I_CODE_BOSS_START: 		script_boss_start( /*(ssc->para1)*/ );	ssc->i_code = I_CODE_DONE;	break;
			}
		}
//		if (0 == ssc->owattayo) 	{	n9++;	}		/* �p���t���O */
		if (NULL==ssc->next)		{	break;	}		/* next��NULL�̏ꍇ */
		if (
				(ssc->douji_chain >= ssc->next->douji_chain) ||
				(0==ssc->douji_chain)
			)
		{	break;	}	/* ��douji_chain �� ��douji_chain ���傫���ꍇ */
		ssc = ssc->next;
	}
//
//
//	inits = 0;
//	if (0 == n9)					/* ���̖��߂̋��� */
	{
//		inits = 1;
	aaa_loop:
		if (NULL == search_script_list)
		{
			goto loop_quit;
		}
		//while ( (I_CODE_DONE!=search_script_list->i_code) /*(search_script_list->owattayo)*/ )		/* ���̖��߂� */
		if ((I_CODE_DONE == search_script_list->i_code))
		{
		//	if ()
		//	if (NULL == search_script_list )
		//	{
		//		break;
		//	}
//			if (NULL == search_script_list->next)
//			{
//				goto loop_quit;
//			}
//			else
			{
				search_script_list = search_script_list->next;/* ����͂������璲�ׂ� */
			}
			goto aaa_loop;
		}
	loop_quit:
		;
	}
//
	if (NULL == search_script_list )
	{
		script_terminate_flag = SCRIPT_01_TERMINATE;/*(1)*/
//		return /*(1)*/;
	}
	else
	{
		script_terminate_flag = SCRIPT_00_CONTINUE;/*(0)*/
//		return /*(0)*/;
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/
static void static_do_script_work_main(void)
{
	/*
	�����Ɠ���ƕ`�悪�����ł��Ă�Γ��삪���̈ʒu�ɂ����Ă����Ȃ��������A����̓n���O�A�b�v����B
	*/
//
	#if 1/*Gu������������v��Ȃ��Ȃ�*/
	{
		psp_clear_screen(); /* [PAUSE] ���A����SDL��ʂ������B */
	}
	#endif
	#if (1==USE_SDL_DRAW)
	s_draw_the_script();	/* �X�N���v�g�`�� */
	#endif /* (1==USE_SDL_DRAW) */
//
	/*
	����ƕ`�悪�����ł��Ȃ���Gu���ł��Ȃ��B
	����o�O�������ĕ����ł��Ȃ��̂ō����Ă��鏈�B
	*/
	/*���ׂ�����̂ŕ`���ɂ����o���Ȃ�*/work_the_script();	/* �X�N���v�g���� */
//


}
/*
	�����Ɠ���ƕ`�悪�����ł��Ă�΁ASDL�`�悪�d���Ȃ�AGu�����Ȃ��Ă�
	�V�i���I�`�敔�������t���[���X�L�b�v(30fps�Ƃ�)�����ďo����񂾂��ǁB
*/

/*---------------------------------------------------------
	�X�N���v�g�I������
---------------------------------------------------------*/

static void do_script_terminate(void)
{
	draw_script_screen = 0; /* ����ӃE�B���h�E�\���t���O off */
	load_script_free();
	aaa_script_reset();
//	is_bg = 0;/* (�`��ƕ����ł��Ȃ�)�o�O�C�� */
}


global void script_move_main(void)
{
	static_do_script_work_main();	/* static_�X�N���v�g���� / �`�� */
//
	/*�{�X�퓬��C�x���g*/	/*�{�X�퓬�O�C�x���g*/
	{
	//	if (1==script_terminate_flag)	/* psp��0���W�X�^������̂�0�Ɣ�r�����ق������� */
		if (/*(0)*/SCRIPT_00_CONTINUE != script_terminate_flag)
		{
			do_script_terminate();
			{
				pd.state_flag &= (~(STATE_FLAG_06_IS_SCRIPT));/*off*/
				pd.state_flag |= STATE_FLAG_12_END_SCRIPT;	/*pd_bo ssmode=B08_START;*/ /*	pd_bo ssmode=B09_STAGE_LOAD;*/
			}
		}
		/*else	{	draw_the_script();}*/
	}	// [***090313	�ǉ�
}


/*---------------------------------------------------------

---------------------------------------------------------*/

static int aaa_script_start(void)	/* �V�i���I�t�@�C�����Ɣw�i�t�@�C���� */	/*, char *bg_name, int width*/
{
	if (0 == s_load_scenario())
	{
		return (0);
	}
	kanji_window_clear();	/* �����E�B���h�E�̓��e�������B */
	home_cursor();			/* �J�[�\�����z�[���|�W�V�����ֈړ� */
//
	script_skip_mode = 0;/*off*/
	script_terminate_flag = SCRIPT_00_CONTINUE;/*(0)*/	/* ������ */
//	home_cursor();				/* �J�[�\�����z�[���|�W�V�����ֈړ� */
	cursor_x_chached	= 0;	/* �J�[�\�������� */
	cursor_y_chached	= 0;	/* �J�[�\�������� */

	//inits 			= 1;
	is_bg				= 0;
	draw_script_screen	= 1/*0*/;

	#if 1
	/* std_obj[]������ */
//	std_obj[SPRITE_tachie_r]			= NULL;
//	std_obj[SPRITE_tachie_l]			= NULL;
	int i;
	for (i=0; i<SCRIPT_TACHIE_OBJ_MAX/*20*/; i++)
	{
		std_obj[i]		= NULL;
	}
	#endif
	search_script_list		= memory_script_list;
	return (1);
}
/*---------------------------------------------------------
	�Q�[���C�x���g���̃X�N���v�g�ǂݍ���(�X�g�[���A�G���f�B���O�ȊO)
---------------------------------------------------------*/
extern int select_player;
global void script_ivent_load(void/*int mode*/)
{
	/* �t�@�C�����쐬 */
	{
		/* 'data' '/text/' �̕��̃I�t�Z�b�g */
		#define DIRECTRY_NAME_OFFSET	(DIRECTRY_NAME_DATA_LENGTH + DIRECTRY_NAME_TEXT_LENGTH)
		strcpy(my_fopen_file_name, DIRECTRY_NAME_DATA_STR DIRECTRY_NAME_TEXT_STR "Z/sZ1" DIRECTRY_NAME_KAKUCHOUSI_TEXT_STR);
		my_fopen_file_name[DIRECTRY_NAME_OFFSET+0] = ('0'+select_player);
		my_fopen_file_name[DIRECTRY_NAME_OFFSET+3] = get_stage_chr(pd.player_now_stage);
	//
		if (/*STATE_FLAG_05_IS_BOSS == */(pd.state_flag & STATE_FLAG_05_IS_BOSS))
		{
			;// my_fopen_file_name[DIRECTRY_NAME_OFFSET+4] = '1';
		}
		else
		{
			my_fopen_file_name[DIRECTRY_NAME_OFFSET+4] = '0';
		}
	}
//
	load_script_free();
	if (0 == aaa_script_start())		// �t�@�C�����Ȃ��ꍇ�̓C�x���g���΂�, /*NULL,*/ (GAME_WIDTH) /*380*/
	{
		pd.state_flag |= STATE_FLAG_12_END_SCRIPT;		/*pd_bo ssmode=B09_STAGE_LOAD;*/
	}
	else
	{
		pd.state_flag |= STATE_FLAG_06_IS_SCRIPT;	/*on*/		/*pd_bo ssmode=B00_NONE;*/ /*B06_AFTER_EVENT*/
		#if 0
		/* �V�i���I���Ƀ{�����������Ă��܂��B�o�O������̂ŁB */
		pd_bomber_time = 0;
		#endif
	}

//	load_script_free();
//	if (0 == aaa_script_start(file_name)), /*NULL,*/ 380
//			{	pd.state_flag |= STATE_FLAG_12_END_SCRIPT;		/*pd_bo ssmode=B08_START;*/ }
//	else	{	pd.state_flag |= STATE_FLAG_06_IS_SCRIPT;		/*pd_bo ssmode=B00_NONE;*/ /*B03_BEFORE_EVENT*/ }
}


/*---------------------------------------------------------

---------------------------------------------------------*/

static void story_script_local_quit(void)
{
	#if 1/*???*/
	/*	�����ǂ��킩��Ȃ����ǁA�����͂�������???�B */
	if (NULL != bg_story_window_surface)
	{
		unloadbmp_by_surface(bg_story_window_surface);	//�L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bbg_story_window_surface = NULL;
	}
	#endif
//	bg_alpha = 0;
	//inits=1;
	draw_script_screen = 0; /* ����ӃE�B���h�E�\���t���O off */
	//
	menu_cancel_and_voice();	/* �^�C�g�����j���[�ֈړ� */
}

static void story_script_local_work(void)
{
	static_do_script_work_main();	/* static_�X�N���v�g���� / �`�� */
	if (/*(0)*/SCRIPT_00_CONTINUE != script_terminate_flag)
	{
		do_script_terminate();
		main_call_func = story_script_local_quit;
	}
}

global void story_script_start(void)
{
//	bg_alpha = 0;
	load_script_free();
	/* �t�@�C�����쐬 */
	{
		strcpy(my_fopen_file_name, DIRECTRY_NAME_DATA_STR DIRECTRY_NAME_TEXT_STR "story" DIRECTRY_NAME_KAKUCHOUSI_TEXT_STR);
	}
	aaa_script_start(); //	/*,NULL*/ /*"story/arasuzi.jpg"*/ /*"story_bg.jpg"*/, 480
	main_call_func = story_script_local_work;
}

/*---------------------------------------------------------
	psp�ł͊J�������퓮��o���Ȃ��̂ŁA�N�����Ɋm�ۂ���(�I�����܂�)�J�����Ȃ�
---------------------------------------------------------*/

global void story_script_system_init(void)/* �g�ݍ��� */
{
	#if 1
	memory_script_list	= NULL; 	/* ���ߕێ��p */
	search_script_list	= NULL; 	/* ���ߎ��s�p */
//
	draw_script_screen	= 0;	/* ����ӃE�B���h�E�\���t���O */
	is_bg				= 0;	/* �w�i�\���t���O */
//
	home_cursor();				/* �J�[�\�����z�[���|�W�V�����ֈړ� */
	cursor_x_chached	= 0;	/* �J�[�\���ʒu �ۑ��p */
	cursor_y_chached	= 0;	/* �J�[�\���ʒu �ۑ��p */
//
	#endif
	bg_alpha			= 255;	/* 255==�����l */

	/* SD L_FreeSurface(); �� psp�ł͑��������Ɠ����Ȃ��̂� ���̑΍� */
//
}


/*---------------------------------------------------------
	�I�����ɊJ�����镔��...�������ǁA
	�n���O�A�b�v�����肷��̂ŁA��芸���������ɂȂ��Ă�B
	(�J�����Ȃ��Ă��I������΁AOS���������������̂Ŗ��Ȃ����Č����Ζ��Ȃ�)
---------------------------------------------------------*/

global void script_system_exit(void)/* �O�� */
{
	/*msg_window_init()*/
//
	#if 0/* �{���͗v�� */
	#endif
}

