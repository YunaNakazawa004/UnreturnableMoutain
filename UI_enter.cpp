//========================================================================
// 
// エンターUI [ UI_enter.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "UI_enter.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"
#include "texture.h"

#include "title.h"
#include "player.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define WAIT_COUNT			(30)			// 待機状態のカウント
#define BLINK_COUNT			(5)				// 点滅状態のカウント
#define FADE_COUNT			(50)			// 画面遷移までのカウント

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
int CEnterUI::m_nIdxTexture = -1;				// テクスチャのインデックス

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CEnterUI::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_nIdxTexture = pTexture->Register("data\\TEXTURE\\UI\\press_enter.png");

	if (m_nIdxTexture == -1)
	{// テクスチャが設定できていない
		OutputDebugStringA("! ! ! テクスチャの設定に失敗しました ! ! !\n");

		return E_FAIL;
	}

	return S_OK;
}

//========================================================================
// テクスチャの破棄
//========================================================================
void CEnterUI::Unload(void)
{
	// テクスチャのインデックスを削除
	memset(&m_nIdxTexture, -1, sizeof m_nIdxTexture);
}

//========================================================================
// エンターUIクラスの生成処理
//========================================================================
CEnterUI* CEnterUI::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CEnterUI* pEnterUI = NULL;

	if (pEnterUI == NULL)
	{// NULLチェック
		// エンターUIの生成
		pEnterUI = new CEnterUI;
	}

	if (pEnterUI != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pEnterUI->Init(pos, fWidth, fHeight)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! エンターUIの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// オブジェクトの種類を設定
		pEnterUI->SetType(TYPE_ENTERUI);

		// テクスチャの割り当て
		pEnterUI->BindTexture(m_nIdxTexture);

		return pEnterUI;
	}

	OutputDebugStringA("! ! ! エンターUIの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// エンターUIクラスのコンストラクタ
//========================================================================
CEnterUI::CEnterUI(const int nPriority) :CObject2D(nPriority)
{
	// エンターUIクラスの値をクリア
	m_state = STATE_NONE;
	m_nCounterState = 0;
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
}

//========================================================================
// エンターUIクラスのデストラクタ
//========================================================================
CEnterUI::~CEnterUI()
{
}

//========================================================================
// エンターUIクラスの初期化処理
//========================================================================
HRESULT CEnterUI::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	if (FAILED(CObject2D::Init(pos, fWidth, fHeight)))
	{// エンターUIの初期化に失敗した場合
		OutputDebugStringA("! ! ! エンターUIの初期化に失敗しました ! ! !\n");

		return E_FAIL;
	}

	// エンターUIクラスの値を初期化
	m_state = STATE_WAIT;
	m_nCounterState = 0;
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	return S_OK;
}

//========================================================================
// エンターUIクラスの終了処理
//========================================================================
void CEnterUI::Uninit(void)
{
	// 終了処理
	CObject2D::Uninit();
}

//========================================================================
// エンターUIクラスの更新処理
//========================================================================
void CEnterUI::Update(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();		// キーボード入力の取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();			// ジョイパッド入力の取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();				// マウス入力の取得
	CSound* pSound = CManager::GetSound();								// サウンドを取得
	CPlayer* pPlayer = CTitle::GetPlayer();
	D3DXCOLOR col = GetColor();

	switch (m_state)
	{
	case STATE_NONE:
		SetDisp(false);
		SetColor(COLOR_WHITE);

		break;

	case STATE_WAIT:
		if (m_nCounterState % WAIT_COUNT == 0)
		{// 一定間隔で点滅
			SwapDisp();
		}

		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || 
			pInputJoypad->GetTrigger(0, CInputJoypad::JOYKEY_A) == true || 
			pInputJoypad->GetTrigger(0, CInputJoypad::JOYKEY_START) == true || 
			(MouseCollision(pInputMouse->GetPosition(),m_fWidth, m_fHeight) == true) && 
			pInputMouse->GetTrigger(CInputMouse::MOUSEBUTTON_LEFT))
		{// スタート
			m_state = STATE_BLINK;
			m_nCounterState = 0;

			pSound->PlaySound(CSound::SE_ENTER);
			pInputJoypad->SetVibration(0, 500, 500, 20);
		}

		break;

	case STATE_BLINK:
		if (m_nCounterState % BLINK_COUNT == 0)
		{// 一定間隔で点滅
			SetDisp(true);
			SetColor((col == COLOR_WHITE) ? COLOR_ORANGE : COLOR_WHITE);
		}

		if (m_nCounterState > FADE_COUNT)
		{// すこし経ってからチュートリアルに進む
			pPlayer->SetState(CPlayer::STATE_TUTORIAL);

			m_state = STATE_NONE;
		}

		break;
	}

	m_nCounterState++;
}

//========================================================================
// エンターUIクラスの描画処理
//========================================================================
void CEnterUI::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}