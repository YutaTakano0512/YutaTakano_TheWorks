// =============================================================
// 概要   : メニュー選択カーソルの演出を管理する
// 制作者 : Takano Yuta
// 制作日 : 2026/06/28
// =============================================================

using UnityEngine;
using UnityEngine.EventSystems;
using DG.Tweening;
using UnityEngine.Serialization;

public class SelectCursor : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler, IPointerDownHandler
{
    [Header("アニメーション設定")]
    [SerializeField] private float expansionScale = 1.1f;  // 拡大時のスケール
    [SerializeField] private float clickScale = 0.9f;      // クリック時のスケール
    [SerializeField] private float duration = 0.15f;       // アニメーションの時間
    [SerializeField] private Ease hoverEase = Ease.OutQuad;// 拡大時のイージング
    [Header("サウンド設定")]
    [FormerlySerializedAs("ClickSE")]
    [SerializeField] private AudioClip clickSound;
    [FormerlySerializedAs("OnPointerSE")]
    [SerializeField] private AudioClip pointerSound;

    private Tween currentTween;

    public void OnPointerEnter(PointerEventData eventData)
    {
        // アニメーションを停止
        currentTween?.Kill();
        // 拡大アニメーションを開始
        currentTween = this.transform.DOScale(expansionScale, duration).SetEase(hoverEase).SetUpdate(true);

        if (AudioManager.Instance != null && pointerSound != null)
        {
            AudioManager.Instance.PlaySE(pointerSound);
        }
    }

    public void OnPointerExit(PointerEventData eventData)
    {
        // アニメーションを停止
        currentTween?.Kill();
        // 元のスケール(1.0)に戻す
        currentTween = this.transform.DOScale(1.0f, duration).SetEase(hoverEase).SetUpdate(true);
    }

    public void OnPointerDown(PointerEventData eventData)
    {
        // アニメーションを停止
        currentTween?.Kill();

        // Sequenceを作成し、アニメーションを順番に実行
        Sequence sequence = DOTween.Sequence();
        sequence.SetUpdate(true);

        // クリック時のスケールに縮小
        sequence.Append(this.transform.DOScale(clickScale, duration * 0.5f).SetEase(Ease.OutQuad));
        sequence.Append(this.transform.DOScale(expansionScale, duration * 1.5f).SetEase(Ease.OutBack));
        currentTween = sequence;

        if (AudioManager.Instance != null && clickSound != null)
        {
            AudioManager.Instance.PlaySE(clickSound);
        }
    }
}
