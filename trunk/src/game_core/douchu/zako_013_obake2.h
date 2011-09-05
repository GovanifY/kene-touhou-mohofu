

/*static*/extern void bullet_create_teki_homing(OBJ *src);

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�����̃U�R
	-------------------------------------------------------
	"���΂�2"(�΂��΂�) 	"���ы�1",		"EYEFO",
	-------------------------------------------------------
	�o�O����H�H�H
---------------------------------------------------------*/

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_obake2(OBJ *src)
{
//	if (0 > src->jyumyou)
//	{
//		src->jyumyou = JYUMYOU_NASI;	/* �����܂� */
//	}
//	else
	if (600 > src->jyumyou) 	/* ��֋A�� */
	{
		src->target_y256 -= t256(1.0);/*fps_factor*/
	}
	else
	if (900 < src->jyumyou) 	/* ���֗��� */
	{
	//	if (src->target_y256 < t256(GAME_HEIGHT))
	//	if (src->target_y256 < t256(difficulty<<6) )/* ��Փx�ɂ�艺�����Ă���ʒu���Ⴄ�B�ő�192(3*64) */
	//	if (src->target_y256 < t256((cg_game_di fficulty)<<5) )/* ��Փx�ɂ�艺�����Ă���ʒu���Ⴄ�B�ő�96(3*32) */
		if (src->target_y256 < t256((3)<<5) )/* ��Փx�ɂ�艺�����Ă���ʒu���Ⴄ�B�ő�96(3*32) */
		{
			src->target_y256 += t256(1.0);/* fps_factor */
		}
	}
//
	if (0 == src->flag1)
	{
		src->radius256 += t256(1.0);	/* fps_factor */
		if (src->radius256 > t256(64.0))/*150*/ /* �ő唼�a */
		{
			src->flag1 = (1);
		}
	}
	else
	{
		src->radius256 -= t256(2.0);	/* fps_factor */	/* 1 */
		if (src->radius256 < t256(8.0)) /* �ŏ����a */	/* <=10 */
		{
			src->flag1 = (0);
		}
	}
	src->tmp_angleCCW1024 += (2);	/* fps_factor */	/* cv1024r(5) �������� */
	mask1024(src->tmp_angleCCW1024);
	//
	zako_move_type11_radius(src);
	/* ��ʊO�Ȃ�Ό����Ȃ� */
	if (0 < src->cy256) /* ��ʓ��Ȃ猂�� */
	{
		if (0==(ra_nd()&(256-1)))/*(1/256�̊m����) (�e�L�g�[)*/
		{
			bullet_create_teki_homing(src); /* �G���̗U���~�T�C�����쐬���� */
		}
	}
	/* �A�j���[�V���� */
	zako_anime_type02(src);
}
//	if (0==(ra_nd()%(1000-(src->teki_rank)*100)))
//	if (0==(ra_nd()%((10-(src->teki_rank))*100)))
//	if (0==(ra_nd()&((1<<(10-(src->teki_rank)))-1)))/*�e�L�g�[*/	/* [10]�m��(���p�x)��������C������ */
//	if (0==(ra_nd()&((1<<(12-(src->teki_rank)))-1)))/*�e�L�g�[*/
//	if (0==(ra_nd()&((1<<(11-(3/*src->teki_rank*/)))-1)))/*(1/256�̊m����) (�e�L�g�[)*/

//	if (0 != (cg_game_di fficulty))/* easy �̓z�[�~���O�����Ȃ�(-r34) */
//	{/* easy �̓z�[�~���O����(r34u1-) */
	//	if (src->teki_rank)
//		{
//		}
//	}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

static void regist_zako_013_obake2(GAME_COMMAND *l, OBJ *h)
{
	h->radius256			= t256(10);
	h->flag1				= (0);
	h->tmp_angleCCW1024 	= (ra_nd()&(1024-1));
	h->jyumyou				= (1200);
	h->target_x256			= t256( 62) + ((ra_nd()&((256*256)-1)));/*t256*/  /*(GAME_WIDTH/2)*/ /* GAME_WIDTH 380 ~= 380 == 62+256+62 */
	h->target_y256			= t256(-32);/*(-100)*/
}
