// =============================================================
// 概要   : チュートリアル画面の操作を管理する
// 制作者 : 高野 佑太
// 制作日 : 2026/06/30
// =============================================================

using UnityEngine;
using UnityEngine.UI;

public class TutorialController : MonoBehaviour
{
    [Header("閉じるボタン")]
    [SerializeField] private Button closeButton;

    [Header("保存用のキー（TutorialManagerと同じものを設定）")]
    [SerializeField] private string tutorialKey = "Tutorial_Stage1";

    private void Start()
    {
        // 閉じるボタンのイベント登録
        if (closeButton != null)
        {
            closeButton.onClick.AddListener(CloseTutorial);
        }
    }

    private void CloseTutorial()
    {
        // 次回から表示しないように保存
        PlayerPrefs.SetInt(tutorialKey, 1);
        PlayerPrefs.Save();

        // ゲーム進行を再開
        Time.timeScale = 1f;

        Hands hands = Object.FindFirstObjectByType<Hands>();
        if (hands != null)
        {
            hands.enabled = true;
        }

        // 生成されたチュートリアル（自分自身）を削除する
        Destroy(gameObject);
    }
}
