//========================================================================
// 
// タイトルロゴヘッダー [ UI_title_logo.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _UI_TITLE_LOGO_H_		// このマクロ定義がされていなかったら
#define _UI_TITLE_LOGO_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object2D.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define TITLELOGO_WIDTH			(600.0f)								// タイトルロゴの幅
#define TITLELOGO_HEIGHT		(400.0f)								// タイトルロゴの高さ

//************************************************************************
// タイトルロゴクラス
//************************************************************************
class CTitleLogo : public CObject2D
{
public:
	CTitleLogo(const int nPriority = PRIORITY_6);
	~CTitleLogo();

	static HRESULT Load(void);
	static void Unload(void);
	static CTitleLogo* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);

	HRESULT Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static int m_nIdxTexture;		// テクスチャのインデックス
};

#endif