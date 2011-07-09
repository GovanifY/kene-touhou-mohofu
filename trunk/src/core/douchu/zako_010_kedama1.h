
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"�ы�1",	"BADGUY",
	-------------------------------------------------------
	�{�Ɩы�
---------------------------------------------------------*/

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void my_nerai(SPRITE *src)
{
	obj_send1->cx256	= (src->target_x256);						/* �e��x256 */
	obj_send1->cy256	= (src->target_y256-t256(64)/*+(20)*/); 	/* �e��y256 */
	tmp_angleCCW65536_src_nerai(obj_send1, src);
	src->vx256			= ((sin65536(src->tmp_angleCCW65536)));
	src->vy256			= ((cos65536(src->tmp_angleCCW65536)));
}

static void move_kedama1(SPRITE *src)
{
	if (512 > src->jyumyou)/* �ޏ� */
	{
		my_nerai(src);
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	}
	else
	if (512+64 > src->jyumyou)/* */
	{
		if (0==(src->jyumyou & 0x0f))
		{
			if (ENEMY_LAST_SHOT_LINE256 > src->cy256)	/* ���̃��C����艺����͓G�������Ȃ� */
			{
				br.BULLET_REGIST_00_speed256				= t256(1.0);	//t256(1.0)+t256((teki_rank)>>1),
				br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN;
			//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
				br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_KOME_00_SIRO+(src->cx256&0x07);
				br.BULLET_REGIST_06_n_way					= (3+(cg_game_difficulty)+(cg_game_difficulty));
				br.BULLET_REGIST_07_VECTOR_div_angle1024	= (int)(1024/24);
				bullet_regist_multi_vector_send1_xy_src(src); 	/* �e��x256 y256 ���S���甭�e�B */
			}
		}
	}
	else
//	if (512+64+64 > src->jyumyou)	/* �o�ꂷ�� */
	{
		my_nerai(src);
		{
			SPRITE *zzz_player;
			zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
			if (//(src->cx256 >= zzz_player->cx256) ||
				(src->cy256 > t256(GAME_HEIGHT/2) ) /*(480-80)*/ /*(272-32)*/
			 )
			{
				src->vy256 = (-src->vy256);
				src->jyumyou	= (512+64-1);
				/* ���ڕW�́A */
				src->target_x256		= t256(((480-128)/2));	//= t256(-100);
				src->target_y256		= t256(0);/*((zzz_player->cy256)&(128-1))*/
				src->speed256			= t256(2.5/*5.0*/);/*fps_factor*/	/*6*/
			}
		}
	}
	/* �ړ����� */
	src->cx256 += (((src->vx256)*src->speed256)>>8);
	src->cy256 += (((src->vy256)*src->speed256)>>8);
	/* �A�j���[�V���� */
	src->zako_anime_rotate_angle1024 = ( 10);
	zako_anime_type04(src); 	/* �O����] */
}

/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

static void regist_zako_010_kedama1(GAME_COMMAND *l, SPRITE *h)
{
	h->color32				= MAKE32RGBA(0xff, 0xff, 0xff, 0xaa);		/*�����ۂ� */
//
	h->jyumyou			= (512+64+64);
	{
		/* ���ڕW�̓v���C���[�̂����ʒu */
		SPRITE *zzz_player;
		zzz_player = &obj99[OBJ_HEAD_02_KOTEI+FIX_OBJ_00_PLAYER];
		h->target_x256				= (zzz_player->cx256);
		h->target_y256				= (zzz_player->cy256);
	}
	h->vx256				= (0);
	h->vy256				= (0);
//	h->speed256 			= ((ra_nd()&((256*2)-1)))*(1+difficulty+teki_rank/3);/*fps_factor*/
	h->speed256 			= ((ra_nd()&((256*2)-1))) + (((cg_game_difficulty)<<8)+ (/*teki_rank*/5<<7/**86*2*/) );/*fps_factor*/
}
