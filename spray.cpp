//========================================================================
// 
// 水しぶき [ spray.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "spray.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "texture.h"

#include "game.h"
#include "watersurface.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define MAX_SIZE				(1.0f)									// 大きさ補正
#define START_ALPHA				(2)										// 透明度補正を始めるパターン数
#define MAX_ALPHA				(0.1f)									// 透明度補正

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
int CSpray::m_nIdxTexture = -1;				// テクスチャのインデックス

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CSpray::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_nIdxTexture = pTexture->Register("data\\TEXTURE\\spray000.png");

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
void CSpray::Unload(void)
{
	// テクスチャのインデックスを削除
	memset(&m_nIdxTexture, -1, sizeof m_nIdxTexture);
}

//========================================================================
// 水しぶきクラスの生成処理
//========================================================================
CSpray* CSpray::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fRadius,
	const D3DXCOLOR col)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CSpray* pSpray = NULL;

	if (pSpray == NULL)
	{// NULLチェック
		// 水しぶきの生成
		pSpray = new CSpray;
	}

	if (pSpray != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pSpray->Init(pos, move, fRadius, col)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! 水しぶきの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// オブジェクトの種類を設定
		pSpray->SetType(TYPE_EFFECT3D);

		// テクスチャを設定
		pSpray->BindTexture(m_nIdxTexture);

		return pSpray;
	}

	OutputDebugStringA("! ! ! 水しぶきの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// 水しぶきクラスのコンストラクタ
//========================================================================
CSpray::CSpray(const int nPriority) :CObjectBillboard(nPriority)
{
	// 水しぶきクラスの値をクリア
	m_move = DEFAULT_VECTER3;
	m_fRadius = 0.0f;
}

//========================================================================
// 水しぶきクラスのデストラクタ
//========================================================================
CSpray::~CSpray()
{
}

//========================================================================
// 水しぶきクラスの初期化処理
//========================================================================
HRESULT CSpray::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fRadius,
	const D3DXCOLOR col)
{
	if (FAILED(CObjectBillboard::Init(pos, fRadius, fRadius)))
	{// 水しぶきの初期化に失敗した場合
		OutputDebugStringA("! ! ! 水しぶきの初期化に失敗しました ! ! !\n");

		return E_FAIL;
	}

	// 色を設定
	SetColor(col);

	// 水しぶきクラスの値を初期化
	m_move = move;
	m_fRadius = fRadius;

	return S_OK;
}

//========================================================================
// 水しぶきクラスの終了処理
//========================================================================
void CSpray::Uninit(void)
{
	CObjectBillboard::Uninit();
}

//========================================================================
// 水しぶきクラスの更新処理
//========================================================================
void CSpray::Update(void)
{
	// ローカル変数宣言
	CDebugProc* pDebugProc = CManager::GetDebugProc();					// デバッグ表示の取得
	CWaterSurface* pWaterSurface = CGame::GetWaterSurface();			// デバッグ表示の取得
	D3DXVECTOR3 pos = CObjectBillboard::GetPosition();
	D3DXCOLOR col = CObjectBillboard::GetColor();
	m_fRadius = CObjectBillboard::GetSize();

	D3DXVECTOR2 nPolygonIdx = {};

	// 位置を更新
	pos += m_move;

	m_move.y += -0.01f;

	m_fRadius += MAX_SIZE;
	col.a -= MAX_ALPHA;

	// 位置/サイズ/色を適用
	SetPosition(pos);
	SetSize(m_fRadius);
	SetColor(col);

	// 水面のポリゴン番号を取得
	nPolygonIdx = pWaterSurface->GetPolygonIdx(pos);

	if (pWaterSurface->GetHeight(pos, nPolygonIdx) > pos.y || col.a < 0.0f)
	{// 水面より下にきた
		// 終了処理
		Uninit();

		return;
	}
}

//========================================================================
// 水しぶきクラスの描画処理
//========================================================================
void CSpray::Draw(void)
{
	// ローカル変数宣言
	CRenderer* pRenderer = CManager::GetRenderer();			// レンダラーへのポインタ
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();			// デバイスへのポインタ

	// ライティングオフ
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// 描画処理
	CObjectBillboard::Draw();

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ライティングオン
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//========================================================================
// 放射状に水しぶきさせる
//========================================================================
void CSpray::Ray(const D3DXVECTOR3 pos, const float fRadius, int type, const float fSpeed, const D3DXCOLOR col)
{
	CSpray::Create(pos, D3DXVECTOR3(sinf(D3DX_PI * 0.0f) * fSpeed, 0.1f, cosf(D3DX_PI * 0.0f) * fSpeed), fRadius, col);
	CSpray::Create(pos, D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * fSpeed, 0.1f, cosf(D3DX_PI * 0.25f) * fSpeed), fRadius, col);
	CSpray::Create(pos, D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * fSpeed, 0.1f, cosf(D3DX_PI * 0.5f) * fSpeed), fRadius, col);
	CSpray::Create(pos, D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * fSpeed, 0.1f, cosf(D3DX_PI * 0.75f) * fSpeed), fRadius, col);
	CSpray::Create(pos, D3DXVECTOR3(sinf(D3DX_PI * 1.0f) * fSpeed, 0.1f, cosf(D3DX_PI * 1.0f) * fSpeed), fRadius, col);
	CSpray::Create(pos, D3DXVECTOR3(sinf(D3DX_PI * -0.75f) * fSpeed, 0.1f, cosf(D3DX_PI * -0.75f) * fSpeed), fRadius, col);
	CSpray::Create(pos, D3DXVECTOR3(sinf(D3DX_PI * -0.5f) * fSpeed, 0.1f, cosf(D3DX_PI * -0.5f) * fSpeed), fRadius, col);
	CSpray::Create(pos, D3DXVECTOR3(sinf(D3DX_PI * -0.25f) * fSpeed, 0.1f, cosf(D3DX_PI * -0.25f) * fSpeed), fRadius, col);
}