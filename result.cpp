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

#include "object2D.h"
#include "UI_result.h"

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
CObject2D* CResult::m_pObject2D = NULL;						// 背景のインスタンス
CResultUI* CResult::m_pResultUI = NULL;						// リザルトUIのインスタンス
bool CResult::m_bClear = false;								// クリアフラグ

//========================================================================
// リザルト画面クラスのコンストラクタ
//========================================================================
CResult::CResult() : CScene(CScene::MODE_RESULT)
{
	// 値をクリア
	m_pObject2D = NULL;
	m_pResultUI = NULL;
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
	pCamera->SetType(CCamera::TYPE_STOP);

	// テクスチャを読み込み
	CResultUI::Load();

	// 背景を生成
	if (m_pObject2D == NULL)
	{// NULLチェック
		m_pObject2D = CObject2D::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 
			CObject::TYPE_BG, "data\\TEXTURE\\UI\\player.png");

		if (m_pObject2D == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! 背景の生成に失敗しました ! ! !\n");

			return E_FAIL;
		}
	}
	
	// リザルトUIを生成
	if (m_pResultUI == NULL)
	{// NULLチェック
		m_pResultUI = CResultUI::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f), 600.0f, 120.0f,
			(m_bClear == true) ? CResultUI::TYPE_CLEAR : CResultUI::TYPE_GAMEOVER);

		if (m_pResultUI == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! リザルトUIの生成に失敗しました ! ! !\n");

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

	// リザルトUIの破棄
	if (m_pResultUI != NULL)
	{// NULLチェック
		m_pResultUI = NULL;
	}
	
	// 背景の破棄
	if (m_pObject2D != NULL)
	{// NULLチェック
		m_pObject2D = NULL;
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