// =============================================================
// 概要   : ワールド空間のポップアップ文字を表示する
// 制作者 : Takano Yuta
// 制作日 : 2026/06/27
// =============================================================

using UnityEngine;
using TMPro;

public class FloatingText3D : MonoBehaviour
{
    [SerializeField] private float moveSpeed = 1.5f;
    [SerializeField] private float lifeTime = 1.5f;

    private TextMeshPro textComponent;
    private Camera targetCamera;
    private float timer = 0;

    public void Setup(string text, Color color)
    {
        textComponent = GetComponent<TextMeshPro>();

        if (textComponent == null)
        {
            textComponent = gameObject.AddComponent<TextMeshPro>();
        }
        textComponent.text = text;
        textComponent.color = color;

        // カメラ未生成時にもポップアップを初期化できるようにする。
        targetCamera = Camera.main;
        if (targetCamera != null)
        {
            transform.LookAt(transform.position + targetCamera.transform.rotation * Vector3.forward,
                targetCamera.transform.rotation * Vector3.up);
        }
    }

    private void LateUpdate()
    {
        if (targetCamera == null)
        {
            targetCamera = Camera.main;
        }

        if (targetCamera != null)
        {
            transform.LookAt(transform.position + targetCamera.transform.rotation * Vector3.forward,
                targetCamera.transform.rotation * Vector3.up);
        }
    }

    private void Update()
    {
        transform.position += Vector3.up * moveSpeed * Time.deltaTime;
        timer += Time.deltaTime;
        // Inspector で寿命を 0 にしても除算で不正値を作らない。
        float alpha = 1.0f - (timer / Mathf.Max(lifeTime, Mathf.Epsilon));
        textComponent.color = new Color(textComponent.color.r, textComponent.color.g, textComponent.color.b, alpha);

        if (timer >= lifeTime)
        {
            Destroy(gameObject);
        }
    }
}
