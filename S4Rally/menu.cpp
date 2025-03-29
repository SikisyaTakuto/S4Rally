//メニューのソースファイル

//ヘッダファイル読み込み
#include "menu.h"

//外部のグローバル変数
enum MenuScene MenuScene;//メニューシーン
enum MenuScene ReturnGameScene;//ゲームシーンに戻る

//メニューシーンの名前
char MenuSceneName[GameSceneCount][GameSceneNameMax]
{
	"メニュー画面"
};

//シーン切り替え後のフレーム数を管理
int MenuSceneFrameCount[GameSceneCount];

//ゲーム画面関係
RECT MenuWindow;//ゲーム画面の矩形
HWND MenuHandle;//ゲームのウィンドウハンドル

//ゲーム画面のハンドルを取得
HWND GetMenuHandle(VOID)
{
	return MenuHandle;
}

//ゲーム画面の矩形を取得
RECT GetMenuWindow(VOID)
{
	return MenuWindow;
}

//ゲーム画面の上下左右の中央座標を取得
POINT GetMenuWindowCenter(VOID)
{
	POINT pt;

	pt.x = GameWidth / 2;
	pt.y = GameHeight / 2;

	return pt;
}

//ゲーム画面の初期化
VOID AllMenuInit(VOID)
{
	return;
}

//メニュー初期化
VOID MenuInit(VOID)
{
	if (GameDebug == TRUE)
	{
		//シーン名表示
		DrawFormatStringToHandle(
			GameWidth - 500, 0,
			Color_brack, fontDefault.Handle,
			"%s%s", MenuSceneName[ChangeGameScene], "初期化");
	}

	//シーンを切り替えたフレーム数を初期化
	MenuSceneFrameCount[ChangeGameScene] = 0;
	return;
}

//メニュー管理
VOID MenuCtrl(VOID)
{
	return;
}

//メニュー処理
VOID MenuProc(VOID)
{
	return;
}
//メニュー描画
VOID MenuDraw(VOID)
{
	return;
}