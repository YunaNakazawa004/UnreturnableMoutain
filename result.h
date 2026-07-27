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
class CShip;
class CLab;

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
	static CShip* GetShip(void) { return m_pShip; }
	static CLab* GetLab(void) { return m_pLab; }
	
	static void SetClear(const bool bClear) { m_bClear = bClear; }

private:
	static CResultUI* m_pResultUI;					// リザルトUIのインスタンス
	static CShip* m_pShip;							// 船のインスタンス
	static CLab* m_pLab;							// 研究所のインスタンス

	static bool m_bClear;			// クリアフラグ
};

#endif