//========================================================================
// 
// スコアヘッダー [ score.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _SCORE_H_		// このマクロ定義がされていなかったら
#define _SCORE_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object.h"

//************************************************************************
// 前方宣言
//************************************************************************
class CNumber;

//************************************************************************
// マクロ定義
//************************************************************************
#define SCORE_NUMPLACE		(8)				// スコアの桁数
#define SCORE_WIDTH			(36.0f)			// スコアの幅
#define SCORE_HEIGHT		(72.0f)			// スコアの高さ

//************************************************************************
// スコアクラス
//************************************************************************
class CScore : public CObject
{
public:
	CScore(const int nPriority = PRIORITY_6);
	~CScore();

	static CScore* Create(const D3DXVECTOR3 pos,
		const float fWidth, const float fHeight);
	HRESULT Init(void) { return S_OK; }
	HRESULT Init(const D3DXVECTOR3 pos,
		const float fWidth, const float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetRotation(void) { return DEFAULT_VECTER3; }
	void SetColor(D3DXCOLOR col);
	int GetScore(void) { return m_nScore; }
	void SetNum(const int nScore);
	void Add(const int nAdd);
	void Minus(const int nMinus);
	void SetDisp(const bool bDisp) { m_bDisp = bDisp; }
	void SwapDisp(void) { m_bDisp = m_bDisp ? false : true; }

private:
	CNumber* m_apNumber[SCORE_NUMPLACE];		// 桁数分の数字のインスタンス
	D3DXVECTOR3 m_pos;		// 基準の位置
	int m_nScore;			// スコア
	bool m_bDisp;			// 表示するかどうか
};

#endif