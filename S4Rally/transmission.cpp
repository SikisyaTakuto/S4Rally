#include "transmission.h"

TransmissionInfo transmissionInfo;

// RPMの計算関数
FLOAT  CarCulateRPM(float velocity, int gear) {
    const float finalDriveRatio = 4.1f; // デファレンシャル比

    // エンジン回転数 (RPM) = (速度 / タイヤ半径) * ギア比 * デフ比 * 60 / (2π)
    float rpm = (velocity / wheelCollider.radius) * transmissionInfo.gearRatios[gear - 1] * finalDriveRatio * 60.0f / (2.0f * DX_PI);

    return rpm;
}

// ギアによるエンジン力の変化
FLOAT CarCulateEngineForce(int gear) {
    // ギア比によってエンジン力を調整
    return carInfo.engineForse * transmissionInfo.gearRatios[gear - 1];
}

// ギア変更処理
VOID ChangeGear(VOID) {
    // ギア変更がロックされていない場合のみ処理を行う
    if (!transmissionInfo.gearChangeLocked) {
        // Xボタンが押され、現在のギアが最大ギア未満であればシフトアップ
        if (GetButton(BUTTON_X) && transmissionInfo.gear < 5) {
            transmissionInfo.gear++; // ギアを1つ上げる
            transmissionInfo.gearChangeLocked = true; // 連続入力を防ぐためにロック
        }
        // Aボタンが押され、現在のギアが1より大きければシフトダウン
        if (GetButton(BUTTON_A) && transmissionInfo.gear > 1) {
            transmissionInfo.gear--; // ギアを1つ下げる
            transmissionInfo.gearChangeLocked = true; // 連続入力を防ぐためにロック
        }
    }

    // XボタンとAボタンの両方が押されていない場合、ギア変更のロックを解除
    if ((!GetButton(BUTTON_X)) && (!GetButton(BUTTON_A))) {
        transmissionInfo.gearChangeLocked = false; // キーを離したら解除
    }
}