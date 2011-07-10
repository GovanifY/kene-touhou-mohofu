
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
---------------------------------------------------------*/

/* オプションのデータ配置 */
//typedef struct
//{
#define PL_OPTION_DATA_angleCCW1024 			user_data00 		/* 下が０として左回り(逆回り)の角度 */
#define PL_OPTION_DATA_anime_wait				user_data01 		/* アニメーション速度のカウンタ */
#define PL_OPTION_DATA_opt_anime_add_id 		user_data02 		/* アニメーション方向の加算値 / オプションID / (レミリア & チルノ)x offset */	/* pos */
#define PL_OPTION_DATA_opt_shot_interval		user_data03 		/* オプションが発射するショットの更新間隔 */
//	/* レミリア & チルノ */
#define PL_OPTION_DATA_offset_x256				user_data04 		/* 円の半径と兼用 */
#define PL_OPTION_DATA_offset_y256				user_data05
#define PL_OPTION_DATA_slow_count				user_data06 		/* FORMATION_03: 減速時用(好きな位置に置けるよ) */
//廃止#define P L_OPTION_DATA_state2			user_data04
//	int st ate1;					/* 共通なので、ここには要らない */
//} PL_OPTION_DATA;

	/*---------------------------------------------------------
		各プレイヤーの性能差
	---------------------------------------------------------*/

#define PLAYERS8					(8)/*5*/


/*---------------------------------------------------------
	プレイヤー武器のあたり判定
---------------------------------------------------------*/

enum /*_player_state_*/
{
	PLAYER_WEAPON_TYPE_00_SHOT = 0, 	/* ショット系 */	/* 自弾が敵にあたった場合に自弾が消滅する */	/* 敵弾にあたると敵弾を消さない */
	PLAYER_WEAPON_TYPE_01_BOMB, 		/* ボム系 */		/* 自弾が敵にあたった場合に自弾が消滅しない */	/* 敵弾にあたると敵弾を消す */
};

/*---------------------------------------------------------
	player_shot.h
---------------------------------------------------------*/

enum /*_weapon_type_*/	/* 武器レベル */
{
	WEAPON_L0 = 0,		/* WEAPON_L0(P000-P008) */
	WEAPON_L1,			/* WEAPON_L1(P008-P015) */
	WEAPON_L2,			/* WEAPON_L2(P016-P031) */
	WEAPON_L3,			/* WEAPON_L3(P032-P063) */
	WEAPON_L4,			/* WEAPON_L4(P064-P127) */
	WEAPON_L5,			/* WEAPON_L5(P128)		*/
	WEAPON_L_MAX		/* 最大数 */
};

enum	/* _player_option_type_ */
{
	OPTION_C1 = 0,
	OPTION_C2,
	OPTION_C3,
	OPTION_C4,
};

/* cg.bomber_time と ->jyumyouで同時に動作 */
#define PD_BOMBER_JYUMYOU180					(180)		/* ボムの寿命==180[flame] */


/*---------------------------------------------------------
	player_local.h
---------------------------------------------------------*/

extern void player_weapon_colision_check(SPRITE *shot, int erase_shot_type);
extern SPRITE *search_teki_from_obj(void);

