//========================================================================
// 
// リザルトUIヘッダー [ UI_result.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _UI_RESULT_H_		// このマクロ定義がされていなかったら
#define _UI_RESULT_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object2D.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define EFFECT2D_WIDTH		(600.0f)								// リザルトUIの幅
#define EFFECT2D_HEIGHT		(120.0f)								// リザルトUIの高さ

//************************************************************************
// リザルトUIクラス
//************************************************************************
class CResultUI : public CObject2D
{
public:
	// 種類
	typedef enum
	{
		TYPE_CLEAR = 0,		// クリア
		TYPE_GAMEOVER,		// ゲームオーバー
		TYPE_MAX
	}TYPE;

	CResultUI(const int nPriority = UI_PRIORITY);
	~CResultUI();

	static HRESULT Load(void);
	static void Unload(void);
	static CResultUI* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const TYPE type);

	HRESULT Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static int m_aIdxTexture[TYPE_MAX];		// テクスチャのインデックス
	TYPE m_type;		// 種類
};

#endif