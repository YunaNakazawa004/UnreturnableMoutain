//========================================================================
// 
// マネージャー [ manager.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "manager.h"

#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "object.h"
#include "object2D.h"
#include "object3D.h"
#include "objectX.h"
#include "ObjectBillboard.h"
#include "number.h"
#include "model.h"

#include "title.h"
#include "game.h"
#include "result.h"

#include "screen.h"
#include "fade.h"

#include "effect2D.h"
#include "effect3D.h"
#include "meshfield.h"
#include "grass.h"

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
CRenderer* CManager::m_pRenderer = NULL;				// レンダラーのインスタンス
CInputKeyboard* CManager::m_pInputKeyboard = NULL;		// キーボード入力のインスタンス
CInputMouse* CManager::m_pInputMouse = NULL;			// マウス入力のインスタンス
CInputJoypad* CManager::m_pInputJoypad = NULL;			// ジョイパッド入力のインスタンス
CSound* CManager::m_pSound = NULL;						// サウンドのインスタンス
CDebugProc* CManager::m_pDebugProc = NULL;				// デバッグ表示のインスタンス
CCamera* CManager::m_pCamera = NULL;					// カメラのインスタンス
CLight* CManager::m_pLight = NULL;						// ライトのインスタンス
CTexture* CManager::m_pTexture = NULL;					// テクスチャのインスタンス
CScene* CManager::m_pScene = NULL;						// シーンのインスタンス
CFade* CManager::m_pFade = NULL;						// フェードのインスタンス
CScreen* CManager::m_pScreen = NULL;					// 画面のインスタンス
int CManager::m_nCountFPS = 0;							// FPSカウンター
bool CManager::m_bPause = false;						// ポーズするかしないか

//========================================================================
// マネージャークラスのコンストラクタ
//========================================================================
CManager::CManager()
{
	// マネージャークラスの値をクリア
	m_pRenderer = NULL;
	m_pInputKeyboard = NULL;
	m_pInputMouse = NULL;
	m_pInputJoypad = NULL;
	m_pSound = NULL;
	m_pDebugProc = NULL;
	m_pCamera = NULL;
	m_pLight = NULL;
	m_pTexture = NULL;
	m_pScene = NULL;
	m_pFade = NULL;
	m_pScreen = NULL;
	m_nCountFPS = 0;
	m_bPause = false;
}

//========================================================================
// マネージャークラスのデストラクタ
//========================================================================
CManager::~CManager()
{
}

//========================================================================
// マネージャークラスの初期化処理
//========================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// レンダラーの生成
	if (SUCCEEDED(CreateInstance(&m_pRenderer)))
	{// レンダラーの生成に成功
		// 初期化処理
		if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
		{// 初期化処理が失敗した場合
			OutputDebugStringA("! ! ! レンダラーの初期化に失敗しました ! ! !\n");

			return E_FAIL;
		}
	}

	// キーボード入力の生成
	if (SUCCEEDED(CreateInstance(&m_pInputKeyboard)))
	{// キーボード入力の生成に成功
		// 初期化処理
		if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
		{// 初期化処理が失敗した場合
			OutputDebugStringA("! ! ! キーボードの初期化に失敗しました ! ! !\n");

			return E_FAIL;
		}
	}

	// マウス入力の生成
	if (SUCCEEDED(CreateInstance(&m_pInputMouse)))
	{// マウス入力の生成に成功
		// 初期化処理
		if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
		{// 初期化処理が失敗した場合
			OutputDebugStringA("! ! ! マウスの初期化に失敗しました ! ! !\n");

			return E_FAIL;
		}
	}

	// ジョイパッド入力の生成
	if (SUCCEEDED(CreateInstance(&m_pInputJoypad)))
	{// ジョイパッド入力の生成に成功
		// 初期化処理
		if (FAILED(m_pInputJoypad->Init()))
		{// 初期化処理が失敗した場合
			OutputDebugStringA("! ! ! ジョイパッドの初期化に失敗しました ! ! !\n");

			return E_FAIL;
		}
	}

	// サウンドの生成
	if (SUCCEEDED(CreateInstance(&m_pSound)))
	{// サウンドの生成に成功
		// 初期化処理
		if (FAILED(m_pSound->Init(hWnd)))
		{// 初期化処理が失敗した場合
			OutputDebugStringA("! ! ! サウンドの初期化に失敗しました ! ! !\n");

			return E_FAIL;
		}

		m_pSound->PlaySoundA(CSound::SOUND_BGM_GAME);
	}

	// カメラの生成
	if (SUCCEEDED(CreateInstance(&m_pCamera)))
	{// カメラの生成に成功
		// 初期化処理
		if (FAILED(m_pCamera->Init()))
		{// 初期化処理が失敗した場合
			OutputDebugStringA("! ! ! カメラの初期化に失敗しました ! ! !\n");

			return E_FAIL;
		}
	}

	// ライトの生成
	if (SUCCEEDED(CreateInstance(&m_pLight)))
	{// ライトの生成に成功
		// 初期化処理
		if (FAILED(m_pLight->Init()))
		{// 初期化処理が失敗した場合
			OutputDebugStringA("! ! ! ライトの初期化に失敗しました ! ! !\n");

			return E_FAIL;
		}
	}

	// デバッグ表示の生成
	if (SUCCEEDED(CreateInstance(&m_pDebugProc)))
	{// デバッグ表示の生成に成功
		// 初期化処理
		if (FAILED(m_pDebugProc->Init()))
		{// 初期化処理が失敗した場合
			OutputDebugStringA("! ! ! デバッグ表示の初期化に失敗しました ! ! !\n");

			return E_FAIL;
		}
	}
	
	// テクスチャの生成
	if (SUCCEEDED(CreateInstance(&m_pTexture)))
	{// テクスチャの生成に成功
		// 読み込み処理
		if (FAILED(m_pTexture->Load()))
		{// 初期化処理が失敗した場合
			OutputDebugStringA("! ! ! 規定テクスチャの読み込みに失敗しました ! ! !\n");

			return E_FAIL;
		}
	}

	// オブジェクトのテクスチャを読み込み
	CEffect2D::Load();
	CEffect3D::Load();
	CNumber::Load();
	CMeshField::Load();
	CGrass::Load();
	
#ifdef MALTITARGET_RENDERING
	// 画面の生成
	if (m_pScreen == NULL)
	{// NULLチェック
		m_pScreen = CScreen::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f), SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);

		if (m_pScreen == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! フェードの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}
	}
#endif
		
	// フェードの生成
	if (m_pFade == NULL)
	{// NULLチェック
#ifdef _DEBUG
		m_pFade = CFade::Create(CScene::MODE_GAME);
#else
		m_pFade = CFade::Create(CScene::MODE_TITLE);
#endif

		if (m_pFade == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! フェードの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}
	}

	// FPSカウンタを初期化
	m_nCountFPS = 0;

	return S_OK;
}

//========================================================================
// マネージャークラスの終了処理
//========================================================================
void CManager::Uninit(void)
{
	// 全てのオブジェクトを破棄
	CObject::ReleaseAll();

	// オブジェクトのテクスチャを破棄
	CGrass::Unload();
	CMeshField::Unload();
	CNumber::Unload();
	CEffect3D::Unload();
	CEffect2D::Unload();

	// フェードの破棄
	if (m_pFade != NULL)
	{// NULLチェック
		// 終了処理
		m_pFade->Uninit();

		delete m_pFade;
		m_pFade = NULL;
	}
	
	// 画面の破棄
	if (m_pScreen != NULL)
	{// NULLチェック
		// 終了処理
		m_pScreen->Uninit();

		delete m_pScreen;
		m_pScreen = NULL;
	}

	// シーンの破棄
	if (m_pScene != NULL)
	{// NULLチェック
#ifdef ENABLE_INHERITANCE_COBJECT
		m_pScene = NULL;
#else
		delete m_pScene;
		m_pScene = NULL;
#endif
	}

	// テクスチャの破棄
	if (m_pTexture != NULL)
	{// NULLチェック
		// 破棄
		m_pTexture->Unload();

		delete m_pTexture;
		m_pTexture = NULL;
	}

	// デバッグ表示の破棄
	if (m_pDebugProc != NULL)
	{// NULLチェック
		// 終了処理
		m_pDebugProc->Uninit();

		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}

	// ライトの破棄
	if (m_pLight != NULL)
	{// NULLチェック
		// 終了処理
		m_pLight->Uninit();

		delete m_pLight;
		m_pLight = NULL;
	}

	// カメラの破棄
	if (m_pCamera != NULL)
	{// NULLチェック
		// 終了処理
		m_pCamera->Uninit();

		delete m_pCamera;
		m_pCamera = NULL;
	}

	// サウンドの破棄
	if (m_pSound != NULL)
	{// NULLチェック
		// サウンドを止める
		m_pSound->StopSound();

		// 終了処理
		m_pSound->Uninit();

		delete m_pSound;
		m_pSound = NULL;
	}

	// ジョイパッド入力の破棄
	if (m_pInputJoypad != NULL)
	{// NULLチェック
		// 終了処理
		m_pInputJoypad->Uninit();

		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}

	// マウス入力の破棄
	if (m_pInputMouse != NULL)
	{// NULLチェック
		// 終了処理
		m_pInputMouse->Uninit();

		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	// キーボード入力の破棄
	if (m_pInputKeyboard != NULL)
	{// NULLチェック
		// 終了処理
		m_pInputKeyboard->Uninit();

		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// レンダラーの破棄
	if (m_pRenderer != NULL)
	{// NULLチェック
		// 終了処理
		m_pRenderer->Uninit();

		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

//========================================================================
// マネージャークラスの更新処理
//========================================================================
void CManager::Update(void)
{
	if (m_pDebugProc != NULL)
	{// NULLチェック
		// FPS を表示
		m_pDebugProc->Print("FPS : %d\n", m_nCountFPS);

		// 現在のシーン名を表示
		m_pDebugProc->Print("現在のシーン : %s\n",
			(m_pScene->GetMode() == CScene::MODE_TITLE) ? "タイトル画面" :
			(m_pScene->GetMode() == CScene::MODE_GAME) ? "ゲーム画面" : "リザルト画面");

		// オブジェクトの総数を表示
		m_pDebugProc->Print("オブジェクトの総数 : %d\n", CObject::GetNumAll());
	}

	if (m_pFade != NULL)
	{// NULLチェック
		// フェードの更新
		m_pFade->Update();
	}
	
	if (m_pScreen != NULL)
	{// NULLチェック
		// 画面の更新
		m_pScreen->Update();
	}

#ifndef ENABLE_INHERITANCE_COBJECT
	if (m_pScene != NULL)
	{// NULLチェック
		// シーンの更新
		m_pScene->Update();
	}
#endif

	if (m_bPause == false)
	{// ポーズしていないときだけ
		if (m_pRenderer != NULL)
		{// NULLチェック
			// レンダラーの更新
			m_pRenderer->Update();
		}

		if (m_pCamera != NULL)
		{// NULLチェック
			// カメラの更新
			m_pCamera->Update();
		}
	}

	if (m_pLight != NULL)
	{// NULLチェック
		// ライトの更新
		m_pLight->Update();
	}

	if (m_pInputKeyboard != NULL)
	{// NULLチェック
		// キーボード入力の更新
		m_pInputKeyboard->Update();

		if (m_pInputKeyboard->GetTrigger(DIK_P) == true)
		{// ポーズ切り替え
			m_bPause = m_bPause ? false : true;

			m_pSound->PlaySoundA(CSound::SOUND_SE_PAUSE);
		}

		if (m_pInputKeyboard->GetTrigger(DIK_F8) == true)
		{// ワイヤーフレーム切り替え
			if (m_pRenderer != NULL)
			{// NULLチェック
				m_pRenderer->SwapWireFrame();
			}
		}
	}

	if (m_pInputMouse != NULL)
	{// NULLチェック
		// マウス入力の更新
		m_pInputMouse->Update();
	}

	if (m_pInputJoypad != NULL)
	{// NULLチェック
		// ジョイパッド入力の更新
		m_pInputJoypad->Update();
	}

	if (m_pDebugProc != NULL)
	{// NULLチェック
		// デバッグ表示の更新
		m_pDebugProc->Update();
	}
}

//========================================================================
// マネージャークラスの描画処理
//========================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{// NULLチェック
		// レンダラーの描画
		m_pRenderer->Draw();
	}
}

//========================================================================
// シーンモード設定
//========================================================================
void CManager::SetMode(const CScene::MODE mode)
{
	// サウンドの停止
	if (m_pSound != NULL)
	{// NULLチェック
		m_pSound->StopSound();
	}

	// 現在のモードの破棄
	if (m_pScene != NULL)
	{// NULLチェック
		// 終了処理
		m_pScene->Uninit();

#ifdef ENABLE_INHERITANCE_COBJECT
		m_pScene = NULL;

#else
		delete m_pScene;
		m_pScene = NULL;

#endif
	}

	// 全てのオブジェクトの破棄
	CObject::ReleaseAll();

	// 新しいモードの作成
	if (m_pScene == NULL)
	{// NULLチェック
		m_pScene = CScene::Create(mode); 
	}

	SetPause(false);
}