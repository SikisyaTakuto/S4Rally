#include "camera.h"

// カメラ情報
VECTOR cameraPosition;
VECTOR cameraTarget;
float cameraDistance = 1000.0f; // カメラ距離
float cameraAngleX = 0.0f;      // カメラの上下角度
float cameraAngleY = 0.0f;      // カメラの水平角度
const float cameraZoomSpeed = 50.0f;   // ズーム速度
const float cameraRotateSpeed = 0.05f; // 回転速度


// ?? 角度の補正（-180～180度範囲に収める）
float NormalizeAngle(float angle) {
    while (angle > 360.0f) angle = 0.0f;
    while (angle < -360.0f) angle = 0.0f;
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
    float targetAngleY = NormalizeAngle(-carInfo.steeringAngle * deltaTime);

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