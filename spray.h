//========================================================================
// 
// 水しぶきヘッダー [ spray.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _SPRAY_H_		// このマクロ定義がされていなかったら
#define _SPRAY_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "ObjectBillboard.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define SPRAY_TEXFILE	"data\\TEXTURE\\spray000.png"		// 水しぶきのテクスチャ
#define SPRAY_WIDTH		(10.0f)								// 水しぶきの幅
#define SPRAY_HEIGHT	(10.0f)								// 水しぶきの高さ

//************************************************************************
// 水しぶきクラス
//************************************************************************
class CSpray : public CObjectBillboard
{
public:
	CSpray(const int nPriority = 4);
	~CSpray();

	static HRESULT Load(void);
	static void Unload(void);
	static CSpray* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fRadius,
		const D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fRadius,
		const D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void Ray(const D3DXVECTOR3 pos, const float fRadius, int type, const float fSpeed,
		const D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

private:
	static int m_nIdxTexture;	// テクスチャのインデックス
	D3DXVECTOR3 m_move;			// 移動量
	float m_fRadius;			// 半径
};

#endif