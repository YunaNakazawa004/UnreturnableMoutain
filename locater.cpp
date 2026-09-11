//========================================================================
// 
// ロケーター [ locater.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "locater.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "texture.h"

//************************************************************************
// マクロ定義
//************************************************************************

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
int CLocater::m_aIdxTexture[TYPE_MAX] = {};				// テクスチャのインデックス

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CLocater::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_aIdxTexture[0] = pTexture->Register("data\\TEXTURE\\UI\\arrow_0.png");
	m_aIdxTexture[1] = pTexture->Register("data\\TEXTURE\\UI\\arrow_1.png");
	m_aIdxTexture[2] = pTexture->Register("data\\TEXTURE\\UI\\arrow_2.png");
	m_aIdxTexture[3] = pTexture->Register("data\\TEXTURE\\UI\\locater.png");

	if (m_aIdxTexture[0] == -1 || m_aIdxTexture[1] == -1 || m_aIdxTexture[2] == -1 || m_aIdxTexture[3] == -1)
	{// テクスチャが設定できていない
		OutputDebugStringA("! ! ! テクスチャの設定に失敗しました ! ! !\n");

		return E_FAIL;
	}

	return S_OK;
}

//========================================================================
// テクスチャの破棄
//========================================================================
void CLocater::Unload(void)
{
	// テクスチャのインデックスを削除
	memset(&m_aIdxTexture[0], -1, sizeof m_aIdxTexture);
}

//========================================================================
// ロケータークラスの生成処理
//========================================================================
CLocater* CLocater::Create(const D3DXVECTOR3 pos, const float fWidth, const float fDepth)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CLocater* pLocater = NULL;

	if (pLocater == NULL)
	{// NULLチェック
		// ロケーターの生成
		pLocater = new CLocater;
	}

	if (pLocater != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pLocater->Init(pos, fWidth, fDepth)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! ロケーターの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// オブジェクトの種類を設定
		pLocater->SetType(TYPE_LOCATER);

		// テクスチャの割り当て
		pLocater->BindTexture(m_aIdxTexture[TYPE_FRAME]);

		return pLocater;
	}

	OutputDebugStringA("! ! ! ロケーターの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// ロケータークラスのコンストラクタ
//========================================================================
CLocater::CLocater(const int nPriority) :CObject3D(nPriority)
{
	// ロケータークラスの値をクリア
	memset(&m_aArrow[0], NULL, sizeof m_aArrow);
}

//========================================================================
// ロケータークラスのデストラクタ
//========================================================================
CLocater::~CLocater()
{
}

//========================================================================
// ロケータークラスの初期化処理
//========================================================================
HRESULT CLocater::Init(const D3DXVECTOR3 pos, const float fWidth, const float fDepth)
{
	if (FAILED(CObject3D::Init(pos, fWidth, 0.0f, fDepth)))
	{// ロケーターの初期化に失敗した場合
		OutputDebugStringA("! ! ! ロケーターの初期化に失敗しました ! ! !\n");

		return E_FAIL;
	}

	// ロケータークラスの値を初期化
	for (int nCnt = 0; nCnt < LOCATER_NUM; nCnt++)
	{
		if (m_aArrow[nCnt] == NULL)
		{// NULLチェック
			m_aArrow[nCnt] = CObject3D::Create(D3DXVECTOR3(pos.x, pos.y + 1.0f,pos.z), 10.0f, 0.0f, 20.0f, TYPE_LOCATER,
				"data\\TEXTURE\\UI\\arrow_0.png", PRIORITY_2, MAINPOS_BOTTOMMID);

			if (m_aArrow[nCnt] != NULL)
			{// NULLチェック
				m_aArrow[nCnt]->SetDisp(false);
			}
		}
	}

	return S_OK;
}

//========================================================================
// ロケータークラスの終了処理
//========================================================================
void CLocater::Uninit(void)
{
	for (int nCnt = 0; nCnt < LOCATER_NUM; nCnt++)
	{
		if (m_aArrow[nCnt] != NULL)
		{// NULLチェック
			m_aArrow[nCnt] = NULL;
		}
	}

	// 終了処理
	CObject3D::Uninit();
}

//========================================================================
// ロケータークラスの更新処理
//========================================================================
void CLocater::Update(void)
{
#ifdef _DEBUG
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();		// キーボード入力の取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();			// ジョイパッド入力の取得

	// UI隠し
	if (pInputKeyboard->GetPress(DIK_F3) == true ||
		pInputJoypad->GetPress(0, CInputJoypad::JOYKEY_BACK) == true)
	{// キーが押された
		SetDisp(false);
		
	}
	else if (pInputKeyboard->GetRelease(DIK_F3) == true ||
		pInputJoypad->GetRelease(0, CInputJoypad::JOYKEY_BACK) == true)
	{// キーが押された
		SetDisp(true);
	}
#endif
}

//========================================================================
// ロケータークラスの描画処理
//========================================================================
void CLocater::Draw(void)
{
	// ローカル変数宣言
	CRenderer* pRenderer = CManager::GetRenderer();			// レンダラーへのポインタ
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();			// デバイスへのポインタ

	// ライティングオフ
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// カリングの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);

	// 描画処理
	CObject3D::Draw();

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);

	// カリングの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ライティングオン
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//========================================================================
// ロケートする
//========================================================================
void CLocater::Locate(const D3DXVECTOR3 pos)
{
	SetPosition(pos);

	for (int nCnt = 0; nCnt < LOCATER_NUM; nCnt++)
	{
		if (m_aArrow[nCnt] != NULL)
		{// NULLチェック
			m_aArrow[nCnt]->SetPosition(pos);
		}
	}
}

//========================================================================
// 矢印の向きなどを設定
//========================================================================
void CLocater::SetArrow(const D3DXVECTOR3 pos, const int nIdx)
{
	if (m_aArrow[nIdx] == NULL)
	{// NULLチェック
		return;
	}

	D3DXVECTOR3 ArrowPos = m_aArrow[nIdx]->GetPosition();
	D3DXVECTOR3 dist = pos - ArrowPos;

	if (D3DXVec3Length(&dist) < 700.0f)
	{// 近い
		m_aArrow[nIdx]->BindTexture(m_aIdxTexture[TYPE_NEAR]);
		m_aArrow[nIdx]->SetDisp(true);
	}
	else if (D3DXVec3Length(&dist) < 1200.0f)
	{// 中くらい
		m_aArrow[nIdx]->BindTexture(m_aIdxTexture[TYPE_MIDDLE]);
		m_aArrow[nIdx]->SetDisp(true);
	}
	else if (D3DXVec3Length(&dist) < 2000.0f)
	{// 遠い
		m_aArrow[nIdx]->BindTexture(m_aIdxTexture[TYPE_FAR]);
		m_aArrow[nIdx]->SetDisp(true);
	}
	else
	{// 遠すぎ
		m_aArrow[nIdx]->BindTexture(m_aIdxTexture[TYPE_FAR]);
		m_aArrow[nIdx]->SetDisp(false);
	}

	// 向き
	D3DXVECTOR3 rot = m_aArrow[nIdx]->GetRotation();
	rot.y = atan2f(dist.x, dist.z);
	m_aArrow[nIdx]->SetRotation(rot);
}

//========================================================================
// 矢印の表示設定
//========================================================================
void CLocater::DeleteArrow(const int nIdx)
{
	if (m_aArrow[nIdx] == NULL)
	{// NULLチェック
		return;
	}

	// 終了処理
	m_aArrow[nIdx]->Uninit();
}