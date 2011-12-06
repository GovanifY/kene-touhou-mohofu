
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	�w�i�R���g���[��
--------------------------------------------------------- */

#define USE_BG_LOAD 0

static int request_bg0_y_scroll_speed256;	/* bg0�̃X�N���[�����x�A�ݒ�l */

/*---------------------------------------------------------

--------------------------------------------------------- */

static s32 current_bg_alpha;	/* bg0�̖��邳�A���ݒl */
static s32 request_bg_alpha;	/* bg0�̖��邳�A�ݒ�l */

global void set_bg_alpha(int set_bg_alpha)
{
	request_bg_alpha = set_bg_alpha;
}


/*---------------------------------------------------------
	�Q�[�����A�w�i�̈ړ�����
	(�ړ������݂̂ŕ`�揈���͂��Ȃ�)
--------------------------------------------------------- */

global void bg_move_all(void)
{
	/*(�w�i�̖��邳�ύX����)*/
	if (current_bg_alpha == request_bg_alpha) /* �ł����肻���ȉ\����r�� */
	{
		;
	}
	else
	{
		if (current_bg_alpha > request_bg_alpha)
		{
			current_bg_alpha -= (4);/*(6)*/
		}
		else
		{
			current_bg_alpha += (4);/*(6)*/
			current_bg_alpha = psp_min(current_bg_alpha, (255) );	/* ���킶�킷��̂� */
		//	if (245 < current_bg_alpha) 	{	current_bg_alpha = (255);	}	/* ���킶�킷��̂� */
		}
		/* converted */
		u8 aaa = (current_bg_alpha/* >>1 */);
	//	cg.conv_bg_alpha = ((aaa<<24)|(aaa<<16)|(aaa<<8)|(aaa<<0));
		cg.conv_bg_alpha = ((aaa&0xf0));
	}
	//
	/*(�w�i�̃X�N���[�����x�ύX����)*/
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
	#if 0/* laster_sprit�f�o�b�O�p */
	/* �p�l���̃X�R�A����laster_sprit�� �O���C�Y���ɒǉ�bg������ �\��������B���Ă����������������Ⴄ�B */
	pd_score		= (t256_floor(laster_sprit256));
	pd_graze_point	= number_of_bg;
	#endif
}

/*---------------------------------------------------------
	BG�R���g���[���R�}���h(�e�X�g��)
--------------------------------------------------------- */

global void game_command_03_bg_control(GAME_COMMAND *l)
{
	request_bg0_y_scroll_speed256 = (l->user_speed256); 	/* bg0�̃X�N���[���A�\�񑬓x��ݒ� */
}
