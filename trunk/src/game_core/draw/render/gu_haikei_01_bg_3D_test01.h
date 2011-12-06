
/*---------------------------------------------------------
	�����͕핗 �` Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	���̃t�@�C���͒��ڃC���N���[�h���܂���B
	"draw_screen.c" ����̂݊ԐړI�ɃC���N���[�h���܂��B
---------------------------------------------------------*/


/*---------------------------------------------------------
	3D�w�i(�e�X�g��)
	-------------------------------------------------------
	Gum���g��Ȃ���(Gu+vfpu)��3D���o����炵���̂ŁA
	�e�X�g�ō���Ă݂�B
	���x�́H�H�H
---------------------------------------------------------*/

#include "psp_vfpu.h"
#if 0
typedef struct quat
{
	float x, y, z, w;
} __attribute__((aligned(16))) ScePspQuatMatrix;

void vfpu_srand(unsigned int x);
float vfpu_randf(float min, float max);

float vfpu_ease_in_out(float t);

void vfpu_perspective_matrix(ScePspFMatrix4 *m, float fovy, float aspect, float near, float far);
void vfpu_identity_matrix(ScePspFMatrix4 *m);

void vfpu_quaternion_copy(ScePspQuatMatrix *dst, ScePspQuatMatrix *src);
void vfpu_quaternion_from_euler(ScePspQuatMatrix *qout, float rx, float ry, float rz);
void vfpu_quaternion_exp(ScePspQuatMatrix *qout, ScePspQuatMatrix *qin);
void vfpu_quaternion_ln(ScePspQuatMatrix *qout, ScePspQuatMatrix *qin);
void vfpu_quaternion_sample_linear(ScePspQuatMatrix *qout, ScePspQuatMatrix *a, ScePspQuatMatrix *b, float t);
void vfpu_quaternion_to_matrix(ScePspQuatMatrix *q, ScePspFMatrix4 *m);
#endif

//								--aabbggrr (ABGR8888)
#define MY_COLOR_ARGB888_WHITE	0xffffffff
#define MY_COLOR_ARGB888_BLUE	0xffff7f7f
#define MY_COLOR_ARGB888_GREEN	0xff7fff7f
#define MY_COLOR_ARGB888_RED	0xff7f7fff

typedef struct
{
	/*float*/u8 	u;			/* 0(���[) ... 255(�E�[) */ 		/* �e�N�X�`��(u, v)���W */
	/*float*/u8 	v;			/* 0(��[) ... 255(���[) */
	u32 			color;		/* 0x00000000 ... 0xffffffff */ 	/* ���_�J���[(ARGB8888) */
	/*float*/s8 	x;			/* -128 ... 0(���S) ... 127 */		/* ���_(x, y, z)���W */
	/*float*/s8 	y;			/* -128 ... 0(���S) ... 127 */
	/*float*/s8 	z;			/* -128 ... 0(���S) ... 127 */
} Vertex_cube;
/*
	���̃|���S���͒P���Ȑ����́B(�T�C�R��)
	�A���ACCW�̏ꍇ�ɁA��������݂Ėʂ��\�ɂȂ�悤�ɂ��Ă���B
	(�O�����猩��ΑS�����Ȃ̂ŕ\������Ȃ�)
	�T�C�R���Ȃ̂ŁA6�ʑ́B1�̖ʂ� GU_TRIANGLES (�O�p�`) �� 2�g���ĕ`���B(1�̖� == �����` == �l�p�` == �O�p�`��2��)
	���������āA (12*3) == (6[��] * 2[�ʓ�����O�p�̐�] * 3[���_�B�O�p�`�Ȃ̂�3���_])

*/
static Vertex_cube	__attribute__((aligned(16))) cube_polygon_model[(12*3)] =
{
	/* �e�N�X�`���͉摜�T�C�Y�ɑ΂���(0...255, 0...255)��(u, v)���W�Ŏw�肷��B*/
	/* (u, v)���W�̓e�N�X�`���̓\��t����ʒu���w�肷��B */
	/* �摜�T�C�Y��512, 64��(u,v)��(  0,  0)�Ȃ�A�摜�ʒu��(  0,  0) */
	/* �摜�T�C�Y��512, 64��(u,v)��(127,127)�Ȃ�A�摜�ʒu��(255, 31) */
	/* �摜�T�C�Y��512, 64��(u,v)��(255,255)�Ȃ�A�摜�ʒu��(511, 63) */
//
	/* �摜�T�C�Y��256,256��(u,v)��(  0,  0)�Ȃ�A�摜�ʒu��(  0,  0) */
	/* �摜�T�C�Y��256,256��(u,v)��(127,127)�Ȃ�A�摜�ʒu��(127,127) */
	/* �摜�T�C�Y��256,256��(u,v)��(255,255)�Ȃ�A�摜�ʒu��(255,255) */
	/* �܂�摜�T�C�Y��256,256�Ȃ�Adot by dot�A�摜�ʒu==(u,v) �ɂȂ�B */
//
	/* ���_�ɐF��t����Ƃ������́A�܂�(�@���Ƃ����Ӗ��ł͂Ȃ���)�O�[���V�F�[�f�B���O�ŕ⊮�����Ƃ������B */
	/* ���̃n�[�h�E�F�A�[�⊮�͕`�惂�[�h(�F�̉𑜓x16bit�Ƃ�)�Ɋ֌W�Ȃ����32bit�ōs����̂ł��ꂢ�B */
//
	/* ���̕ӂ��������悤�ɁAGu�����ɂ͐F32bit(8888)�̕`��G���W�����������B */
	/* �F�̉𑜓x16bit�Ƃ�(5650, 5551, 4444)��clut�̕`�惂�[�h�̏ꍇ�́u�O��(�܂�vram)�]�����v(�܂�`�掞)�ɐF�ϊ����Ă���B */
	/* �uGu����݂�vram�̃A�h���X���ς�鎖�ő��x�ቺ����y�i���e�B�[ >>> �`�掞�̐F�ϊ����x�v �Ȃ̂ŁA16bit��clut�̕������ʓI�ɕ`�悪�����B */
//
	/* �e�N�X�`���̈ʒu 				���_�̈ʒu���W */
	/* u	v  �F(ABGR8888) 			 x	  y    z */
	{  0,	0, MY_COLOR_ARGB888_WHITE,-127,-127, 127}, // #0	// 0	/* �� */
	{255, 255, MY_COLOR_ARGB888_BLUE,  127, 127, 127}, // #2	// 5	/* �� �� */
	{255,	0, MY_COLOR_ARGB888_WHITE,-127, 127, 127}, // #1	// 4[]

	{  0,	0, MY_COLOR_ARGB888_WHITE,-127,-127, 127}, // #0	// 0
	{  0, 255, MY_COLOR_ARGB888_WHITE, 127,-127, 127}, // #3	// 1
	{255, 255, MY_COLOR_ARGB888_BLUE,  127, 127, 127}, // #2	// 5	/* �� �� */
//
	{  0,	0, MY_COLOR_ARGB888_WHITE,-127,-127,-127}, // #4	// 3[]	/* �� */
	{255, 255, MY_COLOR_ARGB888_WHITE, 127, 127,-127}, // #6	// 6
	{255,	0, MY_COLOR_ARGB888_WHITE, 127,-127,-127}, // #5	// 2

	{  0,	0, MY_COLOR_ARGB888_WHITE,-127,-127,-127}, // #4	// 3[]
	{  0, 255, MY_COLOR_ARGB888_WHITE,-127, 127,-127}, // #7	// 7
	{255, 255, MY_COLOR_ARGB888_WHITE, 127, 127,-127}, // #6	// 6
//
	{  0,	0, MY_COLOR_ARGB888_WHITE, 127,-127,-127}, // #5	// 0	/* �� */
	{255, 255, MY_COLOR_ARGB888_GREEN, 127, 127, 127}, // #2	// 7	/* �� �� */
	{255,	0, MY_COLOR_ARGB888_WHITE, 127,-127, 127}, // #3	// 3

	{  0,	0, MY_COLOR_ARGB888_WHITE, 127,-127,-127}, // #5	// 0
	{  0, 255, MY_COLOR_ARGB888_WHITE, 127, 127,-127}, // #6	// 4
	{255, 255, MY_COLOR_ARGB888_GREEN, 127, 127, 127}, // #2	// 7	/* �� �� */
//
	{  0,	0, MY_COLOR_ARGB888_WHITE,-127,-127,-127}, // #4	// 0[]	/* �� */
	{255, 255, MY_COLOR_ARGB888_WHITE,-127, 127, 127}, // #1	// 7[]
	{255,	0, MY_COLOR_ARGB888_WHITE,-127, 127,-127}, // #7	// 3

	{  0,	0, MY_COLOR_ARGB888_WHITE,-127,-127,-127}, // #4	// 0[]
	{  0, 255, MY_COLOR_ARGB888_WHITE,-127,-127, 127}, // #0	// 4
	{255, 255, MY_COLOR_ARGB888_WHITE,-127, 127, 127}, // #1	// 7[]
//
	{  0,	0, MY_COLOR_ARGB888_WHITE,-127, 127,-127}, // #7	// 0	/* �� */
	{255, 255, MY_COLOR_ARGB888_RED,   127, 127, 127}, // #2	// 2	/* �� �� */
	{255,	0, MY_COLOR_ARGB888_WHITE, 127, 127,-127}, // #6	// 1

	{  0,	0, MY_COLOR_ARGB888_WHITE,-127, 127,-127}, // #7	// 0
	{  0, 255, MY_COLOR_ARGB888_WHITE,-127, 127, 127}, // #1	// 3[]
	{255, 255, MY_COLOR_ARGB888_RED,   127, 127, 127}, // #2	// 2	/* �� �� */
//
	{  0,	0, MY_COLOR_ARGB888_WHITE,-127,-127,-127}, // #4	// 4[]	/* �Ԕ� */
	{255, 255, MY_COLOR_ARGB888_WHITE, 127,-127, 127}, // #3	// 6
	{255,	0, MY_COLOR_ARGB888_WHITE,-127,-127, 127}, // #0	// 7

	{  0,	0, MY_COLOR_ARGB888_WHITE,-127,-127,-127}, // #4	// 4[]
	{  0, 255, MY_COLOR_ARGB888_WHITE, 127,-127,-127}, // #5	// 5
	{255, 255, MY_COLOR_ARGB888_WHITE, 127,-127, 127}, // #3	// 6

};


#define TEST_AUTO_ROT 1
/* 3D��Ԃׂ̈̊�{�s�� */
static ScePspFMatrix4 m4_projection3d;	/* �ǂ��킩��Ȃ����ǁA��p���v�Z���Ă����B 			�͕핗�ł͈�ؓ������ĂȂ��B�W����p�ŌŒ�B */
static ScePspFMatrix4 m4_view;			/* (�����_�����O����ꍇ��)���_���W�n�B 				�͕핗�ł͎g���Ă���B */
static ScePspFMatrix4 m4_model; 		/* (�����_�����O����ꍇ��)���[���h���W�n==����W�B	�͕핗�ł͈�ؓ������ĂȂ��B�P�ʍs��ŏ��������ČŒ�B */
/*
	�͕핗�ł́A��{�I��3D��Ԃ͌Œ�ň�ؓ������Ă��Ȃ��B
	���̎��̂���]�����Ă���B
	���̂̉�]�͎���3�̍s��ŊǗ����Ă���B
	p9_qcam 	�J�����s��B�J����(���_)�Ƃ����̂͌䕼�����邩������Ȃ��B
	p9_qa		�s��`�B(�O��̈ʒu������Ă����p)
	p9_qb		�s��a�B(�v�Z���̍���̈ʒu�p)


*/
#if (1==TEST_AUTO_ROT)
static ScePspQuatMatrix p9_qcam;	/* �J����(���m�ɂ͍s�񂶂�Ȃ��ăN�H�[�^�j�I���B�N�H�[�^�j�I��==4x4�s��) */
static ScePspQuatMatrix p9_qa;		/* �s��`(���m�ɂ͍s�񂶂�Ȃ��ăN�H�[�^�j�I���B�N�H�[�^�j�I��==4x4�s��) */
static ScePspQuatMatrix p9_qb;		/* �s��a(���m�ɂ͍s�񂶂�Ȃ��ăN�H�[�^�j�I���B�N�H�[�^�j�I��==4x4�s��) */
#endif
/*
	�N�H�[�^�j�I��:
	�|���S���Ŏp������ӂɊm�肳����ׂɎg��4x4�s��B
	-------------------------------------------------------
	�p�����Ă����Ă��F�X�����ˁB
	��̍��W�n�ɑ΂��ĕ��̂��ǂꂾ���X���Ă邩�̎p���B�Ƃ�
	��̍��W�n�ɑ΂��ăJ����(���_)���ǂꂾ���X���Ă邩�̎p���B�Ƃ�

	-------------------------------------------------------
	�N�H�[�^�j�I���Ƃ����̂́A3D�ł����s���4x4�s��B
	3D�Ȃ�(x,y,z�Ȃ񂾂���)3x3�s��ŗǂ��Ȃ��́H�Ǝv�������m��Ȃ����A
	���ۂ���Ă݂�Δ��邪�A�S�R�ǂ��Ȃ��B
	-------------------------------------------------------
	3D�͕\�����邪�u��Γ������Ȃ����v���Ă�Ȃ�A3x3�s��ł��܂�Ŗ�薳���B
	�񂪁A�Q�[���ł͕��ʁA������ˁB
	�����v�Z�����������Ȃ�A4x4�s�񂶂�Ȃ��ƁA��O�������炯�ŕ��G�ɂȂ��B
	�s���]�������猳�ɖ߂���ŗǂ�����������B
	���[��[���4x4�s��B
	-------------------------------------------------------
	���A�N�H�[�^�j�I�����̂̐������܂����ȁH
	��[�ƁA�|���S���Ŏp��������킷�ꍇ�A��̍��W�n�ɑ΂��Č��ǂǂꂾ������Ă邩��
	����΁A�p���͈�ӂɌ��܂�B
	��̍��W�n�ɑ΂���3x3�s��B�ō����������Ă���΁A�p���͈�ӂɌ��܂�B
	���ꂪ�A�N�H�[�^�j�I���B�ŁA���ۂ�4x4�s��B
	-------------------------------------------------------
	�p�������߂�Ƃ����l�����͉����N�H�[�^�j�I���̐ꔄ�������Ė�ł͂Ȃ��A
	�N�H�[�^�j�I���ŁA�p�������߂�Ƃ����l�����ȊO�ɂ��A�u�I�C���[�p�Ŏp�������߂�Ƃ����l�����v������B
	(�������̕����Â��l����)
	�ł��A�I�C���[�p�Ŏp�������߂�ꍇ�́A�u���ʂ���]���鏇���Ɉˑ�����v�̂ŁA
	��]���鏇�������߂Ă����Ȃ��Ɩ]�ތ��ʂ������Ȃ��B

	��̓I�ɂ́A
	�n�߂�x��������45�x�񂵂āA����y��������60�x�񂵂����ʂ̌����ƁB
	�n�߂�y��������60�x�񂵂āA����x��������45�x�񂵂����ʂ̌����́A
	���������ɂȂ�Ȃ��ŁA�S�R�Ⴄ�����������Ă���Ęb�B

	�N�H�[�^�j�I���̏ꍇ�́A��]���鏇���Ƃ��l�������̂ɖ����̂ŁA�����������͋N����Ȃ��B
*/



/* ���ڍs��ɒl��ݒ肵�Ă݂�e�X�g�B */
#define USE_CHOKUSETU_SETTEI_TEST (1)

#if (1==USE_CHOKUSETU_SETTEI_TEST)
static ScePspQuatMatrix p9_test;	/* �e�X�g�p */
#endif

		#define USE_5SEC (0)
		#if (1==USE_5SEC)/* �����J�E���^�l�̌v�Z */
static	int nnn;
		#else
static	u8 nnn;
		#endif
	// run sample
//	/*int*/float val_x;
//	/*int*/float val_y;
//	/*int*/float val_z;
//	/*int*/float val_w;


#if (1==TEST_AUTO_ROT)
/*
	�V�����J�����ʒu�����߂�B
	�J�����ʒu�́Ax�l, y�l, z�l, ���ɒP�Ȃ闐���B
	(x�l, y�l, z�l�͊p�x)�B
	�p�x�� 0 ... 511 �܂ŁB
	(1��512�����̒P�ʌn)�B

	���� x�l, y�l, z�l �̓x�N�g���ł͂Ȃ��āA
	�I�C���[�p���v�Z����ꍇ�ɉ��J�����̗�(�p�������)�B

	�܂�A
	�I�C���[�p���v�Z����ꍇ�ɁA
	�n�߂�x���� x�l�� �񂵂āA
	����y���� y�l�� �񂵂āA
	�Ō�ɂ�z���� z�l�� �񂷁B
	���̌��ʏo�����x�N�g�����g���B


	�J�����ʒu����A�s��a��

*/
static void test_auto_rot_aaaa(void)
{
	/* �O��̍s����Ƃ��Ƃ�(A���ړ������ʒu�BB�͈ړ����̈ʒu) */
	vfpu_quaternion_copy(&p9_qa, &p9_qb);	/* �O��̍s��a���s��`�ɃR�s�[����B(���m�ɂ͍s�񂶂�Ȃ��ăN�H�[�^�j�I���B�N�H�[�^�j�I��==4x4�s��) */
//	vfpu_quaternion_from_euler(&p9_qb,
//		vfpu_randf(0.0f, 360.0f),
//		vfpu_randf(0.0f, 360.0f),
//		vfpu_randf(0.0f, 360.0f));
	#if (1)
	/* �V�������������߂� */
	vfpu_quaternion_from_euler512_int(&p9_qb,	/* �I�C���[�p�̕����ŉ�]����B��]�����͂����������������̏����ŉ�]������B */
	//	(int)vfpu_randf(0.0f, 512.0f),
	//	(int)vfpu_randf(0.0f, 512.0f),
	//	(int)vfpu_randf(0.0f, 512.0f));
		((vfpu_rand_8888(0, 255))&(512-1)),
		((vfpu_rand_8888(0, 255))&(512-1)),
		((vfpu_rand_8888(0, 255))&(512-1)));
	/* �����ɂ���(B�͈Ӗ����ς��) */
//	vfpu_quaternion_exp(&p9_qb, &p9_qb);
	vfpu_quaternion_ln(&p9_qb, &p9_qb); 		/* �������v�Z����B */
	#else
	/* �ȗ������đ�̂���Ȋ����ł��ǂ������B���̂܂܂��ƘI���ɒx���Ȃ肻��������)*/
//	vfpu_quaternion_identity(&p9_qb);	/* �P�ʃN�H�[�^�j�I���ŏ����� */
	p9_qb.x = (float)((vfpu_rand_8888(0, 255))&(512-1))/512.0f;//�Bx
	p9_qb.y = (float)((vfpu_rand_8888(0, 255))&(512-1))/512.0f;//�By
	p9_qb.z = (float)((vfpu_rand_8888(0, 255))&(512-1))/512.0f;//�Bz
	p9_qb.w = 1.0f;
	#endif
}
#endif

static void gu_init_vfpu(void)
{
	// vfpu has a random number generator,
	// and it needs to be seeded
	{
		u64 tick;
		sceRtcGetCurrentTick(&tick);
	//	vfpu_srand((u32)tick / 10000.0f);	����ł͎g���������������̂ŊԈႢ�B(vfpu_srand �� int(32bit�̐����l)�Œl��ݒ肷��̂� PSPSDK�̃T���v���͊ԈႢ�B����ł�float->int�ϊ��ŏ�ɒl��0�ɂȂ�)
		vfpu_srand((u32)tick ); 			/* vfpu_srand gets int. (����32bit�̂ݎg���Au32�͈̔͂Œl��ݒ肷��B���32bit�͎g��Ȃ��Ŏ̂Ă�B) */
	}
	/* �|���S�������_�����O����ꍇ�́A�p�[�X�����߂�B��p�����Ӑ}�I�ɕς�����ʔ�����������Ȃ��B */
	vfpu_perspective_matrix(&m4_projection3d, 60.0f, 480.0f/272.0f, 0.5f, 1000.0f);
	/* ������ */
	vfpu_identity_matrix(&m4_view); 		/* (���_�p�s���)�P�ʍs��ŏ���������B */
	vfpu_identity_matrix(&m4_model);		/* (���f���p�s���)�P�ʍs��ŏ���������B */
	#if (1==USE_CHOKUSETU_SETTEI_TEST)
	vfpu_quaternion_identity(&p9_test); 	/* �P�ʃN�H�[�^�j�I���ŏ����� */
	#endif

	// build a random starfield, using sphere to cartesian coordinate conversions
	// note how we use vfpu_randf here to generate random radian angles,
	// and how we use vfpu_rand_8888 to generate random colors for the star_cube_polygon_model
//	for (n=0; n<STARS_MAX; n++)
//	{
//		vfpu_sphere_to_cartesian(
//			vfpu_randf(0.0f, 6.28319f),
//			vfpu_randf(0.0f, 3.14159f),
//			1000.0f,
//			(ScePspFVector4 *)&(star_cube_polygon_model[n])
//		);
//		star_cube_polygon_model[n].color = vfpu_rand_8888(160, 255);
//	}

	// start off with 2 random rotations for the interpolation
//	vfpu_quaternion_from_euler(&p9_qa, vfpu_randf(0.0f, 360.0f), vfpu_randf(0.0f, 360.0f), vfpu_randf(0.0f, 360.0f));	vfpu_quaternion_ln(&p9_qa, &p9_qa);
//	vfpu_quaternion_from_euler(&p9_qb, vfpu_randf(0.0f, 360.0f), vfpu_randf(0.0f, 360.0f), vfpu_randf(0.0f, 360.0f));	vfpu_quaternion_ln(&p9_qb, &p9_qb);

	nnn = 0;
	test_auto_rot_aaaa();

//	val_x = 0;
//	val_y = 0;
//	val_z = 0;
//	val_w = 0;

}


static void gu_draw_bg_3D_test01(void)
{

	// when we reach the limit of our interpolation:
	//		copy qb to qa
	//		generate a new random quaternion in qb
	//		take the log of quaternion qb
	//		reset the time counter
	#if (1==USE_5SEC)/* �����J�E���^�l�̌v�Z */
	/*
	nnn: �����J�E���^�l: 0 ... 299 �܂ł̒l�B1�t���[����1�����B
	*/
	nnn++;
	if (nnn >= byou60(5) ) /* 300==(5[sec] x 60[frame])*/
	{
		nnn = 0;
		/* 300�J�E���g�Ɉ�x�����A�V�J�����ʒu�����߂Ȃ����B */
		test_auto_rot_aaaa();
	}
	#else
	/*
	nnn: �����J�E���^�l: 0 ... 255 �܂ł̒l�B1�t���[����1�����B
	*/
	nnn++;
	nnn &= 0xff;
	if (0==nnn) /* 256==(4.266...[sec] x 60[frame])*/
	{
		/* 256�J�E���g�Ɉ�x�����A�V�J�����ʒu�����߂Ȃ����B */
		test_auto_rot_aaaa();
	}
	#endif

#if 0
		{
			SceCtrlData pad;
			sceCtrlPeekBufferPositive(&pad, 1);
			if (pad.Buttons & PSP_CTRL_UP)			{		val_x-=0.01;			}
			if (pad.Buttons & PSP_CTRL_RIGHT)		{		val_y+=0.01;			}
			if (pad.Buttons & PSP_CTRL_DOWN)		{		val_x+=0.01;			}
			if (pad.Buttons & PSP_CTRL_LEFT)		{		val_y-=0.01;			}
		//
			if (pad.Buttons & PSP_CTRL_LTRIGGER)	{		running = 0;			}
			if (pad.Buttons & PSP_CTRL_RTRIGGER)	{		running = 0;			}
		//
			if (pad.Buttons & PSP_CTRL_TRIANGLE)	{		val_z-=0.01;			}
			if (pad.Buttons & PSP_CTRL_CIRCLE)		{		val_w+=0.01;			}
			if (pad.Buttons & PSP_CTRL_CROSS)		{		val_z+=0.01;			}
			if (pad.Buttons & PSP_CTRL_SQUARE)		{		val_w-=0.01;			}
		}
#endif

		// to build the view matrix:
		//		get the interpolated quaternion in qcam
		//		take the exp of qcam
		//		build a rotation matrix from qcam, place into view matrix
		//
		// here we use the function 0.5 + (cos(t) * 0.5) for our 'time' variable
		// this gives us the smooth ease in/ease out motion you see as the demo
		// interpolates between qa and qb
		//
		// this is why we must do 300-n, as cos(0) = 1, cos(PI) = -1, we want
		// the reverse, -1 to 1
	#if (1==TEST_AUTO_ROT)
		{
			/*
			t65536: �����J�E���^�l: 0 ... 65536 �܂ł̒l�B1�t���[����(65536/300)�����B
			*/
		#if (1==USE_5SEC)/* �����l�̌v�Z */
		//	float t = (nnn)*((float)(1.0f/300.0f));
			u32 t65536 = (nnn)*((u32)(65536.0/300.0));
		//	vfpu_quaternion_sample_linear(&p9_qcam, &p9_qa, &p9_qb, vfpu_ease_in_out(t));
			/* �����J�E���^�� 0 ... 300 �Ȃ̂ŁA�񓙕��l�̌v�Z�Ŏg����`���� 0 ... 65536 �֕ϊ�����B */
		#else
		//	float t = (nnn)*((float)(1.0f/256.0f));
			u32 t65536 = (((nnn)<<8)|(nnn));
		//	vfpu_quaternion_sample_linear(&p9_qcam, &p9_qa, &p9_qb, vfpu_ease_in_out(t));
			/* �����J�E���^�� 0 ... 255 �Ȃ̂ŁA�񓙕��l�̌v�Z�Ŏg����`���� 0 ... 65536 �֕ϊ�����B */
		#endif
		#if (1)/* �񓙕��l�̌v�Z */
		u32 i_rate65536;
		//	i_rate65536 = (int)(vfpu_ease_in_out(t)*256.0);
			i_rate65536 = (int)(vfpu_ease_in_out65536(t65536)); 	/* t65536�̓����J�E���^�l���g���A�񓙕��ɂȂ�悤�l���C������B
			�񓙕��̕����́uease_in_out�v����́A�n�߂ƍŌオ�������ŊԂ������Ƃ����z�������B */
		#endif
			/*
				�N�H�[�^�[�j�I���֐ݒ肷��B(�J�����ƍs��`�ƍs��a�֐ݒ肷��)
				�ݒ�̓��j�A�l(�����l)�Őݒ肷��̂ŁA�\�߉������Ƃ��āuease_in_out�v�Ŕ񓙕��l���v�Z���Ă����B
			*/
			vfpu_quaternion_sample_linear65536(&p9_qcam, &p9_qa, &p9_qb, i_rate65536);
		}
		vfpu_quaternion_exp(&p9_qcam, &p9_qcam);			/* �J�����p */
		vfpu_quaternion_to_matrix(&p9_qcam, &m4_view);		/* �J���������_�փZ�b�g */
	#endif

//
	#if 0
	p9_test.x = val_x;
	p9_test.y = val_y;
	p9_test.z = val_z;
	p9_test.w = val_w;
//
	vfpu_quaternion_to_matrix(&p9_test, &m4_projection3d);
	#endif
//
		// load our generated matrix data into the display list
		pgc_set_matrix_PROJECTION(	&m4_projection3d);	//sceGuSetMatrix(GU_PROJECTION, &m4_projection3d);		/* �����ϊ��p�}�g���N�X */
		pgc_set_matrix_VIEW(		&m4_view);			//sceGuSetMatrix(GU_VIEW,		&m4_view);				/* ���_�ϊ��r���[�p�}�g���N�X */
		pgc_set_matrix_MODEL(		&m4_model); 		//sceGuSetMatrix(GU_MODEL,		&m4_model); 			/* ���[���h���W�p�}�g���N�X */

	//	sceGuDrawArray(GU_POINTS,		GU_VERTEX_32BITF | GU_COLOR_8888 | GU_TRANSFORM_3D, STARS_MAX, /*0*/NULL, star_cube_polygon_model);
	//	sceGuDrawArray(GU_LINES,		GU_VERTEX_32BITF | GU_COLOR_8888 | GU_TRANSFORM_3D, STARS_MAX, /*0*/NULL, star_cube_polygon_model);
	//	sceGuDrawArray(GU_TRIANGLES,	GU_VERTEX_32BITF | GU_COLOR_8888 | GU_TRANSFORM_3D, STARS_MAX, /*0*/NULL, star_cube_polygon_model);
	//	sceGuDrawArray(GU_SPRITES,		GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, STARS_MAX, /*0*/NULL, star_cube_polygon_model);
	//	sceGumDrawArray(GU_SPRITES, 	GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, STARS_MAX, /*0*/NULL, star_cube_polygon_model);
//		sceGuDrawArray(GU_SPRITES,		GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, STARS_MAX, /*0*/NULL, star_cube_polygon_model);
//
		// draw cube
	//	sceGuFrontFace(GU_CCW);
//		sceGumDrawArray(GU_TRIANGLES,	GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 12*3, /*0*/NULL, cube_polygon_model);
	//	sceGuDrawArray(GU_TRIANGLES,	GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 12*3, /*0*/NULL, cube_polygon_model);
	//	sceGuDrawArray(GU_TRIANGLES,	GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 12*3, /*0*/NULL, cube_polygon_model);
	//	sceGuDrawArray(GU_TRIANGLES,	GU_TEXTURE_16BIT | GU_COLOR_5650 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, (12*3), /*0*/NULL, cube_polygon_model);
		sceGuDrawArray(GU_TRIANGLES,	GU_TEXTURE_8BIT | GU_COLOR_8888 | GU_VERTEX_8BIT | GU_TRANSFORM_3D, (12*3), /*0*/NULL, cube_polygon_model);
	//	sceGuFrontFace(GU_CW);
}

//#define GU_VERTEX_SHIFT(n)	((n)<<7)
//#define GU_VERTEX_8BIT		GU_VERTEX_SHIFT(1)
//#define GU_VERTEX_16BIT		GU_VERTEX_SHIFT(2)
//#define GU_VERTEX_32BITF		GU_VERTEX_SHIFT(3)
//#define GU_VERTEX_BITS		GU_VERTEX_SHIFT(3)
