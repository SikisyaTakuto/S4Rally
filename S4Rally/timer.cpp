#include "timer.h"

// 外部変数の初期化
int startTime = 0;
int lapStartTime = 0;
std::vector<int> laps;
GameState gameState = STATE_COUNTDOWN;

// 周回の基準となるポジション
const float goalMinX =-300.0f;
const float goalMaxX = 2500.0f;
const float goalMinZ = -25.0f;
const float goalMaxZ = 5.0f;
bool lapPassed = false; // 基準ポジションを通過したかどうかのフラグ

// 初期化処理
VOID TimerInit(VOID)
{
    startTime = GetNowCount();
    lapStartTime = 0;
    laps.clear();
    gameState = STATE_COUNTDOWN;
    lapPassed = false;
}

// 更新処理
VOID TimerUpdate(VOID)
{
    int currentTime = GetNowCount();

    if (gameState == STATE_COUNTDOWN)
    {
        // 5秒カウントダウン
        if ((currentTime - startTime) / 1000 >= 5)
        {
            gameState = STATE_RUNNING;
            lapStartTime = GetNowCount();
        }
    }
    else if (gameState == STATE_RUNNING)
    {
        // 基準ポジションを通過したかを確認
        if (!lapPassed && (carInfo.position.x >= goalMinX && carInfo.position.x <= goalMaxX && carInfo.position.z >= goalMinZ && carInfo.position.z <= goalMaxZ))
        {
            TimerRecordLap(); // 周回タイムを記録
            lapPassed = true; // 通過フラグを立てる

            // 3周完了したらゲームシーンを終了状態に変更
            if (laps.size() == 2)
            {
                gameState = STATE_FINISHED;
                ChangeGameScene = ResultScene;
            }
        }
        else if (lapPassed && (carInfo.position.z < goalMinZ || carInfo.position.z > goalMaxZ || carInfo.position.x < goalMinX || carInfo.position.x > goalMaxX))
        {
            // 基準ポジションから十分離れた場合にフラグをリセット
            lapPassed = false;
        }
    }
}

// 描画処理
VOID TimerDraw(VOID)
{
    int currentTime = GetNowCount();

    if (gameState == STATE_COUNTDOWN)
    {
        // カウントダウン表示
        int countdown = 5 - (currentTime - startTime) / 1000;
        //DrawFormatString(100, 100, GetColor(255, 255, 255), "Starting in: %d", countdown);

        DrawFormatStringToHandleAlign(
           640, 100, Align_Center, Color_tomato, fontJiyu25ptFuch.Handle, "カウント: %d", countdown);
    }
    else if (gameState == STATE_RUNNING)
    {
        // 現在の周回時間を表示
        int elapsedTime = currentTime - lapStartTime;
        DrawFormatString(0, 100, GetColor(255, 255, 255), "Current Lap Time: %.2f seconds", elapsedTime / 1000.0f);

        DrawFormatStringToHandleAlign(
            640, 100, Align_Center, Color_skyblue, fontJiyu25ptFuch.Handle,"タイム: %.2f seconds", elapsedTime / 1000.0f);

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
    }
}

// 周回タイムを記録
VOID TimerRecordLap(VOID)
{
    int currentTime = GetNowCount();
    laps.push_back(currentTime - lapStartTime);
    lapStartTime = currentTime; // 新しい周回の開始時間を更新
}