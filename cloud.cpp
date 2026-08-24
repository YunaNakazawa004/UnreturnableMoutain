//========================================================================
// 
// 雲 [ cloud.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "cloud.h"

#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "input.h"
#include "debugproc.h"

//************************************************************************
// マクロ定義
//************************************************************************

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
int CCloud::m_nIdxTexture = {};				// テクスチャのインデックス

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CCloud::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_nIdxTexture = pTexture->Register("data\\TEXTURE\\sky001.png");

	if (m_nIdxTexture == -1)
	{// テクスチャが設定できていない
		OutputDebugStringA("! ! ! テクスチャの設定に失敗しました ! ! !\n");

		return E_FAIL;
	}

	return S_OK;
}

//========================================================================
// テクスチャの破棄
//========================================================================
void CCloud::Unload(void)
{
	// テクスチャのインデックスを削除
	memset(&m_nIdxTexture, -1, sizeof m_nIdxTexture);
}

//========================================================================
// 雲クラスの生成処理
//========================================================================
CCloud* CCloud::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR2 block,
	const float fRadius)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CCloud* pCloud = NULL;

	if (pCloud == NULL)
	{// NULLチェック
		// 雲の生成
		pCloud = new CCloud;
	}

	if (pCloud != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pCloud->Init(pos, rot, block, fRadius)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! 雲の初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// 種類を設定
		pCloud->SetType(TYPE_CLOUD);

		// テクスチャを設定
		pCloud->BindTexture(m_nIdxTexture);

		return pCloud;
	}

	OutputDebugStringA("! ! ! 雲の生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// 雲クラスのコンストラクタ
//========================================================================
CCloud::CCloud(const int nPriority) :CMeshDome(nPriority)
{
	// 雲クラスの値をクリア
}

//========================================================================
// 雲クラスのデストラクタ
//========================================================================
CCloud::~CCloud()
{
}

//========================================================================
// 雲クラスの初期化処理(オーバーロード)
//========================================================================
HRESULT CCloud::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
	const D3DXVECTOR2 block, const float fRadius)
{
	// 初期化処理
	CMeshDome::Init(pos, rot, block, fRadius);

	return S_OK;
}

//========================================================================
// 雲クラスの終了処理
//========================================================================
void CCloud::Uninit(void)
{
	// 終了処理
	CMeshDome::Uninit();
}

//========================================================================
// 雲クラスの更新処理
//========================================================================
void CCloud::Update(void)
{
	for (int nCount = 0; nCount < GetNumVtx(); nCount++)
	{
		SetTexScroll(nCount, 0.0001f, 0.0f);
	}
}

//========================================================================
// 雲クラスの描画処理
//========================================================================
void CCloud::Draw(void)
{
	// ローカル変数宣言
	CRenderer* pRenderer = CManager::GetRenderer();			// レンダラーへのポインタ
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスへのポインタ

	// ライティングオフ
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 描画処理
	CMeshDome::Draw();

	// ライティングオン
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}