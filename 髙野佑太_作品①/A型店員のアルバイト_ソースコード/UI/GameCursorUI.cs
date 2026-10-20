// =============================================================
// 概要   : ゲームカーソルと商品情報表示を管理する
// 制作者 : 高野 佑太
// 制作日 : 2026/06/27
// =============================================================

using UnityEngine;
using UnityEngine.UI;
using TMPro;
using UnityEngine.InputSystem;

public class GameCursorUI : MonoBehaviour
{
   public static GameCursorUI Instance {  get; private set; }

    [Header("ゲームカーソルUI")]
    [SerializeField] private RectTransform cursorRect;
    [SerializeField] private Image cursorImage;
    [SerializeField] private TextMeshProUGUI timerText;

    [Header("ゲームカーソル画像設定")]
    [SerializeField] private Sprite normalCursorSprite; // 通常のカーソル画像
    [SerializeField] private Sprite holdingCursorSprite;// アイテムを持っている時の画像

    [Header("アイテム情報UI")]
    [SerializeField] private GameObject itemInfoPanel;
    [SerializeField] private TextMeshProUGUI itemNameText;
    [SerializeField] private TextMeshProUGUI itemAttrText;
    [SerializeField] private TextMeshProUGUI itemTimeText;
    [SerializeField] private Vector2 itemInfoOffset = new Vector2(20.0f, 20.0f);

    private bool isGlobalClicking = false; // グローバルクリック中かどうか
    private bool isHoldingItem = false;    // アイテムを持っているかどうか
    private bool inputSubscribed;

    private void Awake()
    {
        if (Instance != null && Instance != this)
        {
            Destroy(gameObject);
            return;
        }

        Instance = this;
        DontDestroyOnLoad(gameObject);
        Cursor.visible = false;
    }

    private void OnEnable()
    {
        SubscribeInput();
    }

    private void Start()
    {
        SubscribeInput();
    }

    private void OnDisable()
    {
        if (inputSubscribed && InputManager.Controls != null)
        {
            InputManager.Controls.Player.Grab.started -= OnGrabStarted;
            InputManager.Controls.Player.Grab.canceled -= OnGrabCanceled;
        }
        inputSubscribed = false;
    }

    private void OnDestroy()
    {
        if (Instance == this)
        {
            Instance = null;
        }
    }

    private void SubscribeInput()
    {
        if (Instance != this || inputSubscribed || InputManager.Controls == null)
        {
            return;
        }

        InputManager.Controls.Player.Grab.started += OnGrabStarted;
        InputManager.Controls.Player.Grab.canceled += OnGrabCanceled;
        inputSubscribed = true;
    }

    private void OnGrabStarted(InputAction.CallbackContext context)
    {
        OnGlobalClickStarted();
    }

    private void OnGrabCanceled(InputAction.CallbackContext context)
    {
        OnGlobalClickCanceled();
    }

    private void Update()
    {
        // マウスの位置にカーソルUIを追従させる
        Vector2 mousePos = InputManager.Controls.Player.Pointer.ReadValue<Vector2>();
        cursorRect.position = mousePos;

        if (itemInfoPanel != null && itemInfoPanel.activeSelf)
        {
            RectTransform infoRect = itemInfoPanel.transform as RectTransform;
            if (infoRect != null)
            {
                float pivotX = 0f;
                float pivotY = 0f;

                // マウスの位置に応じて情報パネルのピボットを設定
                if (mousePos.x > Screen.width / 2f)
                {
                    pivotX = 1.0f;
                }
                else
                {
                    pivotX = 0.0f;
                }

                // マウスの位置に応じて情報パネルのピボットを設定
                if (mousePos.y > Screen.height / 2f)
                {
                    pivotY = 1.0f;
                }
                else
                {
                    pivotY = 0.0f;
                }

                float offsetX = 0f;
                float offsetY = 0f;
                // X軸のオフセットを計算
                if (pivotX == 0f)
                {
                    offsetX = itemInfoOffset.x;
                }
                else
                {
                    offsetX = -itemInfoOffset.x;
                }
                // Y軸のオフセットを計算
                if (pivotY == 0f)
                {
                    offsetY = itemInfoOffset.y;
                }
                else
                {
                    offsetY = -itemInfoOffset.y;
                }

                // 情報パネルのピボットと位置を設定
                infoRect.pivot = new Vector2(pivotX, pivotY);
                infoRect.position = new Vector2(mousePos.x + offsetX,mousePos.y + offsetY);
            }
        }
    }

    // グローバルクリック開始時の処理
    private void OnGlobalClickStarted()
    {
        isGlobalClicking = true;
        UpdateCursorVisuals();
    }

    // グローバルクリック終了時の処理
    private void OnGlobalClickCanceled()
    {
        isGlobalClicking = false;
        UpdateCursorVisuals();
    }

    // Hands.cs から呼び出される、通常のカーソルに戻す処理
    public void SetNormalCursor()
    {
        isHoldingItem = false;

        if (timerText.gameObject.activeSelf)
        {
            timerText.gameObject.SetActive(false);
        }

        UpdateCursorVisuals();
    }

    // Hands.cs から呼び出される、アイテムを持っている時のカーソルに変更する処理
    public void SetHoldingCursor(float remainingTime)
    {
        isHoldingItem = true;

        if (!timerText.gameObject.activeSelf)
        {
            timerText.gameObject.SetActive(true);
        }

        timerText.text = remainingTime.ToString("F1");

        if (remainingTime <= 3.0f)
        {
            timerText.color = Color.red;
        }
        else
        {
            timerText.color = Color.green;
        }

        UpdateCursorVisuals();
    }

    // カーソルの見た目を更新する処理
    private void UpdateCursorVisuals()
    {
        // アイテムを持っているか、グローバルクリック中であれば持っているカーソルに変更
        if (isHoldingItem || isGlobalClicking)
        {
            if (cursorImage.sprite != holdingCursorSprite)
            {
                cursorImage.sprite = holdingCursorSprite;
            }
        }
        else
        {
            if (cursorImage.sprite != normalCursorSprite)
            {
                cursorImage.sprite = normalCursorSprite;
            }
        }
    }

    public void ShowItemInfo(string name, string attr, float time)
    {
        if (itemInfoPanel != null && !itemInfoPanel.activeSelf)
        {
            itemInfoPanel.SetActive(true);
        }

        if (itemNameText != null)
        {
            itemNameText.text = name;
        }
        if (itemAttrText != null)
        {
            itemAttrText.text = "特徴:" + attr;
        }
        if (itemTimeText != null)
        {
            itemTimeText.text = "持てる時間: " + time.ToString("F1") + "秒";
        }
    }

    public void HideItemInfo()
    {
        if (itemInfoPanel != null && itemInfoPanel.activeSelf)
        {
            itemInfoPanel.SetActive(false);
        }
    }
}
