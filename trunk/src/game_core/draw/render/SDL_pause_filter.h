
/*---------------------------------------------------------
	�������G�t�F�N�g(r40-)
	-------------------------------------------------------
	�ŐV�̌���(?)�̌��ʁA������╔����1�p�X�ŕ`�悵�ĂȂ��āA
	���Ȃ��Ƃ�2�p�X�ŕ`�悵�Ă��鎖���������B
	-------------------------------------------------------
	�|�[�Y�t�B���^�[�̌���
	�P�D�F
	�g�Ń|�[�Y��ʂ̃X�N�V�������Ɣ������������F���ۂ��Ȃ�B
	�����Ŕ�==RGB(0xff, 0xff, 0xff)�Ƃ���ƁA
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
---------------------------------------------------------
	�Ƃ肠�����A�悭�킩���̂ŁA4�_�T���v�����O�ɂ��ߖT�t�B���^�������Ă݂��B
src1	����������
src2	����������
		����������
-----------------------[�p�X1]�̏���	p1,p2,p3,p4 �𕽋ς���p1�ɕ`�����ށB
src1	p1 p2 --
src2	p3 -- p4
-----------------------[�p�X2]�̏���	p1,p2,p3,p4 �𕽋ς���p4�ɕ`�����ށB
src1	p1 -- p2
src2	-- p3 p4
-----------------------
		�S��ʏ�����2��J��Ԃ��B
		�������Ax�[��2[pixel]�y�сAy�[��1[pixel]�͏������Ȃ��B
	*/

	/* �p�l������������ */		/* GAME_WIDTH==352 == 480-128 */

//	#define MY_FILTER_WINDOW_SIZE_X_03_DOT	(3)
//	#define MY_FILTER_WINDOW_SIZE_Y_02_DOT	(2)
	#define MY_FILTER_X_SIZE_350_DOT	(GAME_WIDTH-(32)-(2))/*(x��2[pixel]�������Ȃ�, 32[pixel]�������Ȃ�)*/
	#define MY_FILTER_Y_SIZE_271_DOT	(GAME_HEIGHT-(1))/*(y��1[pixel]�������Ȃ�)*/

/* ���z�X�N���[��(SDL_00_VIEW_SCREEN)�ɂ������G�t�F�N�g�������� */
global void psp_pause_filter(void)
{
	#if (1)/* 1==�S��ʂɃG�t�F�N�g��������B 0==SDL�����̂݃G�t�F�N�g��������B */
	s_gu_save_screen();/* vram����SDL��ʂɋ����R�s�[ */
	#endif
	if (SDL_MUSTLOCK(cb.sdl_screen[SDL_00_VIEW_SCREEN]))	{	SDL_LockSurface(cb.sdl_screen[SDL_00_VIEW_SCREEN]); 	}	/* ���b�N */
	{
		// psp_BGR565 == bbbb bggg gggr rrrr
		#define MY_16_G_(aaa) ( (((aaa&0x07e0) )) )
		#define MY_16_BR(aaa) ( (((aaa&0xf81f) )) )
		{/*[�p�X1]�ォ�珈������B*/
			u16 *src1;	src1 = render_image + (32); 							/* �n�߂̃��C��, 32[pixel]���炷 */
			u16 *src2;	src2 = render_image + (32) + (PSP_BUFFER_WIDTH512); 	/* 1���C����, 32[pixel]���炷 */
			int jj;
			for (jj=0; jj<(MY_FILTER_Y_SIZE_271_DOT); jj++)/*(y��1[pixel]�������Ȃ�)*/
			{
				int ii;
				for (ii=0; ii<(MY_FILTER_X_SIZE_350_DOT); ii++)/*(x��2[pixel]�������Ȃ�)*/		/* �p�l������������ */
				{
					u16 pixel1;
					u16 pixel2;
					u16 pixel3;
					u16 pixel4;
					pixel1 = (*(src1  ));
					pixel2 = (*(src1+1));
					pixel3 = (*(src2  ));
					pixel4 = (*(src2+2));
					{
						u32 g_1;
						u32 br1;
						g_1 = (((MY_16_G_(pixel1))+(MY_16_G_(pixel2))+(MY_16_G_(pixel3))+(MY_16_G_(pixel4)))>>2);
						br1 = (((MY_16_BR(pixel1))+(MY_16_BR(pixel2))+(MY_16_BR(pixel3))+(MY_16_BR(pixel4)))>>2);
						//
						*src1 = (
							   ((br1 ) & 0xf81f)		/* 16bit RGBA5650 �Ƃ��Đ� */
							| (((g_1 ) & 0x07e0)   )	/* 16bit RGBA5650 �Ƃ��ė� */
							);	/* ���� */
					}
					src1++;
					src2++;
				}
				src1 += (PSP_BUFFER_WIDTH512-(MY_FILTER_X_SIZE_350_DOT));/* �c�� */ 	/*(x��2[pixel]�������Ȃ�)*/
				src2 += (PSP_BUFFER_WIDTH512-(MY_FILTER_X_SIZE_350_DOT));/* �c�� */ 	/*(x��2[pixel]�������Ȃ�)*/
			}
			/*[�p�X2]�����珈������B*/
			{	/*(�Ō�ɏ��������ړ����͗]�v�Ȃ̂ő��E������B������1[pixel]�]�v�Ȃ̂ł��̕��������B)*/
				src1 -= (PSP_BUFFER_WIDTH512-(MY_FILTER_X_SIZE_350_DOT)+(1));/* �c�� */ /* ���� */
				src2 -= (PSP_BUFFER_WIDTH512-(MY_FILTER_X_SIZE_350_DOT)+(1));/* �c�� */ /* ���� */
			}
			for (jj=0; jj<(MY_FILTER_Y_SIZE_271_DOT); jj++)/*(y��1[pixel]�������Ȃ�)*/
			{
				int ii;
				for (ii=0; ii<(MY_FILTER_X_SIZE_350_DOT); ii++)/*(x��2[pixel]�������Ȃ�)*/	/* �p�l������������ */
				{
					u16 pixel1;
					u16 pixel2;
					u16 pixel3;
					u16 pixel4;
					pixel1 = (*(src1  ));
					pixel2 = (*(src1+2));
					pixel3 = (*(src2+1));
					pixel4 = (*(src2+2));
					{
						u32 g_1;
						u32 br1;
						g_1 = (((MY_16_G_(pixel1))+(MY_16_G_(pixel2))+(MY_16_G_(pixel3))+(MY_16_G_(pixel4)))>>2);
						br1 = (((MY_16_BR(pixel1))+(MY_16_BR(pixel2))+(MY_16_BR(pixel3))+(MY_16_BR(pixel4)))>>2);
						//
						*(src2+2) = (
							   ((br1 ) & 0xf81f)		/* 16bit RGBA5650 �Ƃ��Đ� */
							| (((g_1 ) & 0x07e0)   )	/* 16bit RGBA5650 �Ƃ��ė� */
							);	/* ���� */
					}
					src1--;
					src2--;
				}
				src1 -= (PSP_BUFFER_WIDTH512-(MY_FILTER_X_SIZE_350_DOT));/* �c�� */ 	/*(x��2[pixel]�������Ȃ�)*/
				src2 -= (PSP_BUFFER_WIDTH512-(MY_FILTER_X_SIZE_350_DOT));/* �c�� */ 	/*(x��2[pixel]�������Ȃ�)*/
			}
		}
	}
	/*(�F���A�ϊ�����)
	�{���͐F������������̂����ǁA����͕`����+�����+�����ς݉�ʂ�3��ʕ��̃T�[�t�F�C�X���m�ۂł����
	����ʂƏ����ς݉�ʂ����������̃A���t�@�u�����h�ō������ĕ`���ʂɕ`�����Ŏ����ł���B
	r40���݂�(�������s���Ȃ̂�)2��ʕ��̃T�[�t�F�C�X�����m�ۂ��ĂȂ�����A���̕��@�ł͏o���Ȃ��B
	*/
	{
		u16 *src;	src = render_image + (32);//, 32[pixel]���炷
		int jj;
		for (jj=0; jj<(GAME_HEIGHT); jj++)
		{
			int ii;
			for (ii=0; ii<(GAME_WIDTH -(32)); ii++) 	/* �p�l������������, 32[pixel]���炷 */ 	/* GAME_WIDTH==352 == 480-128 */
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
			src += (PSP_BUFFER_WIDTH512-PSP_WIDTH480+PANEL_WIDTH+32);/* �c��, 32[pixel]���炷 */
		}
	}
	if (SDL_MUSTLOCK(cb.sdl_screen[SDL_00_VIEW_SCREEN]))	{	SDL_UnlockSurface(cb.sdl_screen[SDL_00_VIEW_SCREEN]);	}	/* ���b�N���� */
}
