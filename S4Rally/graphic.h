//画像のヘッダファイル
#pragma once

//ヘッダファイル読み込み
#include "DxLib.h"
#include "geometory.h"
#include "game.h"
#include "fps.h"

//エフェクト無限継続
//※負数なので通常はありえない時間
#define GraFxInfinity	-99999

//エラー
#define ImageLoadErrCap		"が読み込めませんでした"
#define ImageLoadErrTitle	"画像読み込みエラー"

//文字列の長さ
#define ImagePathStrMax	256
#define ImageErrStrMax	512

//画像ファイルのパス
//※日本語や絵文字など2バイト文字を避けること(最悪文字化けで読み込めないときがある)
//※全角空白、半角空白問わず空白を避けること(ファイル名の区切りだと思われることもある)

#define GraphicPathTitleBackImage	".\\graphic\\TitleBack.png"
#define GraphicPathTitleLogo		".\\graphic\\TitleLogo.png"
//#define GraphicPathTitleButton		".\\graphic\\StartButton.png"	//　サイト｜空想曲線	ＵＲＬ｜https://kopacurve.blog.fc2.com/
//#define GraphicPathBackGrass		".\\graphic\\weed.png"
//#define BackGrassNum				15		//草原の背景の個数
//#define GraphicPathGameOverLogo		".\\graphic\\GameOverLogo.png"
//#define GraphicPathNewrecordLogo	".\\graphic\\NewrecordLogo.png"


//列挙型
enum Graphic_Align
{
	GraAlignTopL,		//画像の基準点で上左揃え
	GraAlignTopC,		//画像の基準点で上中央揃え
	GraAlignTopR,		//画像の基準点で上右揃え

	GraAlignBottomL,	//画像の基準点で下左揃え
	GraAlignBottomC,	//画像の基準点で下中央揃え
	GraAlignBottomR,	//画像の基準点で下右揃え

	GraAlignAllCenter,	//画像の基準点で上下も左右も中央揃え
};

//エフェクトの種類(描画の効果)
enum Graphic_Fx
{
	GraFxBlick,			//点滅
	GraFxFadeIn,		//透明→不透明
	GraFxFadeOut,		//不透明→透明
	GraFxFadeInOut,		//不透明←→透明
	GraFxNone			//通常描画
};

//画像構造体のコア部分
typedef struct _GraphicCore
{
	char Path[ImagePathStrMax];			//パス

	POINT Pos;							//画像の基準点(通常は左上)
	int Width = 0;						//幅(ウィドス)
	int Height = 0;						//高さ(ハイト)

	RECT Coll;							//矩形の当たり判定
	RECT CollMargin;					//矩形の当たり判定を挟める

	Graphic_Align Align = GraAlignTopL;	//揃え方向

	float Rate = 1.0f;					//拡大率
	float Angle = 0.0f;					//角度

	BOOL IsDraw = FALSE;				//描画するか？

	BOOL IsFxStartFlg = FALSE;			//描画エフェクトフラグ
	Graphic_Fx Fx = GraFxNone;			//描画エフェクトの種類
	int FxMillTime = 0;					//エフェクト時間カウント
	int FxMillTimeMax = 0;				//エフェクト時間カウントMAX
	int FxInterMillTime = 0;			//エフェクト間隔カウント
	int FxInterMillTimeMax = 0;			//エフェクト間隔カウントMAX
	float Alpha = 0.0f;					//画像の透明度(MIN0.0～MAX1.0を入れること)
}GraphicCore;

//画像構造体
typedef struct _Graphic
{
	GraphicCore c;		//コア
	int Handle = -1;	//ハンドル
}Graphic;

//外部グローバル変数
extern Graphic TitleBackImage;		//タイトル背景画像
extern Graphic TitleLogo;			//タイトルロゴ
extern Graphic TitleButton;			//タイトルボタン
extern Graphic BackImageTurn;		//背景反転
//extern Graphic BackGrass[BackGrassNum];	//草原
extern Graphic GameoverLogo;		//ゲームオーバーロゴ
extern Graphic NewrecordLogo;		//ニューレコードロゴ

//外部プロトタイプ宣言
extern BOOL Graphic_Init(VOID);							//読み込み・ハンドル生成
extern VOID Graphic_End(VOID);							//後始末
extern VOID GraphicLoadErrMessage(const char* path);	//読み込みエラーメッセージ

extern Graphic LoadGraphic(const char* path, POINT point, RECT margin, Graphic_Align align);	//読み込み
extern VOID DrawGraphic(Graphic* graphic);				//描画
extern VOID DeleteGraphic(Graphic graphic);				//解放

extern VOID ReMakeCollRect(Graphic* graphic);			//当たり判定の矩形を再生成

extern VOID GraphicFxStart(Graphic* graphic, Graphic_Fx Fx, int MillTime, int MillInterval);	//エフェクト(Fx)開始