//========================================================================
// 
// 花ヘッダー [ flower.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _FLOWER_H_		// このマクロ定義がされていなかったら
#define _FLOWER_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "objectX.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define FLOWER_RADIUS		(40.0f)			// 花の半径
#define FLOWER_HEIGHT		(40.0f)			// 花の高さ
#define FLOWER_NUM			(3)				// 花の種類
#define COLLECT_NUM			(3)				// 収集アイテムの種類

//************************************************************************
// 花クラス
//************************************************************************
class CFlower : public CObjectX
{
public:
	// 種類
	enum TYPE
	{
		TYPE_NORMAL = 0,		// 通常
		TYPE_C_0,				// 収集アイテムの花
		TYPE_C_1,				// 収集アイテムの花
		TYPE_CL_0,				// 収集アイテムの葉
		TYPE_MAX
	};

	CFlower(const int nPriority = PRIORITY_3);
	virtual ~CFlower();

	static CFlower* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int type = TYPE_NORMAL);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void CollisionPlayer(void);

private:
	static const char* m_aFilename[FLOWER_NUM + COLLECT_NUM];			// 花のファイル名
	D3DXVECTOR3 m_rotOff;				// 向きオフセット
	float m_fShake;						// 揺れ具合;
};

#endif