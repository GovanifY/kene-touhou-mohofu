
/*---------------------------------------------------------
 �����͕핗  �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�����̒e���𐶐����܂��B
---------------------------------------------------------*/


/*---------------------------------------------------------
	�����A�ԂĂ��ƒe��
---------------------------------------------------------*/

static void mima_boss01_nway_fire(SPRITE *src)
{
	static int adj_hari65536;	/* �J�n�n�_ */
//	adj_hari1024 = angle_jikinerai1024(obj_player, src);
	tmp_angleCCW65536_jikinerai(obj_player, src);/*���@�_���p�쐬*/
	adj_hari65536 = (src->tmp_angleCCW65536);
//
	const int speed256 = (0x200)+(ra_nd()&0xff)-((3-difficulty)<<6);/* ��Փx�ʂő��x�ቺ */
	const int src__x256 = (src->x256)+((0<src->vx256)?(t256(16)):(0));/* �E�ړ����͉E�����猂�� */
//
		//	br.BULLET_REGIST_div_angle65536 	= (0);									/* �_�~�[�����p�x(���g�p) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_HARI32_00_AOI; 				/* [���F�j�e] */	/*hari_no_iro*/
			br.BULLET_REGIST_n_way				= (1);									/* [1way] */
			br.BULLET_REGIST_speed_offset		= (0);/*�Ă���*/
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
//
	int ii65536;
	for (ii65536=(adj_hari65536/*0*/); ii65536<(adj_hari65536+65536-(20*64)); ii65536+=(65536/5) )
	{
		int jj1024;
		int kk65536;
		kk65536 = (0);
		for (jj1024 = (0); jj1024<(1024/2); jj1024 += (32) )
		{
			obj_send1->x256 					= (src__x256);							/* �E�ړ����͉E�����猂�� */
			obj_send1->y256 					= (src->y256);							/* �e��y256 */
			br.BULLET_REGIST_speed256			= (int)(speed256 + sin1024((jj1024)));	/* �e�� */
			br.BULLET_REGIST_angle65536 		= ((ii65536)+(kk65536));				/* �p�x */
		//	bullet_regist_vector();
			bullet_regist_angle();/* bullet_regist_vector(); ���኱�V���v���ȈׁA�����Ɍ݊����Ȃ��B */
			kk65536 += ((65536/16)/5);
		}
	}
}

/*---------------------------------------------------------
	�����A�ԂĂ��ƒe��
	-------------------------------------------------------
	�����\�ł�4�����o���}�L���ۂ��̂������Ă��邪�A
	�A�����W����5�����B�ԉf�˂��ۂ��z�ɁB
	-------------------------------------------------------
	�e�L�g�[
-dd 1 0000 0x10 == (xxx)&0x1f
-11 1 0000 luna
-10 1 0000 luna hard
-01 1 0000 luna hard norm
-00 1 0000 luna hard norm easy
---------------------------------------------------------*/
static void danmaku_create_0c_hana_test(SPRITE *src)
{
//	if (0x40==((src->boss_base_danmaku_time_out)&0xcf))/* 4�� */
//	if (0x10==((src->boss_base_danmaku_time_out)&0x1f))/* (16���1��)(128�Ȃ�v8��) */
	if (0x10==((src->boss_base_danmaku_time_out)&0x1f))/* (16���1��)(128�Ȃ�v4��) */
	{
		if (difficulty >= ((src->boss_base_danmaku_time_out)>>5))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		//	bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			bullet_play_04_auto(VOICE11_BOSS_KIRARIN);
		//	voice_play(VOICE11_BOSS_KIRARIN, TRACK04_TEKIDAN);/*�e�L�g�[*/
			#endif
		//
			mima_boss01_nway_fire(src);/*enemy_boss04_fire(2);*/
		}
	}
}

/*---------------------------------------------------------
	�����A�ڋߒe��
	-------------------------------------------------------
	�����ŉ������˂�4�����A�d�͒e���ۂ��z(�\��)
---------------------------------------------------------*/
static void danmaku_create_0d_mima_sekkin(SPRITE *src)
{
	static int src_shot_angle1024;	/* �J�n�n�_ */
//	if (0x40==((src->boss_base_danmaku_time_out)&0xcf))/* 4�� */
//	if (0x10==((src->boss_base_danmaku_time_out)&0x1f))/* (32���1��)(128�Ȃ�v ��) */
	if (0x04==((src->boss_base_danmaku_time_out)&0x07))/* (8���1��) */
	{
	//	if (difficulty >= ((src->boss_base_danmaku_time_out)>>5))
		{
			#if (1)
		//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		//	bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
			bullet_play_04_auto(VOICE11_BOSS_KIRARIN);
		//	voice_play(VOICE11_BOSS_KIRARIN, TRACK04_TEKIDAN);/*�e�L�g�[*/
			#endif
		//
		//	mima_boss01_nway_fire(src);/*enemy_boss04_fire(2);*/
		{
			src_shot_angle1024 -= (16); 		/* cv1024r(10)*/
//
				obj_send1->x256 						= (src->x256);
				obj_send1->y256 						= (src->y256);
				br.BULLET_REGIST_speed256				= t256(1.0);//t256(1+difficulty)/*(3+difficulty)*/ /*(4+difficulty)*/;
//				br.BULLET_REGIST_angle1024				= (src->shot_angle1024&(256-1))+512+128;// /*deg512_2rad*/( (doll_data->br_angle512&(256-1) )+deg_360_to_512(90) );
				br.BULLET_REGIST_angle1024				= (src_shot_angle1024&(256-1))-512-128;// /*deg512_2rad*/( (doll_data->br_angle512&(256-1) )+deg_360_to_512(90) );
			//	br.BULLET_REGIST_jyuryoku_dan_delta256	= ((ra_nd()&0x03)+1);//t256(0.04)/*10*/
				br.BULLET_REGIST_jyuryoku_dan_delta256	= ((ra_nd()&0x03)+2);//t256(0.04)
				br.BULLET_REGIST_bullet_obj_type		= (BULLET_MARU8_00_AKA+(7));	/* �e�O�� */
				br.BULLET_REGIST_regist_type			= REGIST_TYPE_02_GRAVITY02;
				bullet_regist_vector();
		}
		}
	}
}
