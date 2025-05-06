#include "DxLib.h"
#include "fps.h"
#include "game.h"
#include "key.h"
#include "mouse.h"
#include "font.h"
#include "music.h"
#include "graphic.h"
#include "map.h"
#include "timer.h"
#include "controller.h"
#include "car.h"


//プロトタイプ宣言
VOID GameAndDxLibAllEnd(VOID);//ゲーム終了の後始末

//ゲーム終了の後始末
//ここに???_End系を固める
VOID GameAndDxLibAllEnd(VOID)
{
	Font_End();			//フォント終了
	Music_End();		//音楽後始末
	Graphic_End();		//画像後始末
	DxLib_End();		// ＤＸライブラリ使用の終了処理

	return;
}

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)//Windows用ソフトのプログラムのスタート地点となる関数の宣言
{
	SetOutApplicationLogValidFlag(FALSE);//Log.txt を出力しない　
	ChangeWindowMode(TRUE);//ウィンドウモードの設定
	SetGraphMode(GameWidth, GameHeight, GameColor);//解像度を設定
	SetWindowSize(GameWidth, GameHeight);//ウィンドウの大きさを設定
	SetMainWindowText(GameTitle);//ウィンドウのタイトル
	SetBackgroundColor(0, 0, 0);//ウィンドウの背景色
	SetWaitVSyncFlag(GameVsync);//垂直同期の設定
	SetAlwaysRunFlag(TRUE);//非アクティブでも実行

	SetWindowIconID(GameIcon);//アイコンを設定

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//音楽初期化
	if (Music_Init() == FALSE)
	{
		GameAndDxLibAllEnd();	//ゲーム終了の後始末
		return -1;
	}

	//裏画面に描画する（ダブルバッファリング）
	SetDrawScreen(DX_SCREEN_BACK);

	//フォントの初期化
	if (Font_Init() == FALSE)
	{
		//ゲーム終了の後始末
		GameAndDxLibAllEnd();

		return -1;
	}

	//画像初期化
	if (Graphic_Init() == FALSE)
	{
		GameAndDxLibAllEnd();	//ゲーム終了の後始末
		return -1;
	}

	//ゲーム画面の初期化
	GameInit();

	//FPS初期化
	FPSInit();

	//キーボード初期化
	KeyInit();

	//マウス初期化
	MouseInit();

	//コントローラー初期化
	ControllerInit();

	//タイトルシーンを初期化
	TitleInit();

	//タイマー（周回）の初期化
	TimerInit();

	//無限ループ（ゲームループ）
	while (TRUE)
	{
		//メッセージ処理をする（マウス操作やキー入力などを受け続ける）
		if (ProcessMessage() != 0)
		{
			break;//無限ループから出る（ゲーム終了）
		}

		//画面を消去する（1ループずつ書き換える）
		if (ClearDrawScreen() != 0)
		{
			break;//無限ループから出る（ゲーム終了）
		}

		//ここに全てのゲームの動作が入る
		{
			FPSCheck();//FPS計測
			KeyUpdate();//キーボード更新
			MouseUpdate();//マウス更新

			//次のシーンを初期化
			if (NowGameScene != ChangeGameScene)
			{
				//各シーンの初期化を呼び出す
				//シーン切り替え
				switch (ChangeGameScene)
				{
				case TitleScene:
					TitleInit();
					break;
				case PlayScene:
					PlayInit();
					break;
				case ResultScene:
					ResultInit();
					break;
				case RuleScene:
					RuleInit();
					break;
				default:
					break;
				}

				//シーン切り替え後のシーンを現在のシーンにする
			     NowGameScene= ChangeGameScene;
			}
			else
			{
				//シーン切り替え
				switch (NowGameScene)
				{
				case TitleScene:
					TitleCtrl();
					break;
				case PlayScene:
					PlayCtrl();
					break;
				case ResultScene:
					ResultCtrl();
					break;
				case RuleScene:
					RuleCtrl();
				default:
					break;
				}
			}

			FPSDraw();//FPS描画
			FPSWait();//FPS待機
		}

		//裏画面を画像に描画
		ScreenFlip();
	}

	//ゲーム終了の後始末
	GameAndDxLibAllEnd();

	return 0;				// ソフトの終了 
}

