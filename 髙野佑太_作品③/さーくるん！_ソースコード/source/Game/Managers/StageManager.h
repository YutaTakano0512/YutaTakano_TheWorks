// ===================================================
// StageManager.h ステージ処理
// 
// hara sougo  2025/02/26
// ===================================================
#ifndef _STAGE_MANAGER_H_
#define _STAGE_MANAGER_H_


#include "../Systems/sys_general.h"
#include "../Game Object/gameObject.h"
#include "../Game Stage/Stage_Base.h"
#include "../Systems/ui_Confirm.h"

class StageManager
{
private:
    static constexpr int STAGE_MAIN_WAIT_END_FADE = 100;
    static constexpr int TUTORIAL_OUT_PUSH_BTN_FRAME = 40;
    static constexpr int TUTORIAL_CHECK_FRAME = 20;

    void update_ConfirmMenu(void);
    void draw_ConfirmMenu(void);
protected:

    Difficulty m_Diff = Difficulty::Normal;

    Stage_Base* m_Stage = nullptr;

    int m_SingleBossBattleNum = -1;

    bool m_Main_Wait_Flag = true;
    bool m_Main_Wait_EndFlag = false;
    int m_Main_Wait_EndFadeFlag = -1;

    bool m_TutorialOut_ConFlag = false;

    int m_TutorialOutPushBtn = 0;
    int m_BecomeMainPushBtn = 0;


    //TEX_DAT m_Tex_danger_middle = {};
    //TEX_DAT m_Tex_danger_high = {};
    //TEX_DAT m_Tex_stageName = {};
    //TEX_DAT m_Tex_menuback = {};
    //TEX_DAT m_Tex_ready = {};
    //TEX_DAT m_Tex_Operation_description = {};

    TEX_DAT m_Tex_Btn_ZR[2] = {};
    TEX_DAT m_Tex_Bar_ZR[3] = {};
    //TEX_DAT m_Tex_Line_ZR = {};
    TEX_DAT m_Tex_Text_ZR = {};

    TEX_DAT m_Tex_Btn_ZL = {};
    TEX_DAT m_Tex_Mission_Z = {};
    TEX_DAT m_Tex_Text_stageName = {};

    TEX_DAT m_Tex_Check = {};
    TEX_DAT m_Tex_Ttrl_Btn_B = {};
    TEX_DAT m_Tex_Ttrl_Btn_A = {};
    TEX_DAT m_Tex_Ttrl_Btn_LS = {};
    TEX_DAT m_Tex_Ttrl_Btn_X = {};

    int m_Flag_Ttrl_Btn_B[SYS_PLAYER_MAX] = {};
    int m_Flag_Ttrl_Btn_A[SYS_PLAYER_MAX] = {};
    int m_Flag_Ttrl_Btn_LS[SYS_PLAYER_MAX] = {};
    int m_Flag_Ttrl_Btn_X[SYS_PLAYER_MAX] = {};

    TEX_DAT m_Tex_Ttrl_Confirm_Text = {};

    UI_Confirm m_Ttrl_Confirm;

public:

    StageManager(Difficulty dif, int singleBossBattleNum)
        : m_Diff(dif)
        , m_SingleBossBattleNum(singleBossBattleNum)
    {};
    ~StageManager() {};

    virtual void Init();
    virtual void Uninit();
    virtual void Update();
    virtual void Draw();

    Stage_Base* GetStage()
    {
        return m_Stage;
    }
    bool GetWaitFlag() const
    {
        return m_Main_Wait_Flag;
    }
    bool IsGameover() const
    {
        return m_Stage == nullptr ? false : m_Stage->IsGameover();
    }
    bool IsGameoverConfirm() const
    {
        return m_Stage == nullptr ? false : m_Stage->IsGameoverConfirm();
    }

    //virtual void a();
};



#endif