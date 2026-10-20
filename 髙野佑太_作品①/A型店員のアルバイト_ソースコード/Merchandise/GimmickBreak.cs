// =============================================================
// 概要   : 重い商品が上に乗ったときに割れやすい商品を判定する
// 制作者 : 髙野佑太
// 制作日 : 2026/08/19
// =============================================================

using System.Collections.Generic;
using UnityEngine;

public class GimmickBreak : MonoBehaviour
{
    private ItemGimmick itemGimmick;
    private bool isBreakCrushed = false;
    private readonly HashSet<Collider> crushingColliders = new HashSet<Collider>();

    [SerializeField] private float crushBreakTime = 0.5f;
    private float crushBreakTimer = 0.0f;

    private void Start()
    {
        itemGimmick = GetComponent<ItemGimmick>();
        if (itemGimmick != null)
        {
            itemGimmick.AddAttributeText("壊れやすい");
        }
    }
    private void Update()
    {
        if (itemGimmick == null || isBreakCrushed)
        {
            return;
        }

        // 重い商品が上に乗り続けた時間だけを加算する。
        if (crushingColliders.Count > 0)
        {
            crushBreakTimer += Time.deltaTime;
            if (crushBreakTimer >= crushBreakTime)
            {
                isBreakCrushed = true;
                itemGimmick.SpoilItem();
            }
        }
        else
        {
            crushBreakTimer = 0f;
        }
    }

    private void OnCollisionEnter(Collision collision)
    {
        UpdateCrushState(collision);
    }

    private void OnCollisionStay(Collision collision)
    {
        UpdateCrushState(collision);
    }

    private void OnCollisionExit(Collision collision)
    {
        crushingColliders.Remove(collision.collider);
    }

    private void UpdateCrushState(Collision collision)
    {
        Collider otherCollider = collision.collider;
        if (GimmickHeavy.IsOnTopOf(collision, transform))
        {
            crushingColliders.Add(otherCollider);
        }
        else
        {
            crushingColliders.Remove(otherCollider);
        }
    }
}
