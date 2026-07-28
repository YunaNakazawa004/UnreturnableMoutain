//========================================================================
// 
// ジャンプバーヘッダー [ tutorial_jump.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _TUTORIAL_JUMP_H_		// このマクロ定義がされていなかったら
#define _TUTORIAL_JUMP_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object3D.h"

//************************************************************************
// ジャンプバークラス
//************************************************************************
class CTutorialJump : public CObject3D
{
public:
	CTutorialJump(const int nPriority = PRIORITY_3);
	virtual ~CTutorialJump();

	static HRESULT Load(void);
	static void Unload(void);
	static CTutorialJump* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool GetClear(void) { return m_bClear; }

	void CollisionPlayer(void);

private:
	static int m_nIdxTexture;			// テクスチャのインデックス
	bool m_bClear;		// クリア
};

#endif