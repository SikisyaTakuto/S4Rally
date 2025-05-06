//ヘッダファイル読み込み
#include "suspension.h"

// コンストラクタ：各メンバ変数を初期化
SuspensionSpring::SuspensionSpring()
    : spring(35000.0f),    // スプリング定数（N/m）：スプリングの硬さを表す
    damper(4500.0f),     // 減衰係数（Ns/m）：減衰の強さを表す
    targetPosition(0.5f),// 目標位置（0.0 ～ 1.0）：サスペンションの中間点
    restLength(0.3f),    // 自然長（m）：スプリングが力を受けていないときの長さ
    currentLength(0.3f), // 現在の長さ（m）：初期状態では自然長と同じ
    velocity(0.0f)       // 伸縮速度（m/s）：初期状態では静止
{}

// デストラクタ：特別な処理は不要
SuspensionSpring::~SuspensionSpring() {}

// サスペンションに働く力を計算する関数
// deltaTime: 前回の更新からの経過時間（秒）
// newLength: 新しいスプリングの長さ（m）
float SuspensionSpring::CarCulateForce(float deltaTime, float newLength) {
    // 目標位置からの変位を計算
    float displacement = restLength * targetPosition - newLength;

    // フックの法則に基づくスプリング力を計算
    float springForce = spring * displacement;

    // 減衰力を計算（速度に比例）
    float dampingForce = damper * ((newLength - currentLength) / deltaTime);

    // 現在の長さを更新
    currentLength = newLength;

    // 合力（スプリング力 - 減衰力）を返す
    return springForce - dampingForce;
}