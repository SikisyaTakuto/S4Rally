#include "camera.h"

void Camera::Init()
{
    SetCameraNearFar(1.0f, 10000.0f);
}

void Camera::Update()
{
    extern Car car; // 外部変数として車の情報を取得

    // 車の位置と回転を取得
    VECTOR carPos = car.GetPosition();
    VECTOR carRot = car.GetRotation();

    // 車の後ろ方向を計算
    VECTOR backDir = VGet(-sinf(carRot.y), 0.0f, -cosf(carRot.y));
    backDir = VNorm(backDir); // 正規化（方向のみ）

    // カメラと車の距離、カメラの高さ
    float distance = 500.0f;
    float heightOffset = 300.0f;

    // カメラの位置を計算（車の後ろに配置）
    VECTOR cameraPos = VAdd(carPos, VScale(backDir, distance));
    cameraPos.y += heightOffset; // 高さの調整

    // 注視点を車の少し上に設定
    VECTOR target = carPos;
    target.y += 50.0f;  // 車の上の方に注視点を少し調整

    // カメラの位置と注視点を設定
    SetCameraPositionAndTarget_UpVecY(cameraPos, target);

    //// F1キーでデバッグモードON/OFF切り替え
    //if (CheckHitKey(KEY_INPUT_F1) && !debugMode) {
    //    debugMode = true;
    //}
    //else if (CheckHitKey(KEY_INPUT_F2) && debugMode) {
    //    debugMode = false;
    //}

    //if (debugMode) {
    //    // デバッグカメラ操作
    //    const float moveSpeed = 10.0f;
    //    const float rotSpeed = 0.02f;

    //    // 回転（矢印キー）
    //    if (CheckHitKey(KEY_INPUT_LEFT)) {
    //        debugCamYaw -= rotSpeed;
    //    }
    //    if (CheckHitKey(KEY_INPUT_RIGHT)) {
    //        debugCamYaw += rotSpeed;
    //    }
    //    if (CheckHitKey(KEY_INPUT_UP)) {
    //        debugCamPitch -= rotSpeed;
    //    }
    //    if (CheckHitKey(KEY_INPUT_DOWN)) {
    //        debugCamPitch += rotSpeed;
    //    }

    //    // ピッチ制限
    //    if (debugCamPitch > DX_PI_F / 2.0f) debugCamPitch = DX_PI_F / 2.0f;
    //    if (debugCamPitch < -DX_PI_F / 2.0f) debugCamPitch = -DX_PI_F / 2.0f;

    //    // 前方ベクトルを計算
    //    VECTOR forward = VGet(
    //        cosf(debugCamPitch) * sinf(debugCamYaw),
    //        sinf(debugCamPitch),
    //        cosf(debugCamPitch) * cosf(debugCamYaw)
    //    );
    //    forward = VNorm(forward);

    //    // 右ベクトル
    //    VECTOR right = VCross(VGet(0.0f, 1.0f, 0.0f), forward);
    //    right = VNorm(right);

    //    // 上下移動
    //    VECTOR up = VGet(0.0f, 1.0f, 0.0f);

    //    // 移動（WASDQEキー）
    //    if (CheckHitKey(KEY_INPUT_I)) {
    //        debugCamPos = VAdd(debugCamPos, VScale(forward, moveSpeed));
    //    }
    //    if (CheckHitKey(KEY_INPUT_K)) {
    //        debugCamPos = VSub(debugCamPos, VScale(forward, moveSpeed));
    //    }
    //    if (CheckHitKey(KEY_INPUT_J)) {
    //        debugCamPos = VSub(debugCamPos, VScale(right, moveSpeed));
    //    }
    //    if (CheckHitKey(KEY_INPUT_L)) {
    //        debugCamPos = VAdd(debugCamPos, VScale(right, moveSpeed));
    //    }
    //    if (CheckHitKey(KEY_INPUT_U)) {
    //        debugCamPos = VSub(debugCamPos, VScale(up, moveSpeed));
    //    }
    //    if (CheckHitKey(KEY_INPUT_O)) {
    //        debugCamPos = VAdd(debugCamPos, VScale(up, moveSpeed));
    //    }

    //    // 注視点を計算
    //    VECTOR target = VAdd(debugCamPos, forward);

    //    // カメラ設定
    //    SetCameraPositionAndTarget_UpVecY(debugCamPos, target);
    //}
    //else {
    //    // 通常カメラ（車体追従）
    //    VECTOR carPos = car.GetPosition();
    //    VECTOR carRot = car.GetRotation();

    //    VECTOR backDir = VGet(-sinf(carRot.y), 0.3f, -cosf(carRot.y));
    //    backDir = VNorm(backDir);

    //    float distance = 500.0f;
    //    float heightOffset = 300.0f;

    //    VECTOR cameraPos = VAdd(carPos, VScale(backDir, distance));
    //    cameraPos.y += heightOffset;

    //    VECTOR target = carPos;
    //    target.y += 50.0f;

    //    SetCameraPositionAndTarget_UpVecY(cameraPos, target);
    //}
}