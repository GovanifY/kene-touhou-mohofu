
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�����̃U�R
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
			REG_00_SRC_X		= (t256(GAME_WIDTH/2)); 		/* �e��x256 */
			REG_01_SRC_Y		= (t256(0));					/* �e��y256 */
			set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
			tmp_angleCCW65536_src_nerai();
			#if (0)//
			src->vx256 = ((si n1024((deg65536to1024(HATSUDAN_03_angle65536)))*(src->speed256))>>8);/*fps_factor*/	/* CCW�̏ꍇ */
			src->vy256 = ((co s1024((deg65536to1024(HATSUDAN_03_angle65536)))*(src->speed256))>>8);/*fps_factor*/
			#else
			{
				int sin_value_t256; 	//	sin_value_t256 = 0;
				int cos_value_t256; 	//	cos_value_t256 = 0;
				int256_sincos1024( (deg65536to1024(HATSUDAN_03_angle65536)), &sin_value_t256, &cos_value_t256);	/* �u1����65536�����v����u1����1024�����v�֕ϊ�����B */
				src->vx256		= ((sin_value_t256*(src->speed256))>>8);/*fps_factor*/
				src->vy256		= ((cos_value_t256*(src->speed256))>>8);/*fps_factor*/
			}
			#endif
		}
		zako_shot_supeka(src, ZAKO_SPEKA_0d_yukari);
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
	h->speed256 			= (t256(2.5)+(48) );/*3.0*/ /*4*/ /* �n�߂����͑��� */
	//
	set_REG_DEST_XY(h);
	calculate_jikinerai();
	/* CCW�̏ꍇ */
	#if (0)//
	h->vx256		= ((si n1024((deg65536to1024(HATSUDAN_03_angle65536)))*(h->speed256))>>8);/*fps_factor*/ 	/* CCW�̏ꍇ */
	h->vy256		= ((co s1024((deg65536to1024(HATSUDAN_03_angle65536)))*(h->speed256))>>8);/*fps_factor*/
	#else
	{
		int sin_value_t256; 	//	sin_value_t256 = 0;
		int cos_value_t256; 	//	cos_value_t256 = 0;
		int256_sincos1024( (deg65536to1024(HATSUDAN_03_angle65536)), &sin_value_t256, &cos_value_t256);	/* �u1����65536�����v����u1����1024�����v�֕ϊ�����B */
		h->vx256		= ((sin_value_t256*(h->speed256))>>8);/*fps_factor*/
		h->vy256		= ((cos_value_t256*(h->speed256))>>8);/*fps_factor*/
	}
	#endif
//
//	h->jyumyou				= byou60(60);/* �e�L�g�[ */
}
