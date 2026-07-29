//========================================================================
// 
// チュートリアル文 [ tutorial_txt.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "tutorial_txt.h"

#include "renderer.h"
#include "manager.h"
#include "texture.h"

#include "title.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define TUTORIAL_TXT_WIDTH	(100.0f)			// チュートリアル文の幅
#define TUTORIAL_TXT_HEIGHT	(50.0f)				// チュートリアル文の高さ

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
int CTutorialTxt::m_aIdxTexture[TYPE_MAX] = {};				// テクスチャのインデックス

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CTutorialTxt::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_aIdxTexture[0] = pTexture->Register("data\\TEXTURE\\UI\\tutorial_txt000.png");
	m_aIdxTexture[1] = pTexture->Register("data\\TEXTURE\\UI\\tutorial_txt001.png");
	m_aIdxTexture[2] = pTexture->Register("data\\TEXTURE\\UI\\tutorial_txt002.png");
	m_aIdxTexture[3] = pTexture->Register("data\\TEXTURE\\UI\\tutorial_txt003.png");

	if (m_aIdxTexture[0] == -1 || m_aIdxTexture[1] == -1 || m_aIdxTexture[2] == -1 || m_aIdxTexture[3] == -1)
	{// テクスチャが設定できていない
		OutputDebugStringA("! ! ! テクスチャの設定に失敗しました ! ! !\n");

		return E_FAIL;
	}

	return S_OK;
}

//========================================================================
// テクスチャの破棄
//========================================================================
void CTutorialTxt::Unload(void)
{
	// テクスチャのインデックスを削除
	memset(&m_aIdxTexture[0], -1, sizeof m_aIdxTexture);
}

//========================================================================
// チュートリアル文クラスの生成処理
//========================================================================
CTutorialTxt* CTutorialTxt::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int type)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CTutorialTxt* pTutorialTxt = NULL;

	if (pTutorialTxt == NULL)
	{// NULLチェック
		// チュートリアル文の生成
		pTutorialTxt = new CTutorialTxt;
	}

	if (pTutorialTxt != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pTutorialTxt->Init(pos, rot)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! チュートリアル文の初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// テクスチャを設定
		pTutorialTxt->BindTexture(m_aIdxTexture[type]);

		// 種類を設定
		pTutorialTxt->SetType(TYPE_TUTORIAL_TXT);

		return pTutorialTxt;
	}

	OutputDebugStringA("! ! ! チュートリアル文の生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// チュートリアル文クラスのコンストラクタ
//========================================================================
CTutorialTxt::CTutorialTxt(const int nPriority) :CObject3D(nPriority)
{
	// 値をクリア
}

//========================================================================
// チュートリアル文クラスのデストラクタ
//========================================================================
CTutorialTxt::~CTutorialTxt()
{
}

//========================================================================
// チュートリアル文クラスの初期化処理
//========================================================================
HRESULT CTutorialTxt::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// 初期化処理
	CObject3D::Init(pos, TUTORIAL_TXT_WIDTH, TUTORIAL_TXT_HEIGHT, 0.0f, CObject3D::MAINPOS_CENTER);

	// 角度を設定
	SetRotation(rot);

	return S_OK;
}

//========================================================================
// チュートリアル文クラスの終了処理
//========================================================================
void CTutorialTxt::Uninit(void)
{
	// 終了処理
	CObject3D::Uninit();
}

//========================================================================
// チュートリアル文クラスの更新処理
//========================================================================
void CTutorialTxt::Update(void)
{
}

//========================================================================
// チュートリアル文クラスの描画処理
//========================================================================
void CTutorialTxt::Draw(void)
{
	// ローカル変数宣言
	CRenderer* pRenderer = CManager::GetRenderer();			// レンダラーへのポインタ
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();			// デバイスへのポインタ

	// カリングの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// 描画処理
	CObject3D::Draw();

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// カリングの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}