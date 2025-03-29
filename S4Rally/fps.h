//FPS処理のヘッダファイル
#pragma once

//ヘッダファイル読み込み
#include "DxLib.h"

//マクロ定義
#define GameFPS 60//設定したいFPS

#define MicroSecond 1000000.0f//1マイクロ秒
#define MillSecond 1000.0f//1ミリ秒
#define WaitTimeMill 3000//最大で待てるミリ数

//構造体の定義＆名前の再定義
typedef struct _FPS
{
	LONGLONG FirstTakeTime = 0;//1フレーム目の計測時間
	LONGLONG NowTakeTime = 0;//現在の計測時間
	LONGLONG OldTakeTime = 0;//以前の計測時間

	float Deltatime = 0.000001f;//デルタタイム（経過時間）
	int FrameCount = 1;//現在のフレーム数（1フレーム目～MAXフレーム目まで）
	float Average = 0.0f;//平均FPS値
}FPS;

//外部グローバル変数
extern FPS fps;

//外部プロトタイプ宣言
extern VOID FPSInit(VOID);//FPS初期化
extern VOID FPSCheck(VOID);//FPS計測
extern VOID FPSDraw(VOID);//FPS描画
extern VOID FPSWait(VOID);//FPS待機