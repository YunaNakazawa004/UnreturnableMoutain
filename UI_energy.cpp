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
	m_nIdxTexture = pTexture->Register("data\\TEXTURE\\UI\\energy.png");

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
	memset(&m_apNumber[0], NULL, sizeof m_apNumber);
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

	// エネルギーUIクラスの値を初期化
	for (int nCntNum = 0; nCntNum < ENERGYUI_NUMPLACE; nCntNum++)
	{
		if (m_apNumber[nCntNum] == NULL)
		{// NULLチェック
			m_apNumber[nCntNum] =
				CNumber::Create(D3DXVECTOR3((pos.x - fWidth) + (nCntNum * (fWidth / 2.0f)), pos.y - (fHeight / 2.0f), 0.0f),
					fWidth / 2.0f, fHeight);
		}

		if (m_apNumber[nCntNum] != NULL)
		{// NULLチェック
			m_apNumber[nCntNum]->SetTexUV(0, TEXTURE_WIDTH, 1);
		}
	}

	return S_OK;
}

//========================================================================
// エネルギーUIクラスの終了処理
//========================================================================
void CEnergyUI::Uninit(void)
{
	for (int nCntNum = 0; nCntNum < ENERGYUI_NUMPLACE; nCntNum++)
	{
		if (m_apNumber[nCntNum] != NULL)
		{// NULLチェック
			// 終了処理
			m_apNumber[nCntNum]->Uninit();

			delete m_apNumber[nCntNum];
			m_apNumber[nCntNum] = NULL;
		}
	}

	// 終了処理
	CObject2D::Uninit();
}

//========================================================================
// エネルギーUIクラスの更新処理
//========================================================================
void CEnergyUI::Update(void)
{
}

//========================================================================
// エネルギーUIクラスの描画処理
//========================================================================
void CEnergyUI::Draw(void)
{
	// 描画処理
	CObject2D::Draw();

	for (int nCntNum = 0; nCntNum < ENERGYUI_NUMPLACE; nCntNum++)
	{
		if (m_apNumber[nCntNum] != NULL)
		{// NULLチェック
			// 描画処理
			m_apNumber[nCntNum]->Draw();
		}
	}
}

//========================================================================
// エネルギーUIの設定処理
//========================================================================
void CEnergyUI::SetEnergy(const float fEnergy)
{
	// ローカル変数
	int aTexU[ENERGYUI_NUMPLACE];				// 各桁の数字を格納

	for (int nCntNum = 0; nCntNum < ENERGYUI_NUMPLACE; nCntNum++)
	{
		// 各桁の数字を設定
		aTexU[nCntNum] = (int)fEnergy % (int)pow(POWER, ENERGYUI_NUMPLACE - nCntNum) / (int)pow(POWER, ENERGYUI_NUMPLACE - nCntNum - 1);

		// テクスチャ座標を設定
		m_apNumber[nCntNum]->SetTexUV(aTexU[nCntNum], TEXTURE_WIDTH, 1);
	}
}