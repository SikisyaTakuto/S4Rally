// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		ハンドル管理プログラム
// 
// 				Ver 3.24e
// 
// -------------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define DX_MAKE

// インクルード ------------------------------------------------------------------
#include "DxHandle.h"
#include "DxLib.h"
#include "DxStatic.h"
#include "DxASyncLoad.h"
#include "DxBaseFunc.h"
#include "DxMemory.h"
#include "DxLog.h"

#ifndef DX_NON_ASYNCLOAD
#include "DxGraphics.h"
#include "DxSoftImage.h"
#include "DxSound.h"
#include "DxMask.h"
#include "DxNetwork.h"
#include "DxModel.h"
#include "DxFile.h"
#include "DxFont.h"
#endif // DX_NON_ASYNCLOAD

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

// 構造体定義 --------------------------------------------------------------------

// 内部大域変数宣言 --------------------------------------------------------------

HANDLEMANAGE HandleManageArray[ DX_HANDLETYPE_MAX ] ;

// 関数プロトタイプ宣言-----------------------------------------------------------

// プログラム --------------------------------------------------------------------


// ハンドル共通関係

// ハンドル管理情報を初期化する
// ( InitializeFlag には FALSE が入っている必要がある )
extern int InitializeHandleManage(
	int HandleType,
	int OneSize,
	int MaxNum,
	int ( *InitializeFunction )( HANDLEINFO *HandleInfo ),
	int ( *TerminateFunction )( HANDLEINFO *HandleInfo ),
	int ( *DumpInfoFunction )( HANDLEINFO *HandleInfo ),
	const wchar_t *Name
)
{
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	// 既に初期化されていたら何もしない
	if( HandleManage->InitializeFlag )
		return -1 ;

	// NextID以外ゼロ初期化
	int NextID = HandleManage->NextID ;
	_MEMSET( HandleManage, 0, sizeof( *HandleManage ) ) ;
	HandleManage->NextID = NextID ;

	// パラメータセット
	HandleManage->HandleTypeMask = HandleType << DX_HANDLETYPE_ADDRESS ;
	HandleManage->OneSize = OneSize ;
	HandleManage->MaxNum = MaxNum ;
	HandleManage->InitializeFunction = InitializeFunction ;
	HandleManage->TerminateFunction = TerminateFunction ;
	HandleManage->DumpInfoFunction = DumpInfoFunction ;
	HandleManage->Name = Name ;
	ConvString( ( const char * )HandleManage->Name, -1, WCHAR_T_CHARCODEFORMAT, HandleManage->NameUTF16LE, sizeof( HandleManage->NameUTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;

	// ハンドルのデータポインタアドレスを格納するメモリアドレス配列の確保
	HandleManage->Handle = ( HANDLEINFO ** )DXCALLOC( sizeof( HANDLEINFO * ) * MaxNum ) ;
	if( HandleManage->Handle == NULL )
		return -1 ;

	// ハンドルリストの初期化
	InitializeHandleList( &HandleManage->ListFirst, &HandleManage->ListLast ) ;

	// 削除リクエストのハンドルリストの初期化
	InitializeHandleList( &HandleManage->DeleteRequestListFirst, &HandleManage->DeleteRequestListLast ) ;

	// クリティカルセクションの初期化
	CriticalSection_Initialize( &HandleManage->CriticalSection ) ;

	// 初期化フラグを立てる
	HandleManage->InitializeFlag = TRUE ;

	// 終了
	return 0 ;
}

// ハンドル管理情報の後始末を行う
extern int TerminateHandleManage( int HandleType )
{
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	// 既に後始末されていたら何もしない
	if( HandleManage->InitializeFlag == FALSE )
		return -1 ;

	// すべてのハンドルを削除
	AllHandleSub( HandleType ) ;

	// ハンドルポインタ格納用のメモリアドレス配列を解放
	if( HandleManage->Handle != NULL )
	{
		DXFREE( HandleManage->Handle ) ;
		HandleManage->Handle = NULL ;
	}

	// クリティカルセクションの削除
	CriticalSection_Delete( &HandleManage->CriticalSection ) ;

	// NextID以外ゼロ初期化
	int NextID = HandleManage->NextID ;
	_MEMSET( HandleManage, 0, sizeof( *HandleManage ) ) ;
	HandleManage->NextID = NextID ;

	// 初期化フラグを倒す
	HandleManage->InitializeFlag = FALSE ;

	// 終了
	return 0 ;
}

// ハンドルを追加する
extern int AddHandle( int HandleType, int ASyncThread, int Handle )
{
	int NextIndex ;
	int NewHandle ;
	HANDLEINFO **ppHandleInfo ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	if( HandleManage->InitializeFlag == FALSE )
		return -1 ;

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// 追加できない場合は終了
	if( HandleManage->Num == HandleManage->MaxNum )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		DXST_LOGFILEFMT_ADDUTF16LE(( "\x25\x00\x73\x00\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6e\x30\x70\x65\x4c\x30\x50\x96\x4c\x75\x70\x65\x28\x00\x20\x00\x25\x00\x64\x00\x20\x00\x29\x00\x6b\x30\x54\x90\x57\x30\x66\x30\x44\x30\x66\x30\xb0\x65\x5f\x30\x6a\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x92\x30\x5c\x4f\x10\x62\x67\x30\x4d\x30\x7e\x30\x5b\x30\x93\x30\x00"/*@ L"%sハンドルの数が限界数( %d )に達していて新たなハンドルを作成できません" @*/, HandleManage->NameUTF16LE, HandleManage->MaxNum )) ;
		return -1 ;
	}

	// 空き配列番号の検索
	if( Handle != -1 && HandleManage->Handle[ Handle & DX_HANDLEINDEX_MASK ] == NULL )
	{
		NextIndex = Handle & DX_HANDLEINDEX_MASK ;
		ppHandleInfo = &HandleManage->Handle[ NextIndex ] ;
	}
	else
	{
		if( HandleManage->Num == 0 )
		{
			NextIndex = 0 ;
			ppHandleInfo = &HandleManage->Handle[ NextIndex ] ;
		}
		else
		{
			if( HandleManage->AreaMax + 1 < HandleManage->MaxNum )
			{
				NextIndex = HandleManage->AreaMax + 1 ;
				ppHandleInfo = &HandleManage->Handle[ NextIndex ] ;
			}
			else
			if( HandleManage->AreaMin - 1 > 0 )
			{
				NextIndex = HandleManage->AreaMin - 1 ;
				ppHandleInfo = &HandleManage->Handle[ NextIndex ] ;
			}
			else
			{
				ppHandleInfo = HandleManage->Handle ;
				for( NextIndex = 0 ; *ppHandleInfo != NULL ; NextIndex ++, ppHandleInfo ++ ){}
			}
		}
	}

	// データ領域を確保する
	*ppHandleInfo = ( HANDLEINFO * )DXCALLOC( ( size_t )HandleManage->OneSize ) ;
	if( *ppHandleInfo == NULL )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		DXST_LOGFILEFMT_ADDUTF16LE(( "\x25\x00\x73\x00\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6e\x30\xc7\x30\xfc\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"%sハンドルのデータを格納するメモリ領域の確保に失敗しました" @*/, HandleManage->NameUTF16LE )) ;
		return -1 ;
	}

	// エラーチェック用ＩＤの設定
	if( Handle != -1 )
	{
		(*ppHandleInfo)->ID = ( Handle & DX_HANDLECHECK_MASK ) >> DX_HANDLECHECK_ADDRESS ;
	}
	else
	{
		HandleManage->NextID ++ ;
		if( HandleManage->NextID >= ( DX_HANDLECHECK_MASK >> DX_HANDLECHECK_ADDRESS ) )
			HandleManage->NextID = 0 ;

		(*ppHandleInfo)->ID = HandleManage->NextID ;
	}

	// メモリの確保サイズを保存
	( *ppHandleInfo )->AllocSize = HandleManage->OneSize ;

#ifndef DX_NON_ASYNCLOAD
	// 非同期読み込みが完了したらハンドルを削除するフラグを初期化
	( *ppHandleInfo )->ASyncLoadFinishDeleteRequestFlag = FALSE ;

	// 非同期読み込みが完了したら呼ばれるコールバック関数の情報を初期化
	( *ppHandleInfo )->ASyncLoadFinishCallback = NULL ;
	( *ppHandleInfo )->ASyncLoadFinishCallbackData = NULL ;

	// 非同期スレッドから呼ばれた場合は非同期読み込みカウントを１にする
	if( ASyncThread )
	{
		( *ppHandleInfo )->ASyncLoadCount  = 1 ;
		( *ppHandleInfo )->ASyncDataNumber = -1 ;
	}
#endif // DX_NON_ASYNCLOAD

	// ハンドル値を保存
	NewHandle = (*ppHandleInfo)->Handle = NextIndex | HandleManage->HandleTypeMask | ( (*ppHandleInfo)->ID << DX_HANDLECHECK_ADDRESS ) ;

	// ハンドルの数を増やす
	HandleManage->Num ++ ;

	// 使用されているハンドルが存在する範囲を更新する
	if( HandleManage->Num == 1 )
	{
		HandleManage->AreaMax = NextIndex ;
		HandleManage->AreaMin = NextIndex ;
	}
	else
	{
		if( HandleManage->AreaMax < NextIndex ) HandleManage->AreaMax = NextIndex ;
		if( HandleManage->AreaMin > NextIndex ) HandleManage->AreaMin = NextIndex ;
	}

	// リストへ要素を追加
	AddHandleList( &HandleManage->ListFirst, &(*ppHandleInfo)->List, (*ppHandleInfo)->Handle, *ppHandleInfo ) ;

	// 初期化関数を呼ぶ
	if( HandleManage->InitializeFunction )
	{
		HandleManage->InitializeFunction( *ppHandleInfo ) ;
	}

	// クリティカルセクションの解放
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// ハンドルを返す
	return NewHandle ;
}

// SubHandle の実処理関数
static int SubHandleBase( HANDLEMANAGE *HandleManage, HANDLEINFO *HandleInfo )
{
	int Index ;
	int HandleType = ( int )( ( ( DWORD )HandleInfo->Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;

	if( HandleManage->InitializeFlag == FALSE )
		return -1 ;

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	Index = HandleInfo->Handle & DX_HANDLEINDEX_MASK ;

#ifndef DX_NON_ASYNCLOAD
	// 非同期読み込み中である場合でまだ処理が走っていなかったら処理をキャンセルする
	if( HandleInfo->ASyncLoadCount != 0 )
	{
		if( HandleInfo->ASyncDataNumber < 0 ||
			DeleteASyncLoadData( HandleInfo->ASyncDataNumber, TRUE ) < -1 )
		{
			// クリティカルセクションの解放
			CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

			// 削除に失敗したら読み込み処理が終わるまで待つ
			while( HandleInfo->ASyncLoadCount != 0 )
			{
				ProcessASyncLoadRequestMainThread() ;
				Thread_Sleep( 0 );
			}

			// クリティカルセクションの取得
			CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;
		}
	}
#endif // DX_NON_ASYNCLOAD

	// ハンドルタイプ個々の後始末処理
	if( HandleManage->TerminateFunction )
	{
		// 戻り値が１の場合は削除キャンセル
		if( HandleManage->TerminateFunction( HandleInfo ) == 1 )
			goto END ;
	}

	// 削除フラグに-1を代入する
	if( HandleInfo->DeleteFlag != NULL )
	{
		*HandleInfo->DeleteFlag = -1 ;
	}

	// リストから要素を外す
	SubHandleList( &HandleInfo->List ) ;

	// 削除リクエストされていたら削除リクエストリストから外す
	if( HandleInfo->DeleteRequestFlag )
	{
		SubHandleList( &HandleInfo->DeleteRequestList ) ;

		// 削除リクエストされていた数も 1 減らす
		HandleManage->DeleteRequestHandleNum -- ;
	}

	// データ領域を解放する
	DXFREE( HandleInfo ) ;

	// テーブルに NULL をセットする
	HandleManage->Handle[ Index ] = NULL ;

	// ハンドルの総数を減らす
	HandleManage->Num -- ;

	// 有効なハンドルが存在する範囲の更新
	if( HandleManage->Num == 0 )
	{
		HandleManage->AreaMax = 0 ;
		HandleManage->AreaMin = 0 ;
	}
	else
	{
		if( Index == HandleManage->AreaMax )
		{
			while( HandleManage->Handle[ HandleManage->AreaMax ] == NULL )
				HandleManage->AreaMax -- ; 
		}
		else
		if( Index == HandleManage->AreaMin )
		{
			while( HandleManage->Handle[ HandleManage->AreaMin ] == NULL )
				HandleManage->AreaMin ++ ; 
		}
	}

END :

	// クリティカルセクションの解放
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// 終了
	return 0 ;
}

// ハンドルを削除する
extern int SubHandle( int Handle, int ASyncLoadFlag, int ASyncThread )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	if( HandleManage->InitializeFlag == FALSE )
		return -1 ;

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// エラー判定
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return -1 ;
	}

	// 既に削除リクエストリストに登録されている場合はエラー
	if( HandleInfo->DeleteRequestFlag )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return -1 ;
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread || ASyncLoadFlag )
	{
		// 非同期実行希望や別スレッドからの呼び出しの場合は削除リクエストリストに追加
		AddHandleList( &HandleManage->DeleteRequestListFirst, &HandleInfo->DeleteRequestList, HandleInfo->Handle, HandleInfo ) ;

		// 削除リクエストリストに登録されているフラグを立てる
		HandleInfo->DeleteRequestFlag = TRUE ;

		// 削除リクエストのハンドル数をインクリメント
		HandleManage->DeleteRequestHandleNum ++ ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		// 非同期実行希望や別スレッドからの呼び出しではない場合は即座に削除
		if( SubHandleBase( HandleManage, HandleInfo ) < 0 )
		{
			// クリティカルセクションの解放
			CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

			return -1 ;
		}
	}

	// クリティカルセクションの解放
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// 終了
	return 0 ;
}

// ハンドルの情報を格納するメモリ領域のサイズを変更する、非同期読み込み中でないことが前提
extern int ReallocHandle( int Handle, size_t NewSize )
{
	HANDLEINFO *HandleInfo ;
	int Index ;
	void *NewBuffer ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	if( HandleManage->InitializeFlag == FALSE )
		return -1 ;

	// エラー判定
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
		return -1 ;

	Index = Handle & DX_HANDLEINDEX_MASK ;

	// 確保メモリのサイズが変化しない場合は何もせず終了
	if( ( size_t )HandleInfo->AllocSize == NewSize )
		return 0 ;

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// メモリの再確保
	NewBuffer = ( HANDLEINFO * )DXREALLOC( HandleInfo, ( size_t )NewSize ) ;

	// メモリの再確保成功チェック
	if( NewBuffer == NULL )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		// エラー終了
		return -1 ;
	}

	// 確保メモリアドレスが変化した場合はリストのつなぎ直し
	if( NewBuffer != ( void * )HandleInfo )
	{
		// 新しいメモリアドレスをセット
		HandleInfo = HandleManage->Handle[ Index ] = ( HANDLEINFO * )NewBuffer ;

		// リストの繋ぎ直し
		NewMemoryHandleList( &HandleInfo->List, HandleInfo ) ;
	}

	// クリティカルセクションの解放
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// 終了
	return 0 ;
}

// ハンドルの情報を取得する
extern HANDLEINFO *GetHandleInfo( int Handle )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	if( HandleManage->InitializeFlag == FALSE )
		return NULL ;

	// エラー判定
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
		return NULL ;

	// ハンドルの情報を返す
	return HandleInfo ;
}

// ハンドル管理情報に登録されているすべてのハンドルを削除
extern int AllHandleSub( int HandleType, int (*DeleteCancelCheckFunction)( HANDLEINFO *HandleInfo ) )
{
	int i ;
	HANDLEINFO **ppHandleInfo ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	if( HandleManage->InitializeFlag == FALSE )
		return -1 ;

//	// クリティカルセクションの取得
//	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;
//
//#ifndef DX_NON_ASYNCLOAD
//	// 非同期読み込み依頼があったら削除しておく
//	ppHandleInfo = &HandleManage->Handle[ HandleManage->AreaMin ] ;
//	for( i = HandleManage->AreaMin ; i <= HandleManage->AreaMax ; i ++, ppHandleInfo ++ )
//	{
//		if( *ppHandleInfo )
//		{
//			if( DeleteCancelCheckFunction == NULL ||
//				DeleteCancelCheckFunction( *ppHandleInfo ) == FALSE )
//			{
//				if( (*ppHandleInfo)->ASyncLoadCount != 0 )
//				{
//					if( DeleteASyncLoadData( (*ppHandleInfo)->ASyncDataNumber, TRUE ) >= -1 )
//					{
//						(*ppHandleInfo)->ASyncLoadCount = 0 ;
//					}
//				}
//			}
//		}
//	}
//#endif // DX_NON_ASYNCLOAD
//
//	// クリティカルセクションの解放
//	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// すべてのハンドルデータを削除する
	{
		int AreaMin ;
		int AreaMax ;
		int Handle ;
		int DeleteFlag ;
		int LoopFlag ;

		// クリティカルセクションの取得
		CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

		do
		{
			LoopFlag = FALSE ;

			AreaMin = HandleManage->AreaMin ;
			AreaMax = HandleManage->AreaMax ;

			ppHandleInfo = &HandleManage->Handle[ AreaMin ] ;
			for( i = AreaMin ; i <= AreaMax ; i ++, ppHandleInfo ++ )
			{
				if( *ppHandleInfo == NULL )
				{
					continue ;
				}

				if( DeleteCancelCheckFunction != NULL )
				{
					DeleteFlag = DeleteCancelCheckFunction( *ppHandleInfo ) == FALSE ? TRUE : FALSE ;
				}
				else
				{
					DeleteFlag = TRUE ;
				}

				if( DeleteFlag )
				{
					Handle = (*ppHandleInfo)->Handle ;

					// クリティカルセクションの解放
					CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

					SubHandle( Handle, FALSE, FALSE ) ;

					// クリティカルセクションの取得
					CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

					LoopFlag = TRUE ;
					break ;
				}
			}

		}while( LoopFlag == TRUE ) ;

		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;
	}

	// 終了
	return 0 ;
}









#ifndef DX_NON_ASYNCLOAD

// 非同期読み込み関係

// ハンドルの非同期読み込みが完了しているかどうかを取得する( TRUE:まだ完了していない  FALSE:完了している  -1:エラー )
extern int NS_CheckHandleASyncLoad( int Handle )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;
	int Result ;

	if( HandleManage->InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// エラー判定
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return -1 ;
	}

	Result = HandleInfo->ASyncLoadCount != 0 ? TRUE : FALSE ;

	// クリティカルセクションの解放
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	return Result ;
}

// ハンドルの非同期読み込み処理の戻り値を取得する( 非同期読み込み中の場合は一つ前の非同期読み込み処理の戻り値が返ってきます )
extern int NS_GetHandleASyncLoadResult( int Handle )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;
	int Result ;

	if( HandleManage->InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// エラー判定
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return -1 ;
	}

	Result = HandleInfo->ASyncLoadResult ;

	// クリティカルセクションの解放
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	return Result ;
}

// ハンドルの非同期読み込み処理が完了したらハンドルを削除するフラグを立てる
extern int NS_SetASyncLoadFinishDeleteFlag(	int Handle )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	if( HandleManage->InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// エラー判定
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return -1 ;
	}

	// 既に非同期読み込みが完了していたらこの場でハンドルを削除する
	if( HandleInfo->ASyncLoadCount == 0 )
	{
		SubHandle( Handle, FALSE, FALSE ) ;
	}
	else
	{
		HandleInfo->ASyncLoadFinishDeleteRequestFlag = TRUE ;
	}

	// クリティカルセクションの解放
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	return 0 ;
}

// ハンドルの非同期読み込み処理が完了したら呼ばれる関数をセットする
extern int NS_SetASyncLoadFinishCallback( int Handle, void ( *Callback )( int Handle, void *Data ), void *Data )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	if( HandleManage->InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// エラー判定
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return -1 ;
	}

	// 既に非同期読み込みが完了していたらこの場でコールバック関数を呼ぶ
	if( HandleInfo->ASyncLoadCount == 0 )
	{
		Callback( Handle, Data ) ;
	}
	else
	{
		HandleInfo->ASyncLoadFinishCallback = ( volatile void (*)( int, void * ) )Callback ;
		HandleInfo->ASyncLoadFinishCallbackData = Data ;
	}

	// クリティカルセクションの解放
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	return 0 ;
}

// 指定のハンドルの非同期読み込み処理が終了するまで待つ
extern int NS_WaitHandleASyncLoad( int Handle )
{
	return WaitASyncLoad( Handle ) ;
}

// 全ての非同期読み込みデータが読み込み終わるまで待つ
extern int NS_WaitHandleASyncLoadAll( void )
{
	while( NS_GetASyncLoadNum() > 0 )
	{
		if( NS_ProcessMessage() < 0 )
		{
			break ;
		}
		Thread_Sleep( 1 ) ;
	}

	return 0 ;
}

// ハンドルの非同期読み込み中カウントをインクリメントする
extern int IncASyncLoadCount( int Handle, int ASyncDataNumber )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	if( HandleManage->InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// エラー判定
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return -1 ;
	}

	HandleInfo->ASyncLoadCount ++ ;
	HandleInfo->ASyncDataNumber = ASyncDataNumber ;

	// クリティカルセクションの解放
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// 終了
	return 0 ;
}

// ハンドルの非同期読み込み中カウントをデクリメントする
extern int DecASyncLoadCount( int Handle )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	if( HandleManage->InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// エラー判定
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return -1 ;
	}

	HandleInfo->ASyncLoadCount -- ;

	// カウントが 0 だったら処理を分岐
	if( HandleInfo->ASyncLoadCount == 0 )
	{
		// 読み込みが終わったら呼ぶコールバック関数が設定されていたら呼ぶ
		if( HandleInfo->ASyncLoadFinishCallback != NULL )
		{
			HandleInfo->ASyncLoadFinishCallback( Handle, HandleInfo->ASyncLoadFinishCallbackData ) ;
		}

		// 読み込みが終わったら削除するフラグが立っていたらハンドルを削除する
		if( HandleInfo->ASyncLoadFinishDeleteRequestFlag )
		{
			SubHandle( Handle, FALSE, TRUE ) ;
		}
	}

	// クリティカルセクションの解放
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// 終了
	return 0 ;
}

// ハンドルの非同期読み込み完了後に削除するかどうかのフラグを取得する
extern int GetASyncLoadFinishDeleteFlag( int Handle )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;
	int Result ;

	if( HandleManage->InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// エラー判定
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return -1 ;
	}

	Result = HandleInfo->ASyncLoadFinishDeleteRequestFlag ;

	// クリティカルセクションの解放
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// 終了
	return Result ;
}

// ハンドルが非同期読み込み中だった場合、非同期読み込みが完了するまで待つ
extern int WaitASyncLoad( int Handle )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	// メインスレッドではなかったらエラー
	if( CheckMainThread() == FALSE )
	{
		return -1 ;
	}

	if( HandleManage->InitializeFlag == FALSE )
		return -1 ;

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	// エラー判定
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		return -1 ;
	}

	// 非同期読み込み中である場合は処理が完了するまで待つ
	if( HandleInfo->ASyncLoadCount != 0 )
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

		// 処理終了待ち
		while( HandleInfo->ASyncLoadCount != 0 )
		{
			ProcessASyncLoadRequestMainThread() ;
			Thread_Sleep( 1 );
		}
	}
	else
	{
		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManage->CriticalSection ) ;
	}

	// 終了
	return 0 ;
}

// 削除リクエストが来ているハンドルを削除する
extern int DeleteRequestHandleDelete( int AllDelete )
{
	HANDLEINFO *HandleInfo ;
	int StartTime = NS_GetNowCount( FALSE ) ;
	int i ;
	int DeleteRequestTotal ;

	// メインスレッドではなかったらエラー
	if( CheckMainThread() == FALSE )
	{
		return -1 ;
	}

START:

	DeleteRequestTotal = 0 ;

	for( i = 0 ; i < DX_HANDLETYPE_MAX ; i ++ )
	{
		// ハンドル管理データが初期化されていなければ何もしない
		if( HandleManageArray[ i ].InitializeFlag == FALSE )
		{
			continue ;
		}

		// 削除リクエストが無ければ何もしない
		if( HandleManageArray[ i ].DeleteRequestHandleNum == 0 )
		{
			continue ;
		}

		// クリティカルセクションの取得
		CRITICALSECTION_LOCK( &HandleManageArray[ i ].CriticalSection ) ;

		// 削除リクエストが無ければループから抜ける
		while( HandleManageArray[ i ].DeleteRequestHandleNum > 0 )
		{
			// 非同期読み込みが完了しているハンドルを探す
			HandleInfo = ( HANDLEINFO * )HandleManageArray[ i ].DeleteRequestListLast.Prev->Data ;
			while( HandleInfo != NULL && HandleInfo->ASyncLoadCount != 0 )
			{
				HandleInfo = ( HANDLEINFO * )HandleInfo->DeleteRequestList.Next->Data ;
			}

			// 非同期読み込みが完了しているハンドルが無かったらループを抜ける
			if( HandleInfo == NULL )
			{
				break ;
			}

			// ハンドルを削除
			if( HandleInfo != NULL )
			{
				SubHandleBase( &HandleManageArray[ i ], HandleInfo ) ;
			}

			// 2ms経過していたらループを抜ける
			if( NS_GetNowCount( FALSE ) - StartTime >= 2 )
			{
				break ;
			}
		}

		// 削除リクエストの残り数を加算
		DeleteRequestTotal += HandleManageArray[ i ].DeleteRequestHandleNum ;

		// クリティカルセクションの解放
		CriticalSection_Unlock( &HandleManageArray[ i ].CriticalSection ) ;
	}

	// 全て削除する指定で、まだ削除できていないハンドルがある場合は関数の先頭に戻る
	if( AllDelete && DeleteRequestTotal > 0 )
	{
		// メインスレッドが処理する非同期読み込みの処理を行う
		ProcessASyncLoadRequestMainThread() ;

		goto START ;
	}

	// 正常終了
	return 0 ;
}

#endif // DX_NON_ASYNCLOAD









// ハンドルリスト

// リストの初期化
extern int InitializeHandleList( HANDLELIST *First, HANDLELIST *Last )
{
	First->Handle = -1 ;
	First->Data   = NULL ;
	First->Prev   = NULL ;
	First->Next   = Last ;
	Last->Handle  = -1 ;
	Last->Data    = NULL ;
	Last->Prev    = First ;
	Last->Next    = NULL ;

	// 終了
	return 0 ;
}

// リストへ要素を追加
extern int AddHandleList( HANDLELIST *First, HANDLELIST *List, int Handle, void *Data )
{
	List->Handle      = Handle ;
	List->Data        = Data ;
	List->Prev        = First ;
	List->Next        = First->Next ;
	First->Next->Prev = List ;
	First->Next       = List ;

	// 終了
	return 0 ;
}

// リストから要素を外す
extern int SubHandleList( HANDLELIST *List )
{
	List->Prev->Next = List->Next ;
	List->Next->Prev = List->Prev ;
	List->Prev = NULL ;
	List->Next = NULL ;

	return 0 ;
}

// リストが存在するメモリが変更された場合にリストの前後を更新する
extern int NewMemoryHandleList( HANDLELIST *List, void *Data )
{
	List->Data = Data ;

	List->Prev->Next = List ;
	List->Next->Prev = List ;

	// 終了
	return 0 ;
}









// シンプルリスト

// リストの初期化
extern int InitializeSimpleList( SIMPLELIST *First, SIMPLELIST *Last )
{
	First->Data = NULL ;
	First->Prev = NULL ;
	First->Next = Last ;
	Last->Data  = NULL ;
	Last->Prev  = First ;
	Last->Next  = NULL ;

	// 終了
	return 0 ;
}

// リストへ要素を追加
extern int AddSimpleList( SIMPLELIST *First, SIMPLELIST *List, void *Data )
{
	List->Data        = Data ;
	List->Prev        = First ;
	List->Next        = First->Next ;
	First->Next->Prev = List ;
	First->Next       = List ;

	// 終了
	return 0 ;
}

// リストから要素を外す
extern int SubSimpleList( SIMPLELIST *List )
{
	List->Prev->Next = List->Next ;
	List->Next->Prev = List->Prev ;

	return 0 ;
}

// リストが存在するメモリが変更された場合にリストの前後を更新する
extern int NewMemorySimpleList( SIMPLELIST *List, void *Data )
{
	List->Data = Data ;

	List->Prev->Next = List ;
	List->Next->Prev = List ;

	// 終了
	return 0 ;
}







// ハンドルが削除されたときに−１が設定される変数を登録する
extern int NS_SetDeleteHandleFlag( int Handle, int *DeleteFlag )
{
	HANDLEINFO *HandleInfo ;
	int HandleType = ( int )( ( ( DWORD )Handle & DX_HANDLETYPE_MASK ) >> DX_HANDLETYPE_ADDRESS ) ;

	// エラー判定
	if( HANDLECHK_ASYNC( HandleType, Handle, HandleInfo ) )
		return -1 ;

	HandleInfo->DeleteFlag = DeleteFlag  ;

	// 終了
	return 0 ;
}

// 指定のタイプのハンドルの数を取得する
extern int NS_GetHandleNum(	int HandleType /* DX_HANDLETYPE_GRAPH等 */ )
{
	// タイプチェック
	if( HandleType < 0 || HandleType >= DX_HANDLETYPE_MAX )
	{
		return -1 ;
	}

	// ハンドルの数を返す
	return HandleManageArray[ HandleType ].Num ;
}

// 指定のタイプのハンドルが最大で幾つ作成できるかを取得する
extern int NS_GetMaxHandleNum( int HandleType /* DX_HANDLETYPE_GRAPH等 */ )
{
	// タイプチェック
	if( HandleType < 0 || HandleType >= DX_HANDLETYPE_MAX )
	{
		return -1 ;
	}

	// ハンドルの最大数を返す
	return HandleManageArray[ HandleType ].MaxNum ;
}

// 指定のタイプの全ハンドルの情報をログに出力する
extern int NS_DumpHandleInfo( int HandleType /* DX_HANDLETYPE_GRAPH等 */ )
{
	int i ;
	HANDLEINFO **ppHandleInfo ;
	HANDLEMANAGE *HandleManage = &HandleManageArray[ HandleType ] ;

	// タイプチェック
	if( HandleType < 0 || HandleType >= DX_HANDLETYPE_MAX )
	{
		return -1 ;
	}

	if( HandleManage->InitializeFlag == FALSE )
		return -1 ;

	// クリティカルセクションの取得
	CRITICALSECTION_LOCK( &HandleManage->CriticalSection ) ;

	ppHandleInfo = &HandleManage->Handle[ HandleManage->AreaMin ] ;
	for( i = HandleManage->AreaMin ; i <= HandleManage->AreaMax ; i ++, ppHandleInfo ++ )
	{
		if( *ppHandleInfo == NULL )
		{
			continue ;
		}

		if( HandleManage->DumpInfoFunction != NULL )
		{
			HandleManage->DumpInfoFunction( *ppHandleInfo ) ;
		}
	}
	DXST_LOGFILEFMT_ADDW(( L"Total Count : %d", HandleManage->Num )) ;

	// クリティカルセクションの解放
	CriticalSection_Unlock( &HandleManage->CriticalSection ) ;

	// 終了
	return 0 ;
}







#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE
