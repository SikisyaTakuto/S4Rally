//フォントのヘッダファイル
#pragma once

//ヘッダファイル読み込み
#include"DxLib.h"

//フォントのパスと名前

//外部フォント
#define FontPathJiyu "./font/Jiyucho.ttf"
#define FontNameJiyu "じゆうちょうフォント Regular"

//内部フォント
#define FontNameMSMincho "ＭＳ 明朝"

//デフォルトフォント
#define FontDefName FontNameMSMincho//フォント名
#define FontDefSize 20//文字サイズ
#define FontDefThinck 1//文字の太さ
#define FontDefType DX_FONTTYPE_ANTIALIASING_8X8//アンチエイジング	

//文字列の長さ
#define FontNameStrMax 256
#define FontErrStrMax 512

//エラー
#define FontInstallErrCap "がインストールできません"
#define FontInstallErrTitle "フォントインストールエラー"

#define FontCreateErrCap "が作成できません"
#define FontCreateErrTitle "フォント作成エラー"

//文字揃えMAX文字数
#define FontAlignStrMax		512

//列挙型
enum Font_Align
{
	Align_Left,		//左揃え
	Align_Center,	//中央揃え
	Align_Right,	//右揃え
	Align_AllCenter	//上下も左右も中央揃え
};

//構造体
typedef struct _AddFont
{
	char FontPath[FontNameStrMax];
	char FontName[FontNameStrMax];
	BOOL CanInstall = FALSE;//インストールできたか？

}AddFont;//一時的にインストールするフォント情報を

typedef struct _Font
{
	int Handle;//フォントハンドル(フォントデータの管理番号)

	char Name[FontNameStrMax];//フォント名
	int Size;//フォントのサイズ
	int Thinck;//フォントの太さ
	int Type;//アンチエイジング	

}Font;//フォントデータ作成用

//外部グローバル変数
extern AddFont addJiyu;//じゆうちょうフォントを追加
extern Font fontDefault;//デフォルトのフォント
extern Font fontJiyu100pt;//じゆうちょうフォント100pt
extern Font fontJiyu100ptFuch;//フォントの縁
extern Font fontJiyu50pt;//じゆうちょうフォント50pt
extern Font fontJiyu50ptFuch;//フォントの縁
extern Font fontJiyu25pt;//じゆうちょうフォント25pt
extern Font fontJiyu25ptFuch;//フォントの縁

//外部プロトタイプ宣言
extern BOOL Font_Init(VOID);//インストール&生成
extern AddFont FontInstall(const char* path, const char* name);//インストール
extern VOID FontInstallErrMessage(const char* path);//インストールエラーメッセージ
extern Font FontCreate(const char* name, int size, int thinck, int type);//フォントハンドル生成
extern VOID FontCreateErrMessage(Font font);//フォントハンドル生成エラーメッセージ
extern VOID FontDelete(Font font);//フォントハンドル削除
extern VOID FontUnInstall(AddFont add);//アンインストール
extern VOID Font_End(VOID);//削除&アンインストール

//フォントを揃えて描画
extern VOID DrawFormatStringToHandleAlign(int x, int y, Font_Align align,
	unsigned int Color, int FontHandle, const char* FormatString, ...);