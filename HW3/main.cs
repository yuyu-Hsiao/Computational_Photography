using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using Vuforia;

public class script : MonoBehaviour
{
    //------------------------ 文字
    public Text gameOverText;
    public Text startText;
    public Text timeText;
    public Text endtimeText;

    //------------------------ 河內塔的方塊
    public GameObject large;
    public GameObject medium;
    public GameObject small;

    //------------------------ marker
    public GameObject ImageTarget1;
    public GameObject ImageTarget2;
    public GameObject ImageTarget3;

    //------------------------ 虛擬按鈕
    public VirtualButtonBehaviour vb;

    private float gameTime;
    private bool isGameRunning;
    private int minutes;
    private int seconds;
    private string timeString;

    void Start()
    {
        //--------------------------------- 設定虛擬按鈕
        VirtualButtonBehaviour[] vbs = GetComponentsInChildren<VirtualButtonBehaviour>();
        for (int i = 0; i < vbs.Length; i++)
        {
            vbs[i].RegisterOnButtonPressed(OnButtonPressed);
            vbs[i].RegisterOnButtonReleased(OnButtonReleased);
        }

        //--------------------------------- 設置文字初始狀態
        gameOverText.gameObject.SetActive(false);
        startText.gameObject.SetActive(true);
        endtimeText.gameObject.SetActive(false);


        gameTime = 0f;              /*--- 初始時間 ---*/
        isGameRunning = false;  /*--- 遊戲是否在進行中 ---*/
    }


    // Update is called once per frame
    void Update()
    {
        if (isGameRunning)
        {
            gameTime += Time.deltaTime;
            UpdateTimeDisplay();
        }
    }

    void UpdateTimeDisplay()
    {
        //------------------------------------------ 將時間轉換為分鐘和秒的格式
        minutes = Mathf.FloorToInt(gameTime / 60f);
        seconds = Mathf.FloorToInt(gameTime % 60f);

        //------------------------------------------ 格式化時間文本
        timeString = string.Format("{0:00}:{1:00}", minutes, seconds);
        timeText.text = "Time: " + timeString;
    }

    int x = 0;
    //---------------------------------------------------------- 按下按鈕
    public void OnButtonPressed(VirtualButtonBehaviour vb)
    {
        Debug.Log("Pressed");
        print(vb.name + "按鈕按下");
        x++;

        //-------------------------------------------------------------- 每個 marker 的位置
        Vector3 target1WorldPosition = ImageTarget1.transform.position;
        Vector3 target2WorldPosition = ImageTarget2.transform.position;
        Vector3 target3WorldPosition = ImageTarget3.transform.position;

        //-------------------------------------------------------------- 遊戲中的操作
        if (x == 1)
        {
            isGameRunning = true;
            startText.gameObject.SetActive(false);
            small.transform.position = new Vector3(target3WorldPosition.x, target3WorldPosition.y, target3WorldPosition.z);
        }
        if (x == 2)
        {
            medium.transform.position = new Vector3(target2WorldPosition.x, target2WorldPosition.y, target2WorldPosition.z);
        }

        if (x == 3)
        {
            small.transform.position = new Vector3(target2WorldPosition.x, target2WorldPosition.y + 0.2f, target2WorldPosition.z);
        }
        if (x == 4)
        {
            large.transform.position = new Vector3(target3WorldPosition.x, target3WorldPosition.y, target3WorldPosition.z);
        }
        if (x == 5)
        {
            small.transform.position = new Vector3(target1WorldPosition.x, target1WorldPosition.y, target1WorldPosition.z);
        }
        if (x == 6)
        {
            medium.transform.position = new Vector3(target3WorldPosition.x, target3WorldPosition.y + 0.2f, target3WorldPosition.z);
        }
        if (x == 7)
        {
            small.transform.position = new Vector3(target3WorldPosition.x, target3WorldPosition.y + 0.4f, target3WorldPosition.z);
        }
        if (x == 8)
        {
            endtimeText.text = "Total Time: " + timeString;
            gameOverText.gameObject.SetActive(true);
            endtimeText.gameObject.SetActive(true);
            isGameRunning = false;
        }
    }

    //---------------------------------------------------------- 釋放按鈕
    public void OnButtonReleased(VirtualButtonBehaviour vb)
    {
        Debug.Log("Release");
        print(vb.name + "按鈕放開");
    }

}