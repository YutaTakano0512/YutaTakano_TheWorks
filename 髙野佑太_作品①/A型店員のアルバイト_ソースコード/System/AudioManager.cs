// =============================================================
// 概要   : シーン別BGMと効果音を管理する
// 制作者 : 高野 佑太
// 制作日 : 2026/06/29
// =============================================================

using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;

[System.Serializable]
public class SceneBgmMapping
{
    [Header("シーン名 (例: Title, InGame)")]
    public string sceneName;
    [Header("再生するBGM")]
    public AudioClip bgmClip;
}

public class AudioManager : MonoBehaviour
{
    // シングルトンインスタンス
    public static AudioManager Instance {  get; private set; }

    [Header("オーディオソース")]
    [SerializeField] private AudioSource bgmSource;
    [SerializeField] private AudioSource seSource;

    [Header("シーンごとのBGM設定（アプローチ2）")]
    [SerializeField] private SceneBgmMapping[] sceneBgmSettings;
    [SerializeField] private float defaultBgmFadeTime = 1.0f;

    [Header("設定UI")]
    [SerializeField] private GameObject settingsCanvasPrefab;
    private GameObject settingsCanvasInstance;

    [Header("マスター音量（0.0～1.0）")]
    public float masterBgmVolume = 1.0f;
    public float masterSeVolume = 1.0f;

    // 現在実行中のフェードコルーチンを保持する変数
    private Coroutine fadeCoroutine;

    private void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
            DontDestroyOnLoad(this.gameObject);
        }
        else
        {
            Destroy(this.gameObject);
        }

        // 前回保存した音量をロード
        masterBgmVolume = PlayerPrefs.GetFloat("BGM_Volume", 1.0f);
        masterSeVolume = PlayerPrefs.GetFloat("SE_Volume", 1.0f);

        bgmSource.volume = masterBgmVolume;
        seSource.volume = masterSeVolume;

        if (settingsCanvasPrefab != null && settingsCanvasInstance == null)
        {
            settingsCanvasInstance = Instantiate(settingsCanvasPrefab);
            DontDestroyOnLoad(settingsCanvasInstance);

            // 最初は非表示にしておく
            settingsCanvasInstance.SetActive(false);
        }
    }

    private void OnEnable()
    {
        SceneManager.sceneLoaded += OnSceneLoaded;
    }

    private void OnDisable()
    {
        SceneManager.sceneLoaded -= OnSceneLoaded;
    }

    private void OnSceneLoaded(Scene scene, LoadSceneMode mode)
    {
        // シーンがロードされたら、対応するBGMを再生する
        AudioClip nextClip = GetBgmForScene(scene.name);
        if (nextClip != null)
        {
            PlayBGM(nextClip, defaultBgmFadeTime);
        }
    }

    private AudioClip GetBgmForScene(string sceneName)
    {
        if (sceneBgmSettings == null)
        {
            return null;
        }
        foreach (var mapping in sceneBgmSettings)
        {
            if (mapping.sceneName == sceneName)
            {
                return mapping.bgmClip;
            }
        }
        return null;
    }

    // FadeManagerからシーン遷移前に呼ばれる（曲が違う場合のみフェードアウト）
    public void CheckAndFadeOutBGM(string nextSceneName, float fadeDuration)
    {
        AudioClip nextClip = GetBgmForScene(nextSceneName);

        // 次のシーンのBGMが今のBGMと違う場合（または次のBGMが無い場合）、フェードアウトを開始する
        if (bgmSource.isPlaying && bgmSource.clip != nextClip)
        {
            if (fadeCoroutine != null)
            {
                StopCoroutine(fadeCoroutine);
            }
            fadeCoroutine = StartCoroutine(FadeOutOnlyCoroutine(fadeDuration));
        }
    }

    private IEnumerator FadeOutOnlyCoroutine(float duration)
    {
        float startVol = bgmSource.volume;
        float time = 0;
        while (time < duration)
        {
            time += Time.unscaledDeltaTime;
            bgmSource.volume = Mathf.Lerp(startVol, 0f, time / duration);
            yield return null;
        }
        bgmSource.Stop();
        bgmSource.volume = masterBgmVolume; // 音量を元に戻しておく（次に再生するため）
    }

    // BGM再生
    public void PlayBGM(AudioClip clip, float fadeDuration = 1.0f)
    {
        if (bgmSource == null || clip == null)
        {
            return;
        }

        // 同じ曲が既に再生中なら何もしない
        if (bgmSource.clip == clip && bgmSource.isPlaying)
        {
            return;
        }

        // 実行中のフェード処理があれば停止
        if (fadeCoroutine != null)
        {
            StopCoroutine(fadeCoroutine);
        }

        // 新しいフェード処理を開始
        fadeCoroutine = StartCoroutine(FadeBGM(clip, fadeDuration));
    }

    // SE再生
    public void PlaySE(AudioClip clip)
    {
        if (seSource == null || clip == null)
        {
            return;
        }
        seSource.PlayOneShot(clip,masterSeVolume);
    }

    // BGM音量変更
    public void SetBGMVolume(float volume)
    {
        masterBgmVolume = volume;
        bgmSource.volume = masterBgmVolume;
        PlayerPrefs.SetFloat("BGM_Volume", masterBgmVolume);
    }

    // SE音量変更
    public void SetSEVolume(float volume)
    {
        masterSeVolume = volume;
        seSource.volume = masterSeVolume;
        PlayerPrefs.SetFloat("SE_Volume", masterSeVolume);
    }

    // フェードアウト ＆ フェードインを行うコルーチン
    private IEnumerator FadeBGM(AudioClip nextClip, float duration)
    {
        // 再生中ならフェードアウト
        if (bgmSource.isPlaying)
        {
            float startVol = bgmSource.volume;
            float time = 0;
            while (time < duration)
            {
                time += Time.unscaledDeltaTime;
                bgmSource.volume = Mathf.Lerp(startVol, 0f, time / duration);
                yield return null;
            }
            bgmSource.Stop();
        }

        // 次の曲をセットしてフェードイン
        bgmSource.clip = nextClip;
        bgmSource.Play();
        float fadeInTime = 0;

        while (fadeInTime < duration)
        {
            fadeInTime += Time.unscaledDeltaTime;
            bgmSource.volume = Mathf.Lerp(0f, masterBgmVolume, fadeInTime / duration);
            yield return null;
        }

        // 最後に音量を確定させる
        bgmSource.volume = masterBgmVolume;
    }

    public void OpenSettings()
    {
        if (settingsCanvasInstance != null)
        {
            settingsCanvasInstance.SetActive(true);
        }
    }

    public void CloseSettings()
    {
        if (settingsCanvasInstance != null)
        {
            settingsCanvasInstance.SetActive(false);
        }
    }
}
