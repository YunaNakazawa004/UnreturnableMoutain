//========================================================================
// 
// ポーズ画面 [pause.cpp]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "pause.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"

#include "object2D.h"

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
const char* CPause::m_apFilename[MENU_MAX] = {				// テクスチャのファイル名
	"data\\TEXTURE\\UI\\pause000.png",
	"data\\TEXTURE\\UI\\pause001.png",
	"data\\TEXTURE\\UI\\pause002.png",
};

//========================================================================
// ポーズの生成
//========================================================================
CPause* CPause::Create(const MENU menu)
{
	CPause* pPause = NULL;

	if (pPause == NULL)
	{// NULLチェック
		// 草の生成
		pPause = new CPause;
	}

	if (pPause != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pPause->Init(menu)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! 草の初期化に失敗しました ! ! !\n");

			return NULL;
		}

		return pPause;
	}

	OutputDebugStringA("! ! ! 草の生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// ポーズのコンストラクタ
//========================================================================
CPause::CPause()
{
	// 値をクリア
	memset(&m_apObject2D[0], NULL, sizeof m_apObject2D);
	m_menu = MENU_CONTINUE;
	m_nMenu = 0;
}

//========================================================================
// ポーズのデストラクタ
//========================================================================
CPause::~CPause()
{
}

//========================================================================
// ポーズクラスの初期化処理
//========================================================================
HRESULT CPause::Init(const MENU menu)
{
	// 値を初期化
	m_menu = MENU_CONTINUE;
	m_nMenu = 0;

	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		m_apObject2D[nCnt] = CObject2D::Create(D3DXVECTOR3(640.0f, 260.0f + (nCnt * 100.0f), 0.0f), 160.0f, 30.0f,
			CObject::TYPE_PAUSE, m_apFilename[nCnt], UI_PRIORITY);

		if (m_apObject2D[nCnt] != NULL)
		{// NULLチェック
			m_apObject2D[nCnt]->SetDisp(false);
		}
	}

	return S_OK;
}

//========================================================================
// ポーズクラスの終了処理
//========================================================================
void CPause::Uninit(void)
{
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != NULL)
		{// NULLチェック
			// 終了処理
			m_apObject2D[nCnt] = NULL;
		}
	}
}

//========================================================================
// ポーズクラスの更新処理
//========================================================================
void CPause::Update(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();		// キーボード入力の取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();			// ジョイパッド入力の取得

	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != NULL)
		{// NULLチェック
			if (m_nMenu == nCnt)
			{// 選択されている場合
				// 頂点カラーの設定
				m_apObject2D[nCnt]->SetColor(COLOR_WHITE);
			}
			else
			{// 選択されていない場合
				// 頂点カラーの設定
				m_apObject2D[nCnt]->SetColor(COLOR_DARKGRAY);
			}
		}
	}

	if ((pInputKeyboard->GetTrigger(DIK_RETURN) == true || pInputJoypad->GetTrigger(0, CInputJoypad::JOYKEY_A) == true))
	{// 決定キーが押された

		switch (m_nMenu)
		{
		case MENU_CONTINUE:		// ポーズを解除
			m_menu = MENU_CONTINUE;
			CManager::SetPause(false);

			// サウンドの再生
			//PlaySound(SOUND_LABEL_SE_CONTINUE);

			break;

		case MENU_RETRY:			// モード設定(ゲーム画面に移行)
			m_menu = MENU_RETRY;

			CManager::SetMode(CScene::MODE_GAME);

			// サウンドの再生
			//PlaySound(SOUND_LABEL_SE_RETRY);

			break;

		case MENU_QUIT:			// モード設定(タイトル画面に移行)
			m_menu = MENU_QUIT;

			CManager::SetMode(CScene::MODE_TITLE);

			// サウンドの再生
			//PlaySound(SOUND_LABEL_SE_QUIT);

			break;
		}
	}
	else if ((pInputKeyboard->GetTrigger(DIK_P) == true || pInputJoypad->GetTrigger(0, CInputJoypad::JOYKEY_START) == true))
	{// 閉じられた
		m_nMenu = 0;
		m_menu = MENU_CONTINUE;
	}

	if (m_menu == MENU_CONTINUE)
	{
		if (pInputKeyboard->GetRepeat(DIK_W) == true || pInputJoypad->GetRepeat(0, CInputJoypad::JOYKEY_UP) == true || 
			(pInputJoypad->GetStickSlow(0) == true && pInputJoypad->GetStick(0, CInputJoypad::JOYKEY_LEFTSTICK_UP, NULL, NULL) == true))
		{// 上に移動
			m_nMenu = (m_nMenu + MENU_MAX - 1) % MENU_MAX;

			// サウンドの再生
			//PlaySound(SOUND_LABEL_SE_CURSOR);
		}

		else if (pInputKeyboard->GetRepeat(DIK_S) == true || pInputJoypad->GetRepeat(0, CInputJoypad::JOYKEY_DOWN) == true || 
			(pInputJoypad->GetStickSlow(0) == true && pInputJoypad->GetStick(0, CInputJoypad::JOYKEY_LEFTSTICK_DOWN, NULL, NULL) == true))
		{// 下に移動
			m_nMenu = (m_nMenu + 1) % MENU_MAX;

			// サウンドの再生
			//PlaySound(SOUND_LABEL_SE_CURSOR);
		}
	}
}

//========================================================================
// ポーズクラスの描画処理
//========================================================================
void CPause::Draw(void)
{
}

//========================================================================
// 表示状態を設定
//========================================================================
void CPause::SetDisp(const bool bDisp)
{
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != NULL)
		{// NULLチェック
			// 表示設定
			m_apObject2D[nCnt]->SetDisp(bDisp);
		}
	}
}