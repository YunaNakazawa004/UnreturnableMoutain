//========================================================================
// 
// プレイヤーヘッダー [Player.h]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _PLAYER_H_		// このマクロ定義がされていなかったら
#define _PLAYER_H_		// 2重インクルード防止のマクロを定義する

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
// プレイヤークラス
//************************************************************************
class CPlayer : public CObject
{
public:
	// モーションの種類
	enum MOTION_TYPE
	{
		MOTIONTYPE_NEUTRAL = 0,		// 待機
		MOTIONTYPE_MOVE,			// 移動
		MOTIONTYPE_ACTION,			// アクション
		MOTIONTYPE_JUMP,			// ジャンプ
		MOTIONTYPE_LANDING,			// 着地
		MOTIONTYPE_DEATH,			// 死亡
		MOTIONTYPE_MAX
	};

	CPlayer(const int nPriority = PLAYER_PRIORITY);
	virtual ~CPlayer();

	static CPlayer* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
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

	bool Movement(const D3DXVECTOR3 rot);
	bool CollisionEnergyRock(D3DXVECTOR3* pPos);

private:
	HRESULT SetModel(const char* pFilename);

	const char* m_apFileName[MAX_MODEL];		// プレイヤーXファイルの名前
	CModel* m_apModel[MAX_MODEL];				// モデル（パーツ）へのポインタ
	int m_nNumModel;							// モデル（パーツ）の総数
	CMotion* m_pMotion;							// モーションへのポインタ

	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3 m_pos;				// 現在の位置
	D3DXVECTOR3 m_posOld;			// 前回の位置
	D3DXVECTOR3 m_move;				// 移動量
	D3DXVECTOR3 m_rot;				// 向き
	D3DXVECTOR3 m_rotDest;			// 目的の向き
	D3DXVECTOR3 m_scale;			// スケール
	D3DXCOLOR m_col;				// 色
	float m_fRadius;				// 半径
	float m_fHeight;				// 高さ
	float m_fJumpHigh;				// ジャンプ量

	float m_fEnergy;				// エネルギー
	int m_nEnergyCounter;			// エネルギー減少カウンター

	bool m_bJump;					// ジャンプしているか
	bool m_bMove;					// 移動しているか
	bool m_bLand;					// 着地中かどうか
	bool m_bAct;					// アクション中かどうか
	bool m_bDisp;					// 表示するかどうか
};

#endif