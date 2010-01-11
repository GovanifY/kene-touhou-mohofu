
#include "bullet_object.h"

/*---------------------------------------------------------
	���ʃI�u�W�F�N�g
---------------------------------------------------------*/
#if 1

SPRITE *player;
SPRITE *send1_obj;

#endif

/*---------------------------------------------------------
	�{�X�`�ԕύX���̋���
---------------------------------------------------------*/


static SPRITE *obj_effect[8];	/* [��] */
/*static*/ /*extern*/  unsigned int common_boss_flags;

/*---------------------------------------------------------
	�{�X�`�ԕύX���̋��ʃ��[�`��
---------------------------------------------------------*/
extern void draw_spell_card_name(void);/* spell_card.c */
void common_boss_put_items(SPRITE *src)
{
	#if (1)
	draw_spell_card_name();
	#endif
	#if (0==USE_DESIGN_TRACK)
	play_voice_auto_track(VOICE07_BOMB);
	#else
	voice_play(VOICE07_BOMB, TRACK02_ALEART_IVENT);/*�e�L�g�[*/
	#endif
//++	((PLAYER_DATA *)player->data)->bo ssmode=B04_CHANGE;
	bullets_to_hosi();/* �e�S���A���A�C�e���ɂ��� */
	item_create_for_boss(src, ITEM_CREATE_MODE_02);
}


/*---------------------------------------------------------
	�U�R���U�������ꍇ�̋��ʃ��[�`��
	-------------------------------------------------------
---------------------------------------------------------*/

//	/*static*/ void callback_zako_hitbyweapon(SPRITE *src, SPRITE *tama)
void callback_hit_zako(SPRITE *src/*�G����*/, SPRITE *tama /*���e*/)
{
	send1_obj->x256 = tama->x256;
	send1_obj->y256 = tama->y256;
	bakuhatsu_add_type_ddd(send1_obj/*t->x256,t->y256*/,/*0,*/BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/*��Ɏ��s������������*/
//
	{
	//	ENEMY_BASE *data	= (ENEMY_BASE *)s->data;
	//	WEAPON_BASE *w		= (WEAPON_BASE *)tama->data;
		/*data->*/src->base_health -= /*w->*/tama->base_weapon_strength;	/* �U�����đ̗͌��炷(����������) */
		if (0 >= /*data->*/src->base_health)			/* �O�����l�Ȃ�A�|�����B */
		{
			bakuhatsu_add_zako04(src);
			if (NULL != src->callback_loser)
			{
				(src->callback_loser)(src);
				src->callback_loser = NULL;
			}
			bonus_info_any_score_nodel(src, ((/*data->*/src->base_score)*(/*pd->now_stage*/player_now_stage/*le vel*/)) );/*���������֎d�l�ύX*/src->type = SP_DELETE;
			#if (0==USE_DESIGN_TRACK)
			play_voice_auto_track(VOICE08_DEATH);
			#else
			voice_play(VOICE08_DEATH, TRACK05_ZAKO_DEATH);
			#endif
		}
	}
}
//	int up_flags = (t->type==(SP_GROUP_JIKI_GET_ITEM|SP_GROUP_SHOT_ZAKO))?(ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY):(ITEM_MOVE_FLAG_06_RAND_XY);


/*---------------------------------------------------------
	�{�X���U�������ꍇ�̋��ʃ��[�`��
	-------------------------------------------------------
---------------------------------------------------------*/

/*static*/ void callback_hit_boss(SPRITE *src/*�G����*/, SPRITE *tama /*���e*/)/*, int angle*/
{
	send1_obj->x256 = tama->x256;
	send1_obj->y256 = tama->y256;
	bakuhatsu_add_type_ddd(send1_obj/*tama->x256,tama->y256*/,/*0,*/BAKUHATSU_MOVE12/*BAKUHATSU_MINI00*/);/*��Ɏ��s������������*/
//
	{/* BOSS_BASE */
	//	BOSS99_DATA *data	= (BOSS99_DATA *)src->data;
	//	WEAPON_BASE *w		= (WEAPON_BASE *)tama->data;
		/*data->boss_base.boss*/src->base_health -= /*w->*/tama->base_weapon_strength;	/* �U�����đ̗͌��炷(����������) */
		if (spell_card_limit_health >= /*data->boss_base.boss*/src->base_health)		/* �K��l�ȉ��ɂȂ�΃X�y�J���[�h���� */
		{
			spell_card_mode 			= 0/*off*/;
			boss_destroy_check_type(src, DESTROY_CHECK_00_WIN_BOSS);
		}
	}
}

//	parsys_add(NULL,100,0,tama->x,tama->y,30,0,0,50,PIXELATE,NULL);
//	parsys_add(NULL,100,0,src->x,src->y,30,0,0,50,PIXELATE,NULL);



/*---------------------------------------------------------
	�{�X���S����
	-------------------------------------------------------
	�� �U���̏ꍇ�̎��S���� 		DESTROY_CHECK_00_WIN_BOSS
	�� ���Ԑ؂�̏ꍇ�̎��S���� 	DESTROY_CHECK_01_IS_TIME_OUT
---------------------------------------------------------*/

/*static*/ void boss_destroy_check_type(SPRITE *src/*�G����*/, int check_type)
{
	if (0 >= /*data->boss_base.boss*/src->base_health)			/* �O�����l�Ȃ�A�|�����B */
	{
		/*data->boss_base.boss*/src->base_health = 0;
		if (DESTROY_CHECK_00_WIN_BOSS == check_type)
		{
			PLAYER_DATA *pd 	= (PLAYER_DATA *)src->data;
			pd->state_flag		|= STATE_FLAG_09_IS_WIN_BOSS;	/* �U���œ|�����t���Oon */
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
	//	#if (0==USE_DESIGN_TRACK)
	//	play_voice_auto_track(VOICE08_DEATH);
	//	#else
	//	voice_play(VOICE08_DEATH, TRACK05_ZAKO_DEATH);
	//	#endif
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
//		obj_effect[i]->anim_speed			= 0;
	//	obj_effect[i]->type 				= SP_ZAKO/*SP_BOSS01*/;
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

/*---------------------------------------------------------
	�U���~�T�C���G
	-------------------------------------------------------
---------------------------------------------------------*/
//	ENEMY_BASE base;
	///int v x256;	/*union BULLET_VECTOR_DATA*/	/* ���pregist_vector(); */
	///int v y256;	/*union BULLET_VECTOR_DATA*/	/* ���pregist_vector(); */
//
//	int aaa_angle512;
//
typedef struct
{
	int p0_x256;	/* �U���_0(�o�����̃v���C���[�ʒu�֒����ړ����錴�_) */
	int p0_y256;	/* �U���_0(�o�����̃v���C���[�ʒu�֒����ړ����錴�_) */

#if 0
//
	int p1_x256;	/* �U���_1(�o�����̃v���C���[���痣�ꂽ�ʒu) */
	int p1_y256;	/* �U���_1(�o�����̃v���C���[���痣�ꂽ�ʒu) */
//
	int p2_x256;	/* �U���_2(�o�����̃v���C���[�ʒu) */
	int p2_y256;	/* �U���_2(�o�����̃v���C���[�ʒu) */
//
	int state;
//
	int aaa_speed256;
#endif
	int time_out;	/* range delay */
} ENEMY_HOMING_DATA;

#if 0
/*---------------------------------------------------------
	���@�_���e�̊p�x���v�Z
---------------------------------------------------------*/
static int angle_jikinerai512(SPRITE *p, SPRITE *t)
{
	#if 1
		return (atan_512(p->y256-t->y256				 ,p->x256-t->x256				  ));//2082417					/* �����e�̏ꍇ�Ɏ��@�_��(�ȗ���) �e�Ǝ������傫���������Ȃ炸��Ȃ��A�Ⴄ�Ƃ��̕��덷�ɂȂ� */
	//	return (atan_512(p->y256-t->y256+((p->h128-t->h128)),p->x256-t->x256-((p->w128+t->w128))));/*???(original)*/	/* ��e�̏ꍇ�Ɏ��@�_��(?) �Ȃ񂩍��ɂ���� */
//		return (atan_512(p->y256-t->y256+((p->h128-t->h128)),p->x256-t->x256-((p->w128-t->w128))));/**/ 				/* ��e�̏ꍇ�Ɏ��@�_�� */
	//	return (atan_512(p->y256-t->y256+((p->h128-t->h128)),p->x256-t->x256+((p->w128-t->w128))));/**/ 				/* �����e�̏ꍇ�Ɏ��@�_�� */
	//	return (atan_512(p->y256-t->y256+((p->h128	)),p->x256-t->x256-((p->w128	 ))));
	//	return (atan_512(p->y256-t->y256-((p->h128	)),p->x256-t->x256-((p->w128	 ))));
	#endif
}
#endif

/*
�U���z�[�~���O�ɂ��悤�Ǝv�����̂�
�Ȃ����V���b�g����(�쐬��)
*/

/*---------------------------------------------------------
	�G�̗U���z�[�~���O�{��(�Z�p�`�Ş�F�̓z)
---------------------------------------------------------*/
//	PLAYER_DATA *pd = (PLAYER_DATA *)player->data;
//	if (pd->state_flag & STATE_FLAG_03_SCORE_AUTO_GET_ITEM)
//	if (pd->state_flag & (STATE_FLAG_06_IS_SCRIPT|STATE_FLAG_05_IS_BOSS) )
//	{	/* �{�X�Ɏ������ނƍ����_ */
//	//	bonus_info_score(s,SCORE_1000);/*���������֎d�l�ύXsrc->type = SP_DELETE;*/
//		item_create(src, S P_ITEM_05_HOSI, 1, (ITEM_MOVE_FLAG_01_COLLECT|ITEM_MOVE_FLAG_06_RAND_XY) );src->type = SP_DELETE;
//	}
//
enum
{
	ST00 = 0,
//	ST01,
//	ST02,
	ST03,
};

static void enemy_homing_move(SPRITE *src)
{
	ENEMY_HOMING_DATA *data = (ENEMY_HOMING_DATA *)src->data;
	data->time_out -= 1/*fps_fa ctor*/; //data->delay -= 1/*fps_fa ctor*/; data->range-=1/*fps_fa ctor*/;
	#if 1
	/* �f�o�b�O */
	/* ���Ԑ؂�ŏI��� */
	if (data->time_out < 1 )	//if (data->range<=0)
	{
		src->type = SP_DELETE;
		return;
	}
	#endif

	#if 0
	if ( 0x00 == (data->time_out&0x0f) )/*0x07*/
	{
		//static void enemy_homing_update(SPRITE *src)
		{
			switch (data->state)
			{
			case ST00:
				src->m_angleCCW512/*data->aaa_angle512*/ += src->tmp_angleCCW512;
				/* 64 flame�o�߂����� */
				if (128 == (data->time_out) )
				{
					data->state++;
				}
				break;
			#if 0
			case ST01:
				send1_obj->x256 = data->p1_x256;	/* ���e�ʒu�� offset �p */
				send1_obj->y256 = data->p1_y256;	/* ���e�ʒu�� offset �p */
				src->m_angleCCW512/*data->aaa_angle512*/ = angle_jikinerai512(send1_obj,s);
				/* �\�������Â����� */
				if (1024 >(send1_obj->y256-src->y256)+(send1_obj->x256-src->x256) )
				{
					data->state++;
				}
				break;
			case ST02:
				send1_obj->x256 = data->p2_x256;	/* ���e�ʒu�� offset �p */
				send1_obj->y256 = data->p2_y256;	/* ���e�ʒu�� offset �p */
				src->m_angleCCW512/*data->aaa_angle512*/ = angle_jikinerai512(send1_obj,s);
				/* �\�������Â����� */
				if (1024 >(send1_obj->y256-src->y256)+(send1_obj->x256-src->x256) )
				{
					data->state++;
				}
				break;
			#endif
			case ST03:
			//	src->m_angleCCW512/*data->aaa_angle512*/ = angle_jikinerai512(player,s);
				/* ��ʊO�ɏo����I��� */
				if (
					(0 >= src->x256)
					|| (t256(GAME_WIDTH) < src->x256)
					//#if (1==USE_X_HOUKOU)
					|| (0 >= src->y256)
					|| (t256(GAME_HEIGHT) < src->y256)
					//#endif
				)
				{
					src->type = SP_DELETE;
				}
				break;
			}
			mask512(src->m_angleCCW512/*data->aaa_angle512*/);
			src->yx_an im_frame = (((src->m_angleCCW512/*data->aaa_angle512*/))>>(8-3));
		}
//		/*data->v*/src->vx256 = ((sin512((src->m_angleCCW512/*data->aaa_angle512*/))*data->aaa_speed256)>>8)/**fps_fa ctor*/;
//		/*data->v*/src->vy256 = ((cos512((src->m_angleCCW512/*data->aaa_angle512*/))*data->aaa_speed256)>>8)/**fps_fa ctor*/;
	}
	#endif

	data->p0_x256 += /*data->v*/src->vx256/**fps_fa ctor*/;
	data->p0_y256 += /*data->v*/src->vy256/**fps_fa ctor*/;

	#if 1
	/* �f�o�b�O(���_�ňړ�) */
	src->x256 = data->p0_x256;
	src->y256 = data->p0_y256;
	#endif

	#if 1
	if (data->time_out < 128 ) //if (data->range<=0)
	{
		/* �f�o�b�O */
		/* ��ʊO�ɏo����I��� */
		if (
			(0 >= src->x256)
			|| (t256(GAME_WIDTH) < src->x256)
			//#if (1==USE_X_HOUKOU)
			|| (0 >= src->y256)
			|| (t256(GAME_HEIGHT) < src->y256)
			//#endif
		)
		{
			src->type = SP_DELETE;
		}
	}
	#endif

	/* max. reichweite erreicht? self-destruct! */
//	if (data->time_out < 1 )	//if (data->range<=0)
//	{
//		src->type = SP_DELETE;
//	}
}


/*---------------------------------------------------------
	�G���̗U���~�T�C�����쐬����
---------------------------------------------------------*/

void bullet_create_enemy_homing(SPRITE *src)
{
	int i;
	for (i=0; i<8/*HOMING_NUM_OF_ENEMIES*/; i++)
	{
		SPRITE *r;
	//	r					= sprite_add_res(BASE_HOMING16_PNG);	/*�f�ޕύX*/ /*20"rotating_rocket.png"*/
	//	r->type 			= SP_ZAKO;/*SP_ZAKO*/ /*S P_BULLET*/ /*SP_ENEMY_HOMING*/
		r					= sprite_add_gu(TAMA_TYPE_HOMING16_PNG);	/*�f�ޕύX*/ /*20"rotating_rocket.png"*/
		r->type 			= SP_ZAKO;/*SP_ZAKO*/ /*S P_BULLET*/ /*SP_ENEMY_HOMING*/
		r->callback_mover	= enemy_homing_move;
		r->flags			|= (SP_FLAG_VISIBLE|SP_FLAG_COLISION_CHECK|SP_FLAG_TIME_OVER);
		r->x256 			= src->x256;
		r->y256 			= src->y256;
//
		r->m_angleCCW512	= ((i<<6));//	data->aaa_angle512		= (0);
//
		ENEMY_HOMING_DATA *data;
		data				= mmalloc(sizeof(ENEMY_HOMING_DATA));
		r->data 			= data;
//
		/* ���_ */
		data->p0_x256			= src->x256;
		data->p0_y256			= src->y256;
		/* ���_�p�̈ړ����������߂� */
		r->vx256 = ((player->x256 - src->x256)>>6);/* 64���� */
		r->vy256 = ((player->y256 - src->y256)>>6);/* 64���� */
//
		/*data->base.*/r->base_score		= score(5*2);
		/*data->base.*/r->base_health		= 1;		/* �Ȃ񂩋����C������ */

		#if 000
//
		data->p2_x256		= player->x256;
		data->p2_y256		= player->y256;
//
		data->p1_x256		= player->x256+(((sin512((i<<6))))<<6);/* <<6 ���a64 */
		data->p1_y256		= player->y256+(((cos512((i<<6))))<<6);/* <<6 ���a64 */
//
		{
			send1_obj->x256 = data->p1_x256;	/* ���e�ʒu�� offset �p */
			send1_obj->y256 = data->p1_y256;	/* ���e�ʒu�� offset �p */
			int aa_angleCCW512;
			aa_angleCCW512/*data->aaa_angle512*/ = angle_jikinerai512(send1_obj,src);
			r->tmp_angleCCW512 = (((i<<6))-(aa_angleCCW512))>>(6-4);	/* 64����, ���Z�p�x */
		}
		data->state 		= (0);
//
		data->aaa_speed256	= t256(2.0);	/*t256(0.5)+((difficulty)<<7)*/ /*(0==difficulty)?(1):(2)*/
		#endif
		data->time_out		= ((3*64)+1);//data->range	= 200;	data->delay 	= 1/*10*/;	/*enemy_homing_update(r);*/
	}
}

		//	s->yx_an im_frame = (deg_512_to_360(data->angle512)*20)/360; s->yx_an im_frame %=20;
		//	s->yx_an im_frame = (((data->angle512)*(20/2))>>8);
		//	s->yx_an im_frame = (((data->angle512)*(16/2))>>8);
			#if 1
			#else
			/*��������݊�*/
		//	{int aaa512;
		//		aaa512 = 128+ 512 - data->angle512;
		//		mask512(aaa512);
		//		s->yx_an im_frame = (((aaa512))>>(8-3));
		//	}
			#endif

		/* nur jeden 10ten Frame Winkel aktualisieren */
		//data->delay=10;
		//enemy_homing_update(s);
//			ENEMY_HOMING_DATA *data = (ENEMY_HOMING_DATA *)s->data;
			/* Zielwinkel erfassen */

	//SDL_Surface *tmp;
		//tmp=sprite_getcurrimg(s);
		//parsys_add(tmp,1,1,s->x,s->y,10,0,0,30,EXPLODE|DIFFSIZE,NULL);
		//SDL_FreeSurface(tmp);

//	else	/*if (data->delay > 0)*/
//	{
//	}
		#if 0
		/*�_��mono_angle_move(s,(BULLET_ANGLE_DATA *)d);*/
		#else
	//	s->x256 += ((cos512((data->angle512))*data->speed256)>>8)/**fps_fa ctor*/;
	//	s->y256 += ((sin512((data->angle512))*data->speed256)>>8)/**fps_fa ctor*/;
//		s->x256 += ((sin512((data->angle512))*data->speed256)>>8)/**fps_fa ctor*/;
//		s->y256 += ((cos512((data->angle512))*data->speed256)>>8)/**fps_fa ctor*/;
		#endif

