#ifndef _BULLET_SYSTEM_H_
#define _BULLET_SYSTEM_H_

/*---------------------------------------------------------
	�G�e�Ǘ��V�X�e��
---------------------------------------------------------*/

/* MAX_POOL_BULLET: 2��n��̕K�v����(1024�Ƃ�2048�Ƃ�) */
#define MAX_POOL_BULLET 1024
extern TGameSprite bullet_pool[MAX_POOL_BULLET];


extern void bullet_system_init(void);
extern void bullet_system_exit(void);
#endif /* _BULLET_SYSTEM_H_ */
