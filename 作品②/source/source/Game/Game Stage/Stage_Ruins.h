// ===================================================
// Stage_Ruins.h ステージ処理
// 
// hara sougo  2024/12/13
// ===================================================
#ifndef _STAGE_RUINS_H_
#define _STAGE_RUINS_H_


#include "../Systems/sys_general.h"
#include "../Game Object/gameObject.h"
#include "Stage_Base.h"

class Stage_Ruins : public Stage_Base
{
    typedef Stage_Base
        SuperClass; //親クラスにあだ名をつける

private:
    static constexpr int STARTING_FRAME = FRAME_ONE_SECOND * 2;
    static constexpr int STARTING_STAGE_NAME_FRAME = FRAME_ONE_SECOND * 4;

    static constexpr int LimitEnemyNum = 6;

    static constexpr int CountFrameMax = FRAME_ONE_MINUTE * 7;
    //static constexpr int CountFrameMax_H = FRAME_ONE_MINUTE * 10;
    static constexpr int CountFrameMax_H = FRAME_ONE_MINUTE * 20;
    //static constexpr int CountFrameMax = FRAME_ONE_SECOND * 10;

    static constexpr int SpeedClearFrameMax = CountFrameMax / 2;
    static constexpr int SpeedClearFrameMax_H = CountFrameMax_H / 2;

protected:

    GAME_OBJECT_IDENTIFIER m_BossObj = {};

    //unsigned int m_TexId_Mapkoubou1 = 0;

    TEX_DAT m_tex_iseki_mut[5] = {};
    TEX_DAT m_TexId_Mapnaraku = {};
    TEX_DAT m_TexId_Mapkoubou1 = {};
    TEX_DAT m_TexId_Mapkoubou2 = {};
    TEX_DAT m_TexId_MapHikari = {};
    TEX_DAT m_TexId_MapSmoke = {};
    TEX_DAT m_TexId_MapMichi = {};
    TEX_DAT m_TexId_MapTile = {};
    TEX_DAT m_TexId_MapWall = {};
    //TEX_DAT m_TexId_UI_Stage_Start = {};
    TEX_DAT m_TexId_UI_Stage_Start_L = {};
    TEX_DAT m_TexId_UI_Stage_Start_R = {};

    TEX_DAT m_TexId_UI_Stage_Name = {};

    TEX_DAT m_TexId_MutBg0 = {};
    unsigned int m_Model_Ring = 0;
    unsigned int m_Model_Ball = 0;
    int m_ShaderIndex_Ring_Vertex = 0;
    int m_ShaderIndex_Ring_Pixel = 0;

    int m_ShaderIndex_Hikari_Vertex = 0;
    int m_ShaderIndex_Hikari_Pixel = 0;
    int m_ShaderIndex_Fog_Vertex = 0;
    int m_ShaderIndex_Fog_Pixel = 0;

    StageObjList m_WaveEnemiesAndObjects[4] = {};
    std::vector<GAME_OBJECT_IDENTIFIER> m_cachedMapDynamicBlocks = {};


    int m_StartFrame = 0;

    int m_StartStageNameFrame = -1;
    int m_StartSignFrame = -1;
    
    std::vector<BLOCK*> m_cachedMapBlocks = {};

    int m_MutationFrame = -1;
    int m_DestroyStageFrame = -1;

    bool m_flag_set_mutation = false;
    bool m_flag_set_destroyStage = false;

    void update_start();
    void draw_start();

    void update_gameover();
    bool update_wave();
    void updating_wave();
    void update_current_wave();

    void update_mutation();
    void update_destroyStage();
    void set_mutation();
    void set_destroyStage();

    void create_map_objects(const StageObjList& objList);
public:

    Stage_Ruins(Difficulty dif) : SuperClass(dif){};
    ~Stage_Ruins() {};

    virtual void Init() override;
    virtual void Uninit() override;
    virtual void Update() override;
    virtual void Draw() override;

    virtual void Retry() override;
    virtual void Clear() override;
    virtual void GiveUp() override;
    //virtual void a();

    void StageEventMutation(bool isFast);
    void StageEventDestroy();

    void StageDynamicBlockClear();
};



#endif