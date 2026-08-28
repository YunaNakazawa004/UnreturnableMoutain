//========================================================================
// 
// ジャンプバー [ tutorial_jump.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "tutorial_jump.h"

#include "renderer.h"
#include "manager.h"
#include "sound.h"
#include "texture.h"

#include "particle3D.h"
#include "title.h"
#include "player.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define TUTORIALJUMP_WIDTH	(20.0f)				// ジャンプバーの幅
#define TUTORIALJUMP_DEPTH	(20.0f)				// ジャンプバーの奥行

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
int CTutorialJump::m_nIdxTexture = -1;				// テクスチャのインデックス

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CTutorialJump::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_nIdxTexture = pTexture->Register(NULL);

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
void CTutorialJump::Unload(void)
{
	// テクスチャのインデックスを削除
	m_nIdxTexture = -1;
}

//========================================================================
// ジャンプバークラスの生成処理
//========================================================================
CTutorialJump* CTutorialJump::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CTutorialJump* pTutorialJump = NULL;

	if (pTutorialJump == NULL)
	{// NULLチェック
		// ジャンプバーの生成
		pTutorialJump = new CTutorialJump;
	}

	if (pTutorialJump != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pTutorialJump->Init(pos, rot)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! ジャンプバーの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// テクスチャを設定
		pTutorialJump->BindTexture(m_nIdxTexture);

		// 種類を設定
		pTutorialJump->SetType(TYPE_JUMP);

		return pTutorialJump;
	}

	OutputDebugStringA("! ! ! ジャンプバーの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// ジャンプバークラスのコンストラクタ
//========================================================================
CTutorialJump::CTutorialJump(const int nPriority) :CObject3D(nPriority)
{
	// 値をクリア
	m_bClear = false;
}

//========================================================================
// ジャンプバークラスのデストラクタ
//========================================================================
CTutorialJump::~CTutorialJump()
{
}

//========================================================================
// ジャンプバークラスの初期化処理
//========================================================================
HRESULT CTutorialJump::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// 初期化処理
	CObject3D::Init(pos, TUTORIALJUMP_WIDTH, 0.0f, TUTORIALJUMP_DEPTH, CObject3D::MAINPOS_CENTER);

	// 角度を設定
	SetRotation(rot);

	// 色を設定
	SetColor(COLOR_RED);

	return S_OK;
}

//========================================================================
// ジャンプバークラスの終了処理
//========================================================================
void CTutorialJump::Uninit(void)
{
	// 終了処理
	CObject3D::Uninit();
}

//========================================================================
// ジャンプバークラスの更新処理
//========================================================================
void CTutorialJump::Update(void)
{
	if (m_bClear == false)
	{// まだクリアしていない
		CParticle3D::Create(GetPosition(), 1, 1, 4.0f, -0.01f, 0.00f,
			CEffect3D::TYPE_BLENDADD, CParticle3D::TYPE_PIN, 400, 0.3f, false, COLOR_ORANGE, 10.0f, true,
			NULL, D3DXVECTOR3(GetPosition().x, GetPosition().y + 10000.0f, GetPosition().z), 0.0001f);
	}

	CollisionPlayer();
}

//========================================================================
// ジャンプバークラスの描画処理
//========================================================================
void CTutorialJump::Draw(void)
{
	// ローカル変数宣言
	CRenderer* pRenderer = CManager::GetRenderer();			// レンダラーへのポインタ
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();			// デバイスへのポインタ

	// カリングの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// レンダーステートを加算合成にする
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 描画処理
	CObject3D::Draw();

	// レンダーステートを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// カリングの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//========================================================================
// プレイヤーとの当たり判定
//========================================================================
void CTutorialJump::CollisionPlayer(void)
{
	CPlayer* pPlayer = CTitle::GetPlayer();
	CSound* pSound = CManager::GetSound();		// サウンドを取得
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posP = pPlayer->GetPosition();
	float fHeight = pPlayer->GetHeight();

	if (pos.x - TUTORIALJUMP_WIDTH <= posP.x &&
		pos.x + TUTORIALJUMP_WIDTH >= posP.x &&
		pos.z - TUTORIALJUMP_DEPTH <= posP.z &&
		pos.z + TUTORIALJUMP_DEPTH >= posP.z)
	{// 範囲内にいるとき
		if (pos.y < posP.y + fHeight)
		{// 触った
			SetColor(COLOR_GREEN);

			if (m_bClear == false)
			{// 一回だけ
				pSound->PlaySound(CSound::SE_JUMPCLEAR);
			}

			m_bClear = true;
		}
	}
}