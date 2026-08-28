//========================================================================
// 
// アイテムUIヘッダー [ UI_item.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _UI_ITEM_H_		// このマクロ定義がされていなかったら
#define _UI_ITEM_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object2D.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define ITEMUI_WIDTH		(150.0f)				// アイテムUIの幅
#define ITEMUI_HEIGHT		(150.0f)				// アイテムUIの高さ

//************************************************************************
// アイテムUIクラス
//************************************************************************
class CItemUI : public CObject2D
{
public:
	// 種類
	enum TYPE
	{
		TYPE_TREE0 = 0,		// 木１
		TYPE_TREE1,			// 木２
		TYPE_ROCK0,			// 岩１
		TYPE_ROCK1,			// 岩２
		TYPE_FLOWER0,		// 花１
		TYPE_FLOWER1,		// 花２
		TYPE_LEAF0,			// 葉１
		TYPE_LEAF1,			// 葉２
		TYPE_MAX
	};

	// アイテムひとつずつの情報
	typedef struct
	{
		CObject2D* pItem;		// アイテムポリゴン
		CObject2D* pCheck;		// チェック
		int type;				// アイテムの種類
	}Item;

	CItemUI(const int nPriority = PRIORITY_6);
	~CItemUI();

	static HRESULT Load(void);
	static void Unload(void);
	static CItemUI* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);

	HRESULT Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int SetItem(const int Item);
	void Check(const int nIdx);
	void SetDispWOCheck(const bool bDisp);
	void SetDispAll(const bool bDisp);

private:
	static int m_aIdxTexture[TYPE_MAX + 1];		// テクスチャのインデックス
	Item m_Item[TYPE_MAX];			// アイテム情報
	static int m_nNumItem;			// アイテムの総数
};

#endif