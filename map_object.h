//========================================================================
// 
// マップオブジェクトヘッダー [map_object.h]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _MAP_OBJECT_H_		// このマクロ定義がされていなかったら
#define _MAP_OBJECT_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define MAX_MAP_OBJECT			(2000)			// 持てるオブジェクトの最大数

//************************************************************************
// 前方宣言
//************************************************************************
class CObject;

//************************************************************************
// マップオブジェクトクラス
//************************************************************************
class CMapObject
{
public:
	// 生成可能な配置オブジェクト
	enum MAP_OBJ
	{
		MAP_OBJ_GRASS = 0,		// 草
		MAP_OBJ_TREE,			// 木
		MAP_OBJ_ROCK,			// 岩
		MAP_OBJ_FLOWER,			// 花
		MAP_OBJ_ENERGYROCK,		// エネルギー鉱石
		MAP_OBJ_MAX
	};

	// オブジェクトの保存情報
	typedef struct
	{
		CObject* m_apObject;		// 配置したオブジェクトの情報
		int m_aObjType;				// 配置したオブジェクトの種類
		bool m_bCollect;			// 配置したオブジェクトが収集アイテムかどうか
	}Map_Obj;

	CMapObject();
	virtual ~CMapObject();

	static void Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int mapObj, const bool bCollect);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	bool CollectCollision(const D3DXVECTOR3 pos, const int nIdx);

	int GetCollectObj(void) { return m_nNumCollectObj; }
	void SkipCollectObj(void) { m_nNumCollectObj = 0; }

	void ResetData(void);
	HRESULT WriteData(const char* pFilename);
	HRESULT ReadData(const char* pFilename);

private:
	static Map_Obj m_aMapObject[MAX_MAP_OBJECT];		// オブジェクトの保存情報
	static int m_nNumObject;							// 現在オブジェクトの総数
	static int m_nNumCollectObj;						// 収集アイテムの総数
};

#endif