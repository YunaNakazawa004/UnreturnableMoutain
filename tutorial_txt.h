//========================================================================
// 
// チュートリアル文ヘッダー [ tutorial_txt.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _TUTORIAL_TXT_H_		// このマクロ定義がされていなかったら
#define _TUTORIAL_TXT_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object3D.h"

//************************************************************************
// チュートリアル文クラス
//************************************************************************
class CTutorialTxt : public CObject3D
{
public:
	// 種類
	enum TYPE
	{
		TYPE_MOVE = 0,		// 移動
		TYPE_JUMP,			// ジャンプ
		TYPE_ACTION,		// アクション
		TYPE_DEST,			// 目標
		TYPE_MAX
	};

	CTutorialTxt(const int nPriority = PRIORITY_3);
	virtual ~CTutorialTxt();

	static HRESULT Load(void);
	static void Unload(void);
	static CTutorialTxt* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int type);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static int m_aIdxTexture[TYPE_MAX];			// テクスチャのインデックス
};

#endif