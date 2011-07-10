
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�����̃U�R
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
			zako_shot_supeka(src, ZAKO_SPEKA_0a_midori1);
		}
		speed256 = t256(2.5);/*(3.0)*/
	}
	REG_00_SRC_X	= (src->target_x256);					/* �e��x256 */
	REG_01_SRC_Y	= (src->target_y256);					/* �e��y256 */
	set_REG_DEST_XY(src);	/* �e��x256 y256 ���S���甭�e�B */
	tmp_angleCCW65536_src_nerai();
	#if (0)//
	src->cx256 += ((si n1024((deg65536to1024(HATSUDAN_03_angle65536)))*(speed256))>>8);/*fps_factor*/	/* CCW�̏ꍇ */
	src->cy256 += ((co s1024((deg65536to1024(HATSUDAN_03_angle65536)))*(speed256))>>8);/*fps_factor*/
	#else
	{
		int sin_value_t256; 	//	sin_value_t256 = 0;
		int cos_value_t256; 	//	cos_value_t256 = 0;
		int256_sincos1024( (deg65536to1024(HATSUDAN_03_angle65536)), &sin_value_t256, &cos_value_t256);	/* �u1����65536�����v����u1����1024�����v�֕ϊ�����B */
		src->cx256 += ((sin_value_t256*(speed256))>>8);/*fps_factor*/
		src->cy256 += ((cos_value_t256*(speed256))>>8);/*fps_factor*/
	}
	#endif
	/* �A�j���[�V���� */
	src->zako_anime_rotate_angle1024 = (  5);
	zako_anime_type04(src); 	/* �O����] */
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
