//========================================================================
// 
// オブジェクトヘッダー [object.h]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _OBJECT_H_		// このマクロ定義がされていなかったら
#define _OBJECT_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define MAX_OBJECT			(1024)			// 最大オブジェクト数
#define MAX_PRIORITY_NUM	(8)				// 最大優先順位数
#define BG_PRIORITY			(0)				// デフォルト背景優先順位
#define BULLET_PRIORITY		(1)				// デフォルト弾優先順位
#define OBJECT_PRIORITY		(2)				// デフォルトオブジェクト優先順位
#define PLAYER_PRIORITY		(3)				// デフォルトプレイヤー優先順位
#define EFFECT_PRIORITY		(4)				// デフォルトエフェクト優先順位
#define FRAME_PRIORITY		(5)				// デフォルトUIフレーム優先順位
#define UI_PRIORITY			(6)				// デフォルトUI優先順位
#define FADE_PRIORITY		(7)				// デフォルトフェード優先順位

#define COLOR_RED			(D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))
#define COLOR_GREEN			(D3DXCOLOR(0.0f,0.5f,0.0f,1.0f))
#define COLOR_BLUE			(D3DXCOLOR(0.0f,0.0f,1.0f,1.0f))
#define COLOR_YELLOW		(D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))
#define COLOR_WHITE			(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define COLOR_BLACK			(D3DXCOLOR(0.0f,0.0f,0.0f,0.0f))
#define COLOR_DARKGRAY		(D3DXCOLOR(0.3f,0.3f,0.3f,1.0f))
#define COLOR_BLUEGRAY		(D3DXCOLOR(0.439f,0.501f,0.564f,1.0f))
#define COLOR_CYAN			(D3DXCOLOR(0.0f,1.0f,1.0f,1.0f))
#define COLOR_MAGENTA		(D3DXCOLOR(1.0f,0.0f,1.0f,1.0f))
#define COLOR_VIOLET		(D3DXCOLOR(0.933f,0.509f,0.933f,1.0f))
#define COLOR_ORANGE		(D3DXCOLOR(1.0f,0.647f,0.0f,1.0f))
#define COLOR_LIMEGREEN		(D3DXCOLOR(0.0f,1.0f,0.0f,1.0f))
#define COLOR_SKYBLUE		(D3DXCOLOR(0.529f,0.807f,0.921f,1.0f))
#define COLOR_HISUI			(D3DXCOLOR(0.219f,0.705f,0.545f,1.0f))
#define COLOR_RETROFILTER	(D3DXCOLOR(1.000f,	1.000f,	1.000f,	0.080f))
#define COLOR_DISALPHA		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.35f))
#define COLOR_GREENBATTERY	(D3DXCOLOR(0.027f, 1.000f, 0.525f, 1.0f))
#define COLOR_YELLOWBATTERY	(D3DXCOLOR(1.000f, 0.615f, 0.011f, 1.00f))
#define COLOR_REDBATTERY	(D3DXCOLOR(0.85f, 0.00f, 0.00f, 1.0f))
#define COLOR_UIBUBBLE		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f))
#define COLOR_WHITE_ALPHA	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f))

#if 1
#define LIST				// リスト構造にするかどうか
#endif

//************************************************************************
// オブジェクトクラス
//************************************************************************
class CObject
{
public:
	// オブジェクトの種類
	typedef enum
	{
		TYPE_NONE = 0,			// 種類なし
		TYPE_EFFECT2D,			// エフェクト2D
		TYPE_EFFECT3D,			// エフェクト3D
		TYPE_PARTICLE2D,		// パーティクル2D
		TYPE_PARTICLE3D,		// パーティクル3D
		TYPE_OBJECT3D,			// オブジェクト3D
		TYPE_OBJECTX,			// オブジェクトX
		TYPE_OBJECTBILLBOARD,	// オブジェクトビルボード
		TYPE_MESHFIELD,			// メッシュフィールド
		TYPE_PLAYER,			// プレイヤー
		TYPE_ENERGYROCK,		// エネルギー鉱物
		TYPE_GRASS,				// 草
		TYPE_TREE,				// 木
		TYPE_MAX
	}TYPE;

	CObject();
	CObject(const int nPriority = 3);
	virtual ~CObject();

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	virtual D3DXVECTOR3 GetPosition(void) = 0;
	virtual D3DXVECTOR3 GetRotation(void) = 0;
	
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);

	static int GetNumAll(void) { return m_nNumAll; }
	void SetType(const TYPE type) { m_type = type; }
	TYPE GetType(void) { return m_type; }

#ifdef LIST
	static CObject* GetTop(const int nPriority) { return m_apTop[nPriority]; }
	CObject* GetNext(void) { return m_pNext; }

#else
	static CObject* GetObject(const int nPriority, const int nIdx) { return m_apObject[nPriority][nIdx]; }

#endif

protected:
	void Release(void);

private:
#ifdef LIST
	static CObject* m_apTop[MAX_PRIORITY_NUM];			// 先頭のオブジェクトへのポインタ
	static CObject* m_apCur[MAX_PRIORITY_NUM];			// 最後尾のオブジェクトへのポインタ
	CObject* m_pPrev;				// 前のオブジェクトへのポインタ
	CObject* m_pNext;				// 次のオブジェクトへのポインタ

#else
	static CObject* m_apObject[MAX_PRIORITY_NUM][MAX_OBJECT];	// オブジェクトのインスタンス
	int m_nID;						// 自分自身のID

#endif
	int m_nPriority;				// 自分自身の優先順位
	static int m_nNumAll;			// オブジェクトの総数
	TYPE m_type;					// 自分の種類
};

#endif