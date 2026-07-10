//========================================================================
// 
// 画面 [ screen.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "screen.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "texture.h"

//************************************************************************
// マクロ定義
//************************************************************************
#define OBJECT_WIDTH	(180.0f)		// オブジェクトの幅
#define OBJECT_HEIGHT	(90.0f)			// オブジェクトの高さ

//========================================================================
// 画面クラスの生成処理
//========================================================================
CScreen* CScreen::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	CScreen* pScreen = NULL;

	if (pScreen == NULL)
	{// NULLチェック
		// 画面の生成
		pScreen = new CScreen;
	}

	if (pScreen != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pScreen->Init(pos, fWidth, fHeight)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! 画面の初期化に失敗しました ! ! !\n");

			return NULL;
		}

		return pScreen;
	}

	OutputDebugStringA("! ! ! 画面の生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// 画面クラスのコンストラクタ
//========================================================================
CScreen::CScreen()
{
	// 画面クラスの値をクリア
	m_pVtxBuff = NULL;
	m_pos = DEFAULT_VECTER3;
	m_rot = DEFAULT_VECTER3;
	m_col = COLOR_WHITE;
	m_fAngle = 0.0f;
	m_fLength = 0.0f;
	m_bDisp = true;
}

//========================================================================
// 画面クラスのデストラクタ
//========================================================================
CScreen::~CScreen()
{
}

//========================================================================
// 画面クラスの初期化処理
//========================================================================
HRESULT CScreen::Init(const D3DXVECTOR3 pos,
	const float fWidth, const float fHeight)
{
	// ローカル変数宣言
	CRenderer* pRenderer = CManager::GetRenderer();			// レンダラーへのポインタ
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();			// デバイスへのポインタ
	VERTEX_2D* pVtx;

	// クラスの値を初期化
	m_pos = pos;

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{// もし失敗したら
		OutputDebugStringA("! ! ! 頂点バッファの生成に失敗しました ! ! !\n");

		return E_FAIL;
	}

	// 角度
	m_fAngle = atan2f(fWidth, fHeight);

	// 対角線の長さ
	m_fLength = sqrtf(((fWidth * 2) * (fWidth * 2)) + ((fHeight * 2) * (fHeight * 2))) * 0.5f;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報の設定
	// 頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI + m_fAngle + m_rot.z) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI + m_fAngle + m_rot.z) * m_fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI - m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI - m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = m_pos.x + sinf(-m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(-m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = m_pos.x + sinf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

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
// 画面クラスの終了処理
//========================================================================
void CScreen::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//========================================================================
// 画面クラスの更新処理
//========================================================================
void CScreen::Update(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();		// キーボード入力の取得

	if (pInputKeyboard->GetPress(DIK_UP) == true)
	{// サイズ大
		SetSize(m_fLength + 2.0f);
	}
	else if (pInputKeyboard->GetPress(DIK_DOWN) == true)
	{// サイズ小
		SetSize(m_fLength - 2.0f);
	}

	if (pInputKeyboard->GetPress(DIK_LEFT) == true)
	{// 回転
		SetRotation(D3DXVECTOR3(m_rot.x, m_rot.y, m_rot.z + 0.01f));
	}
	else if (pInputKeyboard->GetPress(DIK_RIGHT) == true)
	{// 回転
		SetRotation(D3DXVECTOR3(m_rot.x, m_rot.y, m_rot.z - 0.01f));
	}
}

//========================================================================
// 画面クラスの描画処理
//========================================================================
void CScreen::Draw(void)
{
	if (m_bDisp == false)
	{// 表示しない場合
		return;
	}

	// ローカル変数宣言
	CRenderer* pRenderer = CManager::GetRenderer();			// レンダラーへのポインタ
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスへのポインタ
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, CManager::GetRenderer()->GetTextureMT());

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================================================
// 位置設定
//========================================================================
void CScreen::SetPosition(const D3DXVECTOR3 pos)
{
	VERTEX_2D* pVtx;

	// 位置を変更
	m_pos = pos;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI + m_fAngle + m_rot.z) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI + m_fAngle + m_rot.z) * m_fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI - m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI - m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = m_pos.x + sinf(-m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(-m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = m_pos.x + sinf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//========================================================================
// 位置設定 ( オーバーロード ) 
//========================================================================
void CScreen::SetPosition(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	VERTEX_2D* pVtx;

	// 位置を変更
	m_pos = pos;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos.x = m_pos.x;
	pVtx[0].pos.y = m_pos.y - fHeight;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = m_pos.x + fWidth;
	pVtx[1].pos.y = m_pos.y - fHeight;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = m_pos.x;
	pVtx[2].pos.y = m_pos.y + fHeight;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = m_pos.x + fWidth;
	pVtx[3].pos.y = m_pos.y + fHeight;
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//========================================================================
// サイズ設定
//========================================================================
void CScreen::SetSize(const float fLength)
{
	VERTEX_2D* pVtx;

	// 対角線の長さ
	m_fLength = fLength;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI + m_fAngle + m_rot.z) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI + m_fAngle + m_rot.z) * m_fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI - m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI - m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = m_pos.x + sinf(-m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(-m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = m_pos.x + sinf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//========================================================================
// 角度設定
//========================================================================
void CScreen::SetRotation(const D3DXVECTOR3 rot)
{
	VERTEX_2D* pVtx;

	// 向きを変更
	m_rot = rot;

	// 向きを調整
	CorrectAngle(&m_rot.z, m_rot.z);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI + m_fAngle + m_rot.z) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI + m_fAngle + m_rot.z) * m_fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI - m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI - m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = m_pos.x + sinf(-m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(-m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = m_pos.x + sinf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//========================================================================
// 頂点カラー設定
//========================================================================
void CScreen::SetColor(const D3DXCOLOR col)
{
	m_col = col;

	VERTEX_2D* pVtx;

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
void CScreen::SetTexUV(const int nPatternAnim, const int nTexWidth, const int nTexHeight, const float fScrollX, const float fScrollY)
{
	VERTEX_2D* pVtx;

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

//=============================================================================
// 角度を修正
//=============================================================================
void CScreen::CorrectAngle(float* pAngle, float fAngle)
{
	// 向きを調整
	if (fAngle > D3DX_PI)
	{
		*pAngle -= D3DX_PI * 2.0f;
	}
	else if (fAngle < -D3DX_PI)
	{
		*pAngle += D3DX_PI * 2.0f;
	}
}