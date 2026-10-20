// ===================================================
// gameObject.h オブジェクト制御
// 
// hara sougo  2024/07/10
// ===================================================
#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_


#include "../Systems/sys_general.h"

#include "../Configuration/Param.h"
#include "../Texture/texture.h"
#include "../Sprite/sprite.h"
#include "../Sprite/spriteGraph.h"
#include "../Systems/easing.h"
//#include "../Systems/collision.h"

#include "gameObjectTypes.h"

// ===================================================
// マクロ定義
// ===================================================

//#define GAME_OBJECT_MAX (4096)
//#define GAME_OBJECT_MAX (256)
//#define GAME_OBJECT_MAX (1024)
#define GAME_OBJECT_MAX (2048)

constexpr int GAME_OBJECT_TXT_MAX = 64;

#define GAME_OBJECT_HIT_CYCLE_NUM_MAX (32)
#define GAME_OBJECT_HIT_CHECK_NUM_MAX (32)

#define GAME_OBJECT_HIT_REACTION_MIN (20)

#define GAME_OBJECT_COLLISION_NUM_MAX (16)

#define	GAME_OBJECT_GRAVITY_MAX	(50.0f)	//重力最高速

#define	GAME_OBJECT_BLOCK_HIT_IGNORE	(0.0f)

//オブジェクトの描画優先度目安
#define GAME_OBJECT_ZINDEX_ALWAYS			(0)		//最前面
#define GAME_OBJECT_ZINDEX_UI				(100)	//UI
#define GAME_OBJECT_ZINDEX_COVER			(200)	//ステージの手前オブジェクトなど
#define GAME_OBJECT_ZINDEX_EFFECT			(300)	//エフェクト
#define GAME_OBJECT_ZINDEX_PLAYER_ATTACK	(350)	//プレイヤーの攻撃オブジェクト
#define GAME_OBJECT_ZINDEX_PLAYER			(400)	//プレイヤーオブジェクト
#define GAME_OBJECT_ZINDEX_ENEMY_ATTACK		(450)	//敵の攻撃オブジェクト
#define GAME_OBJECT_ZINDEX_ENEMY			(500)	//敵オブジェクト
#define GAME_OBJECT_ZINDEX_OBJECT			(600)	//オブジェクトギミック等
#define GAME_OBJECT_ZINDEX_AFTEREFFECT		(650)	//残像
#define GAME_OBJECT_ZINDEX_BLOCK			(700)	//ブロック
#define GAME_OBJECT_ZINDEX_BG_EFFECT		(800)	//背景エフェクト
#define GAME_OBJECT_ZINDEX_BG				(900)	//背景
#define GAME_OBJECT_ZINDEX_DEEPEST			(1000)	//最背面

// ===================================================
// 列挙体定義
// ===================================================

//オブジェクトのガードタイプ
enum GAME_OBJECT_GUARD_TYPE {
	GAME_OBJECT_GUARD_TYPE_NORMAL = 0,
	GAME_OBJECT_GUARD_TYPE_SHORT,
	GAME_OBJECT_GUARD_TYPE_HALF,
	GAME_OBJECT_GUARD_TYPE_HARD,
	GAME_OBJECT_GUARD_TYPE_INV,
	GAME_OBJECT_GUARD_TYPE_WEAK,
	GAME_OBJECT_GUARD_TYPE_WEAKEX,

	GAME_OBJECT_GUARD_TYPE_MAX,
};

//ステータスのタイプ

#define STR__GAME_OBJECT_STATUS_TYPE ("GAME_OBJECT_STATUS_TYPE_")

enum GAME_OBJECT_STATUS_TYPE {
	GAME_OBJECT_STATUS_TYPE_VOID = 0,
	GAME_OBJECT_STATUS_TYPE_PLAYER,
	GAME_OBJECT_STATUS_TYPE_ENEMY_SMALL,
	GAME_OBJECT_STATUS_TYPE_ENEMY_MIDIUM,
	GAME_OBJECT_STATUS_TYPE_ENEMY_BOSS,

	GAME_OBJECT_STATUS_TYPE_MAX,
};

//攻撃の属性

#define STR__GAME_OBJECT_COLLISION_ATTACK_ATTRIBUTE ("GAME_OBJECT_COLLISION_ATTACK_ATTRIBUTE_")

enum GAME_OBJECT_COLLISION_ATTACK_ATTRIBUTE {
	GAME_OBJECT_COLLISION_ATTACK_ATTRIBUTE_VOID = 0,

	GAME_OBJECT_COLLISION_ATTACK_ATTRIBUTE_PLAYER_NORMAL_ATTACK,
	GAME_OBJECT_COLLISION_ATTACK_ATTRIBUTE_PLAYER_KAKOPON_ATTACK,
    GAME_OBJECT_COLLISION_ATTACK_ATTRIBUTE_PLAYER_SKILL_ATTACK,

	GAME_OBJECT_COLLISION_ATTACK_ATTRIBUTE_ENEMY_NORMAL_ATTACK,

	GAME_OBJECT_COLLISION_ATTACK_ATTRIBUTE_MAX,
};


enum GAME_OBJECT_COLLIDER_TYPE {
    GAME_OBJECT_COLLIDER_TYPE_NONE = 0,
    GAME_OBJECT_COLLIDER_TYPE_CAPSULE,
};


// ===================================================
// 構造体宣言
// ===================================================

struct GAME_OBJECT_IDENTIFIER {	// ゲームオブジェクトを判別するための構造体
	int objIndex; //オブジェクトのインデックス
	unsigned int objCreateFrame; //オブジェクトが生成されたフレーム
};

struct GAME_OBJECT_POSMOD_RESULT {	// ゲームオブジェクトの座標補正の結果
	bool enable;

	float left;
	float up;
	float right;
	float down;

    int objIndex; //オブジェクトのインデックス

    Float3 beforeVel;
};

struct GAME_OBJECT_STATUS {		// オブジェクトのステータス構造体
	int hp;
	int hpMin;
	int hpMax;
	GAME_OBJECT_GUARD_TYPE guardType;
    int invFrame;
    int hitStopArmorFrame;

	std::string fullName;
	std::string nickName;

	int beforeDamage;
	unsigned int beforeDamageFrame;

	//ステータス上の区分
	GAME_OBJECT_STATUS_TYPE statusType;

	//属性に対するダメージ倍率
	float attributeMod[GAME_OBJECT_COLLISION_ATTACK_ATTRIBUTE_MAX];

	bool deadFlag;
};
struct GAME_OBJECT_GRAPHIC {		// オブジェクトのグラフィック構造体
	bool hidden;
	bool stop;
	bool gray;
    bool shadow;
    Float4 shadowColor;
	bool billboard;
	Float4 color;

    Float3 scale3d;
    //Float3 xRot3d;
    Float3 rot3d;
	//float zPos3d;
	Float3 pos3d;

	bool enableShade;
    DPD_SHADER_OPT shaderOpt;
    DPD_SHADER_TYPE shaderType;
    DRAW_POLYGON_PRIORITY priority;
    bool depthBuffIgnore;

	SPRITE_GRAPH_DAT graph;
};

struct GAME_OBJECT_MOVE_REQUEST {		// 移動リクエスト
	bool enable;

	Float3 startPos;		// 移動前の座標
	Float3 endPos;		// 移動先の座標
	Float3 oldPos;		// 1F前の移動先の座標
	int frame;
	int frameMax;

	//イージング関数を指定する
	std::function<float(float)> easingX;
	std::function<float(float)> easingY;
	std::function<float(float)> easingZ;

	//ブロックとの判定を無視するか
	bool isBlockCollisionIgnore;
};
struct GAME_OBJECT_HITSTOP {	// ヒットストップ
	int frame;
	int frameMax;
};
struct GAME_OBJECT_HIT_REACTION {	// ヒットリアクション
	int frame;
	int frameMax;
};



struct GAME_OBJECT_COLLISION_ATTACK_CHILD {	// 攻撃判定
	bool enable; //有効フラグ
	//float sizeR; //当たり判定の半径
	Float3 pos;				// 座標
	Float3 size;		//当たり判定

	int enableFrame;
	int enableFrameMax;

	int power; //攻撃力
	int hitStopAttacker; //攻撃側のヒットストップ
	int hitStopReciever; //受け側のヒットストップ
	int hitCycle; //判定周期 重なり続けている場合 どのくらいの間隔で再ヒットするか

    int hitStern; //ヒット硬直
    float knockBackPower; //ノックバック
	//float guardSternMod; //ガード硬直倍率

	//bool isGrab; //つかみフラグ

	int groupNumber;
	unsigned int id;

    char tag[GAME_OBJECT_TXT_MAX];

    bool invIgnore; //無敵を無視する
    float guardIgnoreMod; //ガード無視倍率

	GAME_OBJECT_COLLISION_ATTACK_ATTRIBUTE attribute; //属性

	bool subjectFamily[GAME_OBJECT_FAMILY_MAX]; //判定するファミリ(trueのファミリに属するオブジェクトにのみ当たる)
};
struct GAME_OBJECT_COLLISION_RECIEVE_CHILD {	// くらい判定
	bool enable; //有効フラグ
	//float sizeR; //当たり判定の半径
	Float3 pos;				// 座標
	Float3 size;		//当たり判定

	//GAME_OBJECT_GUARD_TYPE guardType;
	float guardMod;

	bool subjectFamily[GAME_OBJECT_FAMILY_MAX]; //判定するファミリ(trueのファミリに属するオブジェクトにのみ当たる)
};

struct GAME_OBJECT_COLLISION_ATTACK {	// 攻撃判定
	bool enable; //有効フラグ
    GAME_OBJECT_IDENTIFIER ownerId;
    bool isOwnerHit;
	
	GAME_OBJECT_COLLISION_ATTACK_CHILD collision[GAME_OBJECT_COLLISION_NUM_MAX];
};
struct GAME_OBJECT_COLLISION_RECIEVE {	// くらい判定
	bool enable; //有効フラグ
	
	GAME_OBJECT_COLLISION_RECIEVE_CHILD collision[GAME_OBJECT_COLLISION_NUM_MAX];
};

struct GAME_OBJECT_COLLISION_DAMAGE_CYCLE {	// 当たり判定の判定周期保存
	//int objIndex; //当たったオブジェクト
	//unsigned int objCreateFrame;
	GAME_OBJECT_IDENTIFIER objIdentifier;

	int groupNumber;
	unsigned int id;

	int frame; //フレームカウント
	int frameMax;
};
//struct GAME_OBJECT_COLLISION_DAMAGE_CHECK {	// 当たり判定の判定をするかのチェック
//	unsigned int hitFrame;
//
//	int objIndex; //当たったオブジェクト
//	unsigned int objCreateFrame;
//
//	int groupNumber;
//	unsigned int id;
//};

struct GAME_OBJECT_COLLISION {	// 当たり判定
	GAME_OBJECT_COLLISION_DAMAGE_CYCLE colAtkCycleList[GAME_OBJECT_HIT_CYCLE_NUM_MAX];
	//GAME_OBJECT_COLLISION_DAMAGE_CHECK colAtkCheckList[GAME_OBJECT_HIT_CHECK_NUM_MAX];

	GAME_OBJECT_COLLISION_ATTACK atk; //攻撃判定
	GAME_OBJECT_COLLISION_RECIEVE rec; //くらい判定
};




struct GAME_OBJECT_COLLIDER {	//障害物としての当たり判定
    GAME_OBJECT_COLLIDER_TYPE type;

    union {
        FloatCapsule capsule;
    };
};





struct GAME_OBJECT_DAMAGE_RESULT {	// ゲームオブジェクトのダメージの結果
	int damage;
	float mod;

    int atkObjChildIndex; //攻撃側(個別)オブジェクト
    int atkObjIndex; //攻撃側オブジェクト
	int recObjIndex; //当たったオブジェクト

	GAME_OBJECT_COLLISION_ATTACK_CHILD colAtk;
	GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec;
};

struct GAME_OBJECT_DAMAGE_COLLISION_HIT_RESULT {	// ゲームオブジェクトのダメージ判定ヒットの結果
	GAME_OBJECT_DAMAGE_RESULT damageResult;
};


class GOE_Base;

struct GAME_OBJECT_EVENTS {	// ゲームオブジェクトのイベント
	GOE_Base* source;

	//std::function<void(int)> Init;					//初期化イベント
	//std::function<void(int)> FirstFrame;			//最初のフレームに呼ばれるイベント(Initより後)
	//std::function<void(int)> Update;				//更新イベント
	//std::function<void(int)> UpdateActive;			//更新イベント(オブジェクトがアクティブ)
	//std::function<void(int)> Draw;					//描画イベント
	//std::function<void(int)> Uninit;				//終了イベント

	//std::function<void(int)> Dead;					//やられイベント
	//std::function<void(int,
	//	GAME_OBJECT_DAMAGE_RESULT)> DamageAttacker;	//ダメージイベント(攻撃側)
	//std::function<void(int,
	//	GAME_OBJECT_DAMAGE_RESULT)> DamageReciever;	//ダメージイベント(受け側)

	//std::function<void(int,
	//	GAME_OBJECT_POSMOD_RESULT)> BlockHit;	//ブロックヒットイベント
};

struct GAME_OBJECT {		// オブジェクト構造体
	
	bool use;				// 使用フラグ
	char delRequ;			//削除フラグ

	unsigned int createFrame;

	unsigned int frame;
	unsigned int frameActive;

	int lifetimeFrame;		//存在できる時間
	int lifetimeFrameMax;

	int ownerObjectIndex;	//指定されたオブジェクトを追従する
	int hitstopOwnerObjectIndex;	//指定されたオブジェクトとヒットストップを共有する

	Float3 pos;				// 座標
	Float3 oldpos;			// 1フレーム前の座標

	int zIndex;				// 数値が小さいほど手前に描画される

	float rot;				// 角度 (見た目のみ)
    //Float2 scale;			// スケール
    Float3 scale;			// スケール

	Float3 vel;				// 速度
	float friction;			// 摩擦
	Float3 velImpact;		// 速度(強く動かす)

	Float2 size;			// サイズ (ブロックに対する当たり判定)
    GAME_OBJECT_COLLIDER collider;
    bool blockCollisionIgnore; //ブロックとの衝突を無視する
    std::function<bool(int, int)> blockCollisionIgnoreFunc; //ブロックとの衝突を無視する条件

	//bool cameraInKeep;		//カメラ外に出ない
    int cameraInKeep_Index;

	bool alwaysUpdate;

	float gravity;
	float gravityVel;
	float isGround;
	float isMoving;

	GAME_OBJECT_COLLISION collision; //当たり判定

	GAME_OBJECT_HIT_REACTION hitReaction;
	GAME_OBJECT_HITSTOP hitStop;
	GAME_OBJECT_MOVE_REQUEST moveRequ;

	GAME_OBJECT_TYPE type; //タイプ
	bool family[GAME_OBJECT_FAMILY_MAX]; //ファミリー
	GAME_OBJECT_GRAPHIC graphic; //グラフィック

	GAME_OBJECT_STATUS status; //ステータス

	GAME_OBJECT_EVENTS events; //オブジェクトのイベント

	std::vector<std::string> tags; //識別とかに使う文字列の動的配列

	int freeInt; //ちょっとした識別に使うかもしれない
	void* privateVariable; //このオブジェクト独自で使う変数の格納場所
};


// ===================================================
// プロトタイプ宣言
// ===================================================
void InitGameObjectStatus(GAME_OBJECT_STATUS* s);

void InitGameObject(GAME_OBJECT* obj);
void InitGameObject(void);
void InitGameObjectGraphic(GAME_OBJECT_GRAPHIC* gra);

void UpdateGameObject(void);
void DrawGameObject(void);
void DrawGameObjectData(GAME_OBJECT* obj);
void UninitGameObject(void);

GAME_OBJECT* CreateGameObject(GAME_OBJECT obj);

bool DeleteGameObject(GAME_OBJECT* obj);
bool DeleteGameObject(int index);
bool DeleteGameObjectAll();

GAME_OBJECT* GetGameObjects();
GAME_OBJECT* GetGameObject(int index);
GAME_OBJECT* GetGameObjectFindByIdentifier(GAME_OBJECT_IDENTIFIER identifier);

int GetGameObjectIndex(GAME_OBJECT* obj);

bool IsActiveGameObject(GAME_OBJECT* obj);

bool IsCameraInGameObject(GAME_OBJECT* obj);
bool IsCameraInGameObject(int cameraIndex, GAME_OBJECT* obj);

bool IsBelongsFamilyGameObject(GAME_OBJECT* obj, GAME_OBJECT_FAMILY family);


bool IsDefeatGameObject(GAME_OBJECT* obj);
bool IsInStageGameObject(GAME_OBJECT* obj);
Float3 InStageGameObjectPos(Float3 pos, Float3 size);


void GameObjectSetLifeTime(GAME_OBJECT* obj, int life);

//移動リクエスト
void GameObjectMoveRequest(GAME_OBJECT* obj,
	int frame,
	Float3 pos,
	std::function<float(float)> easingX = nullptr,
	std::function<float(float)> easingY = nullptr,
	std::function<float(float)> easingZ = nullptr,
	bool isBlockCollisionIgnore = false
);

//ヒットストップ
void GameObjectHitStop(GAME_OBJECT* obj,
	int frame
);

//ヒットリアクション
void GameObjectHitReaction(GAME_OBJECT* obj,
	int frame
);

//ダメージ（計算のみ）
GAME_OBJECT_DAMAGE_RESULT GameObjectGetDamage(GAME_OBJECT* obj, GAME_OBJECT* objAttacker,
    int damage,
    GAME_OBJECT_COLLISION_ATTACK_CHILD colAtk,
    GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec
);
//ダメージ
GAME_OBJECT_DAMAGE_RESULT GameObjectDamage(GAME_OBJECT* obj, GAME_OBJECT* objAttacker,
	int damage,
	GAME_OBJECT_COLLISION_ATTACK_CHILD colAtk,
	GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec
);
GAME_OBJECT_DAMAGE_RESULT GameObjectDamage(
    GAME_OBJECT* obj, GAME_OBJECT* objAttacker,
    GAME_OBJECT_DAMAGE_RESULT dr
);

//ダメージ判定に当たったことにする
GAME_OBJECT_DAMAGE_COLLISION_HIT_RESULT GameObjectDamageCollisionHit(GAME_OBJECT* obj, GAME_OBJECT* objAttacker,
	GAME_OBJECT_COLLISION_ATTACK_CHILD colAtk,
	GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec,
    GAME_OBJECT* objAttackerChild=nullptr
);

//ダメージ判定を追加する
void GameObjectAttackCollisionAdd(GAME_OBJECT* gobj, GAME_OBJECT_COLLISION_ATTACK_CHILD newAtkCol,
	int delayFrame = 0,
	int enableFrame = 0
);

void AddTagGameObject(GAME_OBJECT* gobj, std::string str);
bool HasTagGameObject(GAME_OBJECT* gobj, std::string str);

//オブジェクトが識別子とイッチするかチェックする
bool CheckIdentifierGameObject(GAME_OBJECT* gobj, GAME_OBJECT_IDENTIFIER identifier);

//オブジェクトの識別子を取得する
GAME_OBJECT_IDENTIFIER GetIdentifierGameObject(GAME_OBJECT* gobj);

std::map<int, GAME_OBJECT*> GetGameObjectFindByDistance(Float3 startPos, float distance);

Float3 GetGameObjectColRecAbsolutePos(GAME_OBJECT* gobj);

#endif