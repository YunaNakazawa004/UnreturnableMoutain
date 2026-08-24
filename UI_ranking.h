//========================================================================
// 
// ランキングUIヘッダー [ UI_ranking.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _UI_RANKING_H_		// このマクロ定義がされていなかったら
#define _UI_RANKING_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object2D.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define RANKINGUI_WIDTH		(600.0f)								// ランキングUIの幅
#define RANKINGUI_HEIGHT	(120.0f)								// ランキングUIの高さ

//************************************************************************
// ランキングUIクラス
//************************************************************************
class CRankingUI : public CObject2D
{
public:
	CRankingUI(const int nPriority = PRIORITY_6);
	~CRankingUI();

	static HRESULT Load(void);
	static void Unload(void);
	static CRankingUI* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);

	HRESULT Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static int m_nIdxTexture;		// テクスチャのインデックス
};

#endif