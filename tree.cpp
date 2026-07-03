//========================================================================
// 
// 木 [ tree.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "tree.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "texture.h"

#include "camera.h"
#include "model.h"
#include "game.h"

#include "meshfield.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define TREE_RADIUS			(30.0f)			// 木の幹の半径
#define SHAKE_MOVE			(3.0f)			// 揺れの大きさ
#define SHAKE_INERTIA		(0.3f)			// 揺れの慣性

//========================================================================
// 木クラスの生成処理
//========================================================================
CTree* CTree::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CTree* pTree = NULL;

	if (pTree == NULL)
	{// NULLチェック
		// 木の生成
		pTree = new CTree;
	}

	if (pTree != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pTree->Init(pos, rot)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! 木の初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// 種類を設定
		pTree->SetType(TYPE_TREE);

		return pTree;
	}

	OutputDebugStringA("! ! ! 木の生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// 木クラスのコンストラクタ
//========================================================================
CTree::CTree(const int nPriority) :CObject(nPriority)
{
	// 木クラスの値をクリア
	memset(&m_apModel[0], NULL, sizeof m_apModel);
	m_nNumModel = 0;
	m_pos = DEFAULT_VECTER3;
	m_move = DEFAULT_VECTER3;
	m_rot = DEFAULT_VECTER3;
	m_scale = DEFAULT_VECTER3;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_bShake = true;
	m_bDisp = true;
}

//========================================================================
// 木クラスのデストラクタ
//========================================================================
CTree::~CTree()
{
}

//========================================================================
// 木クラスの初期化処理
//========================================================================
HRESULT CTree::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// 値を初期化
	m_pos = pos;
	m_rot = rot;
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// モデルを設定
	m_apModel[0] = CModel::Create(DEFAULT_VECTER3, m_rot, "data\\MODEL\\MAP_OBJECT\\TREE\\tree.x");
	m_apModel[0]->SetParent(NULL);
	m_apModel[1] = CModel::Create(D3DXVECTOR3(0.0f, 150.0f, 0.0f), m_rot, "data\\MODEL\\MAP_OBJECT\\TREE\\treeleaf.x");
	m_apModel[1]->SetParent(m_apModel[0]);

	// モデル数を設定
	m_nNumModel = 2;

	return S_OK;
}

//========================================================================
// 木クラスの終了処理
//========================================================================
void CTree::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULLチェック
			// 終了処理
			m_apModel[nCntModel]->Uninit();

			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = NULL;
		}
	}

	// 自分自身を破棄
	CObject::Release();
}

//========================================================================
// 木クラスの更新処理
//========================================================================
void CTree::Update(void)
{
	CMeshField* pMeshField = CGame::GetMeshField();					// メッシュフィールドの取得
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posM = m_apModel[0]->GetPosition();

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

	// モデルの位置を戻す
	posM.x = (0.0f - posM.x) * SHAKE_INERTIA;
	posM.z = (0.0f - posM.z) * SHAKE_INERTIA;

	// モデルの位置を適用
	m_apModel[0]->SetPosition(posM);
}

//========================================================================
// 木クラスの描画処理
//========================================================================
void CTree::Draw(void)
{
	if (m_bDisp == false)
	{// 表示しない場合
		return;
	}

	// ローカル変数宣言
	CRenderer* pRenderer = CManager::GetRenderer();			// レンダラーへのポインタ
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスへのポインタ
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;					// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// スケールを反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの描画（全パーツ分）
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULLチェック
			// 描画処理
			m_apModel[nCntModel]->Draw();
		}
	}
}

//========================================================================
// 位置設定
//========================================================================
void CTree::SetPosition(const D3DXVECTOR3 pos)
{
	// 位置を変更
	m_pos = pos;
}

//========================================================================
// スケール設定
//========================================================================
void CTree::SetScale(const D3DXVECTOR3 scale)
{
	// スケールを変更
	m_scale = scale;
}

//========================================================================
// 角度設定
//========================================================================
void CTree::SetRotation(const D3DXVECTOR3 rot)
{
	// 向きを変更
	m_rot = rot;

	// X向きを調整
	CorrectAngle(&m_rot.x, m_rot.x);

	// Y向きを調整
	CorrectAngle(&m_rot.y, m_rot.y);

	// Z向きを調整
	CorrectAngle(&m_rot.z, m_rot.z);
}

//========================================================================
// 頂点カラー設定
//========================================================================
void CTree::SetColor(const D3DXCOLOR col)
{
	// 色を変更
	m_col = col;

	// モデルの色を変更（全パーツ分）
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// NULLチェック
			// 各モデルの色を変更
			m_apModel[nCntModel]->SetColor(col);
		}
	}
}

//========================================================================
// 木との当たり判定
//========================================================================
bool CTree::Collision(D3DXVECTOR3* pPos, D3DXVECTOR3* posOld, D3DXVECTOR3* move,
	const float fRadius, const float fHeight)
{
	// ローカル変数
	bool bColl = false;

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

			if (type == CObject::TYPE_TREE)
			{// 木オブジェクトなら当たり判定する
				D3DXVECTOR3 posObj, dist;

				// 木の位置を取得
				posObj = pObj->GetPosition();

				// 距離を計算
				dist = *pPos - posObj;

				// サイズを代入
				D3DXVECTOR3 Radius = dynamic_cast<CTree*>(pObj)->m_apModel[0]->GetVtxMax();

				if ((D3DXVec3Length(&dist) < Radius.x + fRadius) &&
					pPos->y < posObj.y + Radius.y && pPos->y + fHeight > posObj.y)
				{// 木と重なった
					// 当たり判定
					bColl = dynamic_cast<CTree*>(pObj)->m_apModel[0]->Collision(posObj, pPos, posOld, move, fRadius, fHeight);

					if (dynamic_cast<CTree*>(pObj)->GetShake() == true && bColl == true)
					{// 一度揺らす
						// 揺らす
						dynamic_cast<CTree*>(pObj)->Shake(*pPos);

						// 揺れるフラグを消す
						dynamic_cast<CTree*>(pObj)->SetShake(false);
					}

					return true;
				}
				else
				{// 遠かった
					// 揺れるようにする
					dynamic_cast<CTree*>(pObj)->SetShake(true);
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

				if (type == CObject::TYPE_TREE)
				{// 木オブジェクトなら当たり判定する
					D3DXVECTOR3 posObj, dist;

					// 木の位置を取得
					posObj = pObj->GetPosition();

					// 距離を計算
					dist = *pPos - posObj;
					
					// サイズを代入
					D3DXVECTOR3 Radius = dynamic_cast<CTree*>(pObj)->m_apModel[0]->GetVtxMax();

					if ((D3DXVec3Length(&dist) < Radius.x + fRadius) &&
						pPos->y < posObj.y + Radius.y && pPos->y + fHeight > posObj.y)
					{// 木と重なった
						// 当たり判定
						bColl = dynamic_cast<CTree*>(pObj)->m_apModel[0]->Collision(posObj, pPos, posOld, move, fRadius, fHeight);

						if (dynamic_cast<CTree*>(pObj)->GetShake() == true && bColl == true)
						{// 一度揺らす
							// 揺らす
							dynamic_cast<CTree*>(pObj)->Shake(*pPos);

							// 揺れるフラグを消す
							dynamic_cast<CTree*>(pObj)->SetShake(false);
						}

						return true;
					}
					else
					{// 遠かった
						// 揺れるようにする
						dynamic_cast<CTree*>(pObj)->SetShake(true);
					}
				}
			}
		}

#endif
	}

	return false;
}

//========================================================================
// 木を揺らす処理
//========================================================================
void CTree::Shake(const D3DXVECTOR3 posP)
{
	D3DXVECTOR3 pos = GetPosition();					// 自分の位置
	D3DXVECTOR3 rot = GetRotation();					// 自分の向き
	D3DXVECTOR3 posM = m_apModel[0]->GetPosition();
	D3DXVECTOR3 dist;			// 距離
	float fDist;				// 距離

	// 距離を計算
	dist = pos - posP;
	fDist = D3DXVec3Length(&dist);
	D3DXVec3Normalize(&dist, &dist);

	// 位置のオフセットに代入
	posM.x = sinf(dist.z) * cosf(rot.y) * SHAKE_MOVE;
	posM.z = sinf(dist.x) * cosf(rot.y + D3DX_PI) * SHAKE_MOVE;

	// 位置を適用
	m_apModel[0]->SetPosition(posM);
}