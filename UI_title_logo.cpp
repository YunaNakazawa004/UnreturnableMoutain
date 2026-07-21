//========================================================================
// 
// タイトルロゴ [ UI_title_logo.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "UI_title_logo.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "texture.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define SHAKE_SPEED			(0.02f)			// 揺れる速さ
#define SHAKE_DIST			(0.1f)			// 揺れる大きさ

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
int CTitleLogo::m_nIdxTexture = -1;				// テクスチャのインデックス

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CTitleLogo::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_nIdxTexture = pTexture->Register("data\\TEXTURE\\UI\\title.png");

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
void CTitleLogo::Unload(void)
{
	// テクスチャのインデックスを削除
	memset(&m_nIdxTexture, -1, sizeof m_nIdxTexture);
}

//========================================================================
// タイトルロゴクラスの生成処理
//========================================================================
CTitleLogo* CTitleLogo::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CTitleLogo* pTitleLogo = NULL;

	if (pTitleLogo == NULL)
	{// NULLチェック
		// タイトルロゴの生成
		pTitleLogo = new CTitleLogo;
	}

	if (pTitleLogo != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pTitleLogo->Init(pos, fWidth, fHeight)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! タイトルロゴの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// オブジェクトの種類を設定
		pTitleLogo->SetType(TYPE_TITLELOGO);

		// テクスチャの割り当て
		pTitleLogo->BindTexture(m_nIdxTexture);

		return pTitleLogo;
	}

	OutputDebugStringA("! ! ! タイトルロゴの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// タイトルロゴクラスのコンストラクタ
//========================================================================
CTitleLogo::CTitleLogo(const int nPriority) :CObject2D(nPriority)
{
	// タイトルロゴクラスの値をクリア
}

//========================================================================
// タイトルロゴクラスのデストラクタ
//========================================================================
CTitleLogo::~CTitleLogo()
{
}

//========================================================================
// タイトルロゴクラスの初期化処理
//========================================================================
HRESULT CTitleLogo::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	if (FAILED(CObject2D::Init(pos, fWidth, fHeight)))
	{// タイトルロゴの初期化に失敗した場合
		OutputDebugStringA("! ! ! タイトルロゴの初期化に失敗しました ! ! !\n");

		return E_FAIL;
	}

	// タイトルロゴクラスの値を初期化

	return S_OK;
}

//========================================================================
// タイトルロゴクラスの終了処理
//========================================================================
void CTitleLogo::Uninit(void)
{
	// 終了処理
	CObject2D::Uninit();
}

//========================================================================
// タイトルロゴクラスの更新処理
//========================================================================
void CTitleLogo::Update(void)
{
	// ローカル変数
	D3DXVECTOR3 pos = GetPosition();
	static float fShake = 0.0f;

	fShake += SHAKE_SPEED;

	pos.y += cosf(fShake) * SHAKE_DIST;

	// 位置を設定
	SetPosition(pos);
}

//========================================================================
// タイトルロゴクラスの描画処理
//========================================================================
void CTitleLogo::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}