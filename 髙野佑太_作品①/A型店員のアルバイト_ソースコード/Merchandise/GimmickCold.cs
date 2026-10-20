// =============================================================
// 概要   : 冷たい商品が熱い商品に近づいた時間を判定する
// 制作者 : 髙野佑太
// 制作日 : 2026/08/19
// =============================================================

using UnityEngine;
using UnityEngine.Serialization;

public class GimmickCold : MonoBehaviour
{
    private ItemGimmick itemGimmick;

    [Header("冷たい商品のギミック設定")]
    [FormerlySerializedAs("coldLimitedTime")]
    [SerializeField] private float hotExposureLimit = 2.0f;
    [SerializeField] private GameObject coldEffectPrefab;
    [SerializeField] private float checkRadius = 3.0f;

    private float coldTime = 0.0f;
    private Collider[] nearbyColliders = new Collider[16];

    private void Start()
    {
        itemGimmick = GetComponent<ItemGimmick>();
        if (itemGimmick != null)
        {
            itemGimmick.AddAttributeText("冷たい");
        }

        if (coldEffectPrefab != null)
        {
            Instantiate(coldEffectPrefab, transform.position, Quaternion.identity, transform);
        }
    }

    private void Update()
    {
        if (itemGimmick == null || itemGimmick.GetIsSpoiled())
        {
            return;
        }

        bool isNearHotItem = false;
        // 配列が満杯なら拡張して再検索し、近くの熱い商品を取りこぼさない。
        int colliderCount;
        while (true)
        {
            colliderCount = Physics.OverlapSphereNonAlloc(transform.position, checkRadius, nearbyColliders);
            if (colliderCount < nearbyColliders.Length)
            {
                break;
            }

            System.Array.Resize(ref nearbyColliders, nearbyColliders.Length * 2);
        }

        for (int i = 0; i < colliderCount; i++)
        {
            Collider col = nearbyColliders[i];
            GimmickHot hotItem = col.GetComponentInParent<GimmickHot>();
            if (hotItem != null && hotItem.gameObject != this.gameObject)
            {
                isNearHotItem = true;
                break;
            }
        }

        // 熱い商品が近くにあればカウントを進める
        if (isNearHotItem)
        {
            coldTime += Time.deltaTime;
            if (coldTime >= hotExposureLimit)
            {
                itemGimmick.SpoilItem();
            }
        }
        else
        {
            coldTime = 0.0f; // 離れたらカウントをリセット
        }
    }

    private void OnDrawGizmosSelected()
    {
        Gizmos.color = new Color(0.0f, 1.0f, 1.0f, 0.3f);
        Gizmos.DrawSphere(transform.position, checkRadius);
    }
}
