#include "engine.h"

EngineInfo engineInfo;

// �G���W����]���̌v�Z
VOID UpdateEngineRPM(VOID) {
    // ���݂̑��x�ƃM�A�Ɋ�Â��ăG���W����]�����v�Z
    engineInfo.engineRPM = CarCulateRPM(carInfo.currentSpeed, transmissionInfo.gear);

    // �G���W����]�������b�h���C���𒴂��A���M�A���ő�M�A�����ł���΋����I�ɃV�t�g�A�b�v
    if (engineInfo.engineRPM >engineInfo.redlineRPM && transmissionInfo.gear < 5) {
       transmissionInfo.gear++; // �M�A��1�グ�邱�ƂŃG���W���̉߉�]��h��
    }
}