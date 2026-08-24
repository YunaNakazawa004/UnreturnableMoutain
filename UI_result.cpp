//========================================================================
// 
// リザルトUI [ UI_result.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "UI_result.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"
#include "texture.h"

#include "result.h"
#include "used_energy.h"
#include "score.h"
#include "collect_num.h"
#include "UI_result_list.h"

//************************************************************************
// マクロ定義
//************************************************************************

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
int CResultUI::m_nIdxTexture = -1;						// テクスチャのインデックス
const char* CResultUI::m_pFilename[TYPE_MAX] = {		// テクスチャファイル名
	"data\\TEXTURE\\UI\\decision000.png",
	"data\\TEXTURE\\UI\\decision001.png",
	"data\\TEXTURE\\UI\\result.png"
};

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CResultUI::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_nIdxTexture = pTexture->Register("data\\TEXTURE\\UI\\result.png");

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
void CResultUI::Unload(void)
{
	// テクスチャのインデックスを削除
	memset(&m_nIdxTexture, -1, sizeof m_nIdxTexture);
}

//========================================================================
// リザルトUIクラスの生成処理
//========================================================================
CResultUI* CResultUI::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const TYPE type)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CResultUI* pResultUI = NULL;

	if (pResultUI == NULL)
	{// NULLチェック
		// リザルトUIの生成
		pResultUI = new CResultUI;
	}

	if (pResultUI != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pResultUI->Init(pos, fWidth, fHeight, type)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! リザルトUIの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// オブジェクトの種類を設定
		pResultUI->SetType(TYPE_RESULTUI);

		// テクスチャの割り当て
		pResultUI->BindTexture(m_nIdxTexture);

		return pResultUI;
	}

	OutputDebugStringA("! ! ! リザルトUIの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// リザルトUIクラスのコンストラクタ
//========================================================================
CResultUI::CResultUI(const int nPriority) :CObject2D(nPriority)
{
	// リザルトUIクラスの値をクリア
	m_type = TYPE_CLEAR;
}

//========================================================================
// リザルトUIクラスのデストラクタ
//========================================================================
CResultUI::~CResultUI()
{
}

//========================================================================
// リザルトUIクラスの初期化処理
//========================================================================
HRESULT CResultUI::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const TYPE type)
{
	if (FAILED(CObject2D::Init(pos, fWidth, fHeight)))
	{// リザルトUIの初期化に失敗した場合
		OutputDebugStringA("! ! ! リザルトUIの初期化に失敗しました ! ! !\n");

		return E_FAIL;
	}

	// リザルトUIクラスの値を初期化
	m_type = type;

	if (m_pDecision == NULL)
	{// NULLチェック
		m_pDecision = CObject2D::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f), 400.0f, 200.0f,
			TYPE_RESULTUI, m_pFilename[m_type], PRIORITY_7);

		if (m_pDecision != NULL)
		{// NULLチェック
			m_pDecision->SetRotation(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.1f));

			m_pDecision->SetDisp(false);
		}
	}

	return S_OK;
}

//========================================================================
// リザルトUIクラスの終了処理
//========================================================================
void CResultUI::Uninit(void)
{
	// 終了処理
	CObject2D::Uninit();
}

//========================================================================
// リザルトUIクラスの更新処理
//========================================================================
void CResultUI::Update(void)
{
	// ローカル変数
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();		// キーボード入力の取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();			// ジョイパッド入力の取得
	CSound* pSound = CManager::GetSound();								// サウンドを取得
	CUsedEnergy* pUsedEnergy = CResult::GetUsedEnergy();		// 使用エネルギー量の取得
	CScore* pBaseScore = CResult::GetBaseScore();				// 基礎スコアの取得
	CCollectNum* pCollectNum = CResult::GetCollectNum();		// 収集数の取得
	CScore* pCollectScore = CResult::GetCollectScore();			// 収集スコアの取得
	CScore* pClearScore = CResult::GetClearScore();				// クリアボーナスの取得
	CScore* pFinalScore = CResult::GetFinalScore();				// 最終スコアの取得
	CListUI* pListUI = CResult::GetListUI();					// リザルト内訳UIの取得
	D3DXVECTOR3 pos = GetPosition();

	if (pos.y > 100.0f)
	{// 指定の場所まで
		pos.y -= 2.0f;
	}

	if (pos.y <= 100.0f)
	{// 指定の場所にする
		pos.y = 100.0f;

		pBaseScore->SetDisp(true);
		pListUI->SetDisp(0, true);

		pUsedEnergy->SetDisp(true);
		pListUI->SetDisp(1, true);

		pCollectNum->SetDisp(true);
		pCollectScore->SetDisp(true);
		pListUI->SetDisp(2, true);

		if (pClearScore != NULL)
		{// NULLチェック
			pClearScore->SetDisp(true);
			pListUI->SetDisp(3, true);
		}

		pFinalScore->SetDisp(true);
		pListUI->SetDisp(4, true);

		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
			pInputJoypad->GetTrigger(0, CInputJoypad::JOYKEY_A) == true ||
			pInputJoypad->GetTrigger(0, CInputJoypad::JOYKEY_START) == true)
		{// キーが押された
			m_pDecision->SetDisp(true);
			pSound->PlaySound(CSound::SE_DECISION);

			CResult::SetFadeEnable();
		}
	}

	// 位置を設定
	SetPosition(pos);
}

//========================================================================
// リザルトUIクラスの描画処理
//========================================================================
void CResultUI::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}