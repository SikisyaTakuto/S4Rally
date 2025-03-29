//FPS�����̃\�[�X�t�@�C��

//�w�b�_�t�@�C���ǂݍ���
#include "fps.h"

//�O���[�o���ϐ�
FPS fps;

//������
VOID FPSInit(VOID)
{
	//windows���N�����Ă��猻�݂܂ł̎��Ԃ𓾂�(�}�C�N���b)
	fps.FirstTakeTime = GetNowHiPerformanceCount();

	//���̒l��������
	fps.NowTakeTime = fps.FirstTakeTime;
	fps.OldTakeTime = fps.FirstTakeTime;
	fps.Deltatime = 0.000001f;
	fps.FrameCount = 1;
	fps.Average = 0.0f;

	return;
}

//�v��
VOID FPSCheck(VOID)
{
	//���݂̎������擾
	fps.NowTakeTime= GetNowHiPerformanceCount();

	//�f���^�^�C�����v�Z
	fps.Deltatime = (fps.NowTakeTime - fps.OldTakeTime) / MicroSecond;

	//// �b�P�ʂ̃f���^�^�C�����v�Z
	//float deltaTimeSeconds = (fps.NowTakeTime - fps.OldTakeTime) / MicroSecond;

	//// �t���[���P�ʂ̃f���^�^�C�����v�Z
	//fps.Deltatime = deltaTimeSeconds * GameFPS;

	//���݂̎�����ۑ�
	fps.OldTakeTime = fps.NowTakeTime;

	//���݂�MAX�t���[���ڂȂ�
	if (fps.FrameCount == GameFPS)
	{
		//1�t���[���ځ`MAX�t���[���ڂ܂ł̍��v���Ԃ��v��
		float TotalFrameTime = fps.NowTakeTime - fps.FirstTakeTime;

		//���v���Ԃ𗝑z��FPS�l�Ŋ����ĕ��ϒl���v�Z
		float CalcAverage = TotalFrameTime / GameFPS;

		//1�b������̃t���[�����ɕϊ�
		fps.Average = MicroSecond / CalcAverage;

		//1�t���[���ڂ̎������擾
		fps.FirstTakeTime = GetNowHiPerformanceCount();

		//�t���[������1�ɖ߂�
		fps.FrameCount = 1;
	}
	else 
	{
		//�t���[�������J�E���g�A�b�v
		fps.FrameCount++;
	}

	return;
}

//�`��
VOID FPSDraw(VOID)
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "FPS:%3.1f", fps.Average);

	return;
}

//�ҋ@
VOID FPSWait(VOID)
{
	//�����������ɑҋ@����~���b
	int wait = 0;

	//�҂����Ԃ��v��
	wait = MicroSecond / GameFPS * fps.FrameCount//���z�̎���
		- (fps.NowTakeTime - fps.FirstTakeTime);//���ۂ̎���

	//�}�C�N���b���~���b�ɕϊ�
	wait /= MillSecond;

	//�҂����Ԃ�����Ȃ��
	if (wait > 0 && wait <= WaitTimeMill)
	{
		//�u���b�Z�[�W�v���������Ȃ���҂�
		WaitTimer(wait);

	}

		return;
}