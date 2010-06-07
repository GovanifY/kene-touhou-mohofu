
/*---------------------------------------------------------
 �����͕핗  �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�A���X�̒e���𐶐����܂��B
---------------------------------------------------------*/

/*---------------------------------------------------------
	�A���X(��)	����@�e��
	-------------------------------------------------------
	�e�L�g�[
---------------------------------------------------------*/
static void danmaku_create_09_alice_sentakki(SPRITE *src)
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
			obj_send1->x256 					= (src->x256);							/* �e��x256 */
			obj_send1->y256 					= (src->y256);							/* �e��y256 */
			br.BULLET_REGIST_div_angle1024		= (int)(1024/24);						/* �����p�x */
			br.BULLET_REGIST_n_way				= (8);									/* [8way] */
			br.BULLET_REGIST_regist_type		= REGIST_TYPE_00_MULTI_VECTOR;
	//
		{
			br.BULLET_REGIST_speed256			= (t256(2.0)+((src->boss_base_danmaku_time_out)<<2));	/* �e�� */
			br.BULLET_REGIST_angle1024			= (src->tmp_angleCCW1024);								/* �p�x */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_00_AOI;				/* [�ؒe] */
			bullet_regist_vector();
		//
			br.BULLET_REGIST_speed256			= (t256(2.0)+((src->boss_base_danmaku_time_out)<<2));	/* �e�� */
			br.BULLET_REGIST_angle1024			= ((-src->tmp_angleCCW1024)&(1024-1));					/* �p�x */
			br.BULLET_REGIST_bullet_obj_type	= BULLET_UROKO14_04_MIZUIRO;			/* [���ؒe] */
			bullet_regist_vector();
		}
	}
}

/*---------------------------------------------------------
	�A���X(��)	�A���X�l�`�e��
	-------------------------------------------------------
	�e�L�g�[
---------------------------------------------------------*/
extern void add_zako_alice_doll(SPRITE *src);/* �A���X�l�`�e�� */
static void danmaku_create_0b_alice_doll(SPRITE *src)
{
	if (50==((src->boss_base_danmaku_time_out) ))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		add_zako_alice_doll(src);
	}
}
