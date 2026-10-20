//---------------------------------------------------------------------------
// list.h
// 動的配列の実装
// ほとんどggったやつうつしただけ
// sougo hara  2024/08/15
//---------------------------------------------------------------------------
#ifndef _LIST_H_
#define _LIST_H_

//ノード(単方向リスト)
struct LIST_NODE {
	void* value; //データ
	struct LIST_NODE* next; //次のデータのポインタ
};

//リスト構造
struct LIST_MAIN {
	LIST_NODE* front; //先頭ノードへのポインタ
};

//リストのポインタ
typedef LIST_MAIN* LIST;

//コンストラクタ
LIST NewList(void);

//リストに要素を追加
void ListAdd(LIST , void* );

//リストの要素に処理を適用
void ListForeach(LIST, void (*)(void*));

//リストの要素を番号から取得
void* ListGetValue(LIST, int);

//リストの要素数を取得
int ListGetSize(LIST);

//リスト解放
void ListClear(LIST list, int flag);

#endif //_LIST_H_