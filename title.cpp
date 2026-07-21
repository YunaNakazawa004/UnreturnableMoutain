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

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
CTitleLogo* CTitle::m_pTitleLogo = NULL;				// タイトルロゴのインスタンス
CEnterUI* CTitle::m_pEnterUI = NULL;					// エンターUIのインスタンス

//========================================================================
// タイトル画面クラスのコンストラクタ
//========================================================================
CTitle::CTitle() : CScene(CScene::MODE_TITLE)
{
	// 値をクリア
	m_pTitleLogo = NULL;
	m_pEnterUI = NULL;
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

	// タイトルロゴを生成
	if (m_pTitleLogo == NULL)
	{// NULLチェック
		m_pTitleLogo = CTitleLogo::Create(D3DXVECTOR3(960.0f, 170.0f, 0.0f), 300.0f, 150.0f);

		if (m_pTitleLogo == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! タイトルロゴの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}
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
	}

	return S_OK;
}

//========================================================================
// タイトル画面クラスの終了処理
//========================================================================
void CTitle::Uninit(void)
{
	// テクスチャを破棄
	CEnterUI::Unload();
	CTitleLogo::Unload();

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