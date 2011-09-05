
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�����̃U�R
	-------------------------------------------------------
		"�Ԗы�1",		"MINE",
	-------------------------------------------------------
	���邮�邻�̏�œ����Ȃ��ŁA��]�A�j�����Ă�
	�ق��Ƃ���8�ɕ��􂵂�
	�Ō�ɍ����e�����ė�����(��)
	-------------------------------------------------------
	���̃L�����̂ݕ��􂷂�񂾂��ǁA�V�X�e���Ƃ���
	����̓T�|�[�g���Ȃ��Ȃ邩������Ȃ�(?)�B
	���̏ꍇ�͎n�߂���A8�d�Ȃ��ďo�Ă���B
	����́A�܂�A���X�e������T�|�[�g�����Ƃ�����A
	�ŏI�I�ȃA���X�e�̎d�g��(�܂��m�肵�Ă��Ȃ�)�Ɠ����d�g�݂ɂȂ�Ǝv���B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�G�ړ�(�����)
---------------------------------------------------------*/

static void move_aka_kedama1_2nd(OBJ *src)
{
	if (ENEMY_LAST_SHOT_LINE256 > src->cy256)	/* ���̃��C����艺����͓G�������Ȃ� */
	{
		/*
		0  0/100   0/65536
		1  1/90  728/65536
		2  1/80  819/65536
		3  1/70  936/65536
		*/
		static const u16 kakuritu_tbl[4] =
		{ 0, 728, 819, 936 };
		if (kakuritu_tbl[((cg.game_difficulty))/*(teki_rank)*/] > (ra_nd()&(65536-1)))
		{
			zako_shot_supeka(src, ZAKO_SPEKA_0c_aka_kedama1);
		}
	}
/* CCW�̏ꍇ */
	src->cx256 += (src->vx256);/*fps_factor*/
	src->cy256 += (src->vy256);/*fps_factor*/
	gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	/* �A�j���[�V���� */
	src->zako_anime_rotate_angle1024 = ( 16);/*(20)*/
	zako_anime_type04(src); 	/* �O����] */
}


/*---------------------------------------------------------
	�G��ǉ�����(�ϐg)
---------------------------------------------------------*/

static void add_local_aka_kedama1_2nd(OBJ *src/*, int lv*/)
{
	unsigned int i;
	for (i=0; i<8; i++)/*����*/
	{
		OBJ *h;
		h						= obj_add_A01_teki_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->cx256				= src->cx256;			/* �����O�����̂܂܈����p�� */
			h->cy256				= src->cy256;			/* �����O�����̂܂܈����p�� */
			h->obj_type_set 		= src->obj_type_set;	/* �����O�����̂܂܈����p�� */
			h->base_score			= src->base_score;		/* �����������ق����X�R�A���҂��� */	/* �����O�����̂܂܈����p�� */
			h->base_hp				= src->base_hp; 		/* �����O�̗̑͂����̂܂܈����p��(���炵�Ă���΂��̕���炩��) */
			//
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
			h->callback_mover		= move_aka_kedama1_2nd;
			h->callback_loser		= src->callback_loser;	/* �����O�����̂܂܈����p�� */
			h->callback_hit_teki	= callback_hit_zako;	/* �u�U�R�Ɏ��e�����������ꍇ�̏����v�ɁA�W���̏�����ݒ� */
			//
			{
				const int data_zzz_angle1024				= (i<<7);	/* ����p�x */	//	(/*360*/1024/8)*i;
			//	const int data_zzz_speed256 				= (t256(1.5)+((cg_game_di fficulty)<<6));	/* �������x */	/*(3.0)*/
				const int data_zzz_speed256 				= (t256(1.5)+((3)<<6)); 	/* �������x */	/*(3.0)*/
				//------------------
				HATSUDAN_01_speed256	= (data_zzz_speed256);
				HATSUDAN_03_angle65536	= deg1024to65536(data_zzz_angle1024);
				sincos256();/*(�j�󃌃W�X�^�����̂Œ���)*/
				h->vx256 = REG_03_DEST_Y;//sin_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
				h->vy256 = REG_02_DEST_X;//cos_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
				//------------------
			}
		}
	}
}

//			/*data->base.*/h->base_score		= score(10*2)*(1+(2/*di fficulty*/));		/*(teki_rank)*/ 	/*9*/
//			/*data->base.*/h->base_hp			= (8*(1+ 9))+(1/*di fficulty*/<<(4+3)); 	/*(teki_rank)*/

/*---------------------------------------------------------
	�G�ړ�(�{��)
---------------------------------------------------------*/

static void move_aka_kedama1_1st(OBJ *src)
{
	if (2 == src->jyumyou)	/* ���� */
	{
		add_local_aka_kedama1_2nd(src);/* �ϐg ���� */	/*,data->teki_rank*/
		src->jyumyou = JYUMYOU_NASI;		/* �{�̂͂����܂� */
	}
	else
	if (31+2 > src->jyumyou)
	{
		src->zako_anime_rotate_angle1024 = (  0);	/* �~�܂� */
	}
	else
	{
										/* �ړ� move */
		src->cx256 += (src->vx256); 	/*fps_factor*/
		src->cy256 += (src->vy256); 	/*fps_factor*/
		src->zako_anime_rotate_angle1024 = ( 10);	/* �O����] */
	}
	/* �A�j���[�V���� */
	zako_anime_type04(src); 	/* �O����] */
}


/*---------------------------------------------------------
	�G��ǉ�����(�{��)
---------------------------------------------------------*/

static void regist_zako_005_aka_kedama1(GAME_COMMAND *l, OBJ *h)
{
//	/* �����ʒu */
	REG_00_SRC_X	= ((ra_nd()&((256*256)-1)))+t256(64);/*320?*/ /*t256(ra_nd()%270)+t256(50)*/	/* �e��x256 */
	REG_01_SRC_Y	= ((ra_nd()&((256*128)-1)))+t256(16);/*350?*/ /*t256(ra_nd()%300)+t256(50)*/	/* �e��y256 (256*256) */
	set_REG_DEST_XY(h);
	tmp_angleCCW65536_src_nerai();
	//------------------
	HATSUDAN_01_speed256	= (t256(1.5));
//	HATSUDAN_03_angle65536	= (HATSUDAN_03_angle65536);
	sincos256();/*(�j�󃌃W�X�^�����̂Œ���)*/
	h->vx256 = REG_03_DEST_Y;//sin_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
	h->vy256 = REG_02_DEST_X;//cos_value_t256 // ��CCW�̏ꍇ(�ECW�̏ꍇ�Ƃ�xy���t)
	//------------------
//
	h->jyumyou				= (80)+byou60(1)+(2);
}
