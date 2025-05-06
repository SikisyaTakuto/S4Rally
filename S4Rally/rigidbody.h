//rigidbodyのヘッダファイル
#pragma once

//ヘッダファイル読み込み
#include "DxLib.h"
#include "fps.h"
#include "wheelcollider.h"

class RigidBody
{
    public:
        // 物理プロパティ
        float mass = 1.0f;           // 質量 (Kg)
        float drag = 0.0f;           // 空気抵抗
        float angularDrag = 0.05f;   // 回転抵抗（今回は未実装）
        bool isGravity = true;       // 重力を受けるか
        bool isGrounded = false;     // 地面に接触しているか

        // 状態
        VECTOR position = { 0, 0, 0 }; // 現在の位置
        VECTOR velocity = { 0, 0, 0 }; // 現在の速度
        VECTOR force = { 0, 0, 0 };    // 次のフレームで適用する力

        // コンストラクタ
        RigidBody();

        // 力を加える
        void AddForce(VECTOR f);

        // 毎フレーム更新
        void Update(float deltaTime);
};