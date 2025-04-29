#include "transmission.h"

// ������
Transmission::Transmission() {
    gear = 1;
    maxGear = 5;
    gearChangeLocked = false;
    mode = Manual;

    float defaultRatios[5] = { 25.0f,18.0f, 13.0f, 10.0f, 8.0f };
    float defaultSpeeds[5] = { 5000.0f, 7000.0f, 9000.0f, 11000.0f, 15000.0f };

    SetGearRatios(defaultRatios, 5);
    SetMaxSpeeds(defaultSpeeds, 5);
}

void Transmission::Update(float velocity, float engineRPM) {
    if (mode == Manual) {
        if (!gearChangeLocked) {
            if (CheckHitKey(KEY_INPUT_E) && gear < maxGear) {
                gear++;
                gearChangeLocked = true;
            }
            if (CheckHitKey(KEY_INPUT_Q) && gear > 1) {
                gear--;
                gearChangeLocked = true;
            }
        }
        if (!CheckHitKey(KEY_INPUT_Q) && !CheckHitKey(KEY_INPUT_E)) {
            gearChangeLocked = false;
        }
    }
    else if (mode == Automatic) {
        // �I�[�g�}�`�b�N��
        if (engineRPM > 8000.0f && gear < maxGear) {
            gear++;
        }
        else if (engineRPM < 2000.0f && gear > 1) {
            gear--;
        }
    }

    // ���[�h�؂�ւ��i�Ⴆ��Tab�L�[�j
    if (KeyDown(KEY_INPUT_TAB)) {
        ToggleMode();
    }
}

void Transmission::ToggleMode() {
    if (mode == Manual) {
        mode = Automatic;
    }
    else {
        mode = Manual;
    }
}

void Transmission::ForceShiftUp() {
    if (gear <= maxGear) {
        gear++;
    }
}


// ���݂̃M�A�擾
int Transmission::GetGear() const {
    return gear;
}

// ���݂̃M�A��擾
float Transmission::GetGearRatio() const {
    return gearRatios[gear - 1];
}

// �G���W����]���v�Z
float Transmission::CarculateRPM(float velocity, float wheelRadius) const {
    const float finalDriveRatio = 4.1f; // �f�t�@�����V������
    float rpm = (velocity / wheelRadius) * GetGearRatio() * finalDriveRatio * 60.0f / (2.0f * DX_PI);
    return rpm;
}

// �G���W���t�H�[�X�v�Z�i�M�A�䍞�݁j
float Transmission::CarculateEngineForce(float engineForce) const {
    return engineForce * GetGearRatio();
}

// �M�A��ݒ�
void Transmission::SetGearRatios(const float* ratios, int size) {
    for (int i = 0; i < size && i < 8; ++i) {
        gearRatios[i] = ratios[i];
    }
    maxGear = size;
}

// �e�M�A�̍ō����ݒ�
void Transmission::SetMaxSpeeds(const float* speeds, int size) {
    for (int i = 0; i < size && i < 8; ++i) {
        maxSpeeds[i] = speeds[i];
    }
}