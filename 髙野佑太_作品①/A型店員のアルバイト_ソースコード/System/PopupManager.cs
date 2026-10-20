// =============================================================
// 概要   : 商品状態のポップアップ表示を管理する
// 制作者 : 高野 佑太
// 制作日 : 2026/06/30
// =============================================================

using UnityEngine;

public class PopupManager : MonoBehaviour
{
    public static PopupManager Instance { get; private set; }

    [Header("フワッと出る文字のプレハブ")]
    [SerializeField] private GameObject floatingTextPrefab;

    private void Awake()
    {
        Instance = this;
    }

    public void ShowPopup(Vector3 worldPosition, string text, Color color)
    {
        if (floatingTextPrefab == null)
        {
            return;
        }

        // 商品に文字が重ならないよう、表示位置を少し上げる。
        Vector3 spawnPos = worldPosition + Vector3.up * 0.5f;
        GameObject popup = Instantiate(floatingTextPrefab, spawnPos, Quaternion.identity);

        FloatingText3D script = popup.GetComponent<FloatingText3D>();
        if (script != null)
        {
            script.Setup(text, color);
        }
    }
}
