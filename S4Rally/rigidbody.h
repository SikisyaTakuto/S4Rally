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
    float mass = 1.0f;           // 質量（Kg単位。0以下にすると物理演算無効）
    float drag = 0.0f;           // 移動時の空気抵抗係数（減速に使用）
    float angularDrag = 0.05f;   // 回転時の空気抵抗係数（今回は未使用）
    bool isGravity = true;       // 重力を適用するかどうか（trueなら重力加速度が働く）
    bool isGrounded = false;     // 地面に接触しているか（trueなら重力が働かない）

    //状態管理
    VECTOR position = { 0, 0, 0 }; // 現在のワールド座標
    VECTOR velocity = { 0, 0, 0 }; // 現在の移動速度ベクトル
    VECTOR force = { 0, 0, 0 };    // 今フレームで加えられた合計の力

    //メソッド

    // コンストラクタ
    // 初期化処理を行う（質量、抵抗値、位置など）
    RigidBody();

    // 指定された力を加算する
    // 力は次のUpdate()時に速度や位置に反映される
    void AddForce(VECTOR f);

    // 毎フレーム呼び出し、物理状態を更新する
    // deltaTime: 1フレームあたりの経過時間（秒）
    void Update(float deltaTime);
}