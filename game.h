//========================================================================
// 
// ゲーム画面ヘッダー [ game.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _GAME_H_		// このマクロ定義がされていなかったら
#define _GAME_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "scene.h"

//************************************************************************
// 前方宣言
//************************************************************************
class CPause;
class CPlayer;
class CShip;
class CMountain;
class CBeach;
class CWaterSurface;
class CMapObject;
class CEnergyUI;
class CJumpMeterUI;
class CItemUI;

//************************************************************************
// ゲーム画面クラス
//************************************************************************
class CGame : public CScene
{
public:
	CGame();
	~CGame();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPause* GetPause(void) { return m_pPause; }
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CShip* GetShip(void) { return m_pShip; }
	static CMountain* GetMountain(void) { return m_pMountain; }
	static CBeach* GetBeach(void) { return m_pBeach; }
	static CWaterSurface* GetWaterSurface(void) { return m_pWaterSurface; }
	static CMapObject* GetMapObject(void) { return m_pMapObject; }
	static CEnergyUI* GetEnergyUI(void) { return m_pEnergyUI; }
	static CJumpMeterUI* GetJumpMeterUI(void) { return m_pJumpMeterUI; }
	static CItemUI* GetItemUI(void) { return m_pItemUI; }

	static void SetFadeEnable(void) { m_bFade = true; }

private:
	static CPause* m_pPause;						// ポーズのインスタンス
	static CPlayer* m_pPlayer;						// プレイヤーのインスタンス
	static CShip* m_pShip;							// 船のインスタンス
	static CMountain* m_pMountain;					// 山のインスタンス
	static CBeach* m_pBeach;						// 砂浜のインスタンス
	static CWaterSurface* m_pWaterSurface;			// 海のインスタンス
	static CMapObject* m_pMapObject;				// マップオブジェクトのインスタンス
	static CEnergyUI* m_pEnergyUI;					// エネルギーUIのインスタンス
	static CJumpMeterUI* m_pJumpMeterUI;			// ジャンプメーターUIのインスタンス
	static CItemUI* m_pItemUI;						// アイテムUIのインスタンス

	static bool m_bFade;		// 遷移フラグ
};

#endif