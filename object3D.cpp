//========================================================================
// 
// オブジェクト3D [object3D.cpp]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "object3D.h"

#include "renderer.h"
#include "manager.h"
#include "texture.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define OBJECT_WIDTH	(180.0f)		// オブジェクトの幅
#define OBJECT_HEIGHT	(90.0f)			// オブジェクトの高さ

//========================================================================
// オブジェクト3Dクラスの生成処理
//========================================================================
CObject3D* CObject3D::Create(const D3DXVECTOR3 pos, const float fWidth, const float fDepth,
	const CObject::TYPE type, const char* pFilename, const int nPriority)
{
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}

	CObject3D* pObject3D = NULL;

	if (pObject3D == NULL)
	{// NULLチェック
		// オブジェクト3Dの生成
		pObject3D = new CObject3D(nPriority);
	}

	if (pObject3D != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pObject3D->Init(pos, fWidth, fDepth)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! オブジェクト3Dの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// テクスチャを設定
		CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ
		pObject3D->m_nIdxTexture = pTexture->Register(pFilename);

		// 種類を設定
		pObject3D->SetType(type);

		return pObject3D;
	}

	OutputDebugStringA("! ! ! オブジェクト3Dの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// オブジェクト3Dクラスのコンストラクタ
//========================================================================
CObject3D::CObject3D(const int nPriority) :CObject(nPriority)
{
	// オブジェクト3Dクラスの値をクリア
	m_pVtxBuff = NULL;
	m_nIdxTexture = -1;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_bDisp = true;
}

//========================================================================
// オブジェクト3Dクラスのデストラクタ
//========================================================================
CObject3D::~CObject3D()
{
}

//========================================================================
// オブジェクト3Dクラスの初期化処理(オーバーロード)
//========================================================================
HRESULT CObject3D::Init(const D3DXVECTOR3 pos,
	const float fWidth, const float fDepth)
{
	// ローカル変数宣言
	CRenderer* pRenderer = CManager::GetRenderer();			// レンダラーへのポインタ
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();			// デバイスへのポインタ
	VERTEX_3D* pVtx;

	// クラスの値を初期化
	m_pos = pos;
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{// もし失敗したら
		OutputDebugStringA("! ! ! 頂点バッファの生成に失敗しました ! ! !\n");

		return E_FAIL;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報の設定
	// 頂点座標の設定
	pVtx[0].pos.x = -fWidth;
	pVtx[0].pos.y = 0.0f;
	pVtx[0].pos.z = +fDepth;
	pVtx[1].pos.x = +fWidth;
	pVtx[1].pos.y = 0.0f;
	pVtx[1].pos.z = +fDepth;
	pVtx[2].pos.x = -fWidth;
	pVtx[2].pos.y = 0.0f;
	pVtx[2].pos.z = -fDepth;
	pVtx[3].pos.x = +fWidth;
	pVtx[3].pos.y = 0.0f;
	pVtx[3].pos.z = -fDepth;

	// 法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//========================================================================
// オブジェクト3Dクラスの終了処理
//========================================================================
void CObject3D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// 自分自身を破棄
	CObject::Release();
}

//========================================================================
// オブジェクト3Dクラスの更新処理
//========================================================================
void CObject3D::Update(void)
{
}

//========================================================================
// オブジェクト3Dクラスの描画処理
//========================================================================
void CObject3D::Draw(void)
{
	if (m_bDisp == false)
	{// 表示しない場合
		return;
	}

	// ローカル変数宣言
	CRenderer* pRenderer = CManager::GetRenderer();			// レンダラーへのポインタ
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスへのポインタ
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// スケールを反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,		// 描画する最初の頂点インデックス
		2);		// 描画するプリミティブ数
}

//========================================================================
// 位置設定
//========================================================================
void CObject3D::SetPosition(const D3DXVECTOR3 pos)
{
	// 位置を変更
	m_pos = pos;
}

//========================================================================
// スケール設定
//========================================================================
void CObject3D::SetScale(const D3DXVECTOR3 scale)
{
	// スケールを変更
	m_scale = scale;
}

//========================================================================
// 角度設定
//========================================================================
void CObject3D::SetRotation(const D3DXVECTOR3 rot)
{
	// X向きを調整
	if (m_rot.x > D3DX_PI)
	{
		m_rot.x -= D3DX_PI * 2.0f;
	}
	else if (m_rot.x < -D3DX_PI)
	{
		m_rot.x += D3DX_PI * 2.0f;
	}

	// Y向きを調整
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	// Z向きを調整
	if (m_rot.z > D3DX_PI)
	{
		m_rot.z -= D3DX_PI * 2.0f;
	}
	else if (m_rot.z < -D3DX_PI)
	{
		m_rot.z += D3DX_PI * 2.0f;
	}

	// 向きを変更
	m_rot = rot;
}

//========================================================================
// 頂点カラー設定
//========================================================================
void CObject3D::SetColor(const D3DXCOLOR col)
{
	m_col = col;

	VERTEX_3D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//========================================================================
// テクスチャ座標設定
//========================================================================
void CObject3D::SetTexUV(const int nPatternAnim, const int nTexWidth, const int nTexHeight, const float fScrollX, const float fScrollY)
{
	VERTEX_3D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(nPatternAnim * (1.0f / (float)nTexWidth) + fScrollX, (nPatternAnim / nTexWidth) * (1.0f / (float)nTexHeight) + fScrollY);
	pVtx[1].tex = D3DXVECTOR2((nPatternAnim + 1) * (1.0f / (float)nTexWidth) + fScrollX, (nPatternAnim / nTexWidth) * (1.0f / (float)nTexHeight) + fScrollY);
	pVtx[2].tex = D3DXVECTOR2(nPatternAnim * (1.0f / (float)nTexWidth) + fScrollX, ((nPatternAnim / nTexWidth) + 1) * (1.0f / (float)nTexHeight) + fScrollY);
	pVtx[3].tex = D3DXVECTOR2((nPatternAnim + 1) * (1.0f / (float)nTexWidth) + fScrollX, ((nPatternAnim / nTexWidth) + 1) * (1.0f / (float)nTexHeight) + fScrollY);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}