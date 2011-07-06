
/*---------------------------------------------------------
	�����͕핗	�` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"����1",		"PROBALL",
	-------------------------------------------------------
	-------------------------------------------------------
��	��	 ��
��. ��	/��
�� .�� / ��
��	��/  ��
����++����
   /  .
---------------------------------------------------------*/

/*---------------------------------------------------------
	�G����
---------------------------------------------------------*/

//static void lose_tatsumaki1(SPRITE *src)
//{
//	destoroy++;
//	if ( (N UM_OF_ENEMIES-1) < destoroy/*all_destoroy*/)
//	{
//		destoroy = 0;
//		if (rand_percent(50/*10*/))
//		{
//			item_create(/*zzz*/ src, (SP_ITEM_04_BOMB), 1, ITEM_MOVE_FLAG_06_RAND_XY);
//		}
//	}
//}
/*50%(SP_ITEM_01_P008 or SP_ITEM_04_BOMB) (SP_ITEM_01_P008+(ra_nd()&1)) */
//66%==SP_ITEM_04_BOMB or 33%==SP_ITEM_01_P008 (SP_ITEM_EXTRA_HOMING+(ra_nd()&3/*%3*/)),

/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_tatsumaki1(SPRITE *src)
{
	if (512 > src->jyumyou)/* ���ֈړ� */
	{
		src->cy256 += t256(1.25);/*fps_factor*/
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	}
	else
	if (512+512 > src->jyumyou)/* ��ֈړ� */
	{
		src->cy256 -= t256(1.125);/*fps_factor*/
		if (0 > src->cy256 )
		{
		//	src->speed256 = (-(src->speed256));/* ���] */
			src->jyumyou = (512-1);
		}
	}
	else
//	if (512+512+512 > src->jyumyou)/* ���ֈړ� */
	{
		src->cy256 += t256(1.0);/*fps_factor*/
		if (t256(GAME_HEIGHT/3/*2*/) < src->cy256 ) /*t256(250)*/
		{
			src->jyumyou = (512+512-1);
		}
	}
	//
	src->speed256++;	/* t256(0.1) t256(0.2)������ */
	src->px256 -= (src->speed256);/*fps_factor*/
	if ((0 > src->px256)||(t256(GAME_WIDTH-10) < src->px256))
	{
		src->speed256 = (-(src->speed256));/* ���] */
	}
//
	src->rotationCCW1024 += (src->vx_r);
	if (0 < (src->vx_r) )
			{	src->cx256	= (src->px256); 	}
	else	{	src->cx256	= t256(GAME_WIDTH)-(src->px256); }
	mask1024(src->rotationCCW1024);
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

global void add_zako_tatsumaki1(STAGE_DATA *l)
{
	SPRITE *h;
	h							= sprite_add_gu_error();
	if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
	{
		add_zako_common(l, h);
		h->m_Hit256R			= ZAKO_ATARI16_PNG;
		h->type 				= TEKI_62_TATSUMAKI;
		h->callback_mover		= move_tatsumaki1;
	//	h->callback_loser		= lose_tatsumaki1;/*old*/
	//	h->rotationCCW1024		= ((i&(16-1))<<6);
	//
//		h->jyumyou = (512+512+512-1);
		h->speed256 			= t256(1.2);/* t256(2.0)������ */	/*3+difficulty+lv/3*/
		h->px256				= t256(GAME_WIDTH-10);	/*0*/	/*t256(GAME_WIDTH/2)*/
		/* ��ʉE���̏ꍇ�A�������֐i�� */
		if ( t256((GAME_WIDTH)/2) < h->cx256)
		{
			h->vx_r = (10); /*������*/
		}
		else
		{
			h->vx_r = (-10);
		}
	}
}
