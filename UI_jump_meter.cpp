//========================================================================
// 
// ジャンプメーターUI [ UI_jump_meter.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "UI_jump_meter.h"

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
int CJumpMeterUI::m_nIdxTexture = -1;				// テクスチャのインデックス

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CJumpMeterUI::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_nIdxTexture = pTexture->Register("data\\TEXTURE\\UI\\jumpmeter001.png");

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
void CJumpMeterUI::Unload(void)
{
	// テクスチャのインデックスを削除
	memset(&m_nIdxTexture, -1, sizeof m_nIdxTexture);
}

//========================================================================
// ジャンプメーターUIクラスの生成処理
//========================================================================
CJumpMeterUI* CJumpMeterUI::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fJump)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CJumpMeterUI* pJumpMeterUI = NULL;

	if (pJumpMeterUI == NULL)
	{// NULLチェック
		// ジャンプメーターUIの生成
		pJumpMeterUI = new CJumpMeterUI;
	}

	if (pJumpMeterUI != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pJumpMeterUI->Init(pos, fWidth, fHeight, fJump)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! ジャンプメーターUIの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// オブジェクトの種類を設定
		pJumpMeterUI->SetType(TYPE_JUMPUI);

		// テクスチャの割り当て
		pJumpMeterUI->BindTexture(m_nIdxTexture);

		return pJumpMeterUI;
	}

	OutputDebugStringA("! ! ! ジャンプメーターUIの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// ジャンプメーターUIクラスのコンストラクタ
//========================================================================
CJumpMeterUI::CJumpMeterUI(const int nPriority) :CObject2D(nPriority)
{
	// ジャンプメーターUIクラスの値をクリア
	m_pFrame = NULL;
	m_fJump = 0.0f;
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
}

//========================================================================
// ジャンプメーターUIクラスのデストラクタ
//========================================================================
CJumpMeterUI::~CJumpMeterUI()
{
}

//========================================================================
// ジャンプメーターUIクラスの初期化処理
//========================================================================
HRESULT CJumpMeterUI::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fJump)
{
	if (FAILED(CObject2D::Init(pos, fWidth, fHeight, CObject2D::POS_MID_BOTTOM)))
	{// ジャンプメーターUIの初期化に失敗した場合
		OutputDebugStringA("! ! ! ジャンプメーターUIの初期化に失敗しました ! ! !\n");

		return E_FAIL;
	}

	// ジャンプメーターUIクラスの値を初期化
	m_fJump = fJump;
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	m_pFrame = CObject2D::Create(pos, fWidth, fHeight, TYPE_JUMPUI, 
		"data\\TEXTURE\\UI\\jumpmeter000.png", PRIORITY_5, CObject2D::POS_MID_BOTTOM);

	return S_OK;
}

//========================================================================
// ジャンプメーターUIクラスの終了処理
//========================================================================
void CJumpMeterUI::Uninit(void)
{
	if (m_pFrame != NULL)
	{// NULLチェック
		m_pFrame = NULL;
	}

	// 終了処理
	CObject2D::Uninit();
}

//========================================================================
// ジャンプメーターUIクラスの更新処理
//========================================================================
void CJumpMeterUI::Update(void)
{
}

//========================================================================
// ジャンプメーターUIクラスの描画処理
//========================================================================
void CJumpMeterUI::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}

//========================================================================
// ジャンプメーターUIの設定処理
//========================================================================
void CJumpMeterUI::SetJumpMeter(const float fJump)
{
	m_fJump = fJump;

	// 頂点座標を設定
	SetPosition(GetPosition(), m_fWidth, (m_fHeight * m_fJump));

	// テクスチャ座標を設定
	SetTexUV(0.0f, 1.0f, 1.0f - fJump, 1.0f);
}