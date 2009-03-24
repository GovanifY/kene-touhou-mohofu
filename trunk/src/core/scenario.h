#include "zenkaku.h"

enum _story_state
{
	STORY_FADEOUT1,
	STORY_LOAD,
	STORY_FADEIN,
	STORY_WORKS,
	STORY_FADEOUT2,
	STORY_QUIT
};

enum _script_command
{
	SCP_BGTEXT,
	SCP_CLCURSOR,
	SCP_CLICK,
	SCP_FILTER,
	SCP_JUMP,
	SCP_LOADBG,
	SCP_LOADCUR,
	SCP_LOADL,
	SCP_LOADR,
	SCP_LOADSP,
	SCP_MOVESP,
	SCP_PARAMSP,
	SCP_RELOADSP,
	SCP_SAVECUR,
	SCP_SUBG,
	SCP_SUFILTER,
	SCP_SUL,
	SCP_SUR,
	SCP_SUSPRITE,
	SCP_SUTWINDOW,
	SCP_SUWINDOW,
	SCP_TEXT,
	SCP_TWINDOW,
	SCP_WAIT
};

typedef struct _scenario_script
{
	int command;
	int done;		//�I��������ǂ���
	int chain;		//�������s���ǂ���
					//0=�Ⴄ��	1=1��	2=2��
	char para0[200];
	int para1;
	int para2;
	int para3;
	void *data;
	struct _scenario_script *next;
} S_SCRIPT;

typedef struct _sc_sprite
{
	SDL_Surface *img;				/* Images */
	Uint8 frames;					/* �A�j���[�V����������\�� */
	Uint8 anim_speed;				/* ��ɓ��� */
	Uint8 anim_type;				/* �ǂ��������ɃA�j���[�V���������邩 */
	Uint8 anim_time;				/* �A�j���[�V������ */
	Uint8 alpha;					/* alpha�l */
	Uint8 flags;					/* �����ł͕\�������邩�ǂ����������� */
	int aktframe;					/* ���݂̃t���[�� */
	double anim_count;				/* �A�j���[�V�����̃^�C�~���O */
	double x,y; 					/* x,y���W���� */
	int w,h; 						/**/
	int cw,ch; 						/* ���S���W���� */
	int move_wait;					/* �������������Ƃ��̐���p */
	int move_x;						/* �ڕW���W */
	int move_y;
	int r_course;					/* ��̂̕��� */
	double angle;					/* ���m�ȕ��� */
} SC_SPRITE;

extern int thescript();
extern int script_init(char *filename, char *bg_name,int width);
extern void story_init();
extern void story_work();
