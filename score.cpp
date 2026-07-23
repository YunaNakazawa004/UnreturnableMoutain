//========================================================================
// 
// スコア [score.cpp]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "score.h"

#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "number.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define POWER				(10)									// 10の累乗
#define TEXTURE_WIDTH		(10)									// テクスチャの幅
#define MAX_SCORE			(99999999)								// 最大スコア
#define MIN_SCORE			(0)										// 最小スコア

//========================================================================
// スコアクラスの生成処理
//========================================================================
CScore* CScore::Create(const D3DXVECTOR3 pos,
	const float fWidth, const float fHeight)
{
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}

	CScore* pScore = NULL;

	if (pScore == NULL)
	{// NULLチェック
		// スコアの生成
		pScore = new CScore;
	}

	if (pScore != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pScore->Init(pos, fWidth, fHeight)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! スコアの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// オブジェクトの種類を設定
		pScore->SetType(TYPE_SCORE);

		return pScore;
	}

	OutputDebugStringA("! ! ! スコアの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// スコアクラスのコンストラクタ
//========================================================================
CScore::CScore(const int nPriority):CObject(nPriority)
{
	// スコアクラスの値をクリア
	memset(&m_apNumber[0], NULL, sizeof m_apNumber);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nScore = 0;
}

//========================================================================
// スコアクラスのデストラクタ
//========================================================================
CScore::~CScore()
{
}

//========================================================================
// スコアクラスの初期化処理
//========================================================================
HRESULT CScore::Init(const D3DXVECTOR3 pos,
	const float fWidth, const float fHeight)
{
	for (int nCntScore = 0; nCntScore < SCORE_NUMPLACE; nCntScore++)
	{
		if (m_apNumber[nCntScore] == NULL)
		{// NULLチェック
			m_apNumber[nCntScore] = CNumber::Create(D3DXVECTOR3(pos.x + (nCntScore * fWidth), pos.y, pos.z), fWidth, fHeight);
		}
	}

	// スコアクラスの値を初期化
	m_pos = pos;
	m_nScore = 0;

	// スコアを設定
	SetNum(m_nScore);

	return S_OK;
}

//========================================================================
// スコアクラスの終了処理
//========================================================================
void CScore::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < SCORE_NUMPLACE; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{// NULLチェック
			// 終了処理
			m_apNumber[nCntScore]->Uninit();

			delete m_apNumber[nCntScore];
			m_apNumber[nCntScore] = NULL;
		}
	}

	// 自分自身を破棄
	CObject::Release();
}

//========================================================================
// スコアクラスの更新処理
//========================================================================
void CScore::Update(void)
{
#ifdef _DEBUG
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();		// キーボード入力の取得

	// スコア上昇
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
// スコアクラスの描画処理
//========================================================================
void CScore::Draw(void)
{
	for (int nCntScore = 0; nCntScore < SCORE_NUMPLACE; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{// NULLチェック
			// 描画処理
			m_apNumber[nCntScore]->Draw();
		}
	}
}

//========================================================================
// スコア設定処理
//========================================================================
void CScore::SetNum(const int nScore)
{
	// ローカル変数
	int aTexU[SCORE_NUMPLACE];				// 各桁の数字を格納

	// スコアを保存
	m_nScore = nScore;

	for (int nCntScore = 0; nCntScore < SCORE_NUMPLACE; nCntScore++)
	{
		// 各桁の数字を設定
		aTexU[nCntScore] = m_nScore % (int)pow(POWER, SCORE_NUMPLACE - nCntScore) / (int)pow(POWER, SCORE_NUMPLACE - nCntScore - 1);

		// テクスチャ座標を設定
		m_apNumber[nCntScore]->SetTexUV(aTexU[nCntScore], TEXTURE_WIDTH, 1);
	}
}

//========================================================================
// スコア加算処理
//========================================================================
void CScore::Add(const int nAdd)
{
	// スコアを加算
	m_nScore += nAdd;

	if (m_nScore > MAX_SCORE)
	{// スコアの最大値
		m_nScore = MAX_SCORE;
	}

	// スコアを設定
	SetNum(m_nScore);
}

//========================================================================
// スコア減算処理
//========================================================================
void CScore::Minus(const int nMinus)
{
	// スコアを減算
	m_nScore -= nMinus;

	if (m_nScore < MIN_SCORE)
	{// スコアの最低値
		m_nScore = MIN_SCORE;
	}

	// スコアを設定
	SetNum(m_nScore);
}