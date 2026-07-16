//========================================================================
// 
// アイテムUI [ UI_item.cpp ]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "UI_item.h"

#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "texture.h"

//************************************************************************
// マクロ定義
//************************************************************************

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
int CItemUI::m_aIdxTexture[TYPE_MAX + 1] = {};				// テクスチャのインデックス
int CItemUI::m_nNumItem = 0;								// アイテムの総数

//========================================================================
// テクスチャの生成
//========================================================================
HRESULT CItemUI::Load(void)
{
	// ローカル変数宣言
	CTexture* pTexture = CManager::GetTexture();			// テクスチャへのポインタ

	// テクスチャの設定
	m_aIdxTexture[0] = pTexture->Register("data\\TEXTURE\\UI\\item000.png");
	m_aIdxTexture[1] = pTexture->Register("data\\TEXTURE\\UI\\item001.png");
	m_aIdxTexture[2] = pTexture->Register("data\\TEXTURE\\UI\\item002.png");
	m_aIdxTexture[3] = pTexture->Register("data\\TEXTURE\\UI\\item003.png");
	m_aIdxTexture[4] = pTexture->Register("data\\TEXTURE\\UI\\item_frame.png");

	if (m_aIdxTexture[0] == -1 || m_aIdxTexture[1] == -1 || m_aIdxTexture[2] == -1 || m_aIdxTexture[3] == -1 || m_aIdxTexture[4] == -1)
	{// テクスチャが設定できていない
		OutputDebugStringA("! ! ! テクスチャの設定に失敗しました ! ! !\n");

		return E_FAIL;
	}

	return S_OK;
}

//========================================================================
// テクスチャの破棄
//========================================================================
void CItemUI::Unload(void)
{
	// テクスチャのインデックスを削除
	memset(&m_aIdxTexture[0], -1, sizeof m_aIdxTexture);
}

//========================================================================
// アイテムUIクラスの生成処理
//========================================================================
CItemUI* CItemUI::Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
#ifndef LIST
	if (CObject::GetNumAll() >= MAX_OBJECT)
	{// 最大数のオブジェクトが存在する
		OutputDebugStringA("! ! ! オブジェクトの最大数に達しています ! ! !\n");

		return NULL;
	}
#endif

	CItemUI* pItemUI = NULL;

	if (pItemUI == NULL)
	{// NULLチェック
		// アイテムUIの生成
		pItemUI = new CItemUI;
	}

	if (pItemUI != NULL)
	{// NULLチェック
		// 初期化処理
		if (FAILED(pItemUI->Init(pos, fWidth, fHeight)))
		{// もし失敗した場合
			OutputDebugStringA("! ! ! アイテムUIの初期化に失敗しました ! ! !\n");

			return NULL;
		}

		// オブジェクトの種類を設定
		pItemUI->SetType(TYPE_ITEMUI);

		// テクスチャの割り当て
		pItemUI->BindTexture(m_aIdxTexture[TYPE_MAX]);

		return pItemUI;
	}

	OutputDebugStringA("! ! ! アイテムUIの生成に失敗しました ! ! !\n");

	return NULL;
}

//========================================================================
// アイテムUIクラスのコンストラクタ
//========================================================================
CItemUI::CItemUI(const int nPriority) :CObject2D(nPriority)
{
	// アイテムUIクラスの値をクリア
	memset(&m_Item[0], NULL, sizeof m_Item);
	m_nNumItem = 0;
}

//========================================================================
// アイテムUIクラスのデストラクタ
//========================================================================
CItemUI::~CItemUI()
{
}

//========================================================================
// アイテムUIクラスの初期化処理
//========================================================================
HRESULT CItemUI::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	if (FAILED(CObject2D::Init(pos, fWidth, fHeight, CObject2D::POS_LEFT_TOP)))
	{// アイテムUIの初期化に失敗した場合
		OutputDebugStringA("! ! ! アイテムUIの初期化に失敗しました ! ! !\n");

		return E_FAIL;
	}

	// アイテムUIクラスの値を初期化
	for (int nCnt = 0; nCnt < ITEMUI_NUM; nCnt++)
	{
		if (m_Item[nCnt].pItem == NULL)
		{// NULLチェック
			m_Item[nCnt].pItem =
				CObject2D::Create(D3DXVECTOR3(pos.x + 40.0f + nCnt * 70.0f, pos.y + (fHeight), 0.0f), 30.0f, 30.0f,
					CObject::TYPE_ITEMUI, "data\\TEXTURE\\UI\\item000.png", UI_PRIORITY);
		}

		if (m_Item[nCnt].pCheck == NULL)
		{// NULLチェック
			m_Item[nCnt].pCheck =
				CObject2D::Create(D3DXVECTOR3(pos.x + 40.0f + nCnt * 70.0f, pos.y + (fHeight), 0.0f), 30.0f, 30.0f,
					CObject::TYPE_ITEMUI, "data\\TEXTURE\\UI\\check.png", UI_PRIORITY);
		}

		if (m_Item[nCnt].pCheck != NULL)
		{// NULLチェック
			m_Item[nCnt].pCheck->SetDisp(false);
		}

		m_Item[nCnt].type = TYPE_TREE;
	}

	return S_OK;
}

//========================================================================
// アイテムUIクラスの終了処理
//========================================================================
void CItemUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < ITEMUI_NUM; nCnt++)
	{
		if (m_Item[nCnt].pItem != NULL)
		{// NULLチェック
			// 終了処理
			m_Item[nCnt].pItem->Uninit();
		}

		if (m_Item[nCnt].pCheck != NULL)
		{// NULLチェック
			// 終了処理
			m_Item[nCnt].pCheck->Uninit();
		}
	}

	// 終了処理
	CObject2D::Uninit();
}

//========================================================================
// アイテムUIクラスの更新処理
//========================================================================
void CItemUI::Update(void)
{
}

//========================================================================
// アイテムUIクラスの描画処理
//========================================================================
void CItemUI::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}

//========================================================================
// アイテムUIの設定処理
//========================================================================
void CItemUI::SetItem(const int Item)
{
	if (m_nNumItem >= ITEMUI_NUM)
	{// 要素数を越えている
		return;
	}

	m_Item[m_nNumItem].type = Item;

	if (m_Item[m_nNumItem].pItem != NULL)
	{// NULLチェック
		// テクスチャを設定
		m_Item[m_nNumItem].pItem->BindTexture(m_aIdxTexture[Item]);
	}

	if (m_Item[m_nNumItem].pCheck != NULL)
	{// NULLチェック
		m_Item[m_nNumItem].pCheck->SetDisp(false);
	}

	m_nNumItem++;
}