//========================================================================
// 
// ゲーム画面 [ game.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "game.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"
#include "camera.h"
#include "pause.h"
#include "fade.h"
#include "transition.h"

#include "cloud.h"
#include "mountain.h"
#include "beach.h"
#include "watersurface.h"
#include "grass.h"
#include "map_object.h"
#include "player.h"
#include "ship.h"
#include "energyrock.h"
#include "UI_energy.h"
#include "UI_jump_meter.h"
#include "UI_item.h"
#include "score.h"
#include "UI_action.h"

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
CPause* CGame::m_pPause = NULL;						// ポーズのインスタンス
CPlayer* CGame::m_pPlayer = NULL;					// プレイヤーのインスタンス
CShip* CGame::m_pShip = NULL;						// 船のインスタンス
CMountain* CGame::m_pMountain = NULL;				// 山のインスタンス
CBeach* CGame::m_pBeach = NULL;						// 砂浜のインスタンス
CWaterSurface* CGame::m_pWaterSurface = NULL;		// 海のインスタンス
CMapObject* CGame::m_pMapObject = NULL;				// マップオブジェクトのインスタンス
CEnergyUI* CGame::m_pEnergyUI = NULL;				// エネルギーUIのインスタンス
CJumpMeterUI* CGame::m_pJumpMeterUI = NULL;			// ジャンプメーターUIのインスタンス
CItemUI* CGame::m_pItemUI = NULL;					// アイテムUIのインスタンス
CScore* CGame::m_pScore = NULL;						// スコアのインスタンス
CActionUI* CGame::m_pActionUI = NULL;				// アクションUIのインスタンス

//========================================================================
// ゲーム画面クラスのコンストラクタ
//========================================================================
CGame::CGame() : CScene(CScene::MODE_GAME)
{
	// 値をクリア
	m_pPause = NULL;
	m_pPlayer = NULL;
	m_pShip = NULL;
	m_pMountain = NULL;
	m_pBeach = NULL;
	m_pWaterSurface = NULL;
	m_pMapObject = NULL;
	m_pEnergyUI = NULL;
	m_pJumpMeterUI = NULL;
	m_pItemUI = NULL;
	m_pScore = NULL;
	m_pActionUI = NULL;
}

//========================================================================
// ゲーム画面クラスのデストラクタ
//========================================================================
CGame::~CGame()
{
}

//========================================================================
// ゲーム画面クラスの初期化処理
//========================================================================
HRESULT CGame::Init(void)
{
	// カメラの設定
	CCamera* pCamera = CManager::GetCamera();

	CSound* pSound = CManager::GetSound();		// サウンドを取得

	// テクスチャを読み込み
	CEnergyUI::Load();
	CJumpMeterUI::Load();
	CItemUI::Load();
	CMountain::Load();
	CBeach::Load();
	CWaterSurface::Load();
	CGrass::Load();
	CActionUI::Load();
	CCloud::Load();

	// ポーズを生成
	if (m_pPause == NULL)
	{// NULLチェック
		m_pPause = CPause::Create(CPause::MENU_CONTINUE);

		if (m_pPause == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! ポーズの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}
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

		m_pEnergyUI->SetState(0);
		m_pEnergyUI->SetDisp(false);
	}

	// ジャンプメーターUIを生成
	if (m_pJumpMeterUI == NULL)
	{// NULLチェック
		m_pJumpMeterUI = CJumpMeterUI::Create(D3DXVECTOR3(200.0f, 400.0f, 0.0f), 200.0f, 200.0f, 100.0f);

		if (m_pJumpMeterUI == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! ジャンプメーターUIの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		m_pJumpMeterUI->SetDispAll(false);
	}

	// アイテムUIを生成
	if (m_pItemUI == NULL)
	{// NULLチェック
		m_pItemUI = CItemUI::Create(D3DXVECTOR3(360.0f, 640.0f, 0.0f), 310.0f, 40.0f);

		if (m_pItemUI == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! アイテムUIの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		m_pItemUI->SetDispAll(false);
	}

	// スコアを生成
	if (m_pScore == NULL)
	{// NULLチェック
		m_pScore = CScore::Create(D3DXVECTOR3(500.0f, 560.0f, 0.0f), SCORE_WIDTH, SCORE_HEIGHT);

		if (m_pScore == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! スコアの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		m_pScore->SetNum(1000000);
		m_pScore->SetDisp(false);
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

	CCloud::Create(DEFAULT_VECTER3, DEFAULT_VECTER3, D3DXVECTOR2(16.0f, 8.0f), 10000.0f);

	// 山を生成
	if (m_pMountain == NULL)
	{// NULLチェック
		m_pMountain = CMountain::Create(DEFAULT_VECTER3, DEFAULT_VECTER3, D3DXVECTOR2(32.0f, 32.0f),
			D3DXVECTOR2(10.0f, 10.0f));

		if (m_pMountain == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! 山の生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		// ステージのデータを読み込む
		if (FAILED(m_pMountain->ReadData("data\\stage.bin")))
		{// もし失敗したら
			return E_FAIL;
		}
	}

	// 砂浜を生成
	if (m_pBeach == NULL)
	{// NULLチェック
		m_pBeach = CBeach::Create(DEFAULT_VECTER3, DEFAULT_VECTER3, D3DXVECTOR2(32.0f, 32.0f),
			D3DXVECTOR2(10.0f, 10.0f));

		if (m_pBeach == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! 砂浜の生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		// ステージのデータを読み込む
		if (FAILED(m_pBeach->ReadData("data\\Outstage.bin")))
		{// もし失敗したら
			return E_FAIL;
		}
	}

	// 海を生成
	if (m_pWaterSurface == NULL)
	{// NULLチェック
		m_pWaterSurface = CWaterSurface::Create(D3DXVECTOR3(0.0f, 30.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(150.0f, 150.0f),
			D3DXVECTOR2(100.0f, 100.0f));

		if (m_pWaterSurface == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! 海の生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		// ステージのデータを読み込む
		if (FAILED(m_pWaterSurface->ReadData("data\\watersurface.bin")))
		{// もし失敗したら
			return E_FAIL;
		}
	}

	// マップオブジェクトの生成
	if (SUCCEEDED(CManager::CreateInstance(&m_pMapObject)))
	{// マップオブジェクトの生成に成功
		// 初期化処理
		if (FAILED(m_pMapObject->Init()))
		{// 初期化処理が失敗した場合
			OutputDebugStringA("! ! ! マップオブジェクトの初期化に失敗しました ! ! !\n");

			return E_FAIL;
		}

		// マップオブジェクトのデータを読み込む
		if (FAILED(m_pMapObject->ReadData("data\\map_object.bin")))
		{// もし失敗したら
			return E_FAIL;
		}
	}

	// 船とプレイヤーの生成位置
	D3DXVECTOR3 pos, rot;
	rot.x = 0.0f;
	rot.y = ((float)(rand() % 629 - 314) / 100.0f);
	rot.z = 0.0f;
	pos.x = sinf(rot.y) * 2000.0f;
	pos.y = 40.0f;
	pos.z = cosf(rot.y) * 2000.0f;

	// プレイヤーを生成
	if (m_pPlayer == NULL)
	{// NULLチェック
		m_pPlayer = CPlayer::Create(pos, rot, 80.0f);

		if (m_pPlayer == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! プレイヤーの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		m_pPlayer->SetState(CPlayer::STATE_NONE);
		m_pPlayer->SetDisp(false);
	}

	// 船を生成
	if (m_pShip == NULL)
	{// NULLチェック
		m_pShip = CShip::Create(D3DXVECTOR3(pos.x, pos.y + 1000.0f, pos.z), rot);

		if (m_pShip == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! 船の生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		m_pShip->SetState(CShip::STATE_DOWN);
		pSound->PlaySound(CSound::SE_ROCKET);

		pCamera->SetPosition(D3DXVECTOR3(pos.x, pos.y + 5000.0f, pos.z - 400.0f), D3DXVECTOR3(pos.x, pos.y + 200.0f, pos.z), rot, CCamera::TYPE_STOP);
	}

	return S_OK;
}

//========================================================================
// ゲーム画面クラスの終了処理
//========================================================================
void CGame::Uninit(void)
{
	// テクスチャを破棄
	CCloud::Unload();
	CActionUI::Unload();
	CGrass::Unload();
	CWaterSurface::Unload();
	CBeach::Unload();
	CMountain::Unload();
	CItemUI::Unload();
	CJumpMeterUI::Unload();
	CEnergyUI::Unload();

	// アクションUIの破棄
	if (m_pActionUI != NULL)
	{// NULLチェック
		m_pActionUI = NULL;
	}

	// スコアの破棄
	if (m_pScore != NULL)
	{// NULLチェック
		m_pScore = NULL;
	}

	// アイテムUIの破棄
	if (m_pItemUI != NULL)
	{// NULLチェック
		m_pItemUI = NULL;
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

	// 海の破棄
	if (m_pWaterSurface != NULL)
	{// NULLチェック
		m_pWaterSurface = NULL;
	}

	// 砂浜の破棄
	if (m_pBeach != NULL)
	{// NULLチェック
		m_pBeach = NULL;
	}

	// 山の破棄
	if (m_pMountain != NULL)
	{// NULLチェック
		m_pMountain = NULL;
	}

	// マップオブジェクトの破棄
	if (m_pMapObject != NULL)
	{// NULLチェック
		delete m_pMapObject;
		m_pMapObject = NULL;
	}

	// ポーズの破棄
	if (m_pPause != NULL)
	{// NULLチェック
		// 終了処理
		m_pPause->Uninit();

		delete m_pPause;
		m_pPause = NULL;
	}

#ifdef ENABLE_INHERITANCE_COBJECT
	// 自分自身を破棄
	CObject::Release();
#endif
	}

//========================================================================
// ゲーム画面クラスの更新処理
//========================================================================
void CGame::Update(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();		// キーボード入力の取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();			// ジョイパッド入力の取得
	CFade* pFade = CManager::GetFade();									// フェードの取得
	CTransition* pTransition = CManager::GetTransition();				// 画面遷移の取得

	if (m_pPause != NULL)
	{// NULLチェック
		// ポーズ画面表示/非表示
		m_pPause->SetDisp(CManager::GetPause());
	}

	if (CManager::GetPause() == true)
	{// ポーズ中
		if (m_pPause != NULL)
		{// NULLチェック
			// 更新処理
			m_pPause->Update();
		}

		return;
	}
	else
	{// ポーズじゃないとき
		if (m_pPause != NULL)
		{// NULLチェック
			m_pPause->SetMenu(CPause::MENU_CONTINUE);
		}
	}

	if (m_pMapObject != NULL)
	{// NULLチェック
		// マップオブジェクトの更新
		m_pMapObject->Update();
	}

	// 画面遷移
	if (isFade() == true)
	{// ENTERが押された
		if (pTransition != NULL)
		{// NULLチェック
			pTransition->SetTransition(MODE_RESULT);
		}

		return;
	}

#ifdef _DEBUG
	// 画面遷移（デバッグ）
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
		pInputJoypad->GetTrigger(0, CInputJoypad::JOYKEY_START) == true)
	{// ENTERが押された
		if (pTransition != NULL)
		{// NULLチェック
			pTransition->SetTransition(MODE_RESULT);
		}

		return;
	}
#endif
}

//========================================================================
// ゲーム画面クラスの描画処理
//========================================================================
void CGame::Draw(void)
{
}