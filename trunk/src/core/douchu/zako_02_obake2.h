

/*static*/extern void bullet_create_enemy_homing(SPRITE *src);

/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	"���΂�2"(�΂��΂�) 	"���ы�1",		"EYEFO",
	-------------------------------------------------------
	�o�O����H�H�H
---------------------------------------------------------*/

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_obake2(SPRITE *src)
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
		if (src->target_y256 < t256(difficulty<<5) )/* ��Փx�ɂ�艺�����Ă���ʒu���Ⴄ�B�ő�96(3*32) */
		{
			src->target_y256 += t256(1.0);/*fps_factor*/
		}
	}
//
	if (0 == src->flag1)
	{
		src->radius256 += t256(1.0);	/*fps_factor*/
		if (src->radius256 > t256(64.0)/*150*/) /* �ő唼�a */
		{
			src->flag1=1;
		}
	}
	else
	{
		src->radius256 -= t256(2.0);	/*fps_factor*/	/*1*/
		if (src->radius256 < t256(8.0)) /* �ŏ����a */	/*<=10*/
		{
			src->flag1=0;
		}
	}
	src->tmp_angleCCW1024 += (2);	/*fps_factor*/	/* cv1024r(5) �������� */
	mask1024(src->tmp_angleCCW1024);
	//
	zako_move_type11_radius(src);

	if (0 != difficulty)/* easy �̓z�[�~���O�����Ȃ� */
	{
	//	if (src->enemy_rank)
		{
		//	if (0==(ra_nd()%(1000-(src->enemy_rank)*100)))
		//	if (0==(ra_nd()%((10-(src->enemy_rank))*100)))
	//		if (0==(ra_nd()&((1<<(10-(src->enemy_rank)))-1)))/*�e�L�g�[*/	/* [10]�m��(���p�x)��������C������ */
//			if (0==(ra_nd()&((1<<(12-(src->enemy_rank)))-1)))/*�e�L�g�[*/
			if (0==(ra_nd()&((1<<(11-(3/*src->enemy_rank*/)))-1)))/*�e�L�g�[*/
			{
				/* ��ʊO�Ȃ�Ό����Ȃ� */
				if (0 < src->cy256)	/* ��ʓ��Ȃ猂�� */
				{
					bullet_create_enemy_homing(src);	/* �G���̗U���~�T�C�����쐬���� */
				}
			}
		}
	}
	/* �A�j���[�V���� */
	{
	//	src->type			= TEKI_28_YOUSEI2_5+(((src->cy256>>8)&0x03));
		zako_anime_type02(src, TEKI_28_YOUSEI2_5);
	}
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

global void add_zako_obake2(STAGE_DATA *l)
{
	SPRITE *h;
	h						= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_28_YOUSEI2_5;
		h->callback_mover		= move_obake2;

		h->radius256			= t256(10);
		h->flag1				= 0;
		h->tmp_angleCCW1024 	= (ra_nd()&(1024-1));
		h->jyumyou				= (1200);
		h->target_x256			= t256( 62) + ((ra_nd()&((256*256)-1)))/*t256*/  /*(GAME_WIDTH/2)*/;/* GAME_WIDTH 380 ~= 380 == 62+256+62 */
		h->target_y256			= t256(-32);/*-100*/
	}
}
