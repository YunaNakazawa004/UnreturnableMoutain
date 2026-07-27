//========================================================================
// 
// 山ヘッダー [ mountain.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _MOUTAIN_H_		// このマクロ定義がされていなかったら
#define _MOUTAIN_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "meshfield.h"

//************************************************************************
// マクロ定義
//************************************************************************

//************************************************************************
// 山クラス
//************************************************************************
class CMountain : public CMeshField
{
public:
	CMountain(const int nPriority = PRIORITY_3);
	~CMountain();

	static HRESULT Load(void);
	static void Unload(void);
	static CMountain* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR2 block,
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