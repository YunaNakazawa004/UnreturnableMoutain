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

private:
	static CTitleLogo* m_pTitleLogo;				// タイトルロゴのインスタンス
	static CEnterUI* m_pEnterUI;					// エンターUIのインスタンス
};

#endif