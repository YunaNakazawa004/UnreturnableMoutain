//========================================================================
// 
// ロケーターヘッダー [ locater.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _LOCATER_H_		// このマクロ定義がされていなかったら
#define _LOCATER_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object3D.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define LOCATER_WIDTH		(150.0f)				// ロケーターの幅
#define LOCATER_HEIGHT		(150.0f)				// ロケーターの高さ
#define LOCATER_NUM			(8)						// ロケートするアイテムの数

//************************************************************************
// ロケータークラス
//************************************************************************
class CLocater : public CObject3D
{
public:
	// 矢印の種類
	enum TYPE
	{
		TYPE_NEAR = 0,		// 近い
		TYPE_MIDDLE,		// 中
		TYPE_FAR,			// 遠い	
		TYPE_FRAME,			// 枠
		TYPE_MAX
	};

	CLocater(const int nPriority = PRIORITY_2);
	~CLocater();

	static HRESULT Load(void);
	static void Unload(void);
	static CLocater* Create(const D3DXVECTOR3 pos, const float fWidth, const float fDepth);

	HRESULT Init(const D3DXVECTOR3 pos, const float fWidth, const float fDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Locate(const D3DXVECTOR3 pos);
	void SetArrow(const D3DXVECTOR3 pos, const int nIdx);
	void DeleteArrow(const int nIdx);

private:
	static int m_aIdxTexture[TYPE_MAX];		// テクスチャのインデックス
	CObject3D* m_aArrow[LOCATER_NUM];		// 矢印
};

#endif