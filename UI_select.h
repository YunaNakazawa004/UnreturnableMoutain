//========================================================================
// 
// 選択肢ヘッダー [ UI_select.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _UI_SELECT_H_		// このマクロ定義がされていなかったら
#define _UI_SELECT_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object.h"

//************************************************************************
// 前方宣言
//************************************************************************
class CObject2D;

//************************************************************************
// 選択肢クラス
//************************************************************************
class CSelect : public CObject
{
public:
	// ポーズメニュー
	typedef enum
	{
		MENU_START = 0,			// ゲームスタート
		MENU_TUTORIAL,			// チュートリアル
		MENU_EXIT,				// ゲームをやめる
		MENU_MAX
	}MENU;

	CSelect(const int nPriority = PRIORITY_6);
	~CSelect();

	static CSelect* Create(const MENU menu);
	HRESULT Init(void) { return S_OK; }
	HRESULT Init(const MENU menu);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetMenu(const MENU menu) { m_menu = menu; m_nMenu = (int)menu; }
	MENU GetMenu(void) { return m_menu; }
	void SetDisp(const bool bDisp);
	D3DXVECTOR3 GetPosition(void) { return DEFAULT_VECTER3; }
	D3DXVECTOR3 GetRotation(void) { return DEFAULT_VECTER3; }

private:
	CObject2D* m_apObject2D[MENU_MAX];				// メニューのポリゴン
	static const char* m_apFilename[MENU_MAX];		// テクスチャのファイル名
	MENU m_menu;		// 現在の選択肢
	int m_nMenu;		// 現在の選択肢番号
	float m_fLength;	// 既定の大きさ
	bool m_bSelect;		// 選択できるか
};

#endif