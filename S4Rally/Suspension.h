#pragma once

#include "DxLib.h"

class SuspensionSpring {
public:
    float spring;           // スプリング定数 (N/m)
    float damper;           // 減衰係数 (Ns/m)
    float targetPosition;   // サスペンションの目標位置 (0.0 ～ 1.0)
    float restLength;       // サスペンションの自然長 (m)
    float currentLength;    // 現在のサスペンション長 (m)
    float velocity;         // サスペンションの伸縮速度 (m/s)

    // コンストラクタ・デストラクタ 
    SuspensionSpring();
    ~SuspensionSpring();

    // サスペンションの力を計算する
    float CalculateForce(float deltaTime, float newLength);

};
