// ===================================================
// modelData.h モデルのセット
// 
// hara sougo  2024/12/17
// ===================================================
#ifndef _MODEL_DATA_H_
#define _MODEL_DATA_H_

#include "../Systems/sys_general.h"

#include <iostream>

constexpr auto MODEL_PATH = "rom/model";

// ===================================================
// マクロ定義
// ===================================================
// 
// ===================================================
// 構造体宣言
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================

size_t InitModelDataFiles();
void InitModelData();
void UninitModelData();

//文字列での検索はやや重いです。
//Updateで呼ぶと重大なパフォーマンス低下を引き起こすので
//これはInitで呼ぶようにしてください。
unsigned int GetModelDataIndex(std::string name);

//GetModelDataIndexで取得した番号でモデルデータを取得する
DX_MODEL* GetModelData(unsigned int index);

#endif