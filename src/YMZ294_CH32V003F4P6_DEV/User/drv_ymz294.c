/**
 * @file drv_ymz294.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief  YMZ294ドライバ
 * @version 0.1
 * @date 2025-09-05
 *
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 *
 */

#include "drv_ymz294.h"

SOUND_FREQ_CH_A REG_SOUND_FREQ_CH_A;
SOUND_FREQ_CH_A_2 REG_SOUND_FREQ_CH_A_2;
SOUND_FREQ_CH_B REG_SOUND_FREQ_CH_B;
SOUND_FREQ_CH_B_2 REG_SOUND_FREQ_CH_B_2;
SOUND_FREQ_CH_C REG_SOUND_FREQ_CH_C;
SOUND_FREQ_CH_C_2 REG_SOUND_FREQ_CH_C_2;
NOISE_FREQ REG_NOISE_FREQ;
MIXSER REG_MIXSER;
VOLUME_CTRL_CH_A REG_VOLUME_CTRL_CH_A;
VOLUME_CTRL_CH_B REG_VOLUME_CTRL_CH_B;
VOLUME_CTRL_CH_C REG_VOLUME_CTRL_CH_C;
ENVELOPE_FREQ REG_ENVELOPE_FREQ;
ENVELOPE_FREQ_2 REG_ENVELOPE_FREQ_2;
ENVELOPE_TYPE REG_ENVELOPE_TYPE;

uint8_t *p_reg[YMZ294_REG_CNT] = {
    &REG_SOUND_FREQ_CH_A.BYTE,
    &REG_SOUND_FREQ_CH_A_2.BYTE,
    &REG_SOUND_FREQ_CH_B.BYTE,
    &REG_SOUND_FREQ_CH_B_2.BYTE,
    &REG_SOUND_FREQ_CH_C.BYTE,
    &REG_SOUND_FREQ_CH_C_2.BYTE,
    &REG_NOISE_FREQ.BYTE,
    &REG_MIXSER.BYTE,
    &REG_VOLUME_CTRL_CH_A.BYTE,
    &REG_VOLUME_CTRL_CH_B.BYTE,
    &REG_VOLUME_CTRL_CH_C.BYTE,
    &REG_ENVELOPE_FREQ.BYTE,
    &REG_ENVELOPE_FREQ_2.BYTE,
    &REG_ENVELOPE_TYPE.BYTE,
};

// YMZ294のレジスタのビットマスクテーブル
const uint8_t g_ymz294_reg_bit_mask_tbl[YMZ294_REG_CNT] = {
    0xFF, 0x0F, 0xFF, 0x0F, 0xFF, 0x0F, 0x1F, 0x3F, // Addr 0x00 ~ 0x07
    0x1F, 0x1F, 0x1F, 0xFF, 0xFF, 0x0F              // Addr 0x08 ~ 0x0D
};

// YMZ294用音階テーブル
const uint16_t g_tone_tp_tbl[] = {
    478, // ド  (C4) ... 261.63 Hz (中央)
    425, // レ  (D4) ... 293.66 Hz
    379, // ミ  (E4) ... 329.63 Hz
    358, // ファ(F4) ... 349.23 Hz
    319, // ソ  (G4) ... 392.00 Hz
    284, // ラ  (A4) ... 440.00 Hz (基準)
    253, // シ  (B4) ... 493.88 Hz

    239, // ド  (C5) ... 523.00 Hz
    213, // レ  (D5) ... 587.33 Hz
    190, // ミ  (E5) ... 659.26 Hz
    179, // ファ(F5) ... 698.46 Hz
    159, // ソ  (G5) ... 783.99 Hz
    142, // ラ  (A5) ... 880.00 Hz
    127, // シ  (B5) ... 987.77 Hz

    119, // ド  (C6) ... 1046.50 Hz
    106, // レ  (D6) ... 1174.66 Hz
    95,  // ミ  (E6) ... 1318.51 Hz
    89,  // ファ(F6) ... 1396.91 Hz
    80,  // ソ  (G6) ... 1567.98 Hz
    71,  // ラ  (A6) ... 1760.00 Hz
    63,  // シ  (B6) ... 1975.53 Hz
    60,  // ド  (C7) ... 2093.00 Hz
};

#if 0
// YMZ294用のMIDIノート番号テーブル
const uint16_t g_midi_notenum_tbl[] = {
//  C0     C#0     D0     D#0    E0     F0     F#0    G0
    15289, 14431, 13621, 12856, 12135, 11454, 10811, 10204, // MIDIノートナンバー 0~7

//  G#0    A0     A#0    B0     C1     C#1    D1     D#1
    9631,  9091,  8581,  8099,  7645,  7215,  6810,  6428,  // MIDIノートナンバー 8~15

//  E1     F1     F#1    G1     G#1    A1     A#1    B1
    6067,  5727,  5405,  5102,  4816,  4545,  4290,  4050,  // MIDIノートナンバー 16~23

//  C2     C#2    D2     D#2    E2     F2     F#2    G2
    3822,  3608,  3405,  3214,  3034,  2863,  2703,  2551,  // MIDIノートナンバー 24~31

//  G#2    A2     A#2    B2     C3     C#3    D3     D#3
    2408,  2273,  2145,  2025,  1911,  1804,  1703,  1607,  // MIDIノートナンバー 32~39

//  E3     F3     F#3    G3     G#3    A3     A#3    B3
    1517,  1432,  1351,  1276,  1204,  1136,  1073,  1012,  // MIDIノートナンバー 40~47

//  C4     C#4    D4     D#4    E4     F4     F#4    G4
    956,   902,   851,   804,   758,   716,   676,  638,    // MIDIノートナンバー 48~55

//  G#4    A4     A#4    B4     C5     C#5    D5     D#5
    602,   568,   536,   506,   478,   451,   426,   402,   // MIDIノートナンバー 56~63

//  E5     F5     F#5    G5     G#5    A5     A#5    B5
    379,   358,   338,   319,   301,   284,   268,   253,   // MIDIノートナンバー 64~71

//  C6     C#6    D6     D#6    E6     F6     F#6    G6
    239,   225,   213,   201,   190,   179,   169,   159,   // MIDIノートナンバー 72~79

//  G#6    A6     A#6    B6     C7     C#7    D7     D#7
    150,   142,   134,   127,   119,   113,   106,   100,   // MIDIノートナンバー 80~87

//  E7     F7     F#7    G7     G#7    A7     A#7    B7
    95,    89,    84,    80,    75,    71,    67,    63,    // MIDIノートナンバー 88~95

//  C8     C#8    D8     D#8    E8     F8     F#8    G8
    60,    56,    53,    50,    47,    45,    42,    40,    // MIDIノートナンバー 96~103

//  G#8    A8     A#8    B8     C9     C#9    D9     D#9
    38,    36,    34,    32,    30,    28,    27,    25,    // MIDIノートナンバー 104~111

//  E9     F9     F#9    G9     G#9    A9     A#9    B9
    24,    22,    21,    20,    19,    18,    17,    16,    // MIDIノートナンバー 112~119

//  C10    C#10   D10    D#10   E10    F10    F#10   G10
    15,    14,    13,    13,    12,    11,    11,    10,    // MIDIノートナンバー 120~127

    0                                                       // 無音
};
#endif

static void reg_init_all(void);
static void data_pin_set_byte(uint8_t val);

static void reg_init_all(void)
{
    uint8_t i;

    for (i = 0; i < YMZ294_REG_CNT; i++)
    {
        *p_reg[i] = 0;
    }

    *p_reg[YMZ294_REG_MIXER_ADDR] = MIXSER_OUTPUT_NONE;
}

static void data_pin_set_byte(uint8_t val)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        BitAction bit = ((val >> i) & 0x01) ? Bit_SET : Bit_RESET;

        switch (i)
        {
            case 0:
                GPIO_WriteBit(GPIOD, YMZ294_D0_PIN, bit);
                break;
            case 1:
                GPIO_WriteBit(GPIOD, YMZ294_D1_PIN, bit);
                break;
            case 2:
                GPIO_WriteBit(GPIOC, YMZ294_D2_PIN, bit);
                break;
            case 3:
                GPIO_WriteBit(GPIOC, YMZ294_D3_PIN, bit);
                break;
            case 4:
                GPIO_WriteBit(GPIOC, YMZ294_D4_PIN, bit);
                break;
            case 5:
                GPIO_WriteBit(GPIOC, YMZ294_D5_PIN, bit);
                break;
            case 6:
                GPIO_WriteBit(GPIOC, YMZ294_D6_PIN, bit);
                break;
            case 7:
                GPIO_WriteBit(GPIOD, YMZ294_D7_PIN, bit);
                break;
        }
    }
}

/**
 * @brief YMZ294のレジスタ値設定関数
 *
 * @param addr YMZ294のレジスタアドレス(0x00~0x0D)
 * @param val 設定するレジスタ値
 */
void drv_ymz294_set_reg(uint8_t addr, uint8_t val)
{
    *p_reg[addr] = val & g_ymz294_reg_bit_mask_tbl[addr];

    // アドレス設定
    GPIO_WriteBit(GPIOC, YMZ294_A0_PIN, Bit_RESET);
    GPIO_WriteBit(GPIOD, YMZ294_CS_PIN, Bit_RESET);
    GPIO_WriteBit(GPIOD, YMZ294_WR_PIN, Bit_RESET);
    data_pin_set_byte(addr);
    GPIO_WriteBit(GPIOD, YMZ294_CS_PIN, Bit_SET);
    GPIO_WriteBit(GPIOD, YMZ294_WR_PIN, Bit_SET);

    Delay_Us(100);

    // データ設定
    GPIO_WriteBit(GPIOC, YMZ294_A0_PIN, Bit_SET);
    GPIO_WriteBit(GPIOD, YMZ294_CS_PIN, Bit_RESET);
    GPIO_WriteBit(GPIOD, YMZ294_WR_PIN, Bit_RESET);
    data_pin_set_byte(val);
    GPIO_WriteBit(GPIOD, YMZ294_CS_PIN, Bit_SET);
    GPIO_WriteBit(GPIOD, YMZ294_WR_PIN, Bit_SET);
}

/**
 * @brief YMZ294のレジスタ値取得関数
 *
 * @param addr YMZ294のレジスタアドレス(0x00~0x0D)
 * @return uint8_t 取得したレジスタ値
 */
uint8_t drv_ymz294_get_reg(uint8_t addr)
{
    uint8_t tmp, reg;

    tmp = *p_reg[addr];
    reg = tmp & g_ymz294_reg_bit_mask_tbl[addr];

    return reg;
}

/**
 * @brief YMZ294の出力音量調整関数
 * 
 * @param ch Ch A~C
 * @param volume 音量 0~15の16段階
 */
void drv_ymz294_set_volume(uint8_t ch, uint8_t volume)
{
    switch (ch)
    {
        case YMZ294_TONE_CH_A:
            drv_ymz294_set_reg(YMZ294_REG_VOLUME_CTRL_CH_A_ADDR, volume);
            break;

        case YMZ294_TONE_CH_B:
            drv_ymz294_set_reg(YMZ294_REG_VOLUME_CTRL_CH_B_ADDR, volume);
            break;

        case YMZ294_TONE_CH_C:
            drv_ymz294_set_reg(YMZ294_REG_VOLUME_CTRL_CH_C_ADDR, volume);
            break;
    }
}

/**
 * @brief YMZ294のトーンの周波数をMIDIノートナンバーに応じて設定
 * 
 * @param ch Ch A~C
 * @param notenum MIDIノートナンバー0～127
 */
void drv_ymz294_set_tone_freq_midi_notenum(uint8_t ch, uint8_t notenum)
{
    uint16_t upper, lower;

#if 1
    upper = ((g_tone_tp_tbl[notenum] & 0x0F00) >> 8);
    lower = (g_tone_tp_tbl[notenum] & 0x00FF);
#else
    upper = (uint8_t)(g_midi_notenum_tbl[notenum] >> 8);
    lower = (uint8_t)(g_midi_notenum_tbl[notenum] & 0x00FF);
#endif

    switch (ch)
    {
        case YMZ294_TONE_CH_A:
            drv_ymz294_set_reg(YMZ294_REG_CH_A_SOUND_FREQ_2_ADDR, (uint8_t)upper);
            drv_ymz294_set_reg(YMZ294_REG_CH_A_SOUND_FREQ_ADDR,   (uint8_t)lower);
            break;

        case YMZ294_TONE_CH_B:
            drv_ymz294_set_reg(YMZ294_REG_CH_B_SOUND_FREQ_2_ADDR, (uint8_t)upper);
            drv_ymz294_set_reg(YMZ294_REG_CH_B_SOUND_FREQ_ADDR,   (uint8_t)lower);
            break;

        case YMZ294_TONE_CH_C:
            drv_ymz294_set_reg(YMZ294_REG_CH_C_SOUND_FREQ_2_ADDR, (uint8_t)upper);
            drv_ymz294_set_reg(YMZ294_REG_CH_C_SOUND_FREQ_ADDR,   (uint8_t)lower);
            break;
    }
}

/**
 * @brief YMZ294のミキサー出力(トーンとノイズ)設定
 * 
 * @param type CONFIG_TONE ... トーン出力、CONFIG_NOISE ... ノイズ出力
 * @param val Ch A~Cの値(各ビットが0 ... 出力、1 ... NC)
 */
void drv_ymz294_mixser_config(uint8_t type, uint8_t config)
{
    uint8_t tmp;

    tmp = *p_reg[YMZ294_REG_MIXER_ADDR];
    tmp = tmp & ~config;

    switch (type)
    {
        case MIXSER_CONFIG_TONE:
        case MIXSER_CONFIG_NOISE:
            drv_ymz294_set_reg(YMZ294_REG_MIXER_ADDR, tmp);
            break;

        case MIXSER_CONFIG_TONE_NOISE:
            drv_ymz294_set_reg(YMZ294_REG_MIXER_ADDR, 0x00);
            break;

        case MIXSER_OUTPUT_MUTE:
            drv_ymz294_set_reg(YMZ294_REG_MIXER_ADDR, MIXSER_OUTPUT_NONE);
            break;
    }
}

/**
 * @brief YMZ294 初期化関数
 * 
 */
void drv_ymz294_init(void)
{
    reg_init_all();

    drv_ymz294_mixser_config(MIXSER_CONFIG_TONE, MIXSER_OUTPUT_TONE_CH_A_B_C);
    drv_ymz294_set_volume(YMZ294_TONE_CH_A, 0x0F);
    drv_ymz294_set_volume(YMZ294_TONE_CH_B, 0x0F);
    drv_ymz294_set_volume(YMZ294_TONE_CH_C, 0x0F);
}

void ymz294_test(void)
{
    uint8_t i;
#ifdef DEBUG_PRINTF
    uint16_t tp;
#endif // DEBUG_PRINTF

    for (i = 0; i < 8; i++)
    {
        drv_ymz294_set_tone_freq_midi_notenum(YMZ294_TONE_CH_A, i);
        drv_ymz294_set_tone_freq_midi_notenum(YMZ294_TONE_CH_B, i+7);
        drv_ymz294_set_tone_freq_midi_notenum(YMZ294_TONE_CH_C, i+14);
#ifdef DEBUG_PRINTF
        tp = *p_reg[YMZ294_REG_CH_A_SOUND_FREQ_ADDR];
        tp = tp | (*p_reg[YMZ294_REG_CH_A_SOUND_FREQ_2_ADDR] << 8);
        printf("tp = %d\r\n", tp);
#endif // DEBUG_PRINTF
        Delay_Ms(1000);
    }
}