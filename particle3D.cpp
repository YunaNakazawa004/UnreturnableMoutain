//========================================================================
// 
// パーティクル3D [particle3D.cpp]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "particle3D.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"

#include "ObjectBillboard.h"
#include "effect3D.h"

//************************************************************************
// マクロ定義
//************************************************************************

//========================================================================
// パーティクル3Dクラスの生成処理
//========================================================================
CParticle3D* CParticle3D::Create(const D3DXVECTOR3 posP, const int nLifeP, const int nValue,
	const float fRadius, const float fAddRadius, CEffect3D::TYPE typeE, CParticle3D::TYPE typeP,
	const int nLifeE, const float fSpeedE, const D3DXCOLOR col, const float fDist,
	const bool bHoming, const D3DXVECTOR3 HomingPos, const float fSpeedHoming)
{
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}

	CParticle3D* pParticle3D = NULL;

	if (pParticle3D == NULL)
	{// NULLチェック
		// パーティクル3Dの生成
		pParticle3D = new CParticle3D;
	}

	if (pParticle3D != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pParticle3D->Init(posP, nLifeP, nValue, fRadius, fAddRadius, typeE, typeP, nLifeE, fSpeedE,
			col, fDist, bHoming, HomingPos, fSpeedHoming)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! パーティクル3Dの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// オブジェクトの種類を設定
		pParticle3D->SetType(TYPE_PARTICLE3D);

		return pParticle3D;
	}

	OutputDebugStringA("! ! ! パーティクル3Dの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// パーティクル3Dクラスのコンストラクタ
//========================================================================
CParticle3D::CParticle3D(const int nPriority) :CObject(nPriority)
{
	// パーティクル3Dクラスの値をクリア
	memset(&m_apEffect[0], NULL, sizeof m_apEffect);
	m_posP = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLifeP = 0;
	m_nValue = 0;
	m_fRadius = 0.0f;
	m_fAddRadius = 0.0f;
	m_typeE = CEffect3D::TYPE_NORMAL;
	m_typeP = CParticle3D::TYPE_NORMAL;
	m_nLifeE = 0;
	m_fSpeedE = 0.0f;
	m_col = COLOR_WHITE;
	m_fDist = 0.0f;
	m_bHoming = false;
	m_HomingPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fSpeedHoming = 0.0f;
}

//========================================================================
// パーティクル3Dクラスのデストラクタ
//========================================================================
CParticle3D::~CParticle3D()
{
}

//========================================================================
// パーティクル3Dクラスの初期化処理
//========================================================================
HRESULT CParticle3D::Init(const D3DXVECTOR3 posP, const int nLifeP, const int nValue,
	const float fRadius, const float fAddRadius, CEffect3D::TYPE typeE, CParticle3D::TYPE typeP,
	const int nLifeE, const float fSpeedE, const D3DXCOLOR col, const float fDist,
	const bool bHoming, const D3DXVECTOR3 HomingPos, const float fSpeedHoming)
{
	// パーティクル3Dクラスの値を初期化
	memset(&m_apEffect[0], NULL, sizeof m_apEffect);
	m_posP = posP;
	m_nLifeP = nLifeP;
	m_nValue = nValue;

	if (m_nValue > PARTICLE3D_EFFECT)
	{// エフェクトの最大数
		m_nValue = PARTICLE3D_EFFECT;
	}

	m_fRadius = fRadius;
	m_fAddRadius = fAddRadius;
	m_typeE = typeE;
	m_typeP = typeP;
	m_nLifeE = nLifeE;
	m_fSpeedE = fSpeedE;
	m_col = col;
	m_fDist = fDist;
	m_bHoming = bHoming;
	m_HomingPos = HomingPos;
	m_fSpeedHoming = fSpeedHoming;

	return S_OK;
}

//========================================================================
// パーティクル3Dクラスの終了処理
//========================================================================
void CParticle3D::Uninit(void)
{
	for (int nCntParticle3D = 0; nCntParticle3D < PARTICLE3D_EFFECT; nCntParticle3D++)
	{
		if (m_apEffect[nCntParticle3D] != NULL)
		{// NULLチェック
			// 終了処理
			m_apEffect[nCntParticle3D] = NULL;
		}
	}

	// 自分自身を破棄
	CObject::Release();
}

//========================================================================
// パーティクル3Dクラスの更新処理
//========================================================================
void CParticle3D::Update(void)
{
	// ローカル変数
	D3DXVECTOR3 rot = {};	// エフェクトの移動角度
	D3DXVECTOR3 pos = {};	// エフェクトの発生位置

	for (int nCntAppear = 0; nCntAppear < m_nValue; nCntAppear++)
	{// 生成するだけ繰り返す
		if (m_apEffect[nCntAppear] == NULL)
		{// NULLチェック
			switch (m_typeP)
			{
			case TYPE_NORMAL:		// 通常

				rot.x = (float)(rand() % 629 - 314) / 100.0f;		// 角度を設定
				rot.y = (float)(rand() % 629 - 314) / 100.0f;		// 角度を設定
				rot.z = (float)(rand() % 629 - 314) / 100.0f;		// 角度を設定

				D3DXVec3Normalize(&rot, &rot);						// 正規化

				pos = m_posP;

				break;

			case TYPE_HOMING:		// ホーミング

				rot.x = (float)(rand() % 629 - 314) / 100.0f;		// 角度を設定
				rot.y = (float)(rand() % 629 - 314) / 100.0f;		// 角度を設定
				rot.z = (float)(rand() % 629 - 314) / 100.0f;		// 角度を設定

				D3DXVec3Normalize(&rot, &rot);						// 正規化

				pos = m_posP + rot * m_fDist;

				break;
			}

			// 3Dエフェクトの生成
			m_apEffect[nCntAppear] = CEffect3D::Create(pos, rot, m_fSpeedE, m_nLifeE, m_fRadius, m_fAddRadius,
				m_typeE, m_col, m_bHoming, m_HomingPos, m_fSpeedHoming);
		}
	}

	// 寿命を減らす
	m_nLifeP--;

	if (m_nLifeP < 0)
	{// 寿命が尽きた
		// 終了処理
		Uninit();

		return;
	}
}

//========================================================================
// パーティクル3Dクラスの描画処理
//========================================================================
void CParticle3D::Draw(void)
{
}