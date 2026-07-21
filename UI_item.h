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
#define ITEMUI_NUM			(8)						// UIの数

//************************************************************************
// アイテムUIクラス
//************************************************************************
class CItemUI : public CObject2D
{
public:
	// 種類
	enum TYPE
	{
		TYPE_GRASS = 0,		// 草
		TYPE_TREE,			// 木
		TYPE_ROCK,			// 岩
		TYPE_FLOWER,		// 花
		TYPE_MAX
	};

	// アイテムひとつずつの情報
	typedef struct
	{
		CObject2D* pItem;		// アイテムポリゴン
		CObject2D* pCheck;		// チェック
		int type;				// アイテムの種類
	}Item;

	CItemUI(const int nPriority = UI_PRIORITY);
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

private:
	static int m_aIdxTexture[TYPE_MAX + 1];		// テクスチャのインデックス
	Item m_Item[ITEMUI_NUM];		// アイテム情報
	static int m_nNumItem;			// アイテムの総数
};

#endif