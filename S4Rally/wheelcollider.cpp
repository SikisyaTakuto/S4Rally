#include "wheelcollider.h"

//外部グローバル変数
WheelCollider wheelCollider;
ForwardFriction forwardFriction;
SidewaysFriction sidewaysFriction;
Car car;

//操舵角度の設定
VOID CarApplySteering(VOID)
{
    // フレーム間の時
    float deltaTime = fps.Deltatime;

    // スティック入力の取得（左右）
    float axisX = GetAxisX();  // -1 (左) ～ 1 (右)

    // キー入力でステアリング角度を変更
    if ((CheckHitKey(KEY_INPUT_D) || axisX >= 0.5f) && !stage.lockLeft) {
        carInfo.steeringAngle += carInfo.minSteeringAngle * deltaTime; // 左回転
        // 最小角度を増加、最大角度を減少
        carInfo.minSteeringAngle -= 50.0f * deltaTime; // 徐々に最小角度を増加
        carInfo.maxSteeringAngle -= 50.0f * deltaTime; // 徐々に最大角度を減少
    }

    if ((CheckHitKey(KEY_INPUT_A) || axisX <= -0.5f) && !stage.lockRight) {
        carInfo.steeringAngle += carInfo.maxSteeringAngle * deltaTime; // 右回転
        // 最小角度を減少、最大角度を増加
        carInfo.minSteeringAngle += 50.0f * deltaTime; // 徐々に最小角度を減少
        carInfo.maxSteeringAngle += 50.0f * deltaTime; // 徐々に最大角度を増加
    }

    // ステアリング角度の制限 
    float maxSteeringRad = carInfo.maxSteeringAngle * (DX_PI / 180.0f);
    float minSteeringRad = carInfo.minSteeringAngle * (DX_PI / 180.0f);

    float steeringAngleRad = carInfo.steeringAngle * (DX_PI / 180.0f);

    if (steeringAngleRad > maxSteeringRad) {
        steeringAngleRad = maxSteeringRad;
    }
    if (steeringAngleRad < minSteeringRad) {
        steeringAngleRad = minSteeringRad;
    }

    carInfo.steeringAngle = steeringAngleRad * (180.0f / DX_PI); // ラジアンから度に変換

    //車体が一周したら元に戻す
    if (carInfo.steeringAngle <= -360 || carInfo.steeringAngle >= 360)
    {
        carInfo.steeringAngle = 0;
        carInfo.minSteeringAngle = -50.0f;
        carInfo.maxSteeringAngle = 50.0f;
    }

    // 前輪の角度を更新
    car.frontLeft.rotation.y = carInfo.steeringAngle;
    car.frontRight.rotation.y = carInfo.steeringAngle;

}

// ヨーモーメントとスリップアングルを考慮
VOID ApplyVehicleDynamics(float deltaTime)
{
    float wheelBase = 2.5f; // ホイールベース（仮値）
    float yawMoment = 0.0f;

    // スティック入力の取得（左右）
    float axisX = GetAxisX();  // -1 (左) ～ 1 (右)

    // 過去のステアリング入力を保存
    static float prevSteering = 0.0f;
    static bool flickInitiated = false; // フリック開始判定
    static float flickTimer = 0.0f; // フリックの持続時間

    // スリップアングルの計算
    float slipAngleFront = atan2(carInfo.currentSpeed * sin(carInfo.steeringAngle * (DX_PI / 180.0f)), carInfo.currentSpeed);
    float slipAngleRear = atan2(carInfo.currentSpeed * sin(-carInfo.steeringAngle * (DX_PI / 180.0f)), carInfo.currentSpeed);

    // タイヤの横摩擦によるヨーモーメント
    float frontLateralForce = -slipAngleFront * 2000.0f; // 仮の値
    float rearLateralForce = -slipAngleRear * 1800.0f; // 仮の値

    // 車体の回転に影響
    yawMoment = (frontLateralForce * (wheelBase / 2.0f)) - (rearLateralForce * (wheelBase / 2.0f));

    // 角速度の適用（慣性考慮）
    float angularAcceleration = yawMoment / carInfo.mass;
    wheelCollider.rearWheelRotation.y += angularAcceleration * deltaTime;

    // 現在のステアリングを保存
    prevSteering = axisX;
}

// 摩擦を適用する関数
VOID ApplyFriction(float deltaTime)
{
    float throttle = CarApplyAcceleration(deltaTime);
    float brake = CarApplyBraking(deltaTime);

    // 摩擦の適用
    float baseForwardFriction = wheelCollider.forwardFriction;
    float baseSidewaysFriction = wheelCollider.sidewaysFriction;

    if (throttle > 0.1f) // アクセル時は摩擦を減らす
    {
        car.frontLeft.forwardFriction = baseForwardFriction * 0.9f;
        car.frontRight.forwardFriction = baseForwardFriction * 0.9f;
        car.rearLeft.forwardFriction = baseForwardFriction * 0.8f;
        car.rearRight.forwardFriction = baseForwardFriction * 0.8f;
    }
    else if (brake > 0.1f) // ブレーキ時は摩擦を増やす
    {
        car.frontLeft.forwardFriction = baseForwardFriction * 1.2f;
        car.frontRight.forwardFriction = baseForwardFriction * 1.2f;
        car.rearLeft.forwardFriction = baseForwardFriction * 1.5f;
        car.rearRight.forwardFriction = baseForwardFriction * 1.5f;
    }
    else // アクセルオフ時（エンジンブレーキ）
    {
        car.frontLeft.forwardFriction += (baseForwardFriction - car.frontLeft.forwardFriction) * deltaTime;
        car.frontRight.forwardFriction += (baseForwardFriction - car.frontRight.forwardFriction) * deltaTime;
        car.rearLeft.forwardFriction += (baseForwardFriction - car.rearLeft.forwardFriction) * deltaTime;
        car.rearRight.forwardFriction += (baseForwardFriction - car.rearRight.forwardFriction) * deltaTime;
    }

    // 横方向の摩擦をステアリング角度によって調整
    float steeringFactor = fabsf(carInfo.steeringAngle) / (carInfo.maxSteeringAngle + carInfo.minSteeringAngle); // 0 ～ 1
    float driftFriction = wheelCollider.sidewaysFriction * (1.0f - 0.5f * steeringFactor); // ステア角が大きいほど摩擦減少

    car.frontLeft.sidewaysFriction = driftFriction;
    car.frontRight.sidewaysFriction = driftFriction;
    car.rearLeft.sidewaysFriction = driftFriction;
    car.rearRight.sidewaysFriction = driftFriction;

}