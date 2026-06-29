//========================================================================
// 
// 岩 [rock.cpp]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "rock.h"

#include "renderer.h"
#include "manager.h"

#include "game.h"
#include "meshfield.h"

//************************************************************************
// マクロ定義
//************************************************************************

//========================================================================
// 岩クラスの生成処理
//========================================================================
CRock* CRock::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CRock* pRock = NULL;

	if (pRock == NULL)
	{// NULLチェック
		// 岩の生成
		pRock = new CRock;
	}

	if (pRock != NULL)
	{// NULLチェック
		// モデルファイル設定
		pRock->BindModel("data\\MODEL\\rock.x");

		// 初期化処理
		if (FAILED(pRock->Init(pos, rot)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! 岩の初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// 種類を設定
		pRock->SetType(TYPE_ROCK);

		return pRock;
	}

	OutputDebugStringA("! ! ! 岩の生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// 岩クラスのコンストラクタ
//========================================================================
CRock::CRock(const int nPriority) :CObjectX(nPriority)
{
	// 岩クラスの値をクリア
}

//========================================================================
// 岩クラスのデストラクタ
//========================================================================
CRock::~CRock()
{
}

//========================================================================
// 岩の初期化処理
//========================================================================
HRESULT CRock::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// 初期化処理
	CObjectX::Init(pos);

	// 位置/向きを設定
	CObjectX::SetPosition(pos);
	CObjectX::SetRotation(rot);

	return S_OK;
}

//========================================================================
// 岩の終了処理
//========================================================================
void CRock::Uninit(void)
{
	// 終了処理
	CObjectX::Uninit();
}

//========================================================================
// 岩の更新処理
//========================================================================
void CRock::Update(void)
{
	CMeshField* pMeshField = CGame::GetMeshField();					// メッシュフィールドの取得
	D3DXVECTOR3 pos = GetPosition();

	float fHeight = 0.0f;		// 地面の高さ
	D3DXVECTOR2 polygonIdx = { -1.0f,-1.0f };		// ポリゴン番号

	// ポリゴン番号を取得
	polygonIdx = pMeshField->GetPolygonIdx(pos);

	// 地面の高さを取得
	fHeight = pMeshField->GetHeight(pos, polygonIdx);

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
// 岩の描画処理
//========================================================================
void CRock::Draw(void)
{
	// 描画処理
	CObjectX::Draw();
}

//========================================================================
// 岩との当たり判定
//========================================================================
CRock* CRock::Collision(D3DXVECTOR3* pPos, D3DXVECTOR3* posOld, D3DXVECTOR3* move,
	const float fRadius, const float fHeight, bool* pLand)
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

			if (type == CObject::TYPE_ROCK)
			{// 岩オブジェクトなら当たり判定する
				D3DXVECTOR3 posRock, dist;

				// 岩の位置を取得
				posRock = pObj->GetPosition();

				// 距離を計算
				dist = *pPos - posRock;

				if ((D3DXVec3Length(&dist) < ROCK_RADIUS + fRadius) &&
					pPos->y < posRock.y + ROCK_HEIGHT && pPos->y + fHeight > posRock.y)
				{// 岩と重なった
					// 当たり判定
					if (dynamic_cast<CObjectX*>(pObj)->Collision(pPos, posOld, move, fRadius, fHeight) == true)
					{// 乗っている
						*pLand = true;
					}

					return dynamic_cast<CRock*>(pObj);
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

				if (type == CObject::TYPE_Rock)
				{// 岩オブジェクトなら当たり判定する
					D3DXVECTOR3 posRock, dist;

					// 岩の位置を取得
					posRock = pObj->GetPosition();

					// 距離を計算
					dist = *pPos - posRock;

					if ((D3DXVec3Length(&dist) < Rock_RADIUS + fRadius) &&
						pPos->y < posRock.y + Rock_HEIGHT && pPos->y + fHeight > posRock.y)
					{// 岩と重なった
						// 当たり判定
						dynamic_cast<CObjectX*>(pObj)->Collision(pPos, posOld, move, fRadius, fHeight);

						return dynamic_cast<CRock*>(pObj);
					}
				}
			}
		}

#endif
	}

	return NULL;
}