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

#include "object2D.h"

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
CObject2D* CTitle::m_pObject2D = NULL;						// 背景のインスタンス

//========================================================================
// タイトル画面クラスのコンストラクタ
//========================================================================
CTitle::CTitle() : CScene(CScene::MODE_TITLE)
{
	// 値をクリア
	m_pObject2D = NULL;
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

	// 背景を生成
	if (m_pObject2D == NULL)
	{// NULLチェック
		m_pObject2D = CObject2D::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,
			CObject::TYPE_BG, "data\\TEXTURE\\UI\\title.png");

		if (m_pObject2D == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! 背景の生成に失敗しました ! ! !\n");

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
	if (m_pObject2D != NULL)
	{// NULLチェック
		m_pObject2D = NULL;
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
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
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