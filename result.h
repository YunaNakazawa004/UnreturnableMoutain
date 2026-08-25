//========================================================================
// 
// リザルト画面ヘッダー [ result.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _RESULT_H_		// このマクロ定義がされていなかったら
#define _RESULT_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "scene.h"

//************************************************************************
// 前方宣言
//************************************************************************
class CResultUI;
class CListUI;
class CShip;
class CLab;
class CUsedEnergy;
class CScore;
class CCollectNum;

//************************************************************************
// リザルト画面クラス
//************************************************************************
class CResult : public CScene
{
public:
	CResult();
	~CResult();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CResultUI* GetResultUI(void) { return m_pResultUI; }
	static CListUI* GetListUI(void) { return m_pListUI; }
	static CShip* GetShip(void) { return m_pShip; }
	static CLab* GetLab(void) { return m_pLab; }
	static CUsedEnergy* GetUsedEnergy(void) { return m_pUsedEnergy; }
	static CScore* GetBaseScore(void) { return m_pBaseScore; }
	static CCollectNum* GetCollectNum(void) { return m_pCollectNum; }
	static CScore* GetCollectScore(void) { return m_pCollectScore; }
	static CScore* GetClearScore(void) { return m_pClearScore; }
	static CScore* GetFinalScore(void) { return m_pFinalScore; }
	
	static void SetClear(const bool bClear) { m_bClear = bClear; }
	static bool GetClear(void) { return m_bClear; }

private:
	static CResultUI* m_pResultUI;					// リザルトUIのインスタンス
	static CListUI* m_pListUI;						// リザルト内訳UIのインスタンス
	static CShip* m_pShip;							// 船のインスタンス
	static CLab* m_pLab;							// 研究所のインスタンス
	static CUsedEnergy* m_pUsedEnergy;				// 使用エネルギー量のインスタンス
	static CScore* m_pBaseScore;					// 基礎スコアのインスタンス
	static CCollectNum* m_pCollectNum;				// 収集数のインスタンス
	static CScore* m_pCollectScore;					// 収集スコアのインスタンス
	static CScore* m_pClearScore;					// クリアボーナスのインスタンス
	static CScore* m_pFinalScore;					// 最終スコアのインスタンス

	static bool m_bClear;			// クリアフラグ
};

#endif