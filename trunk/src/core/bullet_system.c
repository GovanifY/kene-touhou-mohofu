
#include "game_main.h"

/*---------------------------------------------------------
	�G�e�Ǘ��V�X�e��
	-------------------------------------------------------
	KETM�̃X�v���C�g�V�X�e����700-800�e���炢�Ȃ���ɖ��Ȃ����A
	1500-2000�e�Ƃ�����������ƁA�x���Ȃ肷���đ��x�I�ɃQ�[���ɂȂ�Ȃ����������B
	(�`�擙�������Ȃ��Ă��A���������ő��x����������Ă��܂�)
	Gu�X�v���C�g�������ł́A�s�\���Ȃ̂ŁA�V����
	�ėp���͖��������x�͑����G�e�Ǘ��V�X�e����ǉ����邱�Ƃ�����B
	-------------------------------------------------------
	�� �����ł�鎖(�\��)�F
	1.�r�����b�g�v�[�������ɂ��G�e�Ǘ��V�X�e���B
	2.����(�R�A)�ƓG�e�̂����蔻��(�G�e�O���C�Y����A�������S����)
	�� �����ł��Ȃ����F
	3.Gu�`��͊O���̓G�e�`���p���[�`���ōs���B
---------------------------------------------------------*/

/* �r�����b�g�v�[���̎��Ԃ͂����̌Œ�z�� */
/*extern*/ TGameSprite bullet_pool[MAX_POOL_BULLET];

static int bullet_system_regist_objects;/* �o�^������ */
static int regist_locate;/* ���̒e���o�^�o�������Ȉʒu */

/*---------------------------------------------------------
	�G�e�o�^
---------------------------------------------------------*/
/* �ő�o�^�� */
#define REGIST_MAX_LIMIT (MAX_POOL_BULLET-10)
int bullet_system_regist_object(int image_resource_num)
{
	if ((REGIST_MAX_LIMIT)<bullet_system_regist_objects)
	{
		return (0); 				/* �o�^�ł��Ȃ� */
	}
//
	u32 i;
	i = regist_locate;
	i++;
bbb_my_loop:
	for (; i<MAX_POOL_BULLET;)
	{
		if (0==bullet_pool[i].used)
		{
			goto aaa_my_regist;
		}
		i++;
	}
	i=0;
	goto bbb_my_loop;
/* �o�^����� */
aaa_my_regist:
	regist_locate = i;
	bullet_system_regist_objects++; 	/* �o�^������ */
	return (1+i);			/* �o�^�����ʒu */
}

/*---------------------------------------------------------
	�V�X�e������������
---------------------------------------------------------*/

void bullet_system_init(void)
{
	regist_locate					= 0;
	bullet_system_regist_objects	= 0;
	u32 i;
	for (i=0; i<MAX_POOL_BULLET; i++)
	{
		bullet_pool[i].used 		= 0;
	}
}

/*---------------------------------------------------------
	�V�X�e���I������
---------------------------------------------------------*/

void bullet_system_exit(void)
{
}
