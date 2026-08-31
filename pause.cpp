//========================================================================
// 
// ポーズ画面 [ pause.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "pause.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "transition.h"

#include "object2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FADE_SPEED				(0.06f)							// フェードの速さ

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
const char* CPause::m_apFilename[MENU_MAX + 1] = {				// テクスチャのファイル名
	"data\\TEXTURE\\UI\\pause000.png",
	"data\\TEXTURE\\UI\\pause001.png",
	"data\\TEXTURE\\UI\\pause002.png",
	"data\\TEXTURE\\UI\\pause_back.png",
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
	m_pBack = NULL;
	m_menu = MENU_CONTINUE;
	m_nMenu = 0;
	m_fade = FADE_NONE;
	m_fLength = 0.0f;
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

	m_pBack = CObject2D::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f), 640.0f, 360.0f,
		CObject::TYPE_PAUSE, m_apFilename[MENU_MAX], CObject::PRIORITY_6);

	if (m_pBack != NULL)
	{// NULLチェック
		m_pBack->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}

	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		m_apObject2D[nCnt] = CObject2D::Create(D3DXVECTOR3(240.0f + (nCnt * 400.0f), 360.0f, 0.0f), 200.0f, 40.0f,
			CObject::TYPE_PAUSE, m_apFilename[nCnt], CObject::PRIORITY_6);

		if (m_apObject2D[nCnt] != NULL)
		{// NULLチェック
			m_apObject2D[nCnt]->SetDisp(false);

			m_fLength = m_apObject2D[nCnt]->GetSize();
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

	if (m_pBack != NULL)
	{// NULLチェック
		// 終了処理
		m_pBack = NULL;
	}
}

//========================================================================
// ポーズクラスの更新処理
//========================================================================
void CPause::Update(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();		// キーボード入力の取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();			// ジョイパッド入力の取得
	CFade* pFade = CManager::GetFade();									// フェードの取得
	CTransition* pTransition = CManager::GetTransition();				// 画面遷移の取得
	CSound* pSound = CManager::GetSound();								// サウンドを取得
	D3DXCOLOR col = m_pBack->GetColor();

	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != NULL)
		{// NULLチェック
			if (m_nMenu == nCnt)
			{// 選択されている場合
				m_apObject2D[nCnt]->SetSize(m_fLength * 1.2f);
			}
			else
			{// 選択されていない場合
				m_apObject2D[nCnt]->SetSize(m_fLength);
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
			pSound->PlaySound(CSound::SE_PAUSE);

			break;

		case MENU_RETRY:			// モード設定(ゲーム画面に移行)
			m_menu = MENU_RETRY;

			if (pTransition != NULL)
			{// NULLチェック
				pTransition->SetTransition(CScene::MODE_GAME);
			}

			// サウンドの再生
			pSound->PlaySound(CSound::SE_ENTER);

			break;

		case MENU_QUIT:			// モード設定(タイトル画面に移行)
			m_menu = MENU_QUIT;

			if (pTransition != NULL)
			{// NULLチェック
				pTransition->SetTransition(CScene::MODE_TITLE);
			}

			// サウンドの再生
			pSound->PlaySound(CSound::SE_ENTER);

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
		if (pInputKeyboard->GetRepeat(DIK_W) == true || pInputKeyboard->GetRepeat(DIK_A) == true 
			|| pInputJoypad->GetRepeat(0, CInputJoypad::JOYKEY_UP) == true || 
			pInputJoypad->GetRepeat(0, CInputJoypad::JOYKEY_LEFT) == true ||
			(pInputJoypad->GetStickSlow(0) == true && pInputJoypad->GetStick(0, CInputJoypad::JOYKEY_LEFTSTICK_UP, NULL, NULL) == true) ||
			(pInputJoypad->GetStickSlow(0) == true && pInputJoypad->GetStick(0, CInputJoypad::JOYKEY_LEFTSTICK_LEFT, NULL, NULL) == true))
		{// 上に移動
			m_nMenu = (m_nMenu + MENU_MAX - 1) % MENU_MAX;

			// サウンドの再生
			pSound->PlaySound(CSound::SE_CURSOR);
		}

		else if (pInputKeyboard->GetRepeat(DIK_S) == true || pInputKeyboard->GetRepeat(DIK_D) == true || 
			pInputJoypad->GetRepeat(0, CInputJoypad::JOYKEY_DOWN) == true ||
			pInputJoypad->GetRepeat(0, CInputJoypad::JOYKEY_RIGHT) == true ||
			(pInputJoypad->GetStickSlow(0) == true && pInputJoypad->GetStick(0, CInputJoypad::JOYKEY_LEFTSTICK_DOWN, NULL, NULL) == true) ||
			(pInputJoypad->GetStickSlow(0) == true && pInputJoypad->GetStick(0, CInputJoypad::JOYKEY_LEFTSTICK_RIGHT, NULL, NULL) == true))
		{// 下に移動
			m_nMenu = (m_nMenu + 1) % MENU_MAX;

			// サウンドの再生
			pSound->PlaySound(CSound::SE_CURSOR);
		}
	}

	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{// フェードイン状態
			SetDisp(false);

			col.a -= FADE_SPEED;			// ポリゴンを透明にしていく

			if (col.a <= 0.0f)
			{// 透明になった
				col.a = 0.0f;
				m_fade = FADE_NONE;			// 何もしていない状態にする
			}
		}

		else if (m_fade == FADE_OUT)
		{// フェードアウト状態
			col.a += FADE_SPEED;			// ポリゴンを不透明にしていく

			if (col.a >= 1.0f)
			{// 不透明になった
				col.a = 1.0f;

				SetDisp(true);
			}
		}
	}
	else
	{
		col.a = 0.0f;
	}

	// 色を適用
	m_pBack->SetColor(col);
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

//========================================================================
// 背景のフェードを設定
//========================================================================
void CPause::SetFade(const int fade)
{
	if (m_fade != fade)
	{// 違うときだけ
		m_fade = fade;
	}
}