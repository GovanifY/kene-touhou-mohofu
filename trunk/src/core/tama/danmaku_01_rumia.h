
/*---------------------------------------------------------
 �����͕핗  �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���[�~�A�̒e���𐶐����܂��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�g1�ʃ{�X ���[�~�A �镄�u�i�C�g�o�[�h�v�ɂ�����Ƃ��������e��(�\��)
	-------------------------------------------------------
	5�e�ڂ����@�_���� 32����16���� �|��e�B
	-------------------------------------------------------
0000 0000 0x00==(time_out)&0x11 ��(0x00==((time_out)&0x20))
0001 0000						��(0x00==((time_out)&0x20))
0010 0000 0x00==(time_out)&0x11 ��
0011 0000						��
0100 0000 0x00==(time_out)&0x11 ��(0x00==((time_out)&0x20))
0101 0000						��(0x00==((time_out)&0x20))
0110 0000 0x00==(time_out)&0x11 ��
0111 0000						��
//
1000 0000 start
---------------------------------------------------------*/
static void danmaku_create_08_night_bird(SPRITE *src)
{
	static int aaa_angle65536;
//	const int src_boss_base_danmaku_time_out = src->boss_base_danmaku_time_out;
//	if (128==((src->boss_base_danmaku_time_out)))
	if (0x10==((src->boss_base_danmaku_time_out)&0x10))
	{
		#if 1
		tmp_angleCCW65536_jikinerai(obj_player, src);/*���@�˂炢�p�쐬*/
		aaa_angle65536 = src->tmp_angleCCW65536;/*���@�˂炢�p*/
		#else
		aaa_angle65536 = (0);/* �e�X�g������ */
		#endif
	//	aaa_angle65536 += (const int)(65536-(int)((65536*5)/(32)));/* 5�e�ڂ����@�_�� */
		aaa_angle65536 += (55296);/* 5�e�ڂ����@�_��(65536-(5*(65536/32))) */
		mask65536(aaa_angle65536);
		/* �p�x(65536[360/360�x]�� 32����) */
	}
	else
	if (0x00==((src->boss_base_danmaku_time_out)&0x10))/* 16�� */
	{
		bullet_play_04_auto(VOICE13_SAKUYA_SHOT02/*VOICE14_BOSS_KOUGEKI_01*/);
		obj_send1->y256 						= (src->y256);		/* �e��y256 */		/* ���e�ʒu�� offset �p */
		{
			int tama_color;/* �e�F */
			int bbb;/* ���� */
			int bbb_angle65536;/* ���� */
			int iii;
				iii = ((src->boss_base_danmaku_time_out)&0x0f);
			if (0x00==((src->boss_base_danmaku_time_out)&0x20))
					{	bbb=-1;tama_color=0;/* �g���e */bbb_angle65536=65536-aaa_angle65536;}
			else	{	bbb= 1;tama_color=1;/* �g���Βe */bbb_angle65536=aaa_angle65536;}
			mask65536(bbb_angle65536);
		//	obj_send1->x256 = src->x256+((16-(iii))<<(3+8))*bbb;/* �{���͔��e�ʒu�� x offset ���� */
			obj_send1->x256 = src->x256+((16-(iii))<<(3+7))*bbb;/* �{���͔��e�ʒu�� x offset ���� */
			br.BULLET_REGIST_speed256			= (t256(1.5));								/* �e�� */
			br.BULLET_REGIST_angle65536 		= (bbb_angle65536); /* ���˒��S�p�x / ����@�\(���@�_��/��) */
	//		br.BULLET_REGIST_div_angle1024		= (0);										/* �_�~�[�����p�x(���g�p) */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_MARU12_02_AOI+tama_color;			/* [�g���e] [�g���Βe] */ 	/*BULLET_MARU12_03_AOI*/
			br.BULLET_REGIST_n_way				= (1);										/* [1way] */
			br.BULLET_REGIST_speed_offset		= (0);/*�Ă���*/
		//	br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;/*���ݎ�ނ�����*/
			bullet_regist_angle();/* bullet_regist_vector(); ���኱�V���v���ȈׁA�����Ɍ݊����Ȃ��B */
		}
		/* ���̒e�̊p�x */
		aaa_angle65536 += (int)(65536/(32));/* �p�x(65536[360/360�x]�� 32����) */
		mask65536(aaa_angle65536);
	}
}
