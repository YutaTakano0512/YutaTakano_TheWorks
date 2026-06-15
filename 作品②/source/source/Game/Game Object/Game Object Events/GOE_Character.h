// ===================================================
// GOE_Character.h キャラクターオブジェクト
// 
// hara sougo  2024/11/11
// ===================================================
#ifndef _GOE_CHARACTER_H_
#define _GOE_CHARACTER_H_


#include "../../Systems/sys_general.h"

#include "../gameObject.h"

#include "../../Entities/character.h"

#include "GOE_Base.h"

class GOE_Character : public GOE_Base
{
	typedef GOE_Base
		SuperGOE; //親クラスにあだ名をつける
public:

private:

protected:
    //unsigned int m_Tex_EfBuff = 0;
    //unsigned int m_Tex_EfBuff2 = 0;
    //unsigned int m_Tex_EfDeBuff = 0;
    //unsigned int m_Tex_EfHeal = 0;
    //static constexpr Int2 m_AnimCount_Buff = { 6, 6 };
    unsigned int m_Tex_EfBuff_Flow = 0;
    unsigned int m_Tex_EfBuff_Flare = 0;
    unsigned int m_Tex_EfHeal_Flow = 0;
    unsigned int m_Tex_EfHeal_Flare = 0;
    unsigned int m_Model_EfBuff = 0;

    int m_EfBuff2_Frame = -1;
    int m_EfBuff2_FrameMax = 1;
    Float3 m_EfBuff2_Pos = {};
    Float3 m_EfBuff2_Size = {};

	Character::OBJECT_DAT* m_CharaData = nullptr;

	int m_JumpFrame = 0;

	int m_WalkFrame = 0;

    Float3 m_BeforeVecBuf = { 0.0f, 1.0f, 0.0f };
	Float3 m_BeforeVec = { 0.0f, 1.0f, 0.0f};

	AXIS_2D m_CurrentAxis = AXIS_2D_N;
	AXIS_2D_EX m_CurrentAxisEx = AXIS_2D_EX_N;

    GAME_OBJECT_COLLISION m_CollisionDefault = {};

    Float3 m_GraphPosDefault = {};
    float m_MovieVisibleAlpha = 1.0f;
public:
	GOE_Character(GAME_OBJECT* obj) : SuperGOE(obj) {}
    virtual ~GOE_Character() {};

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

    Float3 GetBeforeVec() const { return m_BeforeVec; };
    Float3 GetBeforeVecBuf() const { return m_BeforeVecBuf; };

    virtual void GraphicSet(std::string texName, int splitX, int splitY, int cntmax = -1);
    virtual void GraphicSet(const TEX_DAT& texDat, int splitX, int splitY, int cntmax = -1);
    virtual void GraphicSpriteSet(const TEX_DAT& texDat, int splitX, int splitY, int cntmax=-1);

    //enum class CharaEffType {
    //    Buff = 0,
    //    Buff2,
    //    DeBuff,
    //    Heal,
    //};
    enum class CharaEffType {
        Buff = 0,
        Heal,
    };
    virtual void DrawCharaEff(CharaEffType type, Float3 size,
        Float3 pos = {}, int frame = -1
    );

    virtual void StartCharaEffBuff(int frame, Float3 size, Float3 pos = {});

    virtual void SetBeforeVec();
    virtual void SetBeforeVec(
        Float3* bbec,
        Float3* bbecBuf,
        float speed
    );

    Character::OBJECT_DAT* GetChara()
    {
        return m_CharaData;
    };
};

#endif