//========================================================================
// 
// オブジェクト [object.cpp]
// Author : Nakazawa Yuna
// 
//========================================================================
#include "object.h"

#include "manager.h"
#include "camera.h"

//************************************************************************
// 静的メンバ変数宣言
//************************************************************************
CObject* CObject::m_apObject[MAX_PRIORITY_NUM][MAX_OBJECT] = {};	// オブジェクトのインスタンス
int CObject::m_nNumAll = 0;				// オブジェクトの総数

//========================================================================
// オブジェクトクラスのコンストラクタ
//========================================================================
CObject::CObject()
{
	for (int nCntPri = 0; nCntPri < MAX_PRIORITY_NUM; nCntPri++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			if (m_apObject[nCntPri][nCntObj] == NULL)
			{// NULLチェック/使っていない場合
				m_apObject[nCntPri][nCntObj] = this;		// 自分自身を代入
				m_nID = nCntObj;		// 自分自身のIDを保存
				m_nPriority = nCntPri;	// 優先順位を保存

				m_nNumAll++;		// オブジェクトの総数をカウントアップ

				break;
			}
		}
	}
}

//========================================================================
// オブジェクトクラスのコンストラクタ ( オーバーロード )
//========================================================================
CObject::CObject(int nPriority)
{
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		if (m_apObject[nPriority][nCntObj] == NULL)
		{// NULLチェック/使っていない場合
			m_apObject[nPriority][nCntObj] = this;		// 自分自身を代入
			m_nID = nCntObj;			// 自分自身のIDを保存
			m_nPriority = nPriority;	// 優先順位を保存

			m_nNumAll++;		// オブジェクトの総数をカウントアップ

			break;
		}
	}
}

//========================================================================
// オブジェクトクラスのデストラクタ
//========================================================================
CObject::~CObject()
{
}

//========================================================================
// 全てのオブジェクトを解放
//========================================================================
void CObject::ReleaseAll(void)
{
	for (int nCntPri = 0; nCntPri < MAX_PRIORITY_NUM; nCntPri++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			if (m_apObject[nCntPri][nCntObj] != NULL)
			{// NULLチェック
				// 終了処理
				m_apObject[nCntPri][nCntObj]->Uninit();
			}
		}
	}
}

//========================================================================
// オブジェクト(自分自身)を解放
//========================================================================
void CObject::Release(void)
{
	if (m_apObject[m_nPriority][m_nID] != NULL)
	{// NULLチェック
		int nID = m_nID;				// IDを保存
		int nPriority = m_nPriority;	// 優先順位を保存

		// オブジェクトの破棄
		delete m_apObject[nPriority][nID];
		m_apObject[nPriority][nID] = NULL;

		m_nNumAll--;		// オブジェクトの総数を減らす
	}
}

//========================================================================
// 全てのオブジェクトを更新
//========================================================================
void CObject::UpdateAll(void)
{
	for (int nCntPri = 0; nCntPri < MAX_PRIORITY_NUM; nCntPri++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			if (m_apObject[nCntPri][nCntObj] != NULL)
			{// NULLチェック
				// 更新処理
				m_apObject[nCntPri][nCntObj]->Update();
			}
		}
	}
}

//========================================================================
// 全てのオブジェクトを描画
//========================================================================
void CObject::DrawAll(void)
{
	CCamera* pCamera = CManager::GetCamera();		// カメラの取得

	// カメラを設定
	pCamera->SetCamera();

	for (int nCntPri = 0; nCntPri < MAX_PRIORITY_NUM; nCntPri++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			if (m_apObject[nCntPri][nCntObj] != NULL)
			{// NULLチェック
				// 描画処理
				m_apObject[nCntPri][nCntObj]->Draw();
			}
		}
	}
}