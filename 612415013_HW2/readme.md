# 3D Ferris Wheel Scene with Interactive Controls

- **作業環境**：Windows 11 64bit
- **編譯器**：Visual Studio 2022

---

## 功能說明

### 1. 3D 場景建立
- **地板**：建立一塊白色地板作為基礎。
- **摩天輪模型**：包含支撐架、兩個平行的輪子，以及車廂。

### 2. 摩天輪動畫
- **轉動效果**：摩天輪輪子會旋轉，車廂將隨之一起轉動。

### 3. 使用者介面（UI）按鈕
- **HORIZONTAL VIEW 滑桿**：可旋轉水平視角。
- **Quit 按鈕**：離開程式。
- **start/stop 按鈕**：控制摩天輪的旋轉狀態。
  - 按鈕顯示藍色：摩天輪停止轉動。
  - 按鈕顯示紅色：摩天輪正在轉動。

### 4. 光源效果
- **點光源 (Point Light)**：增添光源，使摩天輪更加生動。
- **Light Color 選單**：提供燈光顏色選擇，包括白色、紅色、綠色、藍色四種基本顏色。

### 5. 小鴨模型
- **加入小鴨**：讓小鴨坐上摩天輪。

---

## 額外功能

### 1. ROTATE VIEW 滑桿
- 調整垂直視角，以達到俯視的效果。

### 2. VERTICAL VIEW 滑桿
- 調整摩天輪的高度，從上方或下方觀看模型。

### 3. DISTANCE VIEW 滑桿
- 調整遠近距離觀看模型。

### 4. ROTATE SPEED 滑桿
- 控制摩天輪旋轉的速度。

### 5. 滑桿顯示數值
- 在滑桿旁顯示當前數值，便於確認角度、距離及旋轉速度等狀態。

### 6. 車廂數量調整
- 可使用「+」和「-」按鈕調整車廂數量（最多 12 個，最少 1 個）。

### 7. 環境光源
- 除了點光源外，增設環境光源。

### 8. Switch 按鈕
- 控制環境光的開啟與關閉。

### 9. 更多 Light Color 選項
- 除了基本的白色、紅色、綠色、藍色，增加黃色、紫色、青色選項。

### 10. 小鴨視角
- 增加小鴨的視角，從小鴨的視點觀看場景。

### 11. Duck Color 按鈕
- 改變小鴨的顏色，有紅色、綠色、藍色及黃色選項。

---

## 參考資料
- [SMFLoader.h](https://www.cs.ccu.edu.tw/~damon/graph/model/SMFLoader.h)
- [SMFLoader.cpp](https://www.cs.ccu.edu.tw/~damon/graph/model/SMFLoader.cpp)
- [main.cpp](https://www.cs.ccu.edu.tw/~damon/graph/model/main.cpp)
- [CSDN 教學文章](https://blog.csdn.net/upcdxlq/article/details/79456022)

---

## 執行方式
1. Visual Studio 2022 並設置為 64 位編譯環境。
2. 將程式碼匯入 Visual Studio。
3. 按下 **Start** 或執行按鈕以啟動程式。

