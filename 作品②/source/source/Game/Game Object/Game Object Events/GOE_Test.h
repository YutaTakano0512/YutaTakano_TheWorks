// ===================================================
// GOE_Test.h テストオブジェクト
// 
// hara sougo  2024/07/24
// ===================================================
#ifndef _GOE_TEST_H_
#define _GOE_TEST_H_


#include "../../Systems/sys_general.h"

#include "../gameObject.h"

#include "GOE_Base.h"

class GOE_Test : public GOE_Base
{
	typedef GOE_Base
		SuperGOE; //親クラスにあだ名をつける

private:

protected:


public:
	GOE_Test(GAME_OBJECT* obj) : SuperGOE(obj) {}
    virtual ~GOE_Test() {};

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