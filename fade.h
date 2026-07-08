//=============================================================================
// 
// フェードヘッダー [ fade.h ]
// Author : Nakazawa Yuna
// 
//=============================================================================
#ifndef _FADE_H_		// このマクロ定義がされていなかったら
#define _FADE_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "scene.h"

//*****************************************************************************
// フェードクラス
//*****************************************************************************
class CFade
{
public:
	// フェードの状態
	typedef enum
	{
		FADE_NONE = 0,			// 何もしていない状態
		FADE_IN,				// フェードイン状態
		FADE_OUT,				// フェードアウト状態
		FADE_MAX
	}FADE;

	CFade();
	~CFade();

	static CFade* Create(CScene::MODE modenext);
	HRESULT Init(CScene::MODE modenext);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetFade(CScene::MODE modenext);
	void SetState(FADE fade);
	FADE GetFade(void) { return m_fade; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	FADE m_fade;						// フェードの状態
	CScene::MODE m_modeNext;			// 次の画面(モード)
	D3DXCOLOR m_col;					// 色
};

#endif