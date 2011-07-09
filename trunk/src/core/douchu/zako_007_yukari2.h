
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"���ґ�2",		"GREETER",
	-------------------------------------------------------
	�T�@���ɑ�����A�ґ���s���Ă���U�R�G
	-------------------------------------------------------
	ming_greeter
	rwingx_curver(ming)�Ɏ��Ă�
---------------------------------------------------------*/

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_yukari2(SPRITE *src)
{
	if (byou60(10) > src->jyumyou)
	{
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	}
	else
	{
		if (src->cy256 >= src->max_y256)
		{
			src->jyumyou = byou60(10);
			if (t256(1.5/*2.0*/) < src->speed256)
			{	/* ���� */
				src->speed256 -= t256(0.2);
			//	src->speed256 = t256(2)/*3+difficulty*/;
			}
		//
		//	send1_xy(src);	/* �e��x256 y256 ���S���甭�e�B */
			obj_send1->cx256	= (t256(GAME_WIDTH/2)); 		/* �e��x256 */
			obj_send1->cy256	= (t256(0));					/* �e��y256 */
			tmp_angleCCW65536_src_nerai(obj_send1, src);
			src->vx256=((sin65536((src->tmp_angleCCW65536))*src->speed256)>>8);		/* CCW�̏ꍇ */
			src->vy256=((cos65536((src->tmp_angleCCW65536))*src->speed256)>>8);
		}
//		br.BULLET_REGIST_00_speed256				= t256(2.5);					/* �e�� */	/*�����e*/	/*t256(3+teki_rank)+(0(teki_rank<<6))*/
		br.BULLET_REGIST_00_speed256				= t256((cg_game_difficulty))+t256(1.5); 			/* �e�� */
		br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN; 		/* ���@�_���e */	/* ���˒��S�p�x / ����@�\(���@�_��/��) */
	//	br.BULLET_REGIST_03_VECTOR_regist_type		= VEC TOR_REGIST_TYPE_00_MULTI_VECTOR;
		br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_MINI8_07_DAI_DAI;		/* [�Ԓe] */ /* �e�O�� */
		br.BULLET_REGIST_06_n_way					= (1);							/* [1way] */
	//	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (0);							/* �_�~�[�p�x(���g�p) */
		zako_shot_ra_nd(src);
	}
	/* ���Ă邪������ƈႤ--�ȉ�rwingx.c�Ɠ��� */
	src->cx256 += (src->vx256);/* fps_factor */
	src->cy256 += (src->vy256);/* fps_factor */
	/* �A�j���[�V���� */
	src->zako_anime_rotate_angle1024 = ( 10);
	zako_anime_type04(src); 	/* �O����] */
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

static void regist_zako_007_yukari2(GAME_COMMAND *l, SPRITE *h)
{
	h->max_y256 			= (t256(GAME_HEIGHT)-t256(60));
	tmp_angleCCW65536_jiki_nerai(h);
	h->speed256 			= (t256(2.5/*3.0*/)+((3/*di fficulty*/)<<4) ); /*4*/ /* �n�߂����͑��� */
	/* CCW�̏ꍇ */
	h->vx256=((sin1024(((h->tmp_angleCCW65536)>>6))*h->speed256)>>8);
	h->vy256=((cos1024(((h->tmp_angleCCW65536)>>6))*h->speed256)>>8);
//
//	h->jyumyou				= byou60(60);/* �e�L�g�[ */
}
