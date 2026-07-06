//========================================================================
// 
// レンダリング [ renderer.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "renderer.h"

#include "manager.h"
#include "debugproc.h"
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENABLE_PERSPECTIVFOV_MT				// レンダリングターゲットを透視投影で描画
//#undef ENABLE_PERSPECTIVFOV_MT

//========================================================================
// レンダラークラスのコンストラクタ
//========================================================================
CRenderer::CRenderer()
{
	// レンダラークラスの値のクリア
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
}

//========================================================================
// レンダラークラスのデストラクタ
//========================================================================
CRenderer::~CRenderer()
{
}

//========================================================================
// レンダラークラスの初期化処理
//========================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL)
	{// NULLチェック
		OutputDebugStringA("! ! ! DirectXオブジェクトの生成に失敗しました ! ! !\n");

		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		OutputDebugStringA("! ! ! ディスプレイモードの取得に失敗しました ! ! !\n");

		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));			// パラメータのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;		// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;		// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;		// バックバッファの形式
	d3dpp.BackBufferCount = 1;					// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// ダブルバッファの切り替え(映像信号に動機)
	d3dpp.EnableAutoDepthStencil = TRUE;		// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;					// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル

	// Direct3Dデバイスの生成
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				OutputDebugStringA("! ! ! デバイスの生成に失敗しました ! ! !\n");

				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);			// テクスチャの拡縮補間の設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);			// テクスチャの繰り返しの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;

	// レンダリングターゲット用テクスチャの生成
	m_pD3DDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pTextureMT,
		NULL);

	// テクスチャレンダリング用インターフェースの生成
	m_pTextureMT->GetSurfaceLevel(0, &m_pRenderMT);

	// テクスチャレンダリング用Zバッファの生成
	m_pD3DDevice->CreateDepthStencilSurface(SCREEN_WIDTH, SCREEN_HEIGHT,
		D3DFMT_D16,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pZBuffMT,
		NULL);

	// 現在のレンダリングターゲットを取得（保存）
	m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

	// 現在のZバッファを取得（保存）
	m_pD3DDevice->GetDepthStencilSurface(&pZBuffDef);

	// レンダリングターゲットを生成したテクスチャに設定
	m_pD3DDevice->SetRenderTarget(0, m_pRenderMT);

	// Zバッファを生成したZバッファに設定
	m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT); 

	// レンダリングターゲット用テクスチャのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// レンダリングターゲットをもとに戻す
	m_pD3DDevice->SetRenderTarget(0, pRenderDef);

	// Zバッファを元に戻す
	m_pD3DDevice->SetDepthStencilSurface(pZBuffDef);

	// テクスチャレンダリング用ビューポートの設定
	m_viewportMT.X = 0;
	m_viewportMT.Y = 0;
	m_viewportMT.Width = SCREEN_WIDTH;
	m_viewportMT.Height = SCREEN_HEIGHT;
	m_viewportMT.MinZ = 0.0f;
	m_viewportMT.MaxZ = 1.0f;

	return S_OK;
}

//========================================================================
// レンダラークラスの終了処理
//========================================================================
void CRenderer::Uninit(void)
{
	// Direct3Dデバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//========================================================================
// レンダラークラスの更新処理
//========================================================================
void CRenderer::Update(void)
{
	// 全てのオブジェクトの更新
	CObject::UpdateAll();
}

//========================================================================
// レンダラークラスの描画処理
//========================================================================
void CRenderer::Draw(void)
{
	// 画面クリア(バックバッファとZバッファのクリア)
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// 描画開始が成功した場合
		// 全てのオブジェクトの描画
		CObject::DrawAll();

		// デバッグ表示
		CDebugProc* pDebugProc = CManager::GetDebugProc();

		if (pDebugProc != NULL)
		{// NULLチェック
			// デバッグ表示の描画
			pDebugProc->Draw();
		}

		// 描画終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//========================================================================
// レンダリングターゲットを変更
//========================================================================
void CRenderer::ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU)
{
	D3DXMATRIX mtxView, mtxProjection;
	float fAspect;

	// レンダリングターゲットを生成したテクスチャに設定
	m_pD3DDevice->SetRenderTarget(0, m_pRenderMT);

	// Zバッファを生成したZバッファに設定
	m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);

	// テクスチャレンダリング用のビューポートを設定
	m_pD3DDevice->SetViewport(&m_viewportMT);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&mtxView,
		&posV,			// 視点
		&posR,			// 注視点
		&vecU);			// 上方向ベクトル

	// ビューマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&mtxProjection);

#ifdef ENABLE_PERSPECTIVFOV_MT		// 透視投影
	// プロジェクションマトリックスを作成
	fAspect = (float)m_viewportMT.Width / (float)m_viewportMT.Height;
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		D3DXToRadian(45.0f),					// 視野角
		fAspect,								// 画面のアスペクト比
		10.0f,									// カメラから一番近い描画距離
		80000.0f);								// 最大描画距離

#else		// 平行投影
	// プロジェクションマトリックスを作成
	D3DXMatrixOrthoLH(&mtxProjection,
		(float)m_viewportMT.Width,		// ビューボリュームの幅
		(float)m_viewportMT.Height,		// ビューボリュームの高さ
		10.0f,							// ビューボリュームの近平面までの距離
		80000.0f);						// ビューボリュームの遠平面までの距離
#endif

	// プロジェクションマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}

//========================================================================
// ワイヤーフレーム切り替え
//========================================================================
void CRenderer::SwapWireFrame(void)
{
	static bool bWire = false;

	// true / false 入れ替え
	bWire = bWire ? false : true;

	if (bWire == true)
	{// ワイヤーフレームON
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else
	{// ワイヤーフレームOFF
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}