//========================================================================
// 
// ゲーム画面ヘッダー [game.h]
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
	static CMeshField* GetMeshField(void) { return m_pMeshField; }
	static CMapObject* GetMapObject(void) { return m_pMapObject; }

private:
	static CPause* m_pPause;						// ポーズのインスタンス
	static CPlayer* m_pPlayer;						// プレイヤーのインスタンス
	static CMeshField* m_pMeshField;				// メッシュフィールドのインスタンス
	static CMapObject* m_pMapObject;				// マップオブジェクトのインスタンス
};

#endif