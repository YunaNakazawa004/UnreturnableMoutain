//========================================================================
// 
// 草 [ grass.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "grass.h"

#include "renderer.h"
#include "manager.h"
#include "texture.h"

#include "game.h"
#include "mountain.h"
#include "beach.h"
#include "player.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define GRASS_WIDTH			(5.0f)				// 草の幅
#define GRASS_HEIGHT		(25.0f)				// 草の高さ
#define SHAKE_SPEED			(0.005f)			// 揺れるスピード
#define SHAKE_VALUE			(0.0005f)			// どれくらい揺れるか
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
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

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
	m_rotOff = DEFAULT_VECTER3;
	m_fShake = 0.0f;
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
	m_fShake = 0.0f;

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
	CMountain* pMountain = CGame::GetMountain();					// 山の取得
	CBeach* pBeach = CGame::GetBeach();								// 砂浜の取得
	D3DXVECTOR3 pos = CObject3D::GetPosition();
	D3DXVECTOR3 rot = CObject3D::GetRotation();

	float fHeightM = 0.0f;		// 山の地面の高さ
	D3DXVECTOR2 polygonIdxM = { -1.0f,-1.0f };		// ポリゴン番号

	float fHeightB = 0.0f;		// 砂浜の地面の高さ
	D3DXVECTOR2 polygonIdxB = { -1.0f,-1.0f };		// ポリゴン番号

	float fHeight = 0.0f;		// 地面の高さ

	// オフセットの向きを抜いた値にする
	rot -= m_rotOff;

	// ゆらゆら
	m_fShake += SHAKE_SPEED;
	rot.x += cosf(m_fShake) * SHAKE_VALUE;

	// プレイヤーとの当たり判定
	CollisionPlayer();

	// 山のポリゴン番号を取得
	polygonIdxM = pMountain->GetPolygonIdx(pos);

	// 山の地面の高さを取得
	fHeightM = pMountain->GetHeight(pos, polygonIdxM);

	// 砂浜のポリゴン番号を取得
	polygonIdxB = pMountain->GetPolygonIdx(pos);

	// 砂浜の地面の高さを取得
	fHeightB = pMountain->GetHeight(pos, polygonIdxB);

	// 最終的な高さ
	fHeight = (fHeightM >= fHeightB) ? fHeightM : fHeightB;

	if (fHeight == ERROR_HEIGHT)
	{// 無効な高さだったら
		fHeight = 0.0f;
	}

	// 高さを代入
	pos.y = fHeight;

	// 位置/向きを適用
	SetPosition(pos);
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
	CPlayer* pPlayer = CGame::GetPlayer();				// プレイヤーの取得
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();		// プレイヤーの位置
	D3DXVECTOR3 pos = GetPosition();					// 自分の位置
	D3DXVECTOR3 rot = GetRotation();					// 自分の向き
	D3DXVECTOR3 rotDest;		// 目的の向き
	D3DXVECTOR3 dist;			// 距離
	float fDist;				// 距離

	// 距離を計算
	dist = pos - posPlayer;
	fDist = D3DXVec3Length(&dist);

	if (fDist < PLAYER_DIST)
	{// 距離が近い
		D3DXVec3Normalize(&dist, &dist);

		// 向きのオフセットに代入
		rotDest.x = sinf(dist.z) * cosf(rot.y) * ((PLAYER_DIST - fDist) * 0.05f);
		rotDest.z = sinf(dist.x) * cosf(rot.y + D3DX_PI) * ((PLAYER_DIST - fDist) * 0.05f);
	}
	else
	{// 近くなくなった
		rotDest.x = 0.0f;
		rotDest.z = 0.0f;
	}

	// X向きを調整
	CorrectAngle(&rotDest.x, rotDest.x);

	// Z向きを調整
	CorrectAngle(&rotDest.z, rotDest.z);

	m_rotOff.x += (rotDest.x - m_rotOff.x) * 0.1f;
	m_rotOff.z += (rotDest.z - m_rotOff.z) * 0.1f;

	// X向きを調整
	CorrectAngle(&m_rotOff.x, m_rotOff.x);

	// Z向きを調整
	CorrectAngle(&m_rotOff.z, m_rotOff.z);
}