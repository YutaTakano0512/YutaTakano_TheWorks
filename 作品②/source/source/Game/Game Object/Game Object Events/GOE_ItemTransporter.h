// ===================================================
// GOE_ItemTransporter.h オブジェクトイベントエフェクト
// 
// hara sougo  2025/01/12
// ===================================================
#ifndef _GOE_ITEMTRANSPORTER_H_
#define _GOE_ITEMTRANSPORTER_H_


#include "../../Systems/sys_general.h"

#include "../gameObject.h"

#include "GOE_Base.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// 列挙体定義
// ===================================================


class GOE_ItemTransporter : public GOE_Base
{
	typedef GOE_Base
		SuperGOE; //親クラスにあだ名をつける
public:
private:
    static constexpr int ITEM_PURGE_RANGE = ONE_BLOCK * 10.0f;

protected:
    unsigned int m_Tex_Shadow = 0;
    unsigned int m_Tex_Propeller = 0;
    unsigned int m_Tex_Main = 0;
    unsigned int m_Model_Main = 0;

    GAME_OBJECT_IDENTIFIER m_ObjTarget = {};
    GAME_OBJECT_IDENTIFIER m_ObjItem = {};
    bool m_ItemPurgeFlag = false;
    bool m_ItemTransportFlag = false;
    int m_TransportStartDelay = 0;
public:
    GOE_ItemTransporter(GAME_OBJECT* obj) : SuperGOE(obj) {}
	~GOE_ItemTransporter() {};

	virtual void Init() override;
	virtual void FirstFrame() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void UpdateActive() override;

	virtual void Draw() override;

	virtual void Dead() override;

	virtual void DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr) override;
	virtual void DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr) override;

	virtual void BlockHit(GAME_OBJECT_POSMOD_RESULT pmr) override;

    void StartTransport(int delay=0);

    static GOE_ItemTransporter* Create(
        Float3 startPos,
        GAME_OBJECT* item,
        GAME_OBJECT* target
    );
};

#endif