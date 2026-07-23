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
#include "UI_energy.h"
#include "UI_jump_meter.h"

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
CTitleLogo* CTitle::m_pTitleLogo = NULL;			// タイトルロゴのインスタンス
CEnterUI* CTitle::m_pEnterUI = NULL;				// エンターUIのインスタンス
CPlayer* CTitle::m_pPlayer = NULL;					// プレイヤーのインスタンス
CShip* CTitle::m_pShip = NULL;						// 船のインスタンス
CEnergyUI* CTitle::m_pEnergyUI = NULL;				// エネルギーUIのインスタンス
CJumpMeterUI* CTitle::m_pJumpMeterUI = NULL;		// ジャンプメーターUIのインスタンス

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
	m_pEnergyUI = NULL;
	m_pJumpMeterUI = NULL;
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
	CCamera *pCamera = CManager::GetCamera();
	pCamera->SetType(CCamera::TYPE_STOP);

	// テクスチャを読み込み
	CTitleLogo::Load();
	CEnterUI::Load();
	CEnergyUI::Load();
	CJumpMeterUI::Load();

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
		m_pEnterUI = CEnterUI::Create(D3DXVECTOR3(640.0f, 480.0f, 0.0f), 300.0f, 50.0f);

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

	// プレイヤーを生成
	if (m_pPlayer == NULL)
	{// NULLチェック
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, -15.0f, -130.0f), DEFAULT_VECTER3);

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
		m_pShip = CShip::Create(D3DXVECTOR3(0.0f,-15.0f,-130.0f), DEFAULT_VECTER3);

		if (m_pShip == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! 船の生成に失敗しました ! ! !\n");

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
	CJumpMeterUI::Unload();
	CEnergyUI::Unload();
	CEnterUI::Unload();
	CTitleLogo::Unload();

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

	if (m_pPlayer->GetState() == CPlayer::STATE_TUTORIAL)
	{// チュートリアル中
		m_pEnergyUI->SetDisp(true);
		m_pEnterUI->SetDisp(false);
		m_pJumpMeterUI->SetDisp(true);
		m_pTitleLogo->SetDisp(false);
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