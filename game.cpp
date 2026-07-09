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
#include "debugproc.h"
#include "camera.h"
#include "pause.h"
#include "fade.h"

#include "meshfield.h"
#include "map_object.h"
#include "player.h"
#include "ship.h"
#include "energyrock.h"

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
CPause* CGame::m_pPause = NULL;						// ポーズのインスタンス
CPlayer* CGame::m_pPlayer = NULL;					// プレイヤーのインスタンス
CShip* CGame::m_pShip = NULL;						// 船のインスタンス
CMeshField* CGame::m_pMeshField = NULL;				// メッシュフィールドのインスタンス
CMapObject* CGame::m_pMapObject = NULL;				// マップオブジェクトのインスタンス
bool CGame::m_bFade = false;						// 遷移フラグ

//========================================================================
// ゲーム画面クラスのコンストラクタ
//========================================================================
CGame::CGame() : CScene(CScene::MODE_GAME)
{
	// 値をクリア
	m_pPause = NULL;
	m_pPlayer = NULL;
	m_pShip = NULL;
	m_pMeshField = NULL;
	m_pMapObject = NULL;
	m_bFade = false;
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
	pCamera->SetType(CCamera::TYPE_PLAYER);

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

	// メッシュフィールドを生成
	if (m_pMeshField == NULL)
	{// NULLチェック
		m_pMeshField = CMeshField::Create(DEFAULT_VECTER3, DEFAULT_VECTER3, D3DXVECTOR2(32.0f, 32.0f),
			D3DXVECTOR2(10.0f, 10.0f), CObject::TYPE_MESHFIELD, OBJECT_PRIORITY);

		if (m_pMeshField == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! メッシュフィールドの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		// ステージのデータを読み込む
		if (FAILED(m_pMeshField->ReadData("data\\stage.bin")))
		{// もし失敗したら
			return E_FAIL;
		}
	}

	// プレイヤーを生成
	if (m_pPlayer == NULL)
	{// NULLチェック
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(1500.0f, 0.0f, -1500.0f), DEFAULT_VECTER3);

		if (m_pPlayer == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! プレイヤーの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}
	}

	// 船を生成
	if (m_pShip == NULL)
	{// NULLチェック
		m_pShip = CShip::Create(D3DXVECTOR3(1500.0f, 0.0f, -1500.0f), DEFAULT_VECTER3);

		if (m_pShip == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! 船の生成に失敗しました ! ! !\n");

			return E_FAIL;
		}
	}

	return S_OK;
}

//========================================================================
// ゲーム画面クラスの終了処理
//========================================================================
void CGame::Uninit(void)
{
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

	// メッシュフィールドの破棄
	if (m_pMeshField != NULL)
	{// NULLチェック
		m_pMeshField = NULL;
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
	CFade* pFade = CManager::GetFade();									// フェードの取得

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
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || m_bFade == true)
	{// ENTERが押された
		if (pFade != NULL)
		{// NULLチェック
			pFade->SetFade(MODE_RESULT);
		}

		return;
	}
}

//========================================================================
// ゲーム画面クラスの描画処理
//========================================================================
void CGame::Draw(void)
{
}