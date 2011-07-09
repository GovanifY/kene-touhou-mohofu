
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
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
	if (0 < (src->vx_r) )
			{	src->cx256	= (src->px256); 	}
	else	{	src->cx256	= t256(GAME_WIDTH)-(src->px256); }
	//
	/* �A�j���[�V���� */
	src->zako_anime_rotate_angle1024 = (src->vx_r);
	zako_anime_type04(src);
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

static void regist_zako_003_tatsumaki1(GAME_COMMAND *l, SPRITE *h)
{
//	h->rotationCCW1024		= ((i&(16-1))<<6);
//
//	h->jyumyou = (512+512+512-1);
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