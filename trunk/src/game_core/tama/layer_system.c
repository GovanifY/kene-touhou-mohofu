
#include "game_main.h"

/*---------------------------------------------------------
  �����͕핗 �` Toho Imitation Style.
  �v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���C���[�V�X�e��
	-------------------------------------------------------
	���C���[�V�X�e���́u�e���v�̕����ʂ�A
	�e��P�̂Ƃ��Ă͔F�������A���Ƃ��ĔF������V�X�e���B
	-------------------------------------------------------
---------------------------------------------------------*/

/*---------------------------------------------------------
	�e���̗L���͈́B�L���͈͊O�͉�ʊO�Ɣ��f���A�A�N�V�������N����B
	�A�N�V�����̎�ނ́A
		�e�����������́B
		�e�����˂�����́B
	��������B
---------------------------------------------------------*/


//	/*extern*/global POINT256 bullet_clip_min;		/*	�͈�(�ŏ��l) */
//	/*extern*/global POINT256 bullet_clip_max;		/*	�͈�(�ő�l) */

#if 0
/*---------------------------------------------------------
	�e�����C���[�A�N�V����#00
	-------------------------------------------------------
	�e���̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)�B
	�e������ʊO�̏ꍇ�́A�e�������B
---------------------------------------------------------*/

global void danmaku_action_00_gamen_gai_nara_tama_wo_kesu(void)
{
	unsigned int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++)/* �S�e���ׂ�B */
	{
		SPRITE *obj;
		obj = &obj99[OBJ_HEAD_00_TAMA+ii];
		if (0!=(obj->flags&SP_FLAG_COLISION_CHECK))/* �����蔻��̖����e�͏����Ȃ��B(���e�G�t�F�N�g�p) */
		{
			/* ��ʊO�̏ꍇ�͒e�������B */
			if (
			(0 < (rect_clip.bullet_clip_min.x256)-(obj->cx256) ) ||
			(0 > (rect_clip.bullet_clip_max.x256)-(obj->cx256) ) ||
			(0 < (rect_clip.bullet_clip_min.y256)-(obj->cy256) ) ||
			(0 > (rect_clip.bullet_clip_max.y256)-(obj->cy256) ) )
			{
				obj->jyumyou = JYUMYOU_NASI;
			}
		}
	}
}
#endif

#if 0
/*---------------------------------------------------------
	�e���A�N�V����#01
	-------------------------------------------------------
	�e�̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)�B
	�e���̍��E����ʊO�̏ꍇ�͔��˂���B
	�e���̏㉺����ʊO�̏ꍇ�͒e�������B
---------------------------------------------------------*/

global void danmaku_action_01_gamen_gai_nara_hansya_nomi(void)
{
	unsigned int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++)/* �S�e���ׂ�B */
	{
		SPRITE *obj;
		obj = &obj99[OBJ_HEAD_00_TAMA+ii];
		if (0!=(obj->flags&SP_FLAG_COLISION_CHECK))/* �����蔻��̖����e�͏����Ȃ��B(���e�G�t�F�N�g�p) */
		{
			#if 1/* ������ */
			int test_flag;
			test_flag = 0;
			if (
				(obj->cx256 < t256(GAME_X_OFFSET)/*rect_clip.bullet_clip_min.x256*/) )
			{
				if (0==(obj->hatudan_register_spec_data & TAMA_SPEC_KABE_SAYUU_HANSYA_BIT))
						{	test_flag = 1;			}
				else	{	obj->jyumyou = JYUMYOU_NASI;	}
			}
			else
			if (
				(obj->cx256 > t256(GAME_WIDTH)/*rect_clip.bullet_clip_max.x256*/) )
			{
				if (0==(obj->hatudan_register_spec_data & TAMA_SPEC_KABE_SAYUU_HANSYA_BIT))
						{	test_flag = 1;			}
				else	{	obj->jyumyou = JYUMYOU_NASI;	}
			}
			if (0 != test_flag)
			{
					obj->hatudan_register_spec_data |= TAMA_SPEC_KABE_SAYUU_HANSYA_BIT;
				//	obj->rotationCCW1024 += (1024/4);/* 90�x��] */ 		/* �_��[��2] */
				//	obj->rotationCCW1024 += (1024*3/4);/* -90�x��] */		/* �_��[��2] */
					obj->rotationCCW1024 = (1024)-(obj->rotationCCW1024);	/* ���][��1] */
					mask1024(obj->rotationCCW1024);
			}
			if (
				(obj->cy256 < - t256(100)/*rect_clip.bullet_clip_min.y256*/)||
				(obj->cy256 >	t256(272)/*rect_clip.bullet_clip_max.y256*/)
			)
			{
				obj->jyumyou = JYUMYOU_NASI;
			}
			#endif
		}
	}
}

#endif




#if 0
/*---------------------------------------------------------
	�e���A�N�V����#02
	-------------------------------------------------------
	�e���̋�ݏo���`�F�b�N���s��(���t���[���s���K�v�͂Ȃ�)�B
	�e���̍��E����ʊO�̏ꍇ�͔��ˌ����B
	�e���̏㉺����ʊO�̏ꍇ�͒e�������B
---------------------------------------------------------*/

global void danmaku_action_02_gamen_gai_nara_hansya_gensoku(void)
{
	unsigned int ii;
	for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++)/* �S�e���ׂ�B */
	{
		SPRITE *obj;
		obj = &obj99[OBJ_HEAD_00_TAMA+ii];
		if (0!=(obj->flags&SP_FLAG_COLISION_CHECK))/* �����蔻��̖����e�͏����Ȃ��B(���e�G�t�F�N�g�p) */
		{
			{
				/* [���̕ǂŔ��˂���] �e���e���ݒ�̈��ʂ�菭��(4[dot])��O�̈ʒu�ɗ����ꍇ�B */
				if (0 < (rect_clip.bullet_clip_min.x256) + t256(4) - (obj->cx256) )
				{
					goto common_hansya;
				}
				else
				/* [�E�̕ǂŔ��˂���] �e���e���ݒ�̈��ʂ�菭��(4[dot])��O�̈ʒu�ɗ����ꍇ�B */
				if (0 > (rect_clip.bullet_clip_max.x256) - t256(4) - (obj->cx256) )
				{
					goto common_hansya;
				}
				goto done_hansya;/*(���˂��Ȃ�)*/
			common_hansya:/*(���˂���\������)*/
				/* ���ɔ��˂��Ă���ꍇ�͔��˂��Ȃ��ŁA�e�������B */
				if (0!=(obj->hatudan_register_spec_data & TAMA_SPEC_KABE_SAYUU_HANSYA_BIT))
				{	/*(���ˏ������s��Ȃ�)*/
					obj->jyumyou = JYUMYOU_NASI;/*(�e������)*/
				}
				else
			//	if (JYUMYOU_NASI != obj->jyumyou)
				{	/*(���ˏ������s��)*/
					obj->hatudan_register_spec_data |= TAMA_SPEC_KABE_SAYUU_HANSYA_BIT;
				//	obj->rotationCCW1024 += (1024*3/4);/* -90�x��] */		/* �_��[��2] */
				//	obj->rotationCCW1024 += (1024/4);/* 90�x��] */ 		/* �_��[��2] */
					obj->rotationCCW1024 = (1024)-(obj->rotationCCW1024);	/* ���][��1] */
				//	obj->hatudan_system_speed256					= ((obj->hatudan_system_speed256)>>1);/*����*/
					obj->hatudan_register_speed65536				= ((obj->hatudan_register_speed65536)>>(1));/*����*/
					/*(�������Ă��A��������ꍇ�̃��~�b�g)*/
					#define mmm_MAX_SPEED_65536 	(t256(1.0)<<8)
				//	if (mmm_MAX_SPEED_65536 < (obj->hatudan_register_speed65536))
				//	{
				//		obj->hatudan_register_speed65536 = mmm_MAX_SPEED_65536;
				//	}
					obj->hatudan_register_speed65536 = psp_min(obj->hatudan_register_speed65536, mmm_MAX_SPEED_65536);
					mask1024(obj->rotationCCW1024);
				}
			done_hansya:/*(���ˏ����I���)*/
				;
			}
			/* �㉺�̏ꍇ�͒e�������B */
			if (
				(0 < (rect_clip.bullet_clip_min.y256) - (obj->cy256) )||
				(0 > (rect_clip.bullet_clip_max.y256) - (obj->cy256) )
			)
			{
				obj->jyumyou = JYUMYOU_NASI;
			}
		}
	}
}
#endif

/*---------------------------------------------------------
	�e���R�[���o�b�N�V�X�e��
	-------------------------------------------------------
	���e���ɒe���R���g���[��(�R�[���o�b�N)��o�^���Ă����A
	����𑀍삷��B
---------------------------------------------------------*/

global void danmaku_system_callback(void)
{
	unsigned int jj;
	for (jj=0; jj<DANMAKU_LAYER_04_MAX; jj++)/*(�o�^��S�����ׂ�B)*/
	{
		/*(�o�^����Ă��Ȃ��ꍇ�͉������Ȃ��B)*/
		if (NULL != card.danmaku_callback[jj])
		{
			unsigned int ii;
			for (ii=0; ii<OBJ_POOL_00_TAMA_MAX; ii++)/*(�e��S�e���ׂ�B)*/
			{
				SPRITE *obj;
				obj = &obj99[OBJ_HEAD_00_TAMA+ii];
				if (0!=(obj->flags&SP_FLAG_COLISION_CHECK))/*(�����蔻��̖����e�͑ΏۊO�B���e�G�t�F�N�g�p�B)*/
				{
					if (jj == ((obj->hatudan_register_spec_data)&(0x03)) )	/* �e��[n]�Ȃ� */
					{
						(card.danmaku_callback[jj])(obj);
					}
				}
			}
		}
	}
}
