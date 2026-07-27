//========================================================================
// 
// 岩ヘッダー [ rock.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _ROCK_H_		// このマクロ定義がされていなかったら
#define _ROCK_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "objectX.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define ROCK_RADIUS		(40.0f)			// 岩の半径
#define ROCK_HEIGHT		(40.0f)			// 岩の高さ

//************************************************************************
// 岩クラス
//************************************************************************
class CRock : public CObjectX
{
public:
	CRock(const int nPriority = PRIORITY_3);
	virtual ~CRock();

	static CRock* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRock* Collision(D3DXVECTOR3* pos, D3DXVECTOR3* posOld, D3DXVECTOR3* move,
		const float fRadius, const float fHeight, bool* pLand, bool* pHead);

private:
};

#endif