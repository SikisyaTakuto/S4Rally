#include "camera.h"

void Camera::Init()
{
    SetCameraNearFar(1.0f, 10000.0f);
}

void Camera::Update()
{
    extern Car car; // 外部変数

    // 車体の位置と回転を取得
    VECTOR carPos = car.GetPosition();       // 車の位置
    VECTOR carRot = car.GetRotation();       // 車の回転（Y軸）

    // 後ろ方向（車の後方ベクトル）
    VECTOR backDir = VGet(-sinf(carRot.y), 0.3f, -cosf(carRot.y));
    backDir = VNorm(backDir); // 正規化

    float distance = 500.0f;    // 車からの距離
    float heightOffset = 200.0f; // 高さオフセット

    // カメラ位置を計算（車の後方上空）
    VECTOR cameraPos = VAdd(carPos, VScale(backDir, distance));
    cameraPos.y += heightOffset;

    // 注視点は車の少し上
    VECTOR target = carPos;
    target.y += 50.0f;

    // カメラ設定
    SetCameraPositionAndTarget_UpVecY(cameraPos, target);
}