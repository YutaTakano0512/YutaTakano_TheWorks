// ===================================================
// GOE_Character_Enemy_Miresu_Hyper.h キャラクターオブジェクト
// 
// hara sougo  2024/12/12
// ===================================================
#ifndef _GOE_CHILD_CHARACTER_ENEMY_MIRESU_HYPER_H_
#define _GOE_CHILD_CHARACTER_ENEMY_MIRESU_HYPER_H_


#include "../../../Systems/sys_general.h"

#include "../GOE_Character_Enemy.h"

class GOE_Character_Enemy_Miresu_Hyper : public GOE_Character_Enemy
{
	typedef GOE_Character_Enemy
		SuperGOE; //親クラスにあだ名をつける

private:

protected:

public:
    static constexpr const char* CHARACTER_NAME = "ENEMY_MIRESU_ATTACKER_HYPER";

    GOE_Character_Enemy_Miresu_Hyper(GAME_OBJECT* obj) : SuperGOE(obj) {}
	~GOE_Character_Enemy_Miresu_Hyper() {};

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
};


#endif