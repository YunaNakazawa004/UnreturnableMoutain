//========================================================================
// 
// 海 [ watersurface.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "watersurface.h"

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
int CWaterSurface::m_aIdxTexture[FIELD_TEXTURE_NUM] = {};				// テクスチャのインデックス

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CWaterSurface::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_aIdxTexture[0] = pTexture->Register("data\\TEXTURE\\sea.jpg");
	m_aIdxTexture[1] = pTexture->Register("data\\TEXTURE\\watersurface.jpg");

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
void CWaterSurface::Unload(void)
{
	// テクスチャのインデックスを削除
	memset(&m_aIdxTexture[0], -1, sizeof m_aIdxTexture);
}

//========================================================================
// 海クラスの生成処理
//========================================================================
CWaterSurface* CWaterSurface::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR2 block,
	const D3DXVECTOR2 size)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CWaterSurface* pWaterSurface = NULL;

	if (pWaterSurface == NULL)
	{// NULLチェック
		// 海の生成
		pWaterSurface = new CWaterSurface;
	}

	if (pWaterSurface != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pWaterSurface->Init(pos, rot, block, size)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! 海の初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// 種類を設定
		pWaterSurface->SetType(TYPE_WATERSURFACE);

		// テクスチャを設定
		pWaterSurface->BindTexture(&m_aIdxTexture[0]);

		return pWaterSurface;
	}

	OutputDebugStringA("! ! ! 海の生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// 海クラスのコンストラクタ
//========================================================================
CWaterSurface::CWaterSurface(const int nPriority) :CMeshField(nPriority)
{
	// 海クラスの値をクリア
	m_fWave = 0.0f;
}

//========================================================================
// 海クラスのデストラクタ
//========================================================================
CWaterSurface::~CWaterSurface()
{
}

//========================================================================
// 海クラスの初期化処理(オーバーロード)
//========================================================================
HRESULT CWaterSurface::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
	const D3DXVECTOR2 block, const D3DXVECTOR2 size)
{
	// 初期化処理
	CMeshField::Init(pos, rot, block, size);

	return S_OK;
}

//========================================================================
// 海クラスの終了処理
//========================================================================
void CWaterSurface::Uninit(void)
{
	// 終了処理
	CMeshField::Uninit();
}

//========================================================================
// 海クラスの更新処理
//========================================================================
void CWaterSurface::Update(void)
{
	m_fWave += 0.01f;

	for (int nCnt = 0; nCnt < GetNumVtx(); nCnt++)
	{
		D3DXVECTOR3 pos = GetPosVtx(nCnt);
		D3DXVECTOR3 posDest = DEFAULT_VECTER3;
		D3DXVECTOR3 dist = -pos;
		float fDist = D3DXVec3Length(&dist);

		// 波の動き
		posDest.y = cosf(fDist + m_fWave) * 0.1f;

		// 頂点座標設定
		SetPosVtx(nCnt, pos + posDest);

		// テクスチャ移動
		SetTexScroll(nCnt, 0.0002f, 0.0002f);
		SetTexMScroll(nCnt, -0.0002f, -0.0002f);
	}
}

//========================================================================
// 海クラスの描画処理
//========================================================================
void CWaterSurface::Draw(void)
{
	// ローカル変数宣言
	CRenderer* pRenderer = CManager::GetRenderer();			// レンダラーへのポインタ
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスへのポインタ

	// テクスチャステージステート0の設定
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// テクスチャステージステート1の設定
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);

	// 描画処理
	CMeshField::Draw();

	// テクスチャステージステートの設定			
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
}