// ===================================================
// GOE_General.h オブジェクトイベント汎用
// 
// hara sougo  2024/08/25
// ===================================================
#ifndef _GOE_GENERAL_H_
#define _GOE_GENERAL_H_


#include "../../Systems/sys_general.h"

#include "../gameObject.h"

#include "GOE_Base.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// 列挙体定義
// ===================================================

class GOE_General : public GOE_Base
{
	typedef GOE_Base
		SuperGOE; //親クラスにあだ名をつける

private:

protected:


public:
	GOE_General(GAME_OBJECT* obj) : SuperGOE(obj) {}
    virtual ~GOE_General() {};

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