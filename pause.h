//========================================================================
// 
// ポーズ画面ヘッダー [ pause.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _PAUSE_H_		// このマクロ定義がされていなかったら
#define _PAUSE_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"

//************************************************************************
// 前方宣言
//************************************************************************
class CObject2D;

//************************************************************************
// ポーズ画面クラス
//************************************************************************
class CPause
{
public:
	// ポーズメニュー
	typedef enum
	{
		MENU_CONTINUE = 0,			// ゲームに戻る
		MENU_RETRY,					// ゲームをやり直す
		MENU_QUIT,					// タイトル画面に戻る
		MENU_MAX
	}MENU;

	// 背景のフェード状態
	enum FADE
	{
		FADE_NONE = 0,				// フェードなし
		FADE_IN,					// フェードイン
		FADE_OUT,					// フェードアウト
		FADE_MAX
	};

	CPause();
	~CPause();

	static CPause* Create(const MENU menu);
	HRESULT Init(const MENU menu);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetMenu(const MENU menu) { m_menu = menu; m_nMenu = (int)menu; }
	MENU GetMenu(void) { return m_menu; }
	void SetDisp(const bool bDisp);
	void SetFade(const int fade);

private:
	CObject2D* m_apObject2D[MENU_MAX];				// ポーズメニューのポリゴン
	CObject2D* m_pBack;								// ポーズメニュー背景
	static const char* m_apFilename[MENU_MAX + 1];	// テクスチャのファイル名
	MENU m_menu;		// 現在のポーズ選択肢
	int m_nMenu;		// 現在の選択肢番号
	int m_fade;			// 背景のフェード状態
	float m_fLength;	// 既定の大きさ
};

#endif