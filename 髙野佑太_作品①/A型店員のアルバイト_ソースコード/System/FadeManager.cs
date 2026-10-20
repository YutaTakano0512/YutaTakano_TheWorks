// =============================================================
// 概要   : 画面のフェード演出を管理する
// 制作者 : 高野 佑太
// 制作日 : 2026/06/27
// =============================================================

using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using UnityEngine.SceneManagement;

public class FadeManager : MonoBehaviour
{
    public static FadeManager Instance { get; private set; }

    private Canvas fadeCanvas;
    private Image fadeImage;
    private Coroutine fadeCoroutine;
    private CanvasGroup fadeCanvasGroup;

    [SerializeField] private float defaultFadeTime = 1.0f;
    public float DefaultFadeTime => defaultFadeTime;
    [SerializeField] private Color fadeColor = Color.black;

    private void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
            DontDestroyOnLoad(this.gameObject);
        }

        // フェード用のCanvasとImageを作成
        CreateFadeCanvas();
    }

    private void CreateFadeCanvas()
    {
        // Canvas作成
        fadeCanvas = new GameObject("FadeCanvas").AddComponent<Canvas>();
        fadeCanvas.renderMode = RenderMode.ScreenSpaceOverlay;
        fadeCanvas.sortingOrder = 1000; // 最前面に表示
        fadeCanvas.gameObject.AddComponent<GraphicRaycaster>(); // UIクリック判定用
        DontDestroyOnLoad(fadeCanvas.gameObject);

        fadeCanvasGroup = fadeCanvas.gameObject.AddComponent<CanvasGroup>();
        fadeCanvasGroup.blocksRaycasts = false;

        // Image作成
        fadeImage = new GameObject("FadeImage").AddComponent<Image>();
        fadeImage.transform.SetParent(fadeCanvas.transform, false);
        fadeImage.rectTransform.anchorMin = Vector2.zero;
        fadeImage.rectTransform.anchorMax = Vector2.one;
        fadeImage.rectTransform.sizeDelta = Vector2.zero;
        fadeImage.color = new Color(fadeColor.r, fadeColor.g, fadeColor.b, 0);
    }

    // フェードアウトしてからシーン遷移を行い、フェードインする
    public void LoadSceneWithFade(string sceneName)
    {
        StartCoroutine(TransSceneCoroutine(sceneName,defaultFadeTime));
    }

    // フェードアウトしてからゲームを終了する
    public void ExitGameWithFade()
    {
        StartCoroutine(ExitCoroutine(defaultFadeTime));
    }

    // フェードアウトしてからシーンを再読み込み
    public void ReloadSceneWithFade()
    {
        StartCoroutine(ReloadCoroutine(defaultFadeTime));
    }

    private IEnumerator TransSceneCoroutine(string sceneName, float duration)
    {
        // 次のシーンの曲が違う場合、BGMのフェードアウトを開始する
        if (AudioManager.Instance != null)
        {
            AudioManager.Instance.CheckAndFadeOutBGM(sceneName, duration);
        }

        // 画面を暗くする（フェードアウト）
        FadeOut(duration);

        // 暗くなるまで待つ
        yield return new WaitForSecondsRealtime(duration);

        // シーンを読み込む
        SceneManager.LoadScene(sceneName);

        // 画面を明るくする（フェードイン）
        FadeIn(duration);
    }

    private IEnumerator ExitCoroutine(float duration)
    {
        // ゲーム終了時もBGMをフェードアウト
        if (AudioManager.Instance != null)
        {
            AudioManager.Instance.CheckAndFadeOutBGM("", duration);
        }

        // 画面を暗くする（フェードアウト）
        FadeOut(duration);

        // 暗くなるまで待つ
        yield return new WaitForSecondsRealtime(duration);

        // ゲームを終了する
#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;
#else
        Application.Quit();
#endif
    }

    private IEnumerator ReloadCoroutine(float duration)
    {
        // リロード時は基本的に同じ曲なのでBGMフェードアウトはしない

        // 画面を暗くする（フェードアウト）
        FadeOut(duration);

        // 暗くなるまで待つ
        yield return new WaitForSecondsRealtime(duration);

        // シーンを再読み込み
        SceneManager.LoadScene(SceneManager.GetActiveScene().name);

        // 画面を明るくする（フェードイン）
        FadeIn(duration);
    }

    public void FadeIn(float duration = -1f)
    {
        if (duration < 0f)
        {
            duration = defaultFadeTime;
        }
        StartFade(1f, 0f, duration);
    }

    public void FadeOut(float duration = -1f)
    {
        if (duration < 0f)
        {
            duration = defaultFadeTime;
        }
        StartFade(0f, 1f, duration);
    }

    private void StartFade(float startAlpha, float endAlpha, float duration)
    {
        if (fadeCoroutine != null)
        {
            StopCoroutine(fadeCoroutine);
        }
        fadeCoroutine = StartCoroutine(FadeCoroutine(startAlpha, endAlpha, duration));
    }

    private IEnumerator FadeCoroutine(float start, float end, float duration)
    {
        if (fadeCanvasGroup != null)
        {
            fadeCanvasGroup.blocksRaycasts = true;
        }

        float time = 0f;
        fadeImage.color = new Color(fadeColor.r, fadeColor.g, fadeColor.b, start);

        while (time < duration)
        {
            time += Time.unscaledDeltaTime;
            float t = Mathf.Clamp01(time / duration);
            float alpha = Mathf.Lerp(start, end, t);
            fadeImage.color = new Color(fadeColor.r, fadeColor.g, fadeColor.b, alpha);
            yield return null;
        }

        fadeImage.color = new Color(fadeColor.r, fadeColor.g, fadeColor.b, end);

        if (end == 0f && fadeCanvasGroup != null)
        {
            fadeCanvasGroup.blocksRaycasts = false;
        }

        fadeCoroutine = null;
    }
}
