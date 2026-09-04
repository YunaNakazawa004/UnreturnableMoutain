//========================================================================
// 
// 爆発ヘッダー [ explosion.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _EXPLOSION_H_		// このマクロ定義がされていなかったら
#define _EXPLOSION_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "ObjectBillboard.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define EXPLOSION_TEXFILE	"data\\TEXTURE\\effect000.jpg"		// 爆発のテクスチャ
#define EXPLOSION_WIDTH		(10.0f)								// 爆発の幅
#define EXPLOSION_HEIGHT	(10.0f)								// 爆発の高さ

//************************************************************************
// 爆発クラス
//************************************************************************
class CExplosion : public CObjectBillboard
{
public:
	// 爆発の種類
	enum TYPE
	{
		TYPE_0 = 0,			// タイプ０
		TYPE_1,				// タイプ１
		TYPE_2,				// タイプ２
		TYPE_MAX
	};

	CExplosion(const int nPriority = PRIORITY_3);
	~CExplosion();

	static HRESULT Load(void);
	static void Unload(void);
	static CExplosion* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fRadius,
		int type, const D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fRadius,
		int type, const D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void Ray(const D3DXVECTOR3 pos, const float fRadius, int type, const float fSpeed,
		const D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

private:
	static int m_aIdxTexture[TYPE_MAX];		// テクスチャのインデックス
	D3DXVECTOR3 m_move;			// 移動量
	float m_fRadius;			// 半径
	int m_nCounterAnim;			// アニメーションカウンター
	int m_nPatternAnim;			// アニメーションパターンNO.
	int m_type;					// 種類
};

#endif