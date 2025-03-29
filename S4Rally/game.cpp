//ゲーム処理のソースファイル

//ヘッダファイル読み込み
#include "game.h"
#include "key.h"
#include "mouse.h"
#include "geometory.h"
#include "font.h"
#include "music.h"
#include "graphic.h"
#include "map.h"
//#include "rallycar.h"
#include "timer.h"
#include "controller.h"
#include "car.h"
//#include "wheelcollider.h"
//#include "carbase.h"

//グローバル変数

//ゲームシーン
enum GameScene NowGameScene;//現在のゲームシーン
enum GameScene ChangeGameScene;//切り替わるゲームシーン

//ゲーム画面関係
RECT GameWindow;//ゲーム画面の矩形
HWND GameHandle;//ゲームのウィンドウハンドル

//ゲームシーンの名前
char GameSceneName[GameSceneCount][GameSceneNameMax]
{
	"タイトル画面",
	"プレイ画面",
	"リザルト画面",
	"メニュー画面"
};

//シーン切り替え後のフレーム数を管理
int GameSceneFrameCount[GameSceneCount];

//各シーンの切り替えボタン
CIRCLE StartCircle;//スタートの円
CIRCLE PlayCircle;//プレイの円
CIRCLE ResultCircle;//リザルトの円
CIRCLE MenuCircle;//メニューの円

RECT controllerPointRect; //コントローラーのスティックの円

RECT TimeAttackRect; //タイムアタック
RECT RuleRunRect;    //ルール
RECT LockRunRect;    //ロック

RECT backSceneRect;	 //シーンに戻る
RECT backPanelRect;  //バックパネル

int BGMVolume = VolumeBGM;	// BGMの音量調整の値
int BGMVolumeDrawValue;		// BGMの描画で使う値

int SEVolume = VolumeSE;//SEの音量調整の値
int SEVolumeDrawValue;	// SEの描画で使う値

// 選択感覚を調整するための変数
int stickCooldown = 0; // クールダウンタイマー
const int stickCooldownMax = 10; // クールダウン間隔

float LeftPlayerPosition = 990.0f;
float TopPlayerPosition = 540.0f;
float RightPlayerPosition = 1015.0f;
float BottomPlayerPosition = 565.0f;

float StickSensitivity = 10.0f; // スティックの感度設定（値を調整して動作速度を制御）

float mainSelect=0;

//関数

//ゲーム画面のハンドルを取得
HWND GetGameHandle(VOID)
{
	return GameHandle;
}

//ゲーム画面の矩形を取得
RECT GetGameWindow(VOID)
{
	return GameWindow;
}

//ゲーム画面の上下左右の中央座標を取得
POINT GetGameWindowCenter(VOID)
{
	POINT pt;

	pt.x = GameWidth / 2;
	pt.y = GameHeight / 2;

	return pt;
}

//ゲーム画面の初期化
VOID GameInit(VOID)
{
	//ゲーム画面のハンドル(管理番号)を取得
	GameHandle = GetMainWindowHandle();

	//ゲーム画面の矩形を取得
	GameWindow = GetRect(0, 0, GameWidth, GameHeight);

	//最初のシーンはタイトルから
	NowGameScene = TitleScene;

	//次のシーンもタイトルから
	ChangeGameScene = TitleScene;

	//コントローラーのスティックで操作する矩形
	LeftPlayerPosition = 990.0f;
	TopPlayerPosition = 540.0f;
	RightPlayerPosition = 1015.0f;
	BottomPlayerPosition = 565.0f;
	controllerPointRect= GetRect(LeftPlayerPosition, TopPlayerPosition,RightPlayerPosition, BottomPlayerPosition);

	//各シーンの円の初期化
	StartCircle = GetCircle(GetGameWindowCenter(), 300.0f); //半径300.0fの円を画面中央におく
	PlayCircle = GetCircle(GetPoint(1200.0f, 150.0f), 30.0f);
	ResultCircle = GetCircle(GetGameWindowCenter(), 300.0f);//半径300.0fの円を画面中央におく
	MenuCircle = GetCircle(GetPoint(1200.0f,50.0f), 30.0f);

	//スタートシーンのゲームモード選択時の矩形の初期化
	TimeAttackRect=GetRect(700, 50, 1200, 200);
	RuleRunRect = GetRect(700, 250, 1200, 400);
	LockRunRect = GetRect(700, 450, 1200, 600);

	//スタートシーンのメニュー画面
	backSceneRect = GetRect(50, 630, 300, 700);
	backPanelRect = GetRect(100, 50, 1200,600);

	return;
}
//タイトル初期化
VOID TitleInit(VOID)
{
	//コントローラーのスティックで操作する矩形
	LeftPlayerPosition = 990.0f;
	TopPlayerPosition = 540.0f;
	RightPlayerPosition = 1015.0f;
	BottomPlayerPosition = 565.0f;

	if (GameDebug == TRUE)
	{
		//シーン名表示
		DrawFormatStringToHandle(
			GameWidth - 200, 0,
			Color_brack,fontDefault.Handle,
			"%s%s", GameSceneName[ChangeGameScene], "初期化");
	}

	//シーンを切り替えたフレーム数を初期化
	GameSceneFrameCount[ChangeGameScene] = 0;

	return;
}
//タイトル管理
VOID TitleCtrl(VOID)
{
	TitleProc();//処理しているから
	TitleDraw();// 描画する
	return;
}
//タイトル処理
VOID TitleProc(VOID)
{
	//シーン切り替え後のフレーム数をカウントアップ
	GameSceneFrameCount[NowGameScene]++;

	//音楽を再生
	PlayMusic(TitleBGM);
	
	//フェードインして再生
	//FadeInPlayMusic(&TitleBGM, 5000);	//5秒かけてフェードイン

	//フェードアウトして再生
	//FadeOutPlayMusic(&TitleBGM, 5000);	//5秒かけてフェードイン

	////シーン切り替え
	//if (CollCircleToMouseDown(StartCircle, MOUSE_INPUT_LEFT) == TRUE//円のどこかをマウス左ボタンで押したとき
	//	&& GameSceneFrameCount[NowGameScene] >= GameSceneChangeFrame)	//かつ、切り替え可能なフレーム数を超えたら
	//{
	//	//効果音を再生
	//	PlayMusic(ButtonClickSE);
	//	
	//	//音楽停止
	//	PauseMusic(&TitleBGM);

	//	//シーン切り替え
	//	ChangeGameScene = PlayScene;

	//	//すぐに切り替える
	//	return;
	//}

	////画像エフェクト開始
	//GraphicFxStart(
	//	&TitleButton,		//どの画像にエフェクトをかける？
	//	GraFxFadeInOut,		//どんなエフェクトをかける？
	//	GraFxInfinity,		//エフェクト無限継続
	//	1000);				//1000ミリ秒間隔

	return;
}
//タイトル描画
VOID TitleDraw(VOID)
{
	ControllerUpdate();//コントローラーの更新処理

	// アナログスティックの値を取得して表示
	float axisX = GetAxisX();
	float axisY = GetAxisY();

	if (GameDebug == TRUE)
	{
		//適当に描画
		DrawBox(0, 0, GameWidth, GameHeight, GetColor(178, 216, 255), TRUE);

		//シーン名表示
		DrawFormatStringToHandle(
			GameWidth - 200, 0,
			Color_brack, fontDefault.Handle,
			"%s%s", GameSceneName[NowGameScene], "描画中");

		//メニューの選択
		{
			if (CollRectToRect(TimeAttackRect, controllerPointRect))
			{
				DrawRect(TimeAttackRect, Color_tomato, TRUE);
				if (GetButtonDown(BUTTON_A)||KeyDown(KEY_INPUT_SPACE))
				{
					//効果音を再生
					PlayMusic(ButtonClickSE);

					//音楽停止
					PauseMusic(&TitleBGM);

					//シーン切り替え
					ChangeGameScene = PlayScene;

					//すぐに切り替える
					return;
				}
			}
			else{
				DrawRect(TimeAttackRect, Color_white, TRUE);
			}

			if (CollRectToRect(RuleRunRect, controllerPointRect))
			{
				DrawRect(RuleRunRect, Color_tomato, TRUE);
				if (GetButtonDown(BUTTON_A) || KeyDown(KEY_INPUT_SPACE))
				{
					//効果音を再生
					PlayMusic(ButtonClickSE);

					//音楽停止
					PauseMusic(&TitleBGM);

					//シーン切り替え
					ChangeGameScene = RuleScene;

					//すぐに切り替える
					return;
				}
			}
			else {
				DrawRect(RuleRunRect, Color_white, TRUE);
			}

			DrawRect(LockRunRect, Color_white, TRUE);
		}
		{
			if (CheckHitKey(KEY_INPUT_A) || axisX <= -0.5f) {
				LeftPlayerPosition -= StickSensitivity;
				RightPlayerPosition -= StickSensitivity;
			}
			else if (CheckHitKey(KEY_INPUT_D) || axisX >= 0.5f) {
				LeftPlayerPosition += StickSensitivity;
				RightPlayerPosition += StickSensitivity;
			}

			if (CheckHitKey(KEY_INPUT_W) || axisY >= 0.5f) {
				TopPlayerPosition -= StickSensitivity;
				BottomPlayerPosition -= StickSensitivity;
			}
			else if (CheckHitKey(KEY_INPUT_S) || axisY <= -0.5f) {
				TopPlayerPosition += StickSensitivity;
				BottomPlayerPosition += StickSensitivity;
			}

			// 画面外に出ないように制限する処理
			if (LeftPlayerPosition < 0) {
				LeftPlayerPosition = 0;
				RightPlayerPosition = 25.0f; // 矩形の幅を保持
			}
			if (RightPlayerPosition > GameWidth) {
				RightPlayerPosition = GameWidth;
				LeftPlayerPosition = GameWidth - 25.0f; // 矩形の幅を保持
			}
			if (TopPlayerPosition < 0) {
				TopPlayerPosition = 0;
				BottomPlayerPosition = 25.0f; // 矩形の高さを保持
			}
			if (BottomPlayerPosition > GameHeight) {
				BottomPlayerPosition = GameHeight;
				TopPlayerPosition = GameHeight - 25.0f; // 矩形の高さを保持
			}

			// 更新した値をcontrollerPointRectに反映
			controllerPointRect = GetRect(LeftPlayerPosition, TopPlayerPosition, RightPlayerPosition, BottomPlayerPosition);

			// 矩形を描画
			DrawRect(controllerPointRect, GetColor(255, 0, 0), TRUE);
		}
	}


	DrawFormatStringToHandleAlign(
		300,50, Align_Center, Color_brack, fontJiyu100ptFuch.Handle,
		"%s", "S4Rally");

	DrawFormatStringToHandleAlign(
		950, 100, Align_Center, Color_brack, fontJiyu50ptFuch.Handle,
		"%s", "タイムアタック");

	DrawFormatStringToHandleAlign(
		950, 300, Align_Center, Color_brack, fontJiyu50ptFuch.Handle,
		"%s", "ルール・操作説明");

return;
}

//プレイ初期化
VOID PlayInit(VOID)
{
	if (GameDebug == TRUE)
	{
		//シーン名表示
		DrawFormatStringToHandle(
			GameWidth - 200, 0,
			Color_brack, fontDefault.Handle,
			"%s%s", GameSceneName[ChangeGameScene], "初期化");
	}

	//シーンを切り替えたフレーム数を初期化
	GameSceneFrameCount[ChangeGameScene] = 0;

	return;
}
//プレイ管理
VOID PlayCtrl(VOID)
{
	PlayProc();//処理しているから
	PlayDraw();// 描画する
	return;
}
//プレイ処理
VOID PlayProc(VOID)
{
	ControllerUpdate();

	//音楽を再生
	PlayMusic(PlayBGM);

	//シーン切り替え後のフレーム数をカウントアップ
	GameSceneFrameCount[NowGameScene]++;

	//if (GetButtonDown(BUTTON_START))
	//{
	//	//効果音を再生
	//	PlayMusic(ButtonClickSE);

	//	//音楽停止
	//	PauseMusic(&PlayBGM);

	//	//シーン切り替え
	//	ChangeGameScene =MenuScene;

	//	//すぐに切り替える
	//	return;
	//}

	return;
}
//プレイ描画
VOID PlayDraw(VOID)
{
	if (GameDebug == TRUE)
	{
		GraphDraw();	//ステージを表示

		ControllerUpdate(); //コントローラーの更新

		//シーン名表示
		DrawFormatStringToHandle(
			GameWidth - 200, 0,
			Color_brack, fontDefault.Handle,
			"%s%s", GameSceneName[NowGameScene], "描画中");

	    CarUpdate();

		TimerUpdate();
		TimerDraw();
	}

	return;
}

//リザルト初期化
VOID ResultInit(VOID)
{
	if (GameDebug == TRUE)
	{
		//シーン名表示
		DrawFormatStringToHandle(
			GameWidth - 200, 0,
			Color_brack, fontDefault.Handle,
			"%s%s", GameSceneName[ChangeGameScene], "初期化");

		//音楽停止
		PauseMusic(&PlayBGM);

	}

	//シーンを切り替えたフレーム数を初期化
	GameSceneFrameCount[ChangeGameScene] = 0;

	return;
}
//リザルト管理
VOID ResultCtrl(VOID)
{
	ResultProc();//処理しているから
	ResultDraw();// 描画する
	return;
}
//リザルト処理
VOID ResultProc(VOID)
{
	// シーン切り替え後のフレーム数をカウントアップ
	GameSceneFrameCount[NowGameScene]++;

	// 7秒経過後、タイトルシーンに戻る
	if (GameSceneFrameCount[NowGameScene] >= 420)
	{
		//効果音を再生
		PlayMusic(ButtonClickSE);

		// シーン切り替え
		ChangeGameScene = TitleScene;

		return;
	}

	return;
}
//リザルト描画
VOID ResultDraw(VOID)
{
	if (GameDebug == TRUE)
	{
		//適当に描画
		DrawBox(0, 0, GameWidth, GameHeight, GetColor(255, 230, 179), TRUE);

		//シーン名表示
		DrawFormatStringToHandle(
			GameWidth - 200, 0,
			Color_brack, fontDefault.Handle,
			"%s%s", GameSceneName[NowGameScene], "描画中");
	}

	////円の中にマウスの座標が入ったら円の色を帰る
	//if (CollCircleToMouse(StartCircle) == TRUE)
	//{
	//	//円を描画
	//	DrawEn(StartCircle, Color_tomato, TRUE);
	//}
	//else
	//{
	//	//円を描画
	//	DrawEn(StartCircle, Color_white, TRUE);
	//}

	////とりあえずシーン名を描画
	//DrawFormatStringToHandle(
	//	GetGameWindowCenter().x - 160, GetGameWindowCenter().y + 10, Color_brack, fontJiyu100ptFuch.Handle,
	//	"%s", " Result!!");

	return;
}

//ルール・操作説明初期化
VOID RuleInit(VOID)
{

	if (GameDebug == TRUE)
	{
		//シーン名表示
		DrawFormatStringToHandle(
			GameWidth - 200, 0,
			Color_brack, fontDefault.Handle,
			"%s%s", GameSceneName[ChangeGameScene], "初期化");
	}

	//シーンを切り替えたフレーム数を初期化
	GameSceneFrameCount[ChangeGameScene] = 0;

	return;
}

//ルール・操作説明管理
VOID RuleCtrl(VOID)
{
	RuleProc();
	RuleDraw();
	return;
}

//ルール・操作説明処理
VOID RuleProc(VOID)
{
	//シーン切り替え後のフレーム数をカウントアップ
	GameSceneFrameCount[NowGameScene]++;

	return;
}

//ルール・操作説明描画
VOID RuleDraw(VOID)
{
	ControllerUpdate();//コントローラーの更新処理

	// アナログスティックの値を取得して表示
	float axisX = GetAxisX();
	float axisY = GetAxisY();

	if (GameDebug == TRUE)
	{
		//適当に描画
		DrawBox(0, 0, GameWidth, GameHeight, GetColor(255, 230, 179), TRUE);

		DrawRect(backPanelRect, Color_white, TRUE);

		if (CollRectToRect(backSceneRect, controllerPointRect))
		{
			DrawRect(backSceneRect, Color_tomato, TRUE);
			if (GetButtonDown(BUTTON_A) || KeyDown(KEY_INPUT_SPACE))
			{
				//効果音を再生
				PlayMusic(ButtonClickSE);

				//音楽停止
				PauseMusic(&TitleBGM);

				//シーン切り替え
				ChangeGameScene = TitleScene;

				//すぐに切り替える
				return;
			}
		}
		else {
			DrawRect(backSceneRect, Color_white, TRUE);
		}
	}


	{
		if (CheckHitKey(KEY_INPUT_A) || axisX <= -0.5f) {
			LeftPlayerPosition -= StickSensitivity;
			RightPlayerPosition -= StickSensitivity;
		}
		else if (CheckHitKey(KEY_INPUT_D) || axisX >= 0.5f) {
			LeftPlayerPosition += StickSensitivity;
			RightPlayerPosition += StickSensitivity;
		}

		if (CheckHitKey(KEY_INPUT_W) || axisY >= 0.5f) {
			TopPlayerPosition -= StickSensitivity;
			BottomPlayerPosition -= StickSensitivity;
		}
		else if (CheckHitKey(KEY_INPUT_S) || axisY <= -0.5f) {
			TopPlayerPosition += StickSensitivity;
			BottomPlayerPosition += StickSensitivity;
		}

		// 画面外に出ないように制限する処理
		if (LeftPlayerPosition < 0) {
			LeftPlayerPosition = 0;
			RightPlayerPosition = 25.0f; // 矩形の幅を保持
		}
		if (RightPlayerPosition > GameWidth) {
			RightPlayerPosition = GameWidth;
			LeftPlayerPosition = GameWidth - 25.0f; // 矩形の幅を保持
		}
		if (TopPlayerPosition < 0) {
			TopPlayerPosition = 0;
			BottomPlayerPosition = 25.0f; // 矩形の高さを保持
		}
		if (BottomPlayerPosition > GameHeight) {
			BottomPlayerPosition = GameHeight;
			TopPlayerPosition = GameHeight - 25.0f; // 矩形の高さを保持
		}

		// 更新した値をcontrollerPointRectに反映
		controllerPointRect = GetRect(LeftPlayerPosition, TopPlayerPosition, RightPlayerPosition, BottomPlayerPosition);

		// 矩形を描画
		DrawRect(controllerPointRect, GetColor(255, 0, 0), TRUE);

		DrawFormatStringToHandleAlign(
			170, 640, Align_Center, Color_brack, fontJiyu50ptFuch.Handle,
			"%s", "戻る");
	}


	return;
}