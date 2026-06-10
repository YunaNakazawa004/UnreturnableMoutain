//========================================================================
// 
// エネルギー鉱物ヘッダー [energyrock.h]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _ENERGYROCK_H_		// このマクロ定義がされていなかったら
#define _ENERGYROCK_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "objectX.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define ENERGYROCK_RADIUS		(30.0f)			// エネルギー鉱物の半径
#define ENERGYROCK_HEIGHT		(40.0f)			// エネルギー鉱物の高さ

//************************************************************************
// エネルギー鉱物クラス
//************************************************************************
class CEnergyRock : public CObjectX
{
public:
	CEnergyRock(const int nPriority = OBJECT_PRIORITY);
	virtual ~CEnergyRock();

	static CEnergyRock* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
};

#endif