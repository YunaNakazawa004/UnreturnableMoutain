//========================================================================
// 
// 山 [ meshfield.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "mountain.h"

#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "input.h"
#include "debugproc.h"

#include "player.h"

//************************************************************************
// マクロ定義
//************************************************************************

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
int CMountain::m_aIdxTexture[FIELD_TEXTURE_NUM] = {};				// テクスチャのインデックス

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CMountain::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_aIdxTexture[0] = pTexture->Register("data\\TEXTURE\\field002.jpg");
	m_aIdxTexture[1] = pTexture->Register("data\\TEXTURE\\rock.jpg");

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
void CMountain::Unload(void)
{
	// テクスチャのインデックスを削除
	memset(&m_aIdxTexture[0], -1, sizeof m_aIdxTexture);
}

//========================================================================
// 山クラスの生成処理
//========================================================================
CMountain* CMountain::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR2 block,
	const D3DXVECTOR2 size)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CMountain* pMountain = NULL;

	if (pMountain == NULL)
	{// NULLチェック
		// 山の生成
		pMountain = new CMountain;
	}

	if (pMountain != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pMountain->Init(pos, rot, block, size)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! 山の初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// 種類を設定
		pMountain->SetType(TYPE_MOUNTAIN);

		// テクスチャを設定
		pMountain->BindTexture(&m_aIdxTexture[0]);

		return pMountain;
	}

	OutputDebugStringA("! ! ! 山の生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// 山クラスのコンストラクタ
//========================================================================
CMountain::CMountain(const int nPriority) :CMeshField(nPriority)
{
	// 山クラスの値をクリア
}

//========================================================================
// 山クラスのデストラクタ
//========================================================================
CMountain::~CMountain()
{
}

//========================================================================
// 山クラスの初期化処理(オーバーロード)
//========================================================================
HRESULT CMountain::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
	const D3DXVECTOR2 block, const D3DXVECTOR2 size)
{
	// 初期化処理
	CMeshField::Init(pos, rot, block, size);

	return S_OK;
}

//========================================================================
// 山クラスの終了処理
//========================================================================
void CMountain::Uninit(void)
{
	// 終了処理
	CMeshField::Uninit();
}

//========================================================================
// 山クラスの更新処理
//========================================================================
void CMountain::Update(void)
{
}

//========================================================================
// 山クラスの描画処理
//========================================================================
void CMountain::Draw(void)
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