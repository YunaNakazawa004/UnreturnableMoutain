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
class CFrame;
class CEnergyUI;
class CJumpMeterUI;
class CActionUI;
class CTutorialJump;
class CTutorialTxt;

//************************************************************************
// マクロ定義
//************************************************************************
#define NUM_TUTORIALTXT		(4)		// チュートリアル文の数

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
	static CFrame* GetFrame(void) { return m_pFrame; }
	static CEnergyUI* GetEnergyUI(void) { return m_pEnergyUI; }
	static CJumpMeterUI* GetJumpMeterUI(void) { return m_pJumpMeterUI; }
	static CActionUI* GetActionUI(void) { return m_pActionUI; }
	static CTutorialTxt* GetTutorialTxt(const int nIdx) { return m_apTutorialTxt[nIdx]; }

	static bool isTutorial(void) { return m_bTutorial; }

private:
	static CTitleLogo* m_pTitleLogo;				// タイトルロゴのインスタンス
	static CEnterUI* m_pEnterUI;					// エンターUIのインスタンス
	static CPlayer* m_pPlayer;						// プレイヤーのインスタンス
	static CShip* m_pShip;							// 船のインスタンス
	static CLab* m_pLab;							// 研究所のインスタンス
	static CFrame* m_pFrame;						// 枠のインスタンス
	static CEnergyUI* m_pEnergyUI;					// エネルギーUIのインスタンス
	static CJumpMeterUI* m_pJumpMeterUI;			// ジャンプメーターUIのインスタンス
	static CActionUI* m_pActionUI;					// アクションUIのインスタンス
	static CTutorialTxt* m_apTutorialTxt[NUM_TUTORIALTXT];			// チュートリアル文のインスタンス

	static bool m_bTutorial;		// チュートリアル中かどうかのフラグ
	static bool m_bReady;			// チュートリアル完了フラグ
};

#endif