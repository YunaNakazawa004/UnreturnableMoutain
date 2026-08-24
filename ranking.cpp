//=============================================================================
// 
// ランキング画面 [ ranking.cpp ]
// Author : Nakazawa Yuna
// 
//=============================================================================
#include "ranking.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"
#include "fade.h"
#include "sound.h"

#include "ship.h"
#include "lab.h"
#include "score.h"
#include "rankingscore.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_COUNT				(300)									// 自動的にタイトル画面に戻るまでのカウント
#define RANKING_POSV_CAMERA		(D3DXVECTOR3(0.0f, 100.0f, -200.0f))	// カメラの初期位置		
#define RANKING_POSR_CAMERA		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))			// カメラの初期注視点		
#define RANKING_HEIGHT_CAMERA	(100.0f)								// カメラの高さ
#define RANKING_DISTANCE		(200.0f)								// 視点と注視点間の距離
#define RANKING_PLAYER_FRONT	(0.0f)									// プレイヤーの前に注視点

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
CShip* CRanking::m_pShip = NULL;								// 船のインスタンス
CLab* CRanking::m_pLab = NULL;									// 研究所のインスタンス
CRankingScore* CRanking::m_pRankingScore = NULL;				// ランキングスコアのインスタンス
int CRanking::m_nUserScore = 0;									// 現在プレイヤーのスコア
int CRanking::m_nModeCounter = 0;								// 自動画面遷移のカウント

//========================================================================
// ランキング画面クラスのコンストラクタ
//========================================================================
CRanking::CRanking() : CScene(CScene::MODE_RANKING)
{
	// 値をクリア
	m_pShip = NULL;
	m_pLab = NULL;
	m_pRankingScore = NULL;
	m_nModeCounter = 0;
}

//========================================================================
// ランキング画面クラスのデストラクタ
//========================================================================
CRanking::~CRanking()
{
}

//=============================================================================
// ランキング画面の初期化処理
//=============================================================================
HRESULT CRanking::Init(void)
{
	CSound* pSound = CManager::GetSound();

	// サウンドの停止
	pSound->StopSound();

	// サウンドの開始
	pSound->PlaySound(CSound::SOUND_BGM_GAME);

	// カメラの設定
	CCamera* pCamera = CManager::GetCamera();
	pCamera->SetPosition(D3DXVECTOR3(0.0f, 30.0f, -400.0f), DEFAULT_VECTER3, DEFAULT_VECTER3, CCamera::TYPE_STOP);

	// ランキングスコアを生成
	if (m_pRankingScore == NULL)
	{// NULLチェック
		m_pRankingScore = CRankingScore::Create();

		if (m_pRankingScore == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! ランキングスコアの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		m_pRankingScore->LoadScore();

		if (m_nUserScore > 0)
		{// ユーザーのスコアが存在する
			m_pRankingScore->SaveScore(m_nUserScore);
		}

		m_pRankingScore->SetScore();
	}
	
	// 船を生成
	if (m_pShip == NULL)
	{// NULLチェック
		m_pShip = CShip::Create(D3DXVECTOR3(0.0f, 0.0f, -130.0f), DEFAULT_VECTER3);

		if (m_pShip == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! 船の生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		m_pShip->SetState(CShip::STATE_WAIT);
	}

	// 研究所を生成
	if (m_pLab == NULL)
	{// NULLチェック
		m_pLab = CLab::Create(D3DXVECTOR3(0.0f, 0.0f, -130.0f), DEFAULT_VECTER3);

		if (m_pLab == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! 研究所の生成に失敗しました ! ! !\n");

			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// ランキング画面の終了処理
//=============================================================================
void CRanking::Uninit(void)
{
	// サウンドの停止
	CSound* pSound = CManager::GetSound();
	pSound->StopSound();

	// テクスチャを破棄

	// 研究所の破棄
	if (m_pLab != NULL)
	{// NULLチェック
		m_pLab = NULL;
	}

	// 船の破棄
	if (m_pShip != NULL)
	{// NULLチェック
		m_pShip = NULL;
	}
	
	// ランキングスコアの破棄
	if (m_pRankingScore != NULL)
	{// NULLチェック
		m_pRankingScore = NULL;
	}

#ifdef ENABLE_INHERITANCE_COBJECT
	// 自分自身を破棄
	CObject::Release();
#endif
}

//=============================================================================
// ランキング画面の更新処理
//=============================================================================
void CRanking::Update(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();		// キーボード入力の取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();			// ジョイパッド入力の取得
	CFade* pFade = CManager::GetFade();									// フェードの取得

	m_nModeCounter++;

	// 画面遷移
	if ((pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
		pInputJoypad->GetTrigger(0, CInputJoypad::JOYKEY_A) == true ||
		pInputJoypad->GetTrigger(0, CInputJoypad::JOYKEY_START) == true || 
		m_nModeCounter >= TITLE_COUNT) && isFade() == true)
	{// ENTERが押された
		m_nModeCounter = 0;

		if (pFade != NULL)
		{// NULLチェック
			pFade->SetFade(MODE_TITLE);
		}
	}
}

//=============================================================================
// ランキング画面の描画処理
//=============================================================================
void CRanking::Draw(void)
{
}