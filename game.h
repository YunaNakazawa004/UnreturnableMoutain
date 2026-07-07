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
class CMeshField;
class CMapObject;

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
	static CMeshField* GetMeshField(void) { return m_pMeshField; }
	static CMapObject* GetMapObject(void) { return m_pMapObject; }

	static void SetFadeEnable(void) { m_bFade = true; }

private:
	static CPause* m_pPause;						// ポーズのインスタンス
	static CPlayer* m_pPlayer;						// プレイヤーのインスタンス
	static CShip* m_pShip;							// 船のインスタンス
	static CMeshField* m_pMeshField;				// メッシュフィールドのインスタンス
	static CMapObject* m_pMapObject;				// マップオブジェクトのインスタンス

	static bool m_bFade;		// 遷移フラグ
};

#endif