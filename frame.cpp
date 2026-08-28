//========================================================================
// 
// 枠 [ frame.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "frame.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "texture.h"

#include "game.h"

//************************************************************************
// マクロ定義
//************************************************************************

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
int CFrame::m_nIdxTexture = -1;				// テクスチャのインデックス

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CFrame::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_nIdxTexture = pTexture->Register("data\\TEXTURE\\UI\\frame.png");

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
void CFrame::Unload(void)
{
	// テクスチャのインデックスを削除
	memset(&m_nIdxTexture, -1, sizeof m_nIdxTexture);
}

//========================================================================
// 枠クラスの生成処理
//========================================================================
CFrame* CFrame::Create(void)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CFrame* pFrame = NULL;

	if (pFrame == NULL)
	{// NULLチェック
		// 枠の生成
		pFrame = new CFrame;
	}

	if (pFrame != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pFrame->Init()))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! 枠の初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// オブジェクトの種類を設定
		pFrame->SetType(TYPE_EFFECT3D);

		// テクスチャを設定
		pFrame->BindTexture(m_nIdxTexture);

		return pFrame;
	}

	OutputDebugStringA("! ! ! 枠の生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// 枠クラスのコンストラクタ
//========================================================================
CFrame::CFrame(const int nPriority) :CObject2D(nPriority)
{
	// 枠クラスの値をクリア
}

//========================================================================
// 枠クラスのデストラクタ
//========================================================================
CFrame::~CFrame()
{
}

//========================================================================
// 枠クラスの初期化処理
//========================================================================
HRESULT CFrame::Init(void)
{
	if (FAILED(CObject2D::Init(D3DXVECTOR3(640.0f,360.0f,0.0f), 640.0f, 360.0f)))
	{// 枠の初期化に失敗した場合
		OutputDebugStringA("! ! ! 枠の初期化に失敗しました ! ! !\n");

		return E_FAIL;
	}

	// 枠クラスの値を初期化

	return S_OK;
}

//========================================================================
// 枠クラスの終了処理
//========================================================================
void CFrame::Uninit(void)
{
	CObject2D::Uninit();
}

//========================================================================
// 枠クラスの更新処理
//========================================================================
void CFrame::Update(void)
{
#ifdef _DEBUG
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();		// キーボード入力の取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();			// ジョイパッド入力の取得

	// UI隠し
	if (pInputKeyboard->GetPress(DIK_F3) == true ||
		pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_BACK) == true)
	{// キーが押された
		SetDisp(false);
	}
	else if (pInputKeyboard->GetRelease(DIK_F3) == true ||
		pInputJoypad->GetRelease(0, CInputJoypad::JOYKEY_BACK) == true)
	{// キーが押された
		SetDisp(true);
	}
#endif
}

//========================================================================
// 枠クラスの描画処理
//========================================================================
void CFrame::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}