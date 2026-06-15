//---------------------------------------------------------------------------
// character.cpp
// 
// sougo hara  2024/11/11
//---------------------------------------------------------------------------

#include <fstream>
#include <sstream>
#include <utility>
#include <array>
#include <vector>

#include "../Systems/enum.h"
#include "../Configuration/Param.h"
#include "../../game_main.h"

#include "../Game Object/gameObject.h"
#include "../Game Object/gameObjectPrefab.h"

#include "player.h"
#include "character.h"
//#include "characterFunctionSetter.h"

#include "../Game Object/Game Object Events/GOE_Character.h"

// ===================================================
// マクロ定義
// ===================================================

// ===================================================
// プロトタイプ宣言
// ===================================================
void motionColDatInit(void);
void statusDatInit(void);

// ===================================================
// グローバル変数
// ===================================================
static std::vector<Character::MOTION_COL_DAT> motionColDat[Character::MOTION_COL_TYPE_MAX] = {};
static std::vector<Character::STATUS_DAT> statusDat = {};

static std::vector<Character::MAIN> CharaDatMainCache = {};


void Character::Init(void)
{
	statusDatInit();
	motionColDatInit();
}

void Character::Uninit(void)
{
}


Character::MAIN_LOAD* Character::Load(std::string name)
{
	std::vector<std::string> fileNames;

	std::string folderPath = std::string(CHARACTER_DATA_FOLDER_PATH)
		+ std::string("/") + std::string(CHARACTER_FOLDER_PATH);

    GetFileNames(folderPath, fileNames);

	//指定のフォルダ内を調べる
	for (const auto& fname : fileNames)
	{
		char cpytxt[Character::TXT_MAX] = {};
		char* cpytxtContext = nullptr;
		strcpy_s(cpytxt, Character::TXT_MAX, fname.c_str());

		//char* token = strtok(cpytxt, ".");
		char* token = strtok_s(cpytxt, ".", &cpytxtContext);

		if (token != NULL)
		{
			//ファイル名が一致するかを調べる
			if (name == std::string(token))
			{
				// ファイルを開く
				FILE* fp = nullptr;

				std::string pathstr = folderPath + std::string("/") + fname;

				//DebugPrintf("%s\n", fname.c_str());
				//DebugPrintf("%s\n", name.c_str());
				//DebugPrintf("%s\n", token);

				fopen_s(&fp, pathstr.c_str(), "rb");

				if (fp != nullptr)
				{
					//メモリ確保
					Character::MAIN_LOAD* charaDat = (Character::MAIN_LOAD*)malloc(sizeof(Character::MAIN_LOAD));

					if (charaDat != nullptr)
					{
						// 読み込む
						fread(charaDat, sizeof(*charaDat), 1, fp);
					}

					// ファイルを閉じる
					fclose(fp);

					//読み込んだデータをお返し
					return charaDat;
				}
			}
		}
	}
	return nullptr;
}

void Character::Unload(Character::MAIN_LOAD* charaDat)
{
	free(charaDat);
}


GAME_OBJECT* Character::CreateCharacterGameObject(Character::MAIN_LOAD* charaDat)
{
	if (charaDat == nullptr)
	{
		return  nullptr;
	}

	//--------------------------------------------------
	// キャラクター
	//--------------------------------------------------
	{
		GAME_OBJECT go = {};
		InitGameObject(&go);

		go.type = GAME_OBJECT_TYPE_CHARACTER;

		//go.family[GAME_OBJECT_FAMILY_PLAYER] = true;

		go.zIndex /*	*/ = GAME_OBJECT_ZINDEX_PLAYER;

		go.size /*		*/ = MkF2(Character::DEFAULT_SIZE, Character::DEFAULT_SIZE);
		//go.cameraInKeep = true;
		go.alwaysUpdate = true;

		go.friction /*	*/ = Character::FRICTION;
		go.gravity /*	*/ = Character::GRAVITY;

		go.graphic /*	*/ = {};

		//{ //ステータス
		//	GAME_OBJECT_STATUS* s = &go.status;
		//	s->hpMax = PARAM_GAME_OBJECT_CHARACTER_STATUS_HP;
		//	s->hp = s->hpMax;
		//	s->guardType = GAME_OBJECT_GUARD_TYPE_NORMAL;
		//}

		//{ //イベント
		//	GAME_OBJECT_EVENTS* e = &go.events;
		//	e->Init /*			*/ = GOE_InitCharacterObject;
		//	e->FirstFrame /*	*/ = GOE_FirstFrameCharacterObject;
		//	e->Uninit /*		*/ = GOE_UninitCharacterObject;
		//	e->Update /*		*/ = GOE_UpdateCharacterObject;
		//	e->UpdateActive /*	*/ = GOE_UpdateActiveCharacterObject;
		//	e->Draw /*			*/ = GOE_DrawCharacterObject;

		//	e->Dead /*			*/ = GOE_DeadCharacterObject;
		//	e->DamageAttacker /**/ = GOE_DamageAttackerCharacterObject;
		//	e->DamageReciever /**/ = GOE_DamageRecieverCharacterObject;
		//}

		{ //当たり判定
			GAME_OBJECT_COLLISION* c = &go.collision;

			//攻撃判定
			c->atk.enable = true;
			//{
			//	GAME_OBJECT_COLLISION_ATTACK_CHILD* cc = c->atk.collision + 0;
			//	cc->enable = true;
			//	cc->pos /*				*/ = MkF3(40.0f, 0.0f, 0.0f);
			//	cc->size.x /*			*/ = PARAM_GAME_OBJECT_TEST_COL_ATK_SIZE_X;
			//	cc->size.y /*			*/ = PARAM_GAME_OBJECT_TEST_COL_ATK_SIZE_Y;
			//	cc->size.z /*			*/ = PARAM_GAME_OBJECT_TEST_COL_ATK_SIZE_Z;
			//	cc->power /*			*/ = PARAM_GAME_OBJECT_TEST_COL_ATK_POWER;
			//	cc->hitStopAttacker /*	*/ = PARAM_GAME_OBJECT_TEST_COL_ATK_HITSTOP_ATK;
			//	cc->hitStopReciever/*	*/ = PARAM_GAME_OBJECT_TEST_COL_ATK_HITSTOP_REC;
			//	cc->hitCycle /*			*/ = PARAM_GAME_OBJECT_TEST_COL_ATK_HITCYCLE;
			//	cc->subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;
			//}
			
			//くらい判定
			c->rec.enable = true;
			//{
			//	GAME_OBJECT_COLLISION_RECIEVE_CHILD* cc = c->rec.collision + 0;
			//	cc->enable = true;
			//	cc->pos = MkF3(0.0f, 0.0f, 0.0f);
			//	cc->size.x = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_X;
			//	cc->size.y = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_Y;
			//	cc->size.z = PARAM_GAME_OBJECT_PLAYER_COL_REC_SIZE_Z;
			//}
		}


		//キャラクターのオブジェクトのデータのメモリを確保
		
		Character::OBJECT_DAT* charaGoDat = (Character::OBJECT_DAT*)malloc(sizeof(Character::OBJECT_DAT));

		if (charaGoDat != nullptr)
		{
			(*charaGoDat) = {};
			charaGoDat->dataIndex = -1;

			//メモリ確保
			//Character::MAIN* charaDatCpy = (Character::MAIN*)malloc(sizeof(Character::MAIN));

			//if (charaDatCpy != nullptr)
			{
				//中身をコピー
				
				//(*charaDatCpy) = (*charaDat);


				//Character::MAIN* charaDatMain = nullptr;

				//キャッシュがあったらそれを渡す
				for (int i = 0; i < CharaDatMainCache.size(); i++)
				{
					if (CharaDatMainCache[i].name == charaDat->name)
					{
						//charaGoDat->data = &CharaDatMainCache[i];
						charaGoDat->dataIndex = i;
						break;
					}
				}

				//キャッシュがなかったら新規で読み込む
				//if (charaGoDat->data == nullptr)
				if (charaGoDat->dataIndex < 0)
				{
					Character::MAIN newdat = {};
					newdat.use = true;
					newdat.name = charaDat->name;
					newdat.sprite = charaDat->sprite;
					newdat.spriteXCnt = charaDat->spriteXCnt;
					newdat.spriteYCnt = charaDat->spriteYCnt;
					newdat.statusName = charaDat->statusName;

					for (int i = 0; i < MOTION_MAX; i++)
					{
						const MOTION_LOAD& motionRaw = charaDat->motion[i];
						if (motionRaw.use)
						{
							MOTION motionTmp = {};
							motionTmp.name = motionRaw.name;
							motionTmp.maxFrame = motionRaw.maxFrame;

							for (int j = 0; j < MOTION_FRAME_MAX; j++)
							{
								const MOTION_FRAME_LOAD& motionFrameRaw = motionRaw.frame[j];
								if (motionFrameRaw.use)
								{
									MOTION_FRAME motionFrameTmp = {};
									motionFrameTmp.spriteNumber = motionFrameRaw.spriteNumber;

									for (int k = 0; k < MOTION_COL_MAX; k++)
									{
										const MOTION_COL& motionColRaw = motionFrameRaw.collision[k];
										if (motionColRaw.use)
										{
											motionFrameTmp.collision.push_back(motionColRaw);
										}
									}
									motionTmp.frame.push_back(motionFrameTmp);
								}
							}
							newdat.motion.push_back(motionTmp);
						}
					}
					CharaDatMainCache.push_back(newdat);

					if (CharaDatMainCache.size() > 0)
					{
						//charaGoDat->data = &CharaDatMainCache[CharaDatMainCache.size() - 1];
						charaGoDat->dataIndex = (int)(CharaDatMainCache.size() - 1);
					}
				}

				//charaGoDat->data = charaDatCpy;

				//if (charaGoDat->data != nullptr)
				if (charaGoDat->dataIndex != -1)
				{
					AddTagGameObject(&go, charaDat->name);

					Float3 graPos = { };
                    std::string defaultColAtkName = "COL_ATK_GENERAL_0";
					{ //ステータスをセットしてやる
						Character::STATUS_DAT sd = Character::GetStatusDataFindByName(Character::GetCharacterMainData(charaGoDat->dataIndex)->statusName);
						if (sd.use)
						{

                            go.scale.x *= sd.scale;
                            go.scale.y *= sd.scale;

                            for (int i = 0; i < GAME_OBJECT_FAMILY_MAX; i++)
                            {
                                go.family[i] = sd.family[i];
                            }

							graPos = sd.pos;
                            defaultColAtkName = sd.defaultColAtkName;

                            SetStatusData(&go, sd);
						}
					}

					//{ //イベントをセットしてやる
					//	CharacterFunctionSet(*charaGoDat, &go.events);
					//}

					//テクスチャの読み込み
					//{
     //                   charaGoDat->texture = LoadTexData(Character::GetCharacterMainData(charaGoDat->dataIndex)->sprite.c_str());
     //                   charaGoDat->texture.enable = true;

					//	//charaGoDat->texture = {};
					//	//charaGoDat->texture.enable = true;
					//	//{
     // //                      //TEXTURE_DAT td = LoadTextureEx(Character::GetCharacterMainData(charaGoDat->dataIndex)->sprite.c_str());
     // //                      //charaGoDat->texture.textureId = td.main;
     // //                      charaGoDat->texture.textureId = LoadTexture(Character::GetCharacterMainData(charaGoDat->dataIndex)->sprite.c_str());
					//	//	//charaGoDat->texture.textureIdGray = td.gray;
					//	//}
					//	strcpy_s(charaGoDat->texture.texturePath, TEX_DATA_PATH_MAX, Character::GetCharacterMainData(charaGoDat->dataIndex)->sprite.c_str());
					//}


					//グラフィックの生成
					{
						//GAME_OBJECT_GRAPHIC goGra = {};
						//InitGameObjectGraphic(&goGra);
						//InitSpriteGraph(&goGra.graph);

						//goGra.shadow = true;

						//goGra.color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);

						//goGra.shaderOpt.bloom = true;
						//goGra.shaderOpt.outline = true;

						//{

							//SPRITE_GRAPH_DAT* sgdat = &goGra.graph;
							//sgdat->outlineWidth = 20.0f;
							////sgdat->alpha = 1.0f;

							//int imgW = GetTextureWidth(charaGoDat->texture.textureId);
							//int imgH = GetTextureHeight(charaGoDat->texture.textureId);

							//int imgXCnt = Character::GetCharacterMainData(charaGoDat->dataIndex)->spriteXCnt;
							//int imgYCnt = Character::GetCharacterMainData(charaGoDat->dataIndex)->spriteYCnt;
							//if (imgXCnt <= 0)
							//{
							//	imgXCnt = 1;
							//}
							//if (imgYCnt <= 0)
							//{
							//	imgYCnt = 1;
							//}

							//int img1FW = imgW / imgXCnt;
							//int img1FH = imgH / imgYCnt;

							//float mX = 1.0f;
							//float mY = 1.0f;

							//if (img1FW > img1FH)
							//{
							//	mY = ((float)img1FH) / ((float)img1FW);
							//}
							//else
							//{
							//	mX = ((float)img1FW) / ((float)img1FH);
							//}

                            //Float2 sizetmp = MkF2(
                            //    Character::DEFAULT_SIZE * mX,
                            //    Character::DEFAULT_SIZE * mY
                            //);

							//{ //スプライトをセット
							//	SPRITE_ANIM_DAT* sadat = sgdat->spriteAnim + 0;
							//	sadat->isUse = true;
							//	sadat->texDat = charaGoDat->texture;
							//	//sadat->color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
							//	
							//	sadat->pos3D = graPos;
							//	sadat->rot = 0.0f;
							//	sadat->size = MkF2(sizetmp.x, sizetmp.y);

							//	sadat->no = 0;
							//	sadat->xcnt = GetCharacterMainData(charaGoDat->dataIndex)->spriteXCnt;
							//	sadat->hcnt = GetCharacterMainData(charaGoDat->dataIndex)->spriteYCnt;
							//	//DebugPrintf("%d\n", sadat->xcnt);
							//	//DebugPrintf("%d\n", sadat->hcnt);
							//}

							//goGra.xRot3d = 50.0f;
							//goGra.pos3d.y = -sizetmp.y / 4;
							//goGra.pos3d.z = -sizetmp.y / 2;
						//}
						//go.graphic = goGra;

                        //位置設定
                        go.graphic.graph.spriteAnim[0].pos3D = graPos;

                        Float2 sizetmp = MkF2(
                            Character::DEFAULT_SIZE,
                            Character::DEFAULT_SIZE
                        );

                        //サイズを再設定する
                        go.size = sizetmp;
                        go.size.x *= 0.75f;
                        go.size.y *= 0.75f;
                        { //ついでに判定もセット

                            Character::MOTION_COL_DAT colDetails =
                                Character::GetMotionColDataFindByName(
                                    Character::MOTION_COL_TYPE_ATK,
                                    defaultColAtkName
                                );

                            GAME_OBJECT_COLLISION_ATTACK_CHILD* cc = go.collision.atk.collision + 0;
                            (*cc) = colDetails.atkCol;
                            cc->enable = true;

                            cc->pos = MkF3(0.0f, 0.0f, 0.0f);

                            cc->size = MkF3(100.0f, 100.0f, 100.0f);
                            cc->size.z = (float)sqrt(pow(cc->size.x, 2) + pow(cc->size.y, 2));
                            cc->pos.z = -cc->size.z / 2;
                            //cc->guardMod = 1.0f;

                            //cc->id = GetMainGameFrame() + i;

                            cc->subjectFamily[GAME_OBJECT_FAMILY_PLAYER] = true;
                        }
                        { //ついでにくらい判定もセット

                            Character::MOTION_COL_DAT colDetails =
                                Character::GetMotionColDataFindByName(
                                    Character::MOTION_COL_TYPE_REC,
                                    CHARACTER_DEFAULT_REC_COL_NAME
                                );

                            GAME_OBJECT_COLLISION_RECIEVE_CHILD* cc = go.collision.rec.collision + 0;
                            (*cc) = colDetails.recCol;
                            cc->enable = true;

                            cc->pos = MkF3(0.0f, 0.0f, 0.0f);

                            //cc->size = Float2ToFloat3(go.size);
                            cc->size = MkF3(80.0f, 80.0f, 80.0f);
                            cc->size.z = (float)sqrt(pow(cc->size.x, 2) + pow(cc->size.y, 2));
                            cc->pos.z = -cc->size.z / 2;
                            //cc->guardMod = 1.0f;

                            cc->subjectFamily[GAME_OBJECT_FAMILY_ALL] = true;
                        }
					}
				}
			}

			go.privateVariable = charaGoDat;
		}

		//キャラクターのオブジェクトを生成
		GAME_OBJECT* createdGo = CreateGameObject(go);
		return createdGo;
	}
}

GAME_OBJECT* Character::CreateCharacterGameObject(std::string name)
{
	GAME_OBJECT* r = nullptr;

	//キャラデータロード
	Character::MAIN_LOAD* cd = Character::Load(name);
	if (cd != nullptr)
	{
		//キャラオブジェクト生成
		r = Character::CreateCharacterGameObject(cd);

		//キャラデータアンロード
		Character::Unload(cd);
	}

	return r;
}


const Character::MAIN* Character::GetCharacterMainData(int dataIndex)
{
	return &CharaDatMainCache[dataIndex];
}

void Character::CharacterMainDataClearCache(void)
{
	for (int i = 0; i < CharaDatMainCache.size(); i++)
	{
		for (int j = 0; j < CharaDatMainCache[i].motion.size(); j++)
		{
			for (int k = 0; k < CharaDatMainCache[i].motion[j].frame.size(); k++)
			{
				CharaDatMainCache[i].motion[j].frame[k].collision.clear();
				CharaDatMainCache[i].motion[j].frame[k].collision.shrink_to_fit();
			}
			CharaDatMainCache[i].motion[j].frame.clear();
			CharaDatMainCache[i].motion[j].frame.shrink_to_fit();
		}
		CharaDatMainCache[i].motion.clear();
		CharaDatMainCache[i].motion.shrink_to_fit();
	}
	CharaDatMainCache.clear();
	CharaDatMainCache.shrink_to_fit();
}

int Character::GetCharacterMotionFindByName(const Character::MAIN* charaDat, std::string name)
{
	//for (int i = 0; i < Character::MOTION_MAX; i++)
	//{
	//	if (charaDat->motion[i].use
	//		&& name == std::string(charaDat->motion[i].name))
	//	{
	//		return i;
	//	}
	//}

	//int index = 0;
	//for (const Character::MOTION m : charaDat->motion)
	//{
	//	if (name == std::string(m.name))
	//	{
	//		return index;
	//	}
	//	index++;
	//}
	
	if (charaDat != nullptr)
	{
		for (int i = 0; i < charaDat->motion.size(); i++)
		{
			if (name == charaDat->motion[i].name)
			{
				return i;
			}
		}
	}

	return -1;
}


Character::PLAY_MOTION_ID Character::PlayMotionCharacterGameObject(Character::OBJECT_DAT* charObj,
	int index,
	bool loop,
	int const* frameRef,
	int maxFrame)
{
	//if (index < 0 || index >= Character::MOTION_MAX)
	//{
	//	return 0;
	//}
	if (index < 0 || index >= GetCharacterMainData(charObj->dataIndex)->motion.size())
	{
		return 0;
	}

	//キーを発行
	charObj->playingMotion.playKey = GetMainGameFrame();

	charObj->playingMotion.motion = &GetCharacterMainData(charObj->dataIndex)->motion[index];
	charObj->playingMotion.frame = 0;
	if (maxFrame > 0)
	{
		charObj->playingMotion.maxFrame = maxFrame;
	}
	else
	{
		charObj->playingMotion.maxFrame = charObj->playingMotion.motion->maxFrame;
	}
	charObj->playingMotion.loop = loop;

	charObj->playingMotion.frameReference = frameRef;

	charObj->playingMotion.enable = true;

	return charObj->playingMotion.playKey;
}

Character::OBJECT_DAT* Character::GetObjectDataCharacterGameObject(GAME_OBJECT* obj)
{
	if (obj == nullptr)
	{
		return nullptr;
	}
	return (Character::OBJECT_DAT*)obj->privateVariable;
}

bool Character::GetCtrlPress(Character::OBJECT_DAT* charObj, CONTROLLER_BUTTON_TYPE button)
{
	return charObj->inputDat.buttonPushFrame[button] > 0;
}
bool Character::GetCtrlTrigger(Character::OBJECT_DAT* charObj, CONTROLLER_BUTTON_TYPE button)
{
	return charObj->inputDat.buttonPushFrame[button] == 1;
}
bool Character::GetCtrlRelease(Character::OBJECT_DAT* charObj, CONTROLLER_BUTTON_TYPE button)
{
	return charObj->inputDat.buttonPushFrame[button] == -1;
}

Float2 Character::GetCtrlStickLeft(Character::OBJECT_DAT* charObj)
{
	return charObj->inputDat.stickLeft.stick;
}
Float2 Character::GetCtrlStickRight(Character::OBJECT_DAT* charObj)
{
	return charObj->inputDat.stickRight.stick;
}


CONTROLLER_STICK_DAT Character::GetCtrlStickLeftTrigger(Character::OBJECT_DAT* charObj)
{
	CONTROLLER_STICK_DAT stickDat = charObj->inputDat.stickLeft;
	stickDat.enable = (stickDat.frame == 1);
	return stickDat;
}
CONTROLLER_STICK_DAT Character::GetCtrlStickRightTrigger(Character::OBJECT_DAT* charObj)
{
	CONTROLLER_STICK_DAT stickDat = charObj->inputDat.stickRight;
	stickDat.enable = (stickDat.frame == 1);
	return stickDat;
}

CONTROLLER_STICK_DAT Character::GetCtrlStickLeftPress(Character::OBJECT_DAT* charObj)
{
	CONTROLLER_STICK_DAT stickDat = charObj->inputDat.stickLeft;
	stickDat.enable = (stickDat.frame > 0);
	return stickDat;
}
CONTROLLER_STICK_DAT Character::GetCtrlStickRightPress(Character::OBJECT_DAT* charObj)
{
	CONTROLLER_STICK_DAT stickDat = charObj->inputDat.stickRight;
	stickDat.enable = (stickDat.frame > 0);
	return stickDat;
}

CONTROLLER_STICK_DAT Character::GetCtrlStickLeftRelease(Character::OBJECT_DAT* charObj)
{
	CONTROLLER_STICK_DAT stickDat = charObj->inputDat.stickLeft;
	stickDat.enable = (stickDat.frame == -1);
	return stickDat;
}
CONTROLLER_STICK_DAT Character::GetCtrlStickRightRelease(Character::OBJECT_DAT* charObj)
{
	CONTROLLER_STICK_DAT stickDat = charObj->inputDat.stickRight;
	stickDat.enable = (stickDat.frame == -1);
	return stickDat;
}


Character::MOTION_COL_DAT Character::GetMotionColDataFindByName(
	Character::MOTION_COL_TYPE mtype, std::string name)
{
	const std::vector<Character::MOTION_COL_DAT>& mcdVec = motionColDat[mtype];

	for (const auto& mcd : mcdVec)
	{
		if (std::string(mcd.name) == name)
		{
			return mcd;
		}
	}

	return {};
}

Character::STATUS_DAT Character::GetStatusDataFindByName(std::string name)
{
	for (const auto& sd : statusDat)
	{
		if (std::string(sd.name) == name)
		{
			return sd;
		}
	}

	return {};
}

void Character::SetStatusData(GAME_OBJECT* obj, Character::STATUS_DAT sd)
{
    if (sd.use)
    {
        obj->status = sd.status;
    }
}



void motionColDatInit(void)
{
	motionColDat->clear();
	std::string folderPath = std::string(CHARACTER_DATA_FOLDER_PATH)
		+ std::string("/") + std::string(CHARACTER_COL_FOLDER_PATH);

	for (int i = 0; i < Character::MOTION_COL_TYPE_MAX; i++)
	{
		std::string folderPathChild = folderPath + std::string("/");
		switch (i)
		{
		case Character::MOTION_COL_TYPE_ATK:
			folderPathChild += std::string(CHARACTER_COL_ATK_FOLDER_PATH);
			break;
		case Character::MOTION_COL_TYPE_GRAB:
			folderPathChild += std::string(CHARACTER_COL_GRAB_FOLDER_PATH);
			break;
		case Character::MOTION_COL_TYPE_REC:
			folderPathChild += std::string(CHARACTER_COL_REC_FOLDER_PATH);
			break;
		default:break;
		}

		std::vector<std::string> fileNames;

        GetFileNames(folderPathChild, fileNames);

		//指定のフォルダ内を調べる
		for (const auto& fname : fileNames)
		{
			// ファイルを開く
			FILE* fp = nullptr;

			std::string pathstr = folderPathChild + std::string("/") + fname;

			fopen_s(&fp, pathstr.c_str(), "rb");

			if (fp != nullptr)
			{
				Character::MOTION_COL_DAT newMotionColDat = {};
				newMotionColDat.use = true;
				newMotionColDat.type = (Character::MOTION_COL_TYPE)i;

				{
					char cpytxt[Character::TXT_MAX] = {};
					char* cpytxtContext = nullptr;
					strcpy_s(cpytxt, Character::TXT_MAX, fname.c_str());

					char* token = strtok_s(cpytxt, ".", &cpytxtContext);
					if (token != NULL)
					{
						strcpy_s(newMotionColDat.name, Character::TXT_MAX, cpytxt);
					}
				}

				switch (i)
				{
				case Character::MOTION_COL_TYPE_ATK:
					newMotionColDat.atkCol.enable = true;
					break;
				case Character::MOTION_COL_TYPE_GRAB:
					newMotionColDat.atkCol.enable = true;
					//newMotionColDat.atkCol.isGrab = true;
					break;
				case Character::MOTION_COL_TYPE_REC:
					newMotionColDat.recCol.enable = true;
					break;
				default:break;
				}


				char buf[Character::TXT_MAX] = {};
				while (fgets(buf, sizeof(buf), fp))
				{
					std::string buf2 = std::string(buf);
					buf2.erase(remove(buf2.begin(), buf2.end(), ' '), buf2.end());

					char cpytxt[Character::TXT_MAX] = {};
					char* cpytxtContext = nullptr;
					strcpy_s(cpytxt, Character::TXT_MAX, buf2.c_str());

					char* token = strtok_s(cpytxt, "//", &cpytxtContext);
					if (token != NULL && std::string(token).size() > 0)
					{
						std::string loadTestBuf = std::string(token);
						token = strtok_s(cpytxt, ":", &cpytxtContext);
						if (token != NULL && std::string(token).size() > 0)
						{
							std::string typeBuf = std::string(token);
							if (cpytxtContext != NULL && std::string(cpytxtContext).size() > 0)
							{
								std::string valueBuf = std::string(cpytxtContext);

								//atk
								if (typeBuf == "power")
								{
									newMotionColDat.atkCol.power = atoi(valueBuf.c_str());
								}
								if (typeBuf == "hitStopAttacker")
								{
									newMotionColDat.atkCol.hitStopAttacker = atoi(valueBuf.c_str());
								}
								if (typeBuf == "hitStopReciever")
								{
									newMotionColDat.atkCol.hitStopReciever = atoi(valueBuf.c_str());
								}
                                if (typeBuf == "hitCycle")
                                {
                                    newMotionColDat.atkCol.hitCycle = atoi(valueBuf.c_str());
                                }
                                if (typeBuf == "hitStern")
                                {
                                    newMotionColDat.atkCol.hitStern = atoi(valueBuf.c_str());
                                }
                                if (typeBuf == "knockBackPower")
                                {
                                    newMotionColDat.atkCol.knockBackPower = atof(valueBuf.c_str());
                                }
								if (typeBuf == "attribute")
								{
									STR_SPRIT families = StrSplit(valueBuf, ',', true);

									for (const auto& fam : families)
									{
										std::string fam2 = fam;
										//fam2.erase(remove(fam2.begin(), fam2.end(), '\r'), fam2.end());
										//fam2.erase(remove(fam2.begin(), fam2.end(), '\n'), fam2.end());

										for (int i = 0; i < GAME_OBJECT_FAMILY_MAX; i++)
										{
											if (GetEnumName((GAME_OBJECT_COLLISION_ATTACK_ATTRIBUTE)i)
												== (std::string(STR__GAME_OBJECT_COLLISION_ATTACK_ATTRIBUTE) + fam2))
											{
												newMotionColDat.atkCol.attribute = (GAME_OBJECT_COLLISION_ATTACK_ATTRIBUTE)i;
												break;
											}
										}
									}
									//newStatusDat.family = valueBuf;
								}
                                if (typeBuf == "guardIgnoreMod")
                                {
                                    newMotionColDat.atkCol.guardIgnoreMod = atof(valueBuf.c_str());
                                }

                                //rec
                                if (typeBuf == "guardMod")
                                {
                                    newMotionColDat.recCol.guardMod = (float)atof(valueBuf.c_str());
                                }

								//DebugPrintf("test %s\n", loadTestBuf.c_str());
								//DebugPrintf("t %s  ", typeBuf.c_str());
								//DebugPrintf("v %s\n", valueBuf.c_str());
							}
						}
					}
				}
				motionColDat[i].push_back(newMotionColDat);

				// ファイルを閉じる
				fclose(fp);
			}
		}
	}
}


void statusDatInit(void)
{
	statusDat.clear();

	std::string folderPath = std::string(CHARACTER_DATA_FOLDER_PATH)
		+ std::string("/") + std::string(CHARACTER_STATUS_FOLDER_PATH);

	{
		std::string folderPathChild = folderPath;

		std::vector<std::string> fileNames;

        GetFileNames(folderPathChild, fileNames);

		//指定のフォルダ内を調べる
		for (const auto& fname : fileNames)
		{
			// ファイルを開く
			FILE* fp = nullptr;

			std::string pathstr = folderPathChild + std::string("/") + fname;

			fopen_s(&fp, pathstr.c_str(), "rb");

			if (fp != nullptr)
			{
				Character::STATUS_DAT newStatusDat = {};
				newStatusDat.use = true;
				InitGameObjectStatus(&newStatusDat.status);

				{
					char cpytxt[Character::TXT_MAX] = {};
					char* cpytxtContext = nullptr;
					strcpy_s(cpytxt, Character::TXT_MAX, fname.c_str());

					char* token = strtok_s(cpytxt, ".", &cpytxtContext);
					if (token != NULL)
					{
						strcpy_s(newStatusDat.name, Character::TXT_MAX, cpytxt);
					}
				}

				char buf[Character::TXT_MAX] = {};
				while (fgets(buf, sizeof(buf), fp))
				{
					std::string buf2 = std::string(buf);
					buf2.erase(remove(buf2.begin(), buf2.end(), ' '), buf2.end());

					char cpytxt[Character::TXT_MAX] = {};
					char* cpytxtContext = nullptr;
					strcpy_s(cpytxt, Character::TXT_MAX, buf2.c_str());

					char* token = strtok_s(cpytxt, "//", &cpytxtContext);
					if (token != NULL && std::string(token).size() > 0)
					{
						std::string loadTestBuf = std::string(token);
						token = strtok_s(cpytxt, ":", &cpytxtContext);
						if (token != NULL && std::string(token).size() > 0)
						{
							std::string typeBuf = std::string(token);
							if (cpytxtContext != NULL && std::string(cpytxtContext).size() > 0)
							{
								std::string valueBuf = std::string(cpytxtContext);

                                if (typeBuf == "defaultColAtkName")
                                {
                                    newStatusDat.defaultColAtkName = NormalizeString(valueBuf);
                                }
                                if (typeBuf == "scale")
                                {
                                    newStatusDat.scale = (float)atof(valueBuf.c_str());
                                }
								if (typeBuf == "posX")
								{
									newStatusDat.pos.x = (float)atof(valueBuf.c_str());
								}
								if (typeBuf == "posY")
								{
									newStatusDat.pos.y = (float)atof(valueBuf.c_str());
								}
								if (typeBuf == "posZ")
								{
									newStatusDat.pos.z = (float)atof(valueBuf.c_str());
								}

								if (typeBuf == "hp")
								{
									newStatusDat.status.hpMax = atoi(valueBuf.c_str());
									newStatusDat.status.hp = newStatusDat.status.hpMax;
								}
								if (typeBuf == "fullName")
								{
									newStatusDat.status.fullName = valueBuf;
								}
								if (typeBuf == "nickName")
								{
									newStatusDat.status.nickName = valueBuf;
								}
								if (typeBuf == "family")
								{
									STR_SPRIT families = StrSplit(valueBuf, ',', true);

									for (const auto& fam : families)
									{
										std::string fam2 = fam;
										//fam2.erase(remove(fam2.begin(), fam2.end(), '\r'), fam2.end());
										//fam2.erase(remove(fam2.begin(), fam2.end(), '\n'), fam2.end());

										for (int i = 0; i < GAME_OBJECT_FAMILY_MAX; i++)
										{
											if (GetEnumName((GAME_OBJECT_FAMILY)i) 
												== (std::string(STR__GAME_OBJECT_FAMILY) + fam2))
											{
												newStatusDat.family[i] = true;
												break;
											}
										}
									}
									//newStatusDat.family = valueBuf;
								}
								if (typeBuf == "attributeDamageMod")
								{
									STR_SPRIT typeAndValue = StrSplit(valueBuf, ':', true);
									if (typeAndValue.size() >= 2)
									{
										for (int i = 0; i < GAME_OBJECT_COLLISION_ATTACK_ATTRIBUTE_MAX; i++)
										{
											if (GetEnumName((GAME_OBJECT_COLLISION_ATTACK_ATTRIBUTE)i)
												== (std::string(STR__GAME_OBJECT_COLLISION_ATTACK_ATTRIBUTE) + typeAndValue[0]))
											{
												newStatusDat.status.attributeMod[i] = (float)atof(typeAndValue[1].c_str());
												break;
											}
										}
									}
								}

								if (typeBuf == "statusType")
								{
									std::string fam2 = valueBuf;
									fam2.erase(remove(fam2.begin(), fam2.end(), '\r'), fam2.end());
									fam2.erase(remove(fam2.begin(), fam2.end(), '\n'), fam2.end());
									if (fam2.size() >= 1)
									{
										for (int i = 0; i < GAME_OBJECT_STATUS_TYPE_MAX; i++)
										{
											if (GetEnumName((GAME_OBJECT_STATUS_TYPE)i)
												== (std::string(STR__GAME_OBJECT_STATUS_TYPE) + fam2))
											{
												newStatusDat.status.statusType = (GAME_OBJECT_STATUS_TYPE)i;
												break;
											}
										}
									}
								}

								//DebugPrintf("test %s\n", loadTestBuf.c_str());
								//DebugPrintf("t %s  ", typeBuf.c_str());
								//DebugPrintf("v %s\n", valueBuf.c_str());
							}
						}
					}
				}
				statusDat.push_back(newStatusDat);

				// ファイルを閉じる
				fclose(fp);
			}
		}
	}
}