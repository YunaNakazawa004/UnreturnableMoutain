//========================================================================
// 
// タイトル画面 [ title.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "title.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"
#include "fade.h"
#include "transition.h"

#include "UI_title_logo.h"
#include "UI_enter.h"
#include "player.h"
#include "ship.h"
#include "lab.h"
#include "UI_energy.h"
#include "UI_jump_meter.h"
#include "UI_action.h"

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
CTitleLogo* CTitle::m_pTitleLogo = NULL;			// タイトルロゴのインスタンス
CEnterUI* CTitle::m_pEnterUI = NULL;				// エンターUIのインスタンス
CPlayer* CTitle::m_pPlayer = NULL;					// プレイヤーのインスタンス
CShip* CTitle::m_pShip = NULL;						// 船のインスタンス
CLab* CTitle::m_pLab = NULL;						// 研究所のインスタンス
CEnergyUI* CTitle::m_pEnergyUI = NULL;				// エネルギーUIのインスタンス
CJumpMeterUI* CTitle::m_pJumpMeterUI = NULL;		// ジャンプメーターUIのインスタンス
CActionUI* CTitle::m_pActionUI = NULL;				// アクションUIのインスタンス
bool CTitle::m_bTutorial = false;					// チュートリアル中かどうかのフラグ

//========================================================================
// タイトル画面クラスのコンストラクタ
//========================================================================
CTitle::CTitle() : CScene(CScene::MODE_TITLE)
{
	// 値をクリア
	m_pTitleLogo = NULL;
	m_pEnterUI = NULL;
	m_pPlayer = NULL;
	m_pShip = NULL;
	m_pLab = NULL;
	m_pEnergyUI = NULL;
	m_pJumpMeterUI = NULL;
	m_pActionUI = NULL;
	m_bTutorial = false;
}

//========================================================================
// タイトル画面クラスのデストラクタ
//========================================================================
CTitle::~CTitle()
{
}

//========================================================================
// タイトル画面クラスの初期化処理
//========================================================================
HRESULT CTitle::Init(void)
{
	// カメラの設定
	CCamera* pCamera = CManager::GetCamera();
	pCamera->SetPosition(D3DXVECTOR3(0.0f, 30.0f, -200.0f), DEFAULT_VECTER3, DEFAULT_VECTER3, CCamera::TYPE_STOP);

	// テクスチャを読み込み
	CTitleLogo::Load();
	CEnterUI::Load();
	CEnergyUI::Load();
	CJumpMeterUI::Load();
	CActionUI::Load();

	// タイトルロゴを生成
	if (m_pTitleLogo == NULL)
	{// NULLチェック
		m_pTitleLogo = CTitleLogo::Create(D3DXVECTOR3(960.0f, 170.0f, 0.0f), 300.0f, 150.0f);

		if (m_pTitleLogo == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! タイトルロゴの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		m_pTitleLogo->SetDisp(true);
	}

	// エンターUIを生成
	if (m_pEnterUI == NULL)
	{// NULLチェック
		m_pEnterUI = CEnterUI::Create(D3DXVECTOR3(640.0f, 480.0f, 0.0f), 450.0f, 50.0f);

		if (m_pEnterUI == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! エンターUIの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		m_pEnterUI->SetDisp(true);
	}

	// エネルギーUIを生成
	if (m_pEnergyUI == NULL)
	{// NULLチェック
		m_pEnergyUI = CEnergyUI::Create(D3DXVECTOR3(1180.0f, 70.0f, 0.0f), 100.0f, 50.0f);

		if (m_pEnergyUI == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! エネルギーUIの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		m_pEnergyUI->SetDisp(false);
		m_pEnergyUI->SetState(0);
	}

	// ジャンプメーターUIを生成
	if (m_pJumpMeterUI == NULL)
	{// NULLチェック
		m_pJumpMeterUI = CJumpMeterUI::Create(D3DXVECTOR3(680.0f, 340.0f, 0.0f), 5.0f, 30.0f, 100.0f);

		if (m_pJumpMeterUI == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! ジャンプメーターUIの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		m_pJumpMeterUI->SetDisp(false);
	}

	// アクションUIを生成
	if (m_pActionUI == NULL)
	{// NULLチェック
		m_pActionUI = CActionUI::Create(D3DXVECTOR3(580.0f, 300.0f, 0.0f), 20.0f, 20.0f);

		if (m_pActionUI == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! アクションUIの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		m_pActionUI->SetDisp(false);
	}

	// プレイヤーを生成
	if (m_pPlayer == NULL)
	{// NULLチェック
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 7.0f, -130.0f), DEFAULT_VECTER3, 40.0f);

		if (m_pPlayer == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! プレイヤーの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		m_pPlayer->SetState(CPlayer::STATE_NONE);
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
// タイトル画面クラスの終了処理
//========================================================================
void CTitle::Uninit(void)
{
	// テクスチャを破棄
	CActionUI::Unload();
	CJumpMeterUI::Unload();
	CEnergyUI::Unload();
	CEnterUI::Unload();
	CTitleLogo::Unload();

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

	// プレイヤーの破棄
	if (m_pPlayer != NULL)
	{// NULLチェック
		m_pPlayer = NULL;
	}

	// アクションUIの破棄
	if (m_pActionUI != NULL)
	{// NULLチェック
		m_pActionUI = NULL;
	}

	// ジャンプメーターUIの破棄
	if (m_pJumpMeterUI != NULL)
	{// NULLチェック
		m_pJumpMeterUI = NULL;
	}

	// エネルギーUIの破棄
	if (m_pEnergyUI != NULL)
	{// NULLチェック
		m_pEnergyUI = NULL;
	}

	// エンターUIの破棄
	if (m_pEnterUI != NULL)
	{// NULLチェック
		m_pEnterUI = NULL;
	}

	// タイトルロゴの破棄
	if (m_pTitleLogo != NULL)
	{// NULLチェック
		m_pTitleLogo = NULL;
	}

#ifdef ENABLE_INHERITANCE_COBJECT
	// 自分自身を破棄
	CObject::Release();
#endif
}

//========================================================================
// タイトル画面クラスの更新処理
//========================================================================
void CTitle::Update(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();		// キーボード入力の取得
	CTransition* pTransition = CManager::GetTransition();				// 画面遷移の取得
	CCamera* pCamera = CManager::GetCamera();							// カメラの取得

	if (m_bTutorial == false && m_pPlayer->GetState() == CPlayer::STATE_TUTORIAL)
	{// チュートリアル中
		// チュートリアル状態に設定
		m_pEnergyUI->SetDisp(true);
		m_pEnterUI->SetDisp(false);
		m_pJumpMeterUI->SetDisp(true);
		m_pTitleLogo->SetDisp(false);
		m_pActionUI->SetDisp(true);
		pCamera->SetType(CCamera::TYPE_PLAYER);
		m_pShip->SetState(CShip::STATE_READY);

		// フラグON
		m_bTutorial = true;
	}

	// 画面遷移
	if (isFade() == true)
	{// ENTERが押された
		if (pTransition != NULL)
		{// NULLチェック
			pTransition->SetTransition(MODE_GAME);
		}

		return;
	}
}

//========================================================================
// タイトル画面クラスの描画処理
//========================================================================
void CTitle::Draw(void)
{
}