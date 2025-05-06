//���y�����̃\�[�X�t�@�C��

//�w�b�_�t�@�C���ǂݍ���
#include "music.h"
#include "fps.h"

//�O���[�o���ϐ�
Music TitleBGM;			//�^�C�g���̉��y
Music PlayBGM;			//�v���C�̉��y
Music ResultBGM;		//���U���g�̉��y
Music ButtonClickSE;	//�{�^���N���b�N��
//Music JumpSE;			//�W�����v��
//Music ExplSE;			//������
//Music AvoidSE;			//�����鉹

//�ǂݍ��݁E�n���h������
BOOL Music_Init(VOID)
{

	//BGM
	TitleBGM = LoadMusic(MusicPathTitleBGM, VolumeBGM, DX_PLAYTYPE_LOOP); if (TitleBGM.Handle == -1) { return FALSE; }
	PlayBGM = LoadMusic(MusicPathPlayBGM, VolumeBGM, DX_PLAYTYPE_LOOP); if (PlayBGM.Handle == -1) { return FALSE; }
	
	//ResultBGM = LoadMusic(MusicPathResultBGM, VolumeBGM, DX_PLAYTYPE_LOOP); if (ResultBGM.Handle == -1) { return FALSE; }

	//SE
	ButtonClickSE = LoadMusic(MusicPathButtonClickSE, VolumeSE, DX_PLAYTYPE_BACK); if (ButtonClickSE.Handle == -1) { return FALSE; }

	return TRUE;
}

//��n��
VOID Music_End(VOID)
{
	DeleteMusic(TitleBGM);
	DeleteMusic(PlayBGM);
	DeleteMusic(ResultBGM);
	DeleteMusic(ButtonClickSE);

	return;
}

//�ǂݍ��݃G���[���b�Z�[�W
VOID MusicLoadErrMessage(const char* path)
{
	//�G���[�p������
	char ErrStr[MusicErrStrMax];

	//�G���[�p������̓��e����ɂ���
	for (int i = 0; i < MusicErrStrMax; i++) { ErrStr[i] = '\0'; }	//�k������(\0)�Ŗ��߂�

	//�G���[���e��A�����Đ���
	sprintf_s(ErrStr, sizeof(ErrStr), "%s%s", path, MusicLoadErrCap);

	//��WinAPI�֐�
	//�p�X���Ԉ���Ă���ȂǂŃC���X�g�[���ł��Ȃ��ꍇ�̓G���[
	MessageBox(
		GetMainWindowHandle(),		//�E�B���h�E�n���h��
		ErrStr,						//�G���[���e
		MusicLoadErrTitle,		//�G���[�^�C�g��
		MB_OK);						//OK�{�^���̂�

	return;
}

//�ǂݍ���
//���{�����[����MIN:0�`MAX:100�Ŏw�肷�邱��
Music LoadMusic(const char* path, int volume, int playType)
{
	Music music;

	//�t�@�C���̏ꏊ���R�s�[
	//music.Path = path;	//������Ȃ̂�,���̂܂ܑ���ł��Ȃ�
	strcpy_s(music.Path, sizeof(music.Path), path);

	//���y���������ɓǂݍ���
	music.Handle = LoadSoundMem(music.Path);

	//�ǂݍ��݃G���[
	if (music.Handle == -1)
	{
		MusicLoadErrMessage(music.Path);
	}
	else
	{
		//����ɓǂݍ��݂��ł�����p�����[�^�ݒ�
		music.Volume = volume;						//�ő剹�ʂ��w��
		ChangeVolumeMusic(&music, music.Volume);	//�Đ����ʂ�ݒ�
		music.PlayType = playType;					//�Đ��`���w��
		music.IsPlayStart = TRUE;					//��~��͍ŏ����琶������
	}

	return music;
}

//�Đ�
VOID PlayMusic(Music music)
{
	switch (music.PlayType)
	{
	case DX_PLAYTYPE_BACK:
		//�ʏ�o�b�N�O���E���h�Đ�

		//�����ɍĐ�����
		PlaySoundMem(music.Handle, DX_PLAYTYPE_BACK, music.IsPlayStart);
		break;

	case DX_PLAYTYPE_LOOP:
		//���[�v�Đ�

		//�Đ�����Ă��Ȃ��Ƃ���
		if (CheckSoundMem(music.Handle) == 0)
		{
			//���[�v�Đ�����
			PlaySoundMem(music.Handle, DX_PLAYTYPE_LOOP, music.IsPlayStart);
		}
	default:
		break;
	}

	return;
}

//�ꎞ��~
VOID PauseMusic(Music* music)
{
	//��~��͍��̍Đ��ʒu����Đ�����
	music->IsPlayStart = FALSE;

	//�Đ�����Ă�Ȃ�
	if (CheckSoundMem(music->Handle) != 0)
	{
		//���y��~
		StopSoundMem(music->Handle);

		//�}�X�^���ʂɖ߂�
		ChangeVolumeSoundMem(GetVolume(music->Volume), music->Handle);

		//�t�F�[�h������
		music->FadeInStartFlg = FALSE;
		music->FadeOutStartFlg = FALSE;
	}

	return;
}

//��~
VOID StopMusic(Music* music)
{
	//��~��͍ŏ��̈ʒu����Đ�����
	music->IsPlayStart = TRUE;

	//�Đ�����Ă�Ȃ�
	if (CheckSoundMem(music->Handle) != 0)
	{
		//���y��~
		StopSoundMem(music->Handle);

		//�}�X�^���ʂɖ߂�
		ChangeVolumeSoundMem(GetVolume(music->Volume), music->Handle);

		//�t�F�[�h������
		music->FadeInStartFlg = FALSE;
		music->FadeOutStartFlg = FALSE;
	}

	return;
}

//���
VOID DeleteMusic(Music music)
{
	//����ɓǂݍ��݂��ł��Ă����
	if (music.Handle != -1)
	{
		//������������
		DeleteSoundMem(music.Handle);
	}

	return;
}

//���ʕύX
//���{�����[����MIN:0�`MAX:100�Ŏw�肷�邱��
VOID ChangeVolumeMusic(Music* music, int volume)
{
	//���y�̃}�X�^�[���ʂ��؂�̂ł͂Ȃ��A
	//�Đ�����Ă��鉹�ʂ�ύX����
	if (volume <= music->Volume)
	{
		ChangeVolumeSoundMem(GetVolume(volume), music->Handle);
	}

	return;
}

//�t�F�[�h�C��
//��EffectTime�͔���������ʃ~���b��
BOOL FadeInPlayMusic(Music* music, int EffectTime)
{
	//���߂Ẵt�F�[�h�C�������̎�
	if (music->FadeInStartFlg == FALSE)
	{
		//�t�F�[�h�C���̃~���b��������
		music->FadeInMillTime = 0;
		music->FadeInMillTimeMax = (EffectTime / 1000.0f) * GameFPS;

		//���ʂ�0�ɂ���
		ChangeVolumeMusic(music, 0);

		//���y�Đ�
		PlayMusic(*music);	//*�ŊԐڎQ�Ƃ��Ė{�̂������Ă���

		//�t�F�[�h�C���J�n�t���OON
		music->FadeInStartFlg = TRUE;
	}

	if (music->FadeInStartFlg == TRUE)
	{
		//�t�F�[�h�C����
		if (music->FadeInMillTime <= music->FadeInMillTimeMax)
		{
			//���ʎ��Ԍo��
			music->FadeInMillTime++;

			//���ʂ��グ��
			ChangeVolumeMusic(
				music,
				(float)music->FadeInMillTime / music->FadeInMillTimeMax	//�o�ߎ��ԁ�MAX���ԂőS�̂̊���(%)���v�Z
				* music->Volume);										//%�𐮐��ɕϊ�
		}
		else if (music->FadeInMillTime >= music->FadeInMillTimeMax)
		{
			//�t�F�[�h�C������
			return TRUE;
		}
	}

	return FALSE;
}

//�t�F�[�h�A�E�g
//��EffectTime�͔���������ʃ~���b��
BOOL FadeOutPlayMusic(Music* music, int EffectTime)
{
	//���߂Ẵt�F�[�h�A�E�g�����̎�
	if (music->FadeOutStartFlg == FALSE)
	{
		//�t�F�[�h�A�E�g�̃~���b��������
		music->FadeOutMillTime = 0;
		music->FadeOutMillTimeMax = (EffectTime / 1000.0f) * GameFPS;

		//���ʂ�MAX�ɂ���
		ChangeVolumeMusic(music, music->Volume);

		//���y�Đ�
		PlayMusic(*music);	//*�ŊԐڎQ�Ƃ��Ė{�̂������Ă���

		//�t�F�[�h�A�E�g�J�n�t���OON
		music->FadeOutStartFlg = TRUE;
	}

	if (music->FadeOutStartFlg == TRUE)
	{
		//�t�F�[�h�A�E�g��
		if (music->FadeOutMillTime <= music->FadeOutMillTimeMax)
		{
			//���ʎ��Ԍo��
			music->FadeOutMillTime++;

			//���ʂ��グ��
			ChangeVolumeMusic(
				music,
				(1 - (float)music->FadeOutMillTime / music->FadeOutMillTimeMax)	//MAX�̊���(100% - ���炵����%)
				* music->Volume);												//MAX�̉��ʂ���
		}
		else if (music->FadeOutMillTime >= music->FadeOutMillTimeMax)
		{
			//�t�F�[�h�A�E�g����
			return TRUE;
		}
	}

	return FALSE;
}