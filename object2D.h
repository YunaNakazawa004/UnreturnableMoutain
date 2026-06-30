//========================================================================
// 
// オブジェクト2Dヘッダー [ object2D.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _OBJECT2D_H_		// このマクロ定義がされていなかったら
#define _OBJECT2D_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object.h"

//************************************************************************
// オブジェクト2Dクラス
//************************************************************************
class CObject2D : public CObject
{
public:
	CObject2D(const int nPriority);
	virtual ~CObject2D();

	static CObject2D* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const CObject::TYPE type, 
		const char* pFilename, const int nPriority = BG_PRIORITY);
	HRESULT Init(void);
	HRESULT Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);
	HRESULT Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const bool bLeft);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void BindTexture(const int nIdxTexture) { m_nIdxTexture = nIdxTexture; }

	void SetPosition(const D3DXVECTOR3 pos);
	void SetPosition(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetSize(const float fLength);
	float GetSize(void) { return m_fLength; }
	void SetRotation(const D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	void SetColor(const D3DXCOLOR col);
	D3DXCOLOR GetColor(void) { return m_col; }
	void SetTexUV(const int nPatternAnim, const int nTexWidth, const int nTexHeight, const float fScrollX, const float fScrollY);
	void SetDisp(const bool bDisp) { m_bDisp = bDisp; }
	void SwapDisp(void) { m_bDisp = m_bDisp ? false : true; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファへのポインタ
	int m_nIdxTexture;			// テクスチャのインデックス
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_rot;			// 向き
	D3DXCOLOR m_col;			// 色
	float m_fAngle;				// 角度
	float m_fLength;			// 対角線の長さ
	bool m_bDisp;				// 表示するかどうか
};

#endif