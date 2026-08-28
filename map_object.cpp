//========================================================================
// 
// マップオブジェクト [map_object.cpp]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "map_object.h"

#include "manager.h"
#include "debugproc.h"
#include "input.h"
#include "sound.h"

#include "object.h"
#include "particle3D.h"
#include "game.h"

#include "grass.h"
#include "tree.h"
#include "rock.h"
#include "flower.h"
#include "energyrock.h"
#include "UI_item.h"
#include "score.h"
#include "collect_num.h"

#include "player.h"

#include <fstream>
#include <iostream>

//************************************************************************
// マクロ定義
//************************************************************************
#define COLLECT_DIST		(50.0f)			// プレイヤーと収集アイテムとの距離

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
CMapObject::Map_Obj CMapObject::m_aMapObject[MAX_MAP_OBJECT] = {};	  	// 配置したオブジェクトの情報
int CMapObject::m_nNumObject = 0;						  				// 現在オブジェクトの総数
int CMapObject::m_nNumCollectObj = 0;					  				// 収集アイテムの総数
int CMapObject::m_nMaxCollectObj = 0;					  				// 収集アイテムの初期総数

//========================================================================
// 生成処理
//========================================================================
void CMapObject::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int mapObj, const bool bCollect)
{
	if (m_aMapObject[m_nNumObject].apObject == NULL)
	{// NULLチェック
		switch (mapObj)
		{
		case MAP_OBJ_TREE_C0:		// 収集アイテムを生成
			m_aMapObject[m_nNumObject].apObject = CTree::Create(pos, rot, CTree::TYPE_C);
			m_aMapObject[m_nNumObject].aObjType = mapObj;
			m_aMapObject[m_nNumObject].bCollect = bCollect;

			break;

		case MAP_OBJ_TREE_C1:		// 収集アイテムを生成
			m_aMapObject[m_nNumObject].apObject = CRock::Create(pos, rot, CRock::TYPE_CT);
			m_aMapObject[m_nNumObject].aObjType = mapObj;
			m_aMapObject[m_nNumObject].bCollect = bCollect;

			break;

		case MAP_OBJ_ROCK_C0:		// 収集アイテムを生成
			m_aMapObject[m_nNumObject].apObject = CRock::Create(pos, rot, CRock::TYPE_C0);
			m_aMapObject[m_nNumObject].aObjType = mapObj;
			m_aMapObject[m_nNumObject].bCollect = bCollect;

			break;

		case MAP_OBJ_ROCK_C1:		// 収集アイテムを生成
			m_aMapObject[m_nNumObject].apObject = CRock::Create(pos, rot, CRock::TYPE_C1);
			m_aMapObject[m_nNumObject].aObjType = mapObj;
			m_aMapObject[m_nNumObject].bCollect = bCollect;

			break;

		case MAP_OBJ_FLOWER_C0:		// 収集アイテムを生成
			m_aMapObject[m_nNumObject].apObject = CFlower::Create(pos, rot, CFlower::TYPE_C_0);
			m_aMapObject[m_nNumObject].aObjType = mapObj;
			m_aMapObject[m_nNumObject].bCollect = bCollect;

			break;

		case MAP_OBJ_FLOWER_C1:		// 収集アイテムを生成
			m_aMapObject[m_nNumObject].apObject = CFlower::Create(pos, rot, CFlower::TYPE_C_1);
			m_aMapObject[m_nNumObject].aObjType = mapObj;
			m_aMapObject[m_nNumObject].bCollect = bCollect;

			break;

		case MAP_OBJ_LEAF_C0:		// 収集アイテムを生成
			m_aMapObject[m_nNumObject].apObject = CFlower::Create(pos, rot, CFlower::TYPE_CL_0);
			m_aMapObject[m_nNumObject].aObjType = mapObj;
			m_aMapObject[m_nNumObject].bCollect = bCollect;

			break;

		case MAP_OBJ_LEAF_C1:		// 収集アイテムを生成
			m_aMapObject[m_nNumObject].apObject = CRock::Create(pos, rot, CRock::TYPE_CL);
			m_aMapObject[m_nNumObject].aObjType = mapObj;
			m_aMapObject[m_nNumObject].bCollect = bCollect;

			break;

		case MAP_OBJ_GRASS:			// 草を生成
			m_aMapObject[m_nNumObject].apObject = CGrass::Create(pos, rot);
			m_aMapObject[m_nNumObject].aObjType = mapObj;
			m_aMapObject[m_nNumObject].bCollect = bCollect;

			break;

		case MAP_OBJ_TREE:			// 木を生成
			m_aMapObject[m_nNumObject].apObject = CTree::Create(pos, rot);
			m_aMapObject[m_nNumObject].aObjType = mapObj;
			m_aMapObject[m_nNumObject].bCollect = bCollect;

			break;

		case MAP_OBJ_ROCK:			// 岩を生成
			m_aMapObject[m_nNumObject].apObject = CRock::Create(pos, rot);
			m_aMapObject[m_nNumObject].aObjType = mapObj;
			m_aMapObject[m_nNumObject].bCollect = bCollect;

			break;

		case MAP_OBJ_FLOWER:		// 花を生成
			m_aMapObject[m_nNumObject].apObject = CFlower::Create(pos, rot);
			m_aMapObject[m_nNumObject].aObjType = mapObj;
			m_aMapObject[m_nNumObject].bCollect = bCollect;

			break;

		case MAP_OBJ_ENERGYROCK:	// エネルギー鉱石を生成
			m_aMapObject[m_nNumObject].apObject = CEnergyRock::Create(pos, rot);
			m_aMapObject[m_nNumObject].aObjType = mapObj;
			m_aMapObject[m_nNumObject].bCollect = bCollect;

			break;
		}

		if (m_aMapObject[m_nNumObject].apObject != NULL)
		{// 生成できた
			// 総数をカウントアップ
			m_nNumObject++;

			if (bCollect == true)
			{// 収集アイテムなら
				// 収集アイテムの総数をカウントアップ
				m_nNumCollectObj++;
			}
		}
	}
}

//========================================================================
// マップオブジェクトのコンストラクタ
//========================================================================
CMapObject::CMapObject()
{
	// 値のクリア
	memset(&m_aMapObject[0], NULL, sizeof(m_aMapObject));
	m_nNumObject = 0;
	m_nNumCollectObj = 0;
	m_nMaxCollectObj = 0;
}

//========================================================================
// マップオブジェクトのデストラクタ
//========================================================================
CMapObject::~CMapObject()
{
}

//========================================================================
// マップオブジェクトの初期化処理
//========================================================================
HRESULT CMapObject::Init(void)
{
	// 値の初期化
	memset(&m_aMapObject[0], NULL, sizeof(m_aMapObject));
	m_nNumObject = 0;
	m_nNumCollectObj = 0;
	m_nMaxCollectObj = 0;

	return S_OK;
}

//========================================================================
// マップオブジェクトの終了処理
//========================================================================
void CMapObject::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_MAP_OBJECT; nCnt++)
	{
		if (m_aMapObject[nCnt].apObject != NULL)
		{// NULLチェック
			// 終了処理
			m_aMapObject[nCnt].apObject->Uninit();

			m_aMapObject[nCnt].apObject = NULL;
		}
	}
}

//========================================================================
// マップオブジェクトの更新処理
//========================================================================
void CMapObject::Update(void)
{
	CDebugProc* pDebugProc = CManager::GetDebugProc();					// デバッグ表示の取得
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();		// キーボード入力の取得

	static int nCounter = 0;

	for (int nCnt = 0; nCnt < MAX_MAP_OBJECT; nCnt++)
	{
		if (m_aMapObject[nCnt].apObject != NULL)
		{// NULLチェック
			if (m_aMapObject[nCnt].bCollect == true)
			{// 収集アイテムだったら
				D3DXVECTOR3 pos = m_aMapObject[nCnt].apObject->GetPosition();

				if (nCounter % 3 == 0)
				{// 一定間隔
					CParticle3D::Create(pos, 1, 1, 4.0f, -0.01f, 0.00f,
						CEffect3D::TYPE_BLENDADD, CParticle3D::TYPE_PIN, 400, 0.3f, false, COLOR_ORANGE, 10.0f, true,
						NULL, D3DXVECTOR3(pos.x, pos.y + 10000.0f, pos.z), 0.0001f);
				}

				CollectCollision(m_aMapObject[nCnt].apObject->GetPosition(), nCnt);
			}
		}
	}

	nCounter++;

#ifdef _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_DELETE) == true)
	{// 収集スキップ
		SkipCollectObj();
	}
#endif

	CCollectNum::SetCollectNum(m_nMaxCollectObj - m_nNumCollectObj);

	pDebugProc->Print("収集アイテムの総数 : %d\n", m_nNumCollectObj);
}

//========================================================================
// プレイヤーと収集アイテムの当たり判定
//========================================================================
bool CMapObject::CollectCollision(const D3DXVECTOR3 pos, const int nIdx)
{
	CItemUI* pItemUI = CGame::GetItemUI();		// アイテムUIを取得
	CScore* pScore = CGame::GetScore();			// スコアを取得
	CSound* pSound = CManager::GetSound();		// サウンドを取得
	D3DXVECTOR3 posPlayer = CGame::GetPlayer()->GetPosition();
	D3DXVECTOR3 dist;

	// 距離を計算
	dist = pos - posPlayer;

	if (D3DXVec3Length(&dist) <= COLLECT_DIST)
	{// 収集アイテムと距離が近い
		m_aMapObject[nIdx].bCollect = false;

		CParticle3D::Create(pos, 5, 10, 1.0f, 0.1f, 0.01f,
			CEffect3D::TYPE_BLENDADD, CParticle3D::TYPE_NORMAL, 600, 0.3f, false,
			COLOR_WHITE, 0.0f, true, CGame::GetPlayer(), DEFAULT_VECTER3, 0.05f);

		// アイテムをチェックする
		pItemUI->Check(m_aMapObject[nIdx].nItemIdx);

		// スコア加算
		pScore->Add(20000);

		pSound->PlaySound(CSound::SE_COLLECT);

		// 総数を減らす
		m_nNumCollectObj--;

		return true;
	}

	return false;
}

//========================================================================
// マップオブジェクトの読み込み処理
//========================================================================
void CMapObject::ResetData(void)
{
	// 現在のオブジェクトを破棄
	Uninit();

	// カウントリセット
	m_nNumObject = 0;
	m_nNumCollectObj = 0;
}

//========================================================================
// マップオブジェクトの書き込み処理
//========================================================================
HRESULT CMapObject::WriteData(const char* pFilename)
{
	// バイナリ形式でファイルオープン
	std::ofstream file(pFilename, std::ios_base::out | std::ios_base::binary);

	if (file.is_open() == true)
	{// ファイルが開けた
		// データを書き込む
		file.write((const char*)&m_nNumObject, sizeof(m_nNumObject));

		for (int nCnt = 0; nCnt < m_nNumObject; nCnt++)
		{
			D3DXVECTOR3 pos = m_aMapObject[nCnt].apObject->GetPosition();
			D3DXVECTOR3 rot = m_aMapObject[nCnt].apObject->GetRotation();

			file.write((const char*)&m_aMapObject[nCnt].aObjType, sizeof(int));
			file.write((const char*)&pos, sizeof(pos));
			file.write((const char*)&rot, sizeof(rot));
			file.write((const char*)&m_aMapObject[nCnt].bCollect, sizeof(bool));
		}

		// ファイルを閉じる
		file.close();
	}
	else
	{// ファイルが開けなかった
		OutputDebugStringA("! ! ! ファイルを開けませんでした ! ! !\n");

		return E_FAIL;
	}

	return S_OK;
}

//========================================================================
// マップオブジェクトの読み込み処理
//========================================================================
HRESULT CMapObject::ReadData(const char* pFilename)
{
	CItemUI* pItemUI = CGame::GetItemUI();		// アイテムUIを取得

	// バイナリ形式でファイルオープン
	std::ifstream file(pFilename, std::ios_base::in | std::ios_base::binary);

	if (file.is_open() == true)
	{// ファイルが開けた
		// 現在のオブジェクトを破棄
		Uninit();

		// カウントリセット
		int nNumObject = 0;
		m_nNumObject = 0;
		m_nNumCollectObj = 0;

		// ファイルから読み込む
		file.read((char*)&nNumObject, sizeof(nNumObject));

		for (int nCnt = 0; nCnt < nNumObject; nCnt++)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			file.read((char*)&m_aMapObject[nCnt].aObjType, sizeof(int));
			file.read((char*)&pos, sizeof(pos));
			file.read((char*)&rot, sizeof(rot));
			file.read((char*)&m_aMapObject[nCnt].bCollect, sizeof(bool));

			// オブジェクトの生成
			Create(pos, rot, m_aMapObject[nCnt].aObjType, m_aMapObject[nCnt].bCollect);

			if (m_aMapObject[nCnt].bCollect == true)
			{// 収集アイテムだった場合
				if (pItemUI != NULL)
				{// NULLチェック
					m_aMapObject[nCnt].nItemIdx = pItemUI->SetItem(m_aMapObject[nCnt].aObjType);
				}
			}
		}

		// 総数を確認
		if (m_nNumObject != nNumObject)
		{// もし違ったら
			OutputDebugStringA("! ! ! マップオブジェクトの読み込みに失敗しました ! ! !\n");
		}

		m_nMaxCollectObj = m_nNumCollectObj;

		// ファイルを閉じる
		file.close();
	}
	else
	{// ファイルが開けなかった
		OutputDebugStringA("! ! ! ファイルを開けませんでした ! ! !\n");

		return E_FAIL;
	}

	return S_OK;
}