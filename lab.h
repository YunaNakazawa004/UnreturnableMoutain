//========================================================================
// 
// 研究所ヘッダー [ lab.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _LAB_H_		// このマクロ定義がされていなかったら
#define _LAB_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define MAX_MODEL		(16)		// 研究所の最大

//************************************************************************
// 前方宣言
//************************************************************************
class CModel;

//************************************************************************
// 研究所クラス
//************************************************************************
class CLab : public CObject
{
public:
	CLab(const int nPriority = OBJECT_PRIORITY);
	virtual ~CLab();

	static CLab* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	HRESULT Init(void) { return S_OK; }
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPosition(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetRotation(const D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	void SetScale(const D3DXVECTOR3 scale);
	D3DXVECTOR3 GetScale(void) { return m_scale; }
	void SetColor(const D3DXCOLOR col);
	D3DXCOLOR GetColor(void) { return m_col; }
	void SetDisp(const bool bDisp) { m_bDisp = bDisp; }
	void SwapDisp(void) { m_bDisp = m_bDisp ? false : true; }

	bool Collision(D3DXVECTOR3* pos, D3DXVECTOR3* posOld, D3DXVECTOR3* move,
		const float fRadius, const float fHeight, bool* pLand);

private:
	HRESULT SetModel(const char* pFilename);

	const char* m_apFileName[MAX_MODEL];		// 研究所Xファイルの名前
	CModel* m_apModel[MAX_MODEL];				// モデル（パーツ）へのポインタ
	int m_nNumModel;							// モデル（パーツ）の総数

	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3 m_pos;				// 現在の位置
	D3DXVECTOR3 m_move;				// 移動量
	D3DXVECTOR3 m_rot;				// 向き
	D3DXVECTOR3 m_rotDest;			// 目的の向き
	D3DXVECTOR3 m_scale;			// スケール
	D3DXCOLOR m_col;				// 色
	float m_fRadius;				// 半径
	float m_fHeight;				// 高さ

	bool m_bDisp;					// 表示するかどうか
};

#endif