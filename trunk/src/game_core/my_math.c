
#include "game_main.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
psp では、 at an2(), si n(), sq rt() 等の超越関数系命令は、
psp の MIPS CPU 内 のコプロセッサが処理をする。
コプロセッサ変換処理があちこちにあると、非常にパフォーマンスが悪いので、
一ヶ所に纏めた方が実行速度は遥かに速い。
(CPU内蔵の命令キャッシュに乗るために実行速度が速くなる)

参考:TECH I Vol.39 MIPSプロセッサ入門	http://www.cqpub.co.jp/interface/TechI/Vol39/
---------------------------------------------------------*/
#include "111_my_file.h"/*(my_file_common_name)*/

#include "kanji_system.h"

//#include "psp_vfpu.h"
//#include <malloc.h>

/*---------------------------------------------------------
	将来的にリプレイに対応できるように、
	自前の乱数ルーチンを用意しとく。
	(かなりテキトーですが)
---------------------------------------------------------*/

static int rnd;
global int ra_nd(void)
{
	rnd = (rnd * 8513/*multiplier*/) + (179)/*addend*/;
	rnd = ((rnd) ^ (rnd>>8));
	return (rnd);
}


/*---------------------------------------------------------
	乱数初期値設定(将来的にリプレイに対応できるように、
	面の始めに固定値を設定する)
---------------------------------------------------------*/

global void set_rnd_seed(int set_seed)
{
	rnd = set_seed;
}


/*---------------------------------------------------------
	キー入力関連の処理(汎用)
---------------------------------------------------------*/

global u32 pad_config[KINOU_08_WARIATE_MAX];
global PSP_PAD_GLOBAL_CLASS psp_pad;/*(r34)*/

static int	use_analog; 	/* アナログ使用フラグ(0:使用しない、1:使用する) */

extern void save_screen_shot(void);
global void psp_pad_init(void)
{
	#if (0)
	/*
	初期化には時間がかかる模様。
	このタイミングでは動作しない。
	(初期化してすぐに読もうとしても一番始めの入力が読めない、少し時間が経てば読める)
 */
	/* --- 入力装置の初期設定 */
//	sceCtrlSetSamplingCycle(0);/*うまくいかない*/
//	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);/* うまくいかない */
	#endif
	/* boot_check_analog */
	use_analog = 1;/* アナログ使用フラグ(0:使用しない、1:使用する) */
	{
		SceCtrlData pad;
		sceCtrlReadBufferPositive(&pad, 1);
		/* 起動時にアナログ入力があった場合、故障していると判断しアナログ入力無効。 */
		if (pad.Lx < 64/*70*/)			{	use_analog = 0; 	}
		else if (pad.Lx > 192/*185*/)	{	use_analog = 0; 	}
		//
		if (pad.Ly < 64/*70*/)			{	use_analog = 0; 	}
		else if (pad.Ly > 192/*185*/)	{	use_analog = 0; 	}
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
	/* 現在無効(r99) 将来的にはv_sync取るが、主にSDLを色々しないと何ともならない。*/
	{
		sceCtrlPeekBufferPositive(&pad, 1);
		/* 欠点：vblankをとらない限り、新しくpollingした保証がない。
			vblankをとらない場合は、何らかの時間待ち制御が必要。
	 */
		static u32 last_time = 0;/* 最終時間(前回の時間) */
		u32 now_time;/* 現在の時間(今回の時間) */
		now_time = pad.TimeStamp;/* 現在の時間(今回の時間)を取得 */
		/* 時間カウンタがオーバーフローした場合の修正処理 */
		if (now_time < last_time)	/* 時間カウンタがオーバーフローした場合 */
		{
			now_time	+= 16666;
			last_time	+= 16666;/* 必ずオーバーフローして小さな値になる筈 */
			#if (0)
			if (now_time < last_time)	/* ありえない筈 */
			{	/* 念の為 */
				now_time	= 16666;
				last_time	= 0;
			}
			#endif
		}
		/* ここで必ず (now_time > last_time) になってる筈。 */
		int l_aaa;
		l_aaa = 0;//l_aaa++;	/*1*/
		do
		{
			l_aaa++;	/* 必ず通るので最低でも 1 になる。 */
			now_time -= 16666;	/* 1 [frame]分の時間を引く */
		}
		while ( 16666 < (now_time-last_time) ); 	/* 差分が 1 [frame]以内 でなければ繰り返す。 */
		//
		if (l_aaa < 2) /* 16666.666 ...  == 1fs or less? */ 	/* 1 [frame]以内に描画できれば */
		{
			sceDisplayWaitVblankStart();	/* 1 [frame]以内の場合のみ v_sync取る。 */
		}
		last_time = pad.TimeStamp;/* 現在の時間を最終時間として保存 */
	}
	#else
	/* こちらの方式では v-sync 取るの無理っぽい(?)。 */
	/* profile 採ると入力が不自然に遅いんだけど... */
	/**/
	sceCtrlReadBufferPositive(&pad, 1); 	/* sceCtrlReadBufferPositiveの場合はここで何もしないで1/60秒待つ可能性がある。 */
	/* 欠点：1/60秒以内で動作し、VSYNC取るゲームを作成するのが不可能になる。
		こちらの方式のままでは事実上、弾が多いシューティングを作るのは無理がある。
		入力を待つので(ゲームが)遅くなる。
		VSYNC取りつつ1/60秒で動作するゲームが作成不可能になる。
		(sceCtrlReadBufferPositive();をsceDisplayWaitVblankStart();を動作させるだけで、
		最悪の場合 2フレーム==約2/60秒==1/30秒 時間がかかる為)
		(1/60秒以内[60fps]でゲームを作っても、polling待ちで1/60秒でゲームが動かないで30fpsになっちゃう事がある。
		机上の空論で実際そうなる事は無いが、全部非同期なので最悪ケースで考えれば正しい。)
 */
	#endif
/*
	参考: http://emuhani.seesaa.net/article/127733904.html
	参考: http://forums.qj.net/zx-81s-homebrew-releases/146232-development-question.html
	sceCtrlReadBufferPositive と sceCtrlPeekBufferPositive の違いについて。
*/
	#if (1==USE_KEY_CONFIG)
		/* キーコンフィグありの場合、「PSPのデジタル入力」と「ゲーム動作用のデジタル入力」は扱いが異なる */
		u32 pad_data;
	#else
		/* キーコンフィグなしの場合、「PSPのデジタル入力」を「ゲーム動作用のデジタル入力」にそのまま使う */
		#define pad_data psp_pad.pad_data
	#endif /* (1==USE_KEY_CONFIG) */
	pad_data = pad.Buttons;
	{
		psp_pad.analog_absolute_value_x = 0;
		psp_pad.analog_absolute_value_y = 0;
		/* 標準アナログキー機能 */
	//	if (1==use_analog)
		if (0 != use_analog)
		{
			/* PSPのアナログ入力はデジタル入力へ変換、アナログ量は中心を削除し256固定小数点形式へ補正 */
			if (pad.Lx < 64/*70*/)			{	pad_data |= PSP_CTRL_LEFT;		psp_pad.analog_absolute_value_x = ((64-pad.Lx)<<2); 	}
			else if (pad.Lx > 192/*185*/)	{	pad_data |= PSP_CTRL_RIGHT; 	psp_pad.analog_absolute_value_x = ((pad.Lx-192)<<2);	}
			//
			if (pad.Ly < 64/*70*/)			{	pad_data |= PSP_CTRL_UP;		psp_pad.analog_absolute_value_y = ((64-pad.Ly)<<2); 	}
			else if (pad.Ly > 192/*185*/)	{	pad_data |= PSP_CTRL_DOWN;		psp_pad.analog_absolute_value_y = ((pad.Ly-192)<<2);	}
		}
	}
	#if (1==USE_KEY_CONFIG)
	/* 上下左右のキーコンフィグは現在無効というか設定値を無視。(アナログ入力の都合) */
	/* キーコンフィグありの場合でも、上下左右は(アナログ入力の)都合によりPSPのデジタル入力をそのまま使う */
//	psp_pad.pad_data = 0;
//	psp_pad.pad_data |= (pad_data & (PSP_CTRL_UP|PSP_CTRL_RIGHT|PSP_CTRL_DOWN|PSP_CTRL_LEFT));
	psp_pad.pad_data	= (pad_data & (PSP_CTRL_UP|PSP_CTRL_RIGHT|PSP_CTRL_DOWN|PSP_CTRL_LEFT/*|PSP_CTRL_SELECT|PSP_CTRL_START*/));/* 上下左右/SELECT/STARTは都合によりキーコンフィグなし */
	/* PSPのデジタル入力からキーコンフィグを考慮して入力値を決める */
	if (pad_data & PSP_CTRL_SELECT) 	{	psp_pad.pad_data |= pad_config[KINOU_00_SELECT];		}	//	if (keyboard[KINOU_01_SELECT])		{psp_pad.pad_data |= (PSP_KEY_SELECT);}
	if (pad_data & PSP_CTRL_START)		{	psp_pad.pad_data |= pad_config[KINOU_01_START]; 		}	//	if (keyboard[KINOU_02_PAUSE])		{psp_pad.pad_data |= (PSP_KEY_PAUSE);}
//
	if (pad_data & PSP_CTRL_LTRIGGER)	{	psp_pad.pad_data |= pad_config[KINOU_02_LTRIGGER];		}	//	if (keyboard[KINOU_07_SNAP_SHOT])	{psp_pad.pad_data |= (PSP_KEY_SNAP_SHOT);}
	if (pad_data & PSP_CTRL_RTRIGGER)	{	psp_pad.pad_data |= pad_config[KINOU_03_RTRIGGER];		}	//	if (keyboard[KINOU_08_SYSTEM])		{psp_pad.pad_data |= (PSP_KEY_SYSTEM);}
	if (pad_data & PSP_CTRL_TRIANGLE)	{	psp_pad.pad_data |= pad_config[KINOU_04_TRIANGLE];		}	//	if (keyboard[KINOU_09_SLOW])		{psp_pad.pad_data |= (PSP_KEY_SLOW);}
	if (pad_data & PSP_CTRL_CIRCLE) 	{	psp_pad.pad_data |= pad_config[KINOU_05_CIRCLE];		}	//	if (keyboard[KINOU_10_OPTION])		{psp_pad.pad_data |= (PSP_KEY_OPTION);}
	if (pad_data & PSP_CTRL_CROSS)		{	psp_pad.pad_data |= pad_config[KINOU_06_CROSS]; 		}	//	if (keyboard[KINOU_11_SHOT])		{psp_pad.pad_data |= (PSP_KEY_SHOT_OK);}
	if (pad_data & PSP_CTRL_SQUARE) 	{	psp_pad.pad_data |= pad_config[KINOU_07_SQUARE];		}	//	if (keyboard[KINOU_12_BOMB])		{psp_pad.pad_data |= (PSP_KEY_BOMB_CANCEL);}
	#endif /* (1==USE_KEY_CONFIG) */
	/* スクリーンショット機能。 */
	// keypollに入れると何故かうまくいかなかったのでこっちに場所を変更。
	if (psp_pad.pad_data & PSP_KEY_SNAP_SHOT)	{	save_screen_shot(); 	 }/*keyboard[KINOU_07_SNAP_SHOT]*/

	/* アナログサポート機能 */
//	if (1==use_analog)
	{
		/* デジタルよりアナログ優先 */
		if (0 == (psp_pad.analog_absolute_value_x+psp_pad.analog_absolute_value_y) )
		/*アナログ押してないと思われる場合(アナログ押してる場合はアナログ量をそのまま使う)*/
		{
			/* デジタルよりアナログ量を算出 */
			#if (0)
				 if (pad_data & PSP_CTRL_UP)				{	psp_pad.analog_absolute_value_y 	= 256;	}
			else if (pad_data & PSP_CTRL_DOWN)				{	psp_pad.analog_absolute_value_y 	= 256;	}
				 if (pad_data & PSP_CTRL_RIGHT) 			{	psp_pad.analog_absolute_value_x 	= 256;	}
			else if (pad_data & PSP_CTRL_LEFT)				{	psp_pad.analog_absolute_value_x 	= 256;	}
			#else
			if (pad_data & (PSP_CTRL_UP|PSP_CTRL_DOWN)) 	{	psp_pad.analog_absolute_value_y 	= 256;	}	/* 上下のアナログ量 */
			if (pad_data & (PSP_CTRL_RIGHT|PSP_CTRL_LEFT))	{	psp_pad.analog_absolute_value_x 	= 256;	}	/* 左右のアナログ量 */
			#endif
		}
	}
}
//		 if (keyboard[KINOU_06_LEFT])	{psp_pad.pad_data |= PSP_CTRL_LEFT; 	/*direction=-1;*/		}
//	else if (keyboard[KINOU_04_RIGHT])	{psp_pad.pad_data |= PSP_CTRL_RIGHT;	/*direction=1;*/		}
//		 if (keyboard[KINOU_03_UP]) 	{psp_pad.pad_data |= PSP_CTRL_UP;		}
//	else if (keyboard[KINOU_05_DOWN])	{psp_pad.pad_data |= PSP_CTRL_DOWN; 	}



/*---------------------------------------------------------
	システムの基礎部分
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
		//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
			"memory: メモリを %d バイト確保" "\\n"
			"しようと思いましたが足りません。" "\\n"
			"必要無いメモリを解放する事で、" "\\n"
			"メモリ確保を再挑戦してみます。", size);
		#endif
		imglist_garbagecollect();
		ptr = malloc(size);
		if (NULL == ptr)
		{
			psp_fatal_error( (char*)
			//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
				"memory: メモリが足りません。" );
		}
		#else
		psp_fatal_error( (char*)
		//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
			"memory: メモリを %d バイト確保" "\\n"
			"しようと思いましたが足りませんでした。", size);
		#endif /*(1==USE_KETM_IMAGE_CHACHE)*/
	}
	memset(ptr, 0, size);/* calloc()風に、必ず0クリアーするように変更 */
	return (ptr);
}


#if (1==USE_KETM_IMAGE_CHACHE)
/*---------------------------------------------------------
	画像キャッシュ関連
	-------------------------------------------------------
	同じ画像を複数読み込んだ場合にメモリが無駄になりもったいない。
	そこで同じ画像を読み込んだ場合には、実際には読み込まないで、
	前に読み込んだ画像と同じものを使う。
	トータルでいくつ同じ画像を読み込んだかは、それぞれの画像キャッシュの参照数でわかる。
---------------------------------------------------------*/

typedef struct _imglist
{
	SDL_Surface *img;			/* 読み込んだサーフェイスを保持 */
	struct _imglist *next;		/* 次の画像へのリストポインタ */
	int refcount;				/* 同じ画像の参照数 */
	char name[128]; 			/* 名前 */	/* 128==4*32 (pspの構造上32の倍数で指定) */ 	/*256*/
} MY_IMAGE_LIST;

/* 画像キャッシュのリスト */
static MY_IMAGE_LIST *my_image_list /*= NULL*/;/* ←この初期化処理はpspでは正常に動作しないかも？ */

global void init_imglist(void)
{
	my_image_list = NULL;
}


/*---------------------------------------------------------
	画像キャッシュに新規画像を追加
---------------------------------------------------------*/

static void imglist_add_by_file_name(SDL_Surface *img_surface, char *name)
{
	MY_IMAGE_LIST		*new_list;
	new_list			= my_calloc(sizeof(MY_IMAGE_LIST));
	strcpy(new_list->name, name);
	new_list->refcount	= 1;
	new_list->img		= img_surface;
	if (NULL == my_image_list)		/* 先頭の場合 */
	{
		new_list->next	= NULL;
	}
	else							/* 先頭以外の場合 */
	{
		new_list->next	= my_image_list;
	}
	my_image_list		= new_list;
}


/*---------------------------------------------------------
	画像キャッシュに同じファイル名がないか検索
---------------------------------------------------------*/

static SDL_Surface *imglist_search_by_file_name(char *name)
{
	MY_IMAGE_LIST		*tmp_list;
	tmp_list			= my_image_list;/* 画像キャッシュリストの先頭から調べる。 */
	while (NULL != tmp_list)
	{
		if (0 == tiny_strcmp(name, tmp_list->name))
		{
			tmp_list->refcount++;
			return (tmp_list->img);
		}
		tmp_list		= tmp_list->next;	/* 次 */
	}
	return (NULL);
}


/*---------------------------------------------------------
	画像キャッシュを開放
	メモリが足りなくなったので画像キャッシュ内で使ってないものを開放
---------------------------------------------------------*/

static void imglist_garbagecollect(void)
{
	MY_IMAGE_LIST	*tmp_list;		/* ターゲット */
	MY_IMAGE_LIST	*prev_list; 	/* 前 */
	MY_IMAGE_LIST	*next_list; 	/* 次 */
	tmp_list		= my_image_list;
	prev_list		= NULL;
	next_list		= NULL;
	while (NULL != tmp_list)
	{
		next_list	= tmp_list->next;
		if (0 == tmp_list->refcount)	/* 画像は使ってない */
		{
			/* 先頭の場合は以前が無いので特別処理 */
			if (NULL == prev_list)		/* 先頭の場合 */
			{
				my_image_list		= next_list;/* 先頭を修正 */
			}
			else						/* 先頭以外の場合 */
			{
				prev_list->next 	= next_list;/* 直前に連結 */
			}
		//	SDL_FreeSurface(tmp_list->img);
			free (tmp_list);			/* ターゲットを開放 */
		}
		else	/* 画像は使用中 */
		{
			prev_list	= tmp_list; 	/* 調べた分をとばす */
		}
		tmp_list		= next_list;	/* 次 */
	}
}


/*---------------------------------------------------------

---------------------------------------------------------*/

//global SDL_Surface *load_chache_bmp2(char *set_filename)
global SDL_Surface *load_chache_bmp(void)/*char *file_name*/
{
//	char file_name[128];	/* 128==4*32 (pspの構造上32の倍数で指定) */ 	/*64 50*/
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
		//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
			"load chache bmp: 画像がありません。" "\\n"
			"%s", my_file_common_name);
	}
//	if (use_alpha)
//	{
//		s2 = SDL_DisplayFormatAlpha(s1);/* サーフェスを表示フォーマットに変換する。 */	// α値を持ったsurface
//	}
//	else
	{
		s2 = SDL_DisplayFormat(s1);/* サーフェスを表示フォーマットに変換する。 */
	}
	if (NULL == s2)
	{
		psp_fatal_error(/*ERR_FATAL,*/ (char*)
		//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
			"load chache bmp: 画像変換するメモリが" "\\n"
			"足りません。" "\\n"
			"%s", my_file_common_name);
	}
	SDL_FreeSurface(s1);
	s1 = NULL;
	imglist_add_by_file_name(s2, my_file_common_name);
	return (s2);
}


/*---------------------------------------------------------
	img_surface が MY_IMAGE_LIST内にあるか確認をし、
	画像が見つかった場合、画像キャッシュリストの参照数を一つ減らす。
---------------------------------------------------------*/

global void unloadbmp_by_surface(SDL_Surface *img_surface)
{
	MY_IMAGE_LIST	*tmp_list;
	tmp_list		= my_image_list;/* 画像キャッシュリストの先頭 */
	while (NULL != tmp_list)
	{
		if (img_surface == tmp_list->img)	/* 画像が見つかった */
		{
			if (0 == tmp_list->refcount)	/* 画像キャッシュリストの参照数 */
			{
				/* ロードしてないのに開放。 */
				psp_fatal_error( (char*)
				//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
					"chache: 画像を読みこんでいないのに、" "\\n"
					"解放しようとしました。" "\\n"
					"%s", tmp_list->name);
			}
			else
			{
				tmp_list->refcount--;		/* 一つ減らす。 */
			}
			return; 	/* 正常終了 */
		}
		tmp_list = tmp_list->next;			/* 次 */
	}
	/* 見つからない。 */
	psp_fatal_error( (char*)
	//	"0123456789012345678901234567890123456789"	// 半角40字"最大表示文字数"
		"chache: プログラムミス。" "\\n"
		"解放する画像が、見つかりません。");
//	return; 	/* 異常終了 */
}
#endif /*(1==USE_KETM_IMAGE_CHACHE)*/


/*---------------------------------------------------------
	バックバッファに画像キャッシュを使いながら、背景をロード。
---------------------------------------------------------*/
/*---------------------------------------------------------
	ファイル名で指定
---------------------------------------------------------*/
global void load_SDL_bg_file_name(void)/*char *file_name*/
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
	unloadbmp_by_surface(loadpic);	// キャッシュに入ってるのでNULLに出来ない。loadpic = NULL;
	#else
	SDL_FreeSurface(loadpic);	// キャッシュに入ってるのでNULLに出来ない。
	loadpic = NULL;
	#endif
}


/*---------------------------------------------------------
	ファイル番号で指定
---------------------------------------------------------*/
global void load_SDL_bg(int bg_type_number)
{
	static const char *const_aaa_str[(BG_TYPE_99_MAX_HAIKEI)] =
	{
		DIRECTRY_NAME_DATA_STR"/bg/""title_bg.png",
		DIRECTRY_NAME_DATA_STR"/bg/""name_regist.png",
		DIRECTRY_NAME_DATA_STR"/bg/""key_config.png",
		DIRECTRY_NAME_DATA_STR"/bg/""music_room.png",
		DIRECTRY_NAME_DATA_STR"/bg/""loading.png",
	};
	#if 1
//	char file_name[128];	/* 128==4*32 (pspの構造上32の倍数で指定) */ 	/*64 50*/
	strcpy(my_file_common_name, (char *)const_aaa_str[bg_type_number]);
	load_SDL_bg_file_name();/*file_name*/
	#else
	SDL_Surface *loadpic	= load_chache_bmp2((char *)const_aaa_str[bg_type_number]);//, 0, 0/*1*/);
//	psp_clear_screen();
//	SDL_SetAlpha(loadpic, SDL_SRCALPHA, 255);
//	PSPL_UpperBlit(loadpic, NULL, cb.sdl_screen[SDL_00_VIEW_SCREEN], NULL);
	PSPL_UpperBlit(loadpic, NULL, cb.sdl_screen[SDL_01_BACK_SCREEN], NULL);
	unloadbmp_by_surface(loadpic);	// キャッシュに入ってるのでNULLに出来ない。loadpic = NULL;
	#endif
}


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
