//========================================================================
// 
// 使用エネルギー量ヘッダー [ used_energy.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _USED_ENERGY_H_		// このマクロ定義がされていなかったら
#define _USED_ENERGY_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object.h"

//************************************************************************
// 前方宣言
//************************************************************************
class CNumber;

//************************************************************************
// マクロ定義
//************************************************************************
#define USED_ENERGY_NUMPLACE		(8)				// 使用エネルギー量の桁数
#define USED_ENERGY_WIDTH			(36.0f)			// 使用エネルギー量の幅
#define USED_ENERGY_HEIGHT			(72.0f)			// 使用エネルギー量の高さ

//************************************************************************
// 使用エネルギー量クラス
//************************************************************************
class CUsedEnergy : public CObject
{
public:
	CUsedEnergy(const int nPriority = PRIORITY_6);
	~CUsedEnergy();

	static CUsedEnergy* Create(const D3DXVECTOR3 pos,
		const float fWidth, const float fHeight);
	HRESULT Init(void) { return S_OK; }
	HRESULT Init(const D3DXVECTOR3 pos,
		const float fWidth, const float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRotation(void) { return DEFAULT_VECTER3; }
	static void SetUsedEnergy(const int nUsedEnergy) { m_nUsedEnergy = nUsedEnergy; }
	static int GetUsedEnergy(void) { return m_nUsedEnergy; }
	void SetNum(const int nUsedEnergy);
	void Add(const int nAdd);
	void Minus(const int nMinus);
	void SetDisp(const bool bDisp) { m_bDisp = bDisp; }
	void SwapDisp(void) { m_bDisp = m_bDisp ? false : true; }

private:
	CNumber* m_apNumber[USED_ENERGY_NUMPLACE];		// 桁数分の数字のインスタンス
	D3DXVECTOR3 m_pos;			// 基準の位置
	static int m_nUsedEnergy;	// 使用エネルギー量
	bool m_bDisp;				// 表示するかどうか
};

#endif