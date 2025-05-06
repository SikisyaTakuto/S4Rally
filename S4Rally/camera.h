#pragma once

//ヘッダファイル読み込み
#include"DxLib.h"
#include"fps.h"
#include"car.h"

class Camera
{
public:
    void Init();
    void Update();  // Carの参照を渡す

private:
    bool debugMode = false; // デバッグモードかどうか
    VECTOR debugCamPos = VGet(0.0f, 300.0f, -500.0f); // デバッグカメラ初期位置
    float debugCamYaw = 0.0f;  // 水平方向回転
    float debugCamPitch = 0.0f; // 上下方向回転
};