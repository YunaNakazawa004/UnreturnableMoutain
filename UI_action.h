//========================================================================
// 
// アクションUIヘッダー [ UI_action.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _UI_ACTION_H_		// このマクロ定義がされていなかったら
#define _UI_ACTION_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object2D.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define ACTIONUI_WIDTH		(600.0f)								// アクションUIの幅
#define ACTIONUI_HEIGHT		(120.0f)								// アクションUIの高さ

//************************************************************************
// アクションUIクラス
//************************************************************************
class CActionUI : public CObject2D
{
public:
	// 種類
	enum FADE
	{
		FADE_NONE = 0,		// フェードなし
		FADE_IN,			// フェードイン
		FADE_OUT,			// フェードアウト
		FADE_MAX
	};

	CActionUI(const int nPriority = PRIORITY_6);
	~CActionUI();

	static HRESULT Load(void);
	static void Unload(void);
	static CActionUI* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);

	HRESULT Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetFade(const int fade);
	void NearEnergyrock(const bool bNear) { m_bEnergyrock = bNear; }
	void NearShip(const bool bNear) { m_bShip = bNear; }

private:
	static int m_nIdxTexture;		// テクスチャのインデックス
	int m_fade;				// 種類
	bool m_bEnergyrock;		// エネルギー鉱石の近く
	bool m_bShip;			// 船の近く
};

#endif