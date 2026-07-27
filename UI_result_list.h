//========================================================================
// 
// リザルト内訳UIヘッダー [ UI_result_list.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _UI_RESULT_LIST_H_		// このマクロ定義がされていなかったら
#define _UI_RESULT_LIST_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define LISTUI_WIDTH		(24.0f)			// リザルト内訳UIの幅
#define LISTUI_HEIGHT		(48.0f)			// リザルト内訳UIの高さ

//************************************************************************
// 前方宣言
//************************************************************************
class CObject2D;

//************************************************************************
// リザルト内訳UIクラス
//************************************************************************
class CListUI : public CObject
{
public:
	// 種類
	typedef enum
	{
		TYPE_BASE = 0,			// 基礎Pt
		TYPE_ENERGY,			// 使用エネルギー量
		TYPE_COLLECT,			// 採集したもの
		TYPE_CLEAR,				// クリアボーナス
		TYPE_FINAL,				// 最終Pt
		TYPE_MAX
	}TYPE;

	CListUI(const int nPriority = PRIORITY_6);
	~CListUI();

	static CListUI* Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetPosition(void) { return DEFAULT_VECTER3; }
	D3DXVECTOR3 GetRotation(void) { return DEFAULT_VECTER3; }

	void SetDisp(const int nIdx, const bool bDisp);
	void SetDispAll(const bool bDisp);

	typedef struct
	{
		D3DXVECTOR3 pos;		// 位置
		TYPE type;				// 種類
		float fWidth;			// 幅
		float fHeight;			// 高さ
		const char* pFilename;	// テクスチャのファイル名
	}List_Info;

private:
	CObject2D* m_apList[TYPE_MAX];				// 内訳のインスタンス
	static List_Info m_ListInfo[TYPE_MAX];		// 内訳の情報
};

#endif