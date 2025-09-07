/**
 * @file drv_ymz294.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief  YMZ294ドライバ
 * @version 0.1
 * @date 2025-09-05
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#ifndef DRV_YMZ294_H
#define DRV_YMZ294_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <ch32v00x.h>

#include "common.h"

// (DEBUG)
// #define YMZ294_DRV_DEBUG

// YMZ294 Ch関連
#define YMZ294_TONE_CH_A                        0
#define YMZ294_TONE_CH_B                        1
#define YMZ294_TONE_CH_C                        2
#define YMZ294_TONE_CH_ALL                      0xFF

// YMZ294 ミキサー関連
#define MIXSER_OUTPUT_NONE                      0x3F
#define MIXSER_OUTPUT_TONE_CH_A                 0x01
#define MIXSER_OUTPUT_TONE_CH_B                 0x02
#define MIXSER_OUTPUT_TONE_CH_C                 0x04
#define MIXSER_OUTPUT_TONE_CH_A_B               0x03
#define MIXSER_OUTPUT_TONE_CH_A_C               0x05
#define MIXSER_OUTPUT_TONE_CH_B_C               0x06
#define MIXSER_OUTPUT_TONE_CH_A_B_C             0x07
#define MIXSER_OUTPUT_NOISE_CH_A                0x08
#define MIXSER_OUTPUT_NOISE_CH_B                0x10
#define MIXSER_OUTPUT_NOISE_CH_C                0x20
#define MIXSER_OUTPUT_NOISE_CH_A_B              0x18
#define MIXSER_OUTPUT_NOISE_CH_A_C              0x28
#define MIXSER_OUTPUT_NOISE_CH_B_C              0x30
#define MIXSER_OUTPUT_NOISE_CH_A_B_C            0x38

#define MIXSER_OUTPUT_ON                        0
#define MIXSER_OUTPUT_OFF                       1
#define MIXSER_CONFIG_TONE                      0
#define MIXSER_CONFIG_NOISE                     1
#define MIXSER_CONFIG_TONE_NOISE                0x80
#define MIXSER_OUTPUT_MUTE                      0xFF     // ミュート(ミキサーでトーンとノイズ ChA~Cの出力OFF)

// YMZ294の制御ピン
#define YMZ294_WR_PIN                           GPIO_Pin_4  // PD4
#define YMZ294_CS_PIN                           GPIO_Pin_0  // PD0
#define YMZ294_A0_PIN                           GPIO_Pin_0  // PC0
#define YMZ294_D0_PIN                           GPIO_Pin_3  // PD3
#define YMZ294_D1_PIN                           GPIO_Pin_2  // PD2
#define YMZ294_D2_PIN                           GPIO_Pin_7  // PC7
#define YMZ294_D3_PIN                           GPIO_Pin_6  // PC6
#define YMZ294_D4_PIN                           GPIO_Pin_5  // PC5
#define YMZ294_D5_PIN                           GPIO_Pin_4  // PC4
#define YMZ294_D6_PIN                           GPIO_Pin_3  // PC3
#define YMZ294_D7_PIN                           GPIO_Pin_2  // PC2

// YMZ294のレジスタ
#define YMZ294_REG_CNT                           0x0E // YMZ294のレジスタ数
#define YMZ294_REG_CH_A_TONE_FREQ_ADDR          0x00 // Ch A 楽音周波数レジスタ TP[7:0]
#define YMZ294_REG_CH_A_TONE_FREQ_2_ADDR        0x01 // Ch A 楽音周波数レジスタ TP[11:8]
#define YMZ294_REG_CH_B_TONE_FREQ_ADDR          0x02 // Ch B 楽音周波数レジスタ TP[7:0]
#define YMZ294_REG_CH_B_TONE_FREQ_2_ADDR        0x03 // Ch B 楽音周波数レジスタ TP[11:8]
#define YMZ294_REG_CH_C_TONE_FREQ_ADDR          0x04 // Ch C 楽音周波数レジスタ TP[7:0]
#define YMZ294_REG_CH_C_TONE_FREQ_2_ADDR        0x05 // Ch C 楽音周波数レジスタ TP[11:8]
#define YMZ294_REG_NOISE_ADDR                    0x06 // ノイズ周波数レジスタ NP[4:0]
#define YMZ294_REG_MIXER_ADDR                    0x07 // ミキサーレジスタ
#define YMZ294_REG_VOLUME_CTRL_CH_A_ADDR         0x08 // 音量コントロールDAC Ch Aレジスタ
#define YMZ294_REG_VOLUME_CTRL_CH_B_ADDR         0x09 // 音量コントロールDAC Ch Bレジスタ
#define YMZ294_REG_VOLUME_CTRL_CH_C_ADDR         0x0A // 音量コントロールDAC Ch Cレジスタ
#define YMZ294_REG_ENVELOPE_FREQ_ADDR            0x0B // エンベロープ周波数レジスタ
#define YMZ294_REG_ENVELOPE_FREQ_2_ADDR          0x0C // エンベロープ周波数レジスタ
#define YMZ294_REG_ENVELOPE_TYPE_ADDR            0x0D // エンベロープ形状レジスタ

typedef union {
    uint8_t BYTE;
    struct {
        uint8_t TP0:1;     // bit0
        uint8_t TP1:1;     // bit1
        uint8_t TP2:1;     // bit2
        uint8_t TP3:1;     // bit3
        uint8_t TP4:1;     // bit4
        uint8_t TP5:1;     // bit5
        uint8_t TP6:1;     // bit6
        uint8_t TP7:1;     // bit7
    }BIT;
} TONE_FREQ_CH_A;

typedef union {
    uint8_t BYTE;
    struct {
        uint8_t TP8:1;          // bit0
        uint8_t TP9:1;          // bit1
        uint8_t TP10:1;         // bit2
        uint8_t TP11:1;         // bit3
        uint8_t RESERVED:4;     // bit[7:4]
    }BIT;
} TONE_FREQ_CH_A_2;

typedef union {
    uint8_t BYTE;
    struct {
        uint8_t TP0:1;     // bit0
        uint8_t TP1:1;     // bit1
        uint8_t TP2:1;     // bit2
        uint8_t TP3:1;     // bit3
        uint8_t TP4:1;     // bit4
        uint8_t TP5:1;     // bit5
        uint8_t TP6:1;     // bit6
        uint8_t TP7:1;     // bit7
    }BIT;
} TONE_FREQ_CH_B;

typedef union {
    uint8_t BYTE;
    struct {
        uint8_t TP8:1;          // bit0
        uint8_t TP9:1;          // bit1
        uint8_t TP10:1;         // bit2
        uint8_t TP11:1;         // bit3
        uint8_t RESERVED:4;     // bit[7:4]
    }BIT;
} TONE_FREQ_CH_B_2;

typedef union {
    uint8_t BYTE;
    struct {
        uint8_t TP0:1;     // bit0
        uint8_t TP1:1;     // bit1
        uint8_t TP2:1;     // bit2
        uint8_t TP3:1;     // bit3
        uint8_t TP4:1;     // bit4
        uint8_t TP5:1;     // bit5
        uint8_t TP6:1;     // bit6
        uint8_t TP7:1;     // bit7
    }BIT;
} TONE_FREQ_CH_C;

typedef union {
    uint8_t BYTE;
    struct {
        uint8_t TP8:1;          // bit0
        uint8_t TP9:1;          // bit1
        uint8_t TP10:1;         // bit2
        uint8_t TP11:1;         // bit3
        uint8_t RESERVED:4;     // bit[7:4]
    }BIT;
} TONE_FREQ_CH_C_2;

typedef union {
    uint8_t BYTE;
    struct {
        uint8_t NP0:1;          // bit0
        uint8_t NP1:1;          // bit1
        uint8_t NP2:1;          // bit2
        uint8_t NP3:1;          // bit3
        uint8_t NP4:1;          // bit4
        uint8_t RESERVED:3;     // bit[7:5]
    }BIT;
} NOISE_FREQ;

typedef union {
    uint8_t BYTE;
    struct {
        uint8_t TONE_A:1;       // bit0
        uint8_t TONE_B:1;       // bit1
        uint8_t TONE_C:1;       // bit2
        uint8_t NOSE_A:1;       // bit3
        uint8_t NOSE_B:1;       // bit4
        uint8_t NOSE_C:1;       // bit5
        uint8_t RESERVED:2;     // bit[7:6]
    }BIT;
} MIXSER;

typedef union {
    uint8_t BYTE;
    struct {
        uint8_t L0:1;          // bit0
        uint8_t L1:1;          // bit1
        uint8_t L2:1;          // bit2
        uint8_t L3:1;          // bit3
        uint8_t M:1;           // bit4
        uint8_t RESERVED:3;    // bit[7:5]
    }BIT;
} VOLUME_CTRL_CH_A;

typedef union {
    uint8_t BYTE;
    struct {
        uint8_t L0:1;          // bit0
        uint8_t L1:1;          // bit1
        uint8_t L2:1;          // bit2
        uint8_t L3:1;          // bit3
        uint8_t M:1;           // bit4
        uint8_t RESERVED:3;    // bit[7:5]
    }BIT;
} VOLUME_CTRL_CH_B;

typedef union {
    uint8_t BYTE;
    struct {
        uint8_t L0:1;          // bit0
        uint8_t L1:1;          // bit1
        uint8_t L2:1;          // bit2
        uint8_t L3:1;          // bit3
        uint8_t M:1;           // bit4
        uint8_t RESERVED:3;    // bit[7:5]
    }BIT;
} VOLUME_CTRL_CH_C;

typedef union {
    uint8_t BYTE;
    struct {
        uint8_t EP0:1;     // bit0
        uint8_t EP1:1;     // bit1
        uint8_t EP2:1;     // bit2
        uint8_t EP3:1;     // bit3
        uint8_t EP4:1;     // bit4
        uint8_t EP5:1;     // bit5
        uint8_t EP6:1;     // bit6
        uint8_t EP7:1;     // bit7
    }BIT;
} ENVELOPE_FREQ;

typedef union {
    uint8_t BYTE;
    struct {
        uint8_t EP8:1;      // bit0
        uint8_t EP9:1;      // bit1
        uint8_t EP10:1;     // bit2
        uint8_t EP11:1;     // bit3
        uint8_t EP12:1;     // bit4
        uint8_t EP13:1;     // bit5
        uint8_t EP14:1;     // bit6
        uint8_t EP15:1;     // bit7
    }BIT;
} ENVELOPE_FREQ_2;

typedef union {
    uint8_t BYTE;
    struct {
        uint8_t HOLD:1;        // bit0
        uint8_t ALT:1;         // bit1
        uint8_t ATT:1;         // bit2
        uint8_t CONT:1;        // bit3
        uint8_t RESERVED:4;    // bit[7:4]
    }BIT;
} ENVELOPE_TYPE;

// 楽音アドレス構造体
typedef struct
{
    uint8_t upper_addr; // 上位アドレス
    uint8_t lower_addr; // 下位アドレス
} tone_addr_t;

// 3和音データ構造体
typedef struct
{
    uint8_t data_a; // Ch A用データ
    uint8_t data_b; // Ch B用データ
    uint8_t data_c; // Ch C用データ
} tone_3_chord_data_t;

// YMZ294 音階
typedef enum {
    TONE_NONE = 0,   // 無音

    // オクターブ 0
    TONE_A0,        // ラ  (A0) 27.50 Hz (最低音)
    TONE_AS0,       // ラ# (A#0) 29.14 Hz
    TONE_B0,        // シ  (B0) 30.87 Hz

    // オクターブ 1
    TONE_C1,        // ド  (C1) 32.70 Hz
    TONE_CS1,       // ド# (C#1) 34.65 Hz
    TONE_D1,        // レ  (D1) 36.71 Hz
    TONE_DS1,       // レ# (D#1) 38.89 Hz
    TONE_E1,        // ミ  (E1) 41.20 Hz
    TONE_F1,        // ファ(F1) 43.65 Hz
    TONE_FS1,       // ファ#(F#1) 46.25 Hz
    TONE_G1,        // ソ  (G1) 49.00 Hz
    TONE_GS1,       // ソ# (G#1) 51.91 Hz
    TONE_A1,        // ラ  (A1) 55.00 Hz
    TONE_AS1,       // ラ# (A#1) 58.27 Hz
    TONE_B1,        // シ  (B1) 61.74 Hz

    // オクターブ 2
    TONE_C2,        // ド  (C2) 65.41 Hz
    TONE_CS2,       // ド# (C#2) 69.30 Hz
    TONE_D2,        // レ  (D2) 73.42 Hz
    TONE_DS2,       // レ# (D#2) 77.78 Hz
    TONE_E2,        // ミ  (E2) 82.41 Hz
    TONE_F2,        // ファ(F2) 87.31 Hz
    TONE_FS2,       // ファ#(F#2) 92.50 Hz
    TONE_G2,        // ソ  (G2) 98.00 Hz
    TONE_GS2,       // ソ# (G#2) 103.83 Hz
    TONE_A2,        // ラ  (A2) 110.00 Hz
    TONE_AS2,       // ラ# (A#2) 116.54 Hz
    TONE_B2,        // シ  (B2) 123.47 Hz

    // オクターブ 3
    TONE_C3,        // ド  (C3) 130.81 Hz
    TONE_CS3,       // ド# (C#3) 138.59 Hz
    TONE_D3,        // レ  (D3) 146.83 Hz
    TONE_DS3,       // レ# (D#3) 155.56 Hz
    TONE_E3,        // ミ  (E3) 164.81 Hz
    TONE_F3,        // ファ(F3) 174.61 Hz
    TONE_FS3,       // ファ#(F#3) 185.00 Hz
    TONE_G3,        // ソ  (G3) 196.00 Hz
    TONE_GS3,       // ソ# (G#3) 207.65 Hz
    TONE_A3,        // ラ  (A3) 220.00 Hz
    TONE_AS3,       // ラ# (A#3) 233.08 Hz
    TONE_B3,        // シ  (B3) 246.94 Hz

    // オクターブ 4 (中央)
    TONE_C4,        // ド  (C4) 261.63 Hz (中央)
    TONE_CS4,       // ド# (C#4) 277.18 Hz
    TONE_D4,        // レ  (D4) 293.66 Hz
    TONE_DS4,       // レ# (D#4) 311.13 Hz
    TONE_E4,        // ミ  (E4) 329.63 Hz
    TONE_F4,        // ファ (F4) 349.23 Hz
    TONE_FS4,       // ファ# (F#4) 369.99 Hz
    TONE_G4,        // ソ  (G4) 392.00 Hz
    TONE_GS4,       // ソ#  (G#4) 415.31 Hz
    TONE_A4,        // ラ  (A4) 440.00 Hz (基準音)
    TONE_AS4,       // ラ# (A#4) 466.16 Hz
    TONE_B4,        // シ  (B4) 493.88 Hz

    // オクターブ 5
    TONE_C5,        // ド  (C5) 523.25 Hz
    TONE_CS5,       // ド# (C#5) 554.37 Hz
    TONE_D5,        // レ  (D5) 587.33 Hz
    TONE_DS5,       // レ# (D#5) 622.25 Hz
    TONE_E5,        // ミ  (E5) 659.26 Hz
    TONE_F5,        // ファ(F5) 698.46 Hz
    TONE_FS5,       // ファ#(F#5) 739.99 Hz
    TONE_G5,        // ソ  (G5) 783.99 Hz
    TONE_GS5,       // ソ# (G#5) 830.61 Hz
    TONE_A5,        // ラ  (A5) 880.00 Hz
    TONE_AS5,       // ラ# (A#5) 932.33 Hz
    TONE_B5,        // シ  (B5) 987.77 Hz

    // オクターブ 6
    TONE_C6,        // ド  (C6) 1046.50 Hz
    TONE_CS6,       // ド# (C#6) 1108.73 Hz
    TONE_D6,        // レ  (D6) 1174.66 Hz
    TONE_DS6,       // レ# (D#6) 1244.51 Hz
    TONE_E6,        // ミ  (E6) 1318.51 Hz
    TONE_F6,        // ファ(F6) 1396.91 Hz
    TONE_FS6,       // ファ#(F#6) 1479.98 Hz
    TONE_G6,        // ソ  (G6) 1567.98 Hz
    TONE_GS6,       // ソ# (G#6) 1661.22 Hz
    TONE_A6,        // ラ  (A6) 1760.00 Hz
    TONE_AS6,       // ラ# (A#6) 1864.66 Hz
    TONE_B6,        // シ  (B6) 1975.53 Hz

    // オクターブ 7
    TONE_C7,        // ド  (C7) 2093.00 Hz
    TONE_CS7,       // ド# (C#7) 2217.46 Hz
    TONE_D7,        // レ  (D7) 2349.32 Hz
    TONE_DS7,       // レ# (D#7) 2489.02 Hz
    TONE_E7,        // ミ  (E7) 2637.02 Hz
    TONE_F7,        // ファ(F7) 2793.83 Hz
    TONE_FS7,       // ファ#(F#7) 2959.96 Hz
    TONE_G7,        // ソ  (G7) 3135.96 Hz
    TONE_GS7,       // ソ# (G#7) 3322.44 Hz
    TONE_A7,        // ラ  (A7) 3520.00 Hz
    TONE_AS7,       // ラ# (A#7) 3729.31 Hz
    TONE_B7,        // シ  (B7) 3951.07 Hz

    // オクターブ 8
    TONE_C8,        // ド  (C8) 4186.01 Hz (ピアノの最高音)
#if 0
    TONE_CS8,       // ド# (C#8) 4434.92 Hz
    TONE_D8,        // レ  (D8) 4698.64 Hz
    TONE_DS8,       // レ# (D#8) 4978.03 Hz
    TONE_E8,        // ミ  (E8) 5274.04 Hz
    TONE_F8,        // ファ(F8) 5587.65 Hz
    TONE_FS8,       // ファ#(F#8) 5919.91 Hz
    TONE_G8,        // ソ  (G8) 6271.93 Hz
    TONE_GS8,       // ソ# (G#8) 6644.88 Hz
    TONE_A8,        // ラ  (A8) 7040.00 Hz
    TONE_AS8,       // ラ# (A#8) 7458.62 Hz
    TONE_B8,        // シ  (B8) 7902.13 Hz
#endif
} e_tone_tbl_idx;

void drv_ymz294_set_reg(uint8_t addr, uint8_t val);
uint8_t drv_ymz294_get_reg(uint8_t addr);
void drv_ymz294_set_volume(uint8_t ch, uint8_t volume);
void drv_ymz294_set_tone_freq(uint8_t ch, uint16_t tone);
void drv_ymz294_set_3_chord_tone_freq(tone_3_chord_data_t *p_tone_tbl);
void drv_ymz294_set_tone_off(uint8_t ch);
void drv_ymz294_play_music_tone(const uint8_t *p_tone_tbl, uint16_t size);
void drv_ymz294_play_music_chord_tone(const tone_3_chord_data_t *p_tone_tbl, uint16_t size);
void drv_ymz294_mixser_config(uint8_t type, uint8_t val);
void drv_ymz294_init(void);

#ifdef YMZ294_DRV_DEBUG
void ymz294_test(void);
#endif // YMZ294_DRV_DEBUG

#endif // DRV_YMZ294_H