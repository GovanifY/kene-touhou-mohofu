#include "support.h"

extern SDL_Surface *back_screen;

static SDL_Surface *loaddot;		//load��ʗp
//2709266
void load_ing(void) 	//load��ʗp
{
	static int kazu=0;
	kazu++;
	int i;
	SDL_Rect dott;
	dott.h=0;
	dott.w=0;
	dott.y=248;
//	psp_clear_screen();		//��ʂɑ啝�ȕύX���Ȃ��Ƃ��͂���K�v�Ȃ��H//����ʂ������K�v�͌��X�Ȃ�
	psp_pop_screen();		//SDL_BlitSurface(loadpic, NULL, screen, NULL);
	for (i=0;i<(kazu &(4-1)/*% 4*/);i++)
	{
		if (i!=3)
		{
			dott.x=425+19*i;
			SDL_BlitSurface(loaddot, NULL, screen, &dott);
		}
	}
	SDL_Flip(screen);
}


/* �ǂݍ��񂾏��Ԃɉ摜�L���b�V���ɔz�u�����̂ŁA */
/* �摜�L���b�V���̏��Ԃ����߂�ׂ̓ǂݍ��݁B */
static void preload_gfx(void)
{
	/* �摜�L���b�V���ɍڂ���K�v��������̂��A */
	/* ��ɉ摜�L���b�V���ɍڂ���ׂ̓ǂݍ��� */
	/* �摜�L���b�V���ɍڂ��������ɓǂݍ��ށB  */
	/* �t�@�C���̊m�F�����˂�B */
	/* alle benoetigten Bilder in den Cache laden */

	SDL_Surface *tmp;

	/*[bullets]*/
	load_ing();

	tmp=loadbmp("kugel.png"); unloadbmp_by_surface(tmp);/*�Ԓe*/
	tmp=loadbmp("kugel2.png"); unloadbmp_by_surface(tmp);

//	tmp=loadbmp("bs hoot.png"); unloadbmp_by_surface(tmp);/*�e*/
//	tmp=loadbmp("bs hoot2.png"); unloadbmp_by_surface(tmp);/*�Ύቩ�e*/
	tmp=loadbmp2("bigkugel1.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("bigkugel2.png"); unloadbmp_by_surface(tmp);
	//tmp=loadbmp("sp ell_bullet_r.png"); unloadbmp_by_surface(tmp);/*�q�e*/
	//tmp=loadbmp("sp ell_bullet_g.png"); unloadbmp_by_surface(tmp);/*�f�e*/
	//tmp=loadbmp("sp ell_bullet_b.png"); unloadbmp_by_surface(tmp);/*�a�e*/
	tmp=loadbmp("bullet_maru8.png"); unloadbmp_by_surface(tmp);		/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
	tmp=loadbmp("bullet_ball8.png"); unloadbmp_by_surface(tmp);		/*�ےe�W(�Ԃq�f�a�Ύቩ��)*/
	tmp=loadbmp2("knife.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp2("knife_core.png"); unloadbmp_by_surface(tmp);
//
//	tmp=loadbmp2("knife0.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp2("knife1.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp2("knife2.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp2("knife3.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp2("knife4.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp2("knife5.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp2("knife6.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp2("knife7.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp2("knife8.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp2("knife9.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp2("knife10.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp2("knife11.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp2("knife12.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp2("knife13.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp2("knife14.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp2("knife15.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp2("knife16.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp2("knife17.png"); unloadbmp_by_surface(tmp);/*�p�~*/
	tmp=loadbmp("bullet_ming32.png"); unloadbmp_by_surface(tmp);		/*�{�X�e(���F)*/
	tmp=loadbmp("bullet_beam16.png"); unloadbmp_by_surface(tmp);		/*�j�e(���F)*/
	/*[item]*/
	load_ing();
	#if 0
//	tmp=loadbmp("bonus_p.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp("bonus_p_.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp("bonus_f.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp("bonus_h.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp("bonus_x.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp("coin.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp("bonus_s.png"); unloadbmp_by_surface(tmp);/*�p�~*/
	#else
	tmp=loadbmp("bonus_items.png"); unloadbmp_by_surface(tmp);
	#endif

	/* ���� / �_������ */
	tmp=loadbmp("weapon.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("firebomb.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("energy.png"); unloadbmp_by_surface(tmp);		/*�ǉ�*/
	tmp=loadbmp("extra.png"); unloadbmp_by_surface(tmp);		/*�ǉ�*/
	tmp=loadbmp("speed.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("plus100.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("plus1000.png"); unloadbmp_by_surface(tmp);

	/*�z�[�~���O�̈ʒu�\��*/
//	tmp=loadbmp("target.png"); unloadbmp_by_surface(tmp);/*�p�~*/

	/*bakuhatsu*/
	tmp=loadbmp("tr_blue.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("tr_red.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("tr_green.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp2("bakuha05.png"); unloadbmp_by_surface(tmp);		/*�ǉ�*/
	tmp=loadbmp2("bakuha06.png"); unloadbmp_by_surface(tmp);		/*�ǉ�*/
	tmp=loadbmp2("bakuha07.png"); unloadbmp_by_surface(tmp);		/*�ǉ�*/
	tmp=loadbmp("ex.png"); unloadbmp_by_surface(tmp);

	/*[jiki]*/
	load_ing();



//	tmp=loadbmp("homing.png"); unloadbmp_by_surface(tmp);/*�p�~*/
//	tmp=loadbmp("killray-b.png"); unloadbmp_by_surface(tmp);	/*���V���b�g�p�A���ݎg���ĂȂ�*/	/*�얲*/
//	tmp=loadbmp("killray-r.png"); unloadbmp_by_surface(tmp);	/*���V���b�g�p�A���ݎg���ĂȂ�*/	/*�얲*/
#if 1/*sprite.c*/
	/*�V���b�g*/
	tmp=loadbmp("shot_re.png"); unloadbmp_by_surface(tmp);	/*�얲*/						/*"plasma.png"*/
	tmp=loadbmp("shot_ma.png"); unloadbmp_by_surface(tmp);	/*������*/						/*"plasma_ma.png"*/
	tmp=loadbmp("shot_oz.png"); unloadbmp_by_surface(tmp);	/*���~���A*/					/*"plasma_oz.png"*/
	tmp=loadbmp("shot_ci.png"); unloadbmp_by_surface(tmp);	/*�`���m*/		/*�ǉ�*/		/*"plasma_ci.png"*/

	tmp=loadbmp2("needle_re.png"); unloadbmp_by_surface(tmp);	/*�얲*/		/*"fireball1.png"*/			/*�V���b�g*/
	tmp=loadbmp( "needle_ma.png"); unloadbmp_by_surface(tmp);	/*������*/		/*"missile.png"*/			/*�V���b�g*/
	tmp=loadbmp2("needle_oz.png"); unloadbmp_by_surface(tmp); 	/*���~���A*/	/*"bat.png"*/				/*�V���b�g*/
	tmp=loadbmp( "needle_ci.png"); unloadbmp_by_surface(tmp);	/*�`���m*/		/*"cirno_missile.png"*/		/*�V���b�g*/

	tmp=loadbmp("homing_re.png"); unloadbmp_by_surface(tmp);	/*�얲*/					/*"tshoot.png"*/
	tmp=loadbmp("homing_ma.png"); unloadbmp_by_surface(tmp);	/*������*/					/*"tshoot-ma.png"*/
	tmp=loadbmp("homing_oz.png"); unloadbmp_by_surface(tmp);	/*���~���A*/				/*"tshoot-oz.png"*/
	tmp=loadbmp("homing_ci.png"); unloadbmp_by_surface(tmp);	/*�`���m*/		/*�ǉ�*/	/*"tshoot-ci.png"*/

	/*����*/
	tmp=loadbmp("jiki_re.png"); unloadbmp_by_surface(tmp);	/*�얲*/		/*"ship-med.png"*/
	tmp=loadbmp("jiki_ma.png"); unloadbmp_by_surface(tmp);	/*������*/		/*"ship-med-ma.png"*/
	tmp=loadbmp("jiki_oz.png"); unloadbmp_by_surface(tmp);	/*���~���A*/	/*"ship-med-oz.png"*/	/*�ǉ�*/
	tmp=loadbmp("jiki_ci.png"); unloadbmp_by_surface(tmp);	/*�`���m*/		/*"ship-med-ci.png"*/	/*�ǉ�*/

	/*�����蔻��*/
	tmp=loadbmp("core_re.png"); unloadbmp_by_surface(tmp); 		/*�얲*/		/*"core.png"*/
	tmp=loadbmp("core_ma.png"); unloadbmp_by_surface(tmp);		/*������*/		/*"core-ma.png"*/
	tmp=loadbmp("core_oz.png"); unloadbmp_by_surface(tmp);		/*���~���A*/	/*"core-oz.png"*/
	tmp=loadbmp("core_ci.png"); unloadbmp_by_surface(tmp);		/*�`���m*/		/*"core-ci.png"*/	/*�ǉ�*/

	/* �I�v�V�����p�f�� */
	tmp=loadbmp("option1_re.png"); unloadbmp_by_surface(tmp);		/*�얲*/			/*"option.png"*/
	tmp=loadbmp("option2_re.png"); unloadbmp_by_surface(tmp);		/*�얲*/			/*"option.png"*/
	tmp=loadbmp("option3_re.png"); unloadbmp_by_surface(tmp);		/*�얲*/			/*"option.png"*/
	tmp=loadbmp("option4_re.png"); unloadbmp_by_surface(tmp);		/*�얲*/			/*"option.png"*/
//
	tmp=loadbmp("option1_ma.png"); unloadbmp_by_surface(tmp);		/*������*/			/*"mari_op.png"*/
	tmp=loadbmp("option2_ma.png"); unloadbmp_by_surface(tmp);		/*������*/			/*"mari_op.png"*/
	tmp=loadbmp("option3_ma.png"); unloadbmp_by_surface(tmp);		/*������*/			/*"mari_op.png"*/
	tmp=loadbmp("option4_ma.png"); unloadbmp_by_surface(tmp);		/*������*/			/*"mari_op.png"*/
//
	tmp=loadbmp("option1_oz.png"); unloadbmp_by_surface(tmp);		/*���~���A*/		/*"oze_op1.png"*/
	tmp=loadbmp("option2_oz.png"); unloadbmp_by_surface(tmp);		/*���~���A*/		/*"oze_op2.png"*/
	tmp=loadbmp("option3_oz.png"); unloadbmp_by_surface(tmp);		/*���~���A*/		/*"oze_op3.png"*/
	tmp=loadbmp("option4_oz.png"); unloadbmp_by_surface(tmp);		/*���~���A*/		/*"oze_op4.png"*/
//
	tmp=loadbmp("option1_ci.png"); unloadbmp_by_surface(tmp);		/*�`���m*/			/*"ci_op1.png"*/
	tmp=loadbmp("option2_ci.png"); unloadbmp_by_surface(tmp);		/*�`���m*/			/*"ci_op2.png"*/
	tmp=loadbmp("option3_ci.png"); unloadbmp_by_surface(tmp);		/*�`���m*/			/*"ci_op3.png"*/
	tmp=loadbmp("option4_ci.png"); unloadbmp_by_surface(tmp);		/*�`���m*/			/*"ci_op4.png"*/
//
#endif
	/* �{���p�f�� */
	/*�얲*/
//	tmp=loadbmp("cshoot.png"); unloadbmp_by_surface(tmp);	/*�I�v�V�����p�A���ݎg���ĂȂ�*/
	tmp=loadbmp2("cshoot1.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp2("cshoot2.png"); unloadbmp_by_surface(tmp);
	/*������*/
	tmp=loadbmp("star_shields_blue.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("star_shields_red.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("star_shields_green.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("star_shield_blue.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("star_shield_red.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("star_shield_green.png"); unloadbmp_by_surface(tmp);
	/*���~���A*/
	tmp=loadbmp("cross_red.png"); unloadbmp_by_surface(tmp);	/*���~���A*/
	tmp=loadbmp2("fire_wind_r.png"); unloadbmp_by_surface(tmp); /*���~���A*/
	tmp=loadbmp2("fire_wind_l.png"); unloadbmp_by_surface(tmp); /*���~���A*/
	tmp=loadbmp2("fire_wind_u.png"); unloadbmp_by_surface(tmp);	/*���~���A*/
	/*�`���m*/
	tmp=loadbmp("cirno_shields_blue.png"); unloadbmp_by_surface(tmp);	/*�`���m*/		/*�ǉ�*/
	tmp=loadbmp("cirno_shields_red.png"); unloadbmp_by_surface(tmp);	/*�`���m*/		/*�ǉ�*/
	tmp=loadbmp("cirno_shields_green.png"); unloadbmp_by_surface(tmp);	/*�`���m*/		/*�ǉ�*/
	tmp=loadbmp("cirno_shield_blue.png"); unloadbmp_by_surface(tmp);	/*�`���m*/		/*�ǉ�*/
	tmp=loadbmp("cirno_shield_red.png"); unloadbmp_by_surface(tmp); 	/*�`���m*/		/*�ǉ�*/
	tmp=loadbmp("cirno_shield_green.png"); unloadbmp_by_surface(tmp);	/*�`���m*/		/*�ǉ�*/

//	tmp=loadbmp("fireball.png"); unloadbmp_by_surface(tmp);		/*���{���o�[�p�A���ݎg���ĂȂ�*/


	/*[zako]*/
	load_ing();
	tmp=loadbmp("fairy.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("great_fairy.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("splash.png"); unloadbmp_by_surface(tmp);		//[***090124		�ǉ�

	tmp=loadbmp("ming.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("rwingx8.png"/*"rwing x.png"*/); unloadbmp_by_surface(tmp);/*�d�l�ύX*/
	tmp=loadbmp("iris.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("eyefo.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("12side.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("badguy.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("bgpanel.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("bgpanel2.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("crusher.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("cube.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("protectball.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("plasmaball.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("rotating_rocket.png"); unloadbmp_by_surface(tmp);

	tmp=loadbmp("grounder.png"); unloadbmp_by_surface(tmp);

	/*���o�p(���蔻��Ȃ�)*/
	tmp=loadbmp("wolke01_1.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("wolke02_1.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("wolke03_1.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("wolke01_2.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("wolke02_2.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("wolke03_2.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("wolke01_3.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("wolke02_3.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("wolke03_3.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("wolke01_4.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("wolke02_4.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("wolke03_4.png"); unloadbmp_by_surface(tmp);
	/*[boss]*/
	load_ing();
	tmp=loadbmp("boss01_0.png"/*"boss01-lo.png"*/); unloadbmp_by_surface(tmp);
	tmp=loadbmp("boss01_1.png"/*"boss01-lu.png"*/); unloadbmp_by_surface(tmp);
	tmp=loadbmp("boss01_2.png"/*"boss01-mo.png"*/); unloadbmp_by_surface(tmp);
	tmp=loadbmp("boss01_3.png"/*"boss01-mu.png"*/); unloadbmp_by_surface(tmp);
	tmp=loadbmp("boss01_4.png"/*"boss01-ro.png"*/); unloadbmp_by_surface(tmp);
//	tmp=loadbmp("boss01_5.png"/*"boss01-ru.png"*/); unloadbmp_by_surface(tmp);/*�p�~*/
	tmp=loadbmp("boss02_v2.png"); unloadbmp_by_surface(tmp);
//	tmp=loadbmp("boss02_v2x.png"); unloadbmp_by_surface(tmp);/*�p�~*/
	tmp=loadbmp("boss03-lo.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("boss03-lu.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("boss03-mo.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("boss03-mu.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("boss03-ro.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("boss03-ru.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("boss04.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp2("boss04-lo.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("boss05-lo.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("boss05-mo.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("boss05-ro.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("boss05-lu.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("boss05-mu.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("boss05-ru.png"); unloadbmp_by_surface(tmp);

	#if 1
	/*�t�H���g�̓L���b�V���ɍڂ�����ʖ�???*/
	/*[font]*/
	load_ing();
	tmp=loadbmp("font01.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("font02.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp/*2*/("font03.png"); unloadbmp_by_surface(tmp);/*???*/
	tmp=loadbmp("font04.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("font05.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("font06.png"); unloadbmp_by_surface(tmp);/*�ǉ�*/
	tmp=loadbmp/*2*/("font07.png"); unloadbmp_by_surface(tmp);/*???*/
	#endif

	/*[bg]*/
	load_ing();
	tmp=loadbmp2("health.png"); unloadbmp_by_surface(tmp);		/* �{�X�p���C�t�Q�[�W���[�^�[ */
	tmp=loadbmp("Player_Star.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("Bomb_Star.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp2("weapon_p.png"); unloadbmp_by_surface(tmp);	/*�T�C�h�p�l���p�p���[���[�^�[*/
	tmp=loadbmp("bg2.png"); unloadbmp_by_surface(tmp);			/*�Q�[�����T�C�h�p�l��*/

	#if 0
	/*���̕ӂ̓L���b�V���ɍڂ���K�v������???*/
	/*�L�����Z���p�����G*/
	tmp=loadbmp("sp_reimu_st.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("sp_marisa_st.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("sp_remiria_st.png"); unloadbmp_by_surface(tmp);

	/*�L�����Z���p�w�i*/
	tmp=loadbmp("sp_reimu_bg.jpg"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("sp_marisa_bg.jpg"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("sp_remiria_bg.jpg"); unloadbmp_by_surface(tmp);

	/*�Q�[�����a�f�w�i*/
	load_ing();
	tmp=loadbmp("back1.jpg"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("back1_a-0.jpg"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("back1_a-1.jpg"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("back2.jpg"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("back2_a-0.jpg"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("back2_a-1.jpg"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("back3.jpg"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("back3_a-0.jpg"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("back3_a-1.jpg"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("back4.jpg"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("back4_a-0.jpg"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("back4_a-1.jpg"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("back5.jpg"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("back5_a-0.jpg"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("back5_a-1.jpg"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("back6.jpg"); unloadbmp_by_surface(tmp);

	//tmp=loadbmp("plate.png"); unloadbmp_by_surface(tmp);		/*���O���͗p�f��*/

	tmp=loadbmp2("window.png"); unloadbmp_by_surface(tmp);		/* ��b�p */

	tmp=loadbmp("moon.jpg"); unloadbmp_by_surface(tmp);		/* �^�C�g����� */

	/*�L�[�R���t�B�O�p*/
	tmp=loadbmp("key_icon.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("keylist.png"); unloadbmp_by_surface(tmp);
	tmp=loadbmp("key_bg.png"); unloadbmp_by_surface(tmp);

	/*�I�[�v�j���O�f��*/
//	tmp=loadbmp("badblocks.png"); unloadbmp_by_surface(tmp);	/* �����F�������� */
//	#define USE_KETM_LOGO 0
//	#if (1==USE_KETM_LOGO)
//	tmp=loadbmp("ketm.png"); unloadbmp_by_surface(tmp);			/* ��KETM�^�C�g�� */
//	#endif
	#endif
}


/*
	for (i=2;i<=argc;i++) {
		if (!strncmp(argv[i-1],"-d",2)) {
			debug=1;
			error(ERR_DEBUG,"debug-mode enabled");
		} else if (!strncmp(argv[i-1],"-f",2)) {
			videoflags=SDL_FULLSCREEN;
		} else if (!strncmp(argv[i-1],"-j",2)) {
			use_joystick=1;
		} else if (!strncmp(argv[i-1],"-16",3)) {
			depth=16;
		} else if (!strncmp(argv[i-1],"-24",3)) {
			depth=24;
		} else if (!strncmp(argv[i-1],"-32",3)) {
			depth=32;
		} else if (!strncmp(argv[i-1],"-h",2)) {
			error(ERR_INFO,"%s:  a shoot-em-all game",argv[0]);
			error(ERR_INFO,"-h:  get this help");
			error(ERR_INFO,"-f:  fullscreen mode");
			error(ERR_INFO,"-j:  enable joystick-support (preliminary)");
			error(ERR_INFO,"-d:  enable debug messages");
			error(ERR_INFO,"-16: force 16 bit screen (default)");
			error(ERR_INFO,"-24: force 24 bit screen");
			error(ERR_INFO,"-32: force 32 bit screen");
			exit(0);
		} else {
			error(ERR_WARN,"unknown command line option: %s (try -h to get help)",argv[i-1]);
		}
	}

	fprintf(stdout,"FYI: very early prepreprealpha, debug-mode forced\n"); debug=1;
*/
	//display_vidinfo(screen);
	/*
	if (depth==0) {
		error(ERR_DEBUG,"we want the current screen bit-depth...");
	} else {
		error(ERR_DEBUG,"we want a %d bpp screen...",depth);
	}
	error(ERR_DEBUG,"... and got a %d bpp surface",screen->format->BitsPerPixel);
	*/

	/*
	if (use_joystick) {
		if (debug) {
			error(ERR_DEBUG,"%i joysticks found",SDL_NumJoysticks());
			for (i=0;i<SDL_NumJoysticks();i++) {
				error(ERR_DEBUG,"stick %d: %s",i,SDL_JoystickName(i));
			}
		}
		if (SDL_NumJoysticks()>0) {
			joy=SDL_JoystickOpen(0);
			if (joy) {
				error(ERR_DEBUG,"Joystick 0:");
				error(ERR_DEBUG,"Name: %s",SDL_JoystickName(0));
				error(ERR_DEBUG,"Axes: %d",SDL_JoystickNumAxes(joy));
				error(ERR_DEBUG,"Buttons: %d",SDL_JoystickNumButtons(joy));
				error(ERR_DEBUG,"Balls: %d",SDL_JoystickNumBalls(joy));
			} else {
				error(ERR_WARN,"could not open joystick #0");
			}
		}
	}
	*/
	/*
	error(ERR_DEBUG,"Key-configuration:");
	error(ERR_DEBUG,"up-key    : %s",SDL_GetKeyName(keyconfig.u));
	error(ERR_DEBUG,"down-key  : %s",SDL_GetKeyName(keyconfig.d));
	error(ERR_DEBUG,"left-key  : %s",SDL_GetKeyName(keyconfig.l));
	error(ERR_DEBUG,"right-key : %s",SDL_GetKeyName(keyconfig.r));
	error(ERR_DEBUG,"fire-key  : %s",SDL_GetKeyName(keyconfig.f));
	error(ERR_DEBUG,"escape-key: %s",SDL_GetKeyName(keyconfig.e));
	*/
//
//	int i;
	//SDL_Joystick *joy;

//	b_id=0; 	//[***090129		�ύX
/*
	if (use_joystick)
		initflags|=SDL_INIT_JOYSTICK;
*/
//	Uint32 initflags;	initflags=SDL_INIT_VIDEO;
//#if 0
//	static const int depth=16;
//#else
//	#define depth 16
//#endif
//	/*static*/const Uint32 videoflags = ;

	//SDL_ShowCursor(1);
	//SDL_WM_SetCaption("killeverythingthatmoves","ketm");

/*---------------------------------------------------------
	�V�X�e������������
---------------------------------------------------------*/
extern void init_imglist(void);
extern void init_math(void);
void game_init(/*int argc, char *argv[]*/)
{
	pspDebugScreenInit();
	pspDebugScreenPrintf("Starting K.E.N.E.\n");

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	if (SDL_Init(SDL_INIT_VIDEO/*initflags*/)<0)
	{
		CHECKPOINT;
		error(ERR_FATAL,"cant init SDL: %s",SDL_GetError());
	}
	if (atexit(SDL_Quit))
	{
		CHECKPOINT;
		error(ERR_WARN,"atexit dont returns zero");
	}

	screen = SDL_SetVideoMode(
		PSP_WIDTH,
		PSP_HEIGHT,
		/*depth*/16,
		/*videoflags*/(SDL_FULLSCREEN | SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_HWPALETTE| SDL_HWACCEL)
		);
	if (NULL == screen)
	{
		CHECKPOINT;
		error(ERR_FATAL,"cant open screen: %s",SDL_GetError());
	}
//	back_screen = NULL;
	back_screen = SDL_CreateRGBSurface(
		SDL_SWSURFACE/*SDL_HWSURFACE*/,/*���C����������*/
		PSP_WIDTH/*screen->w*/,
		PSP_HEIGHT/*screen->h*/,
		screen->format->BitsPerPixel,
		screen->format->Rmask,
		screen->format->Gmask,
		screen->format->Bmask,
		screen->format->Amask);
	if (NULL == back_screen)
	{
		CHECKPOINT;
		error(ERR_FATAL,"cant create SDL_Surface: %s",SDL_GetError());
	}
//
	ini_load();
	initSound();/*ini_load();����(moddir���킩��Ȃ�����ʉ����ǂ߂Ȃ�)*/
	init_math();
	init_imglist();

	{//static SDL_Surface *loadpic=NULL;		//load��ʗp
		SDL_Surface *loadpic=loadbmp("loadpng.png");
		SDL_BlitSurface(loadpic, NULL, back_screen, NULL);
		unloadbmp_by_surface(loadpic);
	}
	loaddot=loadbmp("maru1.png");		//���[�h�p�摜1�ŏ\�������
	SDL_SetColorKey(loaddot, SDL_SRCCOLORKEY, 0x00000000);

	preload_gfx();
	load_ing();
	keyboard_clear();
	font_init();
	load_ing();
	menusystem_init();
	fps_init();
	load_ing();


//	unloadbmp_by_surface(loadpic);
	unloadbmp_by_surface(loaddot);

	newstate(/*ST_INTRO*/ST_MENU/*ST_START_INTRO*/,0,1);
}

