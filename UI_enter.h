//========================================================================
// 
// エンターUIヘッダー [ UI_enter.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _UI_ENTER_H_		// このマクロ定義がされていなかったら
#define _UI_ENTER_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object2D.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define ENTERUI_WIDTH		(600.0f)								// エンターUIの幅
#define ENTERUI_HEIGHT		(400.0f)								// エンターUIの高さ

//************************************************************************
// エンターUIクラス
//************************************************************************
class CEnterUI : public CObject2D
{
public:
	// 状態
	enum STATE
	{
		STATE_NONE = 0,		// 状態なし
		STATE_WAIT,			// 待機状態
		STATE_BLINK,		// 点滅状態
		STATE_MAX
	};

	CEnterUI(const int nPriority = PRIORITY_6);
	~CEnterUI();

	static HRESULT Load(void);
	static void Unload(void);
	static CEnterUI* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);

	HRESULT Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static int m_nIdxTexture;		// テクスチャのインデックス
	int m_state;					// 状態
	int m_nCounterState;			// 状態カウンター
	float m_fWidth;					// 幅
	float m_fHeight;				// 高さ
};

#endif