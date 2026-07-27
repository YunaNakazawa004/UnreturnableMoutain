//========================================================================
// 
// リザルト内訳UI [ UI_result_list.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "UI_result_list.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"

#include "object2D.h"

//************************************************************************
// マクロ定義
//***********************************************************************

//***********************************************************************
// 静的メンバ変数宣言
//***********************************************************************
CListUI::List_Info CListUI::m_ListInfo[TYPE_MAX] = {
	{D3DXVECTOR3(40.0f, 160.0f, 0.0f), TYPE_BASE, 280.0f, 35.0f, "data\\TEXTURE\\UI\\result_base.png"},
	{D3DXVECTOR3(40.0f, 240.0f, 0.0f), TYPE_ENERGY, 280.0f, 35.0f, "data\\TEXTURE\\UI\\result_energy.png"},
	{D3DXVECTOR3(-10.0f, 320.0f, 0.0f), TYPE_COLLECT, 280.0f, 35.0f, "data\\TEXTURE\\UI\\result_collect.png"},
	{D3DXVECTOR3(40.0f, 400.0f, 0.0f), TYPE_FINAL, 280.0f, 35.0f, "data\\TEXTURE\\UI\\result_clear.png"},
	{D3DXVECTOR3(-210.0f, 540.0f, 0.0f), TYPE_FINAL, 400.0f, 50.0f, "data\\TEXTURE\\UI\\result_final.png"},
};

//========================================================================
// リザルト内訳UIクラスの生成処理
//========================================================================
CListUI* CListUI::Create(void)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CListUI* pListUI = NULL;

	if (pListUI == NULL)
	{// NULLチェック
		// リザルト内訳UIの生成
		pListUI = new CListUI;
	}

	if (pListUI != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pListUI->Init()))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! リザルト内訳UIの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// オブジェクトの種類を設定
		pListUI->SetType(TYPE_LISTUI);

		return pListUI;
	}

	OutputDebugStringA("! ! ! リザルト内訳UIの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// リザルト内訳UIクラスのコンストラクタ
//========================================================================
CListUI::CListUI(const int nPriority) :CObject(nPriority)
{
	// リザルト内訳UIクラスの値をクリア
	memset(&m_apList[0], NULL, sizeof m_apList);
}

//========================================================================
// リザルト内訳UIクラスのデストラクタ
//========================================================================
CListUI::~CListUI()
{
}

//========================================================================
// リザルト内訳UIクラスの初期化処理
//========================================================================
HRESULT CListUI::Init(void)
{
	// リザルト内訳UIクラスの値を初期化
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		if (m_apList[nCount] == NULL)
		{// NULLチェック
			m_apList[nCount] = CObject2D::Create(m_ListInfo[nCount].pos, m_ListInfo[nCount].fWidth, m_ListInfo[nCount].fHeight,
				CObject::TYPE_LISTUI, m_ListInfo[nCount].pFilename, PRIORITY_6, 2);
		}
	}

	return S_OK;
}

//========================================================================
// リザルト内訳UIクラスの終了処理
//========================================================================
void CListUI::Uninit(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		if (m_apList[nCount] != NULL)
		{// NULLチェック
			m_apList[nCount] = NULL;
		}
	}

	// 自分自身を破棄
	CObject::Release();
}

//========================================================================
// リザルト内訳UIクラスの更新処理
//========================================================================
void CListUI::Update(void)
{
}

//========================================================================
// リザルト内訳UIクラスの描画処理
//========================================================================
void CListUI::Draw(void)
{
}

//========================================================================
// 表示状態を設定
//========================================================================
void CListUI::SetDisp(const int nIdx, const bool bDisp)
{
	m_apList[nIdx]->SetDisp(bDisp);
}

//========================================================================
// 表示状態を設定
//========================================================================
void CListUI::SetDispAll(const bool bDisp)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		if (m_apList[nCount] != NULL)
		{// NULLチェック
			// 表示設定
			m_apList[nCount]->SetDisp(bDisp);
		}
	}
}