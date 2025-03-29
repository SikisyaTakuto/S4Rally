#include "timer.h"

// �O���ϐ��̏�����
int startTime = 0;
int lapStartTime = 0;
std::vector<int> laps;
GameState gameState = STATE_COUNTDOWN;

// ����̊�ƂȂ�|�W�V����
const float goalMinX =-300.0f;
const float goalMaxX = 2500.0f;
const float goalMinZ = -25.0f;
const float goalMaxZ = 5.0f;
bool lapPassed = false; // ��|�W�V������ʉ߂������ǂ����̃t���O

// ����������
VOID TimerInit(VOID)
{
    startTime = GetNowCount();
    lapStartTime = 0;
    laps.clear();
    gameState = STATE_COUNTDOWN;
    lapPassed = false;
}

// �X�V����
VOID TimerUpdate(VOID)
{
    int currentTime = GetNowCount();

    if (gameState == STATE_COUNTDOWN)
    {
        // 5�b�J�E���g�_�E��
        if ((currentTime - startTime) / 1000 >= 5)
        {
            gameState = STATE_RUNNING;
            lapStartTime = GetNowCount();
        }
    }
    else if (gameState == STATE_RUNNING)
    {
        // ��|�W�V������ʉ߂��������m�F
        if (!lapPassed && (carInfo.position.x >= goalMinX && carInfo.position.x <= goalMaxX && carInfo.position.z >= goalMinZ && carInfo.position.z <= goalMaxZ))
        {
            TimerRecordLap(); // ����^�C�����L�^
            lapPassed = true; // �ʉ߃t���O�𗧂Ă�

            // 3������������Q�[���V�[�����I����ԂɕύX
            if (laps.size() == 2)
            {
                gameState = STATE_FINISHED;
                ChangeGameScene = ResultScene;
            }
        }
        else if (lapPassed && (carInfo.position.z < goalMinZ || carInfo.position.z > goalMaxZ || carInfo.position.x < goalMinX || carInfo.position.x > goalMaxX))
        {
            // ��|�W�V��������\�����ꂽ�ꍇ�Ƀt���O�����Z�b�g
            lapPassed = false;
        }
    }
}

// �`�揈��
VOID TimerDraw(VOID)
{
    int currentTime = GetNowCount();

    if (gameState == STATE_COUNTDOWN)
    {
        // �J�E���g�_�E���\��
        int countdown = 5 - (currentTime - startTime) / 1000;
        //DrawFormatString(100, 100, GetColor(255, 255, 255), "Starting in: %d", countdown);

        DrawFormatStringToHandleAlign(
           640, 100, Align_Center, Color_tomato, fontJiyu25ptFuch.Handle, "�J�E���g: %d", countdown);
    }
    else if (gameState == STATE_RUNNING)
    {
        // ���݂̎��񎞊Ԃ�\��
        int elapsedTime = currentTime - lapStartTime;
        DrawFormatString(0, 100, GetColor(255, 255, 255), "Current Lap Time: %.2f seconds", elapsedTime / 1000.0f);

        DrawFormatStringToHandleAlign(
            640, 100, Align_Center, Color_skyblue, fontJiyu25ptFuch.Handle,"�^�C��: %.2f seconds", elapsedTime / 1000.0f);

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
    }
}

// ����^�C�����L�^
VOID TimerRecordLap(VOID)
{
    int currentTime = GetNowCount();
    laps.push_back(currentTime - lapStartTime);
    lapStartTime = currentTime; // �V��������̊J�n���Ԃ��X�V
}