
/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"�Ζы�1",		"XEV",
	-------------------------------------------------------
	�Ζы�	�P�̂œ˂�����ł���B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_midori_kedama1(SPRITE *src)
{
	int speed256;
	if ((JYUMYOU_ZAKO_HASSEI-320) > src->jyumyou)		/* ���E */
	{
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
		speed256 = t256(3.5/*6*/);
	}
	else
	{
		SPRITE *zzz_player;
		zzz_player = &obj00[FIX_OBJ_00_PLAYER];
		if ((src->cx256 >= zzz_player->cx256)||
			(src->cy256 > t256(GAME_HEIGHT-48) ) )/*480-80*/
		{
			src->jyumyou	= (JYUMYOU_ZAKO_HASSEI-320);
			src->target_x256	= t256(-100);
		//	src->target_y256	= zzz_player->cy256-t256(48);
			src->target_y256	= zzz_player->cy256-t256(48);
			{
				obj_send1->cx256 					= src->cx256;
				obj_send1->cy256 					= src->cy256;
				br.BULLET_REGIST_speed256			= t256(2.75);				/* �e�� */
			//	br.BULLET_REGIST_speed256			= speed256; 				/* �e�� */
				br.BULLET_REGIST_angle1024			= ANGLE_JIKI_NERAI_DAN; 	/* ���@�_���e */	/* ���˒��S�p�x / ����@�\(���@�_��/��) */
			//	br.BULLET_REGIST_div_angle1024		= (0);						/* �_�~�[�p�x(���g�p) */
				br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU8_01_AKA;		/* [�Ԓe] */ /* �e�O�� */
				br.BULLET_REGIST_n_way				= (1);						/* [1way] */
				br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
				bullet_regist_vector();
			//
				lose_random_item(src);
			}//t256(1)+((4/*data->enemy_rank*/)<<8)
		}
		speed256 = t256(2.5/*3*/);
	}
	obj_send1->cx256 	= (src->target_x256);					/* �e��x256 */
	obj_send1->cy256 	= (src->target_y256);					/* �e��y256 */
	tmp_angleCCW1024_src_nerai(obj_send1, src);
	src->cx256 += ((sin1024(src->tmp_angleCCW1024)*speed256)>>8);/*fps_factor*/	/* CCW�̏ꍇ */
	src->cy256 += ((cos1024(src->tmp_angleCCW1024)*speed256)>>8);/*fps_factor*/
//
	src->rotationCCW1024 += (5);
	mask1024(src->rotationCCW1024);
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

global void add_zako_midori_kedama1(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_60_AKA_KEDAMA;
		h->callback_mover		= move_midori_kedama1;
	//
		{
			SPRITE *zzz_player;
			zzz_player = &obj00[FIX_OBJ_00_PLAYER];
			h->target_x256			= zzz_player->cx256;
		//	h->target_y256			= zzz_player->cy256; 		/* ���U�֎~ */
			h->target_y256			= ((zzz_player->cy256)>>1);
		}
	}
}
