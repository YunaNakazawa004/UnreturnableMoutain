//========================================================================
// 
// 収集数 [ collect_num.cpp]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "collect_num.h"

#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "number.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define POWER				(10)									// 10の累乗
#define TEXTURE_WIDTH		(10)									// テクスチャの幅
#define MAX_SCORE			(9)										// 最大収集数
#define MIN_SCORE			(0)										// 最小収集数

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
int CCollectNum::m_nCollectNum = 0;			// 収集数

//========================================================================
// 収集数クラスの生成処理
//========================================================================
CCollectNum* CCollectNum::Create(const D3DXVECTOR3 pos,
	const float fWidth, const float fHeight)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CCollectNum* pCollectNum = NULL;

	if (pCollectNum == NULL)
	{// NULLチェック
		// 収集数の生成
		pCollectNum = new CCollectNum;
	}

	if (pCollectNum != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pCollectNum->Init(pos, fWidth, fHeight)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! 収集数の初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// オブジェクトの種類を設定
		pCollectNum->SetType(TYPE_SCORE);

		return pCollectNum;
	}

	OutputDebugStringA("! ! ! 収集数の生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// 収集数クラスのコンストラクタ
//========================================================================
CCollectNum::CCollectNum(const int nPriority):CObject(nPriority)
{
	// 収集数クラスの値をクリア
	memset(&m_apNumber[0], NULL, sizeof m_apNumber);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//========================================================================
// 収集数クラスのデストラクタ
//========================================================================
CCollectNum::~CCollectNum()
{
}

//========================================================================
// 収集数クラスの初期化処理
//========================================================================
HRESULT CCollectNum::Init(const D3DXVECTOR3 pos,
	const float fWidth, const float fHeight)
{
	for (int nCntCollectNum = 0; nCntCollectNum < COLLECT_NUM_NUMPLACE; nCntCollectNum++)
	{
		if (m_apNumber[nCntCollectNum] == NULL)
		{// NULLチェック
			m_apNumber[nCntCollectNum] = CNumber::Create(D3DXVECTOR3(pos.x + (nCntCollectNum * fWidth), pos.y, pos.z), fWidth, fHeight);
		}
	}

	// 収集数クラスの値を初期化
	m_pos = pos;

	// 収集数を設定
	SetNum(m_nCollectNum);

	return S_OK;
}

//========================================================================
// 収集数クラスの終了処理
//========================================================================
void CCollectNum::Uninit(void)
{
	for (int nCntCollectNum = 0; nCntCollectNum < COLLECT_NUM_NUMPLACE; nCntCollectNum++)
	{
		if (m_apNumber[nCntCollectNum] != NULL)
		{// NULLチェック
			// 終了処理
			m_apNumber[nCntCollectNum]->Uninit();

			delete m_apNumber[nCntCollectNum];
			m_apNumber[nCntCollectNum] = NULL;
		}
	}

	// 自分自身を破棄
	CObject::Release();
}

//========================================================================
// 収集数クラスの更新処理
//========================================================================
void CCollectNum::Update(void)
{
#ifdef _DEBUG
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();		// キーボード入力の取得

	// 収集数上昇
	if (pInputKeyboard->GetPress(DIK_1) == true)
	{
		Add((int)pow(POWER, 7));
	}
	if (pInputKeyboard->GetPress(DIK_2) == true)
	{
		Add((int)pow(POWER, 6));
	}
	if (pInputKeyboard->GetPress(DIK_3) == true)
	{
		Add((int)pow(POWER, 5));
	}
	if (pInputKeyboard->GetPress(DIK_4) == true)
	{
		Add((int)pow(POWER, 4));
	}
	if (pInputKeyboard->GetPress(DIK_5) == true)
	{
		Add((int)pow(POWER, 3));
	}
	if (pInputKeyboard->GetPress(DIK_6) == true)
	{
		Add((int)pow(POWER, 2));
	}
	if (pInputKeyboard->GetPress(DIK_7) == true)
	{
		Add((int)pow(POWER, 1));
	}
	if (pInputKeyboard->GetPress(DIK_8) == true)
	{
		Add((int)pow(POWER, 0));
	}
#endif
}

//========================================================================
// 収集数クラスの描画処理
//========================================================================
void CCollectNum::Draw(void)
{
	if (m_bDisp == false)
	{// 表示しない場合
		return;
	}

	for (int nCntCollectNum = 0; nCntCollectNum < COLLECT_NUM_NUMPLACE; nCntCollectNum++)
	{
		if (m_apNumber[nCntCollectNum] != NULL)
		{// NULLチェック
			// 描画処理
			m_apNumber[nCntCollectNum]->Draw();
		}
	}
}

//========================================================================
// 収集数設定処理
//========================================================================
void CCollectNum::SetNum(const int nCollectNum)
{
	// ローカル変数
	int aTexU[COLLECT_NUM_NUMPLACE];				// 各桁の数字を格納

	// 収集数を保存
	m_nCollectNum = nCollectNum;

	for (int nCntCollectNum = 0; nCntCollectNum < COLLECT_NUM_NUMPLACE; nCntCollectNum++)
	{
		// 各桁の数字を設定
		aTexU[nCntCollectNum] = m_nCollectNum % (int)pow(POWER, COLLECT_NUM_NUMPLACE - nCntCollectNum) / 
			(int)pow(POWER, COLLECT_NUM_NUMPLACE - nCntCollectNum - 1);

		// テクスチャ座標を設定
		m_apNumber[nCntCollectNum]->SetTexUV(aTexU[nCntCollectNum], TEXTURE_WIDTH, 1);
	}
}

//========================================================================
// 収集数加算処理
//========================================================================
void CCollectNum::Add(const int nAdd)
{
	// 収集数を加算
	m_nCollectNum += nAdd;

	if (m_nCollectNum > MAX_SCORE)
	{// 収集数の最大値
		m_nCollectNum = MAX_SCORE;
	}

	// 収集数を設定
	SetNum(m_nCollectNum);
}

//========================================================================
// 収集数減算処理
//========================================================================
void CCollectNum::Minus(const int nMinus)
{
	// 収集数を減算
	m_nCollectNum -= nMinus;

	if (m_nCollectNum < MIN_SCORE)
	{// 収集数の最低値
		m_nCollectNum = MIN_SCORE;
	}

	// 収集数を設定
	SetNum(m_nCollectNum);
}