
/*---------------------------------------------------------
	東方模倣風	～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	このファイルは直接インクルードしません。
	"game_main.h" からのみ間接的にインクルードします。
---------------------------------------------------------*/

#ifndef _SUPPORT_H_
#define _SUPPORT_H_

/*---------------------------------------------------------
	サポート設定(一番最初にインクルード)
	本システムを使う上での基本的なマクロ等のサポート関連
---------------------------------------------------------*/

// PSPSDK(standard)

#include <psptypes.h>
#include <pspaudio.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspgu.h>
//#include <pspgum.h>/* gumはguより後にインクルード */
#include <pspiofilemgr.h>
#include <pspkernel.h>
#include <psppower.h>
#include <psprtc.h>

// C(standard)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>/*calloc*/

// PSPSDK(debug)

#include <pspdebug.h>
#ifdef ENABLE_PROFILE
	#include <pspprof.h>
#endif

// PSPSDK(optional)

//#include <pspsdk.h>
//#include <psputility.h>
//#include <pspmoduleinfo.h>
//#include <pspnet_apctl.h>
//#include <pspnet_inet.h>
//#include <pspnet_resolver.h>
//#include <netinet/in.h>
//#include "net_io.h"
//#include <pspuser.h>

// C(optional)

#include <unistd.h>
#include <stdarg.h>
#include <dirent.h>
#include <ctype.h>
#include <math.h>/*sin(),atan2()*/
//#include <signal.h>
//#include <sys/stat.h>


#ifdef ENABLE_PSP
	//# /* カスタムライブラリを使う */
	#include <SDL/SDL.h>//#include "SDL.h"
	#include "SDL_image.h"
	#include <SDL/SDL_mixer.h>//#include "SDL_mixer.h"
#else
	//# /* 標準ライブラリを使う */
	#include <SDL/SDL.h>
	#include <SDL/SDL_image.h>
	#include <SDL/SDL_mixer.h>/*#include "SDL_mixer.h"*/
#endif

	//
	#ifndef UINT8
		#define UINT8 unsigned char
	#endif
	#ifndef UINT16
		#define UINT16 unsigned short
	#endif
	#ifndef UINT32
		#define UINT32 unsigned int
	#endif
	//
	#ifndef INT8
		#define INT8 signed char
	#endif
	#ifndef INT16
		#define INT16 signed short
	#endif
	#ifndef INT32
		#define INT32 signed int
	#endif
	//
	#ifndef offs_t
		#define offs_t unsigned int
	#endif

//	#ifndef TRUE
//		#define TRUE 1
//	#endif
//
//	#ifndef FALSE
//		#define FALSE 0
//	#endif

	#ifndef global
		/* static なのか global なのか 明示する。 */
		#define global /**/
	#endif

	#define my_TRUE 	(1)/**/
	#define my_FALSE	(0)/**/

	/* デバッグ中無効値、常に0. */
	#define TEST000 	(0)

#if (1)
/* RGBA5551:
	psp には RGBA5551 (GU_PSM_5551) モードがある。ここの A の1ビットは、言い換えればハードウェアーのcolor-key機能。
//
	しかし SDL では 5551 モードにしても A の 1 ビットは、無視されて使う事が出来ない。(つまり強制的にRGBA5550となる)
	(直接サーフェイスをいじった場合を除いて)「SDLのみ」(の描画を)使用している限りでは「ハードウェアーのcolor-key機能」は使用出来ない。
	 SDL では 5551 モードの場合、color-keyを別に設定し、「ソフトウェアーのcolor-key機能」を使用する。
//
	間違い//一方、PSPSDK では「ハードウェアーのcolor-key機能」が使える。PSPSDKではalpha-bitが使える。
	間違い//PSPSDK では「ソフトウェアーのcolor-key機能」というものは存在しない為、逆に言えば、
	間違い//必ずここでcolor-keyを設定しなければならない。(PSPSDKではalpha-bit==抜き色==color-key)
//
	間違い//つまり、PSPSDK では RGBA8888(GU_PSM_8888) や RGBA5551(GU_PSM_5551) モードではcolor-key機能があるが、
	間違い//RGBA5650(GU_PSM_5650) モードではcolor-key機能が無い｡
	間違い//(ハードウェアー/ソフトウェアーに関わらず、透明度は設定できるがcolor-key(抜き色)を実現させる方法がない)

	PSPSDK では sceGuBlendFunc(); で color-key が設定できるみたい。

void sceGuBlendFunc(int op, int src, int dest, unsigned int srcfix, unsigned int destfix);

備考:
ブレンド関数を設定します。DirectXで謂うところの「ブレンドファクター」に該当します。
描画先に既に存在する色と書き込む予定の色を比較、演算して実際に書き込む色を決定します。

opに設定できる定数は以下の通り

GU_ADD
GU_SUBTRACT
GU_REVERSE_SUBTRACT
GU_MIN
GU_MAX
GU_ABS
srcとdestに設定できる定数は以下の通り

GU_SRC_COLOR
GU_ONE_MINUS_SRC_COLOR
GU_SRC_ALPHA
GU_ONE_MINUS_SRC_ALPHA
GU_DST_ALPHA
GU_ONE_MINUS_DST_ALPHA
GU_DST_COLOR
GU_ONE_MINUS_DST_COLOR
GU_FIX
srcfixとdestfixに設定する値はそれぞれsrcとdestにGU_FIXが使われる場合のみ適用されます。
R8G8B8A8フォーマットで色を設定する事になりますが、PSPはリトルエンディアンのCPUを使っていることに注意してください。


*/
	/* 16bit(15bit) RGBA5551 */
	/*
	15: abbb bbgg gggr rrrr
	15: -bbb b-gg gg-r rrr- (pspのSDLではaが使えない？ので)
	*/
	#define MAKECOL15A(r, g, b, a)	(((b & 0xf8) << 7) | ((g & 0xf8) << 2) | ((r & 0xf8) >> 3) | ((a & 0x80)<<8))
	#define MAKECOL15(r, g, b)		(((b & 0xf8) << 7) | ((g & 0xf8) << 2) | ((r & 0xf8) >> 3))
	#define GETR15F(col)			(((col << 3) & 0xf8) /*| ((col >>  2) & 0x07)*/)
	#define GETG15F(col)			(((col >> 2) & 0xf8) /*| ((col >>  7) & 0x07)*/)
	#define GETB15F(col)			(((col >> 7) & 0xf8) /*| ((col >> 12) & 0x07)*/)
	#define GETR15(col) 			(((col << 3) & 0xf8) | ((col >>  2) & 0x07))
	#define GETG15(col) 			(((col >> 2) & 0xf8) | ((col >>  7) & 0x07))
	#define GETB15(col) 			(((col >> 7) & 0xf8) | ((col >> 12) & 0x07))
#endif

#if (1)
	/* 16bit RGBA5650 */
	/*
	16: bbbb bggg gggr rrrr
	16: bbbb -ggg gg-r rrr-
	*/
	#define MAKECOL16A(r, g, b, a)	(((b & 0xf8) << 8) | ((g & 0xfc) << 3) | ((r & 0xf8) >> 3))
	#define MAKECOL16(r, g, b)		(((b & 0xf8) << 8) | ((g & 0xfc) << 3) | ((r & 0xf8) >> 3))
	#define GETR16F(col)			(((col << 3) & 0xf8) /*| ((col >>  2) & 0x07)*/)
	#define GETG16F(col)			(((col >> 3) & 0xfc) /*| ((col >>  8) & 0x03)*/)
	#define GETB16F(col)			(((col >> 8) & 0xf8) /*| ((col >> 13) & 0x07)*/)
	#define GETR16(col) 			(((col << 3) & 0xf8) | ((col >>  2) & 0x07))
	#define GETG16(col) 			(((col >> 3) & 0xf8) | ((col >>  8) & 0x07))
	#define GETB16(col) 			(((col >> 8) & 0xf8) | ((col >> 13) & 0x07))
#endif

#if (1)
	/* 32bit RGBA8888 */
	#define MAKE32RGBA(r, g, b, a)	(((a & 0xff) << 24) | ((b & 0xff) << 16) | ((g & 0xff) << 8) | (r & 0xff))
	#define GETR32(col) 			(((col		) & 0xff) )
	#define GETG32(col) 			(((col >>  8) & 0xff) )
	#define GETB32(col) 			(((col >> 16) & 0xff) )
	#define GETA32(col) 			(((col >> 24) & 0xff) )
#endif

/* PSPの表示画面領域 */
#ifndef PSP_BUFFER_WIDTH512
	#define PSP_BUFFER_WIDTH512 	(512)
#endif
#ifndef PSP_WIDTH480
	#define PSP_WIDTH480			(480)
#endif
#ifndef PSP_HEIGHT272
	#define PSP_HEIGHT272			(272)
#endif

/* シューティングGAME部分の表示画面領域 */
//#define GAME_WIDTH			(380)
#define PANEL_WIDTH 			(128)
#define GAME_WIDTH				(PSP_WIDTH480-PANEL_WIDTH)
#define GAME_HEIGHT 			(PSP_HEIGHT272)

/* x位置が480の場合(==481ドット目) Scissorが働くので必ず表示しない */
#define GAME_NOT_VISIBLE480 	(PSP_WIDTH480)

#if 1
	/* ソフトウェアーで Zソート */
	#define USE_ZBUFFER 	(0)
#else
	/* ハードウェアーでソート */
	#define USE_ZBUFFER 	(1)
#endif

#if 1
	/* 単純拡大 */
	#define USE_ZOOM_XY 	(0)
#else
	/* 縦横拡大 */
	#define USE_ZOOM_XY 	(1)
#endif

/* C:/cygwin/pspdev/psp/include/math.h で宣言されているので要らない */
//#ifndef M_PI
//	#define M_PI			(3.14159265358979323846)
//#endif

//#define M_PI_H			(0.52359877559829887307)		/* π ÷ 6 */

//#define degtorad(x)		(((T_GU_M_PI*2)/360.0)*(x)) 	/* 2π ÷ 360 * X */
//#define radtodeg(x)		( (int)((x)*((360.0)/(T_GU_M_PI*2))+360)%360 )

#ifndef T_GU_M_PI
	#define T_GU_M_PI		GU_PI
#endif

//#if 0
/* CWの場合 */
	/* １周が360度の単位系(deg360)を１周が２πの単位系(radian)へ変換。及び逆変換。 */
	#define deg360_2rad(x)		(((T_GU_M_PI*2)/(360.0))*(x))
	#define rad2deg360(x)		( (int)((x)*((360.0)/(T_GU_M_PI*2))+360)%360 )

	/* １周が512度の単位系(deg512)を１周が２πの単位系(radian)へ変換。及び逆変換。 */
//	#define deg512_2rad(x)		(((T_GU_M_PI*2)/(512.0))*(x))
	//#define rad2deg512(x) 	( (int)((x)*((512.0)/(T_GU_M_PI*2))+512)%512 )
	//#define rad2deg512(x) 	( (int)((x)*((512.0)/(T_GU_M_PI*2))+512)&(512-1) )
//	#define rad2deg512(x)		( (int)((x)*((512.0)/(T_GU_M_PI*2))/*+512*/)&(512-1) )

	/* １周が1024度の単位系(deg4096)を１周が２πの単位系(radian)へ変換。及び逆変換。 */
	#define deg1024_2rad(x) 	(((T_GU_M_PI*2)/(1024.0))*(x))
	//#define rad2deg1024(x)	( (int)((x)*((1024.0)/(T_GU_M_PI*2))+1024)%1024 )
	//#define rad2deg1024(x)	( (int)((x)*((1024.0)/(T_GU_M_PI*2))+1024)&(1024-1) )
	#define rad2deg1024(x)		( (int)((x)*((1024.0)/(T_GU_M_PI*2))/*+1024*/)&(1024-1) )

	/* １周が4096度の単位系(deg4096)を１周が２πの単位系(radian)へ変換。及び逆変換。 */
	#define deg4096_2rad(x) 	(((T_GU_M_PI*2)/(4096.0))*(x))
	//#define rad2deg4096(x)	( (int)((x)*((4096.0)/(T_GU_M_PI*2))+4096)%4096 )
	//#define rad2deg4096(x)	( (int)((x)*((4096.0)/(T_GU_M_PI*2))+4096)&(4096-1) )
	#define rad2deg4096(x)		( (int)((x)*((4096.0)/(T_GU_M_PI*2))/*+4096*/)&(4096-1) )

	/* １周が65536度の単位系(deg65536)を１周が２πの単位系(radian)へ変換。及び逆変換。 */
	#define deg65536_2rad(x)	(((T_GU_M_PI*2)/(65536.0))*(x))
	//#define rad2deg65536(x)	( (int)((x)*((65536.0)/(T_GU_M_PI*2))+65536)%65536 )
	//#define rad2deg65536(x)	( (int)((x)*((65536.0)/(T_GU_M_PI*2))+65536)&(65536-1) )
	#define rad2deg65536(x) 	( (int)((x)*((65536.0)/(T_GU_M_PI*2))/*+65536*/)&(65536-1) )

	/* １周が360度の単位系(deg360)を１周が512度の単位系(deg512)へ変換。及び逆変換。 */
//	#define deg_360_to_512(x)	((int)((x)*(512.0/360.0)))
//	#define deg_512_to_360(x)	((int)((x)*(360.0/512.0)))

	/* １周が360度の単位系(deg360)を１周が1024度の単位系(deg1024)へ変換。及び逆変換。 */
	#define deg_360_to_1024(x)	((int)((x)*(1024.0/360.0)))
	#define deg_1024_to_360(x)	((int)((x)*(360.0/1024.0)))

	/* １周が360度の単位系(deg360)を１周が4096度の単位系(deg4096)へ変換。及び逆変換。 */
	#define deg_360_to_4096(x)	((int)((x)*(4096.0/360.0)))
	#define deg_4096_to_360(x)	((int)((x)*(360.0/4096.0)))

	/* １周が360度の単位系(deg360)を１周が65536度の単位系(deg65536)へ変換。及び逆変換。 */
	#define deg360_to_65536(x)	((int)((x)*(65536.0/360.0)))
	#define deg65536_to_360(x)	((int)((x)*(360.0/65536.0)))

//#else
/* CCWの場合 */

	/* １周が360度の単位系(deg360)を１周が512度の単位系(deg512)へ変換。 */
//	#define cv512r(x)		((int)((x)*(512.0/360.0)))
	/* １周が360度の単位系(deg360)を１周が1024度の単位系(deg1024)へ変換。 */
	#define cv1024r(x)		((int)((x)*(1024.0/360.0)))
//#endif

/* １周の範囲内にクリッピング */
//#define mask512(aaa)	{aaa &= (512-1);}/*模倣風でr31現在、無い筈*/
#define mask1024(aaa)	{aaa &= (1024-1);}/*標準精度弾*/
//#define mask4096(aaa) {aaa &= (4096-1);}/*模倣風でr31現在、無い筈*/
#define mask65536(aaa)	{aaa &= (65536-1);}/*精密弾*/	/* 註:※１ */

/*
	註:※１こう書いておくとpspでは、GCC(4.xx以降なら)が and演算などしない。
	単純にショートにするだけ。
	2byte になってるだけだから、pspでは速い筈。
*/

/* 数字をスコアに変換。及び逆変換。 */
#define score(x)	((u32)(((u32)(x))/10))
#define score_r(x)	((u32)(((u32)(x))*10))

/* 数字を256固定小数点形式に変換。及びdou bleへ逆変換。 */
#define t256(x) 				((int)((x)*256))
//#define t256_to_dou ble(x)	(((dou ble)(x))*(1.0/256.0))
#define t256_floor(x)			((x)>>(8))

/* 数字を8固定小数点形式に変換。及びdou bleへ逆変換。 */
#define t8(x)					((short)((x)*8))
//#define t8_to_dou ble(x)		(((dou ble)(x))*(1.0/8.0))
#define t8_floor(x) 			((x)>>(3))


/* aaa%の確率で */
#define rand_percent(aaa) ((unsigned char)(ra_nd())<=(unsigned char)( (aaa*256)/100 ) ) /* aaa%の確率で */

/* ラジアン撲滅対策。pspは dou bleの値を保持するだけでも遅い。(例えば関数の引数とか)
intで値を保持して、使う度に変換、逆変換した方が、ずっと速い。
そこでラジアン撲滅する為のマクロ */
#if 0
	/* １周が512度の単位系の atan2(y,x), cos(x), sin(x)  */
	#define at an_512(y,x)	(rad2deg512(atan2((y),(x))))
	#define co_s512(x)		(cos(deg512_2rad((x))))
	#define si_n512(x)		(sin(deg512_2rad((x))))
	/* 変な名前になってるのは cos( とか sin( で検索するため */
#else
//	#define SINTABLE_SIZE512	512
//	#define SINTABLE_MASK512	(SINTABLE_SIZE512-1)
	#define SINTABLE_SIZE1024	1024
	#define SINTABLE_MASK1024	(SINTABLE_SIZE1024-1)

//	#define OFFS_SIN512 		(0)
//	#define OFFS_COS512 		((SINTABLE_SIZE512/4)-1)/*127 90*/
	#define OFFS_SIN1024		(0)
	#define OFFS_COS1024		((SINTABLE_SIZE1024/4)-1)/*255 90*/
//	extern int at an_512(int y, int x);
	extern int atan_1024(int y, int x);
	extern int atan_65536(int y, int x);
	#if (1==USE_SIN_TABLE)
	//	extern int sin_tbl 512[SINTABLE_SIZE];
	//	#define cos512(x)		(sin_tbl 512[(((x)+OFFS_COS512)&(512-1))])
	//	#define sin512(x)		(sin_tbl 512[(((x)			 )&(512-1))])
	#else
		#include "psp_vfpu.h"
	//	#define cos512(x)		((int)(int256_sin1024((((x+x)+OFFS_COS512+OFFS_COS512)&(1024-1)))))
	//	#define sin512(x)		((int)(int256_sin1024((((x+x)						 )&(1024-1)))))
		#define cos1024(x)		((int)(int256_sin1024((((x)+OFFS_COS1024)&(1024-1)))))
		#define sin1024(x)		((int)(int256_sin1024((((x) 			)&(1024-1)))))
	#endif
#endif

/* rand()撲滅対策。本家にはリプレイ機能がある。
リプレイ機能を再現する為には内蔵の組み込み関数rand()は使えない。
周期の長い自前の擬似乱数の関数を用意する必要がある。
リプレイ開始時やゲーム開始時(面の始め)に乱数系列の初期化をする。 */
/* 変な名前になってるのは rand( で検索するため */
#if 0
	#define ra_nd(x)		(rand(x))
#else
	extern int ra_nd(void);
#endif


#endif /* _SUPPORT_H_ */
