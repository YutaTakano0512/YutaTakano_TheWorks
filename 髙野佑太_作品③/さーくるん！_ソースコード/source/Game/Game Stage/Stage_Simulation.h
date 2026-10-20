// ===================================================
// Stage_Simulation.h ステージ処理
// 
// hara sougo  2025/02/22
// ===================================================
#ifndef _STAGE_SIMULATION_H_
#define _STAGE_SIMULATION_H_


#include "../Systems/sys_general.h"
#include "../Game Object/gameObject.h"
#include "Stage_Base.h"

class Stage_Simulation : public Stage_Base
{
    typedef Stage_Base
        SuperClass; //親クラスにあだ名をつける

private:
    static constexpr int STARTING_FRAME = FRAME_ONE_SECOND * 2;
    static constexpr int STARTING_STAGE_NAME_FRAME = FRAME_ONE_SECOND * 4;

    static constexpr int LimitEnemyNum = 6;

protected:

    TEX_DAT m_TexId_UI_Stage_Start_L = {};
    TEX_DAT m_TexId_UI_Stage_Start_R = {};

    TEX_DAT m_TexId_UI_Stage_Name = {};

    TEX_DAT m_TexId_Yuka = {};
    unsigned int m_Model_Yuka = 0;
    unsigned int m_Model_Ring = 0;

    int m_ShaderIndex_Ring_Vertex = 0;
    int m_ShaderIndex_Ring_Pixel = 0;
    int m_ShaderIndex_Floor_Vertex = 0;
    int m_ShaderIndex_Floor_Pixel = 0;
    int m_ShaderIndex_Hikari_Vertex = 0;
    int m_ShaderIndex_Hikari_Pixel = 0;


    GAME_OBJECT_IDENTIFIER m_BossObj = {};

    int m_StartFrame = 0;

    int m_StartStageNameFrame = -1;
    int m_StartSignFrame = -1;

    int m_SingleBossBattleNum = -1;

    int m_DelayedClearFrame = 0;
    bool m_IsCleared = false;
private:
    void update_start();
    void draw_start();

    void update_gameover();
    bool update_wave();
    void updating_wave();
    void update_current_wave();

    void clear_stage();
public:

    Stage_Simulation(
        Difficulty dif,
        int singleBossBattleNum
    ) 
        : SuperClass(dif)
        , m_SingleBossBattleNum(singleBossBattleNum)
    {};
    ~Stage_Simulation() {};

    virtual void Init() override;
    virtual void Uninit() override;
    virtual void Update() override;
    virtual void Draw() override;

    virtual void Retry() override;
    virtual void Clear() override;
    virtual void GiveUp() override;

    //virtual void a();
};



#endif