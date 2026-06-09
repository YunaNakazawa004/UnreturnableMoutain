//========================================================================
// 
// 3Dエフェクト [effect3D.cpp]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "effect3D.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "texture.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define MOVE				(5.0f)						// 移動量
#define ALPHA_MINUS			(0.1f)						// アルファ値の減衰
#define SIZE_MINUS			(0.1f)						// 半径の変更量
#define EFFECT_INERTIA		(0.017f)					// 慣性

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
int CEffect3D::m_nIdxTexture = -1;				// テクスチャのインデックス

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CEffect3D::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_nIdxTexture = pTexture->Register("data\\TEXTURE\\effect000.jpg");

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
void CEffect3D::Unload(void)
{
	// テクスチャのインデックスを削除
	m_nIdxTexture = -1;
}

//========================================================================
// 3Dエフェクトクラスの生成処理
//========================================================================
CEffect3D* CEffect3D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fSpeed,
	const int nLife, const float fRadius, const float fAddRadius, CEffect3D::TYPE type,
	const D3DXCOLOR col, const bool bHoming, const D3DXVECTOR3 HomingPos, const float fSpeedHoming)
{
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}

	CEffect3D* pEffect3D = NULL;

	if (pEffect3D == NULL)
	{// NULLチェック
		// 3Dエフェクトの生成
		pEffect3D = new CEffect3D;
	}

	if (pEffect3D != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pEffect3D->Init(pos, move, fSpeed, nLife, fRadius, fAddRadius, type,
			col, bHoming, HomingPos, fSpeedHoming)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! 3Dエフェクトの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// オブジェクトの種類を設定
		pEffect3D->SetType(TYPE_EFFECT3D);

		if (type == CEffect3D::TYPE_NORMAL || type == CEffect3D::TYPE_BLENDADD)
		{// テクスチャありの場合
			// テクスチャの割り当て
			pEffect3D->BindTexture(m_nIdxTexture);
		}

		return pEffect3D;
	}

	OutputDebugStringA("! ! ! 3Dエフェクトの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// 3Dエフェクトクラスのコンストラクタ
//========================================================================
CEffect3D::CEffect3D(const int nPriority) :CObjectBillboard(nPriority)
{
	// 3Dエフェクトクラスの値をクリア
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fSpeed = 0.0f;
	m_nLife = 0;
	m_fRadius = 0.0f;
	m_fAddRadius = 0.0f;
	m_type = TYPE_NORMAL;
	m_bHoming = false;
	m_HomingPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fSpeedHoming = 0.0f;
}

//========================================================================
// 3Dエフェクトクラスのデストラクタ
//========================================================================
CEffect3D::~CEffect3D()
{
}

//========================================================================
// 3Dエフェクトクラスの初期化処理
//========================================================================
HRESULT CEffect3D::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const float fSpeed,
	const int nLife, const float fRadius, const float fAddRadius, CEffect3D::TYPE type,
	const D3DXCOLOR col, const bool bHoming, const D3DXVECTOR3 HomingPos, const float fSpeedHoming)
{
	if (FAILED(CObjectBillboard::Init(pos, fRadius, fRadius)))
	{// 3Dエフェクトの初期化に失敗した場合
		OutputDebugStringA("! ! ! 3Dエフェクトの初期化に失敗しました ! ! !\n");

		return E_FAIL;
	}

	// 色を設定
	SetColor(col);

	// 3Dエフェクトクラスの値を初期化
	m_move = move;
	m_fSpeed = fSpeed;
	m_nLife = nLife;
	m_fRadius = fRadius;
	m_fAddRadius = fAddRadius;
	m_type = type;
	m_bHoming = bHoming;
	m_HomingPos = HomingPos;
	m_fSpeedHoming = fSpeedHoming;

	return S_OK;
}

//========================================================================
// 3Dエフェクトクラスの終了処理
//========================================================================
void CEffect3D::Uninit(void)
{
	CObjectBillboard::Uninit();
}

//========================================================================
// 3Dエフェクトクラスの更新処理
//========================================================================
void CEffect3D::Update(void)
{
	// ローカル変数宣言
	CDebugProc* pDebugProc = CManager::GetDebugProc();					// デバッグ表示の取得
	D3DXVECTOR3 pos = CObjectBillboard::GetPosition();
	D3DXCOLOR col = CObjectBillboard::GetColor();
	m_fRadius = CObjectBillboard::GetSize();

	// 位置を更新
	pos += m_move * m_fSpeed;

	if (m_bHoming == true)
	{// ホーミングする場合
		pos += (m_HomingPos - pos) * m_fSpeedHoming;

		if (pos == m_HomingPos)
		{// 着いたら消す
			// 終了処理
			Uninit();

			return;
		}
	}

	// 慣性
	m_fSpeed += (0.0f - m_fSpeed) * EFFECT_INERTIA;

	// サイズを変更する
	m_fRadius += m_fAddRadius;

	// 不透明度を減らす
	col.a -= ALPHA_MINUS;

	// 位置/サイズ/色を適用
	SetPosition(pos);
	SetSize(m_fRadius);
	SetColor(col);

	// 寿命を減らす
	m_nLife--;

	if (m_fRadius < 0.0f || col.a < 0.0f || m_nLife < 0)
	{// 3Dエフェクトが消える場合
		// 終了処理
		Uninit();

		return;
	}
}

//========================================================================
// 3Dエフェクトクラスの描画処理
//========================================================================
void CEffect3D::Draw(void)
{
	// ローカル変数宣言
	CRenderer* pRenderer = CManager::GetRenderer();			// レンダラーへのポインタ
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();			// デバイスへのポインタ

	// ライティングオフ
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (m_type == CEffect3D::TYPE_BLENDADD || m_type == CEffect3D::TYPE_BLENDADD_NULL)
	{// 加算合成
		// レンダーステートを加算合成にする
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	CObjectBillboard::Draw();

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// レンダーステートを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ライティングオン
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}