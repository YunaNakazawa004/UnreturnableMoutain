//========================================================================
// 
// メッシュフィールド [meshfield.cpp]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "meshfield.h"

#include "renderer.h"
#include "manager.h"
#include "texture.h"

//************************************************************************
// マクロ定義
//************************************************************************

//========================================================================
// メッシュフィールドクラスの生成処理
//========================================================================
CMeshField* CMeshField::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR2 block,
	const D3DXVECTOR2 size, const CObject::TYPE type, const char* pFilename, const int nPriority)
{
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}

	CMeshField* pMeshFIeld = NULL;

	if (pMeshFIeld == NULL)
	{// NULLチェック
		// メッシュフィールドの生成
		pMeshFIeld = new CMeshField(nPriority);
	}

	if (pMeshFIeld != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pMeshFIeld->Init(pos, rot, block, size)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! メッシュフィールドの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// テクスチャを設定
		CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ
		pMeshFIeld->m_nIdxTexture = pTexture->Register(pFilename);

		// 種類を設定
		pMeshFIeld->SetType(type);

		return pMeshFIeld;
	}

	OutputDebugStringA("! ! ! メッシュフィールドの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// メッシュフィールドクラスのコンストラクタ
//========================================================================
CMeshField::CMeshField(const int nPriority) :CObject(nPriority)
{
	// メッシュフィールドクラスの値をクリア
	m_pVtxBuff = NULL;
	m_pIdxBuff = NULL;
	m_nIdxTexture = -1;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_block = D3DXVECTOR2(0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nNumVtx = 0;
	m_nNumIdx = 0;
	m_bDisp = true;
}

//========================================================================
// メッシュフィールドクラスのデストラクタ
//========================================================================
CMeshField::~CMeshField()
{
}

//========================================================================
// メッシュフィールドクラスの初期化処理(オーバーロード)
//========================================================================
HRESULT CMeshField::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
	const D3DXVECTOR2 block, const D3DXVECTOR2 size)
{
	// ローカル変数宣言
	CRenderer* pRenderer = CManager::GetRenderer();			// レンダラーへのポインタ
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();			// デバイスへのポインタ
	VERTEX_3D* pVtx;					// 頂点情報へのポインタ
	WORD* pIdx;							// インデックス情報へのポインタ

	// クラスの値を初期化
	m_pos = pos;
	m_rot = rot;
	m_block = block;
	m_size = size;
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_nNumVtx = ((int)m_block.x + 1) * ((int)m_block.y + 1);
	m_nNumIdx = (((int)m_block.x) * ((int)m_block.y) * 2) + (((int)m_block.y - 1) * 4) + 2;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報の設定
	for (int nCntMeshField1 = 0; nCntMeshField1 < (int)m_block.y + 1; nCntMeshField1++)
	{
		for (int nCntMeshField2 = 0; nCntMeshField2 < (int)m_block.x + 1; nCntMeshField2++)
		{
			// 頂点座標の設定
			pVtx[0].pos.x = -((m_block.x * m_size.x) * 0.5f) + (nCntMeshField2 * m_size.x);
			pVtx[0].pos.y = 0.0f;
			pVtx[0].pos.z = ((m_block.y * m_size.y) * 0.5f) - (nCntMeshField1 * m_size.y);

			// rhwの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = COLOR_WHITE;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((float)nCntMeshField2, (float)nCntMeshField1);

			pVtx++;
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// インデックスバッファをロックし、頂点番号データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int nNum = 0;			// 縮退ポリゴン

	// 頂点番号データの設定
	for (int nCntMeshField1 = 0; nCntMeshField1 < m_nNumIdx / 2; nCntMeshField1++)
	{
		if (nCntMeshField1 % ((int)m_block.x + 2) == ((int)m_block.x + 1))
		{// 縮退ポリゴンのところ
			nNum++;

			pIdx[0] = nCntMeshField1 - nNum;
			pIdx[1] = nCntMeshField1 - nNum + ((int)m_block.x + 2);
		}
		else
		{// 縮退以外のポリゴン
			pIdx[0] = (nCntMeshField1 - nNum) + ((int)m_block.x + 1);
			pIdx[1] = (nCntMeshField1 - nNum);
		}

		pIdx += 2;
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	return S_OK;
}

//========================================================================
// メッシュフィールドクラスの終了処理
//========================================================================
void CMeshField::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// インデックスバッファの破棄
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// 自分自身を破棄
	CObject::Release();
}

//========================================================================
// メッシュフィールドクラスの更新処理
//========================================================================
void CMeshField::Update(void)
{
}

//========================================================================
// メッシュフィールドクラスの描画処理
//========================================================================
void CMeshField::Draw(void)
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

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, ((int)m_block.x + 1) * ((int)m_block.y + 1), 0,
		(((int)m_block.x) * ((int)m_block.y) * 2) + (((int)m_block.y - 1) * 4));
}

//========================================================================
// 位置設定
//========================================================================
void CMeshField::SetPosition(const D3DXVECTOR3 pos)
{
	// 位置を変更
	m_pos = pos;
}

//========================================================================
// スケール設定
//========================================================================
void CMeshField::SetScale(const D3DXVECTOR3 scale)
{
	// スケールを変更
	m_scale = scale;
}

//========================================================================
// 角度設定
//========================================================================
void CMeshField::SetRotation(const D3DXVECTOR3 rot)
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
void CMeshField::SetColor(const D3DXCOLOR col)
{
	m_col = col;

	VERTEX_3D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{
		// 頂点カラーの設定
		pVtx[nCntVtx].col = m_col;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//========================================================================
// テクスチャ座標設定
//========================================================================
void CMeshField::SetTexUV(const int nCntVtx, const float ftexU, const float ftexV)
{
	VERTEX_3D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	pVtx[nCntVtx].tex = D3DXVECTOR2(ftexU, ftexV);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}