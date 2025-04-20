#include "suspension.h"

//外部グローバル変数
SuspensionSpring suspensionSpring;

// ヘルパー関数：Y軸回りにベクトルを回転（角度はラジアン）
VECTOR RotateVector(const VECTOR& vec, float angleRad) {
    VECTOR result;
    result.x = vec.x * cosf(angleRad) - vec.z * sinf(angleRad);
    result.y = vec.y;
    result.z = vec.x * sinf(angleRad) + vec.z * cosf(angleRad);
    return result;
}

// バイシクルモデルに基づく車両運動の更新
// 前輪のステアリング角 (carInfo.steeringAngle) と現在の速度から、車体の向きと位置、各ホイールの絶対位置を更新します。
VOID CarBicycleModelUpdate(float deltaTime)
{
    // CarInit で設定した値に基づくホイール配置パラメータ
    const float wheelSpacingX = 150.2f;       // 横幅（左右のホイール間距離）
    const float wheelSpacingZ = 250.5f;       // 縦幅（前後のホイール間距離）
    float halfTrack = wheelSpacingX / 2.0f;   // 車両左右半分
    float frontOffset = wheelSpacingZ / 1.7f;   // 前輪軸までの距離
    float rearOffset = -wheelSpacingZ / 2.4f;  // 後輪軸までの距離

    // ホイールベース（前輪軸と後輪軸の距離）
    float wheelBase = frontOffset - rearOffset; // L

    // 前輪のステアリング角（度→ラジアン）
    float steeringRad = carInfo.steeringAngle/* * (DX_PI / 180.0f)*/;

    // 現在の車体向き（yaw：carInfo.rotation.y を度→ラジアンに変換）
    float currentHeading = car.frontLeft.rotation.y * (DX_PI / 180.0f);

    // 角速度 ω = v / L * tan(δ)
    float angularVelocity = (fabsf(steeringRad) > 0.0001f) ? (carInfo.currentSpeed / wheelBase * tanf(steeringRad)) : 0.0f;

    // 車体の向きを更新
    float newHeading = currentHeading + angularVelocity * deltaTime;
    VECTOR direction = VSub(car.frontRight.center, car.rearRight.center); // 前輪と後輪の方向ベクトル
    carInfo.rotation.y = atan2f(direction.x, direction.z); // 車体の回転角度

    // 車体の中心位置を更新（進行方向に移動）
    carInfo.position.x += carInfo.currentSpeed * sinf(-newHeading);
    carInfo.position.z += carInfo.currentSpeed * cosf(newHeading);

    // 各ホイールのローカルオフセットから絶対位置を計算（車体の向き newHeading に合わせて回転）
    VECTOR localOffset;
    // 前輪左
    localOffset = VGet(-halfTrack, 0.0f, frontOffset);
    car.frontLeft.center = VAdd(carInfo.position, RotateVector(localOffset, newHeading));
    // 前輪右
    localOffset = VGet(halfTrack, 0.0f, frontOffset);
    car.frontRight.center = VAdd(carInfo.position, RotateVector(localOffset, newHeading));
    // 後輪左
    localOffset = VGet(-halfTrack, 0.0f, rearOffset);
    car.rearLeft.center = VAdd(carInfo.position, RotateVector(localOffset, newHeading));
    // 後輪右
    localOffset = VGet(halfTrack, 0.0f, rearOffset);
    car.rearRight.center = VAdd(carInfo.position, RotateVector(localOffset, newHeading));

    car.rearLeft.rotation.y = carInfo.rotation.y;
    car.rearRight.rotation.y = carInfo.rotation.y;

}

//サスペンションの処理
VOID ApplySuspension(WheelCollider wheel, VECTOR hitPos, float deltaTime) {
    // 重力加速度 (m/s^2)
    const float gravity = 9.81f;

    // サスペンションの初期長さ
    float restLength = wheel.suspensionDistance;

    // サスペンションが伸びる最大距離 (適宜調整)
    float maxExtension = 1.0f;

    // サスペンションによる力の初期化
    float springForce = 0.0f;
    float damperForce = 0.0f;

    // 車体の最低地上高 (タイヤ半径 + 余裕)
    float minHeight = wheel.radius + 0.5f;

    // 車の重量に基づく荷重を計算 (各ホイールにかかる重力)
    float wheelLoad = (carInfo.mass * gravity) / 4.0f;

    // 地面との距離を取得
    float groundHeight = hitPos.y;

    // ホイールの現在のY座標
    float wheelPosY = wheel.center.y;

    // サスペンションの圧縮量を計算
    float compression = restLength - (wheelPosY - groundHeight);

    // サスペンションの圧縮量が 0 より大きい場合のみ力を適用
    if (compression > 0.0f) {
        // フックの法則に基づくスプリング力 F = -k * x (k: バネ定数, x: 圧縮量)
        springForce = -suspensionSpring.spring * compression;

        // 減衰力 F_d = -c * v (c: 減衰係数, v: ホイールのY軸速度)
        damperForce = -suspensionSpring.damper * wheel.velocity.y;

        // 総合的な力の計算 (バネ力 + 減衰力 + ホイールにかかる荷重)
        float totalForce = springForce + damperForce + wheelLoad;

        // サスペンションの力を適用 (車体を上下に動かす)
        wheel.center.y += (totalForce / carInfo.mass) * deltaTime;
        carInfo.position.y += (totalForce / carInfo.mass) * deltaTime;
    }

    // サスペンションの最大伸びを超えないように制限
    float maxWheelPosY = groundHeight + restLength + maxExtension;
    if (wheel.center.y > maxWheelPosY) {
        wheel.center.y = maxWheelPosY; // 最大位置を超えたら制限
    }
    else {
        // 過度に沈み込まないように 0.01f ずつ下げる (サスペンションのバウンドを再現)
        wheel.center.y -= 0.01f;

        // ホイールのY座標が0未満にならないように制限
        if (wheel.center.y < 0.0f) wheel.center.y = 0.0f;
    }

    // 車体の最低高さを計算 (4輪の接地面の平均)
    float carMinHeight = (
        (car.frontLeft.center.y - wheel.radius) +
        (car.frontRight.center.y - wheel.radius) +
        (car.rearLeft.center.y - wheel.radius) +
        (car.rearRight.center.y - wheel.radius)
        ) / 4.0f;

    // 車体が最低地上高を下回らないように調整
    if (carInfo.position.y < carMinHeight + minHeight) {
        carInfo.position.y = carMinHeight + minHeight;
    }

}

// 車体のピッチ角とロール角を更新
VOID UpdateCarPitchAndRoll()
{
    // 各ホイールの高さ
    float frontLeftHeight = car.frontLeft.center.y;
    float frontRightHeight = car.frontRight.center.y;
    float rearLeftHeight = car.rearLeft.center.y;
    float rearRightHeight = car.rearRight.center.y;

    // 車両の前後ホイールの平均高さ
    float frontAvgHeight = (frontLeftHeight + frontRightHeight) * 0.5f;
    float rearAvgHeight = (rearLeftHeight + rearRightHeight) * 0.5f;

    // 車両の左右ホイールの平均高さ
    float leftAvgHeight = (frontLeftHeight + rearLeftHeight) * 0.5f;
    float rightAvgHeight = (frontRightHeight + rearRightHeight) * 0.5f;

    // ホイールベース（前後ホイール間の距離）
    float wheelBase = 250.5f; // 仮の値（必要に応じて変更）

    // トレッド幅（左右ホイール間の距離）
    float trackWidth = 150.2f; // 仮の値（必要に応じて変更）

    // ピッチ角の計算（前後ホイールの高さ差から）
    float pitchAngle = atan2f(-frontAvgHeight + rearAvgHeight, wheelBase);

    // ロール角の計算（左右ホイールの高さ差から）
    float rollAngle = atan2f(leftAvgHeight - rightAvgHeight, trackWidth);

    // 車体の回転に反映
    carInfo.rotation.x = pitchAngle; // ピッチ（X軸回転）
    carInfo.rotation.z = rollAngle;  // ロール（Z軸回転）
}

VOID SuspensionDrow(float deltaTime)
{
    // 各ホイールにサスペンションを適用
    ApplySuspension(car.frontLeft, stage.frontLeftHitPos, deltaTime);
    ApplySuspension(car.frontRight, stage.frontRightHitPos, deltaTime);
    ApplySuspension(car.rearLeft, stage.rearLeftHitPos, deltaTime);
    ApplySuspension(car.rearRight, stage.rearRightHitPos, deltaTime);

}