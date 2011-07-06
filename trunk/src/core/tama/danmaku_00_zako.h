
/*---------------------------------------------------------
 �����͕핗  �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�G���̒e���𐶐����܂��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�A���X(��)	����@�e��
	-------------------------------------------------------
	�e�L�g�[
---------------------------------------------------------*/
static void danmaku_create_09_zako_sentakki(SPRITE *src)
{
	if (0==((src->boss_base_danmaku_time_out)&0x03))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		src->tmp_angleCCW1024 -= (int)(1024/24);
		mask1024(src->tmp_angleCCW1024);
	//
				/* ���S���W�Ȃ̂ŁA�I�t�Z�b�g�Ȃ�==�{�X���S����e�o���B */
			obj_send1->cx256 						= (src->cx256);							/* �e��x256 */
			obj_send1->cy256 						= (src->cy256);							/* �e��y256 */
		#if 0
			br.BULLET_REGIST_div_angle1024			= (int)(1024/24);						/* �����p�x */
			br.BULLET_REGIST_n_way					= (8);									/* [8way] */
			br.BULLET_REGIST_regist_type			= REGIST_TYPE_00_MULTI_VECTOR;
	//
			br.BULLET_REGIST_speed256				= (t256(2.0)+((src->boss_base_danmaku_time_out)<<2));	/* �e�� */
		{
			br.BULLET_REGIST_angle1024				= (src->tmp_angleCCW1024);								/* �p�x */
			br.BULLET_REGIST_bullet_obj_type		= BULLET_UROKO14_00_AOI;				/* [�ؒe] */
			bul let_reg ist_vec tor();
		//
			br.BULLET_REGIST_angle1024				= ((-src->tmp_angleCCW1024)&(1024-1));					/* �p�x */
			br.BULLET_REGIST_bullet_obj_type		= BULLET_UROKO14_04_MIZUIRO;			/* [���ؒe] */
			bul let_reg ist_vec tor();
		}
		#endif
		#if 1
			br.BULLET_REGIST_div_angle65536 		= (int)(65536/24);						/* �����p�x */
			br.BULLET_REGIST_n_way					= (8);									/* [8way] */
		//	br.BULLET_REGIST_regist_type			= REGIST_TYPE_00_MULTI_VECTOR;		/* ���݂��ꂵ���Ȃ����v�� */
			br.BULLET_REGIST_speed_offset			= t256(1);		/* ���������e */	/* ���̕����ɂȂ邩������ */
	//
			br.BULLET_REGIST_speed256			= (t256(1.0)+((src->boss_base_danmaku_time_out)<<2));	/* �e�� */
		{
			int first_angle65536 = ((src->tmp_angleCCW1024)<<6);
			br.BULLET_REGIST_angle65536 		= (first_angle65536);								/* �p�x */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_00_AOI;				/* [�ؒe] */
			bullet_regist_angle();	/* �p�x�e�Ƃ��ēo�^ */
		//
			br.BULLET_REGIST_angle65536 		= ((-first_angle65536)&(65536-1));					/* �p�x */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_04_MIZUIRO;			/* [���ؒe] */
			bullet_regist_angle();	/* �p�x�e�Ƃ��ēo�^ */
		}
		#endif
	}
}


