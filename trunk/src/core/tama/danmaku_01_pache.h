
/*---------------------------------------------------------
 �����͕핗  �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�p�`�����[�E�m�[���b�W�̒e���𐶐����܂��B
---------------------------------------------------------*/

extern void add_zako_pache_laser(SPRITE *src);/* �A���X�l�`�e�� */
static void danmaku_create_27_pache_laser2(SPRITE *src)
{
	/*0x280*/if ((640-1)==((src->boss_base_danmaku_time_out) ))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		add_zako_pache_laser(src);
	}
	/*0x200*/if ((640-128)==((src->boss_base_danmaku_time_out) ))
	{
		#if (1)
	//	voice_play(VOICE14_BOSS_KOUGEKI_01, TRACK04_TEKIDAN);
		bullet_play_04_auto(VOICE14_BOSS_KOUGEKI_01);
		#endif
	//
		add_zako_pache_laser(src);
	}
}
