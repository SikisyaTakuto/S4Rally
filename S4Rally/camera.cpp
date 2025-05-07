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
}