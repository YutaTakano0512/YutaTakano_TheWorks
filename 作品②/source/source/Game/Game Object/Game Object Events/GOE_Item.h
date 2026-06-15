// ===================================================
// GOE_Item.h オブジェクトイベントエフェクト
// 
// hara sougo  2025/01/12
// ===================================================
#ifndef _GOE_ITEM_H_
#define _GOE_ITEM_H_


#include "../../Systems/sys_general.h"

#include "../gameObject.h"

#include "GOE_Base.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// 列挙体定義
// ===================================================


class GOE_Item : public GOE_Base
{
	typedef GOE_Base
		SuperGOE; //親クラスにあだ名をつける
public:
    enum class Type
    {
        None,
        Heal,
    };
private:

protected:
    unsigned int m_Tex_Shadow = 0;
    unsigned int m_Tex_Eff_loop = 0;

    unsigned int m_Tex_Heal = 0;
    unsigned int m_Model_Heal = 0;

    Type m_type = Type::None;

    GAME_OBJECT_IDENTIFIER m_ObjOwner = {};
    std::string m_gayaid = {};
public:
    GOE_Item(GAME_OBJECT* obj) : SuperGOE(obj) {}
	~GOE_Item() {};

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

    void SetType(Type type)
    {
        m_type = type;
    }

    static GOE_Item* CreateItem(
        Type type,
        Float3 startPos
    );

    void SetGayaID(std::string id)
    {
        m_gayaid = id;
    }
};

#endif