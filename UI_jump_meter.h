//========================================================================
// 
// ジャンプメーターUIヘッダー [ UI_jump_meter.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _UI_JUMP_METER_H_		// このマクロ定義がされていなかったら
#define _UI_JUMP_METER_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object2D.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define JUMPMETERUI_WIDTH		(150.0f)				// ジャンプメーターUIの幅
#define JUMPMETERUI_HEIGHT		(150.0f)				// ジャンプメーターUIの高さ
#define JUMPMETERUI_NUMPLACE	(3)						// UIの数値の桁数

//************************************************************************
// ジャンプメーターUIクラス
//************************************************************************
class CJumpMeterUI : public CObject2D
{
public:
	CJumpMeterUI(const int nPriority = UI_PRIORITY);
	~CJumpMeterUI();

	static HRESULT Load(void);
	static void Unload(void);
	static CJumpMeterUI* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fJump);

	HRESULT Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fJump);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetJumpMeter(const float fJump);

private:
	static int m_nIdxTexture;					// テクスチャのインデックス
	float m_fJump;				// ジャンプ量
	float m_fWidth;				// 既定の幅
	float m_fHeight;			// 既定の高さ
};

#endif