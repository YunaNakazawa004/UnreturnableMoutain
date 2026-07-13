//========================================================================
// 
// 砂浜ヘッダー [ beach.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _BEACH_H_		// このマクロ定義がされていなかったら
#define _BEACH_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "meshfield.h"

//************************************************************************
// マクロ定義
//************************************************************************

//************************************************************************
// 砂浜クラス
//************************************************************************
class CBeach : public CMeshField
{
public:
	CBeach(const int nPriority = OBJECT_PRIORITY);
	~CBeach();

	static HRESULT Load(void);
	static void Unload(void);
	static CBeach* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR2 block,
		const D3DXVECTOR2 size);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
		const D3DXVECTOR2 block, const D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static int m_aIdxTexture[FIELD_TEXTURE_NUM];	// テクスチャのインデックス

};

#endif