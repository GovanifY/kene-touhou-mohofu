
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	�v���W�F�N�g�y�[�W http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�w�i�R���g���[��
--------------------------------------------------------- */

#define USE_BG_LOAD 0




static int request_bg0_y_scroll_speed256;	/* bg0�̃X�N���[���A�\�񑬓x */


/*---------------------------------------------------------

--------------------------------------------------------- */


static int current_bg_alpha;
static int request_bg_alpha;

global void set_bg_alpha(int set_bg_alpha)
{
	request_bg_alpha = set_bg_alpha;
}
/*---------------------------------------------------------

--------------------------------------------------------- */

//static void tile_work(void)
global void bg2_move_main(void)
{
	if (current_bg_alpha == request_bg_alpha) /* �ł����肻���ȉ\����r�� */
	{
		;
	}
	else
	{
		if (current_bg_alpha > request_bg_alpha)
		{
			current_bg_alpha -= (4)/*6 */;
		}
		else
		{
			current_bg_alpha += (4)/*6 */;
			if (245 < current_bg_alpha) 	/* ���킶�킷��̂� */
			{	current_bg_alpha = (255);	}
		}
		/* converted */
		u8 aaa = (current_bg_alpha/* >>1 */);
	//	cg.conv_bg_alpha = ((aaa<<24)|(aaa<<16)|(aaa<<8)|(aaa<<0));
		cg.conv_bg_alpha = ((aaa&0xf0));
	}

//	if (current_bg_alpha < 250 )/* 255 */
//			{	current_bg_alpha += 6;		}
//	else	{	current_bg_alpha = 255; 	}
//
	if (cg.current_bg0_y_scroll_speed256 == request_bg0_y_scroll_speed256) /* �ł����肻���ȉ\����r�� */
	{
		;
	}
	else
	if (cg.current_bg0_y_scroll_speed256 > request_bg0_y_scroll_speed256)
	{
		cg.current_bg0_y_scroll_speed256--;
	}
	else
	{
		cg.current_bg0_y_scroll_speed256++;
	}
//


//
	#if 0/* laster_sprit�f�o�b�O�p */
	/* �p�l���̃X�R�A����laster_sprit�� �O���C�Y���ɒǉ�bg������ �\��������B���Ă����������������Ⴄ�B */
	pd_score		= (t256_floor(laster_sprit256));
	pd_graze_point	= number_of_bg;
	#endif
}



/*---------------------------------------------------------
	�X�e�[�W�ǂݍ��݊J�n���ɁA���񏉊�������
--------------------------------------------------------- */
global void bg2_start_stage(void)
{
	request_bg0_y_scroll_speed256 = cg.current_bg0_y_scroll_speed256 = t256(0.5);/* �����l */
}

/*---------------------------------------------------------
	BG�R���g���[���R�}���h(�e�X�g��)
--------------------------------------------------------- */

global void game_command_02_bg_control(GAME_COMMAND *l)
{
	request_bg0_y_scroll_speed256 = (l->user_y);	/* bg0�̃X�N���[���A�\�񑬓x��ݒ� */

}
//		{
//			/* KETM�͖�20-30fps����ł����A���ݖ�60fps����Ȃ̂ł��܂�ɑ�������ׁA���x�l�𔼕��ɏC�����܂����B */
//			#if 1
//			{
//		//	new_entry->scroll_speed256/* ctype */ = speed256_tbl[((new_entry->user_255_code) & 0x0f)];
//			}
//			#endif
//		}
//	enum
//	{
//		BG2_00_ERROR = 0,
//		BG2_01_SET_SCROOL_OFFSET,
//		BG2_02_BG_STOP,
//	//	BG2_03_BG_SWAP,
//		BG2_03_DESTOROY_CLOUDS,
//	};
//	short xxx;
//	short yyy;
//	short speed256;
//	xxx = l->user_x;
//	yyy = l->user_y;
//	speed256 = l->scroll_speed256;
	//

//	sw itch (xxx)
//	{
//	ca se BG2_01_SET_SCROOL_OFFSET: /* �X�N���[���l�𒼐ڃZ�b�g */
//		#if (1==USE_BG_LOAD)
//		bg0_bmp_y256 = yyy;
//		#endif /* (1==USE_BG_LOAD) */
//		break;
//	ca se BG2_02_BG_STOP:	/* �e�X�g�� */
//		#if (1==USE_BG_LOAD)
//		laster_sprit256 = t256(272);
//		sprit_flag = 0; 	/* �����ʒu�𔻒f����K�v���Ȃ� */
//		#endif /* (1==USE_BG_LOAD) */
//		break;
//	ca se BG2_03_BG_SWAP:	/* �e�X�g�� */
//		bg0_bmp_y256 = 0;
//		which_bg++;
//		which_bg &= 1;
//		bg2_swap();
//		break;
//	ca se BG2_03_DESTOROY_CLOUDS:	/* �e�X�g�� */
//		clouds_destroy();
//		break;
//	}

/*---------------------------------------------------------
	psp�N�����Ɉ�x��������������
--------------------------------------------------------- */
//extern void clouds_system_init(void);
//global void bg2_system_init(void)
//{
//	/* �摜�ǂݍ��ݗp�T�[�t�F�C�X */
//	clouds_system_init();
//}
