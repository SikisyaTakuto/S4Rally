#include "car.h"

//外部グローバル変数
WheelCollider wheelCollider;
SuspensionSpring suspensionSpring;
ForwardFriction forwardFriction;
SidewaysFriction sidewaysFriction;
Car car;
CarInfo carInfo;

// カメラ情報
VECTOR cameraPosition;
VECTOR cameraTarget;
float cameraDistance = 1000.0f; // カメラ距離
float cameraAngleX = 0.0f;      // カメラの上下角度
float cameraAngleY = 0.0f;      // カメラの水平角度
const float cameraZoomSpeed = 50.0f;   // ズーム速度
const float cameraRotateSpeed = 0.05f; // 回転速度

int tachometerImageHandle;
int needleImageHandle;

// ギア比の例: 1速: 3.5, 2速: 2.8, 3速: 2.2, 4速: 1.8, 5速: 1.5
float gearRatios[] = {3.5f, 2.8f, 2.2f, 1.8f, 1.5f};

// 最大速度テーブル（ギアごとに異なる）
float maxSpeeds[] = { 30.0f, 45.0f, 55.0f, 65.0f,100.0f }; // 各ギアの最大速度 (m/s)

// 車両情報の初期化
VOID CarInit(VOID) {

    carInfo.mass = 1500.0f;                    // 車両重量
    carInfo.drag = 1000.0f;                    //移動時の抵抗
    carInfo.engineForse = 1000.0f;             // エンジンの力 (N)
    carInfo.ballast = 0.0f;                    // 初期バラスト重量
    carInfo.acceleration = 0.0f;               // 加速度
    carInfo.brakeForce = 2500.0f;               // ブレーキ力
    carInfo.turboMultiplier = 1.5f;            // ターボ倍率
    carInfo.turboEnabled = false;              // ターボ無効
    carInfo.steeringAngle = 0.0f;              // 初期操舵角度
    carInfo.maxSteeringAngle = 35.0f;          // 最大操舵角度
    carInfo.minSteeringAngle = -35.0f;         // 最小操舵角度
    carInfo.position = VScale(VAdd(wheelCollider.frontWheelPosition, wheelCollider.rearWheelPosition), 0.5f); // 初期位置
    carInfo.rotation = VGet(0.0f, 0.0f, 0.0f); // 初期回転

    carInfo.gear = 1;                          // 現在のギア (1-5)
    carInfo.currentSpeed = 0.0f;               // 現在速度 (m/s)
    carInfo.maxSpeed = 250.0f;                 // 最大速度 (m/s)
    carInfo.gearChangeLocked = false;          // ギアチェンジのロック

    carInfo.engineRPM = 500.0f;               // エンジン回転数 (初期値 1000 RPM)
    carInfo.idleRPM = 1000.0f;                 // アイドリング回転数
    carInfo.redlineRPM =8200.0f;              // レッドゾーン開始回転数
    carInfo.engineBrakeCoefficient = 4.0f;     // エンジンブレーキの効き

    carInfo.carModelHandle = MV1LoadModel("fbx/Silvia/SILVIABody.fbx"); // 車体モデル

    float wheelSpacingX = 150.2f; // 車の横幅 (左右のホイール間の距離)
    float wheelSpacingZ = 250.5f; // 車の縦幅 (前後のホイール間の距離)

    wheelCollider.mass = 10.0f;               // タイヤ質量 (kg)
    wheelCollider.radius = 0.5f;              // 半径 (メートル) 315mm
    wheelCollider.suspensionDistance = 0.0f;  // サスペンション初期距離 (メートル)
    wheelCollider.velocity = VGet(0.0f, 0.0f, 0.0f);

    // サスペンションスプリング
    wheelCollider.suspensionSpring.spring = 500.0f;       // スプリング力
    wheelCollider.suspensionSpring.damper = 1500.0f;      // 減衰力
    wheelCollider.suspensionSpring.targetPosition = 0.5f; // 初期位置 (割合)

    // 摩擦設定
    wheelCollider.forwardFriction = 1.0f;  // 前方向摩擦
    wheelCollider.sidewaysFriction = 0.8f; // 横方向摩擦

    // タイヤモデルの読み込み
    wheelCollider.wheelModelHandleLeft = MV1LoadModel("fbx/Silvia/SILVIAWheel.fbx");
    wheelCollider.wheelModelHandleRight = MV1LoadModel("fbx/Silvia/SILVIAWheel1.fbx");

    // 各ホイールの位置

    //前輪（左）
    car.frontLeft = wheelCollider;
    car.frontLeft.center = VGet(-wheelSpacingX / 2, 0.0f, wheelSpacingZ / 1.7f);
    car.frontLeft.rotation = VGet(0.0f, 0.0f, 0.0f);

    //前輪（右）
    car.frontRight = wheelCollider;
    car.frontRight.center = VGet(wheelSpacingX / 2, 0.0f, wheelSpacingZ / 1.7f);
    car.frontRight.rotation = VGet(0.0f, 0.0f, 0.0f);

    //後輪（左）
    car.rearLeft = wheelCollider;
    car.rearLeft.center = VGet(-wheelSpacingX / 2, 0.0f, -wheelSpacingZ / 2.4f);
    car.rearLeft.rotation = VGet(0.0f, 0.0f, 0.0f);

    //後輪（右）
    car.rearRight = wheelCollider;
    car.rearRight.center = VGet(wheelSpacingX / 2, 0.0f, -wheelSpacingZ / 2.4f);
    car.rearRight.rotation = VGet(0.0f, 0.0f, 0.0f);

    //前輪
    wheelCollider.frontWheelPosition = VScale(VAdd(car.frontLeft.center, car.frontRight.center), 0.5f); 
    wheelCollider.frontWheelRotation = VScale(VAdd(car.frontLeft.rotation, car.frontRight.rotation), 0.5f);

    //後輪
    wheelCollider.rearWheelPosition = VScale(VAdd(car.rearLeft.center, car.rearRight.center), 0.5f);
    wheelCollider.rearWheelRotation = VScale(VAdd(car.rearLeft.rotation, car.rearRight.rotation), 0.5f);

    // デフォルトは後輪駆動
    car.driveType =RWD;

    tachometerImageHandle = LoadGraph("png/タコメータ.png");
    needleImageHandle = LoadGraph("png/ハリ.png");
}

// ギアによるエンジン力の変化
FLOAT CarCulateEngineForce(int gear) {
    // ギア比によってエンジン力を調整
    return carInfo.engineForse * gearRatios[gear-1];
}

// RPMの計算関数
FLOAT  CarCulateRPM(float velocity, int gear) {
    const float finalDriveRatio = 4.1f; // デファレンシャル比

    // エンジン回転数 (RPM) = (速度 / タイヤ半径) * ギア比 * デフ比 * 60 / (2π)
    float rpm = (velocity / wheelCollider.radius) * gearRatios[gear-1] * finalDriveRatio * 60.0f / (2.0f * DX_PI);

    return rpm;
}

// ギア変更処理
VOID ChangeGear(VOID) {
    // ギア変更がロックされていない場合のみ処理を行う
    if (!carInfo.gearChangeLocked) {
        // Xボタンが押され、現在のギアが最大ギア未満であればシフトアップ
        if (GetButton(BUTTON_X) && carInfo.gear < 5) {
            carInfo.gear++; // ギアを1つ上げる
            carInfo.gearChangeLocked = true; // 連続入力を防ぐためにロック
        }
        // Aボタンが押され、現在のギアが1より大きければシフトダウン
        if (GetButton(BUTTON_A) && carInfo.gear > 1) {
            carInfo.gear--; // ギアを1つ下げる
            carInfo.gearChangeLocked = true; // 連続入力を防ぐためにロック
        }
    }

    // XボタンとAボタンの両方が押されていない場合、ギア変更のロックを解除
    if ((!GetButton(BUTTON_X)) && (!GetButton(BUTTON_A))) {
        carInfo.gearChangeLocked = false; // キーを離したら解除
    }
}

// エンジン回転数の計算
VOID UpdateEngineRPM(VOID) {
    // 現在の速度とギアに基づいてエンジン回転数を計算
    carInfo.engineRPM = CarCulateRPM(carInfo.currentSpeed, carInfo.gear);

    // エンジン回転数がレッドラインを超え、かつギアが最大ギア未満であれば強制的にシフトアップ
    if (carInfo.engineRPM > carInfo.redlineRPM && carInfo.gear < 5) {
        carInfo.gear++; // ギアを1つ上げることでエンジンの過回転を防ぐ
    }
}

//アクセル
FLOAT CarApplyAcceleration(float deltaTime) {
    if (KeyDown(KEY_INPUT_W) || GetButton(BUTTON_RT)) {
        float engineForce = CarCulateEngineForce(carInfo.gear);
        float acceleration = engineForce / carInfo.mass;
        carInfo.currentSpeed += acceleration * deltaTime;

        // ギアの最大速度を超えないようにする
        if (carInfo.currentSpeed > maxSpeeds[carInfo.gear - 1]) {
            carInfo.currentSpeed = maxSpeeds[carInfo.gear - 1];
        }
    }
    else {
        // エンジンブレーキ
        carInfo.currentSpeed -= carInfo.engineBrakeCoefficient * deltaTime;
        if (carInfo.currentSpeed < 0.0f) {
            carInfo.currentSpeed = 0.0f;
        }
    }

    UpdateEngineRPM();
    return  carInfo.currentSpeed;
}

// ブレーキと後退処理
FLOAT CarApplyBraking(float deltaTime) {
    if (KeyDown(KEY_INPUT_S) || GetButton(BUTTON_LT)) {
        if (carInfo.currentSpeed > 0.0f) {
            // 前進中ならブレーキ
            carInfo.currentSpeed -= (carInfo.brakeForce / carInfo.mass) * deltaTime;
        }
        else
        {
            carInfo.currentSpeed -= 5.0f;
        }
    }
    return carInfo.currentSpeed;
}

//サイドブレーキ
VOID SideBrake(float deltaTime)
{
    // スティック入力の取得（左右）
    float axisX = GetAxisX();  // -1 (左) ～ 1 (右)

    if (GetButton(ControllerButton (BUTTON_RB))|| KeyDown(KEY_INPUT_SPACE) && (carInfo.currentSpeed > 0.1f|| carInfo.currentSpeed < -0.1f)) {
        // キー入力でステアリング角度を変更
        if (CheckHitKey(KEY_INPUT_D) || axisX >= -0.5f) {
            carInfo.steeringAngle += carInfo.minSteeringAngle * deltaTime; // 左回転
            // 最小角度を増加、最大角度を減少
            carInfo.minSteeringAngle -= 50.0f * deltaTime; // 徐々に最小角度を増加
            carInfo.maxSteeringAngle -= 50.0f * deltaTime; // 徐々に最大角度を減少

            carInfo.currentSpeed -= 20.0f * deltaTime;

            if (carInfo.currentSpeed<0.0f)
            {
                carInfo.currentSpeed = 0.0f;
            }
        }
        else
        {
            carInfo.currentSpeed -= 10.0f * deltaTime;
            if (carInfo.currentSpeed < 0.0f)
            {
                carInfo.currentSpeed = 0.0f;
            }
        }

        if (CheckHitKey(KEY_INPUT_A) || axisX <= 0.5f) {
            carInfo.steeringAngle += carInfo.maxSteeringAngle * deltaTime; // 右回転
            // 最小角度を減少、最大角度を増加
            carInfo.minSteeringAngle += 50.0f * deltaTime; // 徐々に最小角度を減少
            carInfo.maxSteeringAngle += 50.0f * deltaTime; // 徐々に最大角度を増加

            carInfo.currentSpeed -= 20.0f * deltaTime;
            if (carInfo.currentSpeed < 0.0f)
            {
                carInfo.currentSpeed = 0.0f;
            }
        }
        else
        {
            carInfo.currentSpeed -= 10.0f * deltaTime;
            if (carInfo.currentSpeed < 0.0f)
            {
                carInfo.currentSpeed = 0.0f;
            }
        }
    }
}

//操舵角度の設定
VOID CarApplySteering(VOID)
{
    // フレーム間の時
    float deltaTime = fps.Deltatime;

    // スティック入力の取得（左右）
    float axisX = GetAxisX();  // -1 (左) ～ 1 (右)

    // キー入力でステアリング角度を変更
    if ((CheckHitKey(KEY_INPUT_D) || axisX >= 0.5f)&&!stage.lockLeft) {
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

    ////車体が一周したら元に戻す
    //if (carInfo.steeringAngle <= -360 || carInfo.steeringAngle >= 360)
    //{
    //    carInfo.steeringAngle = 0;
    //    carInfo.minSteeringAngle = -50.0f;
    //    carInfo.maxSteeringAngle = 50.0f;
    //}

    // 前輪の角度を更新
    car.frontLeft.rotation.y = carInfo.steeringAngle;
    car.frontRight.rotation.y = carInfo.steeringAngle;

}

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
    carInfo.position.x += carInfo.currentSpeed  * sinf(-newHeading);
    carInfo.position.z += carInfo.currentSpeed  * cosf(newHeading);

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
    car.rearRight.center = VAdd(carInfo.position, RotateVector(localOffset,newHeading));

    car.rearLeft.rotation.y = carInfo.rotation.y;
    car.rearRight.rotation.y = carInfo.rotation.y;

}

//サスペンションの処理
VOID ApplySuspension(WheelCollider& wheel, VECTOR hitPos,float deltaTime) {
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
        springForce = -wheel.suspensionSpring.spring * compression;

        // 減衰力 F_d = -c * v (c: 減衰係数, v: ホイールのY軸速度)
        damperForce = -wheel.suspensionSpring.damper * wheel.velocity.y;

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

// 重力を適用する関数
VOID ApplyGravity(float deltaTime) {

    float gravity = -9.81f*20.0f; // 重力加速度

    // 4つのタイヤの判定結果をチェック
    struct {
        MV1_COLL_RESULT_POLY_DIM hitPolyDim;
        VECTOR* hitPos;
    } wheels[] = {
        { stage.frontLeftHitPolyDim, &stage.frontLeftHitPos },
        { stage.frontRightHitPolyDim, &stage.frontRightHitPos },
        { stage.rearLeftHitPolyDim, &stage.rearLeftHitPos },
        { stage.rearRightHitPolyDim, &stage.rearRightHitPos }
    };

    for (int i = 0; i < 4; i++) {

        if (wheels[i].hitPolyDim.HitNum > 0.001f) {
            car.frontLeft.center.y = stage.frontLeftHitPos.y;
            car.frontRight.center.y = stage.frontRightHitPos.y;
            car.rearLeft.center.y = stage.rearLeftHitPos.y;
            car.rearRight.center.y = stage.rearRightHitPos.y;
        }
        else {

            // タイヤが接触していない場合、重力を適用
            if (stage.frontLeftHitPolyDim.HitNum == 0)
            {
                car.frontLeft.center.y += (gravity * deltaTime) / 4;
            }

            if (stage.frontRightHitPolyDim.HitNum == 0)
            {
                car.frontRight.center.y += (gravity * deltaTime) / 4;
            }

            if (stage.rearLeftHitPolyDim.HitNum == 0)
            {
                car.rearLeft.center.y += (gravity * deltaTime) / 4;
            }

            if (stage.rearRightHitPolyDim.HitNum == 0)
            {
                car.rearRight.center.y += (gravity * deltaTime) / 4;
            }

            // 4輪の高さの平均を求める
            float avgWheelHeight = (
                car.frontLeft.center.y +
                car.frontRight.center.y +
                car.rearLeft.center.y +
                car.rearRight.center.y
                ) / 4.0f;

            // 車体の Y 位置を更新（ホイールの平均値を参考に）
            carInfo.position.y = avgWheelHeight;
        }
    }
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

VOID CarUpdate(VOID) {

    float deltaTime = fps.Deltatime;

    //前輪
    wheelCollider.frontWheelPosition = VScale(VAdd(car.frontLeft.center, car.frontRight.center), 0.5f);
    wheelCollider.frontWheelRotation = VScale(VAdd(car.frontLeft.rotation, car.frontRight.rotation), 0.5f);

    //後輪
    wheelCollider.rearWheelPosition = VScale(VAdd(car.rearLeft.center, car.rearRight.center), 0.5f);
    wheelCollider.rearWheelRotation = VScale(VAdd(car.rearLeft.rotation, car.rearRight.rotation), 0.5f);

    //carInfo.position = VScale(VAdd(wheelCollider.frontWheelPosition, wheelCollider.rearWheelPosition), 0.5f); // 初期位置
    carInfo.rotation = VScale(VAdd(wheelCollider.frontWheelRotation, wheelCollider.rearWheelRotation), 0.5f); // 初期回転

    //アクセル
     CarApplyAcceleration(deltaTime);

    //ブレーキ
    CarApplyBraking(deltaTime);

    //サイドブレーキ
    SideBrake(deltaTime);

    //ステアリング
    CarApplySteering();

    // バイシクルモデルに基づく運動更新
    CarBicycleModelUpdate(deltaTime);

    // ギア変更処理
    ChangeGear();

    // 重力の適用
    ApplyGravity(deltaTime);

    // 摩擦を適用
    ApplyFriction(deltaTime);

    // ヨーモーメントとスリップアングルを考慮
    ApplyVehicleDynamics(deltaTime);

    // 車体のピッチとロールを更新
    UpdateCarPitchAndRoll();

    // 各ホイールにサスペンションを適用
    ApplySuspension(car.frontLeft,stage.frontLeftHitPos ,deltaTime);
    ApplySuspension(car.frontRight,stage.frontRightHitPos ,deltaTime);
    ApplySuspension(car.rearLeft,stage.rearLeftHitPos ,deltaTime);
    ApplySuspension(car.rearRight,stage.rearRightHitPos ,deltaTime);

    //モデルの描画
    CarDraw();     

    //カメラの設定
    CarSetCamera(); 
}

//モデルの描画
VOID CarDraw(VOID)
{
    float wheelScale = wheelCollider.radius * 2.0f; // 直径に変換

    // 前輪（左）
    MV1SetScale(car.frontLeft.wheelModelHandleLeft, VGet(wheelScale, wheelScale, wheelScale));
    MV1SetPosition(car.frontLeft.wheelModelHandleLeft, car.frontLeft.center);
    MV1SetRotationXYZ(car.frontLeft.wheelModelHandleLeft, car.frontLeft.rotation = VGet(car.frontLeft.rotation.x, carInfo.rotation.y, car.frontLeft.rotation.z));
    MV1DrawModel(car.frontLeft.wheelModelHandleLeft);

    // 前輪（右）
    MV1SetScale(car.frontRight.wheelModelHandleRight, VGet(wheelScale, wheelScale, wheelScale));
    MV1SetPosition(car.frontRight.wheelModelHandleRight, car.frontRight.center);
    MV1SetRotationXYZ(car.frontRight.wheelModelHandleRight, car.frontRight.rotation= VGet(car.frontLeft.rotation.x, carInfo.rotation.y, car.frontLeft.rotation.z));
    MV1DrawModel(car.frontRight.wheelModelHandleRight);

    // 後輪（左）
    MV1SetScale(car.rearLeft.wheelModelHandleLeft, VGet(wheelScale, wheelScale, wheelScale));
    MV1SetPosition(car.rearLeft.wheelModelHandleLeft, car.rearLeft.center);
    MV1SetRotationXYZ(car.rearLeft.wheelModelHandleLeft, car.rearLeft.rotation);
    MV1DrawModel(car.rearLeft.wheelModelHandleLeft);

    // 後輪（右）
    MV1SetScale(car.rearRight.wheelModelHandleRight, VGet(wheelScale, wheelScale, wheelScale));
    MV1SetPosition(car.rearRight.wheelModelHandleRight, car.rearRight.center);
    MV1SetRotationXYZ(car.rearRight.wheelModelHandleRight, car.rearRight.rotation);
    MV1DrawModel(car.rearRight.wheelModelHandleRight);

    //車体
    MV1SetPosition(carInfo.carModelHandle, carInfo.position);   
    MV1SetRotationXYZ(carInfo.carModelHandle, carInfo.rotation);
    MV1DrawModel(carInfo.carModelHandle);


    // 画像の回転と拡大縮小
    DrawRotaGraph(1100, 100, 0.1f, 0.0f, tachometerImageHandle, TRUE);

    // 針の回転角度を計算 (最小角度 -135度、最大角度 135度)

    // メーターの針が指し示す最小角度（ラジアン単位）。-135度をラジアンに変換
    float minAngle = 45.0f * DX_PI / 180.0f;

    // メーターの針が指し示す最大角度（ラジアン単位）。135度をラジアンに変換
    float maxAngle = 235.0f * DX_PI / 180.0f;

    // 現在のエンジン回転数に基づき、針の角度を補間計算
    // carInfo.engineRPM が 0 のとき minAngle、carInfo.redlineRPM のとき maxAngle になる
    float angle = minAngle + (maxAngle - minAngle) * (carInfo.engineRPM / carInfo.redlineRPM);

    // 針の回転の基準点（メーター中心からのオフセット）

    // 針の回転中心の X 座標（メーターの中心）
    int needleCenterX = 1100;

    // 針の回転中心の Y 座標（メーターの中心）
    int needleCenterY = 100;

    // 針画像の回転軸（画像内のオフセット）
    // 画像の左上を基準として、針の回転軸がどこにあるかを指定

    // 画像内の針の回転中心の X 座標（左上基準）
    int needleOffsetX = 10;

    // 画像内の針の回転中心の Y 座標（左上基準）
    int needleOffsetY = 50;

    // 針の画像を回転描画
    // needleCenterX, needleCenterY を回転の基準点として、画像内の (needleOffsetX, needleOffsetY) を中心に回転
    // スケール 0.1f（画像を縮小）し、計算した angle の角度で回転
    // TRUE を指定してアルファブレンドを有効化
    DrawRotaGraph2(needleCenterX, needleCenterY, needleOffsetX, needleOffsetY, 0.1f, angle, needleImageHandle, TRUE);


    // 車の速度と加速度を表示
    DrawFormatString(10, 10, GetColor(255, 255, 255), "速度: %.2f m/s", carInfo.currentSpeed);
    DrawFormatString(10, 30, GetColor(255, 255, 255), "加速度: %.2f m/s^2", carInfo.acceleration);
    DrawFormatString(10, 50, GetColor(255, 255, 255), "ギア: %d", carInfo.gear);
    DrawFormatString(10, 70, GetColor(255, 255, 255), "ステアリング角度: %.2f 度", carInfo.steeringAngle);
    char buffer2[256];
    sprintf_s(buffer2, "CarInfo.position: X=%.2f Y=%.2f Z=%.2f", carInfo.position.x, carInfo.position.y, carInfo.position.z);
    DrawString(10, 290, buffer2, GetColor(255, 255, 255));
    char buffer3[256];
    sprintf_s(buffer3, "CameraPosition: X=%.2f Y=%.2f Z=%.2f", cameraPosition.x, cameraPosition.y, cameraPosition.z);
    DrawString(10, 310, buffer3, GetColor(255, 255, 255));
}

// 🔹 角度の補正（-180～180度範囲に収める）
float NormalizeAngle(float angle) {
    //while (angle > 360.0f) angle = 0.0f;
    //while (angle < -360.0f) angle = 0.0f;
    return angle;
}

 //カメラの設定
VOID CarSetCamera(VOID) {
        // カメラのパラメータ
        const float CAMERA_DISTANCE = 700.0f;   // 車の後方距離
        const float CAMERA_HEIGHT = 200.0f;     // カメラの高さ
        const float CAMERA_SMOOTHNESS = 0.1f;   // 追従スムージング

        float deltaTime = fps.Deltatime;

        //車の回転角度を取得（正規化）
        float targetAngleY = NormalizeAngle(-carInfo.steeringAngle*deltaTime);

        //角度補間をスムーズに処理（急激な回転を防ぐ）
        float angleDiff = NormalizeAngle(targetAngleY - cameraAngleY);
        cameraAngleY += angleDiff * CAMERA_SMOOTHNESS;

        // 車の向きを表す回転行列
        MATRIX carMatrix = MGetRotY(cameraAngleY);

        // 車の後方にオフセットを適用
        VECTOR offset = VTransform(VGet(0.0f, CAMERA_HEIGHT, -CAMERA_DISTANCE), carMatrix);

        // 目標カメラ位置を計算
        VECTOR targetCameraPos = VAdd(carInfo.position, offset);

        // スムーズにカメラを補間
        cameraPosition = VAdd(VScale(cameraPosition, 1.0f - CAMERA_SMOOTHNESS),
            VScale(targetCameraPos, CAMERA_SMOOTHNESS));

        // カメラを設定
        SetCameraPositionAndTarget_UpVecY(cameraPosition, carInfo.position);
}