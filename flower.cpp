//========================================================================
// 
// 花 [ flower.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "flower.h"

#include "renderer.h"
#include "manager.h"
#include "game.h"

#include "mountain.h"
#include "beach.h"
#include "player.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define SHAKE_SPEED			(0.005f)			// 揺れるスピード
#define SHAKE_VALUE			(0.0005f)			// どれくらい揺れるか
#define PLAYER_DIST			(25.0f)				// プレイヤーとの最長距離(これより小さいときに傾く)

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
const char* CFlower::m_aFilename[FLOWER_NUM] = {		// 花のファイル名
	"data\\MODEL\\MAP_OBJECT\\flower_white.x",
	"data\\MODEL\\MAP_OBJECT\\flower_red.x",
	"data\\MODEL\\MAP_OBJECT\\flower_blue.x",
};

//========================================================================
// 花クラスの生成処理
//========================================================================
CFlower* CFlower::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CFlower* pFlower = NULL;

	if (pFlower == NULL)
	{// NULLチェック
		// 花の生成
		pFlower = new CFlower;
	}

	if (pFlower != NULL)
	{// NULLチェック
		// モデルファイル設定
		pFlower->BindModel(m_aFilename[rand() % FLOWER_NUM]);

		// 初期化処理
		if (FAILED(pFlower->Init(pos, rot)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! 花の初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// 種類を設定
		pFlower->SetType(TYPE_FLOWER);

		return pFlower;
	}

	OutputDebugStringA("! ! ! 花の生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// 花クラスのコンストラクタ
//========================================================================
CFlower::CFlower(const int nPriority) :CObjectX(nPriority)
{
	// 花クラスの値をクリア
	m_rotOff = DEFAULT_VECTER3;
	m_fShake = 0.0f;
}

//========================================================================
// 花クラスのデストラクタ
//========================================================================
CFlower::~CFlower()
{
}

//========================================================================
// 花の初期化処理
//========================================================================
HRESULT CFlower::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// 初期化処理
	CObjectX::Init(pos);

	// 位置/向きを設定
	CObjectX::SetPosition(pos);
	CObjectX::SetRotation(rot);

	// 角度を設定
	m_rotOff = rot;
	m_fShake = 0.0f;

	CMountain* pMountain = CGame::GetMountain();					// 山の取得
	CBeach* pBeach = CGame::GetBeach();								// 砂浜の取得
	D3DXVECTOR3 posC = GetPosition();

	float fHeightM = 0.0f;		// 山の地面の高さ
	D3DXVECTOR2 polygonIdxM = { -1.0f,-1.0f };		// ポリゴン番号

	float fHeightB = 0.0f;		// 砂浜の地面の高さ
	D3DXVECTOR2 polygonIdxB = { -1.0f,-1.0f };		// ポリゴン番号

	float fHeight = 0.0f;		// 地面の高さ

	// 山のポリゴン番号を取得
	polygonIdxM = pMountain->GetPolygonIdx(posC);

	// 山の地面の高さを取得
	fHeightM = pMountain->GetHeight(posC, polygonIdxM);

	// 砂浜のポリゴン番号を取得
	polygonIdxB = pBeach->GetPolygonIdx(posC);

	// 砂浜の地面の高さを取得
	fHeightB = pBeach->GetHeight(posC, polygonIdxB);

	// 最終的な高さ
	fHeight = (fHeightM >= fHeightB) ? fHeightM : fHeightB;

	if (fHeight == ERROR_HEIGHT)
	{// 無効な高さだったら
		fHeight = 0.0f;
	}

	// 高さを代入
	posC.y = fHeight;

	// 位置を適用
	SetPosition(posC);

	return S_OK;
}

//========================================================================
// 花の終了処理
//========================================================================
void CFlower::Uninit(void)
{
	// 終了処理
	CObjectX::Uninit();
}

//========================================================================
// 花の更新処理
//========================================================================
void CFlower::Update(void)
{
	// ローカル変数
	D3DXVECTOR3 rot = CObjectX::GetRotation();

	// オフセットの向きを抜いた値にする
	rot -= m_rotOff;

	// ゆらゆら
	m_fShake += SHAKE_SPEED;
	rot.x += cosf(m_fShake) * SHAKE_VALUE;

	// プレイヤーとの当たり判定
	CollisionPlayer();

	// 位置/向きを適用
	SetRotation(m_rotOff + rot);
}

//========================================================================
// 花の描画処理
//========================================================================
void CFlower::Draw(void)
{
	// 描画処理
	CObjectX::Draw();
}

//========================================================================
// プレイヤーとの当たり判定(距離によって傾く)
//========================================================================
void CFlower::CollisionPlayer(void)
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