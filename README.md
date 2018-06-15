# SensorDataTCPSender

## 使用するデバイス
```
Arduino MKR1000
VL53L0X(time-of-flight distance sensor) x 4
```

## インポートする必要のあるライブラリ
```
VL53L0X
WiFi101
Packetizer
Sodaq_wdt
```
### インストール方法
-VL53L0X, WiFi101
  1. Arduino IDEのスケッチ-> ライブラリをインクルード-> ライブラリを管理  
  から、ライブラリマネージャを開く。  
  2. ライブラリを検索し、インストールをクリックする。 

-Packetizer, Sodaq_wdt
  1. 以下のレポジトリを.zipファイルとしてダウンロードする。  
  [Packetizer](https://github.com/hideakitai/Packetizer)  
  [Sodaq_wdt](https://github.com/SodaqMoja/Sodaq_wdt)  
  2. Arduino IDEのスケッチ-> ライブラリをインクルード-> .ZIP形式のライブラリをインストール...  
  からダウンロードした.zipファイルをそれぞれ選択する。


## 使い方
1. arduino_secrets.hの""内それぞれに利用するWiFiのSSIDとPASSを入力する。 
```
/* arduino_secrets.h */

#define SECRET_SSID ""
#define SECRET_PASS ""
```  
 
2. 4つのセンサVL53L0Xの各ピンとArduinoを下の図のように接続する。  
```
    Arduino   VL53L0X board  
    -------   -------------  
        VCC - VIN  
        GND - GND  
        SDA - SDA  
        SCL - SCL  
```
 
3. 4つのセンサVL53L0XのXSHUTピンをそれぞれArduinoの2, 3, 4, 5ピンに接続する。  
 
4. Arduinoにスケッチを書き込み、起動する。
 
5. クライアント側のPCから`python TCP_client.py`を実行する。
