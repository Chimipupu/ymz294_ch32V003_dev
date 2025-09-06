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

// YMZ294 Ch関連
#define YMZ294_TONE_CH_A                0
#define YMZ294_TONE_CH_B                1
#define YMZ294_TONE_CH_C                2

// YMZ294 ミキサー関連
#define MIXSER_OUTPUT_NONE              0x3F
#define MIXSER_OUTPUT_TONE_CH_A         0x07
#define MIXSER_OUTPUT_TONE_CH_B         0x05
#define MIXSER_OUTPUT_TONE_CH_C         0x03
#define MIXSER_OUTPUT_TONE_CH_A_B       0x04
#define MIXSER_OUTPUT_TONE_CH_A_C       0x02
#define MIXSER_OUTPUT_TONE_CH_B_C       0x01
#define MIXSER_OUTPUT_TONE_CH_A_B_C     0x00
#define MIXSER_OUTPUT_NOISE_CH_A        0x07
#define MIXSER_OUTPUT_NOISE_CH_B        0x05
#define MIXSER_OUTPUT_NOISE_CH_C        0x03
#define MIXSER_OUTPUT_NOISE_CH_A_B      0x04
#define MIXSER_OUTPUT_NOISE_CH_A_C      0x02
#define MIXSER_OUTPUT_NOISE_CH_B_C      0x01
#define MIXSER_OUTPUT_NOISE_CH_A_B_C    0x00

#define MIXSER_OUTPUT_ON                0
#define MIXSER_OUTPUT_OFF               1
#define MIXSER_CONFIG_TONE              0
#define MIXSER_CONFIG_NOISE             1
#define MIXSER_CONFIG_TONE_NOISE        0x80
#define MIXSER_OUTPUT_MUTE              0xFF     // ミュート(ミキサーでトーンとノイズ ChA~Cの出力OFF)

// YMZ294の制御ピン
#define YMZ294_WR_PIN     GPIO_Pin_4  // PD4
#define YMZ294_CS_PIN     GPIO_Pin_0  // PD0
#define YMZ294_A0_PIN     GPIO_Pin_0  // PC0
#define YMZ294_D0_PIN     GPIO_Pin_3  // PD3
#define YMZ294_D1_PIN     GPIO_Pin_2  // PD2
#define YMZ294_D2_PIN     GPIO_Pin_7  // PC7
#define YMZ294_D3_PIN     GPIO_Pin_6  // PC6
#define YMZ294_D4_PIN     GPIO_Pin_5  // PC5
#define YMZ294_D5_PIN     GPIO_Pin_4  // PC4
#define YMZ294_D6_PIN     GPIO_Pin_3  // PC3
#define YMZ294_D7_PIN     GPIO_Pin_7  // PD7

// YMZ294のレジスタ
#define YMZ294_REG_CNT                           0x0E // YMZ294のレジスタ数
#define YMZ294_REG_CH_A_SOUND_FREQ_ADDR          0x00 // Ch A 楽音周波数レジスタ TP[7:0]
#define YMZ294_REG_CH_A_SOUND_FREQ_2_ADDR        0x01 // Ch A 楽音周波数レジスタ TP[11:8]
#define YMZ294_REG_CH_B_SOUND_FREQ_ADDR          0x02 // Ch B 楽音周波数レジスタ TP[7:0]
#define YMZ294_REG_CH_B_SOUND_FREQ_2_ADDR        0x03 // Ch B 楽音周波数レジスタ TP[11:8]
#define YMZ294_REG_CH_C_SOUND_FREQ_ADDR          0x04 // Ch C 楽音周波数レジスタ TP[7:0]
#define YMZ294_REG_CH_C_SOUND_FREQ_2_ADDR        0x05 // Ch C 楽音周波数レジスタ TP[11:8]
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
} SOUND_FREQ_CH_A;

typedef union {
    uint8_t BYTE;
    struct {
        uint8_t TP8:1;          // bit0
        uint8_t TP9:1;          // bit1
        uint8_t TP10:1;         // bit2
        uint8_t TP11:1;         // bit3
        uint8_t RESERVED:4;     // bit[7:4]
    }BIT;
} SOUND_FREQ_CH_A_2;

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
} SOUND_FREQ_CH_B;

typedef union {
    uint8_t BYTE;
    struct {
        uint8_t TP8:1;          // bit0
        uint8_t TP9:1;          // bit1
        uint8_t TP10:1;         // bit2
        uint8_t TP11:1;         // bit3
        uint8_t RESERVED:4;     // bit[7:4]
    }BIT;
} SOUND_FREQ_CH_B_2;

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
} SOUND_FREQ_CH_C;

typedef union {
    uint8_t BYTE;
    struct {
        uint8_t TP8:1;          // bit0
        uint8_t TP9:1;          // bit1
        uint8_t TP10:1;         // bit2
        uint8_t TP11:1;         // bit3
        uint8_t RESERVED:4;     // bit[7:4]
    }BIT;
} SOUND_FREQ_CH_C_2;

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

void drv_ymz294_set_reg(uint8_t addr, uint8_t val);
uint8_t drv_ymz294_get_reg(uint8_t addr);
void drv_ymz294_mixser_config(uint8_t type, uint8_t val);
void drv_ymz294_init(void);

#endif // DRV_YMZ294_H