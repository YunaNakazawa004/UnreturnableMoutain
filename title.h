//========================================================================
// 
// タイトル画面ヘッダー [ title.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _TITLE_H_		// このマクロ定義がされていなかったら
#define _TITLE_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "scene.h"

//************************************************************************
// 前方宣言
//************************************************************************
class CTitleLogo;
class CEnterUI;
class CPlayer;
class CShip;
class CLab;
class CEnergyUI;
class CJumpMeterUI;
class CActionUI;

//************************************************************************
// タイトル画面クラス
//************************************************************************
class CTitle : public CScene
{
public:
	CTitle();
	~CTitle();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTitleLogo* GetTitleLogo(void) { return m_pTitleLogo; }
	static CEnterUI* GetEnterUI(void) { return m_pEnterUI; }
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CShip* GetShip(void) { return m_pShip; }
	static CLab* GetLab(void) { return m_pLab; }
	static CEnergyUI* GetEnergyUI(void) { return m_pEnergyUI; }
	static CJumpMeterUI* GetJumpMeterUI(void) { return m_pJumpMeterUI; }
	static CActionUI* GetActionUI(void) { return m_pActionUI; }

private:
	static CTitleLogo* m_pTitleLogo;				// タイトルロゴのインスタンス
	static CEnterUI* m_pEnterUI;					// エンターUIのインスタンス
	static CPlayer* m_pPlayer;						// プレイヤーのインスタンス
	static CShip* m_pShip;							// 船のインスタンス
	static CLab* m_pLab;							// 研究所のインスタンス
	static CEnergyUI* m_pEnergyUI;					// エネルギーUIのインスタンス
	static CJumpMeterUI* m_pJumpMeterUI;			// ジャンプメーターUIのインスタンス
	static CActionUI* m_pActionUI;					// アクションUIのインスタンス

	static bool m_bTutorial;		// チュートリアル中かどうかのフラグ
};

#endif