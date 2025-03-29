#pragma once

#include "DxLib.h"
#include <vector>
#include <string>
#include "game.h"
#include "font.h"
#include "car.h"


// �Q�[���X�e�[�g�̗񋓌^
enum GameState
{
    STATE_COUNTDOWN,
    STATE_RUNNING,
    STATE_FINISHED
};

extern int startTime;           // �J�E���g�_�E���J�n����
extern int lapStartTime;        // ����J�n����
extern std::vector<int> laps;   // ����^�C�����L�^����z��
extern GameState gameState;     // ���݂̃Q�[���X�e�[�g
extern bool isRunning;          // �v�����t���O

// �O���v���g�^�C�v�錾
extern VOID TimerInit(VOID);
extern VOID Timer(VOID);
extern VOID TimerUpdate(VOID);           // �Q�[�����W�b�N�X�V
extern VOID TimerDraw(VOID);             // �`�揈��
extern VOID TimerRecordLap(VOID);        // ����^�C���L�^
