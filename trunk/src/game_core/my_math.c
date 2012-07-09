
#include "game_main.h"

/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
psp �ł́A at an2(), si n(), sq rt() ���̒��z�֐��n���߂́A
psp �� MIPS CPU �� �̃R�v���Z�b�T������������B
�R�v���Z�b�T�ϊ����������������ɂ���ƁA���Ƀp�t�H�[�}���X�������̂ŁA
�ꃖ���ɓZ�߂��������s���x�͗y���ɑ����B
(CPU�����̖��߃L���b�V���ɏ�邽�߂Ɏ��s���x�������Ȃ�)

�Q�l:TECH I Vol.39 MIPS�v���Z�b�T����	http://www.cqpub.co.jp/interface/TechI/Vol39/
---------------------------------------------------------*/

#include "../../libgu/pspgu.h"//#include <pspgu.h>
#include "../../libgu/pgc.h"


#include "111_my_file.h"/*(my_file_common_name)*/

#include "kanji_system.h"

//#include "psp_vfpu.h"
//#include <malloc.h>

/*---------------------------------------------------------
	�����I�Ƀ��v���C�ɑΉ��ł���悤�ɁA
	���O�̗������[�`����p�ӂ��Ƃ��B
	(���Ȃ�e�L�g�[�ł���)
---------------------------------------------------------*/

static int rnd;
global int ra_nd(void)
{
	rnd = (rnd * 8513/*multiplier*/) + (179)/*addend*/;
	rnd = ((rnd) ^ (rnd>>8));
	return (rnd);
}


/*---------------------------------------------------------
	���������l�ݒ�(�����I�Ƀ��v���C�ɑΉ��ł���悤�ɁA
	�ʂ̎n�߂ɌŒ�l��ݒ肷��)
---------------------------------------------------------*/

global void set_rnd_seed(int set_seed)
{
	rnd = set_seed;
}


/*---------------------------------------------------------
	�L�[���͊֘A�̏���(�ėp)
---------------------------------------------------------*/

global u32 pad_config[KINOU_08_WARIATE_MAX];

#if (1==USE_PAD_STRUCT)
global PSP_PAD_GLOBAL_CLASS psp_pad;/*(r34)*/
#endif

static int	use_analog; 	/* �A�i���O�g�p�t���O(0:�g�p���Ȃ��A1:�g�p����) */

extern void save_screen_shot(void);
global void psp_pad_init(void)
{
	#if (0)
	/*
	�������ɂ͎��Ԃ�������͗l�B
	���̃^�C�~���O�ł͓��삵�Ȃ��B
	(���������Ă����ɓǂ����Ƃ��Ă���Ԏn�߂̓��͂��ǂ߂Ȃ��A�������Ԃ��o�ĂΓǂ߂�)
 */
	/* --- ���͑��u�̏����ݒ� */
//	sceCtrlSetSamplingCycle(0);/*���܂������Ȃ�*/
//	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);/* ���܂������Ȃ� */
	#endif
	/* boot_check_analog */
	use_analog = 1;/* �A�i���O�g�p�t���O(0:�g�p���Ȃ��A1:�g�p����) */
	{
		SceCtrlData pad;
		sceCtrlReadBufferPositive(&pad, 1);
		/* �N�����ɃA�i���O���͂��������ꍇ�A�̏Ⴕ�Ă���Ɣ��f���A�i���O���͖����B */
		if (pad.Lx < 64/*70*/)			{use_analog = 0;	}
		else if (pad.Lx > 192/*185*/)	{use_analog = 0;	}
		//
		if (pad.Ly < 64/*70*/)			{use_analog = 0;	}
		else if (pad.Ly > 192/*185*/)	{use_analog = 0;	}
		#if (0)
		if (0 == use_analog)
		{
			error(ERR_FATAL, (char*)"fail analog pad (%d,%d)", pad.Lx, pad.Ly );
		}
		#endif
	}
}



global void do_input_vbl(void)
{
	psp_pad.pad_data_alter = psp_pad.pad_data;
//
	SceCtrlData pad;
	#if (0)//(1==USE_VSYNC)
	/* ���ݖ���(r99) �����I�ɂ�v_sync��邪�A���SDL��F�X���Ȃ��Ɖ��Ƃ��Ȃ�Ȃ��B*/
	{
		sceCtrlPeekBufferPositive(&pad, 1);
		#if (1)
		/* ���_�Fvblank���Ƃ�Ȃ�����A�V����polling�����ۏ؂��Ȃ��B
			vblank���Ƃ�Ȃ��ꍇ�́A���炩�̎��ԑ҂����䂪�K�v�B */
		static u32 last_time = 0;/* �ŏI����(�O��̎���) */
		u32 now_time;/* ���݂̎���(����̎���) */
		now_time = pad.TimeStamp;/* ���݂̎���(����̎���)���擾 */
		/* ���ԃJ�E���^���I�[�o�[�t���[�����ꍇ�̏C������ */
		if (now_time < last_time)	/* ���ԃJ�E���^���I�[�o�[�t���[�����ꍇ */
		{
			now_time	+= 16666;
			last_time	+= 16666;/* �K���I�[�o�[�t���[���ď����Ȓl�ɂȂ锤 */
			#if (0)
			if (now_time < last_time)	/* ���肦�Ȃ��� */
			{/* �O�̈� */
				now_time	= 16666;
				last_time	= 0;
			}
			#endif
		}
		/* �����ŕK�� (now_time > last_time) �ɂȂ��Ă锤�B */
		int l_aaa;
		l_aaa = 0;//l_aaa++;	/*1*/
		do
		{
			l_aaa++;	/* �K���ʂ�̂ōŒ�ł� 1 �ɂȂ�B */
			now_time -= 16666;	/* 1 [frame]���̎��Ԃ����� */
		}
		while ( 16666 < (now_time-last_time) ); 	/* ������ 1 [frame]�ȓ� �łȂ���ΌJ��Ԃ��B */
		//
		if (l_aaa < 2) /* 16666.666 ...  == 1fs or less? */ 	/* 1 [frame]�ȓ��ɕ`��ł���� */
		{
			sceDisplayWaitVblankStart();	/* 1 [frame]�ȓ��̏ꍇ�̂� v_sync���B */
		}
		last_time = pad.TimeStamp;/* ���݂̎��Ԃ��ŏI���ԂƂ��ĕۑ� */
		#endif
	}
	#else
	/* ������̕����ł� v-sync ���̖������ۂ�(?)�B */
	/* profile �̂�Ɠ��͂��s���R�ɒx���񂾂���... */
	/**/
	sceCtrlReadBufferPositive(&pad, 1); 	/* sceCtrlReadBufferPositive�̏ꍇ�͂����ŉ������Ȃ���1/60�b�҂\��������B */
	/* ���_�F1/60�b�ȓ��œ��삵�AVSYNC���Q�[�����쐬����̂��s�\�ɂȂ�B
		������̕����̂܂܂ł͎�����A�e�������V���[�e�B���O�����͖̂���������B
		���͂�҂̂�(�Q�[����)�x���Ȃ�B
		VSYNC����1/60�b�œ��삷��Q�[�����쐬�s�\�ɂȂ�B
		(sceCtrlReadBufferPositive();��sceDisplayWaitVblankStart();�𓮍삳���邾���ŁA
		�ň��̏ꍇ 2�t���[��==��2/60�b==1/30�b ���Ԃ��������)
		(1/60�b�ȓ�[60fps]�ŃQ�[��������Ă��Apolling�҂���1/60�b�ŃQ�[���������Ȃ���30fps�ɂȂ����Ⴄ��������B
		����̋�_�Ŏ��ۂ����Ȃ鎖�͖������A�S���񓯊��Ȃ̂ōň��P�[�X�ōl����ΐ������B)
 */
	#endif
/*
	�Q�l: http://emuhani.seesaa.net/article/127733904.html
	�Q�l: http://forums.qj.net/zx-81s-homebrew-releases/146232-development-question.html
	sceCtrlReadBufferPositive �� sceCtrlPeekBufferPositive �̈Ⴂ�ɂ��āB
*/
	#if (1==USE_KEY_CONFIG)
		/* �L�[�R���t�B�O����̏ꍇ�A�uPSP�̃f�W�^�����́v�Ɓu�Q�[������p�̃f�W�^�����́v�͈������قȂ� */
		u32 pad_data;
	#else
		/* �L�[�R���t�B�O�Ȃ��̏ꍇ�A�uPSP�̃f�W�^�����́v���u�Q�[������p�̃f�W�^�����́v�ɂ��̂܂܎g�� */
		#define pad_data psp_pad.pad_data
	#endif /* (1==USE_KEY_CONFIG) */
	pad_data = pad.Buttons;
	{
		/* �W���A�i���O�L�[�@�\ */
	//	if (1==use_analog)
		if (0 != use_analog)
		{
			/* PSP�̃A�i���O���͂̓f�W�^�����͂֕ϊ��A�A�i���O�ʂ͏����̃��v���C�Ή����l���g�p���Ȃ��B */
			if (pad.Lx < 64/*70*/)			{pad_data |= PSP_CTRL_LEFT; 	}
			else if (pad.Lx > 192/*185*/)	{pad_data |= PSP_CTRL_RIGHT;	}
			//
			if (pad.Ly < 64/*70*/)			{pad_data |= PSP_CTRL_UP;		}
			else if (pad.Ly > 192/*185*/)	{pad_data |= PSP_CTRL_DOWN; 	}
		}
	}
	#if (1==USE_KEY_CONFIG)
	/* �㉺���E�̃L�[�R���t�B�O�͌��ݖ����Ƃ������ݒ�l�𖳎��B(�A�i���O���͂̓s��) */
	/* �L�[�R���t�B�O����̏ꍇ�ł��A�㉺���E��(�A�i���O���͂�)�s���ɂ��PSP�̃f�W�^�����͂����̂܂܎g�� */
//	psp_pad.pad_data = 0;
//	psp_pad.pad_data |= (pad_data & (PSP_CTRL_UP|PSP_CTRL_RIGHT|PSP_CTRL_DOWN|PSP_CTRL_LEFT));
	psp_pad.pad_data	= (pad_data & (PSP_CTRL_UP|PSP_CTRL_RIGHT|PSP_CTRL_DOWN|PSP_CTRL_LEFT/*|PSP_CTRL_SELECT|PSP_CTRL_START*/));/* �㉺���E/SELECT/START�͓s���ɂ��L�[�R���t�B�O�Ȃ� */
	/* PSP�̃f�W�^�����͂���L�[�R���t�B�O���l�����ē��͒l�����߂� */
	if (pad_data & PSP_CTRL_SELECT) 	{psp_pad.pad_data |= pad_config[KINOU_00_SELECT];		}	//	if (keyboard[KINOU_01_SELECT])		{psp_pad.pad_data |= (PSP_KEY_SELECT);}
	if (pad_data & PSP_CTRL_START)		{psp_pad.pad_data |= pad_config[KINOU_01_START];		}	//	if (keyboard[KINOU_02_PAUSE])		{psp_pad.pad_data |= (PSP_KEY_PAUSE);}
//
	if (pad_data & PSP_CTRL_LTRIGGER)	{psp_pad.pad_data |= pad_config[KINOU_02_LTRIGGER]; 	}	//	if (keyboard[KINOU_07_SNAP_SHOT])	{psp_pad.pad_data |= (PSP_KEY_SNAP_SHOT);}
	if (pad_data & PSP_CTRL_RTRIGGER)	{psp_pad.pad_data |= pad_config[KINOU_03_RTRIGGER]; 	}	//	if (keyboard[KINOU_08_SYSTEM])		{psp_pad.pad_data |= (PSP_KEY_SYSTEM);}
	if (pad_data & PSP_CTRL_TRIANGLE)	{psp_pad.pad_data |= pad_config[KINOU_04_TRIANGLE]; 	}	//	if (keyboard[KINOU_09_SLOW])		{psp_pad.pad_data |= (PSP_KEY_SLOW);}
	if (pad_data & PSP_CTRL_CIRCLE) 	{psp_pad.pad_data |= pad_config[KINOU_05_CIRCLE];		}	//	if (keyboard[KINOU_10_OPTION])		{psp_pad.pad_data |= (PSP_KEY_OPTION);}
	if (pad_data & PSP_CTRL_CROSS)		{psp_pad.pad_data |= pad_config[KINOU_06_CROSS];		}	//	if (keyboard[KINOU_11_SHOT])		{psp_pad.pad_data |= (PSP_KEY_SHOT_OK);}
	if (pad_data & PSP_CTRL_SQUARE) 	{psp_pad.pad_data |= pad_config[KINOU_07_SQUARE];		}	//	if (keyboard[KINOU_12_BOMB])		{psp_pad.pad_data |= (PSP_KEY_BOMB_CANCEL);}
	#endif /* (1==USE_KEY_CONFIG) */
	/* �X�N���[���V���b�g�@�\�B */
	// keypoll�ɓ����Ɖ��̂����܂������Ȃ������̂ł������ɏꏊ��ύX�B
	if (psp_pad.pad_data & PSP_KEY_SNAP_SHOT)	{save_screen_shot();	 }/*keyboard[KINOU_07_SNAP_SHOT]*/



}
//		 if (keyboard[KINOU_06_LEFT])	{psp_pad.pad_data |= PSP_CTRL_LEFT; 	/*direction=-1;*/		}
//	else if (keyboard[KINOU_04_RIGHT])	{psp_pad.pad_data |= PSP_CTRL_RIGHT;	/*direction=1;*/		}
//		 if (keyboard[KINOU_03_UP]) 	{psp_pad.pad_data |= PSP_CTRL_UP;		}
//	else if (keyboard[KINOU_05_DOWN])	{psp_pad.pad_data |= PSP_CTRL_DOWN; 	}



/*---------------------------------------------------------
	�V�X�e���̊�b����
---------------------------------------------------------*/

#if (1==USE_KETM_IMAGE_CHACHE)
static void imglist_garbagecollect(void);
#endif /*(1==USE_KETM_IMAGE_CHACHE)*/
global void *my_calloc(size_t size)
{
	void *ptr;
	ptr = malloc(size);
	if (NULL == ptr)
	{
		#if (1==USE_KETM_IMAGE_CHACHE)
		#if (0)
		psp_warning( (char*)
		//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
			"memory: �������� %d �o�C�g�m��" "\\n"
			"���悤�Ǝv���܂���������܂���B" "\\n"
			"�K�v������������������鎖�ŁA" "\\n"
			"�������m�ۂ��Ē��킵�Ă݂܂��B", size);
		#endif
		imglist_garbagecollect();
		ptr = malloc(size);
		if (NULL == ptr)
		{
			psp_fatal_error( (char*)
			//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
				"memory: ������������܂���B" );
		}
		#else
		psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
			"memory: �������� %d �o�C�g�m��" "\\n"
			"���悤�Ǝv���܂���������܂���ł����B", size);
		#endif /*(1==USE_KETM_IMAGE_CHACHE)*/
	}
	memset(ptr, 0, size);/* calloc()���ɁA�K��0�N���A�[����悤�ɕύX */
	return (ptr);
}


#if (1==USE_KETM_IMAGE_CHACHE)
/*---------------------------------------------------------
	�摜�L���b�V���֘A
	-------------------------------------------------------
	�����摜�𕡐��ǂݍ��񂾏ꍇ�Ƀ����������ʂɂȂ���������Ȃ��B
	�����œ����摜��ǂݍ��񂾏ꍇ�ɂ́A���ۂɂ͓ǂݍ��܂Ȃ��ŁA
	�O�ɓǂݍ��񂾉摜�Ɠ������̂��g���B
	�g�[�^���ł��������摜��ǂݍ��񂾂��́A���ꂼ��̉摜�L���b�V���̎Q�Ɛ��ł킩��B
---------------------------------------------------------*/

typedef struct _imglist
{
	SDL_Surface *img;			/* �ǂݍ��񂾃T�[�t�F�C�X��ێ� */
	struct _imglist *next;		/* ���̉摜�ւ̃��X�g�|�C���^ */
	int refcount;				/* �����摜�̎Q�Ɛ� */
	char name[128]; 			/* ���O */	/* 128==4*32 (psp�̍\����32�̔{���Ŏw��) */ 	/*256*/
} MY_IMAGE_LIST;

/* �摜�L���b�V���̃��X�g */
static MY_IMAGE_LIST *my_image_list /*= NULL*/;/* �����̏�����������psp�ł͐���ɓ��삵�Ȃ������H */

global void init_imglist(void)
{
	my_image_list = NULL;
}


/*---------------------------------------------------------
	�摜�L���b�V���ɐV�K�摜��ǉ�
---------------------------------------------------------*/

static void imglist_add_by_file_name(SDL_Surface *img_surface, char *name)
{
	MY_IMAGE_LIST		*new_list;
	new_list			= my_calloc(sizeof(MY_IMAGE_LIST));
	strcpy(new_list->name, name);
	new_list->refcount	= 1;
	new_list->img		= img_surface;
	if (NULL == my_image_list)		/* �擪�̏ꍇ */
	{
		new_list->next	= NULL;
	}
	else							/* �擪�ȊO�̏ꍇ */
	{
		new_list->next	= my_image_list;
	}
	my_image_list		= new_list;
}


/*---------------------------------------------------------
	�摜�L���b�V���ɓ����t�@�C�������Ȃ�������
---------------------------------------------------------*/

static SDL_Surface *imglist_search_by_file_name(char *name)
{
	MY_IMAGE_LIST		*tmp_list;
	tmp_list			= my_image_list;/* �摜�L���b�V�����X�g�̐擪���璲�ׂ�B */
	while (NULL != tmp_list)
	{
		if (0 == tiny_strcmp(name, tmp_list->name))
		{
			tmp_list->refcount++;
			return (tmp_list->img);
		}
		tmp_list		= tmp_list->next;	/* �� */
	}
	return (NULL);
}


/*---------------------------------------------------------
	�摜�L���b�V�����J��
	������������Ȃ��Ȃ����̂ŉ摜�L���b�V�����Ŏg���ĂȂ����̂��J��
---------------------------------------------------------*/

static void imglist_garbagecollect(void)
{
	MY_IMAGE_LIST	*tmp_list;		/* �^�[�Q�b�g */
	MY_IMAGE_LIST	*prev_list; 	/* �O */
	MY_IMAGE_LIST	*next_list; 	/* �� */
	tmp_list		= my_image_list;
	prev_list		= NULL;
	next_list		= NULL;
	while (NULL != tmp_list)
	{
		next_list	= tmp_list->next;
		if (0 == tmp_list->refcount)	/* �摜�͎g���ĂȂ� */
		{
			/* �擪�̏ꍇ�͈ȑO�������̂œ��ʏ��� */
			if (NULL == prev_list)		/* �擪�̏ꍇ */
			{
				my_image_list		= next_list;/* �擪���C�� */
			}
			else						/* �擪�ȊO�̏ꍇ */
			{
				prev_list->next 	= next_list;/* ���O�ɘA�� */
			}
		//	SDL_FreeSurface(tmp_list->img);
			free (tmp_list);			/* �^�[�Q�b�g���J�� */
		}
		else	/* �摜�͎g�p�� */
		{
			prev_list	= tmp_list; 	/* ���ׂ������Ƃ΂� */
		}
		tmp_list		= next_list;	/* �� */
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

//global SDL_Surface *load_chache_bmp2(char *set_filename)
global SDL_Surface *load_chache_bmp(void)/*char *file_name*/
{
//	char file_name[128];	/* 128==4*32 (psp�̍\����32�̔{���Ŏw��) */ 	/*64 50*/
//	strcpy(file_name, DIRECTRY_NAME_DATA_STR "/");
//	strcat(file_name, set_filename);
//	strcpy(file_name, set_filename);
//
	SDL_Surface *s1;
	SDL_Surface *s2;
//
	s1 = imglist_search_by_file_name(my_file_common_name);
	if (NULL != s1)
	{
		return (s1);
	}
	//if ((s1=SDL_LoadBMP(my_file_common_name))==NULL)
//
	s1 = IMG_Load(my_file_common_name);
	if (NULL == s1)
	{
		psp_fatal_error(/*ERR_FATAL,*/ (char*)
		//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
			"load chache bmp: �摜������܂���B" "\\n"
			"%s", my_file_common_name);
	}
//	if (use_alpha)
//	{
//		s2 = SDL_DisplayFormatAlpha(s1);/* �T�[�t�F�X��\���t�H�[�}�b�g�ɕϊ�����B */	// ���l��������surface
//	}
//	else
	{
		s2 = SDL_DisplayFormat(s1);/* �T�[�t�F�X��\���t�H�[�}�b�g�ɕϊ�����B */
	}
	if (NULL == s2)
	{
		psp_fatal_error(/*ERR_FATAL,*/ (char*)
		//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
			"load chache bmp: �摜�ϊ����郁������" "\\n"
			"����܂���B" "\\n"
			"%s", my_file_common_name);
	}
	SDL_FreeSurface(s1);
	s1 = NULL;
	imglist_add_by_file_name(s2, my_file_common_name);
	return (s2);
}


/*---------------------------------------------------------
	img_surface �� MY_IMAGE_LIST���ɂ��邩�m�F�����A
	�摜�����������ꍇ�A�摜�L���b�V�����X�g�̎Q�Ɛ�������炷�B
---------------------------------------------------------*/

global void unloadbmp_by_surface(SDL_Surface *img_surface)
{
	MY_IMAGE_LIST	*tmp_list;
	tmp_list		= my_image_list;/* �摜�L���b�V�����X�g�̐擪 */
	while (NULL != tmp_list)
	{
		if (img_surface == tmp_list->img)	/* �摜���������� */
		{
			if (0 == tmp_list->refcount)	/* �摜�L���b�V�����X�g�̎Q�Ɛ� */
			{
				/* ���[�h���ĂȂ��̂ɊJ���B */
				psp_fatal_error( (char*)
				//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
					"chache: �摜��ǂ݂���ł��Ȃ��̂ɁA" "\\n"
					"������悤�Ƃ��܂����B" "\\n"
					"%s", tmp_list->name);
			}
			else
			{
				tmp_list->refcount--;		/* ����炷�B */
			}
			return; 	/* ����I�� */
		}
		tmp_list = tmp_list->next;			/* �� */
	}
	/* ������Ȃ��B */
	psp_fatal_error( (char*)
	//	"0123456789012345678901234567890123456789"	// ���p40��"�ő�\��������"
		"chache: �v���O�����~�X�B" "\\n"
		"�������摜���A������܂���B");
//	return; 	/* �ُ�I�� */
}
#endif /*(1==USE_KETM_IMAGE_CHACHE)*/


/*---------------------------------------------------------
	�o�b�N�o�b�t�@�ɉ摜�L���b�V�����g���Ȃ���A�w�i�����[�h�B
---------------------------------------------------------*/
/*---------------------------------------------------------
	�t�@�C�����Ŏw��
---------------------------------------------------------*/
global void psp_load_bg_file_name(void)/*char *file_name*/
{
	/* �t�@�C���ԍ��Ŏw��(0x00-0x1f�܂�0x20==' ') */
	if ((u8)my_file_common_name[0] < (u8)' ')
	{
		static const char *const_aaa_str[(BG_TYPE_99_MAX_HAIKEI)] =
		{
			DIRECTRY_NAME_DATA_STR "/bg/" "title_bg.png",
			DIRECTRY_NAME_DATA_STR "/bg/" "name_regist.png",
			DIRECTRY_NAME_DATA_STR "/bg/" "key_config.png",
			DIRECTRY_NAME_DATA_STR "/bg/" "music_room.png",
			DIRECTRY_NAME_DATA_STR "/bg/" "loading.png",
		};
	//	char file_name[128];	/* 128==4*32 (psp�̍\����32�̔{���Ŏw��) */ 	/*64 50*/
		strcpy(my_file_common_name, (char *)const_aaa_str[(int)/*bg_type_number*/(my_file_common_name[0])]);
	}
	{
		SDL_Surface *loadpic;
		#if (1==USE_KETM_IMAGE_CHACHE)
		loadpic 	= load_chache_bmp();/* file_name */
		#else
		loadpic 	= IMG_Load(my_file_common_name);/* file_name */
		#endif
	//	psp_clear_screen();
	//	SDL_SetAlpha(loadpic, SDL_SRCALPHA, 255);
	//	PSPL_UpperBlit(loadpic, NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], NULL);
		PSPL_UpperBlit(loadpic, NULL, cb.sdl_screen[SDL_01_BACK_SCREEN], NULL);
		#if (1==USE_KETM_IMAGE_CHACHE)
		unloadbmp_by_surface(loadpic);	// �L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bloadpic = NULL;
		#else
		SDL_FreeSurface(loadpic);	// �L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��B
		loadpic = NULL;
		#endif
	}
}
	#if 0
	SDL_Surface *loadpic	= load_chache_bmp2((char *)const_aaa_str[bg_type_number]);//, 0, 0/*1*/);
//	psp_clear_screen();
//	SDL_SetAlpha(loadpic, SDL_SRCALPHA, 255);
//	PSPL_UpperBlit(loadpic, NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], NULL);
	PSPL_UpperBlit(loadpic, NULL, cb.sdl_screen[SDL_01_BACK_SCREEN], NULL);
	unloadbmp_by_surface(loadpic);	// �L���b�V���ɓ����Ă�̂�NULL�ɏo���Ȃ��Bloadpic = NULL;
	#endif



/*---------------------------------------------------------
	('0'+cg.game_now_stage)
	get_stage_chr(cg.game_now_stage);
---------------------------------------------------------*/
global char get_stage_chr(int stage_type_number)
{
	static const char const_aaa_chr[(16)] =
	"0123"
	"4567"
	"8901"
	"234";/* + '\0' (EOS) */
	return (const_aaa_chr[stage_type_number&(16-1)]);
}
