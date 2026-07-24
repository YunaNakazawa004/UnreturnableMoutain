//========================================================================
// 
// エネルギーUI [ UI_energy.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "UI_energy.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "texture.h"

#include "number.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define POWER				(10)									// 10の累乗
#define TEXTURE_WIDTH		(10)									// テクスチャの幅

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
int CEnergyUI::m_nIdxTexture = -1;				// テクスチャのインデックス

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CEnergyUI::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_nIdxTexture = pTexture->Register("data\\TEXTURE\\UI\\energy000.png");

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
void CEnergyUI::Unload(void)
{
	// テクスチャのインデックスを削除
	m_nIdxTexture = -1;
}

//========================================================================
// エネルギーUIクラスの生成処理
//========================================================================
CEnergyUI* CEnergyUI::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CEnergyUI* pEnergyUI = NULL;

	if (pEnergyUI == NULL)
	{// NULLチェック
		// エネルギーUIの生成
		pEnergyUI = new CEnergyUI;
	}

	if (pEnergyUI != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pEnergyUI->Init(pos, fWidth, fHeight)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! エネルギーUIの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// オブジェクトの種類を設定
		pEnergyUI->SetType(TYPE_RESULTUI);

		// テクスチャの割り当て
		pEnergyUI->BindTexture(m_nIdxTexture);

		return pEnergyUI;
	}

	OutputDebugStringA("! ! ! エネルギーUIの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// エネルギーUIクラスのコンストラクタ
//========================================================================
CEnergyUI::CEnergyUI(const int nPriority) :CObject2D(nPriority)
{
	// エネルギーUIクラスの値をクリア
	m_nPatternAnim = 0;
	m_state = STATE_NORMAL;
	m_nCounterState = 0;
}

//========================================================================
// エネルギーUIクラスのデストラクタ
//========================================================================
CEnergyUI::~CEnergyUI()
{
}

//========================================================================
// エネルギーUIクラスの初期化処理
//========================================================================
HRESULT CEnergyUI::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	if (FAILED(CObject2D::Init(pos, fWidth, fHeight)))
	{// エネルギーUIの初期化に失敗した場合
		OutputDebugStringA("! ! ! エネルギーUIの初期化に失敗しました ! ! !\n");

		return E_FAIL;
	}

	// 値の初期化
	SetTexUV(3, 4, 1, 0.0f, 0.0f);

	return S_OK;
}

//========================================================================
// エネルギーUIクラスの終了処理
//========================================================================
void CEnergyUI::Uninit(void)
{
	// 終了処理
	CObject2D::Uninit();
}

//========================================================================
// エネルギーUIクラスの更新処理
//========================================================================
void CEnergyUI::Update(void)
{
	switch (m_state)
	{
	case STATE_NONE:		// 状態なし
		SetDisp(false);

		break;

	case STATE_NORMAL:		// 通常状態
		SetDisp(true);

		break;

	case STATE_BLINK:		// 点滅状態
		if (m_nCounterState % 15 == 0)
		{// 一定間隔で点滅
			SwapDisp();
		}

		break;
	}

	m_nCounterState++;
}

//========================================================================
// エネルギーUIクラスの描画処理
//========================================================================
void CEnergyUI::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}

//========================================================================
// エネルギーUIの設定処理
//========================================================================
void CEnergyUI::SetEnergy(const float fEnergy)
{
	if (fEnergy > 70.0f)
	{// 7割より多い
		m_nPatternAnim = 3;
		m_state = STATE_NORMAL;
	}
	else if (fEnergy > 60.0f)
	{// 6割より多い
		m_nPatternAnim = 3;
		m_state = STATE_BLINK;
	}
	else if (fEnergy > 40.0f)
	{// 4割より多い
		m_nPatternAnim = 2;
		m_state = STATE_NORMAL;
	}
	else if (fEnergy > 30.0f)
	{// 3割より多い
		m_nPatternAnim = 2;
		m_state = STATE_BLINK;
	}
	else if (fEnergy > 10.0f)
	{// 1割より多い
		m_nPatternAnim = 1;
		m_state = STATE_NORMAL;
	}
	else if(fEnergy > 0.0f)
	{// 0ではない
		m_nPatternAnim = 1;
		m_state = STATE_BLINK;
	}
	else
	{// 0
		m_nPatternAnim = 0;
		m_state = STATE_NORMAL;
	}

	SetTexUV(m_nPatternAnim, 4, 1, 0.0f, 0.0f);
}