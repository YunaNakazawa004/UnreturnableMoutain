//========================================================================
// 
// メッシュフィールドヘッダー [meshfield.h]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _MESHFIELD_H_		// このマクロ定義がされていなかったら
#define _MESHFIELD_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define ERROR_HEIGHT			(-99999.0f)					// 高さを算出できなかった場合の数値

//************************************************************************
// メッシュフィールドクラス
//************************************************************************
class CMeshField : public CObject
{
public:
	CMeshField(const int nPriority);
	virtual ~CMeshField();

	static CMeshField* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR2 block,
		const D3DXVECTOR2 size, const CObject::TYPE type, const char* pFilename, const int nPriority = BG_PRIORITY);
	HRESULT Init(void) { return S_OK; }
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
		const D3DXVECTOR2 block, const D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void BindTexture(const int nIdxTexture) { m_nIdxTexture = nIdxTexture; }

	void SetPosition(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetRotation(const D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	void SetScale(const D3DXVECTOR3 scale);
	D3DXVECTOR3 GetScale(void) { return m_scale; }
	void SetColor(const D3DXCOLOR col);
	void SetColor(const int nCntVtx, const D3DXCOLOR col);
	D3DXCOLOR GetColor(void) { return m_col; }
	void SetTexUV(const int nCntVtx, const float ftexU, const float ftexV);
	void SetNor(void);
	void SetDisp(const bool bDisp) { m_bDisp = bDisp; }
	void SwapDisp(void) { m_bDisp = m_bDisp ? false : true; }

	D3DXVECTOR2 GetPolygonIdx(const D3DXVECTOR3 pos);
	float GetHeight(const D3DXVECTOR3 pos, const D3DXVECTOR2 polygonIdx);

	HRESULT ReadData(const char* pFilename);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;			// インデックスバッファへのポインタ
	int m_nIdxTexture;			// テクスチャのインデックス
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_scale;		// スケール
	D3DXVECTOR2 m_block;		// 分割数
	D3DXVECTOR2 m_size;			// サイズ
	D3DXCOLOR m_col;			// 色
	int m_nNumVtx;				// 頂点の総数
	int m_nNumIdx;				// インデックスの総数
	bool m_bDisp;				// 表示するかどうか
};

#endif