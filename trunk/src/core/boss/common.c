
#include "bullet_object.h"

/*---------------------------------------------------------
	���ʃI�u�W�F�N�g
---------------------------------------------------------*/
#if 1

SPRITE *player;
SPRITE *send1_obj;

#endif




/*---------------------------------------------------------
	�U�R�o�^�̋��ʃ��[�`��
	-------------------------------------------------------
---------------------------------------------------------*/

void add_zako_common(STAGE_DATA *l, SPRITE *src)
{
	src->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
	src->callback_loser 	= lose_random_item;
	src->callback_hit_enemy = callback_hit_zako;

//
//	src->x256							= (((u32)l->user_x)<<8);
//	src->y256							= (((u32)l->user_y)<<8);
//	/*data->base.*/src->base_hp 		= (((u32)l->user_hp));
//	/*data->base.*/src->base_score		= (((u32)l->user_score));
	src->x256							= ((l->user_x)<<8);
	src->y256							= ((l->user_y)<<8);
	/*data->base.*/src->base_hp 		= ((l->user_hp));
	/*data->base.*/src->base_score		= ((l->user_score));
}


/*---------------------------------------------------------
	�{�X�`�ԕύX���̋���
---------------------------------------------------------*/

static SPRITE *obj_effect[8];	/* [��] */
/*static*/ /*extern*/  unsigned int common_boss_flags;

/*---------------------------------------------------------
	�U�R���U�������ꍇ�̋��ʃ��[�`��
	-------------------------------------------------------
---------------------------------------------------------*/

void callback_hit_zako(SPRITE *src/*�G����*/, SPRITE *tama /*���e*/)
{
	bakuhatsu_add_type_ddd(tama, BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/*��Ɏ��s������������*/
//
	{
	//	ENEMY_BASE *data	= (ENEMY_BASE *)s->data;
	//	WEAPON_BASE *w		= (WEAPON_BASE *)tama->data;
		/*data->*/src->base_hp -= /*w->*/tama->base_weapon_strength;	/* �U�����đ̗͌��炷(����������) */
		if (0 >= /*data->*/src->base_hp)			/* �O�����l�Ȃ�A�|�����B */
		{
			bakuhatsu_add_type_ddd(src, BAKUHATSU_ZAKO04/*BAKUHATSU_MINI00*/);/*�U�R���Ŕ���*/
			if (NULL != src->callback_loser)
			{
				(src->callback_loser)(src);
				src->callback_loser = NULL;
			}
			bonus_info_any_score_nodel(src, ((/*data->*/src->base_score)*(/*pd_now_stage*/player_now_stage/*le vel*/)) );/*���������֎d�l�ύX*/src->type = SP_DELETE;
			voice_play(VOICE08_DEATH, TRACK05_ZAKO_DEATH);
		}
	}
}


/*---------------------------------------------------------
	�{�X���U�������ꍇ�̋��ʃ��[�`��
	-------------------------------------------------------
---------------------------------------------------------*/
int boss_hp_dec_by_frame;/*�{�X�U�������l�A�t���[���P��*/

/*static*/ void callback_hit_boss(SPRITE *src/*�G����*/, SPRITE *tama /*���e*/)/*, int angle*/
{
	bakuhatsu_add_type_ddd(tama, BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/*��Ɏ��s������������*/
//
	{
		boss_hp_dec_by_frame += /*w->*/tama->base_weapon_strength;	/* �U�����đ̗͌��炷(����������) */
	}
}


/*---------------------------------------------------------
	�{�X���S����
	-------------------------------------------------------
	�� �U���̏ꍇ�̎��S���� 		DESTROY_CHECK_00_WIN_BOSS
	�� ���Ԑ؂�̏ꍇ�̎��S���� 	DESTROY_CHECK_01_IS_TIME_OUT
---------------------------------------------------------*/

/*static*/ void boss_destroy_check_type(SPRITE *src/*�G����*/, int check_type)
{
	if (0 >= /*data->boss_base.boss*/src->base_hp)			/* �O�����l�Ȃ�A�|�����B */
	{
		/*data->boss_base.boss*/src->base_hp = 0;
		if (DESTROY_CHECK_00_WIN_BOSS == check_type)
		{

			pd_state_flag		|= STATE_FLAG_09_IS_WIN_BOSS;	/* �U���œ|�����t���Oon */
		}
	//	#if 1
		lose_boss(src);/* ���� */
	//	#else
	//	if (NULL != src->callback_loser)
	//	{
	//		(src->callback_loser)(src);
	//		src->callback_loser = NULL;
	//	}
	//	#endif
	//	voice_play(VOICE08_DEATH, TRACK05_ZAKO_DEATH);
	}
}

/*---------------------------------------------------------

---------------------------------------------------------*/
static int ee_angle512;
static int ff_angle512;
/*static*/ void boss_effect(SPRITE *src)
{
	ee_angle512 += 2;	/* 2 ��]���x */
	ff_angle512 += 1;	/* 1 �g��k�����x */
	int ww_angle512;
	ww_angle512 = ee_angle512;
	int vv_angle512;
	vv_angle512 = ff_angle512;
	int radius;
	int i;
	for (i=0; i<5; i++)
	{
		ww_angle512 += 10;	/* 10 ��]���x */
		vv_angle512 += 100; /* 20 �g��k�����x */
		radius = ((sin512(vv_angle512))>>2)+16; /* 80==16+64 */
		obj_effect[i]->x256 			= src->x256 + ((sin512((ww_angle512))*radius));
		obj_effect[i]->y256 			= src->y256 + ((cos512((ww_angle512))*radius));
		obj_effect[i]->m_zoom_x256		= ( (1/*+255*/+(radius<<2)) );
		#if 1
		/* �`��p�p�x(����0�x�ō����(�����v���)) */
		obj_effect[i]->m_angleCCW512	= -ww_angle512;
		#endif
	}
}
extern void stage_boss_load_texture(void);
/*static*/ void boss_effect_init(void)
{
	//----[EFFECT]
	{int i;
	for (i=0; i<5; i++)
	{
		obj_effect[i]						= sprite_add_gu(JIKI_ATARI_ITEM_16);
	//	obj_effect[i]->type 				= S P_ZA KO/*S P_BO SS01*/;
		obj_effect[i]->type 				= (SPELL_SQUERE_);
	//	obj_effect[i]->flags				|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK);
		obj_effect[i]->flags				|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
		obj_effect[i]->flags				&= (~(SP_FLAG_VISIBLE));	/*��\��*/
	}}
	// �{�X�e�N�X�`���ǂݍ���
	stage_boss_load_texture();
}
/*static*/ void boss_effect_term(void)
{
	int i;
	for (i=0; i<5; i++)
	{
		obj_effect[i]->type 				= SP_DELETE;
	}
}
