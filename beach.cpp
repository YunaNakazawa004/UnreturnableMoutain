//========================================================================
// 
// 砂浜 [ beach.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "beach.h"

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
int CBeach::m_aIdxTexture[FIELD_TEXTURE_NUM] = {};				// テクスチャのインデックス

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CBeach::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_aIdxTexture[0] = pTexture->Register("data\\TEXTURE\\field002.jpg");
	m_aIdxTexture[1] = pTexture->Register("data\\TEXTURE\\sand.jpg");

	if (m_aIdxTexture[0] == -1 || m_aIdxTexture[1] == -1)
	{// テクスチャが設定できていない
		OutputDebugStringA("! ! ! テクスチャの設定に失敗しました ! ! !\n");

		return E_FAIL;
	}

	return S_OK;
}

//========================================================================
// テクスチャの破棄
//========================================================================
void CBeach::Unload(void)
{
	// テクスチャのインデックスを削除
	memset(&m_aIdxTexture[0], -1, sizeof m_aIdxTexture);
}

//========================================================================
// 砂浜クラスの生成処理
//========================================================================
CBeach* CBeach::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR2 block,
	const D3DXVECTOR2 size)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CBeach* pBeach = NULL;

	if (pBeach == NULL)
	{// NULLチェック
		// 砂浜の生成
		pBeach = new CBeach;
	}

	if (pBeach != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pBeach->Init(pos, rot, block, size)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! 砂浜の初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// 種類を設定
		pBeach->SetType(TYPE_BEACH);

		// テクスチャを設定
		pBeach->BindTexture(&m_aIdxTexture[0]);

		return pBeach;
	}

	OutputDebugStringA("! ! ! 砂浜の生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// 砂浜クラスのコンストラクタ
//========================================================================
CBeach::CBeach(const int nPriority) :CMeshField(nPriority)
{
	// 砂浜クラスの値をクリア
}

//========================================================================
// 砂浜クラスのデストラクタ
//========================================================================
CBeach::~CBeach()
{
}

//========================================================================
// 砂浜クラスの初期化処理(オーバーロード)
//========================================================================
HRESULT CBeach::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
	const D3DXVECTOR2 block, const D3DXVECTOR2 size)
{
	// 初期化処理
	CMeshField::Init(pos, rot, block, size);

	return S_OK;
}

//========================================================================
// 砂浜クラスの終了処理
//========================================================================
void CBeach::Uninit(void)
{
	// 終了処理
	CMeshField::Uninit();
}

//========================================================================
// 砂浜クラスの更新処理
//========================================================================
void CBeach::Update(void)
{
}

//========================================================================
// 砂浜クラスの描画処理
//========================================================================
void CBeach::Draw(void)
{
	// ローカル変数宣言
	CRenderer* pRenderer = CManager::GetRenderer();			// レンダラーへのポインタ
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスへのポインタ

	// テクスチャステージステート0の設定
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

	// テクスチャステージステート1の設定
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDCURRENTALPHA);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);

	// 描画処理
	CMeshField::Draw();

	// テクスチャステージステートの設定
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

	// テクスチャステージステートの設定
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}