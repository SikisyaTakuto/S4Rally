#include "engine.h"

EngineInfo engineInfo;

// エンジン回転数の計算
VOID UpdateEngineRPM(VOID) {
    // 現在の速度とギアに基づいてエンジン回転数を計算
    engineInfo.engineRPM = CarCulateRPM(carInfo.currentSpeed, transmissionInfo.gear);

    // エンジン回転数がレッドラインを超え、かつギアが最大ギア未満であれば強制的にシフトアップ
    if (engineInfo.engineRPM >engineInfo.redlineRPM && transmissionInfo.gear < 5) {
       transmissionInfo.gear++; // ギアを1つ上げることでエンジンの過回転を防ぐ
    }
}