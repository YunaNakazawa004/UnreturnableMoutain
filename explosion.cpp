//========================================================================
// 
// 爆発 [ explosion.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "explosion.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "texture.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define TEXTURE_WIDTH			(0.125f)								// テクスチャパターンの幅
#define TEXTURE_NUM				(8)										// テクスチャパターンの列数
#define MAX_TIME				(4)										// テクスチャ切り替えのカウント
#define MAX_SIZE				(1.0f)									// 大きさ補正
#define START_ALPHA				(2)										// 透明度補正を始めるパターン数
#define MAX_ALPHA				(0.1f)									// 透明度補正

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
int CExplosion::m_aIdxTexture[TYPE_MAX] = {};				// テクスチャのインデックス

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CExplosion::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_aIdxTexture[0] = pTexture->Register("data\\TEXTURE\\explosion000.png");
	m_aIdxTexture[1] = pTexture->Register("data\\TEXTURE\\explosion001.png");
	m_aIdxTexture[2] = pTexture->Register("data\\TEXTURE\\explosion002.png");

	if (m_aIdxTexture[0] == -1 || m_aIdxTexture[1] == -1 || m_aIdxTexture[2] == -1)
	{// テクスチャが設定できていない
		OutputDebugStringA("! ! ! テクスチャの設定に失敗しました ! ! !\n");

		return E_FAIL;
	}

	return S_OK;
}

//========================================================================
// テクスチャの破棄
//========================================================================
void CExplosion::Unload(void)
{
	// テクスチャのインデックスを削除
	memset(&m_aIdxTexture[0], -1, sizeof m_aIdxTexture);
}

//========================================================================
// 爆発クラスの生成処理
//========================================================================
CExplosion* CExplosion::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fRadius,
	int type, const D3DXCOLOR col)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CExplosion* pExplosion = NULL;

	if (pExplosion == NULL)
	{// NULLチェック
		// 爆発の生成
		pExplosion = new CExplosion;
	}

	if (pExplosion != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pExplosion->Init(pos, move, fRadius, type, col)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! 爆発の初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// オブジェクトの種類を設定
		pExplosion->SetType(TYPE_EFFECT3D);

		// テクスチャを設定
		pExplosion->BindTexture(m_aIdxTexture[type]);

		return pExplosion;
	}

	OutputDebugStringA("! ! ! 爆発の生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// 爆発クラスのコンストラクタ
//========================================================================
CExplosion::CExplosion(const int nPriority) :CObjectBillboard(nPriority)
{
	// 爆発クラスの値をクリア
	m_move = DEFAULT_VECTER3;
	m_fRadius = 0.0f;
	m_type = TYPE_0;
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
}

//========================================================================
// 爆発クラスのデストラクタ
//========================================================================
CExplosion::~CExplosion()
{
}

//========================================================================
// 爆発クラスの初期化処理
//========================================================================
HRESULT CExplosion::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fRadius,
	int type, const D3DXCOLOR col)
{
	if (FAILED(CObjectBillboard::Init(pos, fRadius, fRadius)))
	{// 爆発の初期化に失敗した場合
		OutputDebugStringA("! ! ! 爆発の初期化に失敗しました ! ! !\n");

		return E_FAIL;
	}

	// 色を設定
	SetColor(col);

	// 爆発クラスの値を初期化
	m_move = move;
	m_fRadius = fRadius;
	m_type = type;
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;

	return S_OK;
}

//========================================================================
// 爆発クラスの終了処理
//========================================================================
void CExplosion::Uninit(void)
{
	CObjectBillboard::Uninit();
}

//========================================================================
// 爆発クラスの更新処理
//========================================================================
void CExplosion::Update(void)
{
	// ローカル変数宣言
	CDebugProc* pDebugProc = CManager::GetDebugProc();					// デバッグ表示の取得
	D3DXVECTOR3 pos = CObjectBillboard::GetPosition();
	D3DXCOLOR col = CObjectBillboard::GetColor();
	m_fRadius = CObjectBillboard::GetSize();

	// 位置を更新
	pos += m_move;

	m_nCounterAnim++;		// カウンターを加算

	if (m_nCounterAnim % MAX_TIME == 0)
	{// 一定時間経過した
		m_fRadius += MAX_SIZE;

		if (m_nPatternAnim > START_ALPHA)
		{
			col.a -= MAX_ALPHA;
		}

		m_nPatternAnim++;	// パターンNO.を更新
	}

	// 位置/サイズ/色を適用
	SetPosition(pos);
	SetSize(m_fRadius);
	SetColor(col);
	SetTexUV(m_nPatternAnim, TEXTURE_NUM, 1, 0.0f, 0.0f);

	if (m_nPatternAnim >= TEXTURE_NUM)
	{// 総パターン数を超えた
		// 終了処理
		Uninit();

		return;
	}
}

//========================================================================
// 爆発クラスの描画処理
//========================================================================
void CExplosion::Draw(void)
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
// 放射状に爆発させる
//========================================================================
void CExplosion::Ray(const D3DXVECTOR3 pos, const float fRadius, int type, const float fSpeed, const D3DXCOLOR col)
{
	CExplosion::Create(pos, D3DXVECTOR3(sinf(D3DX_PI * 0.0f) * fSpeed, 0.1f, cosf(D3DX_PI * 0.0f) * fSpeed), fRadius, type, col);
	CExplosion::Create(pos, D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * fSpeed, 0.1f, cosf(D3DX_PI * 0.25f) * fSpeed), fRadius, type, col);
	CExplosion::Create(pos, D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * fSpeed, 0.1f, cosf(D3DX_PI * 0.5f) * fSpeed), fRadius, type, col);
	CExplosion::Create(pos, D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * fSpeed, 0.1f, cosf(D3DX_PI * 0.75f) * fSpeed), fRadius, type, col);
	CExplosion::Create(pos, D3DXVECTOR3(sinf(D3DX_PI * 1.0f) * fSpeed, 0.1f, cosf(D3DX_PI * 1.0f) * fSpeed), fRadius, type, col);
	CExplosion::Create(pos, D3DXVECTOR3(sinf(D3DX_PI * -0.75f) * fSpeed, 0.1f, cosf(D3DX_PI * -0.75f) * fSpeed), fRadius, type, col);
	CExplosion::Create(pos, D3DXVECTOR3(sinf(D3DX_PI * -0.5f) * fSpeed, 0.1f, cosf(D3DX_PI * -0.5f) * fSpeed), fRadius, type, col);
	CExplosion::Create(pos, D3DXVECTOR3(sinf(D3DX_PI * -0.25f) * fSpeed, 0.1f, cosf(D3DX_PI * -0.25f) * fSpeed), fRadius, type, col);
}