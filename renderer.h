//========================================================================
// 
// レンダリングヘッダー [ renderer.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _RENDERER_H_		// このマクロ定義がされていなかったら
#define _RENDERER_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"

//************************************************************************
// レンダラークラス
//************************************************************************
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

	void ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU);
	LPDIRECT3DTEXTURE9 GetTextureMT(void) { return m_apTextureMT[0]; }

	void SwapWireFrame(void);

private:
	LPDIRECT3D9 m_pD3D;							// Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;				// Direct3Dデバイスへのポインタ

	LPDIRECT3DTEXTURE9 m_apTextureMT[2];		// レンダリングターゲット用テクスチャ
	LPDIRECT3DSURFACE9 m_apRenderMT[2];			// テクスチャレンダリング用インターフェース
	LPDIRECT3DSURFACE9 m_pZBuffMT;				// テクスチャレンダリング用Zバッファ
	D3DVIEWPORT9 m_viewportMT;					// テクスチャレンダリング用ビューポート
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMT;		// フィードバック用ポリゴンの頂点バッファ
};

#endif