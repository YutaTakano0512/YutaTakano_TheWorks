// ===================================================
// Stage_Base.h ステージ処理
// 
// hara sougo  2024/12/13
// ===================================================
#ifndef _STAGE_BASE_H_
#define _STAGE_BASE_H_


#include "../Systems/sys_general.h"
#include "../Managers/gameManager.h"
#include "../Game Object/gameObject.h"
#include "../Entities/character.h"


class Stage_Base
{
private:
protected:
    static constexpr int ENEMIES_MAX = 128;

    Difficulty m_Difficulty = Difficulty::Normal;

    //読み込むキャラデータ
    std::vector<Character::MAIN_LOAD*> m_UseChara = {};

    //現存する敵キャラ

    GAME_OBJECT_IDENTIFIER m_Enemies[ENEMIES_MAX] = { };
    //std::vector<GAME_OBJECT_IDENTIFIER> m_Enemies = { };

    GAME_OBJECT_IDENTIFIER m_EnemyBoss = {};

    int m_CurrentWave = 0;
    int m_BeforeWave = -1;
    int m_CurrentWaveFrame = 0;

    int m_WaveCountWait = 0;

    bool m_CountStop = true;
    int m_CountFrame = 0;
    int m_CountFrameMax = 0;
    int m_CountUpFrame = 0;
    int m_SpeedClearFrame = 0;

    bool m_GameoverConfirmFlag = false;
    bool m_GameoverFlag = false;
    int m_GameoverFrame = 0;
    Float4 m_GameoverFadeColor = {};
    static constexpr int GAME_OVER_AFTER_FRAME = 120;
    int m_RetryCount = 0;
    int m_RetryCount_CurrentWave = 0;

    //マップのタイル数
    Int2 m_MapSize = { 144, 81 };


    struct StageObj {
        int type;
        Float3  pt;   // 点または線の始点（中心系）
        Float3  pt2;  // 線の終点
        float  r;  // 半径
    };

    using StageObjList = std::vector<StageObj>;
    StageObjList LoadStageData(const std::string& stagePath);

public:
    //マップのタイル数
    //static constexpr int MAP_X = 144;
    //static constexpr int MAP_Y = 81;

    Stage_Base(Difficulty dif) : m_Difficulty(dif) {};
    ~Stage_Base() {};

    virtual void Init();
    virtual void Uninit();

    virtual void Update() = 0;
    virtual void Draw() = 0;

    virtual void Retry() {};
    virtual void Clear() {};
    virtual void GiveUp() {};

    virtual int GetCountFrame() const
    {
        return m_CountFrame;
    }
    virtual int GetCountFrameMax() const
    {
        return m_CountFrameMax;
    }
    virtual int GetClearTimeFrame() const
    {
        //return m_CountFrameMax - m_CountFrame;
        return m_CountUpFrame;
    }
    virtual int GetSpeedClearTimeFrame() const
    {
        return m_SpeedClearFrame;
    }
    virtual int GetRetryCount() const
    {
        return m_RetryCount;
    }
    virtual int GetRetryCountCurrentWave() const
    {
        return m_RetryCount_CurrentWave;
    }

    virtual bool IsCountStop() const
    {
        return m_CountStop;
    }
    virtual void SetCountStop(bool b)
    {
        m_CountStop = b;
    }

    virtual void SetGameover()
    {
        m_GameoverFlag = true;
    }
    virtual bool IsGameover() const
    {
        return m_GameoverFlag;
    }
    virtual bool IsGameoverConfirm() const
    {
        return m_GameoverConfirmFlag;
    }

    void EnemyRegister(GAME_OBJECT_IDENTIFIER id)
    {
        for (int i = 0; i < ENEMIES_MAX; i++)
        {
            if (m_Enemies[i].objCreateFrame <= 0)
            {
                m_Enemies[i] = id;
                break;
            }
        }
    }

    Int2 GetStageSize()
    {
        return m_MapSize;
    }

    GAME_OBJECT* GetBossObj();
};



#endif