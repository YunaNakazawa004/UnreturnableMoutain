//=============================================================================
// 
// ランキングスコアヘッダー [ rankingscore.h ]
// Author : Nakazawa Yuna
// 
//=============================================================================
#ifndef _RANKINGSCORE_H_		// このマクロ定義がされていなかったら
#define _RANKINGSCORE_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
#define MAX_RANKING			(5)										// ランキングに表示するスコアの数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScore;

//*****************************************************************************
// ランキングスコアクラス
//*****************************************************************************
class CRankingScore : public CObject
{
public:
	CRankingScore();
	~CRankingScore();

	static CRankingScore* Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetPosition(void) { return DEFAULT_VECTER3; }
	D3DXVECTOR3 GetRotation(void) { return DEFAULT_VECTER3; }

	void SetScore(void);
	int GetUpdate(void);
	void SaveScore(const int nScore);
	void LoadScore(void);

	static int compare(const void* data1, const void* data2);

private:
	CScore* m_apScore[MAX_RANKING];					// ランキングに表示するスコア
	int m_aScore[MAX_RANKING + 1];					// 最新スコアを含めたスコア
	bool m_bSave;									// 一度だけ保存する
	int m_nUpdateScore;								// 更新されたスコア番号
};

#endif