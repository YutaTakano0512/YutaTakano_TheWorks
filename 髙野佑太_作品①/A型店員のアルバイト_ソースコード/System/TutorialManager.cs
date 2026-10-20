// =============================================================
// 概要   : チュートリアルの初回表示と一時停止を管理する
// 制作者 : 高野 佑太
// 制作日 : 2026/06/30
// =============================================================

using UnityEngine;
using System.Collections;

public class TutorialManager : MonoBehaviour
{
    [Header("チュートリアルキャンバスのプレハブ")]
    [SerializeField] private GameObject tutorialCanvasPrefab;

    [Header("表示判定用のキー (TutorialControllerと同じものを設定してください)")]
    [SerializeField] private string tutorialKey = "Tutorial_Stage1";

    [Header("デバッグ用：エディタ実行時は毎回表示する")]
    [SerializeField] private bool alwaysShowInEditor = true;

    private void Start()
    {
        if (tutorialCanvasPrefab == null)
        {
            return;
        }

        bool isAlreadyShown = (PlayerPrefs.GetInt(tutorialKey, 0) == 1);

#if UNITY_EDITOR
        // エディタ上かつデバッグフラグがONの場合は強制的に未表示扱いにする
        if (alwaysShowInEditor)
        {
            isAlreadyShown = false;
        }
#endif

        // 既に表示済みの場合は生成せずに処理を終了
        if (isAlreadyShown)
        {
            return;
        }

        // 未表示の場合はコルーチンを開始してフェードを待つ
        StartCoroutine(ShowTutorialCoroutine());
    }

    private IEnumerator ShowTutorialCoroutine()
    {
        // FadeManagerのデフォルトフェード時間分待機する
        float waitTime = 1.0f;
        if (FadeManager.Instance != null)
        {
            waitTime = FadeManager.Instance.DefaultFadeTime;
        }

        // 時間スケールが0になっている可能性も考慮してリアルタイムで待機
        yield return new WaitForSecondsRealtime(waitTime);

        // プレハブからチュートリアルを生成してゲームを一時停止
        if (tutorialCanvasPrefab != null)
        {
            Instantiate(tutorialCanvasPrefab);

            // UIやEventSystemの初期化が完了するのを1フレーム待つ
            yield return null;

            Time.timeScale = 0f;

            Hands hands = Object.FindFirstObjectByType<Hands>();
            if (hands != null)
            {
                hands.enabled = false;
            }
        }
    }
}
