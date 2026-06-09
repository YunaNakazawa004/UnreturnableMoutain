//=============================================================================
// 
// ライトヘッダー [light.h]
// Author : Nakazawa Yuna
// 
//=============================================================================
#ifndef _LIGHT_H_		// このマクロ定義がされていなかったら
#define _LIGHT_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"

//*****************************************************************************
// ライトクラス
//*****************************************************************************
class CLight
{
public:
	CLight();
	~CLight();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void SetColor(int nIdx, D3DXCOLOR col);

private:
	D3DLIGHT9 m_aLight[3];			// ライト情報
};

#endif