// ===================================================
// gameObject.cpp �Q�[���I�u�W�F�N�g����
// 
// hara sougo  2024/09/03
// ===================================================

#include "../../Core/DirectX/dx_camera.h"

#include "../../game_main.h"

#include "gameObject.h"
#include "gameObjectEventSetter.h"

#include "../../Game/Entities/Block/block.h"
#include "../Systems/collision.h"

#include "../Sprite/sprite.h" 
#include "../Systems/camera.h" 
#include "../Systems/font.h" 

#include "../../Game/Configuration/Param.h" 

#include "../../Scenes/SCENE_Game.h"
#include "../Game Stage/Stage_Base.h"


// ===================================================
// �}�N����`
// ===================================================



// ===================================================
// �\���̐錾
// ===================================================
struct GO_ATTACK_COLLISION_ADD_BUF {
	int frame;
	GAME_OBJECT* obj;
	GAME_OBJECT_COLLISION_ATTACK_CHILD col;

	bool finished;
};

// ===================================================
// �v���g�^�C�v�錾
// ===================================================
void gameObjectHit(GAME_OBJECT* gobj);
void gameObjectMove(GAME_OBJECT* gobj);
void gameObjectPosModification(GAME_OBJECT* gobj, GAME_OBJECT_POSMOD_RESULT* pmr);

void drawGameObject(GAME_OBJECT* gobj,
	SPRITE_GRAPH_DAT* sgd,
	Float2 startPos, float startRot, Float2 startScale,
	Float4 colorAdd,
	Float4 colorMul,
	bool outlineMode,
	bool shadowMode
);


void gameObjectAttackCollisionUpdate();

// ===================================================
// �O���[�o���ϐ�
// ===================================================
static GAME_OBJECT gameObjects[GAME_OBJECT_MAX] = {};	//����

//�I�u�W�F�N�g�̃K�[�h�{��
static float GAME_OBJECT_GUARD_MOD[GAME_OBJECT_GUARD_TYPE_MAX] = {};


static std::vector<GO_ATTACK_COLLISION_ADD_BUF> g_attackCollisionAddBuf = {};

static TEX_DAT g_ShadowTex = {};


// ===================================================
// ������
// ===================================================
void InitGameObjectStatus(GAME_OBJECT_STATUS* s)
{
	s->hp = 1;
	s->hpMax = 1;
	s->guardType = GAME_OBJECT_GUARD_TYPE_NORMAL;

	for (int i = 0; i < GAME_OBJECT_COLLISION_ATTACK_ATTRIBUTE_MAX; i++)
	{
		s->attributeMod[i] = 1.0f;
	}

	s->fullName.clear();
	s->fullName.shrink_to_fit();
	s->nickName.clear();
	s->nickName.shrink_to_fit();
}

void InitGameObject(GAME_OBJECT* obj)
{
	(*obj) = {};
	obj->frame /*		*/ = 0;
	obj->frameActive /*	*/ = 0;

	obj->pos /*			*/ = MkF3(0.0f, 0.0f, 0.0f);
	obj->oldpos /*		*/ = MkF3(0.0f, 0.0f, 0.0f);
	obj->zIndex /*		*/ = GAME_OBJECT_ZINDEX_OBJECT;
	obj->rot /*			*/ = 0.0f;
    obj->scale /*		*/ = MkF3(1.0f, 1.0f, 1.0f);
    //obj->scale /*		*/ = MkF2(1.0f, 1.0f);

	obj->vel /*			*/ = MkF3( 0.0f, 0.0f, 0.0f);
	obj->velImpact /*	*/ = MkF3( 0.0f, 0.0f, 0.0f);
	obj->friction  /*	*/ = 1.0f;
	obj->size /*		*/ = MkF2(PARAM_GAME_OBJECT_GENERAL_SIZE_X, PARAM_GAME_OBJECT_GENERAL_SIZE_Y);
	obj->use /*			*/ = false;

    obj->cameraInKeep_Index = -1;

    obj->ownerObjectIndex = -1;
	obj->hitstopOwnerObjectIndex = -1;
	obj->privateVariable = nullptr;

	{ //�X�e�[�^�X
		InitGameObjectStatus(&obj->status);
	}

	{ //�����蔻��
		GAME_OBJECT_COLLISION* c = &obj->collision;

		c->atk.enable = false;
		for (int i = 0; i < GAME_OBJECT_COLLISION_NUM_MAX; i++)
		{
			c->atk.collision[i].enable = false;
			c->atk.collision[i].pos = {};
			c->atk.collision[i].size.x = PARAM_GAME_OBJECT_GENERAL_COL_ATK_SIZE_X;
			c->atk.collision[i].size.y = PARAM_GAME_OBJECT_GENERAL_COL_ATK_SIZE_Y;
			c->atk.collision[i].size.z = PARAM_GAME_OBJECT_GENERAL_COL_ATK_SIZE_Z;
			c->atk.collision[i].power = PARAM_GAME_OBJECT_GENERAL_COL_ATK_POWER;
			c->atk.collision[i].hitStopAttacker = PARAM_GAME_OBJECT_GENERAL_COL_ATK_HITSTOP_ATK;
			c->atk.collision[i].hitStopReciever = PARAM_GAME_OBJECT_GENERAL_COL_ATK_HITSTOP_REC;
			c->atk.collision[i].hitCycle = PARAM_GAME_OBJECT_GENERAL_COL_ATK_HITCYCLE;
		}

		c->rec.enable = false;
		for (int i = 0; i < GAME_OBJECT_COLLISION_NUM_MAX; i++)
		{
			c->rec.collision[i].enable = false;
			c->rec.collision[i].pos = {};
			c->rec.collision[i].size.x = PARAM_GAME_OBJECT_GENERAL_COL_ATK_SIZE_X;
			c->rec.collision[i].size.y = PARAM_GAME_OBJECT_GENERAL_COL_ATK_SIZE_Y;
			c->rec.collision[i].size.z = PARAM_GAME_OBJECT_GENERAL_COL_ATK_SIZE_Z;
			c->rec.collision[i].guardMod = 1.0f;
			c->rec.collision[i].subjectFamily[GAME_OBJECT_FAMILY_ALL] = true;
		}
	}


	InitSpriteGraph(&obj->graphic.graph);

}
void InitGameObject(void)
{
	GAME_OBJECT_GUARD_MOD[GAME_OBJECT_GUARD_TYPE_NORMAL] /*	*/ = 1.0f;
	GAME_OBJECT_GUARD_MOD[GAME_OBJECT_GUARD_TYPE_SHORT] /*	*/ = 0.7f;
	GAME_OBJECT_GUARD_MOD[GAME_OBJECT_GUARD_TYPE_HALF] /*	*/ = 0.5f;
	GAME_OBJECT_GUARD_MOD[GAME_OBJECT_GUARD_TYPE_HARD] /*	*/ = 0.2f;
	GAME_OBJECT_GUARD_MOD[GAME_OBJECT_GUARD_TYPE_INV] /*	*/ = 0.0f;
	
	GAME_OBJECT_GUARD_MOD[GAME_OBJECT_GUARD_TYPE_WEAK] /*	*/ = 1.2f;
	GAME_OBJECT_GUARD_MOD[GAME_OBJECT_GUARD_TYPE_WEAKEX] /*	*/ = 1.5f;

    g_ShadowTex = LoadTexData("TEX/effect/shadow");

	for (int i = 0; i < GAME_OBJECT_MAX; i++)
	{
		InitGameObject(gameObjects + i);
	}
}

void InitGameObjectGraphic(GAME_OBJECT_GRAPHIC* gra)
{
	(*gra) = {};
	gra->enableShade = true;
    gra->shadowColor = MkF4(0.0f, 0.0f, 0.0f, 0.4f);
    gra->color = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
    gra->priority = DRAW_POLYGON_PRIORITY_NORMAL;
    gra->scale3d = MkF3(1.0f, 1.0f, 1.0f);
    
}

// ===================================================
// �X�V
// ===================================================
void UpdateGameObject(void)
{
	for (int i = 0; i < GAME_OBJECT_MAX; i++)
	{
		GAME_OBJECT* obj = gameObjects + i;
		if (obj->delRequ > 0)
		{
			obj->delRequ--;
			if (obj->delRequ <= 0)
			{
				obj->use = false;
				obj->graphic = {};
				obj->graphic.hidden = true;

				obj->tags.clear();
				obj->tags.shrink_to_fit();
			}
		}
	}

	gameObjectAttackCollisionUpdate();

	for (int i = 0; i < GAME_OBJECT_MAX; i++)
	{
		GAME_OBJECT* obj = gameObjects + i;
		if (IsActiveGameObject(obj))
		{
			bool vanishFlag = false;
			GAME_OBJECT_STATUS* status = &obj->status;

			if (obj->frame <= 0)
			{
				if (obj->events.source != nullptr)
				{ //�ŏ��̃t���[���Ŏ��s�����C�x���g
					obj->events.source->FirstFrame();
				}
			}

			if (!IsActiveGameObject(obj))
			{ //�L������Ȃ�������Ƃ΂�
				continue;
			}

			bool isStop = false;
			//�I�u�W�F�N�g���m�̓����蔻�菈��
			gameObjectHit(obj);


			if (obj->hitstopOwnerObjectIndex >= 0)
			{
				GAME_OBJECT* ownerObj = GetGameObject(obj->hitstopOwnerObjectIndex);
				if (!IsActiveGameObject(ownerObj))
				{
					obj->hitstopOwnerObjectIndex = -1;
				}
				else
				{
					obj->hitStop = ownerObj->hitStop;
				}
			}

			if (obj->hitReaction.frame > 0)
			{ //�q�b�g���A�N�V����
				obj->hitReaction.frame--;
			}

			if (obj->hitStop.frame > 0)
			{ //�q�b�g�X�g�b�v
				isStop = true;
				obj->hitStop.frame--;
			}
			else
			{ //�ʏ폈��

                { //�U������̎����J�E���g
                    GAME_OBJECT_COLLISION* oColA = &obj->collision;

                    if (oColA->atk.enable)
                    {
                        for (int i = 0; i < GAME_OBJECT_COLLISION_NUM_MAX; i++)
                        {
                            if (oColA->atk.collision[i].enable)
                            {
                                if (oColA->atk.collision[i].enableFrameMax > 0)
                                {
                                    if (oColA->atk.collision[i].enableFrame > 0)
                                    {
                                        oColA->atk.collision[i].enableFrame--;
                                        if (oColA->atk.collision[i].enableFrame <= 0)
                                        {
                                            oColA->atk.collision[i].enable = false;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

				//�O���t�B�b�N�̍X�V
				if (!gameObjects[i].graphic.stop)
				{
					UpdateSpriteGraph(&gameObjects[i].graphic.graph);
				}

				// �ړ�
				gameObjectMove(obj);

				if (obj->events.source != nullptr && IsActiveGameObject(obj))
				{ //�A�N�e�B�u�C�x���g
					obj->events.source->UpdateActive();
				}

                if (status->invFrame > 0)
                { //���G�J�E���g�����炷
                    status->invFrame--;
                }
                if (status->hitStopArmorFrame > 0)
                { //
                    status->hitStopArmorFrame--;
                }
                

				if (obj->lifetimeFrame > 0)
				{
					obj->lifetimeFrame--;
					if (obj->lifetimeFrame <= 0)
					{
						vanishFlag = true;
					}
				}
			}


			if (obj->events.source != nullptr && IsActiveGameObject(obj))
			{ //�X�V�C�x���g
				obj->events.source->Update();
			}

			if (status->hpMax > 0 
				&& !status->deadFlag && status->hp <= 0)
			{
				status->deadFlag = true;
				if (obj->events.source != nullptr && IsActiveGameObject(obj))
				{ //����C�x���g
					obj->events.source->Dead();
				}
			}

			if (vanishFlag)
			{ //�폜�t���O�������Ă��������
				DeleteGameObject(obj);
			}

			//�t���[���J�E���g
			if (!isStop)
			{
				obj->frameActive++;
			}
			obj->frame++;
		}
	}
}

//
// �Q�[���I�u�W�F�N�g���m�̓����蔻��
//
void gameObjectHit(GAME_OBJECT* gobj)
{
	GAME_OBJECT_COLLISION* oColA = &gobj->collision;

	if (!oColA->rec.enable)
	{ //���炢���肪�Ȃ������珈�����Ȃ�
		return;
	}

	//��������J�E���g
	for (int i = 0; i < GAME_OBJECT_HIT_CYCLE_NUM_MAX; i++)
	{
		if (oColA->colAtkCycleList[i].frame > 0)
		{
			oColA->colAtkCycleList[i].frame--;
		}
	}

	//�U������ɐG��Ă��邩���`�F�b�N
	for (int i = 0; i < GAME_OBJECT_MAX; i++)
	{
		GAME_OBJECT* ob = gameObjects + i;
		if (gobj != ob && IsActiveGameObject(ob))
		{

			GAME_OBJECT_COLLISION* oCol = &ob->collision;
			if (oCol->atk.enable)
			{ //�U�����肪��������`�F�b�N����

                bool ownerHit = false;
                GAME_OBJECT_COLLISION* attackerOCol = oCol;
                GAME_OBJECT* attackerChildOb = nullptr;
                GAME_OBJECT* attackerOb = ob;
                if (oCol->atk.ownerId.objCreateFrame > 0)
                {
                    GAME_OBJECT* atkObTmp = GetGameObjectFindByIdentifier(oCol->atk.ownerId);
                    if (IsActiveGameObject(atkObTmp))
                    {
                        attackerChildOb = attackerOb;
                        attackerOb = atkObTmp;
                        attackerOCol = &atkObTmp->collision;
                        ownerHit = atkObTmp == gobj;
                    }
                }

                if (ownerHit && !oCol->atk.isOwnerHit)
                {
                    continue;
                }

				//����������͂���
				//if (
    //                //CalculateDistance(
    //                //    ob->pos,
    //                //    gobj->pos
    //                //) > (SCREEN_WIDTH + SCREEN_HEIGHT) / 2
    //                CalculateDistance(
    //                    ob->pos,
    //                    gobj->pos
    //                ) > (SCREEN_WIDTH + SCREEN_HEIGHT) * 4
				//)
				//{
				//	continue;
				//}

				bool isNoHit = true;
				//for (int j = 0; j < GAME_OBJECT_HIT_CYCLE_NUM_MAX; j++)
				//{ //��������̃J�E���g���łȂ������`�F�b�N
				//	if (oColA->colAtkCycleList[j].frame > 0
				//		&& oColA->colAtkCycleList[j].objIndex == i
				//		&& oColA->colAtkCycleList[j].objCreateFrame == ob->createFrame)
				//	{
				//		isNoHit = false;
				//		break;
				//	}
				//}

				bool r_enable = false;
				//int r_power = 0;
				//int r_hitstopAtk = 0;
				//int r_hitstopRec = 0;
				//int r_hitCycle = 0;
				GAME_OBJECT_COLLISION_ATTACK_CHILD r_colAtk = {};
				GAME_OBJECT_COLLISION_RECIEVE_CHILD r_colRec = {};

				//�������Ă��邩���`�F�b�N
				if (isNoHit)
				{
					for (int j = 0; j < GAME_OBJECT_COLLISION_NUM_MAX; j++)
					{
						if (!oCol->atk.collision[j].enable)
						{
							continue;
						}
                        if (!attackerOCol->atk.collision[j].enable)
                        {
                            continue;
                        }


						bool through = false;

						for (int k = 0; k < GAME_OBJECT_HIT_CYCLE_NUM_MAX; k++)
						{ //��������̃J�E���g���łȂ������`�F�b�N
							if (oColA->colAtkCycleList[k].frame > 0
								//&& oColA->colAtkCycleList[k].objIdentifier.objIndex == i
								//&& oColA->colAtkCycleList[k].objIdentifier.objCreateFrame == ob->createFrame
								&& CheckIdentifierGameObject(attackerOb, oColA->colAtkCycleList[k].objIdentifier)

								&& oColA->colAtkCycleList[k].groupNumber == oCol->atk.collision[j].groupNumber
								&& oColA->colAtkCycleList[k].id == oCol->atk.collision[j].id
							)
							{
								through = true;
								break;
							}
						}
						//for (int k = 0; k < GAME_OBJECT_HIT_CHECK_NUM_MAX; k++)
						//{ //���肵�Ă����������`�F�b�N
						//	if (oColA->colAtkCheckList[k].objIndex == i
						//		&& oColA->colAtkCheckList[k].objCreateFrame == ob->createFrame

						//		&& oColA->colAtkCheckList[k].groupNumber == oCol->atk.collision[j].groupNumber
						//		&& oColA->colAtkCheckList[k].id == oCol->atk.collision[j].id)
						//	{
						//		through = true;
						//		break;
						//	}
						//}

						if (through)
						{
							continue;
						}

						bool brk = false;
						if (oColA->rec.enable)
						{
							for (int k = 0; k < GAME_OBJECT_COLLISION_NUM_MAX; k++)
							{
								if (!oColA->rec.collision[k].enable)
								{
									continue;
								}

								bool atkColSubjectFamily = false;
								bool recColSubjectFamily = false;
								if (oCol->atk.collision[j].subjectFamily[GAME_OBJECT_FAMILY_ALL])
								{
									atkColSubjectFamily = true;
								}
								else
								{
									for (int l = 0; l < GAME_OBJECT_FAMILY_MAX; l++)
									{
										if (l != GAME_OBJECT_FAMILY_ALL &&
											oCol->atk.collision[j].subjectFamily[l])
										{
											if (IsBelongsFamilyGameObject(gobj, (GAME_OBJECT_FAMILY)l))
											{
												atkColSubjectFamily = true;
												break;
											}
										}
									}
								}

								if (oColA->rec.collision[k].subjectFamily[GAME_OBJECT_FAMILY_ALL])
								{
									recColSubjectFamily = true;
								}
								else
								{
									for (int l = 0; l < GAME_OBJECT_FAMILY_MAX; l++)
									{
										if (l != GAME_OBJECT_FAMILY_ALL &&
											oColA->rec.collision[k].subjectFamily[l])
										{
											if (IsBelongsFamilyGameObject(ob, (GAME_OBJECT_FAMILY)l))
											{
												recColSubjectFamily = true;
												break;
											}
										}
									}
								}

								if (
									//(oCol->atk.collision[j].subjectFamily[GAME_OBJECT_FAMILY_VOID]
									//	|| oCol->atk.collision[j].subjectFamily[gobj->family])
									//&& (oColA->rec.collision[k].subjectFamily[GAME_OBJECT_FAMILY_VOID]
									//	|| oColA->rec.collision[k].subjectFamily[ob->family]))
									atkColSubjectFamily && recColSubjectFamily
									&& CheckBoxCollider(
										MkF2(ob->pos.x + oCol->atk.collision[j].pos.x, ob->pos.y + oCol->atk.collision[j].pos.y),
										MkF2(gobj->pos.x + oColA->rec.collision[k].pos.x, gobj->pos.y + oColA->rec.collision[k].pos.y),
										MkF2(oCol->atk.collision[j].size.x, oCol->atk.collision[j].size.y),
										MkF2(oColA->rec.collision[k].size.x, oColA->rec.collision[k].size.y))
									&& CheckBoxCollider(
										MkF2(ob->pos.z + oCol->atk.collision[j].pos.z, ob->pos.y + oCol->atk.collision[j].pos.y),
										MkF2(gobj->pos.z + oColA->rec.collision[k].pos.z, gobj->pos.y + oColA->rec.collision[k].pos.y),
										MkF2(oCol->atk.collision[j].size.z, oCol->atk.collision[j].size.y),
										MkF2(oColA->rec.collision[k].size.z, oColA->rec.collision[k].size.y))
									)
								{
									brk = true;
									r_enable = true;

									r_colAtk = oCol->atk.collision[j];
									r_colRec = oColA->rec.collision[k];

									//r_power = oCol->atk.collision[j].power;
									//r_hitstopAtk = oCol->atk.collision[j].hitStopAttacker;
									//r_hitstopRec = oCol->atk.collision[j].hitStopReciever;
									//r_hitCycle = oCol->atk.collision[j].hitCycle;


									break;
								}
							}
						}
						if (brk)
						{
							//brk = true;
							break;
						}
					}
				}

				//float rRecSizeR = oColA->rec.sizeR * ((gobj->scale.x + gobj->scale.y) / 2);
				//float rAtkSizeR = oCol->atk.sizeR * ((ob->scale.x + ob->scale.y) / 2);

				////�������Ă��邩���`�F�b�N
				//if (isNoHit
				//	&& (oCol->atk.subjectFamily[GAME_OBJECT_FAMILY_VOID] || oCol->atk.subjectFamily[gobj->family])
				//	&& (oColA->rec.subjectFamily[GAME_OBJECT_FAMILY_VOID] || oColA->rec.subjectFamily[ob->family])
				//	&& CheckCircleCollider(MkF2(gobj->pos.x, gobj->pos.y),
				//		MkF2(ob->pos.x, ob->pos.y),
				//		rRecSizeR, rAtkSizeR)
				//	&& CheckCircleCollider(MkF2(gobj->pos.z, gobj->pos.y),
				//		MkF2(ob->pos.z, ob->pos.y),
				//		rRecSizeR, rAtkSizeR)
				//)

				if(r_enable)
				{
                    {
                        GameObjectDamageCollisionHit(gobj, attackerOb,
                            r_colAtk,
                            r_colRec,
                            attackerChildOb
                        );
                    }

					//GAME_OBJECT_DAMAGE_RESULT dr = GameObjectDamage(gobj, ob, r_colAtk.power, r_colAtk, r_colRec);
					////dr.colAtk = oCol->atk;
					////dr.colRec = oCol->rec;

					//if (dr.damage > 0)
					//{ //�q�b�g���̉��o

					//	//�_���[�W�ɔ{�����������Ă����ꍇ�q�b�g�X�g�b�v�ɂ��␳��������
					//	float hsmod = (float)(dr.mod > 1.0f ? 1.0f : dr.mod);
					//	int hsA = (int)(r_colAtk.hitStopAttacker);
					//	int hsR = (int)(r_colAtk.hitStopReciever * hsmod);
					//	GameObjectHitStop(ob, hsA);
					//	GameObjectHitStop(gobj, hsR);
					//	GameObjectHitReaction(gobj, hsR + GAME_OBJECT_HIT_REACTION_MIN);
					//}

					//for (int j = 0; j < GAME_OBJECT_HIT_CYCLE_NUM_MAX; j++)
					//{ //����������Z�b�g
					//	if (oColA->colAtkCycleList[j].frame <= 0)
					//	{
					//		oColA->colAtkCycleList[j].objIndex = i;
					//		oColA->colAtkCycleList[j].objCreateFrame = ob->createFrame;

					//		oColA->colAtkCycleList[j].id = r_colAtk.id;
					//		oColA->colAtkCycleList[j].groupNumber = r_colAtk.groupNumber;

					//		oColA->colAtkCycleList[j].frameMax = r_colAtk.hitCycle;
					//		oColA->colAtkCycleList[j].frame = oColA->colAtkCycleList[j].frameMax;
					//		break;
					//	}
					//}

					//{
					//	int indexTmp = 0;
					//	unsigned int fTmp = -1;
					//	for (int j = 0; j < GAME_OBJECT_HIT_CHECK_NUM_MAX; j++)
					//	{
					//		if (oColA->colAtkCheckList[j].hitFrame < fTmp)
					//		{
					//			indexTmp = j;
					//			fTmp = oColA->colAtkCheckList[j].hitFrame;
					//		}
					//	}

					//	{
					//		oColA->colAtkCheckList[indexTmp].hitFrame = GetMainGameFrame();

					//		oColA->colAtkCheckList[indexTmp].id = r_colAtk.id;
					//		oColA->colAtkCheckList[indexTmp].groupNumber = r_colAtk.groupNumber;
					//		oColA->colAtkCheckList[indexTmp].objIndex = i;
					//		oColA->colAtkCheckList[indexTmp].objCreateFrame = ob->createFrame;
					//	}
					//}

					//if (ob->events.DamageAttacker != nullptr)
					//{ //�_���[�W�C�x���g
					//	ob->events.DamageAttacker(i, dr);
					//}
					//if (gobj->events.DamageReciever != nullptr)
					//{ //�_���[�W�C�x���g
					//	gobj->events.DamageReciever((int)(gobj - gameObjects), dr);
					//}
				}
			}
		}
	}
}

//
// �ړ��Ɋւ��鏈��
//
void gameObjectMove(GAME_OBJECT* gobj)
{
	GAME_OBJECT_POSMOD_RESULT pmr = {};

	GAME_OBJECT* obj = gobj;

	// �ύX�O�̍��W���i�[
	obj->oldpos = obj->pos;

	if (gobj->ownerObjectIndex >= 0)
	{
		GAME_OBJECT* ownerObj = GetGameObject(gobj->ownerObjectIndex);
		if (!IsActiveGameObject(ownerObj))
		{
			gobj->ownerObjectIndex = -1;
		}
		else
		{
			gobj->pos = ownerObj->pos;
		}
	}
	else
	{
		if (obj->moveRequ.enable)
		{ //�����ړ�
			float mrModTmp = (float)obj->moveRequ.frame / obj->moveRequ.frameMax;

			float mrModRvTmp = (1.0f - mrModTmp);

			float mrModEasXTmp = mrModRvTmp;
			float mrModEasYTmp = mrModRvTmp;
			float mrModEasZTmp = mrModRvTmp;

			if (obj->moveRequ.easingX != nullptr)
			{
				mrModEasXTmp = obj->moveRequ.easingX(mrModRvTmp);
			}
			if (obj->moveRequ.easingY != nullptr)
			{
				mrModEasYTmp = obj->moveRequ.easingY(mrModRvTmp);
			}
			if (obj->moveRequ.easingZ != nullptr)
			{
				mrModEasZTmp = obj->moveRequ.easingZ(mrModRvTmp);
			}


			float setXtmp = obj->moveRequ.startPos.x
				+ (obj->moveRequ.endPos.x - obj->moveRequ.startPos.x) * mrModEasXTmp;
			float setYtmp = obj->moveRequ.startPos.y
				+ (obj->moveRequ.endPos.y - obj->moveRequ.startPos.y) * mrModEasYTmp;
			float setZtmp = obj->moveRequ.startPos.z
				+ (obj->moveRequ.endPos.z - obj->moveRequ.startPos.z) * mrModEasZTmp;

            pmr.beforeVel = obj->vel;

			if (obj->moveRequ.isBlockCollisionIgnore)
			{ //�u���b�N����𖳎�����ꍇ
				obj->pos.x = setXtmp;
				obj->pos.y = setYtmp;
				obj->pos.z = setZtmp;
			}
			else
			{ //�u���b�N������s���ꍇ
				float addxtmp = setXtmp - obj->moveRequ.oldPos.x;
				float addytmp = setYtmp - obj->moveRequ.oldPos.y;
				float addztmp = setZtmp - obj->moveRequ.oldPos.z;

				obj->pos.x += addxtmp;
				gameObjectPosModification(obj, &pmr);
				obj->pos.y += addytmp;
				gameObjectPosModification(obj, &pmr);
				obj->pos.z += addztmp;
				//gameObjectPosModification(obj);
			}

			obj->moveRequ.oldPos = MkF3(setXtmp, setYtmp, setZtmp);
			obj->vel.x = obj->pos.x - obj->oldpos.x;
			obj->vel.y = obj->pos.y - obj->oldpos.y;
			obj->vel.z = obj->pos.z - obj->oldpos.z;


			if (obj->moveRequ.frame <= 0)
			{
				obj->moveRequ.enable = false;
			}
			else
			{
				obj->moveRequ.frame--;
			}
		}
		else
		{ //�ʏ�
			if (obj->gravity != 0.0f)
			{
				obj->isGround = false;
				obj->gravityVel += obj->gravity;
				if (obj->gravityVel > GAME_OBJECT_GRAVITY_MAX)
				{
					obj->gravityVel = GAME_OBJECT_GRAVITY_MAX;
				}
				obj->pos.y += obj->gravityVel;
				gameObjectPosModification(obj, &pmr);
			}

			// ��R��
			obj->velImpact.x += -1 * obj->velImpact.x * obj->friction;
			obj->velImpact.y += -1 * obj->velImpact.y * obj->friction;

			obj->vel.x += -1 * obj->vel.x * obj->friction;
			obj->vel.y += -1 * obj->vel.y * obj->friction;

            pmr.beforeVel = obj->vel;

			//���W�ύX
			obj->pos.x += obj->vel.x + obj->velImpact.x;
			gameObjectPosModification(obj, &pmr);

			obj->pos.y += obj->vel.y + obj->velImpact.y;
			gameObjectPosModification(obj, &pmr);
		}
	}


	//�J�����O�ɏo�Ȃ��悤�ɂ���
    if (gobj->cameraInKeep_Index >= 0)
    //if (gobj->cameraInKeep)
	{
        if (!IsCameraInGameObject(gobj->cameraInKeep_Index, gobj))
        //if (!IsCameraInGameObject(gobj))
		{
			Float2 rSize = MkF2(gobj->size.x * gobj->scale.x, gobj->size.y * gobj->scale.y);
			// �㉺���E�̓�������������o���邽�߁Aold��p�����`�F�b�N���s��
			// �Q�[���I�u�W�F�N�g��4�ӂ��v�Z
			Float2 ObjPos2d = MkF2(gobj->pos.x, gobj->pos.y);
			float ObjTop = ObjPos2d.y - rSize.y / 2;
			float ObjBottom = ObjPos2d.y + rSize.y / 2;
			float ObjRight = ObjPos2d.x + rSize.x / 2;
			float ObjLeft = ObjPos2d.x - rSize.x / 2;

			// �Q�[���I�u�W�F�N�g��oldpos��4�ӂ��v�Z
			Float2 ObjOldPos2d = MkF2(gobj->oldpos.x, gobj->oldpos.y);
			float ObjOldTop = ObjOldPos2d.y - rSize.y / 2;
			float ObjOldBottom = ObjOldPos2d.y + rSize.y / 2;
			float ObjOldRight = ObjOldPos2d.x + rSize.x / 2;
			float ObjOldLeft = ObjOldPos2d.x - rSize.x / 2;


			// block��4�ӂ��v�Z
			Float3 cpos = GetCameraPos(0);
			Float2 csize = GetCameraSize(0);
			float BlockTop = cpos.y - csize.y / 2;
			float BlockBottom = cpos.y + csize.y / 2;
			float BlockRight = cpos.x + csize.x / 2;
			float BlockLeft = cpos.x - csize.x / 2;

			if (ObjTop > BlockBottom && ObjOldTop <= BlockBottom)
			{
				// �Q�[���I�u�W�F�N�g�̏ꏊ���Œ�
				gobj->pos.y = BlockBottom - 1.0f;

				//pmr.enable = true;
				//pmr.down += abs(ObjTop - BlockBottom);
			}

			if (ObjBottom < BlockTop && ObjOldBottom >= BlockTop)
			{

				gobj->pos.y = BlockTop + 1.0f;
			}

			if (ObjRight < BlockLeft && ObjOldRight >= BlockLeft)
			{
				// �Q�[���I�u�W�F�N�g�̏ꏊ���Œ� 
				gobj->pos.x = BlockLeft + 1.0f;
			}

			if (ObjLeft > BlockRight && ObjOldLeft <= BlockRight)
			{
				// �Q�[���I�u�W�F�N�g�̏ꏊ���Œ� 
				gobj->pos.x = BlockRight - 1.0f;
			}
			//pmr->enable = true;
			//gobj->pos = gobj->oldpos;

		}
	}


	if (pmr.enable)
	{
		if (gobj->events.source != nullptr)
		{ //�u���b�N�q�b�g�C�x���g
			gobj->events.source->BlockHit(pmr);
		}
	}

	if (obj->pos.x != obj->oldpos.x
		|| obj->pos.y != obj->oldpos.y
		|| obj->pos.z != obj->oldpos.z)
	{
		obj->isMoving = true;
	}
	else
	{
		obj->isMoving = false;
	}
}

//
// ���W�ύX�� �ʒu�̏C�����s���֐�
//
void gameObjectPosModification(GAME_OBJECT* gobj, GAME_OBJECT_POSMOD_RESULT* pmr)
{

	if (gobj->blockCollisionIgnore 
		|| (gobj->size.x <= 0.0f && gobj->size.y <= 0.0f)
	)
	{
		return;
	}

    {
        // �u���b�N�̐擪�A�h���X���擾
        BLOCK* pBlock = GetBlocks();

        // �u���b�N�̌����`�F�b�N
        for (int BlockCnt = 0; BlockCnt < BLOCK_MAX; BlockCnt++)
        {
            BLOCK* pBlockThis = &pBlock[BlockCnt];

            if (pBlockThis->isUse)
            {
                if (gobj->blockCollisionIgnoreFunc != nullptr
                    && gobj->blockCollisionIgnoreFunc(
                        GetGameObjectIndex(gobj),
                        BlockCnt
                    )
                    )
                {
                    continue;
                }

                Float2 rSize = MkF2(gobj->size.x * gobj->scale.x, gobj->size.y * gobj->scale.y);
                if (pBlockThis->noCollision
                    || (pBlockThis->size.x <= 0.0f && pBlockThis->size.y <= 0.0f)
                    || (
                        CalculateDistance(
                            Float2ToFloat3(pBlockThis->pos),
                            gobj->pos
                        ) > std::max(std::max(pBlockThis->size.x, pBlockThis->size.y), std::max(rSize.x, rSize.y)) * 2.0f
                        )
                    )
                {
                    continue;
                }

                // �㉺���E�̓�������������o���邽�߁Aold��p�����`�F�b�N���s��
                // �Q�[���I�u�W�F�N�g��4�ӂ��v�Z
                Float2 ObjPos2d = MkF2(gobj->pos.x, gobj->pos.y);
                float ObjTop = ObjPos2d.y - rSize.y / 2;
                float ObjBottom = ObjPos2d.y + rSize.y / 2;
                float ObjRight = ObjPos2d.x + rSize.x / 2;
                float ObjLeft = ObjPos2d.x - rSize.x / 2;

                // �Q�[���I�u�W�F�N�g��oldpos��4�ӂ��v�Z
                Float2 ObjOldPos2d = MkF2(gobj->oldpos.x, gobj->oldpos.y);
                float ObjOldTop = ObjOldPos2d.y - rSize.y / 2;
                float ObjOldBottom = ObjOldPos2d.y + rSize.y / 2;
                float ObjOldRight = ObjOldPos2d.x + rSize.x / 2;
                float ObjOldLeft = ObjOldPos2d.x - rSize.x / 2;


                // block��4�ӂ��v�Z
                float BlockTop = pBlockThis->pos.y - pBlockThis->size.y / 2;
                float BlockBottom = pBlockThis->pos.y + pBlockThis->size.y / 2;
                float BlockRight = pBlockThis->pos.x + pBlockThis->size.x / 2;
                float BlockLeft = pBlockThis->pos.x - pBlockThis->size.x / 2;


                // �������Ă���i�㉺����p�j
                if (abs(abs(pBlockThis->pos.x - ObjPos2d.x) - (pBlockThis->size.x / 2 + rSize.x / 2)) > GAME_OBJECT_BLOCK_HIT_IGNORE //�����|����΍�
                    && CheckBoxCollider(ObjPos2d, pBlockThis->pos, rSize, pBlockThis->size))
                {

                    // �������ɓ�������
                    if (ObjOldTop >= BlockBottom && ObjTop <= BlockBottom)
                    {// �Q�[���I�u�W�F�N�g�̏�ƃQ�[���I�u�W�F�N�g�̉����ׁA�Q�[���I�u�W�F�N�g�̏オold�̎��͉����ɂ���A���݂͏㑤�ɂ���ꍇ
                        // �~�߂�
                        gobj->vel.y = 0.0f;

                        // �Q�[���I�u�W�F�N�g�̏ꏊ���Œ�
                        //gobj->pos.y = BlockBottom + rSize.y / 2 + abs(gobj->oldpos.y - gobj->pos.y) + 1.5f;
                        gobj->pos.y = BlockBottom + rSize.y / 2 + 0.1f;

                        pmr->enable = true;
                        pmr->up += abs(BlockBottom - ObjTop);
                    }

                    // �ォ�牺�ɓ��������i������j
                    if (ObjOldBottom <= BlockTop && ObjBottom >= BlockTop)
                    {// �Q�[���I�u�W�F�N�g�̉��ƃu���b�N�̏���ׁA�Q�[���I�u�W�F�N�g�̉���old�̎��͏㑤�ɂ���A���݂͉����ɂ���ꍇ
                        // ���n
                        gobj->vel.y = 0.0f;
                        gobj->gravityVel = 0.0f;

                        // �Q�[���I�u�W�F�N�g�̏ꏊ���Œ�i�n�ʂɈ���������Ȃ��悤�ɂ��邽��0.1f�����グ��j
                        //gobj->pos.y = BlockTop - rSize.y / 2 - 0.1f;
                        //gobj->pos.y = BlockTop - rSize.x / 2 - abs(gobj->oldpos.y - gobj->pos.y) - 1.5f;
                        gobj->pos.y = BlockTop - rSize.x / 2 - 0.1f;

                        // ���n�����ꍇ�W�����v�t���O��܂�
                        /*if (oldjump)
                        {
                            player.jump = false;
                        }*/

                        //���n�t���O
                        gobj->isGround = true;

                        pmr->enable = true;
                        pmr->down += abs(BlockTop - ObjBottom);
                    }
                }

                // �������Ă���i���E����p�j
                if (abs(abs(pBlockThis->pos.y - ObjPos2d.y) - (pBlockThis->size.y / 2 + rSize.y / 2)) > GAME_OBJECT_BLOCK_HIT_IGNORE //�����|����΍�
                    && CheckBoxCollider(ObjPos2d, pBlockThis->pos, rSize, pBlockThis->size))
                {
                    // ������E�ɓ�������
                    if (ObjOldRight <= BlockLeft && ObjRight >= BlockLeft)
                    {// �Q�[���I�u�W�F�N�g�̉E�ƃu���b�N�̍����ׁA�Q�[���I�u�W�F�N�g�̉E��old�̎��͍����ɂ���A���݂͉E���ɂ���ꍇ
                        // �~�߂�
                        gobj->vel.x = 0.0f;

                        // �Q�[���I�u�W�F�N�g�̏ꏊ���Œ� 
                        //gobj->pos.x = BlockLeft - rSize.x / 2 - abs(gobj->oldpos.x - gobj->pos.x) - 1.5f;
                        gobj->pos.x = BlockLeft - rSize.x / 2 - 0.1f;

                        pmr->enable = true;
                        pmr->right += abs(BlockLeft - ObjRight);
                    }

                    // �E���獶�ɓ�������
                    if (ObjOldLeft >= BlockRight && ObjLeft <= BlockRight)
                    {// �Q�[���I�u�W�F�N�g�̍��ƃu���b�N�̉E���ׁA�Q�[���I�u�W�F�N�g�̍���old�̎��͉E���ɂ���A���݂͍����ɂ���ꍇ
                        // �~�߂�
                        gobj->vel.x = 0.0f;

                        // �Q�[���I�u�W�F�N�g�̏ꏊ���Œ� 
                        //gobj->pos.x = BlockRight + rSize.x / 2 + abs(gobj->oldpos.x - gobj->pos.x) + 1.5f;
                        gobj->pos.x = BlockRight + rSize.x / 2 + 0.1f;

                        pmr->enable = true;
                        pmr->left += abs(BlockRight - ObjLeft);
                    }
                }

            }
        }
    }

    {
        for (int i = 0; i < GAME_OBJECT_MAX; i++)
        {
            GAME_OBJECT* obj = GetGameObjects() + i;
            if (IsActiveGameObject(obj) && obj != gobj)
            {
                if (obj->collider.type == GAME_OBJECT_COLLIDER_TYPE_CAPSULE)
                {
                    Float3 sphereCenter = {
                        gobj->pos.x,
                        gobj->pos.y,
                        gobj->pos.z,
                    };
                    float sphereRadius = (
                        gobj->size.x * gobj->scale.x +
                        gobj->size.y * gobj->scale.y
                    ) / 2.0f;

                    FloatCapsule capsule = obj->collider.capsule;
                    capsule.pointA.x *= obj->scale.x;
                    capsule.pointA.y *= obj->scale.y;
                    capsule.pointA.z *= obj->scale.z;
                    capsule.pointB.x *= obj->scale.x;
                    capsule.pointB.y *= obj->scale.y;
                    capsule.pointB.z *= obj->scale.z;

                    capsule.pointA.x += obj->pos.x;
                    capsule.pointA.y += obj->pos.y;
                    capsule.pointA.z += obj->pos.z;
                    capsule.pointB.x += obj->pos.x;
                    capsule.pointB.y += obj->pos.y;
                    capsule.pointB.z += obj->pos.z;

                    bool result = ResolveSphereCapsuleCollision(
                        sphereCenter, sphereRadius,
                        capsule
                    );
                    if (result)
                    {
                        if (sphereCenter.x != gobj->pos.x)
                        {
                            //gobj->vel.x = 0.0f;

                            gobj->pos.x = sphereCenter.x;
                        }
                        if (sphereCenter.y != gobj->pos.y)
                        {
                            //gobj->vel.y = 0.0f;

                            gobj->pos.y = sphereCenter.y;
                        }
                        if (sphereCenter.z != gobj->pos.z)
                        {
                            //gobj->vel.z = 0.0f;

                            gobj->pos.z = sphereCenter.z;
                        }

                        pmr->enable = true;
                        pmr->objIndex = i;
                    }
                }
            }
        }
    }
}




// ===================================================
// �Q�[���I�u�W�F�N�g�̕`��
// ===================================================
void DrawGameObject(void)
{
	//�ԍ����X�g�̃������m��
	//int* indexList = (int*)malloc(sizeof(int) * GAME_OBJECT_MAX);
	static int indexList[GAME_OBJECT_MAX] = {};

	if (indexList == nullptr)
	{
#ifdef SWITCH_MODE
#else
		MessageBox(NULL, "���������m�ۂł��Ȃ�����", "�G���[", MB_OK);
#endif
		return;
	}

	//�ԍ����X�g�̔z���p��
	for (int i = 0; i < GAME_OBJECT_MAX; i++)
	{
		indexList[i] = i;
	}

	//�ԍ����X�g�̔z���
	//�\���ԍ��̍~���ɕ��ёւ���
	//std::sort(indexList,
	//	indexList + GAME_OBJECT_MAX,
	//	[](const int& a, const int& b) {
	//		//zIndex����ɕ��ёւ��A
	//		//������������z���̍��W����ɕ��ёւ���
	//		return (gameObjects[a].zIndex == gameObjects[b].zIndex ?
	//			gameObjects[a].pos.z > gameObjects[b].pos.z
	//			: gameObjects[a].zIndex > gameObjects[b].zIndex);
	//	}
	//);
	std::sort(indexList,
		indexList + GAME_OBJECT_MAX,
		[](const int& a, const int& b) {
			//z���̍��W����ɕ��ёւ��A
			//(�ق�)������������zIndex����ɕ��ёւ���
			return (abs(gameObjects[a].pos.z - gameObjects[b].pos.z) < 0.05f ?
				gameObjects[a].zIndex > gameObjects[b].zIndex
				: gameObjects[a].pos.z > gameObjects[b].pos.z);
		}
	);

	//�A�E�g���C��
	//for (int i = 0; i < GAME_OBJECT_MAX; i++)
	//{
	//	int indexTmp = indexList[i];
	//	if (IsActiveGameObject(gameObjects + indexTmp))
	//	{
	//		drawGameObject(
	//			gameObjects + indexTmp,
	//			&gameObjects[indexTmp].graphic.graph,
	//			MkF2(0.0f, 0.0f),
	//			0.0f,
	//			MkF2(1.0f, 1.0f),
	//			MkF4(0.0f, 0.0f, 0.0f, 0.0f),
	//			MkF4(1.0f, 1.0f, 1.0f, 1.0f),
	//			true,
	//			false
	//		);
	//	}
	//}
	
	//�{��
	for (int i = 0; i < GAME_OBJECT_MAX; i++)
	{
		int indexTmp = indexList[i];
		if (IsActiveGameObject(gameObjects + indexTmp))
		{
            if (gameObjects[indexTmp].frame <= 0)
            {
                continue;
            }

            DrawGameObjectData(gameObjects + indexTmp);

			if (gameObjects[indexTmp].events.source != nullptr)
			{ //�`��C�x���g
				gameObjects[indexTmp].events.source->Draw();
			}
		}
	}

	//�������J��
	//free(indexList);
}

void DrawGameObjectData(GAME_OBJECT* obj)
{
	//bool deSave = DXDepthStencilStateDepthEnable();
	//if (deSave)
	//{
	//	drawGameObject(
	//		obj,
	//		&obj->graphic.graph,
	//		MkF2(0.0f, 0.0f),
	//		0.0f,
	//		MkF2(1.0f, 1.0f),
	//		MkF4(0.0f, 0.0f, 0.0f, 0.0f),
	//		MkF4(1.0f, 1.0f, 1.0f, 1.0f),
	//		false,
	//		true
	//	);
	//}
	drawGameObject(
		obj,
		&obj->graphic.graph,
		MkF2(0.0f, 0.0f),
		0.0f,
		MkF2(1.0f, 1.0f),
		MkF4(0.0f, 0.0f, 0.0f, 0.0f),
		MkF4(1.0f, 1.0f, 1.0f, 1.0f),
		false,
		false
	);
}


void drawGameObject(GAME_OBJECT* gobj,
	SPRITE_GRAPH_DAT* sgd,
	Float2 startPos, float startRot, Float2 startScale,
	Float4 colorAdd,
	Float4 colorMul,
	bool outlineMode,
	bool shadowMode
)
{
    if (!gobj->graphic.hidden)
    { //��\���������ꍇ���s���Ȃ�

        Float4 colorAddTmp = MkF4(0.0f, 0.0f, 0.0f, 0.0f);
        Float4 colorMulTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);

        Float3 posTmp = gobj->pos;

        float rotTmp = gobj->rot;

        Float3 scaTmp = gobj->scale;

        //if (outlineMode)
        //{
        //	//colorAddTmp = MkF4(-0.3f, -0.3f, -0.3f, 0.0f);
        //	//colorMulTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
        //}

        if (shadowMode)
        {
            colorAddTmp = MkF4(-1.0f, -1.0f, -1.0f, -0.2f);
            //colorMulTmp = MkF4(1.0f, 1.0f, 1.0f, 1.0f);
        }

        bool flashing = false;

        //if (!shadowMode && !outlineMode && gobj->status.invFrame > 1)
        //{ //�҂��҂����点��
        //    int ftmp = gobj->status.invFrame % 6;
        //    if (ftmp < 3)
        //    {
        //        colorAddTmp = MkF4(0.5f, 0.5f, 0.5f, 0.0f);
        //        colorMulTmp = MkF4(1.0f, 1.0f, 1.0f, 0.5f);
        //        flashing = true;
        //    }
        //}

        if (gobj->hitReaction.frame > 0 && gobj->hitReaction.frame < gobj->hitReaction.frameMax - 1)
        { //�q�b�g���A�N�V�����̏���

            //CAMERA ciTmp = GetCameraInfo(); //�J�����̏��擾

            //�i�݋
            float hrModTmp = (float)gobj->hitReaction.frame / gobj->hitReaction.frameMax;

            //�҂��҂����点��
            if (!shadowMode && !outlineMode)
            {
                int ftmp = gobj->hitReaction.frame % 6;
                if (ftmp < 2)
                {
                    //colorAddTmp = MkF4(10.0f, -10.0f, -10.0f, 0.0f);
                    colorAddTmp = MkF4(10.0f, 10.0f, 10.0f, 0.0f);
                    //colorMulTmp = MkF4(2.0f, 2.0f, 2.0f, 1.0f);
                    flashing = true;
                }
                if (ftmp >= 4)
                {
                    colorAddTmp = MkF4(10.0f, -10.0f, -10.0f, 0.0f);
                    //colorAddTmp = MkF4(0.0f, -1.0f, -1.0f, 0.0f);
                    //colorAddTmp = MkF4(-10.0f, -10.0f, -10.0f, 0.0f);
                    //colorMulTmp = MkF4(0.0f, 0.0f, 0.0f, 1.0f);
                    flashing = true;
                }
            }

            //�h�炷
            //�h��͏��X�Ɏ�������
            //�J���������������ꍇ�͂��傫��������
            //posTmp.x += sin(6.4f * gobj->hitReaction.frame) * hrModTmp * (1.0f / ciTmp.trans.zoom) * 9.0f
            //posTmp.x += sin(3.4f * gobj->hitReaction.frame) * hrModTmp * (1.0f / ciTmp.trans.zoom) * 22.0f;
            posTmp.x += sin(1.2f * gobj->hitReaction.frame) * hrModTmp * 100.0f;
            //if (!gobj->isGround)
            //{
            //	//posTmp.y += cos(7.5f * gobj->hitReaction.frame) * hrModTmp * (1.0f / ciTmp.trans.zoom) * 9.0f;
            //	posTmp.y += cos(4.5f * gobj->hitReaction.frame) * hrModTmp * (1.0f / ciTmp.trans.zoom) * 28.0f;
            //}
        }

        posTmp.x += startPos.x;
        posTmp.y += startPos.y;

        rotTmp += startRot;

        scaTmp.x *= startScale.x;
        scaTmp.y *= startScale.y;

        //{
        //	float zposmod = 1.0f + (posZTmp * -1 / 1280);
        //	if (zposmod < 0.01f)
        //	{
        //		zposmod = 0.01f;
        //	}
        //	scaTmp.x *= zposmod;
        //	scaTmp.y *= zposmod;
        //}

        colorAddTmp.x += colorAdd.x;
        colorAddTmp.y += colorAdd.y;
        colorAddTmp.z += colorAdd.z;
        colorAddTmp.w += colorAdd.w;

        colorMulTmp.x *= colorMul.x;
        colorMulTmp.y *= colorMul.y;
        colorMulTmp.z *= colorMul.z;
        colorMulTmp.w *= colorMul.w;

        colorMulTmp.x *= gobj->graphic.color.x;
        colorMulTmp.y *= gobj->graphic.color.y;
        colorMulTmp.z *= gobj->graphic.color.z;
        colorMulTmp.w *= gobj->graphic.color.w;

        //����
        //colorMulTmp.w *= 0.4f;

        bool deSave = DXDepthStencilStateDepthEnable();

        Float3 pos3DRaw = gobj->graphic.pos3d;
        pos3DRaw.x *= gobj->scale.x;
        pos3DRaw.y *= gobj->scale.y;
        pos3DRaw.z *= gobj->scale.y;

        Float3 rot3dGra = gobj->graphic.rot3d;
        Float3 scale3DRaw = gobj->graphic.scale3d;

        Float3 pos3d = posTmp;
        Float3 rot3d = MkF3(0.0f, 0.0f, rotTmp);

        Float3 posAdd3d = {};

        if (shadowMode)
        {
            if (deSave)
            {
                //posAdd3d.y += -gobj->size.y * 0.75f;
                //posAdd3d.y += -gobj->size.y * 0.625f;
                posAdd3d.y += -gobj->size.y * 0.499f;
            }

            posAdd3d.z += 0.1f;
        }
        else if (outlineMode)
        {
            if (deSave)
            {
                float rad = CalculateDegToRad(rot3dGra.x);
                posAdd3d.y = -sin(rad) * 6.0f;
                posAdd3d.z = cos(rad) * 6.0f;

                //posAdd3d.y += -gobj->size.y * 0.125f;
            }
            posAdd3d.x += pos3DRaw.x;
            posAdd3d.y += pos3DRaw.y;
            posAdd3d.z += pos3DRaw.z;

            rot3d.x += rot3dGra.x;
            rot3d.y += rot3dGra.y;
            rot3d.z += rot3dGra.z;
            //if (gobj->family[GAME_OBJECT_FAMILY_PLAYER])
            //{
            //	DebugPrintf("%f, %f\n", posAdd3d.y, posAdd3d.z);
            //}
        }
        else
        {
            if (deSave)
            {
                //posAdd3d.y += -gobj->size.y * 0.125f;
            }
            posAdd3d.x += pos3DRaw.x;
            posAdd3d.y += pos3DRaw.y;
            posAdd3d.z += pos3DRaw.z;

            rot3d.x += rot3dGra.x;
            rot3d.y += rot3dGra.y;
            rot3d.z += rot3dGra.z;
        }
        pos3d.x += posAdd3d.x;
        pos3d.y += posAdd3d.y;
        pos3d.z += posAdd3d.z;

        //DrawSpriteGraph(
        //	sgd,
        //	//MkF3(posTmp.x, posTmp.y, (outlineMode ? 0.1f : -gobj->size.y / 2 * gobj->scale.y)),
        //	pos3d,
        //	//MkF3(0.0f + (outlineMode ? 0.0f : 50.0f), 0.0f, rotTmp),
        //	rot3d,
        //	scaTmp,
        //	colorAddTmp,
        //	colorMulTmp,
        //	((shadowMode || outlineMode || flashing || gobj->graphic.gray) ? LOADTEXTURETYPE_GRAY : LOADTEXTURETYPE_MAIN),
        //	false,
        //	outlineMode
        //);
        {
            DRAW_SPRITE_GRAPH_OPTION optTmp = {};
            optTmp.startPos = pos3d;
            optTmp.startRot = rot3d;
            //if (gobj->graphic.billboard)
            //{
            //	DX_CAMERA_INFO cinfo = DX_GetCameraInfo();
            //	optTmp.startRot.x = cinfo.CameraVector.x * -90.0f;
            //	optTmp.startRot.y = cinfo.CameraVector.y * -90.0f;
            //	optTmp.startRot.z = 0.0f;
            //	//optTmp.startRot.z = cinfo.CameraVector.z * -90.0f;
            //}
            optTmp.startScale = Float3ToFloat2(scaTmp);
            optTmp.startScale.x *= scale3DRaw.x;
            optTmp.startScale.y *= scale3DRaw.y;

            optTmp.colorAdd = colorAddTmp;
            optTmp.colorMul = colorMulTmp;

            optTmp.loadTexType = LOADTEXTURETYPE_MAIN;
            if (shadowMode || outlineMode || flashing || gobj->graphic.gray)
            {
                optTmp.loadTexType = LOADTEXTURETYPE_GRAY;
            }

            optTmp.fixed = false;
            optTmp.outlineMode = outlineMode;
            optTmp.billboard = gobj->graphic.billboard;

            optTmp.enableShade = gobj->graphic.enableShade;
            if (outlineMode)
            {
                optTmp.enableShade = false;
            }

            optTmp.shaderOpt = gobj->graphic.shaderOpt;

            optTmp.priority = gobj->graphic.priority;
            optTmp.depthBuffIgnore = gobj->graphic.depthBuffIgnore;

            DrawSpriteGraphEx(sgd, optTmp);
        }

        //�e
        if (gobj->graphic.shadow)
        {
            DRAW_SPRITE_DAT dsdat = {};
            dsdat.x = gobj->pos.x;
            dsdat.y = gobj->pos.y;
            //dsdat.z = gobj->pos.z - 0.1f - (0.1f / GAME_OBJECT_MAX) * GetGameObjectIndex(gobj);
            //dsdat.z = gobj->pos.z - 0.005f;
            //dsdat.z = -4.0f;
            dsdat.z = -8.0f;
            //dsdat.z = -0.005f;
            dsdat.w = gobj->size.x * gobj->scale.x * 0.85f;
            dsdat.h = gobj->size.y * gobj->scale.y * 0.85f;
            dsdat.rot = 0.0f;
            //dsdat.color = { 1.0f, 1.0f, 1.0f, 0.5f };
            //dsdat.color = { 1.0f, 1.0f, 1.0f, 0.75f };
            //dsdat.color = { 1.0f, 1.0f, 1.0f, 0.4f };
            dsdat.color = gobj->graphic.shadowColor;
            

            dsdat.texNo = g_ShadowTex.textureId;
            dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

            dsdat.texUV = { 0.0f, 0.0f };
            dsdat.texWH = { 1.0f, 1.0f };
            dsdat.startPos = { 0.0f, 0.0f };
            dsdat.startRot = 0.0f;
            dsdat.startScale = { 1.0f, 1.0f };

            DrawSpriteInCamera(dsdat, false, false,
                true
            );
        }
    }

#if defined(_DEBUG) || defined(DEBUG)

	static bool infoView = false;
	//�f�o�b�O  �I���I�t
	{
		static bool kp = false;
		if (GetAsyncKeyState('V'))
		{
			if (!kp)
			{
				infoView = !infoView;
				kp = true;
			}
		}
		else
		{
			kp = false;
		}
	}


	if (!infoView || shadowMode || outlineMode)
	{
		return;
	}

	{ //�����蔻��
		GAME_OBJECT_COLLISION* c = &gobj->collision;

		if (c->atk.enable)
		{
			for (int i = 0; i < GAME_OBJECT_COLLISION_NUM_MAX; i++)
			{
				if (c->atk.collision[i].enable)
				{
                    DRAW_POLYGON_DAT drawDatTmp = {};
                    drawDatTmp.size = MkF3(
                       c->atk.collision[i].size.x,
                       c->atk.collision[i].size.y,
                       c->atk.collision[i].size.z
                    );

                    drawDatTmp.color = MkF4(2.0f, 0.0f, 0.0f, 0.5f);

                    drawDatTmp.texNo = 0;
                    drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                    drawDatTmp.texUV = MkF2(
                        0.0f,
                        0.0f
                    );
                    drawDatTmp.texWH = {
                        1.0f,
                        1.0f
                    };

                    drawDatTmp.startPos3D = MkF3(
                        gobj->pos.x + c->atk.collision[i].pos.x,
                        gobj->pos.y + c->atk.collision[i].pos.y,
                        gobj->pos.z + c->atk.collision[i].pos.z
                    );

                    drawDatTmp.startRot3D = MkF3(0.0f, 0.0f, 0.0f);

                    drawDatTmp.startScale = MkF2(1.0f, 1.0f);
                    drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

                    drawDatTmp.modelNo = 0;
                    drawDatTmp.vertex.type = DRAW_POLYGON_TYPE_CUBE;

                    drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;

                    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                    opt.reverse = false;
                    opt.fixed = false;
                    opt.enableShade = false;
                    opt.billboard = false;
                    opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;

                    DrawPolygonInCameraEx(drawDatTmp, opt);

					//DRAW_SPRITE_DAT dsdat = {};
					//dsdat.x = gobj->pos.x + c->atk.collision[i].pos.x;
					//dsdat.y = gobj->pos.y + c->atk.collision[i].pos.y;
     //               dsdat.z = gobj->pos.z + c->atk.collision[i].pos.z;
					//dsdat.w = c->atk.collision[i].size.x;
					//dsdat.h = c->atk.collision[i].size.y;
					//dsdat.rot = 0.0f;
					//dsdat.color = { 1.0f, 0.0f, 0.0f, 0.8f };
					//if (c->atk.collision[i].isGrab)
					//{
					//	dsdat.color = { 1.0f, 0.0f, 1.0f, 0.8f };
					//}

					//dsdat.texNo = 0;
					//dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

					//dsdat.texUV = { 0.0f, 0.0f };
					//dsdat.texWH = { 1.0f, 1.0f };
					//dsdat.startPos = { 0.0f, 0.0f };
					//dsdat.startRot = 0.0f;
					//dsdat.startScale = { 1.0f, 1.0f };

					//DrawSpriteInCamera(dsdat);
				}
			}
		}
		if (c->rec.enable)
		{
			for (int i = 0; i < GAME_OBJECT_COLLISION_NUM_MAX; i++)
			{
				if (c->rec.collision[i].enable)
				{
                    DRAW_POLYGON_DAT drawDatTmp = {};
                    drawDatTmp.size = MkF3(
                        c->rec.collision[i].size.x,
                        c->rec.collision[i].size.y,
                        c->rec.collision[i].size.z
                    );

                    drawDatTmp.color = MkF4(0.0f, 1.0f, 0.2f, 0.8f);

                    drawDatTmp.texNo = 0;
                    drawDatTmp.loadTexType = LOADTEXTURETYPE_MAIN;

                    drawDatTmp.texUV = MkF2(
                        0.0f,
                        0.0f
                    );
                    drawDatTmp.texWH = {
                        1.0f,
                        1.0f
                    };

                    drawDatTmp.startPos3D = MkF3(
                        gobj->pos.x + c->rec.collision[i].pos.x,
                        gobj->pos.y + c->rec.collision[i].pos.y,
                        gobj->pos.z + c->rec.collision[i].pos.z
                    );

                    drawDatTmp.startRot3D = MkF3(0.0f, 0.0f, 0.0f);

                    drawDatTmp.startScale = MkF2(1.0f, 1.0f);
                    drawDatTmp.startScale3D = MkF3(1.0f, 1.0f, 1.0f);

                    drawDatTmp.modelNo = 0;
                    drawDatTmp.vertex.type = DRAW_POLYGON_TYPE_CUBE;

                    drawDatTmp.shaderOpt.bloom = DPD_SHADER_OPT_BLOOM_NORMAL;

                    DRAW_POLYGON_IN_CAMERA_EX_OPTION opt = {};
                    opt.reverse = false;
                    opt.fixed = false;
                    opt.enableShade = false;
                    opt.billboard = false;
                    opt.priority = DRAW_POLYGON_PRIORITY_NML_ALWAYS;

                    DrawPolygonInCameraEx(drawDatTmp, opt);

				/*	DRAW_SPRITE_DAT dsdat = {};
					dsdat.x = gobj->pos.x + c->rec.collision[i].pos.x;
					dsdat.y = gobj->pos.y + c->rec.collision[i].pos.y;
					dsdat.w = c->rec.collision[i].size.x;
					dsdat.h = c->rec.collision[i].size.y;
					dsdat.rot = 0.0f;
					dsdat.color = { 0.0f, 1.0f, 0.2f, 0.8f };

					dsdat.texNo = 0;
					dsdat.loadTexType = LOADTEXTURETYPE_MAIN;

					dsdat.texUV = { 0.0f, 0.0f };
					dsdat.texWH = { 1.0f, 1.0f };
					dsdat.startPos = { 0.0f, 0.0f };
					dsdat.startRot = 0.0f;
					dsdat.startScale = { 1.0f, 1.0f };

					DrawSpriteInCamera(dsdat);*/
				}
			}
		}
	}

    return;
    if (!gobj->family[GAME_OBJECT_FAMILY_PLAYER]
        && !gobj->family[GAME_OBJECT_FAMILY_ENEMY]
        && !gobj->family[GAME_OBJECT_FAMILY_GIMIK]
        )
    {
        return;
    }

    if (!gobj->graphic.hidden)
	{
		//CAMERA cinfo = GetCameraInfo();
		//Float2 cposBase = {};
		//cposBase.x = gobj->pos.x - cinfo.trans.pos.x - cinfo.cameraDrawDat.zoomAddPos.x;
		//cposBase.y = gobj->pos.y - cinfo.trans.pos.y - cinfo.cameraDrawDat.zoomAddPos.y;

		//cposBase.x *= (cinfo.trans.zoom * cinfo.trans.scale.x) - cinfo.cameraDrawDat.zoomAddScale;
		//cposBase.y *= (cinfo.trans.zoom * cinfo.trans.scale.y) - cinfo.cameraDrawDat.zoomAddScale;
		Float2 cposBase = WorldPosToCameraPos(0, gobj->pos);

		{
			Float2 cpos = cposBase;

			cpos.x += 10.0f;
			cpos.y += -170.0f;

			std::string str = "HP: " + std::to_string(gobj->status.hp) + " / " + std::to_string(gobj->status.hpMax);
			DrawFont(str, FONT_TYPE_DEFAULT,
				cpos,
				28,
				FONT_TEXT_COLOR_WHITE,
				FONT_TEXT_ALIGNMENT_LEFT
			);
			cpos.x += -4.0f;
			cpos.y += -4.0f;
			DrawFont(str, FONT_TYPE_DEFAULT,
				cpos,
				28,
				FONT_TEXT_COLOR_WHITE,
				FONT_TEXT_ALIGNMENT_LEFT
			);
		}
		{
			Float2 cpos = cposBase;

			cpos.x += 10.0f;
			cpos.y += -190.0f;

			std::string str = "" + gobj->status.fullName;
			DrawFont(str, FONT_TYPE_DEFAULT,
				cpos,
				22,
                FONT_TEXT_COLOR_BLACK,
				FONT_TEXT_ALIGNMENT_LEFT
			);
			cpos.x += -4.0f;
			cpos.y += -4.0f;
			DrawFont(str, FONT_TYPE_DEFAULT,
				cpos,
				22,
				FONT_TEXT_COLOR_WHITE,
				FONT_TEXT_ALIGNMENT_LEFT
			);
		}
		{
			static unsigned int debugBeforeDamageFrame[GAME_OBJECT_MAX] = {};
			static int debugDamageViewCount[GAME_OBJECT_MAX] = {};
			constexpr int debugDamageViewCountMax = 80;

			int indexObj = GetGameObjectIndex(gobj);

			if (debugBeforeDamageFrame[indexObj] != gobj->status.beforeDamageFrame)
			{
				debugDamageViewCount[indexObj] = debugDamageViewCountMax;

				debugBeforeDamageFrame[indexObj] = gobj->status.beforeDamageFrame;
			}

			if (debugDamageViewCount[indexObj] > 0)
			{
				Float2 cpos = cposBase;

				cpos.x += 30.0f;
				cpos.y += -70.0f - 70.0f * ((float)debugDamageViewCount[indexObj] / debugDamageViewCountMax);

				std::string str = "-" + std::to_string(gobj->status.beforeDamage);
				DrawFont(str, FONT_TYPE_DEFAULT,
					cpos,
					30,
                    FONT_TEXT_COLOR_BLACK,
					FONT_TEXT_ALIGNMENT_LEFT
				);
				cpos.x += -4.0f;
				cpos.y += -4.0f;
				DrawFont(str, FONT_TYPE_DEFAULT,
					cpos,
					30,
                    FONT_TEXT_COLOR_WHITE,
					FONT_TEXT_ALIGNMENT_LEFT
				);

				debugDamageViewCount[indexObj]--;
			}
		}
	}
#endif
}

// ===================================================
// �Q�[���I�u�W�F�N�g�̏I������
// ===================================================
void UninitGameObject(void)
{
	for (int i = 0; i < GAME_OBJECT_MAX; i++)
	{
		DeleteGameObject(gameObjects + i);
	}

    UnloadTexData(g_ShadowTex);
}

GAME_OBJECT* CreateGameObject(GAME_OBJECT obj)
{
	for (int i = 0; i < GAME_OBJECT_MAX; i++)
	{
		if (!gameObjects[i].use && gameObjects[i].delRequ <= 0)
		{
			gameObjects[i] = obj;
			gameObjects[i].createFrame = GetMainGameFrame();

			gameObjects[i].use = true;

            if (gameObjects[i].events.source == nullptr)
            {
                //GOE�̃Z�b�g
                GameObjectEventSet(gameObjects + i);
            }

			if (gameObjects[i].events.source != nullptr)
			{ //�������C�x���g
				gameObjects[i].events.source->Init();
			}

			return gameObjects + i;
		}
	}
	return nullptr;
}

bool DeleteGameObject(GAME_OBJECT* obj)
{
    if (obj == nullptr)
    {
        return false;
    }

	if (obj->use)
	{
		if (obj->events.source != nullptr)
		{ //�I���C�x���g
			obj->events.source->Uninit();
		}

		if (obj->privateVariable != nullptr)
		{ //�t���[�ϐ��̉��
			free(obj->privateVariable);
			obj->privateVariable = nullptr;
		}

		if (obj->events.source != nullptr)
		{ //�C�x���g�̎��̂�j��
			delete obj->events.source;
			obj->events.source = nullptr;
		}

		//obj->use = false;
		obj->delRequ = 2; //�폜�t���O�𗧂Ă�

		return true;
	}
	return false;
}
bool DeleteGameObject(int index)
{
	if (index >= 0 && index < GAME_OBJECT_MAX)
	{
		return DeleteGameObject(gameObjects + index);
	}

	return false;
}
bool DeleteGameObjectAll()
{
	bool r = false;
	for (int i = 0; i < GAME_OBJECT_MAX; i++)
	{
		bool rc = DeleteGameObject(i);
		if (rc)
		{
			r = true;
		}
	}

	return r;
}

// ===================================================
// �I�u�W�F�N�g�̎擾
// ===================================================
GAME_OBJECT* GetGameObjects()
{
	return gameObjects;
}
GAME_OBJECT* GetGameObject(int index)
{
	if (index < 0 || index >= GAME_OBJECT_MAX)
	{
		return nullptr;
	}
	return gameObjects + index;
}


GAME_OBJECT* GetGameObjectFindByIdentifier(GAME_OBJECT_IDENTIFIER identifier)
{
	for (int i = 0; i < GAME_OBJECT_MAX; i++)
	{
		if (IsActiveGameObject(gameObjects + i)
			&& CheckIdentifierGameObject(gameObjects + i, identifier))
		{
			return gameObjects + i;
		}
	}
	return nullptr;
}



int GetGameObjectIndex(GAME_OBJECT* obj)
{
	//�����Ǝ��̉�����Ă��邱�Ƃ��m�F���Ă���Ԃ�
	for (int i = 0; i < GAME_OBJECT_MAX; i++)
	{
		if (obj == GetGameObjects() + i)
		{
			return (int)(obj - GetGameObjects());
		}
	}
	return -1;
	
	//���Ȃ񂩂��܂�������
	//if (obj != nullptr
	//	&& ((int)obj) >= ((int)GetGameObjects())
	//	&& ((int)obj) < ((int)GetGameObjects() + (int)GAME_OBJECT_MAX))
	//{
	//	return (int)(obj - GetGameObjects());
	//}
	//return -1;
}

bool IsActiveGameObject(GAME_OBJECT* obj)
{
	if (obj == nullptr)
	{
		return false;
	}
	//return obj->use && (obj->alwaysUpdate || IsCameraInGameObject(obj));
	return obj->use;
}

bool IsCameraInGameObject(GAME_OBJECT* obj)
{
    if (obj == nullptr)
    {
        return false;
    }
    for (int i = 0; i < CAMERA_NUM; i++)
    {
        if (IsCameraInGameObject(i, obj))
        {
            return true;
        }
    }
    return false;
}

bool IsCameraInGameObject(int cameraIndex, GAME_OBJECT* obj)
{
	if (obj == nullptr)
	{
		return false;
	}
	return CheckBoxCollider(CameraPosToWorldPos(cameraIndex, MkF2(0.0f, 0.0f)), MkF2(obj->pos.x, obj->pos.y), GetCameraSize(cameraIndex), MkF2(1.0f, 1.0f));
}

bool IsBelongsFamilyGameObject(GAME_OBJECT* obj, GAME_OBJECT_FAMILY family)
{
	if (family >= GAME_OBJECT_FAMILY_MAX || family < 0)
	{
		return false;
	}

	if (obj->family[GAME_OBJECT_FAMILY_ALL])
	{
		return true;
	}
	return obj->family[family];
}

bool IsDefeatGameObject(GAME_OBJECT* obj)
{
    if (!IsActiveGameObject(obj))
    {
        return false;
    }
	return obj->status.hp <= 0;
}

bool IsInStageGameObject(GAME_OBJECT* obj)
{
    bool isInStage = false;
    {
        Stage_Base* stg = GetCurrentStageSceneGame();
        if (stg != nullptr)
        {
            Int2 stgsize = stg->GetStageSize();

            Float2 size = {};
            size.x = BLOCK_WIDTH * stgsize.x;
            size.y = BLOCK_HIGHT * stgsize.y;

            if (CheckBoxCollider(Float3ToFloat2(obj->pos), {}, MkF2(1.0f, 1.0f), size))
            {
                isInStage = true;
            }
        }
    }
    return isInStage;
}
Float3 InStageGameObjectPos(Float3 pos, Float3 size)
{
    Float3 posr = pos;
    {
        Stage_Base* stg = GetCurrentStageSceneGame();
        if (stg != nullptr)
        {
            Int2 stgsize = stg->GetStageSize();

            Float2 stgsizeb = {};
            stgsizeb.x = BLOCK_WIDTH * stgsize.x;
            stgsizeb.y = BLOCK_HIGHT * stgsize.y;

            posr.x = std::min(std::max(posr.x,
                (-stgsizeb.x + size.x) / 2.0f),
                (stgsizeb.x - size.x) / 2.0f);
            posr.y = std::min(std::max(posr.y,
                (-stgsizeb.y + size.y) / 2.0f),
                (stgsizeb.y - size.y) / 2.0f);
        }
    }
    return posr;
}


void GameObjectSetLifeTime(GAME_OBJECT* obj, int life)
{
	obj->lifetimeFrameMax = life;
	obj->lifetimeFrame = obj->lifetimeFrameMax;
}

//�ړ����N�G�X�g
void GameObjectMoveRequest(GAME_OBJECT* obj,
	int frame,
	Float3 pos,
	std::function<float(float)> easingX,
	std::function<float(float)> easingY,
	std::function<float(float)> easingZ,
	bool isBlockCollisionIgnore
)
{
	if (obj != nullptr && frame > 0)
	{
		GAME_OBJECT_MOVE_REQUEST* mr = &obj->moveRequ;
		(*mr) = {};
		mr->enable = true;

		mr->frameMax = frame;
		mr->frame = mr->frameMax;

		mr->startPos = obj->pos;
		mr->endPos = pos;
		mr->oldPos = mr->startPos;

		mr->easingX = easingX;

		mr->easingY = easingY;
		if (easingX != nullptr && easingY == nullptr)
		{
			mr->easingY = easingX;
		}

		mr->easingZ = easingZ;
		if (easingX != nullptr && easingZ == nullptr)
		{
			mr->easingZ = easingX;
		}

		mr->isBlockCollisionIgnore = isBlockCollisionIgnore;
	}
}

//�q�b�g�X�g�b�v
void GameObjectHitStop(GAME_OBJECT* obj,
	int frame
)
{
	if (obj != nullptr && frame > 0)
	{
		GAME_OBJECT_HITSTOP* hs = &obj->hitStop;
		hs->frameMax = frame;
		hs->frame = hs->frameMax;
	}
}

//�q�b�g���A�N�V����
void GameObjectHitReaction(GAME_OBJECT* obj,
	int frame
)
{
	if (obj != nullptr && frame > 0)
	{
		GAME_OBJECT_HIT_REACTION* hr = &obj->hitReaction;
		hr->frameMax = frame;
		hr->frame = hr->frameMax;
	}
}


GAME_OBJECT_DAMAGE_RESULT GameObjectGetDamage(GAME_OBJECT* obj, GAME_OBJECT* objAttacker,
    int damage,
    GAME_OBJECT_COLLISION_ATTACK_CHILD colAtk,
    GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec
)
{
    GAME_OBJECT_DAMAGE_RESULT dr = {};
    dr.colAtk = colAtk;
    dr.colRec = colRec;

    if (obj != nullptr)
    {
        if (objAttacker != nullptr)
        {
            dr.atkObjIndex = (int)(objAttacker - gameObjects);
        }
        dr.recObjIndex = (int)(obj - gameObjects);

        const GAME_OBJECT_STATUS* s = &obj->status;

        //
        //float damModTmp = GAME_OBJECT_GUARD_MOD[s->guardType] * GAME_OBJECT_GUARD_MOD[colRec.guardType];
        //float damModTmp = GAME_OBJECT_GUARD_MOD[s->guardType] * (colRec.guardMod > 0 ? colRec.guardMod : 1.0f);
        float damModTmp = GAME_OBJECT_GUARD_MOD[s->guardType] * colRec.guardMod;
        if (s->attributeMod[colAtk.attribute] != 1.0f)
        { //
            damModTmp *= s->attributeMod[colAtk.attribute];
        }

        dr.mod = damModTmp;
        if (damModTmp > 0.0 && (s->invFrame <= 0 || colAtk.invIgnore))
        {
            dr.damage = (int)(damage * damModTmp);
            if (objAttacker != nullptr)
            {
                if (objAttacker->events.source != nullptr)
                {
                    objAttacker->events.source->CalcAttackerDamage(
                        &dr,
                        obj, objAttacker,
                        damage,
                        colAtk,
                        colRec
                    );
                }
            }
            if (obj->events.source != nullptr)
            {
                obj->events.source->CalcDamage(
                    &dr,
                    obj, objAttacker,
                    damage,
                    colAtk,
                    colRec
                );
            }

            if (dr.damage < 0)
            {
                dr.damage = 0;
            }
        }
    }

    return dr;
}
//�_���[�W
GAME_OBJECT_DAMAGE_RESULT GameObjectDamage(GAME_OBJECT* obj, GAME_OBJECT* objAttacker,
	int damage,
	GAME_OBJECT_COLLISION_ATTACK_CHILD colAtk,
	GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec
)
{
	GAME_OBJECT_DAMAGE_RESULT dr = GameObjectGetDamage(obj, objAttacker,
        damage,
        colAtk,
        colRec
    );

	return GameObjectDamage(obj, objAttacker, dr);
}

GAME_OBJECT_DAMAGE_RESULT GameObjectDamage(
    GAME_OBJECT* obj, GAME_OBJECT* objAttacker,
    GAME_OBJECT_DAMAGE_RESULT dr
)
{
    if (obj != nullptr)
    {
        GAME_OBJECT_STATUS* s = &obj->status;

        //
        s->beforeDamageFrame = obj->frame;
        s->beforeDamage = dr.damage;
        s->hp -= s->beforeDamage;

        if (s->hp > s->hpMax)
        {
            s->hp = s->hpMax;
        }
        if (s->hp < s->hpMin)
        {
            s->hp = s->hpMin;
        }
    }


    if (objAttacker != nullptr)
    {
        if (objAttacker->events.source != nullptr)
        { //�_���[�W�C�x���g
            objAttacker->events.source->DamageAttacker(dr);
        }
    }
    if (obj != nullptr)
    {
        if (obj->events.source != nullptr)
        { //�_���[�W�C�x���g
            obj->events.source->DamageReciever(dr);
        }
    }

    return dr;
}

GAME_OBJECT_DAMAGE_COLLISION_HIT_RESULT GameObjectDamageCollisionHit(GAME_OBJECT* obj, GAME_OBJECT* objAttacker,
	GAME_OBJECT_COLLISION_ATTACK_CHILD colAtk,
	GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec,
    GAME_OBJECT* objAttackerChild
)
{
	GAME_OBJECT_DAMAGE_COLLISION_HIT_RESULT result = {};

	GAME_OBJECT_DAMAGE_RESULT dr = GameObjectDamage(obj, objAttacker, colAtk.power, colAtk, colRec);
    if (objAttackerChild != nullptr)
    {
        if (objAttackerChild->events.source != nullptr)
        { 
            objAttackerChild->events.source->DamageAttacker(dr);
        }
    }
	//dr.colAtk = oCol->atk;
	//dr.colRec = oCol->rec;

	if (dr.damage > 0)
	{ //�q�b�g���̉��o

		//�_���[�W�ɔ{�����������Ă����ꍇ�q�b�g�X�g�b�v�ɂ��␳��������
		float hsmod = (float)(dr.mod > 1.0f ? 1.0f : dr.mod);
		int hsA = (int)(colAtk.hitStopAttacker);
		int hsR = (int)(colAtk.hitStopReciever * hsmod);
        if (objAttacker != nullptr && objAttacker->status.hitStopArmorFrame <= 0)
        {
            GameObjectHitStop(objAttacker, hsA);
        }
        if (obj != nullptr && obj->status.hitStopArmorFrame <= 0)
        {
            GameObjectHitStop(obj, hsR);
        }
		GameObjectHitReaction(obj, hsR + GAME_OBJECT_HIT_REACTION_MIN);
	}

	for (int j = 0; j < GAME_OBJECT_HIT_CYCLE_NUM_MAX; j++)
	{ //����������Z�b�g
		if (obj != nullptr && obj->collision.colAtkCycleList[j].frame <= 0)
		{
			//obj->collision.colAtkCycleList[j].objIndex = GetGameObjectIndex(objAttacker);
			//obj->collision.colAtkCycleList[j].objCreateFrame = objAttacker->createFrame;
			obj->collision.colAtkCycleList[j].objIdentifier = GetIdentifierGameObject(objAttacker);

			obj->collision.colAtkCycleList[j].id = colAtk.id;
			obj->collision.colAtkCycleList[j].groupNumber = colAtk.groupNumber;

			obj->collision.colAtkCycleList[j].frameMax = colAtk.hitCycle;
			obj->collision.colAtkCycleList[j].frame = colAtk.hitCycle;
			break;
		}
	}

	//���ʂ��Z�b�g
	result.damageResult = dr;

	return result;
}


void gameObjectAttackCollisionUpdate()
{
	for (int i = 0; i < g_attackCollisionAddBuf.size(); i++)
	{
		if (g_attackCollisionAddBuf[i].frame > 0)
		{
			g_attackCollisionAddBuf[i].frame--;
		}
		else
		{
			if (g_attackCollisionAddBuf[i].obj != nullptr
				&& IsActiveGameObject(g_attackCollisionAddBuf[i].obj)
			)
			{
				GAME_OBJECT_COLLISION* oColA = &g_attackCollisionAddBuf[i].obj->collision;
				oColA->atk.enable = true;
				for (int j = 0; j < GAME_OBJECT_COLLISION_NUM_MAX; j++)
				{
					if (!oColA->atk.collision[j].enable)
					{
						oColA->atk.collision[j] = g_attackCollisionAddBuf[i].col;
                        oColA->atk.collision[j].enable = true;
						break;
					}
				}
			}

			g_attackCollisionAddBuf[i].finished = true;
		}
	}

	auto it = std::find_if(
		g_attackCollisionAddBuf.begin(),
		g_attackCollisionAddBuf.end(),
		[](GO_ATTACK_COLLISION_ADD_BUF buf) {
			return buf.finished;
		}
	);
	if (it != g_attackCollisionAddBuf.end()) {
		// ���������v�f���폜
		g_attackCollisionAddBuf.erase(it);
		g_attackCollisionAddBuf.shrink_to_fit();
	}
}
void GameObjectAttackCollisionAdd(GAME_OBJECT* gobj, GAME_OBJECT_COLLISION_ATTACK_CHILD newAtkCol,
	int delayFrame,
	int enableFrame
)
{
	GO_ATTACK_COLLISION_ADD_BUF buf = {};
	buf.finished = false;
	buf.frame = delayFrame;
	buf.obj = gobj;

	GAME_OBJECT_COLLISION_ATTACK_CHILD newSetAtkCol = newAtkCol;
	newSetAtkCol.enable = true;
	newSetAtkCol.id = GetMainGameFrame();

	if (enableFrame > 0)
	{
		newSetAtkCol.enableFrameMax = enableFrame;
		newSetAtkCol.enableFrame = enableFrame;
	}

	buf.col = newSetAtkCol;

	if (buf.obj != nullptr)
	{
		g_attackCollisionAddBuf.push_back(buf);
	}
}

void AddTagGameObject(GAME_OBJECT* gobj, std::string str)
{
	gobj->tags.push_back(str);
}
bool HasTagGameObject(GAME_OBJECT* gobj, std::string str)
{
	auto it = std::find(gobj->tags.begin(), gobj->tags.end(), str);
	if (it != gobj->tags.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CheckIdentifierGameObject(GAME_OBJECT* gobj, GAME_OBJECT_IDENTIFIER identifier)
{
	if (gobj == nullptr)
	{
		return false;
	}
	return (
		gobj->createFrame == identifier.objCreateFrame
		&& GetGameObjectIndex(gobj) == identifier.objIndex
	);
}

GAME_OBJECT_IDENTIFIER GetIdentifierGameObject(GAME_OBJECT* gobj)
{
	GAME_OBJECT_IDENTIFIER identifier = {};
	if (gobj != nullptr)
	{
		identifier.objIndex = GetGameObjectIndex(gobj);
		identifier.objCreateFrame = gobj->createFrame;
	}
	return identifier;
}


std::map<int, GAME_OBJECT*> GetGameObjectFindByDistance(Float3 startPos, float distance)
{
    int index = 0;
    std::map<int, GAME_OBJECT*> objectsTmp = {};

    for (int i = 0; i < GAME_OBJECT_MAX; i++)
    {
        GAME_OBJECT* obj = GetGameObjects() + i;
        if (obj != nullptr && IsActiveGameObject(obj))
        {
            float distanceTmp = CalculateDistance(startPos, obj->pos);
            if (distanceTmp <= distance)
            {
                objectsTmp[index] = obj;
                index++;
            }
        }
    }

    //std::sort(players.begin(), players.end(),
    //	[](const PLAYER& a, const PLAYER& b) {
    //		return a.;
    //	}
    //);
    return objectsTmp;
}

Float3 GetGameObjectColRecAbsolutePos(GAME_OBJECT* gobj)
{
    if (gobj == nullptr)
    {
        return {};
    }

    //有効なくらい判定を探す
    GAME_OBJECT_COLLISION_RECIEVE_CHILD colRec = {};
    for (int j = 0; j < GAME_OBJECT_COLLISION_NUM_MAX; j++)
    {
        if (gobj->collision.rec.collision[j].enable)
        {
            colRec = gobj->collision.rec.collision[j];
            break;
        }
    }

    if (colRec.enable)
    { //判定が有効なら処理する
        Float3 pos = {};
        pos.x = gobj->pos.x + colRec.pos.x;
        pos.y = gobj->pos.y + colRec.pos.y;
        pos.z = gobj->pos.z + colRec.pos.z;

        return pos;
    }
    return {};
}
