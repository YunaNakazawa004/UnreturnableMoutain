//========================================================================
// 
// シーンヘッダー [ scene.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _SCENE_H_		// このマクロ定義がされていなかったら
#define _SCENE_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define ENABLE_INHERITANCE_COBJECT				// シーンクラスがCObjectを継承するかどうか
#undef ENABLE_INHERITANCE_COBJECT

//************************************************************************
// シーンクラス
//************************************************************************
#ifdef ENABLE_INHERITANCE_COBJECT
class CScene : public CObject

#else
class CScene

#endif
{
public:
	typedef enum
	{
		MODE_TITLE = 0,		// タイトル
		MODE_GAME,			// ゲーム
		MODE_RESULT,		// リザルト
		MODE_MAX
	}MODE;

	CScene(const MODE mode);
	~CScene();

	static CScene* Create(const MODE mode);
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	MODE GetMode(void) { return m_mode; }

#ifdef ENABLE_INHERITANCE_COBJECT
	D3DXVECTOR3 GetPosition(void) { return DEFAULT_VECTER3; }
	D3DXVECTOR3 GetRotation(void) { return DEFAULT_VECTER3; }
#endif

private:
	MODE m_mode;			// シーンのモード
};

#endif