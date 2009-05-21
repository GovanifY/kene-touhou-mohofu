
/*---------------------------------------------------------

---------------------------------------------------------*/

#include "support.h"

#include "enemy.h"
#include "loadlv.h"
#include "scenario.h"

extern STAGE_DATA *leveltab;

extern int select_player;
extern int practice_mode;

/*---------------------------------------------------------

---------------------------------------------------------*/

extern void add_clouds( 				STAGE_DATA *l);

/* �U�R�Ő錾���K�v�Ȃ���(�O���[�o��) */
	//	���̑�
extern void add_enemy_all_clear(		STAGE_DATA *l);
	//	�{�X
extern void add_boss_alice( 			STAGE_DATA *l);/*int lv*/
extern void add_boss_cirno( 			STAGE_DATA *l);/*int lv*/
extern void add_boss_aya(				STAGE_DATA *l);/*int lv*/
extern void add_boss_kaguya(			STAGE_DATA *l);/*int lv*/
extern void add_boss_sakuya(			STAGE_DATA *l);/*int lv*/
	//	[���^�G]�d��
extern void add_chuu_youkai1(			STAGE_DATA *l);/*int lv*/

	//	���̑��U�R
extern void add_zako_obake( 			STAGE_DATA *l);/*int lv*/
extern void add_zako_yukari1(			STAGE_DATA *l);/*int lv*/
extern void add_zako_yukari2(			STAGE_DATA *l);/*int lv*/
	//	�ы�
extern void add_zako_aka_kedama1(		STAGE_DATA *l);/*int lv*/
extern void add_zako_niji_kedama1(		STAGE_DATA *l);/*int lv*/
extern void add_zako_midori_kedama1(	STAGE_DATA *l);/*int lv*/
extern void add_zako_midori_kedama2(	STAGE_DATA *l);/*int lv*/
extern void add_zako_kedama1(			STAGE_DATA *l);/*int lv*/
	//	���� �A�z��
extern void add_zako_inyou1(			STAGE_DATA *l);/*int lv*/
extern void add_zako_tatsumaki1(		STAGE_DATA *l);/*int lv*/
	//	�d��
extern void add_zako_kakomi1(			STAGE_DATA *l);/*int lv*/
	//	���d��
extern void add_zako_meido1(			STAGE_DATA *l);/*int lv*/
extern void add_zako_meido2(			STAGE_DATA *l);/*int lv*/
extern void add_zako_meido3(			STAGE_DATA *l);/*int lv*/
extern void add_zako_meido4(			STAGE_DATA *l);/*int lv*/
	//	���d��
extern void add_zako_ao_yousei1(		STAGE_DATA *l);/*int lv*/
extern void add_zako_ao_yousei2(		STAGE_DATA *l);/*int lv*/

extern void add_enemy_panels(			STAGE_DATA *l);/* const short xxx, const short yyy, const short speed256, int type*/

extern void bg2_control(				STAGE_DATA *l);

extern void add_enemy_load_bg(			STAGE_DATA *l);// unsigned char bg_type/*l->user_y*/, int err_time/*l->time*/
extern void add_enemy_eigo_string(		STAGE_DATA *l);// /*char *text, int y*/
extern void add_enemy_load_picture( 	STAGE_DATA *l);// /*char *filename,int xpos, int ypos*/


/*---------------------------------------------------------

---------------------------------------------------------*/

int difficulty=RANK_NORMAL;

static Uint32 stage_start_time;
//static Uint32 game_start_time;

/*---------------------------------------------------------

---------------------------------------------------------*/

static void enemy_error(STAGE_DATA *l)/*int lv*/
{
//	error(ERR_WARN,"unknown enemy ");
}

/*---------------------------------------------------------
	�V���[�e�B���O �Q�[�� �f�[�^�[�̉��
	-------------------------------------------------------
	�����ɒǋL����΂���قǁA�V���[�e�B���O�Q�[���{�̂�
	�x���Ȃ�̂ŁA���ӂ��ĒǋL���Ă���B
	������(parth)����loadlv.c�ŗ\�ߍς܂��Ă������B
---------------------------------------------------------*/

static void thegame_level(STAGE_DATA *l)/*, int lev*/
{
	/* ���ԃR�[�h�`���̃R�}���h����e�֐��ɕ��򂷂� */
	/* enemy�̐�����ԍ��ŊǗ�(loadlv.c��ctype_name[]�ɑΉ����Ă���) */
	void (*aaa[ETYPE_MAX])(STAGE_DATA *l) =/*int*/
	{
		add_clouds,/* [�ԕ���؂�] */	/*NULL*/	/* add_clouds();�����Ō�����Ȃ��ꍇ�́Aenemy_error(); */
	//	���̑�
		add_enemy_all_clear,		/* �Q�[�� �S�X�e�[�W �N���A�[ */
	//	�{�X
		add_boss_alice,
		enemy_error/*add_boss_cirno*/,
		add_boss_aya,
		add_boss_kaguya,
		add_boss_sakuya,		// [***090207		�ǉ�
	//	����G[���^�G]
		add_chuu_youkai1,			/*	="�d��1",	ok	*/		// [***090207	�ǉ�
	//	���̑��U�R
		add_zako_obake, 			/*	-"���΂�1", ok. */
		add_zako_yukari1,			/*	C"���ґ�1", ok? */		/*enemy_error*/
		add_zako_yukari2,			/*	C"���ґ�2", ok? */		/*enemy_error*/
	//	�ы�
		add_zako_aka_kedama1,		/*	a"�Ԗы�1", ok. */
		add_zako_niji_kedama1,		/*	C"���ы�1", 	*/		/*enemy_error*/
		add_zako_midori_kedama1,	/*	-"�Ζы�1", ok. */
		add_zako_midori_kedama2,	/*	C"�Ζы�2", ok? */		/*enemy_error*/
		add_zako_kedama1,			/*	-"�ы�1",	ok. */
	//	���� �A�z��
		add_zako_inyou1,			/*	-"�A�z��1", ok. */
		add_zako_tatsumaki1,		/*	C"����1",	ok? */		/*enemy_error*/
	//	�d��
		add_zako_kakomi1,			/*	-"�͗d��1", ok. */
	//	���d��
		add_zako_meido1,			/*	-"���C�h1", ok. */
		add_zako_meido2,			/*	-"���C�h2", ok. */
		add_zako_meido3,			/*	C"���C�h3", ok--?	*/	/*enemy_error*/
		add_zako_meido4,			/*	-"���C�h4", ok. */
	//	���d��
		add_zako_ao_yousei1,		/*	="�d��1", ok	*/		// [***090207	�ǉ�
		add_zako_ao_yousei2,		/*	="�d��2", ok	*/		// [***090124	�ǉ�
//
		enemy_error,/* [�ԕ���؂�] */	/*BTYPE_00_NONE*/
		add_enemy_panels,/*BTYPE_01_BGPANEL1*/
		add_enemy_panels,/*BTYPE_02_BGPANEL2*/
		add_enemy_panels,/*BTYPE_03_GROUNDER*/
		add_enemy_panels,/*BTYPE_04_MAGIC_FORMATION*/
		bg2_control,/*BTYPE_05_BG_CONTROL*/
//
		add_enemy_eigo_string,/*ETYPE_01_ENGLISH_TEXT*/
		add_enemy_load_bg,/*ETYPE_02_LOAD_BG*/
		add_enemy_load_picture,/*ETYPE_03_PICTURE*/
	};
	(*aaa[ (int)(l->user_command) ])(l);	/* ���ԃR�[�h�`���̃R�}���h����e�֐��ɕ��򂷂� */
}

/*---------------------------------------------------------

---------------------------------------------------------*/
void init_stage_start_time(void)
{
	stage_start_time=psp_get_uint32_ticks();
}

/*---------------------------------------------------------
	�V���[�e�B���O�Q�[���{�̂̏�����
---------------------------------------------------------*/
extern int continue_stage;

#define TIME_20_DBWAIT /*20*/20/*2*/
static int dbwait/*=TIME_20_DBWAIT*/;		//�{�X��|�����Ƃ��ɒe�����ł����邽�߂̎��Ԋm��
extern void set_rnd_seed(int set_seed);
extern /*int*/void load_stage(void/*int level*/);
extern void player_init(void);
extern void score_panel_init(void);
void common_load_init(void)
{
	set_rnd_seed(player_now_stage); 	/* �����n��̏����� */
//
	/* Load next stage */
	load_stage();//if (load_stage(/*level*/)==0)	{	error(ERR_WARN,"no entrys for level %d",level);}
	// ���[�h���͏����������Ă���̂ŁA���[�h��Ɏ��Ԃ��č쐬����B
	init_stage_start_time();/*stage_start_time=psp_get_uint32_ticks();*/
//	game_start_time=psp_get_uint32_ticks();

//	stage_start_time=psp_get_uint32_ticks();
//	game_time=(psp_get_uint32_ticks()-stage_start_time);
//	game_time=(psp_get_uint32_ticks()-stage_start_time);
	//	play_music(BGM_01_stage1);	�R�����g�A�E�g
	psp_loop = (ST_WORK_GAME_PLAY|0);
}
//	int stage;	stage=((PLAYER_DATA *)player->data)->stage;

void shooting_game_init(void)
{
	play_voice_auto_track(VOICE01_HIT);
//
	dbwait=TIME_20_DBWAIT;
//
	score_panel_init();
	//controller_remove_all();
	player_init();
//
	#if (1==USE_ENDING_DEBUG)
	/*player_init();����̕K�v*/
	if (5==continue_stage)
	{	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	//	pd->bo ssmode	= B07_AF TER_LOAD;
		pd->state_flag	|= (STATE_FLAG_10_IS_LOAD_SCRIPT|STATE_FLAG_05_IS_BOSS|STATE_FLAG_11_IS_BOSS_DESTROY);
		continue_stage--;
		practice_mode=0;
	}
	#endif //(1==USE_ENDING_DEBUG)
	player_now_stage/*data->now_stage*/ /*level*/	= continue_stage/*+1-1*/ /*1*/;
//
	common_load_init();
}

extern void init_stage_start_time(void);
extern void player_stage_clear(void);
void stage_clear_work(void)
{
	//PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	//
	player_stage_clear();
//
	common_load_init();
}

/*---------------------------------------------------------
	�{�X���ꏈ��
---------------------------------------------------------*/
static Uint32 game_time;
//	if (B02_BOSS_DESTROY==pd->bo ssmode) //�{�X��|�����Ƃ��̏���
//	if (/*STATE_FLAG_11_IS_BOSS_DESTROY==*/ (pd->state_flag & STATE_FLAG_11_IS_BOSS_DESTROY))
//	{
//	}
//	else
extern void player_loop_quit(void);
void boss_destroy(void) 	//�{�X��|�����Ƃ��̏���
{
		item_from_bullets(SP_ITEM_05_HOSI);
		//TIME_20_DBWAIT�t���[���҂��Ă�����s�B�{�X��|�������ɉ�ʂɕ\������Ă���e��S�ď��������̂��߂ɕK�v�B
		stop_music();
		play_voice_auto_track(VOICE03_BOSS_HAKAI);		// [***090313		�ǉ�	�����ƃX�}�[�g�Ȃ��������肻�������ǎv�����Ȃ������B
		;
//
	{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
		pd->state_flag &= (~(STATE_FLAG_13_DRAW_BOSS_GAUGE));/*off*/
//		pd->state_flag		&= (~(STATE_FLAG_03_SCORE_AUTO_GET_ITEM));		/* �I��� */
		if (0==practice_mode)/*���K���[�h�ł̓{�X��C�x���g�͌���Ȃ���B*/
		{
		//	pd->bo ssmode=B07_AFTER_LOAD;
			pd->state_flag |= STATE_FLAG_10_IS_LOAD_SCRIPT;
		}
		else/*���K���[�h*/
		{
			player_loop_quit();
		}
	}
}
void incliment_scene(void)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	{
		/*�{�X�퓬��C�x���g*/
	//	if (B09_STAGE_LOAD==pd->bo ssmode) // 9:stage�ǂݍ���
		if (/*STATE_FLAG_05_IS_BOSS == */(pd->state_flag & STATE_FLAG_05_IS_BOSS))
		{
			psp_loop = (ST_WORK_STAGE_CLEAR|0);
		}
		/*�{�X�퓬�O�C�x���g*/
		else
	//	if (B08_START == pd->bo ssmode) // 8:�{�X�Ȃ�炵�A1�{�X�Ƃ̐퓬�ցB
		{
			set_music_volume(128);
			play_music( player_now_stage+8 );
			pd->state_flag |= (STATE_FLAG_05_IS_BOSS|STATE_FLAG_13_DRAW_BOSS_GAUGE);
		}
	}
}
//
extern void script_load(void);
void my_special(void)
{
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
//	if (pd->bo ssmode==B05_BEFORE_LOAD) 	// [***090313	�ǉ�
//	if (pd->bo ssmode==B07_AFTER_LOAD)		// [***090313	�ǉ�
	if (pd->state_flag & STATE_FLAG_10_IS_LOAD_SCRIPT)		// [***090313	�ǉ�
	{
		pd->state_flag &= (~(STATE_FLAG_10_IS_LOAD_SCRIPT));/*off*/
		script_load(/*0 1*/);
	}
	if (pd->state_flag & (STATE_FLAG_11_IS_BOSS_DESTROY))
	{
	//	pd->state_flag |= STATE_FLAG_03_SCORE_AUTO_GET_ITEM;
	//	if (TIME_20_DBWAIT==dbwait)
	//	{
	//	}
	//	else
		dbwait--;
	//	if (0==dbwait)		// [***090313		�ύX
		if (1>dbwait)		// [***090313		�ύX
		{
			dbwait=TIME_20_DBWAIT;
		//
			pd->state_flag &= (~(STATE_FLAG_11_IS_BOSS_DESTROY));/*off*/
			boss_destroy();
		//
		}
	}
	/* �X�N���v�g���I������H */
	if (pd->state_flag & (STATE_FLAG_12_END_SCRIPT))
	{
		pd->state_flag &= (~(STATE_FLAG_12_END_SCRIPT));/*off*/ 	/*	pd->bo ssmode=B00_NONE;*/
	//	pd->state_flag &= (~(STATE_FLAG_12_END_SCRIPT));/*off*/ 	/*	pd->bo ssmode=B00_NONE;*/	/*B01_BA TTLE*/
		incliment_scene();
	}
}
/*---------------------------------------------------------
	�V���[�e�B���O�Q�[���{�̂̃��C�����[�`��
	-------------------------------------------------------
	�����ɒǋL����΂���قǁA�V���[�e�B���O�Q�[���{�̂�
	�x���Ȃ�̂ŁA���ӂ��ĒǋL���Ă���B
	�߂����Ɏ��s���Ȃ����͊֐������ĊO�ɒǂ��o�����B
---------------------------------------------------------*/

extern void script_display(void);
extern void score_display(void);
extern void bg_work_draw(void);
extern void draw_score_chache(void);
void shooting_game_work(void)
{
//	if (psp_loop != (ST_WORK_GAME_PLAY&0xff00) /*|| state.newsta te==1*/) return;
	/* game_time=Zeit seit Spielbeginn in 1/10 sec. */
	game_time=(psp_get_uint32_ticks()-stage_start_time);
//
	PLAYER_DATA *pd=(PLAYER_DATA *)player->data;
	if (pd->state_flag & STATE_FLAG_14_GAME_LOOP_QUIT)
	{
		;	/* GAMEOUT�� */
	}
	else
	{
		/* �����Ă� */
	//	if (0==my_pad)
		if (0==(my_pad & PSP_KEY_PAUSE))
		{
			if (my_pad_alter & PSP_KEY_PAUSE)
			{
			//	if (0==(pd->state_flag & STATE_FLAG_06_IS_SCRIPT))/*���܂ɂ��܂������Ȃ���������*/
				{
					psp_loop=(ST_INIT_MENU|ST_MENU_SUB_PAUSE);//newsta te(ST_MENU,ST_MENU_SUB_PAUSE,1);
				}
			}
		}
		#if 1
		{
			STAGE_DATA *l;
			/*
			This routine, serch back to begin.
			���̃��[�`���͋t���Ɍ������܂��B
			*/
			l = leveltab;
			while (l != NULL)/* [head ==NULL] then end. */
			{
				if (l->done == 0)	/* enemy set done flag */
				{
					#if 1
					if (game_time >= l->time)
					#else
					if (v_time >= ((l->time) ) )
					#endif
					{
						thegame_level(l);	/*, *level*/
						l->done=1;	/* enemy set done flag */
					}
				}
				l=l->next;/* choice alter. */
			}
		}
		#else

		#endif
//
		/* ���ꏈ�� */
	//	if (B00_NONE != pd->bo ssmode)
		if (pd->state_flag & (STATE_FLAG_10_IS_LOAD_SCRIPT|STATE_FLAG_11_IS_BOSS_DESTROY|STATE_FLAG_12_END_SCRIPT))
		{
			my_special();
		}
//
	}
//
	#if 0/*�Q�[�����ԃf�o�b�O�p*/
	/* �p�l���̃X�R�A���ɃQ�[�����Ԃ� �\��������B���Ă����������������Ⴄ�B */
	((PLAYER_DATA *)player->data)->score		= (game_time);
	#endif
//
	/* ���� */
	bg_work_draw();
	//controller_work();
	sprite_work000(SP_GROUP_ALL);
//	sprite_work222(SP_GROUP_ALL);/*�e���p*/
	/* �`�� */
	sprite_display000(SP_GROUP_ALL);
//	sprite_display222(SP_GROUP_ALL);/*�e���p*/
	draw_score_chache();
	// ���̕ӂ͑��x�ቺ����̂ŃR�[���o�b�N�ɂ��ׂ�
	if (/*STATE_FLAG_06_IS_SCRIPT==*/(pd->state_flag & STATE_FLAG_06_IS_SCRIPT))				{	script_display();	}		//parsys_display();
	if (/*STATE_FLAG_09_IS_PANEL_WINDOW==*/(pd->state_flag & STATE_FLAG_09_IS_PANEL_WINDOW))	{	score_display();	}
}

/*---------------------------------------------------------
	�|�[�Y���j���[�p���Ԓ���
	-------------------------------------------------------
	�|�[�Y���͎��Ԃ������̂�(�Q�[�����Ԃ�)��������ׂ̎d�g��
	���j���[(menu.c)�̃|�[�Y���畜�A����ۂɂ̂ݎg�p����B
---------------------------------------------------------*/

void adjust_start_time(Uint32 pause_time)
{
	stage_start_time += psp_get_uint32_ticks()-pause_time;
}
