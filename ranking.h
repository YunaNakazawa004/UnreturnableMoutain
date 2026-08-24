//=============================================================================
// 
// ランキング画面ヘッダー [ ranking.h ]
// Author : Nakazawa Yuna
// 
//=============================================================================
#ifndef _RANKING_H_		// このマクロ定義がされていなかったら
#define _RANKING_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CShip;
class CLab;
class CUsedEnergy;
class CScore;
class CRankingScore;

//*****************************************************************************
// ランキングクラス
//*****************************************************************************
class CRanking : public CScene
{
public:
	CRanking();
	~CRanking();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CShip* GetShip(void) { return m_pShip; }
	static CLab* GetLab(void) { return m_pLab; }
	static CRankingScore* GetRankingScore(void) { return m_pRankingScore; }

	static void SetUserScore(const int nScore) { m_nUserScore = nScore; }

private:
	static CShip* m_pShip;							// 船のインスタンス
	static CLab* m_pLab;							// 研究所のインスタンス
	static CRankingScore* m_pRankingScore;			// ランキングスコアのインスタンス

	static int m_nUserScore;						// 現在プレイヤーのスコア
	static int m_nModeCounter;						// 自動画面遷移のカウント
};

#endif