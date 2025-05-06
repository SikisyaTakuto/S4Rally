#include "timer.h"

// �O���ϐ��̏�����
int startTime = 0;
int lapStartTime = 1;
std::vector<int> laps;
GameState gameState = STATE_COUNTDOWN;

// ����̊�ƂȂ�|�W�V����
const float goalMinX =4100.0f;
const float goalMaxX = 4200.0f;
const float goalMinZ = -3700.0f; 
const float goalMaxZ = -2650.0f;
bool lapPassed = false; // ��|�W�V������ʉ߂������ǂ����̃t���O

int finishTime = 0; // STATE_FINISHED�ɓ���������
const int returnToTitleDelay = 10000; // 10�b��i�~���b�j

// --- �ǉ��F�`�F�b�N�|�C���g�G���A ---
const float checkpointMinX = 4100.0f;
const float checkpointMaxX = 4200.0f;
const float checkpointMinZ = 4700.0f; 
const float checkpointMaxZ = 6000.0f;
bool checkpointPassed = false; // �`�F�b�N�|�C���g��ʉ߂�����

// ����������
VOID TimerInit(VOID)
{
    startTime = GetNowCount();
    lapStartTime = 0;
    laps.clear();
    gameState = STATE_COUNTDOWN;
    lapPassed = false;
    checkpointPassed = true;
}

// �X�V����
VOID TimerUpdate(VOID)
{
    extern Car car;
    int currentTime = GetNowCount();

    if (gameState == STATE_COUNTDOWN)
    {
        gameState = STATE_RUNNING;
    }
    else if (gameState == STATE_RUNNING)
    {
        // --- �`�F�b�N�|�C���g�ʉߔ��� ---
        if (!checkpointPassed &&
            car.carBodyPosition.x >= checkpointMinX && car.carBodyPosition.x <= checkpointMaxX &&
            car.carBodyPosition.z >= checkpointMinZ && car.carBodyPosition.z <= checkpointMaxZ)
        {
            checkpointPassed = true; // �`�F�b�N�|�C���g�ʉ߁I
        }

        // --- �S�[���G���A�ʉߔ��� ---
        if (checkpointPassed && !lapPassed &&
            car.carBodyPosition.x >= goalMinX && car.carBodyPosition.x <= goalMaxX &&
            car.carBodyPosition.z >= goalMinZ && car.carBodyPosition.z <= goalMaxZ)
        {
            if (lapStartTime == 0)
            {
                lapStartTime = GetNowCount();
            }
            else
            {
                TimerRecordLap();
            }
            lapPassed = true;
            checkpointPassed = false; // �`�F�b�N�|�C���g�t���O�����Z�b�g�I
        }
        else if (lapPassed && (car.carBodyPosition.z < goalMinZ || car.carBodyPosition.z > goalMaxZ ||
            car.carBodyPosition.x < goalMinX || car.carBodyPosition.x > goalMaxX))
        {
            // �S�[���G���A����o���烊�Z�b�g
            lapPassed = false;
        }
    }
    else if (gameState == STATE_FINISHED)
    {
        if (GetNowCount() - finishTime >= returnToTitleDelay)
        {
            ChangeGameScene = TitleScene; // 10�b�o������^�C�g���ɖ߂�
        }
    }
}

// �`�揈��
VOID TimerDraw(VOID)
{
    int currentTime = GetNowCount();

    if (gameState == STATE_RUNNING)
    {
        // ���݂̎��񎞊Ԃ�\��
        int elapsedTime = currentTime - lapStartTime;

        if (lapStartTime != 0)
        {
            DrawFormatStringToHandleAlign(
                640, 100, Align_Center, Color_skyblue, fontJiyu25ptFuch.Handle, "�^�C��: %.2f seconds", elapsedTime / 1000.0f);
        }

        // �L�^�ς݂̎���^�C����\��
        int yOffset = 100;
        for (size_t i = 0; i < laps.size(); ++i)
        {
            DrawFormatString(1000, yOffset, GetColor(0, 255, 0), "Lap %d: %.2f seconds", i + 1, laps[i] / 1000.0f);
            yOffset += 20;
        }
    }
    else if (gameState == STATE_FINISHED)
    {
        // 3���̃^�C����\��
        int minLapIndex = 0;
        for (size_t i = 1; i < laps.size(); ++i)
        {
            if (laps[i] < laps[minLapIndex])
            {
                minLapIndex = i;
            }
        }

        int yOffset = 100;
        for (size_t i = 0; i < laps.size(); ++i)
        {
            int color = (i == minLapIndex) ? GetColor(255, 0, 0) : GetColor(0, 0, 0); // �ő��^�C���͐ԁA���̑��͍�
            //DrawFormatString(100, yOffset, color, "Lap %d: %.2f seconds", i + 1, laps[i] / 1000.0f);

            DrawFormatStringToHandleAlign(
                640, yOffset, Align_Center, color, fontJiyu25ptFuch.Handle, "���b�v�^�C�� %d: %.2f seconds", i + 1, laps[i] / 1000.0f);
            yOffset += 200;
        }

        // �^�C�g���ɖ߂�܂ł̕b����\��
        int remainingTime = (returnToTitleDelay - (GetNowCount() - finishTime)) / 1000;
        if (remainingTime < 0) remainingTime = 0;
        DrawFormatStringToHandleAlign(
            640,650, Align_Center, GetColor(0, 0, 0), fontJiyu25ptFuch.Handle,
            "�^�C�g���ɖ߂�܂�: %d�b", remainingTime);
    }
}

// ����^�C�����L�^
VOID TimerRecordLap(VOID)
{
    int currentTime = GetNowCount();
    laps.push_back(currentTime - lapStartTime);
    lapStartTime = currentTime; // �V��������̊J�n���Ԃ��X�V

    // 3�������Ń��U���g��
    if (laps.size() >= 3)
    {
        gameState = STATE_FINISHED;
        ChangeGameScene = ResultScene;
        finishTime = GetNowCount();
    }
}