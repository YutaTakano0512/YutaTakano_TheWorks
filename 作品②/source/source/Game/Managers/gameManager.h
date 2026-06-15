// ===================================================
// gameManager.h ゲーム全体の制御
// 
// hara sougo  2024/07/22
// ===================================================
#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_


#include "../Systems/sys_general.h"

// ===================================================
// マクロ定義
// ===================================================
constexpr int MOVIE_SKIP_FLAG_FRAME = FRAME_ONE_SECOND * 2;
constexpr int MOVIE_SKIP_FLAG_START_FRAME = 15;
constexpr int MOVIE_SKIP_FLAG_PUSH_FRAME = 20;

// ===================================================
// 列挙体宣言
// ===================================================
enum class Difficulty {
    Normal = 0,
    Hard,
    Max
};

enum RESULT_SCORE_EX_TYPE {
    RESULT_SCORE_EX_TYPE_CLEAR = 0, //クリアボーナス
    RESULT_SCORE_EX_TYPE_ENEMY_DEFEAT, //エネミーを倒したことによる加点
    RESULT_SCORE_EX_TYPE_MANY_ENEMY_DEFEAT, //たくさん倒したことによる加点
    RESULT_SCORE_EX_TYPE_RARE_ENEMY_DEFEAT, //レアエネミー（ランダムイベント）
    RESULT_SCORE_EX_TYPE_SPEED_CLEAR, //スピードクリア
    RESULT_SCORE_EX_TYPE_DAMAGE_LESS, //ダメージレス

    RESULT_SCORE_EX_TYPE_MAX
};

enum PLAYER_CHARACTER_TYPE {
    PLAYER_CHARACTER_TYPE_None = -1, //空き席
    PLAYER_CHARACTER_TYPE_Kaito = 0,
    PLAYER_CHARACTER_TYPE_Neo,
    PLAYER_CHARACTER_TYPE_Contrius
};

// ===================================================
// 構造体宣言
// ===================================================

struct GAME_MANAGER_SAVE_RESULT {
    bool isClear;

    int score; //スコアの統計
    int scoreEx[RESULT_SCORE_EX_TYPE_MAX]; //スコア加点の詳細
    int rank;

    int clearTimeFrame;

    //参加プレイヤー
    // 配列 0～3番 が 1P～4Pに対応
    PLAYER_CHARACTER_TYPE players[SYS_PLAYER_MAX];
    int playersNum; //参加プレイ人数

    //難易度
    Difficulty difficulty;
};

// ===================================================
// プロトタイプ宣言
// ===================================================

#ifdef IS_MAIN //メイン以外からは呼び出せないようにしておく
void InitGameManager(void);
void UpdateGameManager(void);
void DrawGameManager(bool enable);
void UninitGameManager(void);
#endif

void GameStopRequest(int frame);
void GameStopResetRequest();
void GameSlowRequest(int frame, int rate);
void GamePause(bool enable);
bool IsGamePause();
bool IsGameStop();
void GameManagerSetSceneRequest(int sce, int delay, bool isClear);
void GameManagerSetSceneRequestCancel();

GAME_MANAGER_SAVE_RESULT GameManagerGetResult(); //リザルトシーンで取得する結果

void GameManagerResetResult(); //結果をリセット
void GameManagerResetResultScore(); //スコアの値をリセット
void GameManagerResultAddScore(int addscore, RESULT_SCORE_EX_TYPE type); //スコアを加算
int GameManagerGetResultAddScoreBuf(); //加算したスコアの値を取得(ほぼUI用)
void GameManagerResetResultAddScoreBuf(); //加算したスコアの値をリセット
void GameManagerSetResultClearTime(int time); //クリアタイム
void GameManagerSetResultRank(int rank); //ランク

void GameManagerResetResultAddScoreEvent(); //加算したスコアの値をリセット

void GameManagerResetSetDiff(Difficulty diff); //難易度の設定を保存
void GameManagerSetPlayerNum(int num); //参加プレイ人数を設定
void GameManagerSetPlayer(int index, PLAYER_CHARACTER_TYPE type); //参加プレイヤーを設定

bool GameManagerIsPlayerCtrlStop(void);
void GameManagerSetPlayerCtrlStop(bool b);

bool GameManagerIsMovieFlag(void);
void GameManagerSetMovieFlag(bool b, bool skip = false);
bool GameManagerIsMoviePlayerHiddenFlag(void);
void GameManagerSetMoviePlayerHiddenFlag(bool b);

int GameManagerGetMovieSkipFlagPushFrame(void);
int GameManagerGetMovieSkipFlagStartFrame(void);
int GameManagerGetMovieSkipFlagFrame(void);
bool GameManagerIsMovieSkipTrigger(void);

void GameManagerAddEnemyDefeatCall();

#endif