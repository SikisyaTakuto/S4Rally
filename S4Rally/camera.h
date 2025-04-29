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
};