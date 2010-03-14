
#include "bullet_object.h"

/*---------------------------------------------------------
		"���[�~�A", 	"GFAIRY",
	-------------------------------------------------------

---------------------------------------------------------*/

//typedef struct
//{
//	ENEMY_BASE base;
//	BO SS_BASE boss_base;
//------------ �ړ��֘A
//	int st ate1;
#define time_out		user_data00 	/* �s��wait */
#define repeat			user_data01 	/* �J��Ԃ��� */
#define start_danmaku	user_data02 	/* */
#define start_x256		user_data03 	/* �o��x���W */
//} RUMIA_DATA;
//	int wait2;	/* �U��wait */

enum
{
	SS00 = 0,
	SS01,
	SS02,
	SS03,
	SS04,
};
//	ST02,
//	ST02bbb,
//	ST03,

/*---------------------------------------------------------
	s = �{�X�{�� boss sprite
	t = �v���C���[�̒e player's weapon
---------------------------------------------------------*/

static void callback_hit_rumia1(SPRITE *src/*�G����*/, SPRITE *tama /*���e*/)
{
	bakuhatsu_add_type_ddd(tama, BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);
//
//	RUMIA_DATA *data	= (RUMIA_DATA *)src->data;
//	WEAPON_BASE *w		= (WEAPON_BASE *)tama->data;
	/*data->base.*/src->base_hp -= /*w->*/tama->base_weapon_strength;	/* �U�����đ̗͌��炷(����������) */
	if (0 >= /*data->base.*/src->base_hp)			/* �O�����l�Ȃ�A�|�����B */
	{
		src->base_hp 	= (9999);/* �Q��͓|���Ȃ� */
		bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
	//
		const u8 item_tbl[4] =
		{
			(SP_ITEM_02_P128&0xff), 	/* [F] */
			(SP_ITEM_01_P008&0xff), 	/* [P]�� */
			(SP_ITEM_00_P001&0xff), 	/* [p]�� */
			(SP_ITEM_05_TENSU&0xff),	/* [�_] */
		};
		item_create(src, (SP_GROUP_ITEMS|(item_tbl[difficulty])), 7, ITEM_MOVE_FLAG_06_RAND_XY);/* ����΂� */
		player_add_score(/*data->base.*/src->base_score);
//
		bakuhatsu_add_circle(src, 0);
	//	src->type				= SP_DELETE;
		src->boss_base_state001 		= SS04; 		/* ��֑ޏ� */
		/* �R�[���o�b�N�o�^ */
		src->callback_hit_enemy = NULL; 	/* �_�~�[�R�[���o�b�N�o�^ */
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

//static void lose_rumia1(SPRITE *src)
//{
//
//}


/*---------------------------------------------------------
	�e�����I���܂ő҂B
	-------------------------------------------------------

---------------------------------------------------------*/

static void danmaku_state_check_holding(SPRITE *src)
{
	if (DANMAKU_00 == src->boss_base_danmaku_type)
	{
		src->boss_base_state001++/* = nextstate*/;
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/

static void move_rumia1(SPRITE *src)
{
	src->time_out--;
	switch (src->boss_base_state001)
	{
	case SS00:	/* �ォ��o�� */
		src->y256		+= t256(2); 	/*fps_factor*/
		if (src->start_x256 < src->y256)	{	src->time_out=30;	src->boss_base_state001 = SS03; }
		break;
//---------
	case SS01:	/* �e���Z�b�g */
			/*src->wait2=8 10*/
			src->boss_base_state001++;/* = SS02;*/
			;/* 8�����e */
			;/* 7��e */
			src->boss_base_danmaku_type 	= src->start_danmaku/*DANMAKU_01*/; 		/* �S�e�����Z�b�g */
			src->boss_base_danmaku_time_out = DANMAKU_01_SET_TIME;	/* �S�e���̔������� */
		break;
	case SS02:	/* �S�e�����I���܂őҋ@ */
		danmaku_state_check_holding(src);	/*, SA07*/ /*, SAKUYA_ANIME_04_CENTER_A*/
		break;
	case SS03:	/* ���΂��҂� */
		if (0 > src->time_out)
		{
			src->time_out = 100/*150*/;
			/* �K��̌J��Ԃ��񐔂��Ȃ�����ޏ� */
			src->repeat--;
			if ((0) != src->repeat) 	{	src->boss_base_state001 	= SS01; 		}/* �J��Ԃ� */
			else						{	src->boss_base_state001++/* = SS04*/;		}/* ��֑ޏ� */
		}
		break;
//---------
	case SS04:	/* ��֑ޏ� */
		src->y256		-= t256(2); 	/*fps_factor*/
		#if 0/*���݁A�����_�Ȃ̂ŁA�{���͂���*/
		if ( 0 > (src->y256+((src->height_size256/*�c�̃T�C�Y*/))))
		#endif
		#if 0/*Gu(���S���W��_�ɂȂ�����A�{���͂����B*/
		if ( 0 > (src->y256+((src->height_harf_size256/*�c�̔����T�C�Y*/))))
		#endif
		if ( 0 > (src->y256+t256(55.0)))/* 55�h�b�g�Ƃ���(�����_�A�c�̃T�C�Y) */
	//	if ( 0 > src->y256)/* �ȗ��� */
		{
			src->type = SP_DELETE;
		}
		break;
	}
	if (SP_DELETE != src->type)
	{
		src->type			= TEKI_00_BOSS11+((src->time_out&0x30)>>4);
	}
//	src->yx_an im_frame = ((data->time_out&0x10)>>4);
//
	danmaku_generator(src); /* �e������ */
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

void add_chuu_rumia(STAGE_DATA *l)
{
	{
		SPRITE *sakuya;
		sakuya						= sprite_add_gu(ZAKO_TYPE_ATARI16_PNG);
		sakuya->type				= TEKI_00_BOSS11;
		sakuya->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		sakuya->callback_mover		= move_rumia1;
	//	sakuya->callback_loser		= lose_rumia1;
		sakuya->callback_hit_enemy	= callback_hit_rumia1;	/* �R�[���o�b�N�o�^ */
		sakuya->y256				= t256(-30);
	//	sakuya->x256				= ((l->user_x)*t256(35))+t256(40);
	//	sakuya->x256				= ((l->user_x)*t256(32))+t256(48);
	//	sakuya->x256				= ((l->user_x)<<(8+5))+t256(48);
		sakuya->x256				= ((l->user_x)<<(8));
//
		/*data->base.*/sakuya->base_hp		= (8*200)+(2/*di fficulty*/<<(4+3));	/* easy�ł����݊�����ۂÂ���ׂ� 200 �͕K�v */ 	// 50+150*di fficulty;
		/*data->base.*/sakuya->base_score	= score(100)+score(100)*difficulty;
//
//		RUMIA_DATA *data;
//		data								= mmalloc(sizeof(RUMIA_DATA));
//		sakuya->data						= data;
		sakuya->boss_base_state001						= SS00;
//		sakuya->time_out						= 30;
//		sakuya->wait2						= 10;/*0*/
		sakuya->repeat						= (2+2+1);
		sakuya->start_x256					= ((l->user_y)<<(8));/* t256(50) */
		#if 1
	//------------ �e���֘A
		sakuya->start_danmaku				= ((l->user_1_moji+0x10/*�Ƃ肠����r27�݊�*/)&0x1f);
		sakuya->boss_base_danmaku_type		= 0;
		sakuya->boss_base_danmaku_time_out	= 0;
		#endif
	}
}
