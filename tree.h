//========================================================================
// 
// 木ヘッダー [ tree.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _TREE_H_		// このマクロ定義がされていなかったら
#define _TREE_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define MAX_MODEL		(16)		// プレイヤーの最大

//************************************************************************
// 前方宣言
//************************************************************************
class CModel;
class CMotion;

//************************************************************************
// 木クラス
//************************************************************************
class CTree : public CObject
{
public:
	CTree(const int nPriority = PLAYER_PRIORITY);
	virtual ~CTree();

	static CTree* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	HRESULT Init(void) { return S_OK; }
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetShake(const bool bShake) { m_bShake = bShake; }
	bool GetShake(void) { return m_bShake; }

	void SetPosition(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetRotation(const D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	void SetScale(const D3DXVECTOR3 scale);
	D3DXVECTOR3 GetScale(void) { return m_scale; }
	void SetColor(const D3DXCOLOR col);
	D3DXCOLOR GetColor(void) { return m_col; }
	void SetDisp(const bool bDisp) { m_bDisp = bDisp; }
	bool GetDisp(void) { return m_bDisp; }
	void SwapDisp(void) { m_bDisp = m_bDisp ? false : true; }

	static bool Collision(D3DXVECTOR3* pos, D3DXVECTOR3* posOld, D3DXVECTOR3* move,
		const float fRadius, const float fHeight);
	void Shake(const D3DXVECTOR3 posP);

private:
	CModel* m_apModel[MAX_MODEL];				// モデル（パーツ）へのポインタ
	int m_nNumModel;							// モデル（パーツ）の総数

	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3 m_pos;			// 現在の位置
	D3DXVECTOR3 m_move;			// 移動量
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_scale;		// スケール
	D3DXCOLOR m_col;			// 色

	bool m_bShake;				// 揺らすかどうか
	bool m_bDisp;				// 表示するかどうか
};

#endif