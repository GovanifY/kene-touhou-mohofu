
/*---------------------------------------------------------
	�������G�t�F�N�g
	��낤���Ǝv�������ǁA����ĂȂ�
	-------------------------------------------------------
	�Ƃ肠�������Ȃ�B
	-------------------------------------------------------
	�|�[�Y�t�B���^�[�̌���
	�P�D�F
	�g�Ń|�[�Y��ʂ̃X�N�V�������Ɣ������������F���ۂ��Ȃ�B
	�����Ŕ�==RGB(0xff,0xff,0xff)�Ƃ���ƁA
���FR == (��G * 0.863) / 16;
���FG == ((��R * 0.863)*0.863);
���FB == (��B * 0.863);
��������Ȃ��B
	-------------------------------------------------------
 R G B	 R G B
 0 0 1	 0 0 1
 1 0 0	 0 1 0
 1 1 1	 0 1 1

R	  G 	B
253   253	253   == ��
0xfd  0xfd	0xfd

13	  190	221   == ���F
0x0d  0xbe	0xdd

 x 1.158...

0x0f  0xdd 0xff



1.15837104072398190045248868778281 == 256/221
0.86328125% == 221/256

13/16 == 0.8125

---------------------------------------------------------*/

/* ���z�X�N���[��(SDL_00_VIEW_SCREEN)�ɂ������G�t�F�N�g�������� */
void psp_pause_filter(void)
{
	#if 1/* 1==�S��ʂɃG�t�F�N�g��������B 0==SDL�����̂݃G�t�F�N�g��������B */
	gu_save_screen();/* vram����SDL��ʂɋ����R�s�[ */
	#endif
	if (SDL_MUSTLOCK(sdl_screen[SDL_00_VIEW_SCREEN]))	{	SDL_LockSurface(sdl_screen[SDL_00_VIEW_SCREEN]);	}	/* ���b�N */
	#if (0!=USE_DEBUG_PAUSE_FILTER)
	#else
		#if 1/*(r33)*/
	{
		u16 *src;	src = render_image;
		int jj;
		for (jj=0; jj<(272/*-1 (�������G�t�F�N�g�ɂ���Ȃ�'-1'���v��) */); jj++)
		{
			int ii;
			for (ii=0; ii<(352); ii++)		/* �p�l������������ */		/* 352 == 480-128 */
			{
				u16 pixel;
				pixel = *src;
				if (0 != pixel)
				{
					pixel = (
						   (pixel & 0xf800) 		/* 16bit RGBA5650 �Ƃ��Đ̂�(�e�X�g) */
						| ((pixel & 0x0700)>>8) 	/* 16bit RGBA5650 �Ƃ��ė΂� ���Ƃ��ĐԂ� */
						| ((pixel & 0x001f)<<5) 	/* 16bit RGBA5650 �Ƃ��ĐԂ� ���Ƃ��ė΂� */
						);	/* �e�L�g�[���� */
					/* �␳ */
					if (0==pixel)
					{
						pixel = 0x0800;
					}
					*src = pixel/*0x4444*/;
				}
				src++;
			}
			src += (512-480+128);/* �c�� */
		}
	}
		#endif/*(r33)*/
		#if 0/*(r32)*/
	{
		u16 *src;	src = render_image;
		int jj;
		for (jj=0; jj<(272/*-1 (�������G�t�F�N�g�ɂ���Ȃ�'-1'���v��) */); jj++)
		{
			int ii;
			for (ii=0; ii<(352); ii++)		/* �p�l������������ */		/* 352 == 480-128 */
			{
				u16 pixel;
				pixel = *src;
				if (0 != pixel)
				{
/* 16bit RGBA5650 ��p */
	/*
	16: bbbb bggg gggr rrrr
	16: bbbb -ggg gg-r rrr-
		1111 0111 1101 1110 == 0xfede;
		0000 1000 0010 0001 == 0x0821; LSB
	*/
		//			pixel &= 0xfede;	/* 16bit RGBA5650 �Ƃ��čŉ��ʃr�b�g���}�X�N */
					pixel &= 0xf800;	/* 16bit RGBA5650 �Ƃ��Đ̂�(�e�X�g) */
			//		pixel >>= 1;		/* �P�x���� */
					//
			//		u16 *src2;	src2 = src+(1024/*512*/);/*1024 [bytes] == 512[words(u16)] x 2 */
			//		u16 pixel2;
			//		pixel2 = *src2;
			//		pixel2 &= 0xfede;	/* 16bit RGBA5650 �Ƃ��čŉ��ʃr�b�g���}�X�N */
			//		pixel2 >>= 1;		/* �P�x���� */
					//
					pixel = (pixel /*| pixel2 | 0x0821*/);	/* �e�L�g�[���� */
					/* �␳ */
					if (0==pixel)
					{
						pixel = 0x0800;
					}
					*src = pixel/*0x4444*/;
				}
				src++;
			}
			src += (512-480+128);/* �c�� */
		}
	}
		#endif/*(r32)*/
	#endif /* (USE_DEBUG_PAUSE_FILTER) */
	//#define psp_push_screen(aaa) psp_move_screen( SDL_00_VIEW_SCREEN, SDL_01_BACK_SCREEN )
	/* ����Gu�ŕ`�����ꍇ�B�n�[�h�E�F�A�[�@�\�ŁA�u����������̂ō��̂����ɂ܂Ƃ߂Ƃ� */
	//SDL_FillRect(sdl_screen[SDL_00_VIEW_SCREEN], NULL, 0/*SD L_MapRGB(sdl_screen[SDL_00_VIEW_SCREEN]->format,0,0,0)*/);
	if (SDL_MUSTLOCK(sdl_screen[SDL_00_VIEW_SCREEN]))	{	SDL_UnlockSurface(sdl_screen[SDL_00_VIEW_SCREEN]);	}	/* ���b�N���� */
}
