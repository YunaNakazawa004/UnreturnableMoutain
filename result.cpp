//========================================================================
// 
// リザルト画面 [ result.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "result.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"
#include "fade.h"

#include "UI_result.h"
#include "ship.h"
#include "lab.h"
#include "used_energy.h"
#include "score.h"
#include "collect_num.h"

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
CResultUI* CResult::m_pResultUI = NULL;						// リザルトUIのインスタンス
CShip* CResult::m_pShip = NULL;								// 船のインスタンス
CLab* CResult::m_pLab = NULL;								// 研究所のインスタンス
CUsedEnergy* CResult::m_pUsedEnergy = NULL;					// 使用エネルギー量のインスタンス
CScore* CResult::m_pBaseScore = NULL;						// 基礎スコアのインスタンス
CCollectNum* CResult::m_pCollectNum = NULL;					// 収集数のインスタンス
CScore* CResult::m_pCollectScore = NULL;					// 収集スコアのインスタンス
CScore* CResult::m_pClearScore = NULL;						// クリアボーナスのインスタンス
CScore* CResult::m_pFinalScore = NULL;						// 最終スコアのインスタンス
bool CResult::m_bClear = false;								// クリアフラグ

//========================================================================
// リザルト画面クラスのコンストラクタ
//========================================================================
CResult::CResult() : CScene(CScene::MODE_RESULT)
{
	// 値をクリア
	m_pResultUI = NULL;
	m_pShip = NULL;
	m_pLab = NULL;
	m_pUsedEnergy = NULL;
	m_pBaseScore = NULL;
	m_pCollectNum = NULL;
	m_pCollectScore = NULL;
	m_pClearScore = NULL;
	m_pFinalScore = NULL;
}

//========================================================================
// リザルト画面クラスのデストラクタ
//========================================================================
CResult::~CResult()
{
}

//========================================================================
// リザルト画面クラスの初期化処理
//========================================================================
HRESULT CResult::Init(void)
{
	// カメラの設定
	CCamera *pCamera = CManager::GetCamera();
	pCamera->SetPosition(D3DXVECTOR3(0.0f, 30.0f, -400.0f), DEFAULT_VECTER3, DEFAULT_VECTER3, CCamera::TYPE_STOP);

	// テクスチャを読み込み
	CResultUI::Load();

	// リザルトUIを生成
	if (m_pResultUI == NULL)
	{// NULLチェック
		m_pResultUI = CResultUI::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f), 300.0f, 60.0f,
			(m_bClear == true) ? CResultUI::TYPE_CLEAR : CResultUI::TYPE_GAMEOVER);

		if (m_pResultUI == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! リザルトUIの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}
	}

	// 最終スコアを生成
	if (m_pFinalScore == NULL)
	{// NULLチェック
		m_pFinalScore = CScore::Create(D3DXVECTOR3(440.0f, 540.0f, 0.0f), 50.0f, 100.0f);

		if (m_pFinalScore == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! 最終スコアの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		m_pFinalScore->SetDisp(false);
		m_pFinalScore->SetNum(0);
	}

	// 基礎スコアを生成
	if (m_pBaseScore == NULL)
	{// NULLチェック
		m_pBaseScore = CScore::Create(D3DXVECTOR3(640.0f, 160.0f, 0.0f), 36.0f, 72.0f);

		if (m_pBaseScore == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! 基礎スコアの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		m_pBaseScore->SetDisp(false);
		m_pBaseScore->SetNum(1000000);
		m_pFinalScore->Add(1000000);
	}
	
	// 使用エネルギー量を生成
	if (m_pUsedEnergy == NULL)
	{// NULLチェック
		m_pUsedEnergy = CUsedEnergy::Create(D3DXVECTOR3(640.0f, 240.0f, 0.0f), 36.0f, 72.0f);

		if (m_pUsedEnergy == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! 使用エネルギー量の生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		m_pUsedEnergy->SetDisp(false);
		m_pFinalScore->Minus(CUsedEnergy::GetUsedEnergy());
	}
	
	// 収集数を生成
	if (m_pCollectNum == NULL)
	{// NULLチェック
		m_pCollectNum = CCollectNum::Create(D3DXVECTOR3(520.0f, 320.0f, 0.0f), 36.0f, 72.0f);

		if (m_pCollectNum == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! 収集数の生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		m_pCollectNum->SetDisp(false);
	}
	
	// 収集スコアを生成
	if (m_pCollectScore == NULL)
	{// NULLチェック
		m_pCollectScore = CScore::Create(D3DXVECTOR3(640.0f, 320.0f, 0.0f), 36.0f, 72.0f);

		if (m_pCollectScore == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! 収集スコアの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		m_pCollectScore->SetDisp(false);
		m_pCollectScore->SetNum(20000 * CCollectNum::GetCollectNum());
		m_pFinalScore->Add(20000 * CCollectNum::GetCollectNum());
	}

	if (m_bClear == true)
	{// クリアした場合
		// クリアボーナスを生成
		if (m_pClearScore == NULL)
		{// NULLチェック
			m_pClearScore = CScore::Create(D3DXVECTOR3(640.0f, 400.0f, 0.0f), 36.0f, 72.0f);

			if (m_pClearScore == NULL)
			{// NULLチェック
				OutputDebugStringA("! ! ! クリアボーナスの生成に失敗しました ! ! !\n");

				return E_FAIL;
			}

			m_pClearScore->SetDisp(false);
			m_pClearScore->SetNum(500000);
			m_pFinalScore->Add(500000);
		}
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

//========================================================================
// リザルト画面クラスの終了処理
//========================================================================
void CResult::Uninit(void)
{
	// テクスチャを破棄
	CResultUI::Unload();

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

	// クリアボーナスの破棄
	if (m_pClearScore != NULL)
	{// NULLチェック
		m_pClearScore = NULL;
	}
	
	// 収集スコアの破棄
	if (m_pCollectScore != NULL)
	{// NULLチェック
		m_pCollectScore = NULL;
	}
	
	// 収集数の破棄
	if (m_pCollectNum != NULL)
	{// NULLチェック
		m_pCollectNum = NULL;
	}
	
	// 使用エネルギー量の破棄
	if (m_pUsedEnergy != NULL)
	{// NULLチェック
		m_pUsedEnergy = NULL;
	}
	
	// 基礎スコアの破棄
	if (m_pBaseScore != NULL)
	{// NULLチェック
		m_pBaseScore = NULL;
	}
	
	// 最終スコアの破棄
	if (m_pFinalScore != NULL)
	{// NULLチェック
		m_pFinalScore = NULL;
	}
	
	// リザルトUIの破棄
	if (m_pResultUI != NULL)
	{// NULLチェック
		m_pResultUI = NULL;
	}
	
	// クリアフラグを戻しておく
	m_bClear = false;

#ifdef ENABLE_INHERITANCE_COBJECT
	// 自分自身を破棄
	CObject::Release();
#endif
}

//========================================================================
// リザルト画面クラスの更新処理
//========================================================================
void CResult::Update(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();		// キーボード入力の取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();			// ジョイパッド入力の取得
	CFade* pFade = CManager::GetFade();									// フェードの取得

	// 画面遷移
	if ((pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
		pInputJoypad->GetTrigger(0, CInputJoypad::JOYKEY_A) == true ||
		pInputJoypad->GetTrigger(0, CInputJoypad::JOYKEY_START) == true) && isFade() == true)
	{// ENTERが押された
		if (pFade != NULL)
		{// NULLチェック
			pFade->SetFade(MODE_TITLE);
		}
	}
}

//========================================================================
// リザルト画面クラスの描画処理
//========================================================================
void CResult::Draw(void)
{
}