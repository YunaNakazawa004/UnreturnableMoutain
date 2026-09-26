//========================================================================
// 
// 選択肢 [ UI_select.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "UI_select.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "sound.h"

#include "object2D.h"
#include "title.h"
#include "ship.h"
#include "camera.h"
#include "UI_title_logo.h"
#include "player.h"
#include "particle3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
const char* CSelect::m_apFilename[MENU_MAX] = {				// テクスチャのファイル名
	"data\\TEXTURE\\UI\\select000.png",
	"data\\TEXTURE\\UI\\select001.png",
	"data\\TEXTURE\\UI\\select002.png",
};

//========================================================================
// 選択肢の生成
//========================================================================
CSelect* CSelect::Create(const MENU menu)
{
	CSelect* pSelect = NULL;

	if (pSelect == NULL)
	{// NULLチェック
		// 草の生成
		pSelect = new CSelect;
	}

	if (pSelect != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pSelect->Init(menu)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! 草の初期化に失敗しました ! ! !\n");

			return NULL;
		}

		return pSelect;
	}

	OutputDebugStringA("! ! ! 草の生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// 選択肢のコンストラクタ
//========================================================================
CSelect::CSelect(const int nPriority) : CObject(nPriority)
{
	// 値をクリア
	memset(&m_apObject2D[0], NULL, sizeof m_apObject2D);
	m_menu = MENU_START;
	m_nMenu = 0;
	m_fLength = 0.0f;
	m_bSelect = false;
}

//========================================================================
// 選択肢のデストラクタ
//========================================================================
CSelect::~CSelect()
{
}

//========================================================================
// 選択肢クラスの初期化処理
//========================================================================
HRESULT CSelect::Init(const MENU menu)
{
	// 値を初期化
	m_menu = MENU_START;
	m_nMenu = 0;

	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		m_apObject2D[nCnt] = CObject2D::Create(D3DXVECTOR3(270.0f, 360.0f + (nCnt * 100.0f), 0.0f), 200.0f, 40.0f,
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
// 選択肢クラスの終了処理
//========================================================================
void CSelect::Uninit(void)
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
// 選択肢クラスの更新処理
//========================================================================
void CSelect::Update(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();		// キーボード入力の取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();			// ジョイパッド入力の取得
	CSound* pSound = CManager::GetSound();								// サウンドを取得
	CShip* pShip = CTitle::GetShip();
	CCamera* pCamera = CManager::GetCamera();
	CTitleLogo* pTitleLogo = CTitle::GetTitleLogo();
	CPlayer* pPlayer = CTitle::GetPlayer();

	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != NULL)
		{// NULLチェック
			if (m_nMenu == nCnt)
			{// 選択されている場合
				m_apObject2D[nCnt]->SetColor(COLOR_WHITE);
			}
			else
			{// 選択されていない場合
				m_apObject2D[nCnt]->SetColor(COLOR_GRAY);
			}
		}
	}

	if (m_bSelect == true)
	{// 選択肢中
		if ((pInputKeyboard->GetTrigger(DIK_RETURN) == true || pInputJoypad->GetTrigger(0, CInputJoypad::JOYKEY_A) == true))
		{// 決定キーが押された

			switch (m_nMenu)
			{
			case MENU_START:		// ゲームスタート
				m_menu = MENU_START;

				// サウンドの再生
				pSound->PlaySound(CSound::SE_ENTER);

				// 出発
				pShip->SetState(CShip::STATE_CLOSE);
				pCamera->SetPosition(D3DXVECTOR3(0.0f, 30.0f, 300.0f), DEFAULT_VECTER3, DEFAULT_VECTER3, CCamera::TYPE_STOP);
				SetDisp(false);
				pTitleLogo->SetDisp(false);

				break;

			case MENU_TUTORIAL:		// チュートリアル
				m_menu = MENU_TUTORIAL;

				// サウンドの再生
				pSound->PlaySound(CSound::SE_ENTER);

				// チュートリアルへ
				pPlayer->SetState(CPlayer::STATE_TUTORIAL);
				pPlayer->SetPosition(D3DXVECTOR3(0.0f, 20.0f, -1340.0f));
				pPlayer->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

				break;

			case MENU_EXIT:			// ゲームをやめる
				m_menu = MENU_EXIT;

				// サウンドの再生
				pSound->PlaySound(CSound::SE_ENTER);

				CManager::Quit();

				break;
			}
		}

		if (m_menu == MENU_START)
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
	}
}

//========================================================================
// 選択肢クラスの描画処理
//========================================================================
void CSelect::Draw(void)
{
}

//========================================================================
// 表示状態を設定
//========================================================================
void CSelect::SetDisp(const bool bDisp)
{
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != NULL)
		{// NULLチェック
			// 表示設定
			m_apObject2D[nCnt]->SetDisp(bDisp);
		}
	}

	m_bSelect = bDisp;
}