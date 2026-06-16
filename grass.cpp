//========================================================================
// 
// 草 [grass.cpp]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "grass.h"

#include "renderer.h"
#include "manager.h"
#include "texture.h"

#include "player.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define GRASS_WIDTH			(5.0f)				// 草の幅
#define GRASS_HEIGHT		(25.0f)				// 草の高さ
#define FIRST_ROT_X			(-D3DX_PI * 0.03f)	// 初期のX軸向き
#define PLAYER_DIST			(25.0f)				// プレイヤーとの最長距離(これより小さいときに傾く)

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
int CGrass::m_nIdxTexture = -1;				// テクスチャのインデックス

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CGrass::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_nIdxTexture = pTexture->Register("data\\TEXTURE\\grass.png");

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
void CGrass::Unload(void)
{
	// テクスチャのインデックスを削除
	m_nIdxTexture = -1;
}

//========================================================================
// 草クラスの生成処理
//========================================================================
CGrass* CGrass::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}

	CGrass* pGrass = NULL;

	if (pGrass == NULL)
	{// NULLチェック
		// 草の生成
		pGrass = new CGrass;
	}

	if (pGrass != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pGrass->Init(pos, rot)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! 草の初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// テクスチャを設定
		pGrass->BindTexture(m_nIdxTexture);

		// 種類を設定
		pGrass->SetType(TYPE_GRASS);

		return pGrass;
	}

	OutputDebugStringA("! ! ! 草の生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// 草クラスのコンストラクタ
//========================================================================
CGrass::CGrass(const int nPriority) :CObject3D(nPriority)
{
	// 値をクリア
	m_rotOff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//========================================================================
// 草クラスのデストラクタ
//========================================================================
CGrass::~CGrass()
{
}

//========================================================================
// 草クラスの初期化処理
//========================================================================
HRESULT CGrass::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// 初期化処理
	CObject3D::Init(pos, GRASS_WIDTH, GRASS_HEIGHT, 0.0f, CObject3D::MAINPOS_BOTTOMMID);

	// 角度を設定
	SetRotation(rot);
	m_rotOff = rot;

	return S_OK;
}

//========================================================================
// 草クラスの終了処理
//========================================================================
void CGrass::Uninit(void)
{
	// 終了処理
	CObject3D::Uninit();
}

//========================================================================
// 草クラスの更新処理
//========================================================================
void CGrass::Update(void)
{
	// ローカル変数
	D3DXVECTOR3 rot = CObject3D::GetRotation();
	static float fSin = 0.0f;

	// オフセットの向きを抜いた値にする
	rot -= m_rotOff;

	// ゆらゆら
	fSin += 0.005f;
	rot.x += cosf(fSin) * 0.003f;

	// プレイヤーとの当たり判定
	CollisionPlayer();

	// 向きを適用
	SetRotation(m_rotOff + rot);
}

//========================================================================
// 草クラスの描画処理
//========================================================================
void CGrass::Draw(void)
{
	// ローカル変数宣言
	CRenderer* pRenderer = CManager::GetRenderer();			// レンダラーへのポインタ
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();			// デバイスへのポインタ

	// カリングの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// 描画処理
	CObject3D::Draw();

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// カリングの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//========================================================================
// プレイヤーとの当たり判定(距離によって傾く)
//========================================================================
void CGrass::CollisionPlayer(void)
{
	// ローカル変数
	CPlayer* pPlayer = CManager::GetPlayer();			// プレイヤーの取得
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();		// プレイヤーの位置
	D3DXVECTOR3 pos = GetPosition();					// 自分の位置
	D3DXVECTOR3 rot = GetRotation();					// 自分の向き
	D3DXVECTOR3 rotDest;		// 目的の向き
	D3DXVECTOR3 dist;			// 距離
	float fDist;				// 距離

	// 距離を計算
	dist = posPlayer - pos;
	fDist = D3DXVec3Length(&dist);

	if (fDist < PLAYER_DIST)
	{// 距離が近い
		D3DXVec3Normalize(&dist, &dist);

		// 向きのオフセットに代入
		rotDest.x = -sinf(dist.z + rot.y) * ((PLAYER_DIST - fDist) * 0.05f);
		rotDest.z = sinf(dist.x + rot.y) * ((PLAYER_DIST - fDist) * 0.05f);
	}
	else
	{// 近くなくなった
		rotDest.x = 0.0f;
		rotDest.z = 0.0f;
	}

	// X向きを調整
	if (rotDest.x > D3DX_PI)
	{
		rotDest.x -= D3DX_PI * 2.0f;
	}
	else if (rotDest.x < -D3DX_PI)
	{
		rotDest.x += D3DX_PI * 2.0f;
	}

	// Z向きを調整
	if (rotDest.z > D3DX_PI)
	{
		rotDest.z -= D3DX_PI * 2.0f;
	}
	else if (rotDest.z < -D3DX_PI)
	{
		rotDest.z += D3DX_PI * 2.0f;
	}

	m_rotOff.x += (rotDest.x - m_rotOff.x) * 0.1f;
	m_rotOff.z += (rotDest.z - m_rotOff.z) * 0.1f;

	// X向きを調整
	if (m_rotOff.x > D3DX_PI)
	{
		m_rotOff.x -= D3DX_PI * 2.0f;
	}
	else if (m_rotOff.x < -D3DX_PI)
	{
		m_rotOff.x += D3DX_PI * 2.0f;
	}

	// Z向きを調整
	if (m_rotOff.z > D3DX_PI)
	{
		m_rotOff.z -= D3DX_PI * 2.0f;
	}
	else if (m_rotOff.z < -D3DX_PI)
	{
		m_rotOff.z += D3DX_PI * 2.0f;
	}
}