//========================================================================
// 
// エネルギーUIヘッダー [ UI_energy.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _UI_ENERGY_H_		// このマクロ定義がされていなかったら
#define _UI_ENERGY_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object2D.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define ENERGYUI_WIDTH		(150.0f)				// エネルギーUIの幅
#define ENERGYUI_HEIGHT		(150.0f)				// エネルギーUIの高さ
#define ENERGYUI_NUMPLACE	(3)						// UIの数値の桁数

//************************************************************************
// 前方宣言
//************************************************************************
class CNumber;

//************************************************************************
// エネルギーUIクラス
//************************************************************************
class CEnergyUI : public CObject2D
{
public:
	CEnergyUI(const int nPriority = UI_PRIORITY);
	~CEnergyUI();

	static HRESULT Load(void);
	static void Unload(void);
	static CEnergyUI* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);

	HRESULT Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetEnergy(const float fEnergy);

private:
	static int m_nIdxTexture;					// テクスチャのインデックス
	CNumber* m_apNumber[ENERGYUI_NUMPLACE];		// エネルギーの数値
};

#endif