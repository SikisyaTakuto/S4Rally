#include "car.h"

//外部グローバル変数
CarInfo carInfo;

int tachometerImageHandle;
int needleImageHandle;

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

    transmissionInfo.gear = 1;                 // 現在のギア 
    transmissionInfo.maxGear = 5;
    carInfo.currentSpeed = 0.0f;               // 現在速度 (m/s)
    carInfo.maxSpeed = 250.0f;                 // 最大速度 (m/s)
    transmissionInfo.gearChangeLocked = false;          // ギアチェンジのロック

    engineInfo.engineRPM = 500.0f;               // エンジン回転数 (初期値 1000 RPM)
    engineInfo.idleRPM = 1000.0f;                 // アイドリング回転数
    engineInfo.redlineRPM =8200.0f;              // レッドゾーン開始回転数
    engineInfo.engineBrakeCoefficient = 4.0f;     // エンジンブレーキの効き

    carInfo.carModelHandle = MV1LoadModel("fbx/Silvia/SILVIABody.fbx"); // 車体モデル

    float wheelSpacingX = 150.2f; // 車の横幅 (左右のホイール間の距離)
    float wheelSpacingZ = 250.5f; // 車の縦幅 (前後のホイール間の距離)

    wheelCollider.mass = 10.0f;               // タイヤ質量 (kg)
    wheelCollider.radius = 0.5f;              // 半径 (メートル) 315mm
    wheelCollider.suspensionDistance = 0.0f;  // サスペンション初期距離 (メートル)
    wheelCollider.velocity = VGet(0.0f, 0.0f, 0.0f);

    transmissionInfo.gearRatios[0] = 3.5f;
    transmissionInfo.gearRatios[1] = 2.8f;
    transmissionInfo.gearRatios[2] = 2.2f;
    transmissionInfo.gearRatios[3] = 1.8f;
    transmissionInfo.gearRatios[4] = 1.5f;

    transmissionInfo.maxSpeeds[0] = 30.0f;
    transmissionInfo.maxSpeeds[1] = 45.0f;
    transmissionInfo.maxSpeeds[2] = 55.0f;
    transmissionInfo.maxSpeeds[3] = 65.0f;
    transmissionInfo.maxSpeeds[4] = 100.0f;

    // サスペンションスプリング
    suspensionSpring.spring = 500.0f;       // スプリング力
    suspensionSpring.damper = 1500.0f;      // 減衰力
    suspensionSpring.targetPosition = 0.5f; // 初期位置 (割合)

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

//アクセル
FLOAT CarApplyAcceleration(float deltaTime) {
    if (KeyDown(KEY_INPUT_W) || GetButton(BUTTON_RT)) {
        float engineForce = CarCulateEngineForce(transmissionInfo.gear);
        float acceleration = engineForce / carInfo.mass;
        carInfo.currentSpeed += acceleration * deltaTime;

        // ギアの最大速度を超えないようにする
        if (carInfo.currentSpeed > transmissionInfo.maxSpeeds[transmissionInfo.gear - 1]) {
            carInfo.currentSpeed = transmissionInfo.maxSpeeds[transmissionInfo.gear - 1];
        }
    }
    else {
        // エンジンブレーキ
        carInfo.currentSpeed -= engineInfo.engineBrakeCoefficient * deltaTime;
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

//// 重力を適用する関数
//VOID ApplyGravity(float deltaTime) {
//
//    float gravity = -9.81f*20.0f; // 重力加速度
//
//    // 4つのタイヤの判定結果をチェック
//    struct {
//        MV1_COLL_RESULT_POLY_DIM hitPolyDim;
//        VECTOR* hitPos;
//    } wheels[] = {
//        { stage.frontLeftHitPolyDim, &stage.frontLeftHitPos },
//        { stage.frontRightHitPolyDim, &stage.frontRightHitPos },
//        { stage.rearLeftHitPolyDim, &stage.rearLeftHitPos },
//        { stage.rearRightHitPolyDim, &stage.rearRightHitPos }
//    };
//
//    for (int i = 0; i < 4; i++) {
//
//        if (wheels[i].hitPolyDim.HitNum > 0.001f) {
//            car.frontLeft.center.y = stage.frontLeftHitPos.y;
//            car.frontRight.center.y = stage.frontRightHitPos.y;
//            car.rearLeft.center.y = stage.rearLeftHitPos.y;
//            car.rearRight.center.y = stage.rearRightHitPos.y;
//        }
//        else {
//
//            // タイヤが接触していない場合、重力を適用
//            if (stage.frontLeftHitPolyDim.HitNum == 0)
//            {
//                car.frontLeft.center.y += (gravity * deltaTime) / 4;
//            }
//
//            if (stage.frontRightHitPolyDim.HitNum == 0)
//            {
//                car.frontRight.center.y += (gravity * deltaTime) / 4;
//            }
//
//            if (stage.rearLeftHitPolyDim.HitNum == 0)
//            {
//                car.rearLeft.center.y += (gravity * deltaTime) / 4;
//            }
//
//            if (stage.rearRightHitPolyDim.HitNum == 0)
//            {
//                car.rearRight.center.y += (gravity * deltaTime) / 4;
//            }
//
//            // 4輪の高さの平均を求める
//            float avgWheelHeight = (
//                car.frontLeft.center.y +
//                car.frontRight.center.y +
//                car.rearLeft.center.y +
//                car.rearRight.center.y
//                ) / 4.0f;
//
//            // 車体の Y 位置を更新（ホイールの平均値を参考に）
//            carInfo.position.y = avgWheelHeight;
//        }
//    }
//}

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

    //// 重力の適用
    //ApplyGravity(deltaTime);

    // 摩擦を適用
    ApplyFriction(deltaTime);

    // ヨーモーメントとスリップアングルを考慮
    ApplyVehicleDynamics(deltaTime);

    // 車体のピッチとロールを更新
    UpdateCarPitchAndRoll();

    // 各ホイールにサスペンションを適用
    SuspensionDrow(deltaTime);

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
    float angle = minAngle + (maxAngle - minAngle) * (engineInfo.engineRPM / engineInfo.redlineRPM);

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
    DrawFormatString(10, 50, GetColor(255, 255, 255), "ギア: %d", transmissionInfo.gear);
    DrawFormatString(10, 70, GetColor(255, 255, 255), "ステアリング角度: %.2f 度", carInfo.steeringAngle);
    char buffer2[256];
    sprintf_s(buffer2, "CarInfo.position: X=%.2f Y=%.2f Z=%.2f", carInfo.position.x, carInfo.position.y, carInfo.position.z);
    DrawString(10, 290, buffer2, GetColor(255, 255, 255));
    //char buffer3[256];
    //sprintf_s(buffer3, "CameraPosition: X=%.2f Y=%.2f Z=%.2f", cameraPosition.x, cameraPosition.y, cameraPosition.z);
    //DrawString(10, 310, buffer3, GetColor(255, 255, 255));
}
