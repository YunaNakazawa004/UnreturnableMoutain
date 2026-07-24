//========================================================================
// 
// 海ヘッダー [ watersurface.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _WATERSURFACE_H_		// このマクロ定義がされていなかったら
#define _WATERSURFACE_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "meshfield.h"

//************************************************************************
// マクロ定義
//************************************************************************

//************************************************************************
// 海クラス
//************************************************************************
class CWaterSurface : public CMeshField
{
public:
	CWaterSurface(const int nPriority = PRIORITY_4);
	~CWaterSurface();

	static HRESULT Load(void);
	static void Unload(void);
	static CWaterSurface* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR2 block,
		const D3DXVECTOR2 size);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
		const D3DXVECTOR2 block, const D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static int m_aIdxTexture[FIELD_TEXTURE_NUM];	// テクスチャのインデックス
	float m_fWave;			// 波
};

#endif