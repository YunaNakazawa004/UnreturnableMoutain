//=============================================================================
// 
// 線ヘッダー [ line.h ]
// Author : Nakazawa Yuna
// 
//=============================================================================
#ifndef _LINE_H_		// このマクロ定義がされていなかったら
#define _LINE_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object.h"

//*****************************************************************************
// 船クラス
//*****************************************************************************
class CLine : public CObject
{
public:
	CLine(const int nPriority = 4);
	~CLine();

	static CLine* Create(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
	HRESULT Init(void) { return S_OK; }
	HRESULT Init(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRotation(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	void SetDisp(const bool bDisp) { m_bDisp = bDisp; }
	void SwapDisp(void) { m_bDisp = m_bDisp ? false : true; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;							// 線の位置情報
	D3DXCOLOR m_col;							// 色
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	int m_nDeleteCounter;						// 線が消えるまで
	bool m_bDisp;								// 表示するかどうか
};

#endif