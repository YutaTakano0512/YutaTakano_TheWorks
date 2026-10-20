// ===================================================
// SCENE_Result.cpp 
//
// hara sougo  2024/07/22
// ===================================================
#include "../game_main.h"

#include "../Core/Input/controller_main.h"
#include "../Game/Texture/texData.h"
#include "../Game/Managers/bg.h"
#include "../Game/Managers/gameManager.h"
#include "../Game/Managers/soundManager.h"
#include "../Game/Managers/scene.h"
#include "../Game/Systems/camera.h"
#include "../Game/Systems/fade.h"
#include "../Game/Systems/ui.h"
#include"../Game/Sprite/spriteNumber.h"
#include "SCENE_Result.h"

#include <iostream>
#include <fstream>

#define MENU_UI_MAX (2)
//スコア表示用

#define NUMBER_DIGIT			(5)								// スコアの桁
#define NUMBER_NUMBER_GAP		(0.0f)						    // スコアの数値と数値の隙間
#define NUMBER_PATTERN_NUM_W	(5)								// 横パターン数
#define NUMBER_PATTERN_NUM_H	(5)								// 縦パターン数
#define NUMBER_PATTERN_SIZE_W	(1.0f / NUMBER_PATTERN_NUM_W)	// 横パターンサイズ
#define NUMBER_PATTERN_SIZE_H	(1.0f / NUMBER_PATTERN_NUM_H)	// 縦パターンサイズ
//ランキング保存
#define MAX_SCORES (1000)   // 保存できる最大スコア数
#define TOP_N (5)           // 表示するスコア数

enum RESULT_FILE_TYPE {
    RESULT_FILE_TYPE_SCORE = 0,
    RESULT_FILE_TYPE_TIME,
    RESULT_FILE_TYPE_MAX,
};
struct RESULT_FILE {
    RESULT_FILE_TYPE type;
    const char* name;
};
constexpr RESULT_FILE RESULT_FILES[] = {
    {RESULT_FILE_TYPE_SCORE, "rom\\result_data\\scores_normal.txt" },
    {RESULT_FILE_TYPE_SCORE, "rom\\result_data\\scores_hard.txt" },

    {RESULT_FILE_TYPE_TIME, "rom\\result_data\\times_normal.txt" },
    {RESULT_FILE_TYPE_TIME, "rom\\result_data\\times_hard.txt" },
};
constexpr int SCORE_SAVE_DAT_LIST_NUM = ARRAYSIZE(RESULT_FILES);

//スコアのアニーション
#define SCORE_ANIM_TIME (50)    //スコアのアニメの時間(フレーム)
#define SCORE_DISTANCE (42)     //リザルトのスコアの縦の隙間距離
#define SCORE_GAP (-5)          //リザルトのスコアの数字の隙間
#define SCORE_SIZE (30)         //スコアの大きさ
#define SCORE_POS_X (180)       //スコアのX座標
#define SCORE_POS_Y (-170)      //スコアのY座標
#define SCORE_ZOOM (1.2)        //計算中のスコアの大きさ
//#define SCORE_TOTAL_SIZE (55)   //トータルスコアサイズ
//#define SCORE_TOTAL_POSX (170)  //トータルスコアのX座標
//#define SCORE_TOTAL_POSY (70)   //トータルスコアのY座標
//#define SCORE_TOTAL_GAP (-15)   //トータルスコアの数字の隙間
#define SCORE_TOTAL_SIZE (45)   //トータルスコアサイズ
#define SCORE_TOTAL_POSX (170)  //トータルスコアのX座標
#define SCORE_TOTAL_POSY (65)   //トータルスコアのY座標
#define SCORE_TOTAL_GAP (-12)   //トータルスコアの数字の隙間
#define SCORE_TOTAL_ZOOM (1.1)  //計算中のトータルスコアの大きさ
//紙のアニメーション
//リザルト終了時
#define MOVE_RESULTPAPER_X (-150)//リザルトの動き(x方向)
#define MOVE_RESULTPAPER_Y (0)//リザルトの動き(y方向)
#define MOVE_RESULTTIME (60)//リザルトの紙が動く時間(フレーム)
#define MOVE_RESULT_EASING (EasingOutQuart)
#define SELECT_START_X (-200)//セレクト紙の初期位置(x)
#define SELECT_START_Y (0)//セレクト紙の初期位置(y)
//ランキング画面に切り替え
#define MOVE_RANKING_START_X (0)
#define MOVE_RANKING_START_Y (0)
#define MOVE_RANKINGPAPER_X (0)
#define MOVE_RANKINGPAPER_Y (0)
#define MOVE_RANKINGTIME (00)
#define MOVE_RESULTPAPER_X_RANK (0)
#define MOVE_RESULTPAPER_Y_RANK (500)
//ランク
//#define RANK_RANGE (5000) //ランクの範囲
//#define RANK_POSX (-50)    //ランクのX座標
//#define RANK_POSY (72)      //ランクのY座標
#define RANK_POSX (-70)    //ランクのX座標
#define RANK_POSY (92)      //ランクのY座標

//static TEX_DAT g_tex_result_clear = {};
//static TEX_DAT g_tex_result_gameover = {};
//static TEX_DAT g_tex_result_totitle_text = {};

enum ResultAnimeStep//リザルトのアニメーション
{
    STEP_START_ZOOM,              //始めの動き(現在はズームのみ)
    STEP_SCORE_START,             //スコアのアニメを始める
    STEP_SCORE_FINISHED_ANIME,    //スコアのアニメ終了
    STEP_RANK,                    //ランク
    STEP_STAMP,                   //スタンプ押す
    STEP_MOVE_RESULT_PAPER,
    STEP_MOVE_RANKING_PAPER,
    STEP_ANIME_FINISHED,


};

enum ScoreStep//スコアのアニメーション(数字のみ)
{
    STEP_NORMAL_SCORE,
    STEP_CLEAR_SCORE,
    STEP_ENEMY_KILL_SCORE,
    STEP_RANDUM_EVENT_SCORE,
    STEP_BOSS_SCORE,
    STEP_CLEAR_TIME_SCORE,
    STEP_TOTAL_SCORE,
    STEP_SCORE_FINISHED
};

struct SCORE_SAVE_DAT {
    int score;
    int time;

    int charaNum;
    PLAYER_CHARACTER_TYPE chara[SYS_PLAYER_MAX];
};

struct SCORE_SAVE_DAT_RANK {
    RESULT_FILE_TYPE type;

    std::vector<SCORE_SAVE_DAT> rank;
};

struct SCORE_SAVE_DAT_LIST {
    SCORE_SAVE_DAT_RANK scores[SCORE_SAVE_DAT_LIST_NUM];
};

static TEX_DAT g_tex_clear_result = {};
static TEX_DAT g_tex_title_text = {};
static TEX_DAT g_tex_ranking_text = {};
static TEX_DAT g_tex_replay_text = {};
static TEX_DAT g_tex_end_text = {};
static TEX_DAT g_tex_menu[4] = {};

static TEX_DAT g_tex_gameover_text = {};
static TEX_DAT g_tex_score_text = {};
static TEX_DAT g_tex_result_bg_desk = {};
static TEX_DAT g_tex_result_paper = {};
static TEX_DAT g_tex_result_light = {};
static TEX_DAT g_tex_result_paper_text = {};
static TEX_DAT g_tex_result_clear_stamp = {};
static TEX_DAT g_tex_result_failure_stamp = {};
static TEX_DAT g_numberTex = {};

static TEX_DAT g_tex_rank[5] = {};

static TEX_DAT g_tex_result_ranking_paper = {};
static TEX_DAT g_tex_result_select_paper = {};

static TEX_DAT g_tex_title_text_on = {};
static TEX_DAT g_tex_result_text_on = {};
static TEX_DAT g_tex_ranking_text_on = {};

static TEX_DAT g_tex_chara[3] = {};

static TEX_DAT g_tex_deffecult[2] = {};
static TEX_DAT g_tex_arrow = {};


static unsigned int g_tex_Meter_time_01 = 0;
static unsigned int g_tex_time_icon = 0;
static unsigned int g_tex_teamlogo = 0;
//TEX_TYPE_UI_RESULT_RANKING_TEXT_ON
//TEX_TYPE_UI_RESULT_RANKING_RESULT_TEXT_ON
//TEX_TYPE_UI_RESULT_TITLE_TEXT_ON
//メニュー
static int menu_num;

static float g_in;

//ランキング画面
static bool isranking = false;

//static int scores[MAX_SCORES] = {};  // ランキング用のスコアの配列
//static int count;               //ランキングカウント
static SCORE_SAVE_DAT_LIST scores = {};  // ランキング用のスコアの配列

static bool disp_ranking_normal;       //ランキングの難易度表示(trueはノーマル)
static bool disp_ranking_time;       //ランキングのタイム表示(trueはタイマー)

//スコア
static int clear_bonus;        //クリアボーナス
static int enemy_bonus ;       // 敵撃破数ボーナス
static int randomevent_bonus;  // ランダムイベントボーナス
static int no_damage_bonus ;   //ノーダメージボーナス
static int cler_time_bonus;    //クリアタイムボーナス
static int total_score;        //トータルスコア

//スコアアニーション
static int random_score;
//アニメ終了
static bool isAnime;

//シェイク処理
static float shakeOffset;
static bool is_Shake;
static int shakeFrame;

//ズーム処理
static float zoom;
static float zoomProgress; // 進行度（0.0 ～ 1.0）
//ムーブ処理
static float MoveResultProgress;//リザルトのムーブ進行度
static float MoveRankingProgress;//ランキングのムーブ進行度
static float StampProgress;
static float StampProgress2;
static float RankProgress;

static int elapsed_time; // アニメーション用の経過時間
ResultAnimeStep anime_step;
ScoreStep current_step;
static GAME_MANAGER_SAVE_RESULT result = {};
//BGM処理
static float bgmTotalFrame;

static bool isFanfarePlaying;
static int bgmFrameCount;

//static int players;//参加プレイ人数
//static PLAYER_CHARACTER_TYPE players_chara[SYS_PLAYER_MAX];//参加キャラ


void SpriteResultDraw(int textureid, Float2 pos, Float2 size, DRAW_POLYGON_PRIORITY priority, Float4 color,float rot=0.0f);
void SpriteResultNumberGapDraw(int v, Float2 pos, Float2 size, float gap,bool texon=false);

//int LoadScores(int scores[], int max_scores,bool dif);
//void SaveScores(int scores[], int count);
//void SortScores(int scores[], int count);
void LoadScores(SCORE_SAVE_DAT_LIST& scoresL);
void SaveScores(SCORE_SAVE_DAT_LIST& scoresL);
void SortScores(SCORE_SAVE_DAT_LIST& scoresL);

void Result_Score_Anime(int& random_score, int score, int time, int& elapsed_time, ScoreStep next);

bool camera_zoom(float targetZoom, float duration, float deltaTime);
static Float2 F2Lerp(float posx, float posy, float aftermoveingx, float aftermoveingy,
    float movetime, std::function<float(float)> easing, float& progress= MoveResultProgress);
static float FLerp(float start, float end, float movetime, std::function<float(float)> easing, float& progress);
static Float2 F2Lerpshare(float startX, float startY, float endX, float endY,
    float progress, std::function<float(float)> easing);
static float FLerpshare(float start, float end,
    float progress, std::function<float(float)> easing);
static void UpdateProgress(float& progress, float moveTime);

void chara_icon_draw(PLAYER_CHARACTER_TYPE playerschara[SYS_PLAYER_MAX], int playernum, float posx, float posy);
float LerpEasePingPong(float start, float end, float currentTime, float duration, std::function<float(float)> easing);


// ===================================================
// シーンの初期化
// ===================================================
void InitializeSceneResult(void)
{
    //SetMainGameDrawFPS(2);


    //以下追加（2024_11_28 福田）
  //テクスチャ
    g_tex_title_text = GetTexData(TEX_TYPE_UI_RESULT_TITLE_TEXT);

   
    g_tex_ranking_text = GetTexData(TEX_TYPE_UI_RESULT_RANKING_TEXT);
 

    g_tex_result_bg_desk = GetTexData(TEX_TYPE_BG_RESULT);
   
   
    g_tex_result_paper = GetTexData(TEX_TYPE_UI_RESULT_PAPER);

    

    g_tex_result_clear_stamp = GetTexData(TEX_TYPE_UI_RESULT_CLERSTAMP);
    g_tex_result_failure_stamp = GetTexData(TEX_TYPE_UI_RESULT_FAILURESTAMP);

    g_numberTex = GetTexData(TEX_TYPE_UI_NUMBER);

  

    g_tex_rank[4] = GetTexData(TEX_TYPE_UI_RESULT_SCORE_RANK_SS);
    g_tex_rank[3] = GetTexData(TEX_TYPE_UI_RESULT_SCORE_RANK_S);
    g_tex_rank[2] = GetTexData(TEX_TYPE_UI_RESULT_SCORE_RANK_A);
    g_tex_rank[1] = GetTexData(TEX_TYPE_UI_RESULT_SCORE_RANK_B);
    g_tex_rank[0] = GetTexData(TEX_TYPE_UI_RESULT_SCORE_RANK_C);//0～5000
    g_tex_result_ranking_paper = GetTexData(TEX_TYPE_UI_RESULT_RANKING_PAPER);

    g_tex_result_select_paper = GetTexData(TEX_TYPE_UI_RESULT_SELECT_PAPER);

    g_tex_title_text_on   = GetTexData(TEX_TYPE_UI_RESULT_TITLE_TEXT_ON);
    g_tex_result_text_on  = GetTexData(TEX_TYPE_UI_RESULT_RESULT_TEXT_ON);
    g_tex_ranking_text_on = GetTexData(TEX_TYPE_UI_RESULT_RANKING_TEXT_ON);

    //メニューのテキスト
    g_tex_menu[0] = g_tex_title_text;
    g_tex_menu[1] = g_tex_ranking_text;
      g_tex_menu[2] = g_tex_title_text_on;
      g_tex_menu[3] = g_tex_ranking_text_on;
      /*  g_tex_menu[2] = g_tex_replay_text;
        g_tex_menu[3] = g_tex_end_text;*/
	//メニュー
      
	menu_num = 1;

    g_tex_chara[0] = GetTexData(TEX_TYPE_CHARACTER_KAITO_ON);
    g_tex_chara[1] = GetTexData(TEX_TYPE_CHARACTER_NEO_ON);
    g_tex_chara[2] = GetTexData(TEX_TYPE_CHARACTER_KONTO_ON);
    //難易度
    g_tex_deffecult[0] = GetTexData(TEX_TYPE_UI_RESULT_NORMAL);
    g_tex_deffecult[1] = GetTexData(TEX_TYPE_UI_RESULT_HARD);

    g_tex_arrow = GetTexData(TEX_TYPE_CHARACTER_SELECT_UP);//ランキングの選択

    //g_tex_Meter_time_01 = LoadTexData("TEX/ui/in_game_master/countdown/Meter_time_01").textureId;
    g_tex_Meter_time_01 = LoadTexData("TEX/ui/in_game_master/countdown/time_stick_01").textureId;
    g_tex_time_icon = LoadTexData("TEX/ui/in_game_master/countdown/time_stick_icon").textureId;
    g_tex_teamlogo = LoadTexData("TEX/ui/result/teamlogo").textureId;

	isranking = false;

    //振動の処理
    shakeOffset = 0.0f;
    is_Shake = false;
    shakeFrame = 0.0f;

    //for (int i = 0; i < MAX_SCORES; i++) {
    //    scores[i] = 0;
    //}
    for (int i = 0; i < SCORE_SAVE_DAT_LIST_NUM; i++) {
        scores.scores[i].rank.clear();
        scores.scores[i].rank.shrink_to_fit();
    }

    elapsed_time = 0;
	UIReset();
    //
    GameManagerSetSceneRequestCancel();

    ResetCamera(); //すまん InitCameraは初期化用だからこっちよんでもろて (2025/01/19 Hara)

    SetBg(BG_TYPE_NONE);

	GameStopResetRequest();
    //スコア
    result = GameManagerGetResult();

    if (result.isClear == true) {
        //BGMを再生
        
        //SoundBGMPlayRequest(SOUND_TYPE_BGM_4, 1, 0.8f);
        SoundBGMIntroPlayRequest(SOUND_TYPE_BGM_4, 1, 0.8f);
        SoundBGMPlayRequest(GetSoundData("SOUND/BGM/Result_BGM_Loop"), 
            1, 0.8, FRAME_ONE_MINUTE * 0 + (FRAME_ONE_SECOND * 8)
        );
    }
    else if(result.isClear == false)
    {
        //SoundBGMPlayRequest(SOUND_TYPE_BGM_5, 1, 0.8f);
        
        //ループしないようにする
        //2025/03/09 Hara
        SoundBGMIntroPlayRequest(SOUND_TYPE_BGM_5, 1, 0.4f);
    }
    


    
    /*result.players;*/
   /* result.score = 10000;*/
    clear_bonus = result.scoreEx[RESULT_SCORE_EX_TYPE_CLEAR];//クリアボーナス
    enemy_bonus = result.scoreEx[RESULT_SCORE_EX_TYPE_ENEMY_DEFEAT]
        + result.scoreEx[RESULT_SCORE_EX_TYPE_MANY_ENEMY_DEFEAT];  // 敵撃破数ボーナス
    randomevent_bonus = result.scoreEx[RESULT_SCORE_EX_TYPE_RARE_ENEMY_DEFEAT];  // ランダムイベントボーナス
    no_damage_bonus = result.scoreEx[RESULT_SCORE_EX_TYPE_DAMAGE_LESS];    //ノーダメージボーナス
    cler_time_bonus = result.scoreEx[RESULT_SCORE_EX_TYPE_SPEED_CLEAR];//クリアタイムボーナス
   
    total_score =
        clear_bonus
        + enemy_bonus
        + randomevent_bonus
        + no_damage_bonus
        + cler_time_bonus
    ;

   
    //ランダムスコア
    random_score = 0;

   
    //リザルトアニメ管理
    anime_step = STEP_START_ZOOM;

    //スコアアニメ管理
    current_step = STEP_NORMAL_SCORE;
    isAnime = false;
    //ズームリセット
    zoom = 1;
    zoomProgress=0;
    //ムーブリセット
    MoveResultProgress = 0;
    MoveRankingProgress = 0;
    StampProgress = 0;
    StampProgress2 = 0;
    RankProgress = 0;

    bgmTotalFrame = FRAME_ONE_MINUTE * 0 + (FRAME_ONE_SECOND * 11);
    
    bgmFrameCount = 0;
    isFanfarePlaying = true;
    
    ////最初に表示するランキング
    //if (result.difficulty == Difficulty::Normal)
    //{
    //    disp_ranking_normal=true;
    //}
    //else if (result.difficulty == Difficulty::Hard)
    //{
    //    disp_ranking_normal = false;
    //}
    //else
    //{
    //    disp_ranking_normal = false;
    //}
    //count = LoadScores(scores, MAX_SCORES, disp_ranking_normal);//スコアのロード

    // 新しいランキングの入力
    //scores[count++] = total_score;

    // スコアをファイルに保存
    //SaveScores(scores, count);

    //スコアのロード
    LoadScores(scores);

    //最初に表示するランキング
    disp_ranking_normal = result.difficulty == Difficulty::Normal;
    disp_ranking_time = false;

    //新しいランキングの入力
    {
        SCORE_SAVE_DAT savedat = {};
        savedat.score = total_score;
        savedat.time = result.clearTimeFrame;

        savedat.charaNum = result.playersNum;
        for (int j = 0; j < SYS_PLAYER_MAX; j++) {
            savedat.chara[j] = result.players[j];
        }

        //スコア
        scores.scores[!disp_ranking_normal + 0].rank.push_back(savedat);
        //クリアタイム
        if (result.isClear)
        {
            scores.scores[!disp_ranking_normal + 2].rank.push_back(savedat);
        }
    }

    //スコアをソート
    //for (int i = 0; i < SCORE_SAVE_DAT_LIST_NUM; i++)
    //{
    //    SortScores(scores.scores[i]);
    //}
    SortScores(scores);

    //スコアをファイルに保存
    SaveScores(scores);

}

// ===================================================
// シーンの更新
// ===================================================
void UpdateSceneResult(void)
{
   
    //// スコアをソート（降順）
    //SortScores(scores, count);

    //if (result.isClear == true) {

    //    if (isFanfarePlaying)
    //    {
    //        bgmFrameCount++;
    //        if (bgmFrameCount >= bgmTotalFrame)
    //        {
    //            SoundBGMStopRequest(30);
    //            SoundBGMPlayRequest(SOUND_TYPE_BGM_3, 1, 0.8f);
    //            isFanfarePlaying = false;
    //        }
    //    }
    //}
  
    

    //フェード終了
    if (!IsFading())
    {

        CONTROLLER_STICK_DAT stickTrigger_Dat = GetControllerMainLeftStickTrigger();
        bool stickTrigger_UP = false;
        bool stickTrigger_DOWN = false;
        bool stickTrigger_LEFT = false;
        bool stickTrigger_RIGHT = false;
        // スティックの入力を受け取る
        if (stickTrigger_Dat.enable)
        {
            if (stickTrigger_Dat.stick.y > 0.4f)
            {
                stickTrigger_UP = true;
            }
            if (stickTrigger_Dat.stick.y < -0.4f)
            {
                stickTrigger_DOWN = true;
            }
            if (stickTrigger_Dat.stick.x > 0.4f)
            {
                stickTrigger_RIGHT = true;
            }
            if (stickTrigger_Dat.stick.x < -0.4f)
            {
                stickTrigger_LEFT = true;
            }
        }


        float ZoomTime = 1.0f / 60.0f;
       
        //リザルトのアニメーション処理
        switch (anime_step)
        {
        case STEP_START_ZOOM:
            isAnime = true;
            // カメラズームが完了するまで待機
            if (!camera_zoom(2.0f, 1.0f, ZoomTime))
            {
                return; // ズーム完了前は何もしない
               
            }

            anime_step = STEP_SCORE_START;
            break;
        case STEP_SCORE_START:
            if (current_step == STEP_NORMAL_SCORE)
            {
                current_step = STEP_CLEAR_SCORE;
            }
            break;
        case STEP_SCORE_FINISHED_ANIME:

            elapsed_time++;

            if (elapsed_time >= 0.0f)
            {
                is_Shake = true;
                elapsed_time = 0;
                anime_step = STEP_RANK;

            }
            break;

        case STEP_RANK:
            elapsed_time++;
            UpdateProgress(RankProgress, 20);
            if (RankProgress >= 1.0f)
            {
                is_Shake = true;
                elapsed_time = 0;
                anime_step = STEP_STAMP;
            }
            break;
        case STEP_STAMP:

            ////クリアしていなかったら
            ////スタンプはスキップする
            ////2025/03/09 Hara
            //if (!result.isClear)
            //{
            //    anime_step = STEP_MOVE_RESULT_PAPER;
            //}
            //else
            {
                UpdateProgress(StampProgress, 20);
                if (StampProgress >= 1.0f)
                {
                    //クリア時のみ揺らす
                    if (result.isClear 
                        && StampProgress2 == 0.0f)
                    {
                        CameraShakeRequestAll(CAMERA_SHAKE_SMALL_FRAME, CAMERA_SHAKE_SMALL_POWER);
                        is_Shake = true;
                    }
                    UpdateProgress(StampProgress2, 20);
                }
                if (StampProgress2 >= 1.0f) {
                    anime_step = STEP_MOVE_RESULT_PAPER;
                }
                /* if (StampProgress >= 1.0f)
                 {

                     is_Shake = true;
                     anime_step = STEP_MOVE_RESULT_PAPER;
                 }*/
            }
            break;

        case STEP_MOVE_RESULT_PAPER:
            if (MoveResultProgress >= 1.0)
            {
                is_Shake = true;
                anime_step = STEP_ANIME_FINISHED;
            }
            break;
        case STEP_MOVE_RANKING_PAPER:
            UpdateProgress(MoveRankingProgress, MOVE_RANKINGTIME);
            if (MoveRankingProgress >= 1.0)
            {
                anime_step = STEP_ANIME_FINISHED;
            }

            break;
        case STEP_ANIME_FINISHED:
            isAnime = false;
            //コントローラー処理
            if ((GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_UP)|| stickTrigger_UP))
            {
                SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT, 4.0f, 0.0f);
               
                menu_num--;
                if (menu_num < 0) {
                    menu_num = 1;
                }
            }
            if ((GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_DOWN)|| stickTrigger_DOWN))
            {
                SoundEffectPlayRequest(SOUND_TYPE_SE_UI_SELECT, 4.0f, 0.0f);

                menu_num++;
                if (menu_num > 1) {
                    menu_num = 0;
                }
               
            }
           
            if ((GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_A)))
            {
                is_Shake = true;
                if (menu_num == 0)
                {
                    
                    SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0.0f);
                    SetSceneFade(SCENE_TITLE);

                }
                else if (menu_num == 1)
                {
                    SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0.0f);
                    isranking = !isranking;
                    if (isranking)
                    {
                        MoveRankingProgress = 0;
                        anime_step = STEP_MOVE_RANKING_PAPER;
                    }

                }
            }

            if (isranking) 
            {
                if ((GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_LEFT) || stickTrigger_LEFT))
                {
                    SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0.0f);

                    disp_ranking_normal = !disp_ranking_normal;
                    //for (int i = 0; i < MAX_SCORES; i++) {
                    //    scores[i] = 0;
                    //}
                    //count = LoadScores(scores, MAX_SCORES, disp_ranking_normal);//スコアのロード

                }
                if ((GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_RIGHT) || stickTrigger_RIGHT))
                {
                    SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0.0f);

                    disp_ranking_normal = !disp_ranking_normal;
                    //for (int i = 0; i < MAX_SCORES; i++) {
                    //    scores[i] = 0;
                    //}
                    //count = LoadScores(scores, MAX_SCORES, disp_ranking_normal);//スコアのロード

                }

                if (GetControllerMainTrigger(CONTROLLER_BUTTON_TYPE_Y))
                {
                    SoundEffectPlayRequest(SOUND_TYPE_SE_UI_OK, 4.0f, 0.0f);

                    disp_ranking_time = !disp_ranking_time;

                }
            }
            break;
        default:
            break;
        }
            //スコアのアニメーション処理
            switch (current_step)
            {
            case STEP_NORMAL_SCORE:

                break;
            case STEP_CLEAR_SCORE:
                Result_Score_Anime(random_score, clear_bonus, SCORE_ANIM_TIME, elapsed_time, STEP_ENEMY_KILL_SCORE);

                break;

            case STEP_ENEMY_KILL_SCORE:
                Result_Score_Anime(random_score, enemy_bonus, SCORE_ANIM_TIME, elapsed_time, STEP_RANDUM_EVENT_SCORE);

                break;

            case  STEP_RANDUM_EVENT_SCORE:
                Result_Score_Anime(random_score, randomevent_bonus, SCORE_ANIM_TIME, elapsed_time, STEP_BOSS_SCORE);

                break;
            case  STEP_BOSS_SCORE:
                Result_Score_Anime(random_score, no_damage_bonus, SCORE_ANIM_TIME, elapsed_time, STEP_CLEAR_TIME_SCORE);

                break;
            case   STEP_CLEAR_TIME_SCORE:
                Result_Score_Anime(random_score, cler_time_bonus, SCORE_ANIM_TIME, elapsed_time, STEP_TOTAL_SCORE);

                break;
            case  STEP_TOTAL_SCORE:
                Result_Score_Anime(random_score, total_score, SCORE_ANIM_TIME, elapsed_time, STEP_SCORE_FINISHED);

                break;
            case STEP_SCORE_FINISHED:
                // すべてのスコアアニメーションが終わった後の処理
                if (anime_step == STEP_SCORE_START)
                    anime_step = STEP_SCORE_FINISHED_ANIME;

                break;
            }
    }

}


// ===================================================
// シーンの描画
// ===================================================
void DrawSceneResult(void)
{
    //背景の深度が DRAW_POLYGON_PRIORITY_DEEPEST なので、
    //必ずそれより上に表示するために
    //一段階上のものに変えた (2025/01/19 Hara)

    //新しく追加2024/12/25

    //リザルトとランキング画面り替え
    if (isranking) {
        g_tex_menu[1] = GetTexData(TEX_TYPE_UI_RESULT_RANKING_RESULT_TEXT);
        g_tex_menu[3] = g_tex_result_text_on;
    }
    else
    {
        g_tex_menu[1] = g_tex_ranking_text;
        g_tex_menu[3] = g_tex_ranking_text_on;
    }
    // 共通
    //カメラシェイク
    if (is_Shake)
    {
        shakeOffset = sin(shakeFrame) * 5.0f;
        shakeFrame++;

        if (shakeFrame > 10.0f)
        {
            is_Shake = false;
            shakeOffset = 0.0f;
            shakeFrame = 0.0f;
        }
    }

    //背景
    SpriteResultDraw(g_tex_result_bg_desk.textureId, MkF2(0.0f, 0.0f), MkF2(SCREEN_WIDTH , SCREEN_HEIGHT ),DRAW_POLYGON_PRIORITY_NML_DEEPEST, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
   /*SpriteResultDraw(g_tex_result_bg_desk_obj.textureId, MkF2(0.0f, 0.0f), MkF2(SCREEN_WIDTH , SCREEN_HEIGHT ), DRAW_POLYGON_PRIORITY_NORMAL, MkF4(1.0f, 1.0f, 1.0f, 1.0f));*/

 
 //アニメーション
    Float2 Moveresultpaper;
    Float2 Moverankingpaper;
    
    if (anime_step>=STEP_MOVE_RESULT_PAPER)
    {
        Moveresultpaper = F2Lerp(0.0f, 0.0f, 0 + MOVE_RESULTPAPER_X, 0.0f+ MOVE_RESULTPAPER_Y, MOVE_RESULTTIME, MOVE_RESULT_EASING,MoveResultProgress);
    }
    else
    {
        
        Moveresultpaper = MkF2(0.0f, 0.0f);
    }

    if (anime_step == STEP_MOVE_RANKING_PAPER)
    {
       /* Moverankingpaper = MkF2(0.0f, 0.0f);*/
        Moverankingpaper = F2Lerpshare(MOVE_RANKING_START_X, MOVE_RANKING_START_Y, MOVE_RANKINGPAPER_X, MOVE_RANKINGPAPER_Y, MoveRankingProgress, EasingInSine);
        /*Moveresultpaper = F2Lerpshare(0.0f+MOVE_RESULTPAPER_X,  0.0f+MOVE_RESULTPAPER_Y, MOVE_RESULTPAPER_X+ MOVE_RESULTPAPER_X_RANK, MOVE_RESULTPAPER_Y+ MOVE_RESULTPAPER_Y_RANK, MoveResultProgress, EasingInSine);*/
    }
    else
    {
       
        Moverankingpaper = MkF2(0.0f, 0.0f);
    }

    if (!isranking) {
        //調査書文字
           /*SpriteResultDraw(g_tex_result_paper_text.textureId, MkF2(0.0f, 0.0f), MkF2(SCREEN_WIDTH * 1.2, SCREEN_HEIGHT * 1.2), DRAW_POLYGON_PRIORITY_NORMAL, MkF4(1.0f, 1.0f, 1.0f, 1.0f));*/
            //リザルトの紙
            SpriteResultDraw(g_tex_result_paper.textureId, Moveresultpaper, MkF2(SCREEN_WIDTH, SCREEN_HEIGHT), DRAW_POLYGON_PRIORITY_NORMAL, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
            SpriteResultDraw(g_tex_deffecult[(int)result.difficulty].textureId,MkF2(Moveresultpaper.x - 160.0f, Moveresultpaper.y - 210.0f), MkF2(283/2, 139/2), DRAW_POLYGON_PRIORITY_NORMAL, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
           /* result.players[0] = PLAYER_CHARACTER_TYPE_Kaito;
            result.players[2] = PLAYER_CHARACTER_TYPE_Contrius;
            result.players[1] = PLAYER_CHARACTER_TYPE_Neo;*/
          /*  chara_icon_draw((int)result.players, result.playersNum, Moveresultpaper.x - 160.0f, Moveresultpaper.y + 70.0f);*/
            //chara_icon_draw(result.players, result.playersNum, Moveresultpaper.x - 160.0f, Moveresultpaper.y + 70.0f);
            chara_icon_draw(result.players, result.playersNum, 
                Moveresultpaper.x - 170.0f, 
                Moveresultpaper.y + 90.0f);
            
            SpriteResultDraw(g_tex_teamlogo,
                MkF2(
                    Moveresultpaper.x + 150.0f,
                    Moveresultpaper.y + 185.0f
                ),
                MkF2(140.0f, 140.0f),
                DRAW_POLYGON_PRIORITY_NORMAL, 
                MkF4(1.0f, 1.0f, 1.0f, 0.5f));
        if (anime_step >= STEP_RANK)
        {
           //ランク
      /*   SS：30, 000~
            S：25, 000～29, 9
            A：15, 000 ~24, 999
            B：10, 000 ~14, 999
            C： 0~9,999*/
            Float2 sizetmp = F2Lerpshare(
                364 * 0.6,
                186 * 0.6,
                364 * 0.35,
                186 * 0.35,
                RankProgress, EasingInOutQuad);
            Float2 sizeZanzotmp = F2Lerpshare(
                364 * 0.5, 
                186 * 0.5, 
                364 * 1.0,
                186 * 1.0, 
                RankProgress, EasingOutQuint);

            SpriteResultDraw(g_tex_rank[result.rank].textureId,
                MkF2(Moveresultpaper.x + RANK_POSX, Moveresultpaper.y + RANK_POSY),
                sizetmp,
                DRAW_POLYGON_PRIORITY_NML_u2,
                MkF4(1.0f, 1.0f, 1.0f, FLerpshare(1.0, 1.5, RankProgress, EasingInOutQuad)));
            SpriteResultDraw(g_tex_rank[result.rank].textureId,
                MkF2(Moveresultpaper.x + RANK_POSX, Moveresultpaper.y + RANK_POSY),
                sizeZanzotmp,
                DRAW_POLYGON_PRIORITY_NML_u2,
                MkF4(1.0f, 1.0f, 1.0f, FLerpshare(1.0, 0.0, RankProgress, EasingOutQuint)));

                    //if (0<= total_score && total_score <10000)
                    //{//C 
                    //    SpriteResultDraw(g_tex_rank[0].textureId,
                    //        MkF2(Moveresultpaper.x +RANK_POSX, Moveresultpaper.y + 72.0f),
                    //        sizetmp,
                    //        DRAW_POLYGON_PRIORITY_NML_u2,
                    //        MkF4(1.0f, 1.0f, 1.0f, FLerpshare(1.0,1.5,RankProgress, EasingInOutQuad)));
                    //    SpriteResultDraw(g_tex_rank[0].textureId,
                    //        MkF2(Moveresultpaper.x + RANK_POSX, Moveresultpaper.y + 72.0f),
                    //        sizeZanzotmp,
                    //        DRAW_POLYGON_PRIORITY_NML_u2,
                    //        MkF4(1.0f, 1.0f, 1.0f, FLerpshare(1.0, 0.0, RankProgress, EasingOutQuint)));
                    //}
                    //else if (10000 <= total_score && total_score < 15000)
                    //{//B
                    //    SpriteResultDraw(g_tex_rank[1].textureId,
                    //        MkF2(Moveresultpaper.x+ RANK_POSX, Moveresultpaper.y + RANK_POSY),
                    //        F2Lerpshare(364 * 0.6, 186 * 0.6, 364 * 0.35, 186 * 0.35, RankProgress, EasingInOutQuad),
                    //        DRAW_POLYGON_PRIORITY_NML_u2,
                    //        MkF4(1.0f, 1.0f, 1.0f, FLerpshare(1.0, 1.5, RankProgress, EasingInOutQuad)));
                    //    SpriteResultDraw(g_tex_rank[1].textureId,
                    //        MkF2(Moveresultpaper.x + RANK_POSX, Moveresultpaper.y + 72.0f),
                    //        F2Lerpshare(364 * 0.5, 186 * 0.5, 364 * 1.0, 186 * 1.0, RankProgress, EasingOutQuint),
                    //        DRAW_POLYGON_PRIORITY_NML_u2,
                    //        MkF4(1.0f, 1.0f, 1.0f, FLerpshare(1.0, 0.0, RankProgress, EasingOutQuint)));
                    //}
                    //else if (15000 <= total_score && total_score < 25000)
                    //{//A
                    //    SpriteResultDraw(g_tex_rank[2].textureId,
                    //        MkF2(Moveresultpaper.x+ RANK_POSX, Moveresultpaper.y+ RANK_POSY),
                    //        F2Lerpshare(364 * 0.6, 186 * 0.6, 364 * 0.35, 186 * 0.35, RankProgress, EasingInOutQuad),
                    //        DRAW_POLYGON_PRIORITY_NML_u2,
                    //        MkF4(1.0f, 1.0f, 1.0f, FLerpshare(1.0, 1.5, RankProgress, EasingInOutQuad)));
                    //    SpriteResultDraw(g_tex_rank[2].textureId,
                    //        MkF2(Moveresultpaper.x + RANK_POSX, Moveresultpaper.y + 72.0f),
                    //        F2Lerpshare(364 * 0.5, 186 * 0.5, 364 * 1.0, 186 * 1.0, RankProgress, EasingOutQuint),
                    //        DRAW_POLYGON_PRIORITY_NML_u2,
                    //        MkF4(1.0f, 1.0f, 1.0f, FLerpshare(1.0, 0.0, RankProgress, EasingOutQuint)));
                    //}
                    //else if (25000 <= total_score && total_score < 30000)
                    //{//S
                    //    SpriteResultDraw(g_tex_rank[3].textureId,
                    //        MkF2(Moveresultpaper.x+ RANK_POSX, Moveresultpaper.y + RANK_POSY),
                    //        F2Lerpshare(364 * 0.6, 186 * 0.6, 364 * 0.35, 186 * 0.35, RankProgress, EasingInOutQuad),
                    //        DRAW_POLYGON_PRIORITY_NML_u2,
                    //        MkF4(1.0f, 1.0f, 1.0f, FLerpshare(1.0, 1.5, RankProgress, EasingInOutQuad)));
                    //    SpriteResultDraw(g_tex_rank[3].textureId,
                    //        MkF2(Moveresultpaper.x + RANK_POSX, Moveresultpaper.y + 72.0f),
                    //        F2Lerpshare(364 * 0.5, 186 * 0.5, 364 * 1.0, 186 * 1.0, RankProgress, EasingOutQuint),
                    //        DRAW_POLYGON_PRIORITY_NML_u2,
                    //        MkF4(1.0f, 1.0f, 1.0f, FLerpshare(1.0, 0.0, RankProgress, EasingOutQuint)));
                    //}
                    //else if (30000<=total_score)
                    //{//SS
                    //    SpriteResultDraw(g_tex_rank[4].textureId,
                    //        MkF2(Moveresultpaper.x+ RANK_POSX, Moveresultpaper.y + RANK_POSY),
                    //        F2Lerpshare(364 * 0.6, 186 * 0.6, 364 * 0.33, 186 * 0.33, RankProgress, EasingInOutQuad),
                    //        DRAW_POLYGON_PRIORITY_NML_u2,
                    //        MkF4(1.0f, 1.0f, 1.0f, FLerpshare(1.0, 1.5, RankProgress, EasingInOutQuad)));
                    //    SpriteResultDraw(g_tex_rank[4].textureId,
                    //        MkF2(Moveresultpaper.x + RANK_POSX, Moveresultpaper.y + 72.0f),
                    //        F2Lerpshare(364 * 0.5, 186 * 0.5, 364 * 1.0, 186 * 1.0, RankProgress, EasingOutQuint),
                    //        DRAW_POLYGON_PRIORITY_NML_u2,
                    //        MkF4(1.0f, 1.0f, 1.0f, FLerpshare(1.0, 0.0, RankProgress, EasingOutQuint)));
                    //}

                    if (result.isClear)
                    {
                        SPRITE_NUMBER_DRAW_EX sndex = {};
                        sndex.value = result.clearTimeFrame;
                        sndex.texNo = g_tex_Meter_time_01;
                        sndex.texCntX = 11;
                        sndex.texCntY = 1;

                        sndex.digit = 3;
                        //sndex.gap = -25;
                        //sndex.gap = -24.0f;
                        sndex.gap = -10.0f;
                        //sndex.gap += -6.0f;

                        sndex.plus_minus = false;
                        //sndex.color = MkF4(0.3f, 0.3f, 0.3f, 1.0f);
                        sndex.color =
                            MkF4(1.0f, 1.0f, 1.0f,
                                FLerpshare(0.0, 1.0, RankProgress * StampProgress, 
                                    EasingInOutQuad));
                        sndex.align = SPRITE_NUMBER_DRAW_EX_ALIGN::Center;

                        //sndex.size = MkF2(
                        //    45.0f * zoom,
                        //    45.0f * zoom
                        //);
                        sndex.size = MkF2(
                            30.0f * zoom,
                            30.0f * zoom
                        );
                        sndex.pos = MkF2(
                            //+SCREEN_WIDTH / 2 - 100.0f,
                            //-SCREEN_HEIGHT / 2 + 100.0f
                            //(Moveresultpaper.x + ((sndex.size.x - 0.0f) * (sndex.digit * 2 + (sndex.digit - 1))) / 8
                            (Moveresultpaper.x + ((sndex.size.x - 0.0f) * (sndex.digit * 2 + (sndex.digit - 1))) / 2 - 25.0f //+ 72.0f
                                + shakeOffset) * zoom,
                            (Moveresultpaper.y + 72.0f + 45.0f
                                ) * zoom
                        );
                        sndex.gap *= zoom;

                        //{
                        //    int shadownum = 8;
                        //    for (int i = 0; i < shadownum; i++)
                        //    {
                        //        SPRITE_NUMBER_DRAW_EX sndex2 = sndex;
                        //        //sndex2.color = MkF4(2.0f, 2.0f, 2.0f, 1.0f);
                        //        sndex2.color = MkF4(0.3f, 0.3f, 0.3f, 1.0f);
                        //        //sndex2.color.w *= enterMod1 * shadowalpha;
                        //        sndex2.pos.x += sin(2.0f * SYS_GENERAL_PI / shadownum * i) * sndex.size.x / 20.0f;
                        //        sndex2.pos.y += cos(2.0f * SYS_GENERAL_PI / shadownum * i) * sndex.size.y / 20.0f;
                        //        SpriteNumberTimerDraw(sndex2);
                        //    }
                        //}
                        SpriteNumberTimerDraw(sndex);
                    }
                    //SpriteResultDraw(g_tex_time_icon,
                    //    MkF2(Moveresultpaper.x + 5.0f,
                    //        Moveresultpaper.y + 72.0f + 45.0f
                    //    ),
                    //    MkF2(
                    //        40.0f,
                    //        40.0f
                    //    ),
                    //    DRAW_POLYGON_PRIORITY_NML_u2,
                    //    MkF4(1.0f, 1.0f, 1.0f, 
                    //        FLerpshare(1.0, 1.5, RankProgress, EasingInOutQuad))
                    //);
                    

        } 
        if (anime_step>= STEP_STAMP)
        {
            unsigned int tex = g_tex_result_clear_stamp.textureId;

            //Float2 posTmp = MkF2(
            //    Moveresultpaper.x + 0.0f,
            //    Moveresultpaper.y + 175.0f);
            //float rottmp = .0f;
            Float2 sizeTmp = F2Lerpshare(
                578 * 1.2, 205 * 1.2,
                578 * 0.3, 205 * 0.3,
                StampProgress, EasingInOutQuad);
            Float2 size2Tmp = F2Lerpshare(
                578 * 0.3, 205 * 0.3,
                578 * 1.2, 205 * 1.2,
                StampProgress2, EasingOutQuint);
            Float2 posTmp = MkF2(
                Moveresultpaper.x + -110.0f,
                Moveresultpaper.y + 175.0f);
            float rottmp = 4.0f;

            //スタンプの種類を設定
            //2025/03/09 Hara
            if (!result.isClear)
            {
                tex = g_tex_result_failure_stamp.textureId;


                //SpriteResultDraw(tex, MkF2(Moveresultpaper.x + 0.0f, Moveresultpaper.y + 165.0f), F2Lerpshare(578 * 1.2, 205 * 1.2, 578 * 0.45, 205 * 0.45, StampProgress, EasingInOutQuad), DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, FLerpshare(0.0f, 1.0f, StampProgress, EasingInOutQuad)));
                SpriteResultDraw(tex,
                    posTmp,
                    sizeTmp,
                    DRAW_POLYGON_PRIORITY_NML_u2,
                    MkF4(1.0f, 1.0f, 1.0f, FLerpshare(0.0f, 1.0f, StampProgress, EasingInOutQuad)),
                    rottmp
                );
            }
            else
            {

                //(tex, MkF2(Moveresultpaper.x + 0.0f, Moveresultpaper.y + 165.0f), F2Lerpshare(578 * 1.2, 205 * 1.2, 578 * 0.45, 205 * 0.45, StampProgress, EasingInOutQuad), DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, FLerpshare(0.0f, 1.0f, StampProgress, EasingInOutQuad)));

                //if (StampProgress >= 1.0f)
                //    SpriteResultDraw(tex, MkF2(Moveresultpaper.x + 0.0f, Moveresultpaper.y + 165.0f), F2Lerpshare(578 * 0.45, 205 * 0.45, 578 * 1.2, 205 * 1.2, StampProgress2, EasingOutQuint), DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, FLerpshare(1.0f, 0.0f, StampProgress2, EasingOutQuint)));


                SpriteResultDraw(tex,
                    posTmp,
                    sizeTmp,
                    DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, FLerpshare(0.0f, 1.0f, StampProgress, EasingInOutQuad)),
                    rottmp
                );

                if (StampProgress >= 1.0f)
                    SpriteResultDraw(tex,
                        posTmp,
                        size2Tmp, DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, FLerpshare(1.0f, 0.0f, StampProgress2, EasingOutQuint)),
                        rottmp
                    );

            }
        }

        int bonuses[6];
        bonuses[0] = clear_bonus;
        bonuses[1] = enemy_bonus;
        bonuses[2] = randomevent_bonus;
        bonuses[3] = no_damage_bonus;
        bonuses[4] = cler_time_bonus;
        bonuses[5] = total_score;
        Float2 movescore;
        Float2 movetotalscore;
       
        //スコアボーナスの表示
        if (current_step == STEP_NORMAL_SCORE)//スコアアニーションの始めの表示
        {
            for (int i = 0; i < 6; i++) {
                //if (anime_step >= STEP_MOVE_RESULT_PAPER)
                //{
                //    /*movetotalscore = MoveSprite(SCORE_TOTAL_POSX, SCORE_TOTAL_POSY, SCORE_TOTAL_POSX + MOVE_RESULTPAPER, SCORE_TOTAL_POSY, MOVE_RESULTTIME, EasingInSine);
                //    movescore = MoveSprite(SCORE_POS_X, SCORE_POS_Y + SCORE_DISTANCE * i, SCORE_POS_X + MOVE_RESULTPAPER, SCORE_POS_Y + SCORE_DISTANCE * i, MOVE_RESULTTIME, EasingInSine);*/
                //    MkF2(Moveresultpaper.x - 120.0f, Moveresultpaper.y + 240.0f),
                //}
                //else
                //{
                //    /*movetotalscore = MkF2(SCORE_TOTAL_POSX, SCORE_TOTAL_POSY);
                //    movescore = MkF2(SCORE_POS_X, SCORE_POS_Y + SCORE_DISTANCE * i);*/
            
                    movetotalscore = MkF2(Moveresultpaper.x + SCORE_TOTAL_POSX, Moveresultpaper.y+ SCORE_TOTAL_POSY),
                    movescore = MkF2(Moveresultpaper.x + SCORE_POS_X, Moveresultpaper.y + SCORE_POS_Y + SCORE_DISTANCE * i);
                

                if (i == 5)
                {
                   
                    /*SpriteResultNumberGapDraw(0, movetotalscore, MkF2(SCORE_TOTAL_SIZE, SCORE_TOTAL_SIZE), SCORE_TOTAL_GAP,true);*/
                   
                }
                else 
                {
                   
                    SpriteResultNumberGapDraw(0, movescore, MkF2(SCORE_SIZE, SCORE_SIZE), SCORE_GAP);
                }
            }
        }
        else
        {//スコアアニーション中

            for (int i = 0; i < 6; i++)
            {
                movetotalscore = MkF2(Moveresultpaper.x + SCORE_TOTAL_POSX, Moveresultpaper.y + SCORE_TOTAL_POSY),
                movescore = MkF2(Moveresultpaper.x + SCORE_POS_X, Moveresultpaper.y + SCORE_POS_Y + SCORE_DISTANCE * i);
                if (i < current_step - 1)
                {//スコア表示後
                    if (i == 5)
                    {
                        
                        SpriteResultNumberGapDraw(bonuses[i], movetotalscore, MkF2(SCORE_TOTAL_SIZE, SCORE_TOTAL_SIZE), SCORE_TOTAL_GAP,true);

                    }
                    else SpriteResultNumberGapDraw(bonuses[i], movescore, MkF2(SCORE_SIZE, SCORE_SIZE), SCORE_GAP);
                   
                }
                else if (i == current_step - 1)
                {//スコア表示中
                    if (i == 5)
                    {
                        SpriteResultNumberGapDraw(random_score, movetotalscore, MkF2(SCORE_TOTAL_SIZE * SCORE_TOTAL_ZOOM, SCORE_TOTAL_SIZE * SCORE_TOTAL_ZOOM), SCORE_TOTAL_GAP,true);

                    }
                    else SpriteResultNumberGapDraw(random_score, movescore, MkF2(SCORE_SIZE * SCORE_ZOOM, SCORE_SIZE * SCORE_ZOOM), SCORE_GAP);//表示中

                }
                else
                {//スコア表示前
                    if (i == 5)
                    {
                       /* SpriteResultNumberGapDraw(0, movetotalscore, MkF2(SCORE_TOTAL_SIZE, SCORE_TOTAL_SIZE), SCORE_TOTAL_GAP,true);*/

                    }
                    else SpriteResultNumberGapDraw(0,  movescore, MkF2(SCORE_SIZE, SCORE_SIZE), SCORE_GAP);
                }

            }
        }
    }
    
    //ランキング時
    if (isranking)
    {
        //ランキングの紙
        SpriteResultDraw(g_tex_result_ranking_paper.textureId, Moverankingpaper, MkF2(SCREEN_WIDTH , SCREEN_HEIGHT ), DRAW_POLYGON_PRIORITY_NORMAL, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
        //難易度の紙
        int index = disp_ranking_normal ? 0 : 1; // trueなら0, falseなら1
        SpriteResultDraw(g_tex_deffecult[index].textureId,
            MkF2(Moverankingpaper.x - 300.0f, Moverankingpaper.y - 210.0f),
            MkF2(283 / 2, 139 / 2),
            DRAW_POLYGON_PRIORITY_NORMAL,
            MkF4(1.0f, 1.0f, 1.0f, 1.0f));


        SpriteResultDraw(g_tex_arrow.textureId, MkF2(Moverankingpaper.x +150.0f,  Moverankingpaper.y-50), MkF2(39*1.5, 26*1.5), DRAW_POLYGON_PRIORITY_NORMAL, MkF4(1.0f, 1.0f, 1.0f, 1.0f),90.0f);
        SpriteResultDraw(g_tex_arrow.textureId, MkF2(Moverankingpaper.x - 430.0f, Moverankingpaper.y-50), MkF2(39 * 1.5, 26 * 1.5), DRAW_POLYGON_PRIORITY_NORMAL, MkF4(1.0f, 1.0f, 1.0f, 1.0f),-90.0f);

        int scoresIndex = index + disp_ranking_time * 2;

        int loopn = std::min(
            (int)TOP_N, 
            (int)scores.scores[scoresIndex].rank.size()
        );

        //ランキングスコア
        for (int i = 0; i < loopn; i++)
        {
            //int scoresIndex = index;
            if (scores.scores[scoresIndex].rank.size() <= i)
            {
                continue;
            }

            if(scores.scores[scoresIndex].type == RESULT_FILE_TYPE_TIME)
            {
                SPRITE_NUMBER_DRAW_EX sndex = {};
                sndex.value = scores.scores[scoresIndex].rank[i].time;
                sndex.texNo = g_tex_Meter_time_01;
                sndex.texCntX = 11;
                sndex.texCntY = 1;

                sndex.digit = 3;
                sndex.gap = -12.0f;

                sndex.plus_minus = false;
                sndex.color =
                    MkF4(1.0f, 1.0f, 1.0f,
                        FLerpshare(0.0, 1.0, RankProgress * StampProgress,
                            EasingInOutQuad));
                sndex.align = SPRITE_NUMBER_DRAW_EX_ALIGN::Center;

                sndex.size = MkF2(
                    30.0f * zoom,
                    30.0f * zoom
                );

                if (i == 0) {
                    sndex.gap    *= 1.2f;
                    sndex.size.x *= 1.3f;
                    sndex.size.y *= 1.3f;
                    sndex.pos = MkF2(
                        (Moverankingpaper.x + 65.0f
                            + shakeOffset) * zoom,
                        (Moverankingpaper.y - 79 + ((i - 1) * 48)
                            ) * zoom
                    );
                }
                else if (i == 1) {
                    sndex.gap    *= 1.1f;
                    sndex.size.x *= 1.1f;
                    sndex.size.y *= 1.1f;
                    sndex.pos = MkF2(
                        (Moverankingpaper.x + 58.0f
                            + shakeOffset) * zoom,
                        (Moverankingpaper.y - 51 + ((i - 1) * 48)
                            ) * zoom
                    );
                }
                else
                {
                    sndex.pos = MkF2(
                        (Moverankingpaper.x + 50.0f
                            + shakeOffset) * zoom,
                        (Moverankingpaper.y - 50 + ((i - 1) * 48)
                            ) * zoom
                    );
                }

                sndex.gap *= zoom;
                SpriteNumberTimerDraw(sndex);
            }
            else
            {
                if (i == 0) 
                {
                    SpriteResultNumberGapDraw(
                        scores.scores[scoresIndex].rank[i].score,
                        MkF2(Moverankingpaper.x + 15, Moverankingpaper.y - 130), MkF2(60, 60), -15);
                    
                }
                else if (i == 1)
                {
                    SpriteResultNumberGapDraw(
                        scores.scores[scoresIndex].rank[i].score,
                        MkF2(Moverankingpaper.x + 10, Moverankingpaper.y - 55), MkF2(45, 45), -12);
                    
                }
                else
                {
                    SpriteResultNumberGapDraw(
                        scores.scores[scoresIndex].rank[i].score,
                        MkF2(Moverankingpaper.x, Moverankingpaper.y - 50 + ((i - 1) * 48)), MkF2(35, 35), -8);
                    
                }
            }

            {
                TEX_DAT charatex = g_tex_chara[(int)scores.scores[scoresIndex].rank[i].chara[0]];
                float heightmod =
                    (float)charatex.height
                    / charatex.width
                    ;
                if (i == 0)
                {
                    SpriteResultDraw(
                        charatex.textureId,
                        MkF2(Moverankingpaper.x - 180, Moverankingpaper.y - 120), MkF2(56.0f, 56.0f * heightmod), DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));

                }
                else if (i == 1)
                {
                    SpriteResultDraw(
                        charatex.textureId,
                        MkF2(Moverankingpaper.x - 150, Moverankingpaper.y - 55), MkF2(56.0f, 56.0f * heightmod), DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));

                }
                else
                {
                    SpriteResultDraw(
                        charatex.textureId,
                        MkF2(Moverankingpaper.x - 150, Moverankingpaper.y - 50 + ((i - 1) * 48)), MkF2(56.0f, 56.0f * heightmod), DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));

                }
            }
        }

        if (scores.scores[scoresIndex].type == RESULT_FILE_TYPE_TIME)
        {
            if (result.isClear)
            {
                SPRITE_NUMBER_DRAW_EX sndex = {};
                sndex.value = result.clearTimeFrame;
                sndex.texNo = g_tex_Meter_time_01;
                sndex.texCntX = 11;
                sndex.texCntY = 1;

                sndex.digit = 3;
                sndex.gap = -12.0f;

                sndex.plus_minus = false;
                sndex.color =
                    MkF4(1.0f, 1.0f, 1.0f,
                        FLerpshare(0.0, 1.0, RankProgress * StampProgress,
                            EasingInOutQuad));
                sndex.align = SPRITE_NUMBER_DRAW_EX_ALIGN::Center;

                sndex.size = MkF2(
                    40.0f * zoom,
                    40.0f * zoom
                );
                sndex.pos = MkF2(
                    (Moverankingpaper.x + 70.0f
                        + shakeOffset) * zoom,
                    (Moverankingpaper.y + 170
                        ) * zoom
                );
                sndex.gap *= zoom;
                SpriteNumberTimerDraw(sndex);
            }
        }
        else
        {
            SpriteResultNumberGapDraw(total_score,
                MkF2(Moverankingpaper.x, Moverankingpaper.y + 170),
                MkF2(50, 50),
                -10.0f,//1,
                true);
        }
    }
 
    Float2 movemenu;
    //セレクト紙
    if (anime_step >= STEP_MOVE_RESULT_PAPER)
    {
        
        movemenu = F2Lerp(SELECT_START_X, SELECT_START_Y, 0.0f, 0.0f, MOVE_RESULTTIME, MOVE_RESULT_EASING);
    }
    else
    {
        movemenu = MkF2(SELECT_START_X, SELECT_START_Y);
    }
        SpriteResultDraw(g_tex_result_select_paper.textureId, MkF2(movemenu.x-130,movemenu.y), MkF2(SCREEN_WIDTH, SCREEN_HEIGHT), DRAW_POLYGON_PRIORITY_NML_d2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
        
       /* DRAW_POLYGON_DAT drawDatTmp = {};
        drawDatTmp.pos = MkF3(0.0f, -30.0f, 0.0f);*/
        //メニュー
        for (int i = 0; i < MENU_UI_MAX; i++) {

            if (i == menu_num) {
                SpriteResultDraw(g_tex_menu[i+2].textureId,MkF2(movemenu.x,  movemenu.y),  MkF2(SCREEN_WIDTH,  SCREEN_HEIGHT), DRAW_POLYGON_PRIORITY_NML_d2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));

            }
            
            else
            {
                SpriteResultDraw(g_tex_menu[i].textureId, movemenu, MkF2(SCREEN_WIDTH , SCREEN_HEIGHT ), DRAW_POLYGON_PRIORITY_NML_d2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));

            }
        }
    
    
}

// ===================================================
// シーンの終了処理
// ===================================================
void FinalizeSceneResult(void)
{
    //BGMを止める
    SoundBGMStopRequest();
	//SoundBGMStopRequest(SOUND_TYPE_BGM_3);
    //SoundBGMStopRequest(SOUND_TYPE_BGM_4);
	////タイトル用のBGMを流す
	//SoundBGMPlayRequest(SOUND_TYPE_BGM_1, (60 * 60 * 1) + (60 * 42), 0.5);

    DXSetDepthStencilStateDepth(DXDepthStencilStateDepthEnable());

    SetMainGameDrawFPS(1);
}
// ===================================================
// ローカル関数
// ===================================================

//リザルト用のスプライトドロー
void SpriteResultDraw(int textureid, Float2 pos, Float2 size, DRAW_POLYGON_PRIORITY priority, Float4 color,float rot)
{
    DRAW_POLYGON_DAT drawDatTmp = {};
    drawDatTmp.pos = MkF3(
        (pos.x+shakeOffset) * zoom,
        pos.y * zoom /** 0.3f*/,
        0.0f
    );
   
    drawDatTmp.size = MkF3(size.x*zoom, size.y*zoom, 1.0f);

    drawDatTmp.rot = rot;
    drawDatTmp.rot3D = MkF3(0.0f, 0.0f, 0.0f);

    drawDatTmp.color = color;

    drawDatTmp.texNo = textureid;

    drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

    drawDatTmp.texUV = { 1.0f, 1.0f };
    drawDatTmp.texWH = { 1.0f, 1.0f };
    drawDatTmp.startScale = MkF2(1.0f, 1.0f);
    drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

    drawDatTmp.modelNo = 0;


    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
    opt.reverse = false;
    opt.fixed = true;
    opt.enableShade = true;
    opt.billboard = false;
    opt.depthBuffIgnore = true;
   /* opt.cameraFix = true;*/
    //opt.priority = DRAW_POLYGON_PRIORITY_DEEPEST;
    opt.priority = priority;
    DrawPolygonInCameraEx(drawDatTmp, opt);

}
void SpriteResultNumberGapDraw(int v, Float2 pos, Float2 size, float gap,bool texon)
{
    SPRITE_NUMBER_DRAW_EX sndex = {};
    sndex.value = v;

    if (texon)
    {
        sndex.texNo = LoadTexData("TEX/ui/in_game_master/score_up_sprite").textureId;
        sndex.texCntX = 11;
    }
    else
    {
        sndex.texNo = LoadTexData("TEX/ui/result/score_result").textureId;
        sndex.texCntX = 11;
    }
    sndex.texCntY = 1;
    sndex.pos= MkF2((pos.x - size.x+shakeOffset)*zoom,  pos.y * zoom);
    sndex.size = MkF2(size.x * zoom, size.y * zoom);
    sndex.digit = 5;
    sndex.gap = gap*zoom;
    sndex.plus_minus = false;
    sndex.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
    sndex.align = SPRITE_NUMBER_DRAW_EX_ALIGN::Center;
    SpriteNumberDrawEx(sndex);
}


// スコアをファイルから読み込む
void LoadScores(SCORE_SAVE_DAT_LIST& scoresL) 
{

    //if (dif==true)
    //{//ノーマル
    //    FILE* file = fopen(FILE_NAME_N, "r");
    //    if (file == NULL) {
    //        return 0; // ファイルがない場合はスコア0
    //    }

    //    int count = 0;
    //    while (count < max_scores && fscanf(file, "%d", &scores[count]) == 1) {
    //        count++;
    //    }

    //    fclose(file);
    //    return count;
    //}
    //else if (dif == false)
    //{//ハード
    //    FILE* file = fopen(FILE_NAME_H, "r");
    //    if (file == NULL) {
    //        return 0; // ファイルがない場合はスコア0
    //    }

    //    int count = 0;
    //    while (count < max_scores && fscanf(file, "%d", &scores[count]) == 1) {
    //        count++;
    //    }

    //    fclose(file);
    //    return count;
    //}

    for (int i = 0; i < SCORE_SAVE_DAT_LIST_NUM; i++)
    {   
        // ファイル名の指定
        std::string filename = RESULT_FILES[i].name;

        // テキストファイルを開く
        std::ifstream file(filename);
        if (!file.is_open()) {
            return;
        }

        scoresL.scores[i].type = RESULT_FILES[i].type;

        // vectorにデータを格納
        std::vector<SCORE_SAVE_DAT>& data = scoresL.scores[i].rank;
        data.clear();
        data.shrink_to_fit();

        std::string value;
        while (file >> value)
        {
            STR_SPRIT strsplit = StrSplit(value, '_', true);

            SCORE_SAVE_DAT valdat = {};
            if (strsplit.size() >= 1)
            {
                valdat.score = std::atoi(strsplit[0].c_str());
            }

            if (strsplit.size() >= 2)
            {
                valdat.time = std::atoi(strsplit[1].c_str());
            }

            if (strsplit.size() >= 3)
            {
                //valdat.chara = std::atoi(strsplit[2].c_str());
                {
                    STR_SPRIT strsplit2 = StrSplit(strsplit[2], '~', true);
                    if (strsplit2.size() >= SYS_PLAYER_MAX + 1)
                    {
                        valdat.charaNum = std::atoi(strsplit2[0].c_str());
                        for (int j = 0; j < SYS_PLAYER_MAX; j++) {
                            valdat.chara[j] =
                                (PLAYER_CHARACTER_TYPE)std::atoi(
                                    strsplit2[1 + j].c_str());
                        }
                    }
                }
            }

            data.push_back(valdat);
        }

        // ファイルを閉じる
        file.close();
    }
}

// スコアをファイルに保存する
void SaveScores(SCORE_SAVE_DAT_LIST& scoresL) 
{

    //if (result.difficulty == Difficulty::Normal)
    //{
    //    FILE* file = fopen(FILE_NAME_N, "w");
    //    if (file == NULL) {

    //        return;
    //    }

    //    for (int i = 0; i < count; i++) {
    //        fprintf(file, "%d\n", scores[i]);
    //    }

    //    fclose(file);
    //}

    //if (result.difficulty == Difficulty::Hard)
    //{

    //    FILE* file = fopen(FILE_NAME_H, "w");
    //    if (file == NULL) {

    //        return;
    //    }

    //    for (int i = 0; i < count; i++) {
    //        fprintf(file, "%d\n", scores[i]);
    //    }

    //    fclose(file);

    //}

    for (int i = 0; i < SCORE_SAVE_DAT_LIST_NUM; i++)
    {
        // ファイル名の指定
        std::string filename = RESULT_FILES[i].name;

        // テキストファイルを開く (書き込みモード)
        std::ofstream file(filename);
        if (!file.is_open()) {
            return;
        }

        scoresL.scores[i].type = RESULT_FILES[i].type;

        std::vector<SCORE_SAVE_DAT>& data = scoresL.scores[i].rank;

        // データをファイルに書き込む
        for (const auto& value : data) 
        {
            std::string charadat = "";
            charadat += std::to_string(value.charaNum);
            for (int j = 0; j < SYS_PLAYER_MAX; j++) {
                charadat += "~" + std::to_string(value.chara[j]);
            }

            std::string valdat =
                std::to_string(value.score)
                + "_" + std::to_string(value.time)
                + "_" + charadat
                ;

            file << valdat << std::endl;
        }

        // ファイルを閉じる
        file.close();
    }
}

// 降順ソート（バブルソートを使用）
//void SortScores(int scores[], int count) {
//    for (int i = 0; i < count; ++i) {
//        for (int j = i+1; j < count; ++j) {
//            if (scores[i] < scores[j]) {
//                int temp = scores[i];
//                scores[i] = scores[j];
//                scores[j] = temp;
//            }
//        }
//    }
//}
void SortScores(SCORE_SAVE_DAT_LIST& scoresL) {
    //std::sort(
    //    scores.begin(),
    //    scores.end(),
    //    [](const SCORE_SAVE_DAT& a, const SCORE_SAVE_DAT& b) {
    //        return a.score < b.score;
    //    }
    //);

    for (int i = 0; i < SCORE_SAVE_DAT_LIST_NUM; i++)
    {
        std::vector<SCORE_SAVE_DAT> ssd = scoresL.scores[i].rank;
        if (scoresL.scores[i].type == RESULT_FILE_TYPE_TIME)
        {
            if (ssd.size() >= 2)
            {
                std::sort(
                    ssd.begin(),
                    ssd.end(),
                    [](const SCORE_SAVE_DAT& a, const SCORE_SAVE_DAT& b) {
                        return a.time < b.time;
                    }
                );
            }
        }
        else
        {
            if (ssd.size() >= 2)
            {
                std::sort(
                    ssd.begin(),
                    ssd.end(),
                    [](const SCORE_SAVE_DAT& a, const SCORE_SAVE_DAT& b) {
                        return a.score > b.score;
                    }
                );
            }
        }

        scoresL.scores[i].rank.clear();
        scoresL.scores[i].rank.shrink_to_fit();
        for (int j = 0; j < std::min((int)TOP_N, (int)ssd.size()); j++)
        {
            scoresL.scores[i].rank.push_back(ssd[j]);
        }
    }
}

void Result_Score_Anime(int& random_score, int score, int time, int& elapsed_time, ScoreStep next)
{
    if (elapsed_time < time)
    {
        int plusnumber = score / time;
        random_score += plusnumber;
        elapsed_time++;

        if (elapsed_time >= time)
        {
            is_Shake = true;
            random_score = score;  // 最終スコアに到経つ
            elapsed_time = 0;
            random_score = 0; // 次のスコア処理へリセット
            current_step = next;
            
        }
    }
}
//void camera_zoom(float multiptr, float time, float deltaTime)
//{
//    static float t = 0.0f;
//
//    // 時間経過に基づいて `t` を更新
//    t += deltaTime / time;
//    if (t > 1.0f) t = 1.0f;  // `t` が 1 を超えないように制限
//
//    // EasingOutBounce を適用してズーム値を更新
//    float easedT = EasingOutBounce(t);
//    zoom = 1.0f * (1 - easedT) + multiptr * easedT;
//}

bool camera_zoom(float targetZoom, float duration, float deltaTime)
{
    //zoomProgress = 0.0f; // 進行度（0.0 ～ 1.0）
    static bool isZooming = true;     // ズーム中かどうか

    //if (!isZooming) return true; // ズーム完了済みなら何もしない

    // イージング計算
    zoomProgress += deltaTime / duration;
    if (zoomProgress > 1.0f) zoomProgress = 1.0f;

    float t = EasingInOutSine(zoomProgress);
    zoom = 1.0f * (1.0f - t) + targetZoom * t;

    // ズーム完了チェック
    if (zoomProgress >= 1.0f)
    {
        isZooming = false; // ズーム完了
        return true;       // 完了フラグを返す
    }
    return false; // まだズーム中
}

static Float2 F2Lerp(float x, float y, float aftermoveingx, float aftermoveingy,
    float movetime, std::function<float(float)> easing, float& progress)
{
    // 進行度を更新
    progress += 1.0f / movetime;
    if (progress > 1.0f) progress = 1.0f;

    float t = easing(progress);
    Float2 MovedPos = MkF2(x * (1.0f - t) + aftermoveingx * t,y * (1.0f - t) + aftermoveingy * t);

    // 移動完了時は目的地の座標を返す
    if (progress >= 1.0f)
    {
        progress = 1.0f; // 進行度を固定
        return MkF2(aftermoveingx, aftermoveingy);
    }

    return MovedPos; // まだ移動中
}
static float FLerp(float start, float end, float movetime, std::function<float(float)> easing, float& progress)
{
    // 進行度を更新
    progress += 1.0f / movetime;
    if (progress > 1.0f) progress = 1.0f;

    // イージング適用
    float t = easing(progress);

    // 補間計算
    float movedValue = start * (1.0f - t) + end * t;

    // 完了チェック
    if (progress >= 1.0f)
    {
        progress = 1.0f;
        return end;  // 最終的にendにピッタリ合わせる
    }

    return movedValue;  // 途中経過を返す
}


void chara_icon_draw(PLAYER_CHARACTER_TYPE playerschara[SYS_PLAYER_MAX],int playernum, float posx, float posy)
{
    /*int chara;*/
   
    int playercount = 0;
    
    //一人
    if (playernum <=1) 
    {
        //for (int i = 0; i < 4; i++)
        for (int i = 0; i < SYS_PLAYER_MAX; i++)
        {
            float heightmod =
                (float)g_tex_chara[playerschara[i]].height
                / g_tex_chara[playerschara[i]].width
                ;
            if (playerschara[i] != PLAYER_CHARACTER_TYPE_None)
            {
                if (playerschara[i] == PLAYER_CHARACTER_TYPE_Neo) {
                    SpriteResultDraw(
                        g_tex_chara[playerschara[i]].textureId,
                        MkF2(posx, posy),
                        MkF2(85.0f * 1.1, 85.0f * 1.1 * heightmod),
                        DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
                }
                else
                {
                    SpriteResultDraw(
                        g_tex_chara[playerschara[i]].textureId,
                        MkF2(posx, posy),
                        MkF2(85.0f, 85.0f * heightmod),
                        DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));

                }
                
            }
           
        }

    }  
    else if (playernum == 2)
    {
        //for (int i = 0; i < 4; i++)
        for (int i = 0; i < SYS_PLAYER_MAX; i++)
        {
            float heightmod =
                (float)g_tex_chara[playerschara[i]].height
                / g_tex_chara[playerschara[i]].width
                ;
            if (playerschara[i] != PLAYER_CHARACTER_TYPE_None)
            {
                playercount++;
                if (playercount == 1) {
                    if (playerschara[i] == PLAYER_CHARACTER_TYPE_Neo) {
                        SpriteResultDraw(
                            g_tex_chara[playerschara[i]].textureId,
                            MkF2(posx, posy - 20.0f),
                            MkF2(57.0f * 1.1, 57.0f * 1.1 * heightmod),
                            DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
                    }
                    else
                    {
                        SpriteResultDraw(
                            g_tex_chara[playerschara[i]].textureId,
                            MkF2(posx, posy - 20.0f),
                            MkF2(57.0f, 57.0f * heightmod),
                            DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
                    }
                }
                else if (playercount == 2)
                {
                    ;

                    if (playerschara[i] == PLAYER_CHARACTER_TYPE_Neo) {
                        SpriteResultDraw(
                            g_tex_chara[playerschara[i]].textureId,
                            MkF2(posx, posy + 20.0f),
                            MkF2(57.0f * 1.1, 57.0f * 1.1 * heightmod),
                            DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
                    }
                    else
                    {
                        SpriteResultDraw(
                            g_tex_chara[playerschara[i]].textureId,
                            MkF2(posx, posy + 20.0f),
                            MkF2(57.0f, 57.0f * heightmod),
                            DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
                    }
                }

            }
        }
    }
    else if (playernum == 3)
    {
        //for (int i = 0; i < 4; i++)
        for (int i = 0; i < SYS_PLAYER_MAX; i++)
        {
            float heightmod =
                (float)g_tex_chara[playerschara[i]].height
                / g_tex_chara[playerschara[i]].width
                ;
            if (playerschara[i] != PLAYER_CHARACTER_TYPE_None)
            {
                playercount++;
                if (playercount == 1) {
                    if (playerschara[i] == PLAYER_CHARACTER_TYPE_Neo) {
                        SpriteResultDraw(
                            g_tex_chara[playerschara[i]].textureId, 
                            MkF2(posx + 2.0f, posy - 15),
                            MkF2(57.0f * 1.1, 57.0f * 1.1 * heightmod),
                            DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
                    }
                    else
                    {
                        SpriteResultDraw(
                            g_tex_chara[playerschara[i]].textureId,
                            MkF2(posx + 2.0f, posy - 15),
                            MkF2(57.0f, 57.0f * heightmod),
                            DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
                    }
                }
                else if (playercount == 2)
                {
                    ;

                    if (playerschara[i] == PLAYER_CHARACTER_TYPE_Neo) {
                        SpriteResultDraw(
                            g_tex_chara[playerschara[i]].textureId,
                            MkF2(posx + 24.0f, posy + 23.0f),
                            MkF2(57.0f * 1.1, 57.0f * 1.1 * heightmod),
                            DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
                    }
                    else
                    {
                        SpriteResultDraw(
                            g_tex_chara[playerschara[i]].textureId, 
                            MkF2(posx + 24.0f, posy + 20.0f),
                            MkF2(57.0f, 57.0f * heightmod),
                            DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
                    }
                }
                else if (playercount == 3)
                {
                    if (playerschara[i] == PLAYER_CHARACTER_TYPE_Neo) {
                        SpriteResultDraw(
                            g_tex_chara[playerschara[i]].textureId,
                            MkF2(posx - 21.0f, posy + 23.0f),
                            MkF2(57.0f * 1.1, 57.0f * 1.1 * heightmod),
                            DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
                    }
                    else
                    {
                        SpriteResultDraw(
                            g_tex_chara[playerschara[i]].textureId,
                            MkF2(posx - 21.0f, posy + 20.0f),
                            MkF2(57.0f, 57.0f * heightmod),
                            DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
                    }
                }


            }
        }
    }
    else if (playernum == 4)
    {
        //for (int i = 0; i < 4; i++)
        for (int i = 0; i < SYS_PLAYER_MAX; i++)
        {
            float heightmod =
                (float)g_tex_chara[playerschara[i]].height
                / g_tex_chara[playerschara[i]].width
                ;
            if (playerschara[i] != PLAYER_CHARACTER_TYPE_None)
            {
                playercount++;
                if (playercount == 1) {
                    if (playerschara[i] == PLAYER_CHARACTER_TYPE_Neo) {
                        SpriteResultDraw(
                            g_tex_chara[playerschara[i]].textureId,
                            MkF2(posx + 2.0f, posy - 15), 
                            MkF2(57.0f * 1.1, 57.0f * 1.1 * heightmod),
                            DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
                    }
                    else
                    {
                        SpriteResultDraw(
                            g_tex_chara[playerschara[i]].textureId,
                            MkF2(posx + 2.0f, posy - 15), 
                            MkF2(57.0f, 57.0f * heightmod),
                            DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
                    }
                }
                else if (playercount == 2)
                {
                    ;

                    if (playerschara[i] == PLAYER_CHARACTER_TYPE_Neo) {
                        SpriteResultDraw(
                            g_tex_chara[playerschara[i]].textureId,
                            MkF2(posx + 24.0f, posy + 20.0f),
                            MkF2(57.0f * 1.1, 57.0f * 1.1 * heightmod),
                            DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
                    }
                    else
                    {
                        SpriteResultDraw(
                            g_tex_chara[playerschara[i]].textureId,
                            MkF2(posx + 24.0f, posy + 20.0f),
                            MkF2(57.0f, 57.0f * heightmod),
                            DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
                    }
                }
                else if (playercount == 3)
                {
                    if (playerschara[i] == PLAYER_CHARACTER_TYPE_Neo) {
                        SpriteResultDraw(
                            g_tex_chara[playerschara[i]].textureId,
                            MkF2(posx - 21.0f, posy + 20.0f),
                            MkF2(57.0f * 1.1, 57.0f * 1.1 * heightmod),
                            DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
                    }
                    else
                    {
                        SpriteResultDraw(
                            g_tex_chara[playerschara[i]].textureId,
                            MkF2(posx - 21.0f, posy + 20.0f),
                            MkF2(57.0f, 57.0f * heightmod),
                            DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
                    }
                }
                else if (playercount == 4)
                {
                    if (playerschara[i] == PLAYER_CHARACTER_TYPE_Neo) {
                        SpriteResultDraw(
                            g_tex_chara[playerschara[i]].textureId,
                            MkF2(posx - 21.0f, posy + 20.0f),
                            MkF2(57.0f * 1.1, 57.0f * 1.1 * heightmod),
                            DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
                    }
                    else
                    {
                        SpriteResultDraw(
                            g_tex_chara[playerschara[i]].textureId,
                            MkF2(posx - 21.0f, posy + 20.0f),
                            MkF2(57.0f, 57.0f * heightmod),
                            DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
                    }
                }

            }
        }
    }
    //else if (playernum == 3)
    //{
    //    //三人
    //    SpriteResultDraw(g_tex_chara[chara].textureId, MkF2(posx + 2.0f, posy - 15), MkF2(169 / 3, 160 / 3), DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
    //    SpriteResultDraw(g_tex_chara[chara].textureId, MkF2(posx + 24.0f, posy + 20.0f), MkF2(169 / 3, 160 / 3), DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
    //    SpriteResultDraw(g_tex_chara[chara].textureId, MkF2(posx - 21.0f, posy + 20.0f), MkF2((169 / 3) * 1.1, (160 / 3) * 1.1), DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
    //}
    //else
    //{
    //    SpriteResultDraw(g_tex_chara[1].textureId, MkF2(posx, posy), MkF2(169 / 2, 160 / 2), DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
    //}



}
float LerpEasePingPong(float start, float end, float currentTime, float duration, std::function<float(float)> easing) {
    if (duration <= 0.0f) return end;  // 0秒指定なら即終了

    // 現在の往復サイクルの経過時間
    float cycleTime = fmod(currentTime, duration * 2); // 行き+戻りで2倍の時間

    float t = 0.0f;

    if (cycleTime < duration) {
        // 行き（start -> end）
        t = cycleTime / duration;
    }
    else {
        // 戻り（end -> start）
        t = (cycleTime - duration) / duration;
        t = 1.0f - t;  // 戻りなので反転
    }

    t = easing(t);  // イージング適用
    return start + (end - start) * t;
}
void UpdateProgress(float& progress, float moveTime)
{
    progress += 1.0f / moveTime;
    if (progress > 1.0f) progress = 1.0f;
}
// 進行度から補間位置を求める関数（イージング対応版）
Float2 F2Lerpshare(float startX, float startY, float endX, float endY,
    float progress, std::function<float(float)> easing)
{
    float t = easing(progress);  // 進行度にイージング適用
    float x = startX * (1.0f - t) + endX * t;
    float y = startY * (1.0f - t) + endY * t;
    return MkF2(x, y);
}

// 進行度からfloat値を補間（数値版）
float FLerpshare(float start, float end,
    float progress, std::function<float(float)> easing)
{
    float t = easing(progress);  // 進行度にイージング適用
    return start * (1.0f - t) + end * t;
}

//キャラアイコン
            //一人
            /*SpriteResultDraw(g_tex_chara[0].textureId,MkF2( Moveresultpaper.x -160.0f, Moveresultpaper.y + 70.0f), MkF2(169/2, 160/2), DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));*/
            //二人
            //SpriteResultDraw(g_tex_chara[1].textureId, MkF2(Moveresultpaper.x - 160.0f, Moveresultpaper.y + 55.0f), MkF2((169 / 2.5)*1.1, (160 / 2.5)*1.1), DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
            //SpriteResultDraw(g_tex_chara[0].textureId, MkF2(Moveresultpaper.x - 160.0f, Moveresultpaper.y + 95.0f), MkF2(169 / 2.5, 160 / 2.5), DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
            ///*RANK_POSX(-150)*/
           //三人
//SpriteResultDraw(g_tex_chara[0].textureId, MkF2(Moveresultpaper.x - 162.0f, Moveresultpaper.y + 55.0f), MkF2(169 / 3, 160 / 3), DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
//SpriteResultDraw(g_tex_chara[2].textureId, MkF2(Moveresultpaper.x - 184.0f, Moveresultpaper.y + 90.0f), MkF2(169 / 3, 160 / 3), DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
//SpriteResultDraw(g_tex_chara[1].textureId, MkF2(Moveresultpaper.x - 139.0f, Moveresultpaper.y + 90.0f), MkF2((169 / 3) * 1.1, (160 / 3) * 1.1), DRAW_POLYGON_PRIORITY_NML_u2, MkF4(1.0f, 1.0f, 1.0f, 1.0f));
