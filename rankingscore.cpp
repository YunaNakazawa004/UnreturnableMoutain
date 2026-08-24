//=============================================================================
// 
// ランキングスコア [ rankingscore.cpp ]
// Author : Nakazawa Yuna
// 
//=============================================================================
#include "rankingscore.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "sound.h"
#include "scene.h"

#include "score.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RANKINGSCORE_WIDTH	(40.0f)									// ランキングスコアの幅
#define RANKINGSCORE_HEIGHT	(75.0f)									// ランキングスコアの高さ
#define FIRST_SCORE			(1000000)								// 初期スコア
#define FIRST_SCORE_DIFF	(150000)								// 初期スコアの差
#define SCORE_POS_X			(471.0f)								// スコア全体の基準位置X座標
#define SCORE_POS_X_FIRST	(430.0f)								// スコア全体最初の基準位置X座標
#define SCORE_POS_X_MOVE	(200.0f)								// スコアそれぞれずらすX座標
#define SCORE_POS_Y			(120.0f)								// スコア全体の基準位置Y座標
#define SCORE_LINES			(17.0f)									// スコアとスコアの行間
#define SCORE_MOVE			(D3DXVECTOR3(10.0f, 0.0f, 0.0f))		// スコアが動く速さ
#define TEXTURE_WIDTH		(0.1f)									// テクスチャの幅
#define BLINK_COUNT			(3)										// 点滅の間隔
#define RANKING_FILE		"data\\ranking.bin"						// ランキングのファイル

//========================================================================
// ランキングスコアクラスの生成処理
//========================================================================
CRankingScore* CRankingScore::Create(void)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CRankingScore* pRankingScore = NULL;

	if (pRankingScore == NULL)
	{// NULLチェック
		// ランキングスコアの生成
		pRankingScore = new CRankingScore;
	}

	if (pRankingScore != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pRankingScore->Init()))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! ランキングスコアの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// オブジェクトの種類を設定
		pRankingScore->SetType(TYPE_RANKINGSCORE);

		return pRankingScore;
	}

	OutputDebugStringA("! ! ! ランキングスコアの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// ランキングスコアクラスのコンストラクタ
//========================================================================
CRankingScore::CRankingScore() : CObject(PRIORITY_6)
{
	// 値をクリア
	memset(&m_apScore[0], NULL, sizeof m_apScore);
	memset(&m_aScore[0], 0, sizeof m_aScore);
	m_bSave = true;
	m_nUpdateScore = -1;
}

//========================================================================
// ランキングスコアクラスのデストラクタ
//========================================================================
CRankingScore::~CRankingScore()
{
}

//=============================================================================
// ランキングスコアの初期化処理
//=============================================================================
HRESULT CRankingScore::Init(void)
{
	for (int nCount = 0; nCount < MAX_RANKING; nCount++)
	{
		if (m_apScore[nCount] == NULL)
		{// NULLチェック
			m_apScore[nCount] = CScore::Create(D3DXVECTOR3(SCORE_POS_X + SCORE_POS_X_FIRST + (SCORE_POS_X_MOVE * (MAX_RANKING - nCount)), SCORE_POS_Y + (nCount * (SCORE_LINES + RANKINGSCORE_HEIGHT)), 0.0f), RANKINGSCORE_WIDTH, RANKINGSCORE_HEIGHT);
			
			if (m_apScore[nCount] != NULL)
			{// NULLチェック
				m_apScore[nCount]->SetNum(0);
			}
		}
	}

	m_bSave = true;

	return S_OK;
}

//=============================================================================
// ランキングスコアの終了処理
//=============================================================================
void CRankingScore::Uninit(void)
{
	m_nUpdateScore = -1;

	for (int nCount = 0; nCount < MAX_RANKING; nCount++)
	{
		if (m_apScore[nCount] != NULL)
		{// NULLチェック
			m_apScore[nCount] = NULL;
		}
	}
}

//=============================================================================
// ランキングスコアの更新処理
//=============================================================================
void CRankingScore::Update(void)
{
	// ローカル変数
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();		// キーボード入力の取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();			// ジョイパッド入力の取得
	CSound* pSound = CManager::GetSound();								// サウンドを取得
	static int nUpdateCounter = 0;
	nUpdateCounter++;

	for (int nCount = 0; nCount < MAX_RANKING; nCount++)
	{
		if (m_apScore[nCount] != NULL)
		{// NULLチェック
			D3DXVECTOR3 pos = m_apScore[nCount]->GetPosition();

			if (pos.x > SCORE_POS_X)
			{// 既定の位置についてない場合動かす
				pos.x -= SCORE_MOVE.x;

				if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || 
					pInputJoypad->GetTrigger(0, CInputJoypad::JOYKEY_A) == true || 
					pInputJoypad->GetTrigger(0, CInputJoypad::JOYKEY_START) == true)
				{// 途中でキーを押すとスキップ
					pos.x = SCORE_POS_X;
					pSound->PlaySound(CSound::SE_ENTER);

					// 振動
					pInputJoypad->SetVibration(0, 2000, 3000, 10);
				}
			}
			else
			{// 規定の位置についたら止める
				pos.x = SCORE_POS_X;

				if (nCount == MAX_RANKING - 1)
				{// 最後のスコアが規定の位置についた
					CScene::SetFadeEnable();
				}
			}

			m_apScore[nCount]->SetPosition(pos);

			if (m_nUpdateScore == nCount && nUpdateCounter % BLINK_COUNT == 0)
			{
				static D3DXCOLOR col = COLOR_WHITE;
				col = (col == COLOR_WHITE) ? COLOR_YELLOW : COLOR_WHITE;

				m_apScore[nCount]->SetColor(col);
			}
		}
	}
}

//=============================================================================
// ランキングスコアの描画処理
//=============================================================================
void CRankingScore::Draw(void)
{
}

//=============================================================================
// ランキングスコアの設定処理
//=============================================================================
void CRankingScore::SetScore(void)
{
	for (int nCount = 0; nCount < MAX_RANKING; nCount++)
	{
		if (m_apScore[nCount] != NULL)
		{// NULLチェック
			m_apScore[nCount]->SetNum(m_aScore[nCount]);
		}
	}
}

//=============================================================================
// ランキングスコア更新の取得
//=============================================================================
int CRankingScore::GetUpdate(void)
{
	return m_nUpdateScore;
}

//=============================================================================
// ランキングスコアの書き込み処理
//=============================================================================
void CRankingScore::SaveScore(const int nScore)
{
	if (m_bSave == true)
	{
		FILE* pFile;

		pFile = fopen(RANKING_FILE, "wb");

		if (pFile != NULL)
		{// ファイルが開けた場合
			m_aScore[MAX_RANKING] = nScore;
			qsort(&m_aScore[0], MAX_RANKING + 1, sizeof(int), compare);

			fwrite(&m_aScore[0], sizeof(int), MAX_RANKING, pFile);

			fclose(pFile);
		}
		else
		{// ファイルが開けなかった場合
			printf("ファイルを開けませんでした");
		}

		for (int nCount = 0; nCount < MAX_RANKING; nCount++)
		{
			if (m_aScore[nCount] == nScore && m_nUpdateScore == -1 && nScore != FIRST_SCORE)
			{// ランキングが更新されたら、該当のランキングを保存する
				m_nUpdateScore = nCount;
			}
		}

		m_bSave = false;
	}
}

//=============================================================================
// ランキングスコアの読み込み処理
//=============================================================================
void CRankingScore::LoadScore(void)
{
	FILE* pFile;

	pFile = fopen(RANKING_FILE, "rb");

	if (pFile != NULL)
	{// ファイルが開けた場合
		fread(&m_aScore[0], sizeof(int), MAX_RANKING, pFile);

		fclose(pFile);
	}
	else
	{// ファイルが開けなかった場合
		for (int nCount = 0; nCount < MAX_RANKING; nCount++)
		{
			m_aScore[nCount] = FIRST_SCORE + (nCount * FIRST_SCORE_DIFF);
		}

		SaveScore(FIRST_SCORE);
	}

	m_bSave = true;
}

//=============================================================================
// 比較関数
//=============================================================================
int CRankingScore::compare(const void* data1, const void* data2)
{
	return (*(int*)data2 - *(int*)data1);
}