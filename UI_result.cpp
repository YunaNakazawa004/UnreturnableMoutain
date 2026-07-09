//========================================================================
// 
// リザルトUI [ UI_result.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "UI_result.h"

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
int CResultUI::m_aIdxTexture[TYPE_MAX] = { -1 };				// テクスチャのインデックス

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CResultUI::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_aIdxTexture[0] = pTexture->Register("data\\TEXTURE\\UI\\result000.png");
	m_aIdxTexture[1] = pTexture->Register("data\\TEXTURE\\UI\\result001.png");

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
void CResultUI::Unload(void)
{
	// テクスチャのインデックスを削除
	memset(&m_aIdxTexture[0], -1, sizeof m_aIdxTexture);
}

//========================================================================
// リザルトUIクラスの生成処理
//========================================================================
CResultUI* CResultUI::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const TYPE type)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CResultUI* pResultUI = NULL;

	if (pResultUI == NULL)
	{// NULLチェック
		// リザルトUIの生成
		pResultUI = new CResultUI;
	}

	if (pResultUI != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pResultUI->Init(pos, fWidth, fHeight, type)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! リザルトUIの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// オブジェクトの種類を設定
		pResultUI->SetType(TYPE_RESULTUI);

		// テクスチャの割り当て
		pResultUI->BindTexture(m_aIdxTexture[type]);

		return pResultUI;
	}

	OutputDebugStringA("! ! ! リザルトUIの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// リザルトUIクラスのコンストラクタ
//========================================================================
CResultUI::CResultUI(const int nPriority) :CObject2D(nPriority)
{
	// リザルトUIクラスの値をクリア
	m_type = TYPE_CLEAR;
}

//========================================================================
// リザルトUIクラスのデストラクタ
//========================================================================
CResultUI::~CResultUI()
{
}

//========================================================================
// リザルトUIクラスの初期化処理
//========================================================================
HRESULT CResultUI::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const TYPE type)
{
	if (FAILED(CObject2D::Init(pos, fWidth, fHeight)))
	{// リザルトUIの初期化に失敗した場合
		OutputDebugStringA("! ! ! リザルトUIの初期化に失敗しました ! ! !\n");

		return E_FAIL;
	}

	// リザルトUIクラスの値を初期化
	m_type = type;

	return S_OK;
}

//========================================================================
// リザルトUIクラスの終了処理
//========================================================================
void CResultUI::Uninit(void)
{
	// 終了処理
	CObject2D::Uninit();
}

//========================================================================
// リザルトUIクラスの更新処理
//========================================================================
void CResultUI::Update(void)
{
}

//========================================================================
// リザルトUIクラスの描画処理
//========================================================================
void CResultUI::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}