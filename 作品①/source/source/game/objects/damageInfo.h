#pragma once
#include "../../math/vector2.h"

//=============================================================================
// DamageInfo 構造体
// 攻撃がヒットした際に、攻撃側から被弾側に渡される情報
//=============================================================================
struct DamageInfo
{
    float   damage = 1.0f;// ダメージ量
    Vector2 knockback = { 0.0f, 0.0f };// ノックバックの方向と強さ
    float   hitstopDuration = 0.0f; // ヒットストップ時間 (秒)
    float   shakeIntensity = 0.0f;// 被弾したキャラのヒットシェイク強度
    float   shakeDuration = 0.0f;// 被弾したキャラのヒットシェイク時間 (秒)
    float   cameraShakeIntensity = 0.0f;// カメラシェイク強度
    float   cameraShakeDuration = 0.0f;// カメラシェイク時間 (秒)
};
