
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
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

static void move_aka_kedama1_2nd(SPRITE *src)
{
	{
		/*
		0  0/100   0/65536
		1  1/90  728/65536
		2  1/80  819/65536
		3  1/70  936/65536
		*/
		static const u16 kakuritu_tbl[4] =
		{ 0, 728, 819, 936 };
	//	if (0==(ra_nd()%(100-teki_rank*10)))
		if (kakuritu_tbl[((cg_game_difficulty))/*(teki_rank)*/] > (ra_nd()&(65536-1)))
		{
			obj_send1->cx256							= src->cx256;
			obj_send1->cy256							= src->cy256;
			br.BULLET_REGIST_00_speed256				= t256(2.5)+(0/*(teki_rank<<6)*/);				/* �e�� */	/*�����e*/	/*t256(3+teki_rank)*/
		//	br.BULLET_REGIST_00_speed256				= speed256; 					/* �e�� */
			br.BULLET_REGIST_02_VECTOR_angle1024		= ANGLE_JIKI_NERAI_DAN; 		/* ���@�_���e */	/* ���˒��S�p�x / ����@�\(���@�_��/��) */
			br.BULLET_REGIST_04_bullet_obj_type 		= BULLET_MINI8_07_DAI_DAI;		/* [�Ԓe] */ /* �e�O�� */
			br.BULLET_REGIST_05_regist_type 			= REGIST_TYPE_00_MULTI_VECTOR;
			br.BULLET_REGIST_06_n_way					= (1);							/* [1way] */
		//	br.BULLET_REGIST_07_VECTOR_div_angle1024	= (0);							/* �_�~�[�p�x(���g�p) */
			bullet_regist_vector();
		}
	}
/* CCW�̏ꍇ */
	src->cx256 += (src->vx256);/*fps_factor*/
	src->cy256 += (src->vy256);/*fps_factor*/
	gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
//
	src->rotationCCW1024 += (16);	/*(20)*/
	mask1024(src->rotationCCW1024);
}


/*---------------------------------------------------------
	�G��ǉ�����(�ϐg)
---------------------------------------------------------*/

static void add_local_aka_kedama1_2nd(SPRITE *src/*, int lv*/)
{
	int i;
	for (i=0; i<8; i++)/*����*/
	{
		SPRITE *h;
		h						= obj_add_01_teki_error();
		if (NULL!=h)/* �o�^�ł����ꍇ�̂� */
		{
			h->m_Hit256R			= ZAKO_ATARI16_PNG;
		//	h->type 				= TEKI_56_CHEN;
			h->type 				= src->type;			/* �����O�����̂܂܈����p�� */
			h->callback_mover		= move_aka_kedama1_2nd;
			h->callback_loser		= lose_random_item; 	/* �����������ق����A�C�e�����҂��� */
			h->callback_hit_teki	= callback_hit_zako;		/* �u�U�R�Ɏ��e�����������ꍇ�̏����v�ɁA�W���̏�����ݒ� */
			//
			h->flags				= src->flags;			/* �����O�����̂܂܈����p�� */
			h->cx256				= src->cx256;			/* �����O�����̂܂܈����p�� */
			h->cy256				= src->cy256;			/* �����O�����̂܂܈����p�� */
			h->base_score			= src->base_score;		/* �����������ق����X�R�A���҂��� */
			h->base_hp				= src->base_hp; 		/* �����O�̗̑͂����̂܂܈����p��(���炵�Ă���΂��̕���炩��) */
			{
				const int data_zzz_angle1024				= (i<<7);	/* ����p�x */	//	(/*360*/1024/8)*i;
				const int data_zzz_speed					= (t256(1.5)+((cg_game_difficulty)<<6));	/* �������x */	/*(3.0)*/
			//
				h->vx256 = ((sin1024((data_zzz_angle1024))*(data_zzz_speed))>>8);/*fps_factor*/
				h->vy256 = ((cos1024((data_zzz_angle1024))*(data_zzz_speed))>>8);/*fps_factor*/
			}
		}
	}
}

//			/*data->base.*/h->base_score		= score(10*2)*(1+(2/*di fficulty*/));		/*(teki_rank)*/ 	/*9*/
//			/*data->base.*/h->base_hp			= (8*(1+ 9))+(1/*di fficulty*/<<(4+3)); 	/*(teki_rank)*/

/*---------------------------------------------------------
	�G�ړ�(�{��)
---------------------------------------------------------*/

static void move_aka_kedama1_1st(SPRITE *src)
{
	if (2 == src->jyumyou)	/* ���� */
	{
		add_local_aka_kedama1_2nd(src/*,data->teki_rank*/);/* �ϐg ���� */
		src->jyumyou = JYUMYOU_NASI;		/* �{�̂͂����܂� */
	}
	else
	if (31+2 > src->jyumyou)
	{
		;							/* �~�܂� */
	}
	else
	{
										/* �ړ� move */
		src->cx256 += (src->vx256); 	/*fps_factor*/
		src->cy256 += (src->vy256); 	/*fps_factor*/
		//
		src->rotationCCW1024 += (10);	/* �O����] */
		mask1024(src->rotationCCW1024);
	}
}


/*---------------------------------------------------------
	�G��ǉ�����(�{��)
---------------------------------------------------------*/

static void regist_zako_005_aka_kedama1(GAME_COMMAND *l, SPRITE *h)
{
//		/* �����ʒu */
	obj_send1->cx256		= ((ra_nd()&((256*256)-1)))+t256(64);/*320?*/ /*t256(ra_nd()%270)+t256(50)*/	/* �e��x256 */
//	obj_send1->cy256		= ((ra_nd()&((256*256)-1)))+t256(16);/*350?*/ /*t256(ra_nd()%300)+t256(50)*/
	obj_send1->cy256		= ((ra_nd()&((256*128)-1)))+t256(16);/*350?*/ /*t256(ra_nd()%300)+t256(50)*/	/* �e��y256 */
	tmp_angleCCW1024_src_nerai(obj_send1, h);
	h->vx256				= ((sin1024(h->tmp_angleCCW1024)*t256(1.5))>>8);/*fps_factor*/	/* CCW�̏ꍇ */
	h->vy256				= ((cos1024(h->tmp_angleCCW1024)*t256(1.5))>>8);/*fps_factor*/
//
	h->jyumyou				= 80+byou60(1)+2;
}
