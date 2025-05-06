//マップ処理のヘッダファイル
#pragma once

//ヘッダファイル読み込み
#include "DxLib.h"
#include "car.h"
#include "wheelcollider.h"
#include "rigidbody.h"

// 定数定義
#define LINE_AREA_SIZE 1000000.0f // ラインを描く範囲
#define LINE_NUM 1000             // ラインの数

class Map
{
public:

    int modelHandle;  // モデルハンドル

    Map();          // コンストラクタ
    ~Map();         // デストラクタ

    void Init();    // マップ初期化
    void Update();  // マップ更新（必要なら）
    void Draw();    // マップ描画（モデル＋ライン）
};