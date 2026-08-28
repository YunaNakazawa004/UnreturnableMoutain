//========================================================================
// 
// 枠ヘッダー [ frame.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _FRAME_H_		// このマクロ定義がされていなかったら
#define _FRAME_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "Object2D.h"

//************************************************************************
// マクロ定義
//************************************************************************

//************************************************************************
// 枠クラス
//************************************************************************
class CFrame : public CObject2D
{
public:
	CFrame(const int nPriority = PRIORITY_5);
	~CFrame();

	static HRESULT Load(void);
	static void Unload(void);
	static CFrame* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static int m_nIdxTexture;	// テクスチャのインデックス
};

#endif