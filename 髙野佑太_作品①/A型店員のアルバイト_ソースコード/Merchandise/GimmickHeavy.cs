// =============================================================
// 概要   : 重い商品の属性と上面接触判定を管理する
// 制作者 : 髙野佑太
// 制作日 : 2026/08/19
// =============================================================

using UnityEngine;

public class GimmickHeavy : MonoBehaviour
{
    // 重い商品のギミックが発動しているかどうかを判定するメソッド
    public static bool IsOnTopOf(Collision collision, Transform lowerItem)
    {
        for (int i = 0; i < collision.contactCount; i++)
        {
            ContactPoint contact = collision.GetContact(i);// 接触点の情報を取得
            Collider lowerCollider = contact.thisCollider;// 接触しているコライダーのうち、下側の商品に属するコライダーを取得
            Collider heavyCollider = contact.otherCollider;// 接触しているコライダーのうち、上側の商品に属するコライダーを取得

            if (lowerCollider == null || !lowerCollider.transform.IsChildOf(lowerItem) ||
                heavyCollider == null || heavyCollider.GetComponentInParent<GimmickHeavy>() == null)
            {
                lowerCollider = contact.otherCollider;
                heavyCollider = contact.thisCollider;

                if (lowerCollider == null || !lowerCollider.transform.IsChildOf(lowerItem) ||
                    heavyCollider == null || heavyCollider.GetComponentInParent<GimmickHeavy>() == null)
                {
                    continue;
                }
            }

            Bounds lowerBounds = lowerCollider.bounds;
            Bounds heavyBounds = heavyCollider.bounds;

            float topTolerance = Mathf.Max(0.05f, lowerBounds.size.y * 0.25f);// 上面の接触点の許容範囲を設定
            bool upwardSurface = Mathf.Abs(Vector3.Dot(contact.normal, Vector3.up)) >= 0.65f;// 接触点の法線が上向きであるかどうかを判定
            bool nearTop = contact.point.y >= lowerBounds.max.y - topTolerance;// 接触点が下側商品の上面に近いかどうかを判定
            bool heavyAbove = heavyBounds.center.y > lowerBounds.center.y &&
                              heavyBounds.center.y >= contact.point.y;// 上側商品の中心が下側商品の中心より上にあり、接触点よりも上にあるかどうかを判定

            if (upwardSurface && nearTop && heavyAbove)
            {
                return true;
            }
        }

        return false;
    }

    private void Start()
    {
        ItemGimmick itemGimmick = GetComponent<ItemGimmick>();
        if (itemGimmick != null)
        {
            itemGimmick.AddAttributeText("重い");
        }
    }
}
