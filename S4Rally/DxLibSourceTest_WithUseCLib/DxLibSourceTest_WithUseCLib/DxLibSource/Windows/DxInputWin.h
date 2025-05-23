// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		WindowsOS用入力情報プログラムヘッダファイル
// 
// 				Ver 3.24e
// 
// -------------------------------------------------------------------------------

#ifndef DX_INPUTWIN_H
#define DX_INPUTWIN_H

// インクルード ------------------------------------------------------------------
#include "../DxCompileConfig.h"
#include "../DxLib.h"
#include "DxDirectX.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

// 構造体定義 --------------------------------------------------------------------

// 振動の環境依存情報
struct INPUTVIBRATIONDATA_PF
{
	D_IDirectInputEffect	*DIEffect ;							// パッドデバイスエフェクトオブジェクト
} ;

// ゲームパッドの環境依存情報
struct INPUTPADDATA_PF
{
	int						XInputDeviceNo ;					// XInputでアクセスするデバイスの場合は０以上の値が入る( DirectInput で扱う場合は -1 )

	D_IDirectInputDevice7	*Device ;							// パッドデバイスオブジェクト
	HANDLE					Event ;								// パッドのイベントハンドル
	int						RightStickFlag ;					// 右スティックがあるかどうか
	DWORD					MultimediaAPICaps ;					// JOYCAPSW 構造体の wCaps					

	wchar_t					InstanceName[ MAX_PATH ];			// デバイスの登録名
	wchar_t					ProductName[ MAX_PATH ];			// デバイスの製品登録名
	GUID					InstanceGUID ;						// Instance GUID
	GUID					ProductGUID ;						// Product GUID
} ;

// 入力システム用環境依存データ構造体型
struct INPUTSYSTEMDATA_PF
{
	HMODULE					XInputDLL ;							// ＸＩｎｐｕｔのＤＬＬ

	DWORD					( WINAPI * XInputGetStateFunc )( DWORD dwUserIndex, D_XINPUT_STATE* pState ) ;
	DWORD					( WINAPI * XInputSetStateFunc )( DWORD dwUserIndex, D_XINPUT_VIBRATION* pVibration ) ;

	int						NoUseXInputFlag ;					// ＸＩｎｐｕｔを使用しないかどうかのフラグ
	int						NoUseDirectInputFlag ;				// ＤｉｒｅｃｔＩｎｐｕｔを使用しないかどうかのフラグ
	int						UseDirectInputFlag ;				// ＤｉｒｅｃｔＩｎｐｕｔを使用するかどうかのフラグ( NoUseDirectInputFlag を補助するもの )
	int						UseDirectInput8Flag ;				// ＤｉｒｅｃｔＩｎｐｕｔ８を使用するかどうかのフラグ
	int						UseXboxControllerDirectInputFlag ;	// Xbox360コントローラや Xbox Oneコントローラを DirectInputコントローラとしても検出するかどうかのフラグ( TRUE:DirectInputコントローラとしても検出する  FALSE:DirectInputコントローラとしては検出しない(デフォルト) )
	int						DirectInputMouseMode ;				// マウスの入力に DirectInput を使用する場合の動作モード( 0:ウィンドウがアクティブな場合のみ DirectInput を使用する  1:ウィンドウのアクティブ状態に関係なく DirectInput を使用する )
	D_IDirectInput7			*DirectInputObject ;				// ＤｉｒｅｃｔＩｎｐｕｔオブジェクト
	int						XInputPadNum ;						// ＸＩｎｐｕｔ対応のゲームパッドの数

	D_IDirectInputDevice7	*MouseDeviceObject ;				// マウスデバイスオブジェクト
	D_IDirectInputDevice7	*KeyboardDeviceObject ;				// キーボードデバイスオブジェクト
	HANDLE					KeyEvent ;							// キーボードのイベントハンドル

	int						KeyExclusiveCooperativeLevelFlag ;	// キーボードの協調レベルが排他レベルになっているかどうかフラグ
	int						KeyboardNotUseDirectInputFlag ;		// キーボードデバイスのアクセスに DirectInput を使用しないかどうかのフラグ

	int						PadInstanceGUIDNum ;				// ジョイパッドのインスタンスGUIDの数
	GUID					PadInstanceGUID[ MAX_JOYPAD_NUM ] ;	// ジョイパッドのインスタンスGUID
} ;

// 内部大域変数宣言 --------------------------------------------------------------

// 関数プロトタイプ宣言-----------------------------------------------------------

extern	int	KeyboradBufferProcess( void ) ;											// キーボードのバッファからデータを取得する処理

extern	int RefreshInputDeviceAcquireState( void ) ;								// ＤｉｒｅｃｔＩｎｐｕｔデバイスの取得状態を更新する

extern	int CheckUseDirectInputMouse( int IsButton = FALSE ) ;						// マウスの情報取得にＤｉｒｅｃｔＩｎｐｕｔを使用しているかどうかを取得する( 戻り値  TRUE:DirectInputを使用している  FALSE:DirectInputを使用していない )
extern	int GetDirectInputMouseMoveZ( int CounterReset = TRUE ) ;					// ＤｉｒｅｃｔＩｎｐｕｔを使用したマウスホイールの移動値を取得する
extern	float GetDirectInputMouseMoveZF( int CounterReset = TRUE ) ;				// ＤｉｒｅｃｔＩｎｐｕｔを使用したマウスホイールの移動値を取得する( float版 )

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_INPUTWIN_H
