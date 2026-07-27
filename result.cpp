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

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
CResultUI* CResult::m_pResultUI = NULL;						// リザルトUIのインスタンス
CShip* CResult::m_pShip = NULL;								// 船のインスタンス
CLab* CResult::m_pLab = NULL;								// 研究所のインスタンス
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
	CFade* pFade = CManager::GetFade();									// フェードの取得

	// 画面遷移
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
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