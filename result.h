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
class CObject2D;
class CResultUI;

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

	static CObject2D* GetObject2D(void) { return m_pObject2D; }
	static CResultUI* GetResultUI(void) { return m_pResultUI; }
	
	static void SetClear(const bool bClear) { m_bClear = bClear; }

private:
	static CObject2D* m_pObject2D;						// 背景のインスタンス
	static CResultUI* m_pResultUI;						// リザルトUIのインスタンス

	static bool m_bClear;			// クリアフラグ
};

#endif