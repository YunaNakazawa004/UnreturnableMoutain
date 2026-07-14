//========================================================================
// 
// エネルギー鉱物 [ energyrock.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "energyrock.h"

#include "renderer.h"
#include "manager.h"

#include "game.h"
#include "mountain.h"
#include "beach.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define FIRST_LIFE				(3)					// 取れる数

//========================================================================
// エネルギー鉱物クラスの生成処理
//========================================================================
CEnergyRock* CEnergyRock::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CEnergyRock* pEnergyRock = NULL;

	if (pEnergyRock == NULL)
	{// NULLチェック
		// エネルギー鉱物の生成
		pEnergyRock = new CEnergyRock;
	}

	if (pEnergyRock != NULL)
	{// NULLチェック
		// モデルファイル設定
		pEnergyRock->BindModel("data\\MODEL\\MAP_OBJECT\\energyrock.x");

		// 初期化処理
		if (FAILED(pEnergyRock->Init(pos, rot)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! エネルギー鉱物の初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// 種類を設定
		pEnergyRock->SetType(TYPE_ENERGYROCK);

		return pEnergyRock;
	}

	OutputDebugStringA("! ! ! エネルギー鉱物の生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// エネルギー鉱物クラスのコンストラクタ
//========================================================================
CEnergyRock::CEnergyRock(const int nPriority) :CObjectX(nPriority)
{
	// エネルギー鉱物クラスの値をクリア
	m_nLife = 0;
}

//========================================================================
// エネルギー鉱物クラスのデストラクタ
//========================================================================
CEnergyRock::~CEnergyRock()
{
}

//========================================================================
// エネルギー鉱物の初期化処理
//========================================================================
HRESULT CEnergyRock::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// 初期化処理
	CObjectX::Init(pos);

	// 位置/向きを設定
	CObjectX::SetPosition(pos);
	CObjectX::SetRotation(rot);

	m_nLife = FIRST_LIFE;

	return S_OK;
}

//========================================================================
// エネルギー鉱物の終了処理
//========================================================================
void CEnergyRock::Uninit(void)
{
	// 終了処理
	CObjectX::Uninit();
}

//========================================================================
// エネルギー鉱物の更新処理
//========================================================================
void CEnergyRock::Update(void)
{
	CMountain* pMountain = CGame::GetMountain();					// 山の取得
	CBeach* pBeach = CGame::GetBeach();								// 砂浜の取得
	D3DXVECTOR3 pos = GetPosition();

	float fHeightM = 0.0f;		// 山の地面の高さ
	D3DXVECTOR2 polygonIdxM = { -1.0f,-1.0f };		// ポリゴン番号

	float fHeightB = 0.0f;		// 砂浜の地面の高さ
	D3DXVECTOR2 polygonIdxB = { -1.0f,-1.0f };		// ポリゴン番号

	float fHeight = 0.0f;		// 地面の高さ

	// 山のポリゴン番号を取得
	polygonIdxM = pMountain->GetPolygonIdx(pos);

	// 山の地面の高さを取得
	fHeightM = pMountain->GetHeight(pos, polygonIdxM);
	
	// 砂浜のポリゴン番号を取得
	polygonIdxB = pBeach->GetPolygonIdx(pos);

	// 砂浜の地面の高さを取得
	fHeightB = pBeach->GetHeight(pos, polygonIdxB);

	// 最終的な高さ
	fHeight = (fHeightM >= fHeightB) ? fHeightM : fHeightB;

	if (fHeight == ERROR_HEIGHT)
	{// 無効な高さだったら
		fHeight = 0.0f;
	}

	// 高さを代入
	pos.y = fHeight;

	// 位置を適用
	SetPosition(pos);
}

//========================================================================
// エネルギー鉱物の描画処理
//========================================================================
void CEnergyRock::Draw(void)
{
	// 描画処理
	CObjectX::Draw();
}

//========================================================================
// 取れる数を減らす
//========================================================================
void CEnergyRock::Minus(const int nValue)
{
	m_nLife -= nValue;

	if (m_nLife <= 0)
	{// なくなった
		// 破棄
		Uninit();
	}
}

//========================================================================
// エネルギー鉱物との当たり判定
//========================================================================
CEnergyRock* CEnergyRock::Collision(D3DXVECTOR3* pPos, D3DXVECTOR3* posOld, D3DXVECTOR3* move,
	const float fRadius, const float fHeight)
{
	for (int nCntPri = 0; nCntPri < MAX_PRIORITY_NUM; nCntPri++)
	{
#ifdef LIST
		CObject* pObj = CObject::GetTop(nCntPri);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();			// 次のオブジェクトを保存
			CObject::TYPE type;

			// オブジェクトの種類を取得
			type = pObj->GetType();

			if (type == CObject::TYPE_ENERGYROCK)
			{// エネルギー鉱物オブジェクトなら当たり判定する
				D3DXVECTOR3 posRock, dist;

				// エネルギー鉱物の位置を取得
				posRock = pObj->GetPosition();

				// 距離を計算
				dist = *pPos - posRock;

				if ((D3DXVec3Length(&dist) < ENERGYROCK_RADIUS + fRadius) &&
					pPos->y < posRock.y + ENERGYROCK_HEIGHT && pPos->y + fHeight > posRock.y)
				{// エネルギー鉱物と重なった
					// 当たり判定
					dynamic_cast<CObjectX*>(pObj)->Collision(pPos, posOld, move, fRadius, fHeight, NULL);

					return dynamic_cast<CEnergyRock*>(pObj);
				}
			}

			pObj = pObjNext;			// 次のオブジェクトを代入
		}

#else
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			CObject* pObj;

			// オブジェクトを取得
			pObj = GetObject(nCntPri, nCntObj);

			if (pObj != NULL)
			{// NULLチェック
				CObject::TYPE type;

				// オブジェクトの種類を取得
				type = pObj->GetType();

				if (type == CObject::TYPE_ENERGYROCK)
				{// エネルギー鉱物オブジェクトなら当たり判定する
					D3DXVECTOR3 posRock, dist;

					// エネルギー鉱物の位置を取得
					posRock = pObj->GetPosition();

					// 距離を計算
					dist = *pPos - posRock;

					if ((D3DXVec3Length(&dist) < ENERGYROCK_RADIUS + fRadius) &&
						pPos->y < posRock.y + ENERGYROCK_HEIGHT && pPos->y + fHeight > posRock.y)
					{// エネルギー鉱物と重なった
						// 当たり判定
						dynamic_cast<CObjectX*>(pObj)->Collision(pPos, posOld, move, fRadius, fHeight);

						return dynamic_cast<CEnergyRock*>(pObj);
					}
				}
			}
		}

#endif
	}

	return NULL;
}