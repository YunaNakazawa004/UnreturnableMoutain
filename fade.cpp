//=============================================================================
// 
// フェード [ fade.cpp ]
// Author : Nakazawa Yuna
// 
//=============================================================================
#include "fade.h"

#include "renderer.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BLK_VISIBLE_VTX			(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))		// 黒いポリゴン(不透明)
#define BLK_INVISIBLE_VTX		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))		// 黒いポリゴン(透明)
#define WHT_VISIBLE_VTX			(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))		// 白いポリゴン(不透明)
#define WHT_INVISIBLE_VTX		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f))		// 白いポリゴン(透明)
#define FADE_SPEED				(0.03f)									// フェードの速さ

//=============================================================================
// フェードの生成処理
//=============================================================================
CFade* CFade::Create(CScene::MODE modenext)
{
	CFade* pFade = NULL;

	if (pFade == NULL)
	{// NULLチェック
		// フェードの生成
		pFade = new CFade;
	}

	if (pFade != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pFade->Init(modenext)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! フェードの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		return pFade;
	}

	OutputDebugStringA("! ! ! フェードの生成に失敗しました ! ! !\n");

	return NULL;
}

//=============================================================================
// フェードのコンストラクタ
//=============================================================================
CFade::CFade()
{
	// 値をクリア
	m_pVtxBuff = NULL;
	m_fade = FADE_NONE;
	m_modeNext = CScene::MODE_TITLE;
	m_col = COLOR_WHITE;
}

//=============================================================================
// フェードのデストラクタ
//=============================================================================
CFade::~CFade()
{
}

//=============================================================================
// フェードの初期化処理
//=============================================================================
HRESULT CFade::Init(CScene::MODE modenext)
{
	// ローカル変数宣言
	VERTEX_2D* pVtx;					// 頂点情報へのポインタ

	// デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();			// レンダラーへのポインタ
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスへのポインタ

	// 初期化
	m_fade = FADE_IN;				// フェードイン状態に
	m_modeNext = modenext;			// 次の画面(モード)を設定
	m_col = BLK_INVISIBLE_VTX;

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

	// モードの設定
	CManager::SetMode(m_modeNext);

	return S_OK;
}

//=============================================================================
// フェードの終了処理
//=============================================================================
void CFade::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// フェードの更新処理
//=============================================================================
void CFade::Update(void)
{
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
				m_fade = FADE_IN;			// フェードイン状態にする

				// モードの設定
				CManager::SetMode(m_modeNext);
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

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// フェードの描画処理
//=============================================================================
void CFade::Draw(void)
{
	// デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();			// レンダラーへのポインタ
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスへのポインタ

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,					// 描画する最初の頂点インデックス
		2);					// 描画するプリミティブ数
}

//=============================================================================
// フェードの設定処理
//=============================================================================
void CFade::SetFade(CScene::MODE modenext)
{
	if (m_fade == FADE_NONE)
	{
		m_fade = FADE_OUT;					// フェードアウト状態に
		m_modeNext = modenext;				// 次の画面(モード)を設定
		m_col = BLK_INVISIBLE_VTX;
	}
}

//=============================================================================
// フェードの状態設定処理
//=============================================================================
void CFade::SetState(FADE fade)
{
	m_fade = fade;
}