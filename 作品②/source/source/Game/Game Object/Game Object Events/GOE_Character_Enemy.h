// ===================================================
// GOE_Character_Enemy.h キャラクターオブジェクト
// 
// hara sougo  2024/11/25
// ===================================================
#ifndef _GOE_CHARACTER_ENEMY_H_
#define _GOE_CHARACTER_ENEMY_H_


#include "../../Systems/sys_general.h"

#include "../gameObject.h"
#include "../../Entities/character.h"

#include "GOE_Character.h"

// ===================================================
// マクロ定義
// ===================================================

class GOE_Character_Enemy : public GOE_Character
{
	typedef GOE_Character
		SuperGOE; //親クラスにあだ名をつける

private:
    //static constexpr int APP_FRAME = 180;
    static constexpr int APP_FRAME = FRAME_ONE_SECOND * 5.0f;
    static constexpr int APP_IN_FRAME = 60;
    static constexpr int APP_OUT_FRAME = 25;

protected:
	Float3 m_SpawnPoint = {}; //初期位置

	GAME_OBJECT_IDENTIFIER m_TargetObjIdentifier = {}; //攻撃対象
	unsigned int m_TargetFrame = 0; //攻撃対象を見つけてから経過したフレーム

	bool m_DeadFlag = false;
    int m_DefeatScore = 0;
    int m_DefeatScoreType = 0;

    bool m_AppFlag = false;
    int m_AppFrame = -1;
    int m_AppFrameMax = -1;
    GAME_OBJECT_GRAPHIC m_AppBeforeGra = {};
    GAME_OBJECT_COLLISION m_AppBeforeCol = {};
    Float3 m_AppPos = {};
    unsigned int m_Tex_AppEf = 0;
    unsigned int m_Model_AppEf = 0;

    bool m_DefeatedByPlayerFlag = false;
public:
	GOE_Character_Enemy(GAME_OBJECT* obj) : SuperGOE(obj) {}
    virtual ~GOE_Character_Enemy() {};

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

    virtual void EnemyVanish(Float3 addPos);
    void EnemyApp();
    bool IsApp();

    void BecomeApp(int appmaxf= APP_FRAME);

    static int CalcRecvDamageMultiPlay(int damage);
    static void CalcRecvDamageGuardMod(
        float mod,
        GAME_OBJECT_COLLISION_ATTACK_CHILD colAtk,
        GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec,
        GAME_OBJECT_DAMAGE_RESULT* output
    );

    static void CalcHpMod(GAME_OBJECT* obj, int retryCount);
};

#endif