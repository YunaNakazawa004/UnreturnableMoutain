//========================================================================
// 
// 船ヘッダー [ ship.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _SHIP_H_		// このマクロ定義がされていなかったら
#define _SHIP_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define MAX_MODEL		(16)		// 船の最大

//************************************************************************
// 前方宣言
//************************************************************************
class CModel;

//************************************************************************
// 船クラス
//************************************************************************
class CShip : public CObject
{
public:
	// 状態
	typedef enum
	{
		STATE_NONE = 0,		// 状態無し
		STATE_WAIT,			// 待機状態
		STATE_APPEAR,		// 出現状態
		STATE_NORMAL,		// 通常状態
		STATE_READY,		// 準備完了状態
		STATE_MAX
	}STATE;

	CShip(const int nPriority = OBJECT_PRIORITY);
	virtual ~CShip();

	static CShip* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
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

private:
	HRESULT SetModel(const char* pFilename);

	const char* m_apFileName[MAX_MODEL];		// 船Xファイルの名前
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

	STATE m_state;					// 状態
	int m_nCounterState;			// 状態カウンター

	bool m_bDisp;					// 表示するかどうか
};

#endif