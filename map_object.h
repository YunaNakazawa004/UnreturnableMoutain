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
#define MAX_MAP_OBJECT			(1000)			// 持てるオブジェクトの最大数

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
		MAP_OBJ_MAX
	};

	CMapObject();
	virtual ~CMapObject();

	static void Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int mapObj);
	HRESULT Init(void);
	void Uninit(void);

	void ResetData(void);
	HRESULT WriteData(const char* pFilename);
	HRESULT ReadData(const char* pFilename);

private:
	static CObject* m_apObject[MAX_MAP_OBJECT];			// 配置したオブジェクトの情報
	static int m_aObjType[MAX_MAP_OBJECT];				// 配置したオブジェクトの種類
	static int m_nNumObject;							// 現在オブジェクトの総数
};

#endif