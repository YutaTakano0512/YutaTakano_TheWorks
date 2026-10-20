// =============================================================
// 概要   : 棘のある商品の保持可能時間を設定する
// 制作者 : 髙野佑太
// 制作日 : 2026/08/19
// =============================================================

using UnityEngine;

public class GimmickSpiky : MonoBehaviour
{
    private ItemGimmick itemGimmick;

    [Header("棘のある商品のギミック設定")]
    [SerializeField] private float spikyLimitTime = 3.0f;// 持てる時間の制限

    private void Start()
    {
        // 自分自身についてる ItemGimmick コンポーネントを取得
        itemGimmick = GetComponent<ItemGimmick>();

        if (itemGimmick != null)
        {
            itemGimmick.SetHoldLimitTime(spikyLimitTime);
            itemGimmick.AddAttributeText("とげとげ");
        }
    }
}
