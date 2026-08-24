//========================================================================
// 
// 雲ヘッダー [ cloud.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _CLOUD_H_		// このマクロ定義がされていなかったら
#define _CLOUD_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "meshdome.h"

//************************************************************************
// マクロ定義
//************************************************************************

//************************************************************************
// 雲クラス
//************************************************************************
class CCloud : public CMeshDome
{
public:
	CCloud(const int nPriority = PRIORITY_2);
	~CCloud();

	static HRESULT Load(void);
	static void Unload(void);
	static CCloud* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR2 block,
		const float fRadius);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
		const D3DXVECTOR2 block, const float fRadius);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static int m_nIdxTexture;	// テクスチャのインデックス

};

#endif