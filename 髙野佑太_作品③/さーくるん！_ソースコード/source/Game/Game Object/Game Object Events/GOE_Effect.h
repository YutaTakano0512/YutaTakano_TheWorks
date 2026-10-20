// ===================================================
// GOE_Effect.h オブジェクトイベントエフェクト
// 
// hara sougo  2025/01/06
// ===================================================
#ifndef _GOE_EFFECT_H_
#define _GOE_EFFECT_H_


#include "../../Systems/sys_general.h"

#include "../gameObject.h"
#include "../../Systems/camera.h"

#include "GOE_Base.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// 列挙体定義
// ===================================================

enum GO_EFFECT_TYPE {
	GO_EFFECT_TYPE_VOID = 0,

	GO_EFFECT_TYPE_SHOCKWAVE,
	GO_EFFECT_TYPE_FLASH_0,
	GO_EFFECT_TYPE_FLASH_1,

	GO_EFFECT_TYPE_SHOCKWAVE_ENEM,
	GO_EFFECT_TYPE_FLASH_0_ENEM,
	GO_EFFECT_TYPE_FLASH_1_ENEM,

	GO_EFFECT_TYPE_TWINKLE,
	GO_EFFECT_TYPE_SPARK,

    GO_EFFECT_TYPE_EXCLAMATION,
    GO_EFFECT_TYPE_QUESTION,

	GO_EFFECT_TYPE_SHOCKWAVE_3D,
	GO_EFFECT_TYPE_HITMARK, //未使用

    GO_EFFECT_TYPE_N_ATTACK_HIT,
    GO_EFFECT_TYPE_HITSTAR_VANISH,
    GO_EFFECT_TYPE_SMOKE,

    GO_EFFECT_TYPE_EXPLOSION_1,
    GO_EFFECT_TYPE_EXPLOSION_2,
    GO_EFFECT_TYPE_EXPLOSION_3,
    GO_EFFECT_TYPE_EXPLOSION_4,
    GO_EFFECT_TYPE_EXPLOSION_SMOKE,

    GO_EFFECT_TYPE_EXPLOSION_1_GROUND,
    GO_EFFECT_TYPE_EXPLOSION_1_GROUND2,

    GO_EFFECT_TYPE_HAMMER_SHOCKWAVE,
    GO_EFFECT_TYPE_HAMMER_SHOCKDECAL,
    GO_EFFECT_TYPE_HITSTAR_VANISH_REV,
    GO_EFFECT_TYPE_SCOREEFF,
    GO_EFFECT_TYPE_SCOREEFF_REV,
    GO_EFFECT_TYPE_HAMMER_CHARGE_LOOP,
    GO_EFFECT_TYPE_HAMMER_CHARGE_LOOP_GROUND,
    GO_EFFECT_TYPE_HAMMER_CHARGE_END,
    GO_EFFECT_TYPE_HAMMER_CHARGE_END_GROUND,
    GO_EFFECT_TYPE_HAMMER_CHARGE_END_NON_BILLBOARD,

    GO_EFFECT_TYPE_WARP_IN,
    GO_EFFECT_TYPE_WARP_OUT,

    GO_EFFECT_TYPE_OVERLAY_BALL,

    GO_EFFECT_TYPE_AFTEREFFECT,

	GO_EFFECT_TYPE_MAX,
};

class GOE_Effect : public GOE_Base
{
	typedef GOE_Base
		SuperGOE; //親クラスにあだ名をつける

private:
	int m_Dpd_Num = 0;
	DRAW_POLYGON_DAT* m_Dpd = nullptr;
	DRAW_POLYGON_IN_CAMERA_EX_OPTION* m_DpdOpt = nullptr;

protected:


public:
	GOE_Effect(GAME_OBJECT* obj) : SuperGOE(obj) {}
	~GOE_Effect() {};

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

GAME_OBJECT* GO_CreateEffect(GO_EFFECT_TYPE et, Float3 pos);
GAME_OBJECT* GO_CreateEffect(GO_EFFECT_TYPE et, GAME_OBJECT* owner);

GAME_OBJECT* GO_CreateAfterEffect(GAME_OBJECT* owner, int life, Float4 color);

void GO_CreateExplosion(Float3 pos, float sca);

#endif