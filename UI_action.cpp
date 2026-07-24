//========================================================================
// 
// アクションUI [ UI_action.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "UI_action.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "texture.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define FADE_SPEED				(0.09f)									// フェードの速さ

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
int CActionUI::m_nIdxTexture = -1;				// テクスチャのインデックス

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CActionUI::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_nIdxTexture = pTexture->Register("data\\TEXTURE\\UI\\action.png");

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
void CActionUI::Unload(void)
{
	// テクスチャのインデックスを削除
	memset(&m_nIdxTexture, -1, sizeof m_nIdxTexture);
}

//========================================================================
// アクションUIクラスの生成処理
//========================================================================
CActionUI* CActionUI::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CActionUI* pActionUI = NULL;

	if (pActionUI == NULL)
	{// NULLチェック
		// アクションUIの生成
		pActionUI = new CActionUI;
	}

	if (pActionUI != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pActionUI->Init(pos, fWidth, fHeight)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! アクションUIの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// オブジェクトの種類を設定
		pActionUI->SetType(TYPE_RESULTUI);

		// テクスチャの割り当て
		pActionUI->BindTexture(m_nIdxTexture);

		return pActionUI;
	}

	OutputDebugStringA("! ! ! アクションUIの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// アクションUIクラスのコンストラクタ
//========================================================================
CActionUI::CActionUI(const int nPriority) :CObject2D(nPriority)
{
	// アクションUIクラスの値をクリア
	m_fade = FADE_NONE;
	m_bEnergyrock = false;
	m_bShip = false;
}

//========================================================================
// アクションUIクラスのデストラクタ
//========================================================================
CActionUI::~CActionUI()
{
}

//========================================================================
// アクションUIクラスの初期化処理
//========================================================================
HRESULT CActionUI::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	if (FAILED(CObject2D::Init(pos, fWidth, fHeight)))
	{// アクションUIの初期化に失敗した場合
		OutputDebugStringA("! ! ! アクションUIの初期化に失敗しました ! ! !\n");

		return E_FAIL;
	}

	return S_OK;
}

//========================================================================
// アクションUIクラスの終了処理
//========================================================================
void CActionUI::Uninit(void)
{
	// 終了処理
	CObject2D::Uninit();
}

//========================================================================
// アクションUIクラスの更新処理
//========================================================================
void CActionUI::Update(void)
{
	D3DXCOLOR col = GetColor();

	if (m_fade == FADE_IN)
	{// フェードイン状態
		col.a -= FADE_SPEED;			// ポリゴンを透明にしていく

		if (col.a <= 0.0f)
		{// 透明になった
			col.a = 0.0f;
		}
	}

	else if (m_fade == FADE_OUT)
	{// フェードアウト状態
		col.a += FADE_SPEED;			// ポリゴンを不透明にしていく

		if (col.a >= 1.0f)
		{// 不透明になった
			col.a = 1.0f;
		}
	}

	// 色を適用
	SetColor(col);
}

//========================================================================
// アクションUIクラスの描画処理
//========================================================================
void CActionUI::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}

//========================================================================
// フェード状態を設定
//========================================================================
void CActionUI::SetFade(const int fade)
{
	if (m_fade != fade)
	{// 違うときだけ設定
		if (fade == FADE_IN)
		{// フェードインのときは慎重に
			if (m_bEnergyrock == false && m_bShip == false)
			{// どちらも近くにないときだけ
				m_fade = fade;
			}
		}
		else
		{// フェードアウトのときは気にしない
			m_fade = fade;
		}
	}
}