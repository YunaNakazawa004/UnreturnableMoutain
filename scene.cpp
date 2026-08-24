//========================================================================
// 
// シーン [ scene.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "scene.h"

#include "title.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "manager.h"
#include "sound.h"

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
bool CScene::m_bFade = false;						// 遷移フラグ

//========================================================================
// シーンクラスのコンストラクタ
//========================================================================
#ifdef ENABLE_INHERITANCE_COBJECT
CScene::CScene(const MODE mode) : CObject(0)

#else
CScene::CScene(const MODE mode)

#endif
{
	// 値をクリア
	m_mode = mode;
	m_bFade = false;
}

//========================================================================
// シーンクラスのデストラクタ
//========================================================================
CScene::~CScene()
{
}

//========================================================================
// シーンクラスの生成
//========================================================================
CScene* CScene::Create(const CScene::MODE mode)
{
	CSound* pSound = CManager::GetSound();
	CScene* pScene = NULL;

	if (pScene == NULL)
	{// NULLチェック
		// シーンの生成
		switch (mode)
		{
		case MODE_TITLE:		// タイトル
			pScene = new CTitle;
			pSound->PlaySound(CSound::BGM_TITLE);

			break;

		case MODE_GAME:			// ゲーム
			pScene = new CGame;
			pSound->PlaySound(CSound::BGM_GAME);

			break;

		case MODE_RESULT:		// リザルト
			pScene = new CResult;
			pSound->PlaySound(CSound::BGM_GAME);

			break;

		case MODE_RANKING:		// ランキング
			pScene = new CRanking;
			pSound->PlaySound(CSound::BGM_TITLE);

			break;
		}
	}

	if (pScene != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pScene->Init()))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! シーンの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		return pScene;
	}

	OutputDebugStringA("! ! ! シーンの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// シーンクラスの初期化処理
//========================================================================
HRESULT CScene::Init(void)
{
	return S_OK;
}

//========================================================================
// シーンクラスの終了処理
//========================================================================
void CScene::Uninit(void)
{
}

//========================================================================
// シーンクラスの更新処理
//========================================================================
void CScene::Update(void)
{
}

//========================================================================
// シーンクラスの描画処理
//========================================================================
void CScene::Draw(void)
{
}