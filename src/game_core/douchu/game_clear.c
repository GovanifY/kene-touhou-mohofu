
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	エンディング後の終了判定
	-------------------------------------------------------
---------------------------------------------------------*/
extern void ini_file_save(void);

#if (0)/*(旧パスワード機能。メモ)*/
	/*static*/ char str_pass_word[32];/*20*/
static void game_clear_set_password(void)
{
	#if (1)
	/******自分でコンパイルする人へ******
	このままだと配布バイナリと違うので
	文字列"無駄"を適当に変えといて下さい。
	************************************/
	/* チルド (chilled) は、冷却されること。
	「冷やす」を意味する英語の動詞チル (chill) の過去分詞である。
	何で chillno じゃなくて cirno なんだろな？ */
	#define YUYUKO_PASSWORD 	"TABERARENAINO9"
	#define CIRNO_PASSWORD		"CIRNO9PASSWORD"
	#define REMILIA_PASSWORD	"RUMIAISMYBRIDE"
	if (0 < difficulty)/* NORMAL 以上でクリアせよ */
	{
		if (0==tiny_strcmp(str_pass_word, YUYUKO_PASSWORD)) /* 既にクリアしてたら */
		{
		/*	strcpy(str_pass_word, FURAN_PASSWORD)*/;	/* ??? ＆ 幽々子 ＆ チルノ ＆ レミリア 開放 */
		}
		else
		if (0==tiny_strcmp(str_pass_word, CIRNO_PASSWORD))	/* 既にクリアしてたら */
		{
			strcpy(str_pass_word, YUYUKO_PASSWORD); 	/* 幽々子 ＆ チルノ ＆ レミリア 開放 */
		}
		else
		if (0==tiny_strcmp(str_pass_word, REMILIA_PASSWORD)) /* 既にクリアしてたら */
		{
			strcpy(str_pass_word, CIRNO_PASSWORD);		/* チルノ ＆ レミリア 開放 */
		}
		else
		{
			strcpy(str_pass_word, REMILIA_PASSWORD);	/* レミリア 開放 */
		}
	}
	#endif
	{
		ini_file_save();
	}
}
#else
	#define game_clear_set_password ini_file_save
#endif

extern void player_loop_quit(void);
global void game_command_04_game_all_clear(GAME_COMMAND *l)
{
	#if 0
	/* なんか知らんが、Lunaticでちょっとチェックしたら、オーバーフローするｗ。 */
	player_dummy_add_score(adjust_score_by_difficulty((
				(cg.zanki * score(3000000)) +	/* 残機   x 300万 */
				(cg.bombs * score(1000000)) 	/* 残ボム x 100万 */
			)));
	#else
	/* なんか知らんが、オーバーフローするので、個別に足してみる。 */
	player_dummy_add_score(adjust_score_by_difficulty(( (((u32)cg.zanki) * score(3000000)) ))); 	/* 残機   x 300万 */
	player_dummy_add_score(adjust_score_by_difficulty(( (((u32)cg.bombs) * score(1000000)) ))); 	/* 残ボム x 100万 */
	#endif
	/* 清算して消える */
	cg.zanki = 0;
	cg.bombs = 0;
	//
	game_clear_set_password();
	#if 1/* この２つのセットで自動的に終了(GAME OVER)する */
	cg.game_now_max_continue = 1;	/* コンティニューさせない */
	player_loop_quit();
	#endif
}
