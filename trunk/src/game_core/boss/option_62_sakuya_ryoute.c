
/*-------------------------------------------------------
	s->cx256	�i�C�t��x���W
	data->cx256 �m�[�h��x���W
	s->w/2		�i�C�t�̉����̔���
	�i�C�t��x���W == �m�[�h��x���W + co_s1024((data->angle1024)) x length - �i�C�t�̉����̔���
	#define ryoute_knife_length256 t256(128)
	��angle �� �C��/(2�΁~ryoute_knife_length/��2)
---------------------------------------------------------*/

#include "boss.h"//#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	��� ����i�C�t�J�[�h
	-------------------------------------------------------
	(r35)�i�C�t�̐�΍��W�ʒu����x�ŋ��߂�l��������A
	���΍��W(�i�C�t�̈ړ��x�N�g���ʁA���݈ʒu����̍���)�����߂�l�����Ɉڍs���A
	�v���O�������V���v���ɏC���B
	-------------------------------------------------------
	�g�p���W�X�^
//	REG_08_REG0 	�J�E���^�B
	REG_09_REG1 	���e�p�J�E���^�B(���E�ŋ��p)
//	REG_0a_REG2 	aaa
//	REG_0b_REG3 	bbb
	-------------------------------------------------------
	�{�X���ʋK�i�g�p�f�[�^�[:
---------------------------------------------------------*/

/*---------------------------------------------------------
	����i�C�t�e(��)
	-------------------------------------------------------
	length	teki-obj_player�Ԃ̋���/��2
	r_or_l	0==�E	1==��
	-------------------------------------------------------
	ToDo:
	�e�V�X�e���Ɉڍs����ׁA�p�~�\��B
	�J�[�h�X�N���v�g�V�X�e�����ғ�����ׂɂ́A���e���ꃖ���ɓ������Ȃ���
	���x�I�Ȗʂł��̂܂܂̌`�Ԃł͖���������̂ŁA
	(�����̔��e�����̂����ő��̒e��������������)
	�����̔��e������ hatudan_system_regist_single �ɒu��������K�v������B
---------------------------------------------------------*/


// �o�O�΍�B user_data02 �ǂ����J���i�C���ǋ󂯂Ƃ��B()
	/*-r34���d�l����*/ /* r(-1)==+128, l(+1)==-128 */	/* -1==l, +1==r */
	/*(==TAMA_SPEC_AKA_AO_KUNAI_BIT; tama_system�� HATSUDAN_04_tama_spec �Ɉڍs�\��)*/
	#define RYOUTE_KNIFE_DATA_l_or_r		user_data03


#define KNIFE_TYPE_RIGHT (0)

/*---------------------------------------------------------
	[�e���O���[�v(1)�Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/

static void sakuya_ryoute_knife_danmaku_01_callback(SPRITE *src)
{
	/*(�i�C�t���i�s���鎞�ɋȂ���p�x[��])*/
	{
		int aaa;
		aaa = (1);
	//	if (KNIFE_TYPE_RIGHT!=src->RYOUTE_KNIFE_DATA_l_or_r)
		if (KNIFE_TYPE_RIGHT==src->RYOUTE_KNIFE_DATA_l_or_r)
		{
			aaa = -aaa;
		}
		/* �`��p�p�x(����0�x�ō����(�����v���)) */
		src->rotationCCW1024 += (aaa);			/*fps_factor*/	/* R(0)== +2, L(1)== -2 */		/*(r35-)*/
	}
//�s�v	mask1024(src->rotationCCW1024);//(sincos�̏ꍇ�s�v)
	mask1024(src->rotationCCW1024);/*�K���v��*/
	/*(�i�C�t�̑��Έړ��ʁB�x�N�g���B)*/
	#if (0)//
	src->cx256		+= ((si n1024((src->rotationCCW1024)))<<1);/*fps_factor*/	/* CCW�̏ꍇ */
	src->cy256		+= ((co s1024((src->rotationCCW1024)))<<1);/*fps_factor*/
	#else
	{
		int sin_value_t256; 	//	sin_value_t256 = 0;
		int cos_value_t256; 	//	cos_value_t256 = 0;
		int256_sincos1024( (src->rotationCCW1024), &sin_value_t256, &cos_value_t256);
		/*(�i�C�t�̐i�s�ړ���)*/
	//	src->cx256 += ((sin_value_t256)<<1);/*fps_factor*/
	//	src->cy256 += ((cos_value_t256)<<1);/*fps_factor*/
		src->cx256 += ((sin_value_t256) );/*fps_factor*/
		src->cy256 += ((cos_value_t256) );/*fps_factor*/
	}
	#endif
	/*(�q�i�C�t�̔��e�Ԋu)*/
	/*(���̃J�E���^�͍��E�ŋ��p�Ȃ̂�2�{�i��)*/
	REG_09_REG1++;
	if (((10)+(10)) < REG_09_REG1)
	{
		REG_09_REG1 = (0);
		/* �e�i�C�t(��)���A1�񕪂̎q�i�C�t�e(��)��4�e�A���B */
		/*(�q�i�C�t�e��4�e�A����)*/
		set_REG_DEST_XY(src);	/* �q�i�C�t�e�͐e�i�C�t(��)�e�̈ʒu���甭�e�B */
		//
		HATSUDAN_02_speed_offset		= t256(0);/*(�e�X�g)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_00)|(TAMA_SPEC_3000_EFFECT_NONE)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
		HATSUDAN_05_bullet_obj_type 	= BULLET_KNIFE20_04_AOI;	/* [�i�C�t�e] */
		{
			int kk_speed256;
			kk_speed256 = (t256(1.6));
			unsigned int jj;
			for (jj=0; jj<4; jj++)/*(1���4�e�A����)*/
			{
				kk_speed256 += (t256(0.2));
				HATSUDAN_01_speed256		= (kk_speed256);
				const /*int*/ /*u16*/s32/*(s16�ŉ\)*/ jj_bbb[4] =
				{
					(int)((65536*6/12)-(65536*6/12)),	//	kk_speed256 = (t256(1.8));
					(int)((65536*6/12)-(65536*5/12)),	//	kk_speed256 = (t256(2.0));
					(int)((65536*6/12)-(65536*4/12)),	//	kk_speed256 = (t256(2.2));
					(int)((65536*6/12)-(65536*3/12)),	//	kk_speed256 = (t256(2.4));
				};
				#if (1)/*r35-*/
				{	int aaa 	= (((int)(jj_bbb[jj]) ));
					/*(�����͖{�������Ŏ��ׂ������A�Ƃ肠���������Ŏ����Ă���B�����ō��E�̔��f���t�ɂ���)*/
					if (KNIFE_TYPE_RIGHT!=src->RYOUTE_KNIFE_DATA_l_or_r)	{	aaa = -aaa; 	}
				//	HATSUDAN_03_angle65536 = (aaa) + (deg1024to65536(src->rotationCCW1024)); /* �u1����1024�����v����u1����65536�����v�֕ϊ�����B */
					HATSUDAN_03_angle65536 = (aaa) + (deg1024to65536(src->rotationCCW1024)); /* �u1����1024�����v����u1����65536�����v�֕ϊ�����B */
				}
				#endif
				hatudan_system_regist_single();
			}
		}
	}
}
/*---------------------------------------------------------
	[�������Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/
#if (0)/*(�e���V�X�e���Ɉڍs����ꍇ)*/
local void bullet_init_sakuya_ryoute_knife(SPRITE *src)
{
//	card.danmaku_callback[0] = danmaku_00_standard_angle_mover;/*(�ʏ�e�p)*/
	card.danmaku_callback[1] = sakuya_ryoute_knife_danmaku_01_callback;/*(�e�i�C�t�B)*/
//	card.danmaku_callback[2] = NULL;/*(���g�p)*/
//	card.danmaku_callback[3] = NULL;/*(���g�p)*/
}
#endif
/*---------------------------------------------------------
	[���e�Z�N�V����]
	-------------------------------------------------------
---------------------------------------------------------*/
global void bullet_create_sakuya_ryoute_knife(SPRITE *src)
{
	REG_09_REG1 	= (0);/*(���E���p���e�J�E���^)*/
	//
	/* ���@�_���p�� HATSUDAN_03_angle65536 �ɍ쐬 */
//	REG_02_DEST_X	= (src->cx256);/*(���̌��݈ʒu���W)*/
//	REG_03_DEST_Y	= (src->cy256);/*(���̌��݈ʒu���W)*/
	set_REG_DEST_XY(src);	/* �i�C�t(��)�e�͍��̈ʒu���甭�e�B */
	calculate_jikinerai();
	/* �u1����65536�����v����u1����1024�����v�֕ϊ�����B */
	HATSUDAN_03_angle1024 = (deg65536to1024(HATSUDAN_03_angle65536));
	//
	#if (0)
	/*(�e���V�X�e���Ɉڍs����ꍇ)*/
//	set_REG_DEST_XY(src);	/* �i�C�t(��)�e�͍��̈ʒu���甭�e�B */	/*(���Ɏ��@�_���̎��ɐݒ�ς�)*/
	HATSUDAN_01_speed256			= (0);				/* �e��(0) */
	HATSUDAN_02_speed_offset		= t256(0);/*(�Ă���)*/
	HATSUDAN_05_bullet_obj_type 	= (BULLET_KNIFE20_06_YUKARI);	/* �i�C�t�e(��) */
	#endif
	int r_or_l;
	for (r_or_l=0; r_or_l<2; r_or_l++)/*r35-*/
	{
		SPRITE *h;
		h					= obj_add_00_tama_error();/*�S�����i�C�t(��)*/
		if (NULL != h)
		{
			#if (1)
			h->cx256			= (src->cx256); 	/* �i�C�t(��)�e�͍��̈ʒu���甭�e�B */
			h->cy256			= (src->cy256); 	/* �i�C�t(��)�e�͍��̈ʒu���甭�e�B */
		//	h->type 			= BULLET_KNIFE20_04_AOI;/* �i�C�t�e(��) */
			h->type 			= BULLET_KNIFE20_06_YUKARI;/* �i�C�t�e(��) */
			reflect_sprite_spec444(h, OBJ_BANK_SIZE_00_TAMA);
			h->callback_mover	= sakuya_ryoute_knife_danmaku_01_callback;
			h->m_Hit256R		= TAMA_ATARI_KNIFE18_PNG;
			#else
			/*(�e���V�X�e���Ɉڍs����ꍇ)*/
			#endif
		//
			#if (1)/*(���ʏ���)*/
			/*(�i�C�t�̍��E�ʃt���O������)*/
			h->RYOUTE_KNIFE_DATA_l_or_r 		= r_or_l;
			/*(�i�C�t�̐i�s����������)*/
			{
				int tmp1024;
				tmp1024 = (128);
				if (KNIFE_TYPE_RIGHT!=r_or_l)	{tmp1024 = -tmp1024;	}
				tmp1024 += (HATSUDAN_03_angle1024); 	/* �u1����1024�����v */
			//	mask1024(tmp1024);
				#if (1)
				h->rotationCCW1024	= (tmp1024);	/*(�i�C�t�̐i�s�����͕\�������Ɠ����Ȃ̂ŁA�\�������ŕێ�����)*/
				#else
				/*(�e���V�X�e���Ɉڍs����ꍇ)*/
				HATSUDAN_03_angle65536			= (tmp1024);
				#endif
			}
			/*(�i�C�t�̔��e���W������)*/
			#if (0)//
			h->cx256	= (h->cx256) + ((si n1024((h->rotationCCW1024)))<<7);/*fps_factor*/ /* CCW�̏ꍇ */
			h->cy256	= (h->cy256) + ((co s1024((h->rotationCCW1024)))<<7);/*fps_factor*/
			#else
			{
				int sin_value_t256; 		//	sin_value_t256 = 0;
				int cos_value_t256; 		//	cos_value_t256 = 0;
				int256_sincos1024( (h->rotationCCW1024), &sin_value_t256, &cos_value_t256);
		//		h->cx256		= (h->cx256) + ((sin_value_t256)<<7);/*fps_factor*/
		//		h->cy256		= (h->cy256) + ((cos_value_t256)<<7);/*fps_factor*/
				h->cx256		= (h->cx256) + ((sin_value_t256)<<3);/*fps_factor*/
				h->cy256		= (h->cy256) + ((cos_value_t256)<<3);/*fps_factor*/
			}
			#endif
			/*(�i�C�t�̎���������)*/
			h->jyumyou						= (128+(REG_0f_GAME_DIFFICULTY<<6));/*(r33)*/	/*�Ӗ�����(?)*/
			#endif
		}
		#if (0)
		/*(�e���V�X�e���Ɉڍs����ꍇ)*/
		HATSUDAN_04_tama_spec			= (DANMAKU_LAYER_01)|(r_or_l<<8)|(TAMA_SPEC_0000_TILT);/* (r33-)�W���e */
		hatudan_system_regist_single();
		#endif
	}
}
