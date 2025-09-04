# YMZ294ドライバ開発(CH32V003)

YMZ294のドライバをCH32V003で個人開発しているリポジトリ

## 開発環境

### H/W

- 音源IC
  - [YMZ294](https://akizukidenshi.com/catalog/g/g112141/)🔗
    - 矩形波 ... x3ch
    - ノイズ ... x1ch
    - エンベロープ ... x1ch

- マイコン ... 型番:[CH32V003F4P6](https://akizukidenshi.com/catalog/g/g118061/)🔗
  - CPU ... [QingKeV2 (32bit RV32EC RISC-V)](https://wch-ic.com/downloads/QingKeV2_Processor_Manual_PDF.html)🔗
  - ROM ... 16KB
  - RAM ... 2KB
  - Clock ... HSIで48MHz動作
  - GPIO ... 18本
  - DMA ... (未使用)
  - タイマー ... (未使用)
  - I2C ... (未使用)
  - SPI ... (未使用)
  - UART ... (未使用)
  - ADC ... (未使用)

### S/W

- IDE/SDK/コンパイラ
  - [MounRiver Studio (MRS) V2.20](https://www.mounriver.com/download)🔗
- 最適化
  - デバッグ時 ... `-Og`
  - 通常 ... `-O2`

### デバッガ

- [WCH-LinkE Ver1.3](https://akizukidenshi.com/catalog/g/g118065)🔗

### デバッグ

#### SDI

- [WCH-LinkE <--> 評価基板]
  - SWDIO <--> PD1ピン
  - GND <--> GND

#### printf()

- SDIでprintf()をする（USRTは使わない）
  - F/W
    - `#define SDI_PRINT    SDI_PR_OPEN`を追加
  - MounRiver Studio
    - `Download Settings`で`Enable SDI Printf`にチェックを入れる
    - `Debug Settings`のStartupタブで`Enable Semihosting`にチェックを入れる
