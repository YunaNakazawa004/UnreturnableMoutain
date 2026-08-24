//========================================================================
// 
// ランキングUI [ UI_ranking.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "UI_ranking.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "texture.h"

//************************************************************************
// マクロ定義
//************************************************************************

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
int CRankingUI::m_nIdxTexture = -1;				// テクスチャのインデックス

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CRankingUI::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_nIdxTexture = pTexture->Register("data\\TEXTURE\\UI\\ranking.png");

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
void CRankingUI::Unload(void)
{
	// テクスチャのインデックスを削除
	memset(&m_nIdxTexture, -1, sizeof m_nIdxTexture);
}

//========================================================================
// ランキングUIクラスの生成処理
//========================================================================
CRankingUI* CRankingUI::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CRankingUI* pRankingUI = NULL;

	if (pRankingUI == NULL)
	{// NULLチェック
		// ランキングUIの生成
		pRankingUI = new CRankingUI;
	}

	if (pRankingUI != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pRankingUI->Init(pos, fWidth, fHeight)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! ランキングUIの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// オブジェクトの種類を設定
		pRankingUI->SetType(TYPE_RESULTUI);

		// テクスチャの割り当て
		pRankingUI->BindTexture(m_nIdxTexture);

		return pRankingUI;
	}

	OutputDebugStringA("! ! ! ランキングUIの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// ランキングUIクラスのコンストラクタ
//========================================================================
CRankingUI::CRankingUI(const int nPriority) :CObject2D(nPriority)
{
	// ランキングUIクラスの値をクリア
}

//========================================================================
// ランキングUIクラスのデストラクタ
//========================================================================
CRankingUI::~CRankingUI()
{
}

//========================================================================
// ランキングUIクラスの初期化処理
//========================================================================
HRESULT CRankingUI::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	if (FAILED(CObject2D::Init(pos, fWidth, fHeight)))
	{// ランキングUIの初期化に失敗した場合
		OutputDebugStringA("! ! ! ランキングUIの初期化に失敗しました ! ! !\n");

		return E_FAIL;
	}

	// ランキングUIクラスの値を初期化

	return S_OK;
}

//========================================================================
// ランキングUIクラスの終了処理
//========================================================================
void CRankingUI::Uninit(void)
{
	// 終了処理
	CObject2D::Uninit();
}

//========================================================================
// ランキングUIクラスの更新処理
//========================================================================
void CRankingUI::Update(void)
{
}

//========================================================================
// ランキングUIクラスの描画処理
//========================================================================
void CRankingUI::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}