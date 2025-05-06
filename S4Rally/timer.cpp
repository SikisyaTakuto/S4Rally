#include "timer.h"

// 外部変数の初期化
int startTime = 0;
int lapStartTime = 1;
std::vector<int> laps;
GameState gameState = STATE_COUNTDOWN;

// 周回の基準となるポジション
const float goalMinX =4100.0f;
const float goalMaxX = 4200.0f;
const float goalMinZ = -3700.0f; 
const float goalMaxZ = -2650.0f;
bool lapPassed = false; // 基準ポジションを通過したかどうかのフラグ

int finishTime = 0; // STATE_FINISHEDに入った時刻
const int returnToTitleDelay = 10000; // 10秒後（ミリ秒）

// --- 追加：チェックポイントエリア ---
const float checkpointMinX = 4100.0f;
const float checkpointMaxX = 4200.0f;
const float checkpointMinZ = 4700.0f; 
const float checkpointMaxZ = 6000.0f;
bool checkpointPassed = false; // チェックポイントを通過したか

// 初期化処理
VOID TimerInit(VOID)
{
    startTime = GetNowCount();
    lapStartTime = 0;
    laps.clear();
    gameState = STATE_COUNTDOWN;
    lapPassed = false;
    checkpointPassed = true;
}

// 更新処理
VOID TimerUpdate(VOID)
{
    extern Car car;
    int currentTime = GetNowCount();

    if (gameState == STATE_COUNTDOWN)
    {
        gameState = STATE_RUNNING;
    }
    else if (gameState == STATE_RUNNING)
    {
        // --- チェックポイント通過判定 ---
        if (!checkpointPassed &&
            car.carBodyPosition.x >= checkpointMinX && car.carBodyPosition.x <= checkpointMaxX &&
            car.carBodyPosition.z >= checkpointMinZ && car.carBodyPosition.z <= checkpointMaxZ)
        {
            checkpointPassed = true; // チェックポイント通過！
        }

        // --- ゴールエリア通過判定 ---
        if (checkpointPassed && !lapPassed &&
            car.carBodyPosition.x >= goalMinX && car.carBodyPosition.x <= goalMaxX &&
            car.carBodyPosition.z >= goalMinZ && car.carBodyPosition.z <= goalMaxZ)
        {
            if (lapStartTime == 0)
            {
                lapStartTime = GetNowCount();
            }
            else
            {
                TimerRecordLap();
            }
            lapPassed = true;
            checkpointPassed = false; // チェックポイントフラグをリセット！
        }
        else if (lapPassed && (car.carBodyPosition.z < goalMinZ || car.carBodyPosition.z > goalMaxZ ||
            car.carBodyPosition.x < goalMinX || car.carBodyPosition.x > goalMaxX))
        {
            // ゴールエリアから出たらリセット
            lapPassed = false;
        }
    }
    else if (gameState == STATE_FINISHED)
    {
        if (GetNowCount() - finishTime >= returnToTitleDelay)
        {
            ChangeGameScene = TitleScene; // 10秒経ったらタイトルに戻る
        }
    }
}

// 描画処理
VOID TimerDraw(VOID)
{
    int currentTime = GetNowCount();

    if (gameState == STATE_RUNNING)
    {
        // 現在の周回時間を表示
        int elapsedTime = currentTime - lapStartTime;

        if (lapStartTime != 0)
        {
            DrawFormatStringToHandleAlign(
                640, 100, Align_Center, Color_skyblue, fontJiyu25ptFuch.Handle, "タイム: %.2f seconds", elapsedTime / 1000.0f);
        }

        // 記録済みの周回タイムを表示
        int yOffset = 100;
        for (size_t i = 0; i < laps.size(); ++i)
        {
            DrawFormatString(1000, yOffset, GetColor(0, 255, 0), "Lap %d: %.2f seconds", i + 1, laps[i] / 1000.0f);
            yOffset += 20;
        }
    }
    else if (gameState == STATE_FINISHED)
    {
        // 3周のタイムを表示
        int minLapIndex = 0;
        for (size_t i = 1; i < laps.size(); ++i)
        {
            if (laps[i] < laps[minLapIndex])
            {
                minLapIndex = i;
            }
        }

        int yOffset = 100;
        for (size_t i = 0; i < laps.size(); ++i)
        {
            int color = (i == minLapIndex) ? GetColor(255, 0, 0) : GetColor(0, 0, 0); // 最速タイムは赤、その他は黒
            //DrawFormatString(100, yOffset, color, "Lap %d: %.2f seconds", i + 1, laps[i] / 1000.0f);

            DrawFormatStringToHandleAlign(
                640, yOffset, Align_Center, color, fontJiyu25ptFuch.Handle, "ラップタイム %d: %.2f seconds", i + 1, laps[i] / 1000.0f);
            yOffset += 200;
        }

        // タイトルに戻るまでの秒数を表示
        int remainingTime = (returnToTitleDelay - (GetNowCount() - finishTime)) / 1000;
        if (remainingTime < 0) remainingTime = 0;
        DrawFormatStringToHandleAlign(
            640,650, Align_Center, GetColor(0, 0, 0), fontJiyu25ptFuch.Handle,
            "タイトルに戻るまで: %d秒", remainingTime);
    }
}

// 周回タイムを記録
VOID TimerRecordLap(VOID)
{
    int currentTime = GetNowCount();
    laps.push_back(currentTime - lapStartTime);
    lapStartTime = currentTime; // 新しい周回の開始時間を更新

    // 3周完了でリザルトへ
    if (laps.size() >= 3)
    {
        gameState = STATE_FINISHED;
        ChangeGameScene = ResultScene;
        finishTime = GetNowCount();
    }
}