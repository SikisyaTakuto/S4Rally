#include "engine.h"
#include "transmission.h"

// RPM�̌v�Z�֐�
static float CarCulateRPM(float velocity, float wheelRadius, float gearRatio) {
    const float finalDriveRatio = 4.1f;
    return (velocity / wheelRadius) * gearRatio * finalDriveRatio * 60.0f / (2.0f * DX_PI);
}

Engine::Engine() {
    engineRPM = 1000.0f;
    idleRPM = 800.0f;
    maxRPM = 13000.0f;
    redlineRPM = 11000.0f;
    engineBrakeCoefficient = 0.02f;

    carModelHandle = -1;
}

void Engine::Update(float velocity, Transmission& transmission) {
    // ���݂̑��x�ƃM�A���RPM���Čv�Z
    engineRPM = CarCulateRPM(velocity, 0.3f, transmission.GetGearRatio()); // ���Ƀ^�C�����a 0.3m

    // ���b�h���C���𒴂��Ă����狭���V�t�g�A�b�v
    if (engineRPM > redlineRPM && transmission.GetGear() < 5) {
        transmission.ForceShiftUp();
    }
}

float Engine::GetRPM() const {
    return engineRPM;
}

float Engine::GetBrakeCoefficient() const {
    return engineBrakeCoefficient;
}

void Engine::SetCarModelHandle(int handle) {
    carModelHandle = handle;
}

void Engine::SetRPM(float rpm) {
    engineRPM = rpm;
}