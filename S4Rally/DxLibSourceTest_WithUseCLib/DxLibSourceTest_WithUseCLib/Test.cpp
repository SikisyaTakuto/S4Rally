#include "DxLib.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	int x, add ;

	// �E�C���h�E���[�h�ŋN��
	ChangeWindowMode( TRUE ) ;

	// �c�w���C�u�����̏�����
	if( DxLib_Init() < 0 ) return -1 ;

	// �`���𗠉�ʂɂ���
	SetDrawScreen( DX_SCREEN_BACK ) ;

	// ���C�����[�v
	x = 0 ;
	add = 8 ;
	while( ProcessMessage() == 0 )
	{
		// �ړ�
		x += add ;
		if( x < 0 || x > 640 ) add = -add ;

		// ��ʂ̃N���A
		ClearDrawScreen() ;

		// �l�p�`�̕`��
		DrawBox( x, 240 - 32, x + 64, 240 + 32, GetColor( 255,255,255 ), TRUE ) ;

		// ����ʂ̓��e��\��ʂɔ��f
		ScreenFlip() ;
	}

	// �c�w���C�u�����̌�n��
	DxLib_End() ;

	// �\�t�g�̏I��
	return 0 ;
}
