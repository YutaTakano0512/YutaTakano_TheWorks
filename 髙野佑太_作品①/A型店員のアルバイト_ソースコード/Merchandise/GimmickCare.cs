// =============================================================
// 概要   : 天地無用商品の傾きを判定する
// 制作者 : 髙野佑太
// 制作日 : 2026/08/19
// =============================================================

using UnityEngine;
using UnityEngine.Serialization;

public class GimmickCare : MonoBehaviour
{
    private ItemGimmick itemGimmick;
    [Header("天地無用商品のギミック設定")]
    [FormerlySerializedAs("destroyAngleSet")]
    [SerializeField] private float maxTiltAngle = 90.0f; // 倒れたと判定する角度

    private bool isSpoiledByTilt; // 天地無用商品のギミックが発動したかどうか

    private void Start()
    {
        // 自分自身についてる ItemGimmick コンポーネントを取得
        itemGimmick = GetComponent<ItemGimmick>();
        if (itemGimmick != null)
        {
            itemGimmick.AddAttributeText("取り扱い注意");
        }
    }

    private void Update()
    {
        if (itemGimmick == null || isSpoiledByTilt || itemGimmick.GetIsSpoiled())
        {
            return;
        }

        // 上向きベクトルの内積から傾きを判定し、Y軸回転の影響を受けないようにする。
        float uprightDot = Vector3.Dot(transform.up, Vector3.up);
        float limitDot = Mathf.Cos(Mathf.Clamp(maxTiltAngle, 0f, 180f) * Mathf.Deg2Rad);
        if (uprightDot < limitDot)
        {
            isSpoiledByTilt = true;
            itemGimmick.SpoilItem();
        }
    }
}
