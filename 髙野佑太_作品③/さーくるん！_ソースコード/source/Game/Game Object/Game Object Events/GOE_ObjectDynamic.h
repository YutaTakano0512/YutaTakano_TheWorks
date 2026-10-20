// ===================================================
// GOE_ObjectDynamic.h オブジェクトイベントエフェクト
// 
// hara sougo  2025/07/01
// ===================================================
#ifndef _GOE_OBJECTDYNAMIC_H_
#define _GOE_OBJECTDYNAMIC_H_


#include "../../Systems/sys_general.h"

#include "../gameObject.h"

#include "GOE_Base.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// 列挙体定義
// ===================================================

class GOE_Character_Player;

class GOE_ObjectDynamic : public GOE_Base
{
	typedef GOE_Base
		SuperGOE; //親クラスにあだ名をつける
public:
    enum class Type
    {
        None,
        General,
    };
private:
    TEX_DAT m_tex_block[5] = {};
    unsigned int m_Tex_Shadow = 0;

    Type m_type = Type::None;

    GAME_OBJECT_IDENTIFIER m_ObjOwner = {};

    int m_hitcooldown = 0;
    bool m_isFalling = false;
    int m_clearFrame = -1;

public:
    GOE_ObjectDynamic(GAME_OBJECT* obj) : SuperGOE(obj) {}
	~GOE_ObjectDynamic() {};

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

    virtual void CalcDamage(
        GAME_OBJECT_DAMAGE_RESULT* output,
        const GAME_OBJECT* obj, const GAME_OBJECT* objAttacker,
        int damage,
        GAME_OBJECT_COLLISION_ATTACK_CHILD colAtk,
        GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec
    ) override;

    void SetType(Type type)
    {
        m_type = type;
    }

    void PlayerHit(GOE_Character_Player* player);
    void Destroy();
    void Clear();

    void StartFall(int inframe=80);

    void SetOwner(GAME_OBJECT_IDENTIFIER id);

    static GOE_ObjectDynamic* CreateObjectDynamic(
        Type type,
        Float3 startPos,
        const GAME_OBJECT_COLLIDER& collider
    );
};

#endif