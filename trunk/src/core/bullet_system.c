
#include "game_main.h"

/*---------------------------------------------------------
	敵弾管理システム
	-------------------------------------------------------
	KETMのスプライトシステムは700-800弾ぐらいなら特に問題ないが、
	1500-2000弾とか数が増えると、遅くなりすぎて速度的にゲームにならない事が判明。
	(描画等何もしなくても、そこだけで速度を消費しつくしてしまう)
	Guスプライト化だけでは、不十分なので、新しく
	汎用性は無いが速度は速い敵弾管理システムを追加することを決定。
	-------------------------------------------------------
	★ ここでやる事(予定)：
	1.ビュレットプール方式による敵弾管理システム。
	2.自分(コア)と敵弾のあたり判定(敵弾グレイズ判定、自分死亡判定)
	★ ここでやらない事：
	3.Gu描画は外部の敵弾描画専用ルーチンで行う。
---------------------------------------------------------*/

/* ビュレットプールの実態はただの固定配列 */
/*extern*/ TGameSprite bullet_pool[MAX_POOL_BULLET];

static int bullet_system_regist_objects;/* 登録した数 */
static int regist_locate;/* 次の弾が登録出来そうな位置 */

/*---------------------------------------------------------
	敵弾登録
---------------------------------------------------------*/
/* 最大登録数 */
#define REGIST_MAX_LIMIT (MAX_POOL_BULLET-10)
int bullet_system_regist_object(int image_resource_num)
{
	if ((REGIST_MAX_LIMIT)<bullet_system_regist_objects)
	{
		return (0); 				/* 登録できない */
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
/* 登録するよ */
aaa_my_regist:
	regist_locate = i;
	bullet_system_regist_objects++; 	/* 登録した数 */
	return (1+i);			/* 登録した位置 */
}

/*---------------------------------------------------------
	システム初期化処理
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
	システム終了処理
---------------------------------------------------------*/

void bullet_system_exit(void)
{
}
