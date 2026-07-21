//========================================================================
// 
// マネージャーヘッダー [ manager.h ]
// Author : Nakazawa Yuna
// 
//========================================================================
#ifndef _MANAGER_H_		// このマクロ定義がされていなかったら
#define _MANAGER_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "scene.h"

//************************************************************************
// 前方宣言
//************************************************************************
class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CInputJoypad;
class CSound;
class CDebugProc;
class CCamera;
class CLight;
class CTexture;
class CScene;
class CFade;
class CTransition;
class CScreen;

//************************************************************************
// マネージャークラス
//************************************************************************
class CManager
{
public:
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRenderer* GetRenderer(void) { return m_pRenderer; }
	static CInputKeyboard* GetInputKeyboard(void) { return m_pInputKeyboard; }
	static CInputMouse* GetInputMouse(void) { return m_pInputMouse; }
	static CInputJoypad* GetInputJoypad(void) { return m_pInputJoypad; }
	static CSound* GetSound(void) { return m_pSound; }
	static CDebugProc* GetDebugProc(void) { return m_pDebugProc; }
	static CCamera* GetCamera(void) { return m_pCamera; }
	static CLight* GetLight(void) { return m_pLight; }
	static CTexture* GetTexture(void) { return m_pTexture; }
	static CFade* GetFade(void) { return m_pFade; }
	static CTransition* GetTransition(void) { return m_pTransition; }
	static CScreen* GetScreen(void) { return m_pScreen; }

	static void SetMode(const CScene::MODE mode);
	static CScene::MODE GetMode(void) { return m_pScene->GetMode(); }

	static int GetFPS(void) { return m_nCountFPS; }
	static void SetFPS(const int nCountFPS) { m_nCountFPS = nCountFPS; };
	static bool GetPause(void) { return m_bPause; }
	static void SetPause(const bool bPause) { m_bPause = bPause; }

	template <class T> static HRESULT CreateInstance(T** instance);

private:
	static CRenderer* m_pRenderer;					// レンダラーのインスタンス
	static CInputKeyboard* m_pInputKeyboard;		// キーボード入力のインスタンス
	static CInputMouse* m_pInputMouse;				// マウス入力のインスタンス
	static CInputJoypad* m_pInputJoypad;			// ジョイパッド入力のインスタンス
	static CSound* m_pSound;						// サウンドのインスタンス
	static CDebugProc* m_pDebugProc;				// デバッグ表示のインスタンス
	static CCamera* m_pCamera;						// カメラのインスタンス
	static CLight* m_pLight;						// ライトのインスタンス
	static CTexture* m_pTexture;					// テクスチャのインスタンス
	static CScene* m_pScene;						// シーンのインスタンス
	static CFade* m_pFade;							// フェードのインスタンス
	static CTransition* m_pTransition;				// 画面遷移のインスタンス
	static CScreen* m_pScreen;						// 画面のインスタンス
		
	static int m_nCountFPS;							// FPSカウンター
	static bool m_bPause;							// ポーズするかしないか
};

//************************************************************************
// インスタンス生成のテンプレート関数
//************************************************************************
template <class T> HRESULT CManager::CreateInstance(T** instance)
{
	if (*instance == NULL)
	{// NULLチェック
		*instance = new T;

		if (*instance == NULL)
		{// NULLチェック
			OutputDebugStringA("! ! ! インスタンスの生成に失敗しました ! ! !\n");

			return E_FAIL;
		}

		return S_OK;
	}

	return E_FAIL;
}

#endif