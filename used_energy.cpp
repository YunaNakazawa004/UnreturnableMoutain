//========================================================================
// 
// 使用エネルギー量 [ used_energy.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "used_energy.h"

#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "number.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define POWER				(10)									// 10の累乗
#define TEXTURE_WIDTH		(10)									// テクスチャの幅
#define MAX_SCORE			(999999)								// 最大使用エネルギー量
#define MIN_SCORE			(0)										// 最小使用エネルギー量

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
int CUsedEnergy::m_nUsedEnergy = 0;			// 使用エネルギー量

//========================================================================
// 使用エネルギー量クラスの生成処理
//========================================================================
CUsedEnergy* CUsedEnergy::Create(const D3DXVECTOR3 pos,
	const float fWidth, const float fHeight)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CUsedEnergy* pUsedEnergy = NULL;

	if (pUsedEnergy == NULL)
	{// NULLチェック
		// 使用エネルギー量の生成
		pUsedEnergy = new CUsedEnergy;
	}

	if (pUsedEnergy != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pUsedEnergy->Init(pos, fWidth, fHeight)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! 使用エネルギー量の初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// オブジェクトの種類を設定
		pUsedEnergy->SetType(TYPE_USED_ENERGY);

		return pUsedEnergy;
	}

	OutputDebugStringA("! ! ! 使用エネルギー量の生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// 使用エネルギー量クラスのコンストラクタ
//========================================================================
CUsedEnergy::CUsedEnergy(const int nPriority):CObject(nPriority)
{
	// 使用エネルギー量クラスの値をクリア
	memset(&m_apNumber[0], NULL, sizeof m_apNumber);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bDisp = true;
}

//========================================================================
// 使用エネルギー量クラスのデストラクタ
//========================================================================
CUsedEnergy::~CUsedEnergy()
{
}

//========================================================================
// 使用エネルギー量クラスの初期化処理
//========================================================================
HRESULT CUsedEnergy::Init(const D3DXVECTOR3 pos,
	const float fWidth, const float fHeight)
{
	for (int nCntUsedEnergy = 0; nCntUsedEnergy < USED_ENERGY_NUMPLACE; nCntUsedEnergy++)
	{
		if (m_apNumber[nCntUsedEnergy] == NULL)
		{// NULLチェック
			m_apNumber[nCntUsedEnergy] = CNumber::Create(D3DXVECTOR3(pos.x + (nCntUsedEnergy * fWidth), pos.y, pos.z), fWidth, fHeight);
		}
	}

	// 使用エネルギー量クラスの値を初期化
	m_pos = pos;

	// 使用エネルギー量を設定
	SetNum(m_nUsedEnergy);

	return S_OK;
}

//========================================================================
// 使用エネルギー量クラスの終了処理
//========================================================================
void CUsedEnergy::Uninit(void)
{
	for (int nCntUsedEnergy = 0; nCntUsedEnergy < USED_ENERGY_NUMPLACE; nCntUsedEnergy++)
	{
		if (m_apNumber[nCntUsedEnergy] != NULL)
		{// NULLチェック
			// 終了処理
			m_apNumber[nCntUsedEnergy]->Uninit();

			delete m_apNumber[nCntUsedEnergy];
			m_apNumber[nCntUsedEnergy] = NULL;
		}
	}

	// 自分自身を破棄
	CObject::Release();
}

//========================================================================
// 使用エネルギー量クラスの更新処理
//========================================================================
void CUsedEnergy::Update(void)
{
}

//========================================================================
// 使用エネルギー量クラスの描画処理
//========================================================================
void CUsedEnergy::Draw(void)
{
	if (m_bDisp == false)
	{// 表示しない場合
		return;
	}

	for (int nCntUsedEnergy = 0; nCntUsedEnergy < USED_ENERGY_NUMPLACE; nCntUsedEnergy++)
	{
		if (m_apNumber[nCntUsedEnergy] != NULL)
		{// NULLチェック
			// 描画処理
			m_apNumber[nCntUsedEnergy]->Draw();
		}
	}
}

//========================================================================
// 使用エネルギー量設定処理
//========================================================================
void CUsedEnergy::SetNum(const int nUsedEnergy)
{
	// ローカル変数
	int aTexU[USED_ENERGY_NUMPLACE];				// 各桁の数字を格納

	// 使用エネルギー量を保存
	m_nUsedEnergy = nUsedEnergy;

	for (int nCntUsedEnergy = 0; nCntUsedEnergy < USED_ENERGY_NUMPLACE; nCntUsedEnergy++)
	{
		// 各桁の数字を設定
		aTexU[nCntUsedEnergy] = m_nUsedEnergy % (int)pow(POWER, USED_ENERGY_NUMPLACE - nCntUsedEnergy) / 
			(int)pow(POWER, USED_ENERGY_NUMPLACE - nCntUsedEnergy - 1);

		// テクスチャ座標を設定
		m_apNumber[nCntUsedEnergy]->SetTexUV(aTexU[nCntUsedEnergy], TEXTURE_WIDTH, 1);
	}
}

//========================================================================
// 使用エネルギー量加算処理
//========================================================================
void CUsedEnergy::Add(const int nAdd)
{
	// 使用エネルギー量を加算
	m_nUsedEnergy += nAdd;

	if (m_nUsedEnergy > MAX_SCORE)
	{// 使用エネルギー量の最大値
		m_nUsedEnergy = MAX_SCORE;
	}

	// 使用エネルギー量を設定
	SetNum(m_nUsedEnergy);
}

//========================================================================
// 使用エネルギー量減算処理
//========================================================================
void CUsedEnergy::Minus(const int nMinus)
{
	// 使用エネルギー量を減算
	m_nUsedEnergy -= nMinus;

	if (m_nUsedEnergy < MIN_SCORE)
	{// 使用エネルギー量の最低値
		m_nUsedEnergy = MIN_SCORE;
	}

	// 使用エネルギー量を設定
	SetNum(m_nUsedEnergy);
}