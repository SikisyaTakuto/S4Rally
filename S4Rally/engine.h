#pragma once

#include "DxLib.h"

// 前方宣言（循環依存防止）
class Transmission;

class Engine {
public:
    float engineRPM;                 // エンジン回転数
    float idleRPM;                  // アイドリング回転数
    float maxRPM;                   // 最大回転数
    float redlineRPM;              // レッドゾーン開始回転数
    float engineBrakeCoefficient;  // エンジンブレーキの効き

    int carModelHandle;            // 車体モデルハンドル

    Engine();

    void Update(float velocity, Transmission& transmission); // RPM更新処理（ギアとの連携）

    // 各種ゲッター
    float GetRPM() const;
    float GetBrakeCoefficient() const;

    // セッター（必要に応じて）
    void SetCarModelHandle(int handle);
    void SetRPM(float rpm);
};