//=============================================================================
// 
// 画面遷移 [ transition.cpp ]
// Author : Nakazawa Yuna
// 
//=============================================================================
#include "transition.h"

#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "camera.h"

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define WHT_VISIBLE_VTX			(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))		// 白いポリゴン(不透明)
#define WHT_INVISIBLE_VTX		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f))		// 白いポリゴン(透明)
#define FADE_SPEED				(0.03f)									// 画面遷移の速さ

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
int CTransition::m_nIdxTexture = -1;				// テクスチャのインデックス

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CTransition::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_nIdxTexture = pTexture->Register("data\\TEXTURE\\UI\\bg.png");

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
void CTransition::Unload(void)
{
	// テクスチャのインデックスを削除
	m_nIdxTexture = -1;
}

//=============================================================================
// 画面遷移の生成処理
//=============================================================================
CTransition* CTransition::Create(void)
{
	CTransition* pTransition = NULL;

	if (pTransition == NULL)
	{// NULLチェック
		// 画面遷移の生成
		pTransition = new CTransition;
	}

	if (pTransition != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pTransition->Init()))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! 画面遷移の初期化に失敗しました ! ! !\n");

			return NULL;
		}

		return pTransition;
	}

	OutputDebugStringA("! ! ! 画面遷移の生成に失敗しました ! ! !\n");

	return NULL;
}

//=============================================================================
// 画面遷移のコンストラクタ
//=============================================================================
CTransition::CTransition()
{
	// 値をクリア
	m_pVtxBuff = NULL;
	m_fade = FADE_NONE;
	m_modeNext = CScene::MODE_TITLE;
	m_col = COLOR_WHITE;
	m_pMesh = NULL;
	m_pBuffMat = NULL;
	memset(&m_apTexture[0], -1, sizeof m_apTexture);
	m_dwNumMat = 0;
	m_pos = DEFAULT_VECTER3;
	m_rot = DEFAULT_VECTER3;
}

//=============================================================================
// 画面遷移のデストラクタ
//=============================================================================
CTransition::~CTransition()
{
}

//=============================================================================
// 画面遷移の初期化処理
//=============================================================================
HRESULT CTransition::Init(void)
{
	// ローカル変数宣言
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();			// レンダラーへのポインタ
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスへのポインタ
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ
	D3DXMATERIAL* pMat;

	// 初期化
	m_fade = FADE_NONE;						// フェードイン状態に
	m_modeNext = CScene::MODE_TITLE;		// 次の画面(モード)を設定
	m_col = WHT_INVISIBLE_VTX;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// モデルの初期化
	m_pos = D3DXVECTOR3(-200.0f, 30.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX("data\\MODEL\\rocket000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh)))
	{// ファイルの読み込みに失敗
		OutputDebugStringA("! ! ! Xファイルの読み込みに失敗しました ! ! !\n");

		return E_FAIL;
	}

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// 色を保存
	m_col = pMat->MatD3D.Diffuse;

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{// テクスチャファイルが存在する
			m_apTexture[nCntMat] = pTexture->Register(pMat[nCntMat].pTextureFilename);
		}
	}

	return S_OK;
}

//=============================================================================
// 画面遷移の終了処理
//=============================================================================
void CTransition::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// メッシュの破棄
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// マテリアルの破棄
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	// テクスチャの破棄
	for (int nCntModel = 0; nCntModel < MAX_MODEL_TEXTURE; nCntModel++)
	{
		m_apTexture[nCntModel] = -1;
	}
}

//=============================================================================
// 画面遷移の更新処理
//=============================================================================
void CTransition::Update(void)
{
	CCamera* pCamera = CManager::GetCamera();				// カメラへのポインタ

	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{// フェードイン状態
			m_col.a -= FADE_SPEED;			// ポリゴンを透明にしていく

			if (m_col.a <= 0.0f)
			{// 透明になった
				m_col.a = 0.0f;
				m_fade = FADE_NONE;			// 何もしていない状態にする
			}
		}
		else if (m_fade == FADE_OUT)
		{// フェードアウト状態
			m_col.a += FADE_SPEED;			// ポリゴンを不透明にしていく

			if (m_col.a >= 1.0f)
			{// 不透明になった
				m_col.a = 1.0f;

				m_pos.x += 3.0f;

				// カメラを変更
				pCamera->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -300.0f), DEFAULT_VECTER3, DEFAULT_VECTER3, CCamera::TYPE_STOP);

				if (m_pos.x > 200.0f)
				{// 一定の距離を進んだ
					m_fade = FADE_IN;			// フェードイン状態にする

					// モードの設定
					CManager::SetMode(m_modeNext);
				}
			}
		}
	}
	else
	{
		m_col.a = 0.0f;
	}

	// ローカル変数宣言
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// テクスチャ座標の設定
	pVtx[0].tex.x += 0.001f;
	pVtx[1].tex.x += 0.001f;
	pVtx[2].tex.x += 0.001f;
	pVtx[3].tex.x += 0.001f;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 画面遷移の描画処理
//=============================================================================
void CTransition::Draw(void)
{
	// デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();			// レンダラーへのポインタ
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスへのポインタ
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;							// マテリアルデータへのポインタ

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,					// 描画する最初の頂点インデックス
		2);					// 描画するプリミティブ数

	if (m_col.a == 1.0f)
	{// 画面遷移中
		// Zテストを無効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// モデルの描画
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, pTexture->GetAddress(m_apTexture[nCntMat]));

			// モデルパーツの描画
			m_pMesh->DrawSubset(nCntMat);
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);

		// Zテストを有効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

//=============================================================================
// 画面遷移の設定処理
//=============================================================================
void CTransition::SetTransition(CScene::MODE modenext)
{
	if (m_fade == FADE_NONE)
	{
		m_fade = FADE_OUT;					// フェードアウト状態に
		m_modeNext = modenext;				// 次の画面(モード)を設定
		m_col = WHT_INVISIBLE_VTX;

		m_pos = D3DXVECTOR3(-200.0f, 30.0, 0.0f);
	}
}

//=============================================================================
// 画面遷移の状態設定処理
//=============================================================================
void CTransition::SetState(FADE fade)
{
	m_fade = fade;
}