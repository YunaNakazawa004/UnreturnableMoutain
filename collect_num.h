//========================================================================
// 
// 収集数ヘッダー [ collect_num.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _COLLECT_NUM_H_		// このマクロ定義がされていなかったら
#define _COLLECT_NUM_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "object.h"

//************************************************************************
// 前方宣言
//************************************************************************
class CNumber;

//************************************************************************
// マクロ定義
//************************************************************************
#define COLLECT_NUM_NUMPLACE		(1)				// 収集数の桁数
#define COLLECT_NUM_WIDTH			(36.0f)			// 収集数の幅
#define COLLECT_NUM_HEIGHT			(72.0f)			// 収集数の高さ

//************************************************************************
// 収集数クラス
//************************************************************************
class CCollectNum : public CObject
{
public:
	CCollectNum(const int nPriority = PRIORITY_6);
	~CCollectNum();

	static CCollectNum* Create(const D3DXVECTOR3 pos,
		const float fWidth, const float fHeight);
	HRESULT Init(void) { return S_OK; }
	HRESULT Init(const D3DXVECTOR3 pos,
		const float fWidth, const float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRotation(void) { return DEFAULT_VECTER3; }
	static void SetCollectNum(const int nCollectNum) { m_nCollectNum = nCollectNum; }
	static int GetCollectNum(void) { return m_nCollectNum; }
	void SetNum(const int nCollectNum);
	void Add(const int nAdd);
	void Minus(const int nMinus);
	void SetDisp(const bool bDisp) { m_bDisp = bDisp; }
	void SwapDisp(void) { m_bDisp = m_bDisp ? false : true; }

private:
	CNumber* m_apNumber[COLLECT_NUM_NUMPLACE];		// 桁数分の数字のインスタンス
	D3DXVECTOR3 m_pos;			// 基準の位置
	static int m_nCollectNum;	// 収集数
	bool m_bDisp;				// 表示するかどうか
};

#endif