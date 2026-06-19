//========================================================================
// 
// 草ヘッダー [grass.h]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _GRASS_H_		// このマクロ定義がされていなかったら
#define _GRASS_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object3D.h"

//************************************************************************
// 草クラス
//************************************************************************
class CGrass : public CObject3D
{
public:
	CGrass(const int nPriority = OBJECT_PRIORITY);
	virtual ~CGrass();

	static HRESULT Load(void);
	static void Unload(void);
	static CGrass* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void CollisionPlayer(void);

private:
	static int m_nIdxTexture;			// テクスチャのインデックス
	D3DXVECTOR3 m_rotOff;				// 向きオフセット
	float m_fShake;						// 揺れ具合;
};

#endif