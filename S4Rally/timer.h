#pragma once

#include "DxLib.h"
#include <vector>
#include <string>
#include "game.h"
#include "font.h"
#include "car.h"


// ゲームステートの列挙型
enum GameState
{
    STATE_COUNTDOWN,
    STATE_RUNNING,
    STATE_FINISHED
};

extern int startTime;           // カウントダウン開始時間
extern int lapStartTime;        // 周回開始時間
extern std::vector<int> laps;   // 周回タイムを記録する配列
extern GameState gameState;     // 現在のゲームステート
extern bool isRunning;          // 計測中フラグ

// 外部プロトタイプ宣言
extern VOID TimerInit(VOID);
extern VOID Timer(VOID);
extern VOID TimerUpdate(VOID);           // ゲームロジック更新
extern VOID TimerDraw(VOID);             // 描画処理
extern VOID TimerRecordLap(VOID);        // 周回タイム記録
