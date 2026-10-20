// =============================================================
// 概要   : 商品が床に触れたことを判定する
// 制作者 : 小野田喬
// 制作日 : 2025/10/29
// 追記: 髙野佑太 2026/07/03
// =============================================================

using UnityEngine;

public class FloorChecker : MonoBehaviour
{
    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.layer == LayerMask.NameToLayer("HotField"))
        {
            return;
        }
        Transform root = other.transform.root;
        if (root.CompareTag("Merchandise"))
        {
            ItemGimmick item = root.GetComponent<ItemGimmick>();
            if (item != null)
            {
                // スコアマネージャーにペナルティを追加（SEも鳴ります）
                if (ScoreManager.Instance != null)
                {
                    ScoreManager.Instance.AddDropPenalty(item);
                }
                // ポップアップを表示
                if (PopupManager.Instance != null)
                {
                    PopupManager.Instance.ShowPopup(root.position, "スコア半減！", Color.red);
                }
                // 机の上に戻す
                item.ReturnToDesk();
            }
        }
    }
}
