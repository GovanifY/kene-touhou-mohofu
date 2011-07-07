
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
		"���΂�1",		"DRAGER",
	-------------------------------------------------------
	�ˌ����΂��A(���Ȃ肤��o���A���΂�����Ȃ����������H)
	-------------------------------------------------------
�d�l�F
	�� dat��score���w�肷��B
	�� dat�ő̗͂��w�肷��B
	�� dat�Ŋl���A�C�e�����w�肷��B
	�� dat�ŏo���ʒu x, y ���W���w�肷��B
	�� �G���̑��x�͓�Փx�Ɋ֌W�Ȃ��Œ�B���\���߁B(t256(2.0))
	�� �G���͎��@�_���œˌ����Ă���B(�r���Ō����͕ς��܂���)
	�� ���Ԋu�Œe�������Ă���B(�����|���������L��)
		easy	�����Ȃ��B
		normal	1way�e�B
		hard	3way�e�B
		luna	5way�e�B
	�� �e�̑��x�́A�G���̑��x���x���B���x�͓�Փx�Ɋ֌W�Ȃ��Œ�B(t256(1.25))
	(��ʂɎc��̂ő����|���������L��)
	-------------------------------------------------------
	�܂�t�����[�E�C�U���E�F�C�̋t�o�[�W�����ł��B
	�ƌ���������������₷�����ȁH
---------------------------------------------------------*/

/*---------------------------------------------------------
	"覐�1", "�� �� ��1",		"MING",
	-------------------------------------------------------
�d�l�F
	�� dat��score���w�肷��B
	�� dat�ő̗͂��w�肷��B
	�� dat�Ŋl���A�C�e�����w�肷��B
	�� dat�ŏo���ʒu x, y ���W���w�肷��B



	�P�̂ł΂�΂�ɁA�����Ă���U�R�G
	���@�_���ԒP�e��A�˂��Ă���
	-------------------------------------------------------
	�����ɕK�v���Y�񂾂��A���Ǝ����d�l�̕ґ���s�G�͕K�v�Ȃ��B
	�P���ɗ����Ă���G�ɕύX����B
	-------------------------------------------------------
	10�@�O�p����ɑ�����A�ґ���s���Ă���U�R�G
	ming
	rwingx_curver�Ɏ��Ă�
---------------------------------------------------------*/


/*---------------------------------------------------------
	�G�ړ�
---------------------------------------------------------*/

static void move_obake1(SPRITE *src)
{
	/* �U�R��������1[�b]�ȏ�o�߂��Ă���΁A�`�F�b�N */
	if (( (JYUMYOU_ZAKO_HASSEI-60) > src->jyumyou ))
	{
		gamen_gai_nara_zako_osimai(src);/* ��ʊO�Ȃ炨���܂� */
	}
	if (0==((src->jyumyou)&0x3f))/* ���Ԋu(�e�L�g�|) */
	{
		if (ENEMY_LAST_SHOT_LINE256 > src->cy256)	/* ���̃��C����艺����͓G�������Ȃ� */
		{
			if (0<(cg_game_difficulty))
			{
				obj_send1->cx256 = src->cx256;
				obj_send1->cy256 = src->cy256;
				br.BULLET_REGIST_00_speed256					= (t256(1.25));
				br.BULLET_REGIST_02_VECTOR_angle1024			= (src->tmp_angleCCW1024);		/*(ANGLE_JIKI_NERAI_DAN)*/
				br.BULLET_REGIST_04_bullet_obj_type 			= BULLET_KNIFE20_04_AOI;		/* [�i�C�t�e] */
				br.BULLET_REGIST_05_regist_type 				= REGIST_TYPE_00_MULTI_VECTOR;
				br.BULLET_REGIST_06_n_way						= ((cg_game_difficulty)+(cg_game_difficulty)-1);	/* [1 3 5way] */
				br.BULLET_REGIST_07_VECTOR_div_angle1024		= (int)(1024/(18)); 			/* �����p�x */
				bullet_regist_vector();
			}
		}
	}
//
	{
		src->cx256 += (src->vx256);/*fps_factor*/
		src->cy256 += (src->vy256);/*fps_factor*/
	}
	/* �A�j���[�V���� */
	{
		if (0==src->gra_anime_type)
		{
			/* offset�����͉�] */
			src->rotationCCW1024 += (10);/*�O����]*/
			mask1024(src->rotationCCW1024);
		}
		else
		{
		//	src->type			= TEKI_28_YOUSEI2_5+(((src->cy256>>8)&(4-1)));
			zako_anime_type02(src, TEKI_28_YOUSEI2_5);
		}
	}
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

static void regist_zako_012_obake1(GAME_COMMAND *l, SPRITE *h)
{
//	h->jyumyou				= (60);
//
	tmp_angleCCW1024_jiki_nerai(h);
	h->vx256 = ((sin1024((h->tmp_angleCCW1024))));
	h->vy256 = ((cos1024((h->tmp_angleCCW1024))));
	h->vx256 += h->vx256;
	h->vy256 += h->vy256;
	h->gra_anime_type		= (1);	/* 0==��]�A�j���A�O���^�C�v */
}


/*---------------------------------------------------------
	�G��ǉ�����
---------------------------------------------------------*/

static void regist_zako_006_inseki1(GAME_COMMAND *l, SPRITE *h)
{
//	h->jyumyou				= (0x3ff);
//
	h->gra_anime_type		= (0);	/* 0==��]�A�j���A�O���^�C�v */
	{
	//
	u32 rrrr;
		rrrr = (ra_nd());
		h->vx256 = ((rrrr)&0x01ff);/*�E����*/
		/* ��ʉE���̏ꍇ�A�������֐i�� */
		if ( t256((GAME_WIDTH)/2) < h->cx256)
		{
			h->vx256 = (-(h->vx256));/*������*/
		}
		h->vy256 = ((rrrr>>16)&0x01ff) + t256(1.00);/*������*/
	}
}
			/* �����ʒu */
//			h->cx256 = (((u32)l->user_x)<<8);//((rrrr)&0xffff) + t256((GAME_WIDTH-256)/2);
//			h->cy256 = -t256(30);
