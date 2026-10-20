//---------------------------------------------------------------------------
// list.cpp
// 
// sougo hara  2024/08/15
//---------------------------------------------------------------------------

#include <stdlib.h>
#include "list.h"

LIST NewList(void)
{
	//メモリ確保
	LIST list = (LIST)malloc(sizeof(LIST_MAIN));
	if (!list)
	{//メモリ確保失敗
		abort();
	}

	//リストの先頭ノードをNULLにセットする
	list->front = NULL;
	return list;
}


void ListAdd(LIST list, void* value)
{
	//メモリ確保
	LIST_NODE* node = (LIST_NODE*)malloc(sizeof(LIST_NODE));
	if (!node)
	{//メモリ確保失敗
		abort();
	}

	//ノードにデータをセット
	node->value = value;

	//新しいノードの次はNULL
	node->next = NULL;

	//末尾ノードの次に新しいノードをセット
	if (!list->front)
	{//空リストの場合
		//先頭要素に追加
		list->front = node;
	}
	else
	{
		//現在の末尾ノード(「次のノードがNULL」となっているノード)を探す
		LIST_NODE* n = list->front;
		while (n->next)
		{
			n = n->next;
		}

		//末尾ノードの次が新しいノード
		n->next = node;
	}
}

void ListForeach(LIST list, void (*func)(void*))
{
	for (LIST_NODE* node = list->front; node; node = node->next)
	{ //リストの先頭からノードを辿っていく
		//ノード内のデータに処理を適用
		func(node->value);
	}
}

void* ListGetValue(LIST list, int index)
{
	int i = 0;
	LIST_NODE* n = list->front;
	while (n != nullptr)
	{
		//指定の番号問一致したらノード内のデータを返す
		if (i == index)
		{
			return n->value;
		}
		n = n->next;
		i++;
	}
	return NULL;
}

int ListGetSize(LIST list)
{
	int i = 0;
	LIST_NODE* n = list->front;
	while (n != nullptr)
	{
		n = n->next;
		i++;
	}
	return i;
}

void ListClear(LIST list, int flag)
{
	//リストの先頭から順にノードを解放していく
	while (list->front)
	{
		LIST_NODE* node = list->front;

		//malloc等で確保した要素を追加した場合、そのメモリを解放
		if (flag)
		{
			free(node->value);
		}

		//リストの先頭ノードを繋ぎ替える
		list->front = node->next;

		//ノード解放
		free(node);
	}
}