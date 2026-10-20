// =============================================================
// 概要   : 熱い商品の判定範囲とエフェクトを生成する
// 制作者 : 髙野佑太
// 制作日 : 2026/08/19
// =============================================================

using UnityEngine;

public class GimmickHot : MonoBehaviour
{
    private ItemGimmick itemGimmick;

    [Header("熱い商品のギミック設定")]
    [SerializeField] private GameObject hotHitBoxPrefab;
    [SerializeField] private Vector3 hotBoxScale = new Vector3(1.0f, 1.0f, 1.0f);
    [SerializeField] private GameObject hotEffectPrefab;

    private void Start()
    {
        itemGimmick = GetComponent<ItemGimmick>();
        if (itemGimmick != null)
        {
            itemGimmick.AddAttributeText("熱い");
        }

        // 商品と一緒に動く熱判定を子オブジェクトとして生成する。
        if (hotHitBoxPrefab != null)
        {
            GameObject childHotBox = Instantiate(hotHitBoxPrefab, transform);
            childHotBox.transform.localPosition = Vector3.zero;
            childHotBox.transform.localRotation = Quaternion.identity;
            childHotBox.transform.localScale = hotBoxScale;
        }

        if (hotEffectPrefab != null)
        {
            Instantiate(hotEffectPrefab, transform.position, Quaternion.identity, transform);
        }
    }
}
