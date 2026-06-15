// ===================================================
// GOE_Base.h キャラクターオブジェクト
// 
// hara sougo  2024/12/10
// ===================================================
#ifndef _GOE_BASE_H_
#define _GOE_BASE_H_


#include "../../Systems/sys_general.h"

#include "../gameObject.h"


class GOE_Base
{
private:

protected:
    static constexpr float ONE_BLOCK = 96.0f * 1.0f;


	GAME_OBJECT* m_Obj = nullptr;

public:
	GOE_Base(GAME_OBJECT* obj) : m_Obj(obj) {};
    virtual ~GOE_Base() {};

	virtual void Init() = 0;
	virtual void FirstFrame() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void UpdateActive() = 0;

	virtual void Draw() = 0;

	virtual void Dead() = 0;

    virtual void CalcDamage(
        GAME_OBJECT_DAMAGE_RESULT* output,

        const GAME_OBJECT* obj, const GAME_OBJECT* objAttacker,
        int damage,
        GAME_OBJECT_COLLISION_ATTACK_CHILD colAtk,
        GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec
    ) {
    };
    virtual void CalcAttackerDamage(
        GAME_OBJECT_DAMAGE_RESULT* output,

        const GAME_OBJECT* obj, const GAME_OBJECT* objAttacker,
        int damage,
        GAME_OBJECT_COLLISION_ATTACK_CHILD colAtk,
        GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec
    ) {
    };

	virtual void DamageAttacker(GAME_OBJECT_DAMAGE_RESULT dr) = 0;
	virtual void DamageReciever(GAME_OBJECT_DAMAGE_RESULT dr) = 0;

	virtual void BlockHit(GAME_OBJECT_POSMOD_RESULT pmr) = 0;

    bool IsInStage()
    {
        return IsInStageGameObject(m_Obj);
    }

    GAME_OBJECT* GetGameObj() const
    {
        return m_Obj;
    }
};

#endif