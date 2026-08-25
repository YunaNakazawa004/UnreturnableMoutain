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
		BGM_TITLE,					// タイトルBGM
		BGM_GAME,					// ゲームBGM
		SE_COLLECT,					// 収集
		SE_ROBOT_ENERGY,			// エネルギー収集時のロボットアーム
		SE_ENERGY,					// エネルギー回収
		SE_ENTER,					// 選択
		SE_JUMPCLEAR,				// チュートリアルのジャンプクリア
		SE_LEAVE,					// 出発ボタン選択
		SE_OPENCLOSE,				// 開閉
		SE_CURSOR,					// カーソル
		SE_PAUSE,					// ポーズ
		SE_ROCKET,					// ロケット発射
		SE_SEA,						// 海
		SE_TIRE,					// タイヤ
		SE_WIND,					// 風
		SE_DECISION,				// 判定
		SE_GAMEOVER,				// ゲームオーバー
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
