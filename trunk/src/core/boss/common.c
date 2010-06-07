
#include "boss.h"//#include "game_main.h"//#include "bullet_object.h"

/*---------------------------------------------------------
	�����͕핗  �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�U�R�o�^�̋��ʃ��[�`��
	-------------------------------------------------------
---------------------------------------------------------*/

global void add_zako_common(STAGE_DATA *l, SPRITE *src)
{
	src->flags				|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE|SP_FLAG_TIME_OVER*/);
	src->callback_loser 	= lose_random_item;
	src->callback_hit_enemy = callback_hit_zako;

//
//	src->x256				= (((u32)l->user_x)<<8);
//	src->y256				= (((u32)l->user_y)<<8);
//	src->base_hp			= (((u32)l->user_hp));
//	src->base_score 		= (((u32)l->user_score));
	src->x256				= ((l->user_x)<<8);
	src->y256				= ((l->user_y)<<8);
	src->base_hp			= ((l->user_hp));
	src->base_score 		= ((l->user_score));
}


/*---------------------------------------------------------
	�{�X�`�ԕύX���̋���
---------------------------------------------------------*/

static SPRITE *obj_effect[8];	/* [��] */

/*---------------------------------------------------------
	�U�R���U�������ꍇ�̋��ʃ��[�`��
	-------------------------------------------------------
---------------------------------------------------------*/

void callback_hit_zako(SPRITE *src/*�G����*/, SPRITE *tama /*���e*/)
{
	bakuhatsu_add_type_ddd(tama, BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/*��Ɏ��s������������*/
//
	{
		src->base_hp -= tama->base_weapon_strength; 	/* �U�����đ̗͌��炷(����������) */
		if (0 >= src->base_hp)			/* �O�����l�Ȃ�A�|�����B */
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
static int ee_angle1024;
static int ff_angle1024;
/*static*/ void move_boss_effect(SPRITE *src)
{
	ee_angle1024 += (4);	/* 4 ��]���x */
	ff_angle1024 += (2);	/* 2 �g��k�����x */
	int ww_angle1024;	ww_angle1024 = ee_angle1024;
	int vv_angle1024;	vv_angle1024 = ff_angle1024;
	int radius;
	int i;
	for (i=0; i<5; i++)
	{
		ww_angle1024 += 20; 	/* 20 ��]���x */
		vv_angle1024 += 200;	/* 40 �g��k�����x */
		radius = ((sin1024(vv_angle1024))>>2)+16; /* 80==16+64 */
		obj_effect[i]->x256 			= src->x256 + ((sin1024((ww_angle1024))*radius));
		obj_effect[i]->y256 			= src->y256 + ((cos1024((ww_angle1024))*radius));
		obj_effect[i]->m_zoom_x256		= ( (1/*+255*/+(radius<<2)) );
		#if 1
		/* �`��p�p�x(����0�x�ō����(�����v���)) */
		obj_effect[i]->m_angleCCW1024	= -ww_angle1024;
		#endif
	}
}

/*static*/ void boss_effect_sprite_add(void)
{
	//----[EFFECT]
	{int i;
		for (i=0; i<5; i++)
		{
			SPRITE *h;
			h						= sprite_add_gu_error();
//			if (NULL!=h)/* �o�^�ł����ꍇ�̂� */	/* �����o�^ */	/*...���[��*/		/* �d�l�o�O(?) */
			{
				obj_effect[i]		= h;
			}
		}
	}

}
/*static*/ void boss_effect_reset(void)
{
	//----[EFFECT]
	{int i;
		for (i=0; i<5; i++)
		{
			{
				SPRITE *h;
				h					= obj_effect[i];
				h->m_Hit256R		= JIKI_ATARI_ITEM_16;/*???*/
			//	h->type 			= S P_ZA KO/*S P_BO SS01*/;
				h->type 			= (SPELL_SQUERE_);
			//	h->flags			|= (SP_FLAG_COLISION_CHECK/*|SP_FLAG_VISIBLE*/);
			//	h->flags			|= (/*SP_FLAG_VISIBLE|*/SP_FLAG_TIME_OVER);
//				h->flags			&= (~(SP_FLAG_VISIBLE));	/*��\��*/
			}
		}
	}

}
/*static*/ void boss_effect_initialize_position(void)
{
	{int i;
		for (i=0; i<5; i++)
		{
		//	obj_effect[i]->type 				= SP_DELETE;
			sprite_initialize_position(obj_effect[i]);
		}
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

#if 0/*����*/
/* �{�X���ʋK�i */
	#define target_x256 		user_data00 	/* �ڕWx���W */
	#define target_y256 		user_data01 	/* �ڕWy���W */
	#define vvv256				user_data02 	/* �ڕW���W�ւ̓��B���� */
	#define time_out			user_data03 	/* �������� */
#endif


global unsigned int alice_anime_count;

global void alice_yuudou_move_only(SPRITE *src)
{
	src->vvv256 -= (1);
	if (1 > src->vvv256 )
	{
		src->vvv256 = (0);
	}
}

global void alice_yuudou_calc(SPRITE *src)	/* �ڕW��ݒ肵�A�U���ړ� */
{
	{	/* ���� == (�e�̌��ݍ��W - �e�̗U�����W) */
		int x_sa256 = (src->x256 - src->target_x256);
		int y_sa256 = (src->y256 - src->target_y256);
		/* ���Z���� == (�e�̍������W * �U���䗦) */
		int aaa_x256 = ((x_sa256 * ((src->vvv256) ))>>8);	/*fps_factor*/
		int aaa_y256 = ((y_sa256 * ((src->vvv256) ))>>8);	/*fps_factor*/
		src->x256 = (src->target_x256) + (aaa_x256);	/*fps_factor*/
		src->y256 = (src->target_y256) + (aaa_y256);	/*fps_factor*/
	}
}

static void alice_animation_only(SPRITE *src)
{
	if (alice_anime_count)	/* �U���A�j���[�V���� */
	{
		alice_anime_count--;
				if ((32)>alice_anime_count) 	{	src->type = TEKI_09_BOSS32; }	/*src->an im_frame = 0x23;*/
		else	if ((40)>alice_anime_count) 	{	src->type = TEKI_10_BOSS33; }	/*src->an im_frame = 0x22;*/
		else									{	src->type = TEKI_09_BOSS32; }	/*src->an im_frame = 0x21;*/
	}
	else	/* �ړ��A�j���[�V���� */
	{
		int aaa;
		aaa = (src->x256> src->target_x256)?(TEKI_00_BOSS11/*0x00*/):(TEKI_04_BOSS21/*0x10*/); /*���E*/

		#if 0
				if ( ( 16) > src->vvv256)		{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else	if ( (200) > src->vvv256)		{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else	if ( (224) > src->vvv256)		{	aaa+=2;}	/*128.0==t256(0.5)*/
		else	if ( (240) > src->vvv256)		{	aaa+=1;}	/*179.2==t256(0.7)*/
		else									{	aaa+=0;}
		#else
				if ( (256- 16) < src->vvv256)	{	aaa+=2;}	/* 25.6==t256(0.1)*/
		else	if ( (256-200) < src->vvv256)	{	aaa+=3;}	/* 76.8==t256(0.3)*/
		else	if ( (256-224) < src->vvv256)	{	aaa+=2;}	/*128.0==t256(0.5)*/
		else	if ( (256-240) < src->vvv256)	{	aaa+=1;}	/*179.2==t256(0.7)*/
		else									{	aaa+=0;}
		#endif
		src->type = aaa;
	}
}


/*---------------------------------------------------------
	�{�X�s��
---------------------------------------------------------*/

global void move_alice(SPRITE *src)
{
//	ch eck_regist_spell_card(src);	/* �X�y�J�o�^�\�Ȃ�o�^ */
//	sp ell_card_generator222(src);	/* �X�y�J���� */
	check_regist_generate_spell_card(src);	/* �X�y�J�o�^�\�Ȃ�o�^ / �X�y�J���� */
	#if 1/* [�X�y�J�V�X�e�����Ɉړ��\��] */
	/*---------------------------------------------------------
		�p�`�F�ړ�����
	---------------------------------------------------------*/
	//
	//	enemy_boss04_setpos(src, xxx,yyy);
	//
	boss_move_vx_vy(src);	boss_clip_rect(src);
	alice_animation_only(src);
//	move_all_doll(src);
	#endif
//
	move_boss_effect(src);		/* ��G�t�F�N�g */
	danmaku_generator(src); 	/* �e������ */
}

/*---------------------------------------------------------
	�{�X�I�v�V�����A����
---------------------------------------------------------*/
global void check_boss_option_time_out(SPRITE *src)
{
	#if 1
	/* �{�X��|���ƊF�j�󂳂��B */
	if (0==boss_life_value)/*�_������(?)*/
	{
		src->time_out = 0;
	}
	src->time_out--;
	if (0 > src->time_out)
	{
		#if 1
		src->base_hp	= (0);/* (?) ����ŗǂ������؂̕K�v���� */
		#endif
		src->type		= SP_DELETE;
	}
	#endif
}
