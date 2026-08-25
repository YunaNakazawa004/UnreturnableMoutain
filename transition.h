//=============================================================================
// 
// 画面遷移ヘッダー [ transition.h ]
// Author : Nakazawa Yuna
// 
//=============================================================================
#ifndef _TRANSITION_H_		// このマクロ定義がされていなかったら
#define _TRANSITION_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "scene.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define MAX_MODEL_TEXTURE		(16)		// モデルの最大テクスチャ

//*****************************************************************************
// 画面遷移クラス
//*****************************************************************************
class CTransition
{
public:
	// 画面遷移の状態
	typedef enum
	{
		FADE_NONE = 0,			// 何もしていない状態
		FADE_IN,				// フェードイン状態
		FADE_OUT,				// フェードアウト状態
		FADE_MAX
	}FADE;

	// 遷移の種類
	typedef enum
	{
		TYPE_NORMAL = 0,		// 通常
		TYPE_GAMEOVER,			// ゲームオーバー
		TYPE_MAX
	}TYPE;

	CTransition();
	~CTransition();

	static HRESULT Load(void);
	static void Unload(void);
	static CTransition* Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetTransition(CScene::MODE modenext, const TYPE type = TYPE_NORMAL);
	void SetState(FADE fade);
	FADE GetTransition(void) { return m_fade; }

private:
	static int m_aIdxTexture[TYPE_MAX];	// テクスチャのインデックス
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	FADE m_fade;						// 画面遷移の状態
	TYPE m_type;						// 遷移の種類
	CScene::MODE m_modeNext;			// 次の画面(モード)
	D3DXCOLOR m_col;					// 色
	int m_nPatternAnim;					// パターンNO
	int m_nCounterAnim;					// アニメーションカウンター

	LPD3DXMESH m_pMesh;						// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;				// マテリアルへのポインタ
	int m_apTexture[MAX_MODEL_TEXTURE];		// テクスチャへのポインタ
	DWORD m_dwNumMat;						// マテリアルの数
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_rot;			// 向き
};

#endif