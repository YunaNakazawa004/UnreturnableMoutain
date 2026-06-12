//========================================================================
// 
// エネルギー鉱物 [energyrock.cpp]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "energyrock.h"

#include "renderer.h"
#include "manager.h"
#include "debugproc.h"

//************************************************************************
// マクロ定義
//************************************************************************

//========================================================================
// エネルギー鉱物クラスの生成処理
//========================================================================
CEnergyRock* CEnergyRock::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}

	CEnergyRock* pEnergyRock = NULL;

	if (pEnergyRock == NULL)
	{// NULLチェック
		// エネルギー鉱物の生成
		pEnergyRock = new CEnergyRock;
	}

	if (pEnergyRock != NULL)
	{// NULLチェック
		// モデルファイル設定
		pEnergyRock->BindModel("data\\MODEL\\energyrock.x");

		// 初期化処理
		if (FAILED(pEnergyRock->Init(pos, rot)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! エネルギー鉱物の初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// 種類を設定
		pEnergyRock->SetType(TYPE_ENERGYROCK);

		return pEnergyRock;
	}

	OutputDebugStringA("! ! ! エネルギー鉱物の生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// エネルギー鉱物クラスのコンストラクタ
//========================================================================
CEnergyRock::CEnergyRock(const int nPriority) :CObjectX(nPriority)
{
	// エネルギー鉱物クラスの値をクリア
}

//========================================================================
// エネルギー鉱物クラスのデストラクタ
//========================================================================
CEnergyRock::~CEnergyRock()
{
}

//========================================================================
// エネルギー鉱物の初期化処理
//========================================================================
HRESULT CEnergyRock::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// 初期化処理
	CObjectX::Init(pos);

	// 位置/向きを設定
	CObjectX::SetPosition(pos);
	CObjectX::SetRotation(rot);

	return S_OK;
}

//========================================================================
// エネルギー鉱物の終了処理
//========================================================================
void CEnergyRock::Uninit(void)
{
	// 終了処理
	CObjectX::Uninit();
}

//========================================================================
// エネルギー鉱物の更新処理
//========================================================================
void CEnergyRock::Update(void)
{
}

//========================================================================
// エネルギー鉱物の描画処理
//========================================================================
void CEnergyRock::Draw(void)
{
	// 描画処理
	CObjectX::Draw();
}

//========================================================================
// 当たり判定
//========================================================================
bool CEnergyRock::Collision(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove,
	const float fRadius, const float fHeight)
{
	CDebugProc* pDebugProc = CManager::GetDebugProc();					// デバッグ表示の取得
	bool bLand = false;		// 着地しているか
	int nCntLand = 0;	// 辺の内側に入った数(4回入っていれば、オブジェクトの内側にいる)

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		D3DXVECTOR3 start, end;			// 始点/終点
		float fXS, fZS, fXE, fZE;		// 始点のXZ座標/終点のXZ座標

		D3DXVECTOR3 vecLine, vecMove, vecToPos, vecToPosOld, vecNor, vecMoveRef, vecMoveDest;		// 各ベクトル
		D3DXVECTOR3 vecLineW, posDest;

		// 位置/向きを取得
		D3DXVECTOR3 pos = CObjectX::GetPosition();
		D3DXVECTOR3 rot = CObjectX::GetRotation();

		D3DXVECTOR3 VtxMax = CObjectX::GetVtxMax();
		D3DXVECTOR3 VtxMin = CObjectX::GetVtxMin();

		float fRate, fDot;		// 交点の割合/内積
		float fXLength = VtxMax.x - VtxMin.x;		// X方向の長さ
		float fZLength = VtxMax.z - VtxMin.z;		// Z方向の長さ

		float fOffXS = (nCnt == 0 || nCnt == 3) ? +(fXLength * 0.5f + fRadius) : -(fXLength * 0.5f + fRadius);		// 向き0のときの座標
		float fOffZS = (nCnt == 0 || nCnt == 1) ? +(fZLength * 0.5f + fRadius) : -(fZLength * 0.5f + fRadius);		// 向き0のときの座標
		float fOffXE = (nCnt == 1 || nCnt == 0) ? +(fXLength * 0.5f + fRadius) : -(fXLength * 0.5f + fRadius);		// 向き0のときの座標
		float fOffZE = (nCnt == 1 || nCnt == 2) ? +(fZLength * 0.5f + fRadius) : -(fZLength * 0.5f + fRadius);		// 向き0のときの座標
		
		float fSin = sinf(-rot.y);		// 現在向きのsin
		float fCos = cosf(-rot.y);		// 現在向きのcos

		// Y向きを調整
		if (rot.y > D3DX_PI)
		{
			rot.y -= D3DX_PI * 2.0f;
		}
		else if (rot.y < -D3DX_PI)
		{
			rot.y += D3DX_PI * 2.0f;
		}

		// 始点のXZ座標
		fXS = fOffXS * fCos - fOffZS * fSin;
		fZS = fOffXS * fSin + fOffZS * fCos;

		// 終点のXZ座標
		fXE = fOffXE * fCos - fOffZE * fSin;
		fZE = fOffXE * fSin + fOffZE * fCos;

		// 始点
		start.x = pos.x + fXS;
		start.y = 0.0f;
		start.z = pos.z + fZS;

		// 終点
		end.x = pos.x + fXE;
		end.y = 0.0f;
		end.z = pos.z + fZE;

		// 境界線ベクトル
		vecLine.x = (end.x) - (start.x);
		vecLine.y = 0.0f;
		vecLine.z = (end.z) - (start.z);

		// 移動ベクトル
		vecMove.x = pPos->x - pPosOld->x;
		vecMove.y = 0.0f;
		vecMove.z = pPos->z - pPosOld->z;

		// 始点から現在位置へのベクトル
		vecToPos.x = pPos->x - (start.x);
		vecToPos.y = 0.0f;
		vecToPos.z = pPos->z - (start.z);

		// 始点から前回位置へのベクトル
		vecToPosOld.x = pPosOld->x - (start.x);
		vecToPosOld.y = 0.0f;
		vecToPosOld.z = pPosOld->z - (start.z);

		// 法線ベクトル
		vecNor.x = -vecLine.z;
		vecNor.y = 0.0f;
		vecNor.z = vecLine.x;
		D3DXVec3Normalize(&vecNor, &vecNor);

		// 内積
		fDot = (-vecMove.x * vecNor.x) + (-vecMove.z * vecNor.z);

		// 交点の割合
		fRate = ((vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z)) /
			((vecLine.z * vecMove.x) - (vecLine.x * vecMove.z));

		// 反射後の移動ベクトル
		vecMoveRef.x = vecMove.x + ((vecNor.x * fDot) * 2);
		vecMoveRef.y = 0.0f;
		vecMoveRef.z = vecMove.z + ((vecNor.z * fDot) * 2);

		// 現在の外積
		float fPosLine = (float)((int)(((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z)) * 1.0f) / (int)1);

		// 前回の外積
		float fPosOldLine = (float)((int)(((vecLine.z * vecToPosOld.x) - (vecLine.x * vecToPosOld.z)) * 1.0f) / (int)1);

		pDebugProc->Print("%f\n", fRate);

		if (fRate >= 0.0f && fRate <= 1.0f)
		{// 交点の割合が範囲内
			if (fPosLine > 0.0f && (fPosOldLine <= 0.0f))
			{// 交差した
				if((pos.y + VtxMin.y - fHeight <= pPos->y) &&
					(pos.y + VtxMax.y >= pPos->y))
				{// 高さが合っている
					// 現在の移動ベクトル
					D3DXVECTOR3 move = vecMove;
					move.y = 0.0f;

					float fDotN = D3DXVec3Dot(&move, &vecNor);

					if (fDotN < 0.0f)
					{// 壁に向かっているときだけ法線成分を消す
						vecMoveDest = move - (vecNor * fDotN);
					}

					// 位置をずらす
					pPos->x = start.x + (vecLine.x * fRate) + vecMoveDest.x;
					pPos->z = start.z + (vecLine.z * fRate) + vecMoveDest.z;
				}
			}
		}

		if (fPosLine > 0.0f)
		{// 今の位置が内側にいる
			nCntLand++;

			if (nCntLand == 4)
			{// 全ての内側に入っていたら
				if ((pos.y + VtxMin.y - fHeight <= pPos->y) &&
					(pos.y + VtxMax.y >= pPos->y))
				{// 範囲内
					bLand = true;		// 着地フラグ
				}

				// 位置調整
				if ((pos.y + VtxMin.y - fHeight >= pPosOld->y) &&
					(pos.y + VtxMin.y - fHeight <= pPos->y))
				{// 下からの当たり判定
					pPos->y = pos.y + VtxMin.y - fHeight;
					pMove->y = -0.5f;							// 移動量を0にする
				}
				else if ((pos.y + VtxMax.y <= pPosOld->y) &&
					(pos.y + VtxMax.y >= pPos->y))
				{// 上からの当たり判定
					pPos->y = pos.y + VtxMax.y;
					pMove->y = 0.0f;							// 移動量を0にする
				}
			}
		}
	}

	return bLand;
}