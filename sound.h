//=============================================================================
// 
// サウンド処理 [ sound.h ]
// Author : Nakazawa Yuna
// 
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドクラス
//*****************************************************************************
class CSound
{
public:
	// サウンド一覧
	typedef enum
	{
		SOUND_BGM_GAME,						// ゲームBGM
		SOUND_SE_SHOT,						// ショット
		SOUND_SE_HIT,						// ヒット
		SOUND_SE_BURST,						// バースト
		SOUND_SE_ENTER,						// 選択
		SOUND_SE_CANCEL,					// キャンセル
		SOUND_SE_CLEAR,						// クリア
		SOUND_SE_FAIL,						// 失敗
		SOUND_SE_CURSOR,					// カーソル
		SOUND_SE_PAUSE,						// ポーズ
		SOUND_MAX,
	} SOUND_LABEL;

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);

private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	static IXAudio2* m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice* m_pMasteringVoice;			// マスターボイス
	static IXAudio2SourceVoice* m_apSourceVoice[SOUND_MAX];		// ソースボイス
	static BYTE* m_apDataAudio[SOUND_MAX];						// オーディオデータ
	static DWORD m_aSizeAudio[SOUND_MAX];						// オーディオデータサイズ

	// サウンド情報の構造体定義
	typedef struct
	{
		const char* pFilename;	// ファイル名
		int nCntLoop;			// ループカウント
	} SOUNDINFO;

	static SOUNDINFO m_aSoundInfo[SOUND_MAX];					// サウンドの情報
};

#endif
