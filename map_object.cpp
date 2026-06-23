//========================================================================
// 
// マップオブジェクト [map_object.cpp]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "map_object.h"

#include "object.h"

#include "grass.h"
#include "tree.h"
#include "rock.h"

#include <fstream>
#include <iostream>

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
CObject* CMapObject::m_apObject[MAX_MAP_OBJECT] = {};	  	// 配置したオブジェクトの情報
int CMapObject::m_aObjType[MAX_MAP_OBJECT] = {};		  	// 配置したオブジェクトの種類
int CMapObject::m_nNumObject = 0;						  	// 現在オブジェクトの総数

//========================================================================
// 生成処理
//========================================================================
void CMapObject::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int mapObj)
{
	if (m_apObject[m_nNumObject] == NULL)
	{// NULLチェック
		switch (mapObj)
		{
		case MAP_OBJ_GRASS:			// 草を生成
			m_apObject[m_nNumObject] = CGrass::Create(pos, rot);
			m_aObjType[m_nNumObject] = mapObj;

			break;

		case MAP_OBJ_TREE:			// 木を生成
			m_apObject[m_nNumObject] = CTree::Create(pos, rot);
			m_aObjType[m_nNumObject] = mapObj;

			break;

		case MAP_OBJ_ROCK:			// 岩を生成
			m_apObject[m_nNumObject] = CRock::Create(pos, rot);
			m_aObjType[m_nNumObject] = mapObj;

			break;
		}

		// 総数をカウントアップ
		m_nNumObject++;
	}
}

//========================================================================
// マップオブジェクトのコンストラクタ
//========================================================================
CMapObject::CMapObject()
{
	// 値のクリア
	memset(&m_apObject[0], NULL, sizeof(m_apObject));
	memset(&m_aObjType[0], MAP_OBJ_GRASS, sizeof(m_aObjType));
	m_nNumObject = 0;
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
	memset(&m_apObject[0], NULL, sizeof(m_apObject));
	memset(&m_aObjType[0], MAP_OBJ_GRASS, sizeof(m_aObjType));
	m_nNumObject = 0;

	return S_OK;
}

//========================================================================
// マップオブジェクトの終了処理
//========================================================================
void CMapObject::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_MAP_OBJECT; nCnt++)
	{
		if (m_apObject[nCnt] != NULL)
		{// NULLチェック
			// 終了処理
			m_apObject[nCnt]->Uninit();

			m_apObject[nCnt] = NULL;
		}
	}
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
			D3DXVECTOR3 pos = m_apObject[nCnt]->GetPosition();
			D3DXVECTOR3 rot = m_apObject[nCnt]->GetRotation();

			file.write((const char*)&m_aObjType[nCnt], sizeof(int));
			file.write((const char*)&pos, sizeof(pos));
			file.write((const char*)&rot, sizeof(rot));
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
	// バイナリ形式でファイルオープン
	std::ifstream file(pFilename, std::ios_base::in | std::ios_base::binary);

	if (file.is_open() == true)
	{// ファイルが開けた
		// 現在のオブジェクトを破棄
		Uninit();

		// カウントリセット
		int nNumObject = 0;
		m_nNumObject = 0;

		// ファイルから読み込む
		file.read((char*)&nNumObject, sizeof(nNumObject));

		for (int nCnt = 0; nCnt < nNumObject; nCnt++)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			file.read((char*)&m_aObjType[nCnt], sizeof(int));
			file.read((char*)&pos, sizeof(pos));
			file.read((char*)&rot, sizeof(rot));

			// オブジェクトの生成
			Create(pos, rot, m_aObjType[nCnt]);
		}

		// 総数を確認
		if (m_nNumObject != nNumObject)
		{// もし違ったら
			OutputDebugStringA("! ! ! マップオブジェクトの読み込みに失敗しました ! ! !\n");
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