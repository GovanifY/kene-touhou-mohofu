
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
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
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		if ((src->cx256 >= zzz_player->cx256)||
			(src->cy256 > t256(GAME_HEIGHT-48) ) )/*480-80*/
		{
			src->jyumyou	= (JYUMYOU_ZAKO_HASSEI-320);
			src->target_x256	= t256(-100);
		//	src->target_y256	= zzz_player->cy256-t256(48);
			src->target_y256	= zzz_player->cy256-t256(48);
			{
				obj_send1->cx256						= src->cx256;
				obj_send1->cy256						= src->cy256;
				br.BULLET_REGIST_00_speed256			= t256(2.75);				/* �e�� */
			//	br.BULLET_REGIST_00_speed256			= speed256; 				/* �e�� */
				br.BULLET_REGIST_02_VECTOR_angle1024	= ANGLE_JIKI_NERAI_DAN; 	/* ���@�_���e */	/* ���˒��S�p�x / ����@�\(���@�_��/��) */
				br.BULLET_REGIST_04_bullet_obj_type 	= BULLET_MINI8_01_AKA;		/* [�Ԓe] */ /* �e�O�� */
				br.BULLET_REGIST_05_regist_type 		= REGIST_TYPE_00_MULTI_VECTOR;
				br.BULLET_REGIST_06_n_way				= (1);						/* [1way] */
			//	br.BULLET_REGIST_07_VECTOR_div_angle1024		= (0);						/* �_�~�[�p�x(���g�p) */
				bullet_regist_vector();
			//
				lose_random_item(src);
			}//t256(1.0)+((4.0/*teki_rank*/)<<8)
		}
		speed256 = t256(2.5);/*(3.0)*/
	}
	obj_send1->cx256	= (src->target_x256);					/* �e��x256 */
	obj_send1->cy256	= (src->target_y256);					/* �e��y256 */
	tmp_angleCCW1024_src_nerai(obj_send1, src);
	src->cx256 += ((sin1024(src->tmp_angleCCW1024)*speed256)>>8);/*fps_factor*/ /* CCW�̏ꍇ */
	src->cy256 += ((cos1024(src->tmp_angleCCW1024)*speed256)>>8);/*fps_factor*/
//
	src->rotationCCW1024 += (5);
	mask1024(src->rotationCCW1024);
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

static void regist_zako_008_midori_kedama1(GAME_COMMAND *l, SPRITE *h)
{
	{
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		h->target_x256			= zzz_player->cx256;
	//	h->target_y256			= zzz_player->cy256;		/* ���U�֎~ */
		h->target_y256			= ((zzz_player->cy256)>>1);
	}
}
