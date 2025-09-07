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

TONE_FREQ_CH_A REG_TONE_FREQ_CH_A;
TONE_FREQ_CH_A_2 REG_TONE_FREQ_CH_A_2;
TONE_FREQ_CH_B REG_TONE_FREQ_CH_B;
TONE_FREQ_CH_B_2 REG_TONE_FREQ_CH_B_2;
TONE_FREQ_CH_C REG_TONE_FREQ_CH_C;
TONE_FREQ_CH_C_2 REG_TONE_FREQ_CH_C_2;
NOISE_FREQ REG_NOISE_FREQ;
MIXSER REG_MIXSER;
VOLUME_CTRL_CH_A REG_VOLUME_CTRL_CH_A;
VOLUME_CTRL_CH_B REG_VOLUME_CTRL_CH_B;
VOLUME_CTRL_CH_C REG_VOLUME_CTRL_CH_C;
ENVELOPE_FREQ REG_ENVELOPE_FREQ;
ENVELOPE_FREQ_2 REG_ENVELOPE_FREQ_2;
ENVELOPE_TYPE REG_ENVELOPE_TYPE;

uint8_t *p_reg[YMZ294_REG_CNT] = {
    &REG_TONE_FREQ_CH_A.BYTE,
    &REG_TONE_FREQ_CH_A_2.BYTE,
    &REG_TONE_FREQ_CH_B.BYTE,
    &REG_TONE_FREQ_CH_B_2.BYTE,
    &REG_TONE_FREQ_CH_C.BYTE,
    &REG_TONE_FREQ_CH_C_2.BYTE,
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

// 楽音アドレステーブル
const tone_addr_t g_tone_addr_tbl[] = {
    {YMZ294_REG_CH_A_TONE_FREQ_2_ADDR, YMZ294_REG_CH_A_TONE_FREQ_ADDR},
    {YMZ294_REG_CH_B_TONE_FREQ_2_ADDR, YMZ294_REG_CH_B_TONE_FREQ_ADDR},
    {YMZ294_REG_CH_C_TONE_FREQ_2_ADDR, YMZ294_REG_CH_C_TONE_FREQ_ADDR}
};

// YMZ294用音階テーブル
const uint16_t g_tone_tp_tbl[] = {
    0,    // 無音

    // オクターブ 0
    4545, // ラ  (A0) 27.50 Hz (最低音)
    4290, // ラ# (A#0) 29.14 Hz
    4050, // シ  (B0) 30.87 Hz

    // オクターブ 1
    3822, // ド  (C1) 32.70 Hz
    3608, // ド# (C#1) 34.65 Hz
    3405, // レ  (D1) 36.71 Hz
    3214, // レ# (D#1) 38.89 Hz
    3034, // ミ  (E1) 41.20 Hz
    2863, // ファ(F1) 43.65 Hz
    2703, // ファ#(F#1) 46.25 Hz
    2551, // ソ  (G1) 49.00 Hz
    2408, // ソ# (G#1) 51.91 Hz
    2273, // ラ  (A1) 55.00 Hz
    2145, // ラ# (A#1) 58.27 Hz
    2025, // シ  (B1) 61.74 Hz

    // オクターブ 2
    1911, // ド  (C2)  65.41 Hz
    1804, // ド# (C#2) 69.30 Hz
    1703, // レ  (D2)  73.42 Hz
    1607, // レ# (D#2) 77.78 Hz
    1517, // ミ  (E2)  82.41 Hz
    1432, // ファ(F2)  87.31 Hz
    1351, // ファ#(F#2) 92.50 Hz
    1276, // ソ  (G2)  98.00 Hz
    1204, // ソ# (G#2) 103.83 Hz
    1136, // ラ  (A2) 110.00 Hz
    1073, // ラ# (A#2) 116.54 Hz
    1012, // シ  (B2) 123.47 Hz

    // オクターブ 3
    956,  // ド  (C3) 130.81 Hz
    902,  // ド# (C#3) 138.59 Hz
    851,  // レ  (D3) 146.83 Hz
    803,  // レ# (D#3) 155.56 Hz
    758,  // ミ  (E3) 164.81 Hz
    716,  // ファ(F3) 174.61 Hz
    676,  // ファ#(F#3) 185.00 Hz
    638,  // ソ  (G3) 196.00 Hz
    602,  // ソ# (G#3) 207.65 Hz
    568,  // ラ  (A3) 220.00 Hz
    536,  // ラ# (A#3) 233.08 Hz
    506,  // シ  (B3) 246.94 Hz

    // オクターブ 4 (中央)
    478,  // ド  (C4) 261.63 Hz (中央)
    451,  // ド# (C#4) 277.18 Hz
    425,  // レ  (D4) 293.66 Hz
    402,  // レ# (D#4) 311.13 Hz
    379,  // ミ  (E4) 329.63 Hz
    358,  // ファ (F4) 349.23 Hz
    338,  // ファ# (F#4) 369.99 Hz
    319,  // ソ  (G4) 392.00 Hz
    301,  // ソ#  (G#4) 415.31 Hz
    284,  // ラ  (A4) 440.00 Hz (基準)
    268,  // ラ# (A#4) 466.16 Hz
    253,  // シ  (B4) 493.88 Hz

    // オクターブ 5
    239,  // ド  (C5) 523.25 Hz
    225,  // ド# (C#5) 554.37 Hz
    213,  // レ  (D5) 587.33 Hz
    201,  // レ# (D#5) 622.25 Hz
    190,  // ミ  (E5) 659.26 Hz
    179,  // ファ(F5) 698.46 Hz
    169,  // ファ#(F#5) 739.99 Hz
    159,  // ソ  (G5) 783.99 Hz
    150,  // ソ# (G#5) 830.61 Hz
    142,  // ラ  (A5) 880.00 Hz
    134,  // ラ# (A#5) 932.33 Hz
    127,  // シ  (B5) 987.77 Hz

    // オクターブ 6
    119,  // ド  (C6) 1046.50 Hz
    113,  // ド# (C#6) 1108.73 Hz
    106,  // レ  (D6) 1174.66 Hz
    100,  // レ# (D#6) 1244.51 Hz
    95,   // ミ  (E6) 1318.51 Hz
    89,   // ファ(F6) 1396.91 Hz
    84,   // ファ#(F#6) 1479.98 Hz
    80,   // ソ  (G6) 1567.98 Hz
    75,   // ソ# (G#6) 1661.22 Hz
    71,   // ラ  (A6) 1760.00 Hz
    67,   // ラ# (A#6) 1864.66 Hz
    63,   // シ  (B6) 1975.53 Hz

    // オクターブ 7
    60,   // ド  (C7) 2093.00 Hz
    56,   // ド# (C#7) 2217.46 Hz
    53,   // レ  (D7) 2349.32 Hz
    50,   // レ# (D#7) 2489.02 Hz
    47,   // ミ  (E7) 2637.02 Hz
    45,   // ファ(F7) 2793.83 Hz
    42,   // ファ#(F#7) 2959.96 Hz
    40,   // ソ  (G7) 3135.96 Hz
    38,   // ソ# (G#7) 3322.44 Hz
    36,   // ラ  (A7) 3520.00 Hz
    34,   // ラ# (A#7) 3729.31 Hz
    32,   // シ  (B7) 3951.07 Hz

    // オクターブ 8
    30,   // ド  (C8) 4186.01 Hz (ピアノの最高音)
#if 0
    28,   // ド# (C#8) 4434.92 Hz
    27,   // レ  (D8) 4698.64 Hz
    25,   // レ# (D#8) 4978.03 Hz
    24,   // ミ  (E8) 5274.04 Hz
    22,   // ファ(F8) 5587.65 Hz
    21,   // ファ#(F#8) 5919.91 Hz
    20,   // ソ  (G8) 6271.93 Hz
    19,   // ソ# (G#8) 6644.88 Hz
    18,   // ラ  (A8) 7040.00 Hz
    17,   // ラ# (A#8) 7458.62 Hz
    16,   // シ  (B8) 7902.13 Hz
#endif
};

static void reg_init_all(void);
static void data_pin_set_byte(uint8_t val);
static void set_tone_tp(uint8_t ch, uint8_t upper, uint8_t lower);
static void set_3_chord_tone_tp(tone_3_chord_data_t *p_tone_tbl);

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
                GPIO_WriteBit(GPIOC, YMZ294_D7_PIN, bit);
                break;
        }
    }
}

static void set_tone_tp(uint8_t ch, uint8_t upper, uint8_t lower)
{
    switch (ch)
    {
        case YMZ294_TONE_CH_A:
            drv_ymz294_set_reg(YMZ294_REG_CH_A_TONE_FREQ_2_ADDR, upper);
            drv_ymz294_set_reg(YMZ294_REG_CH_A_TONE_FREQ_ADDR,   lower);
            break;

        case YMZ294_TONE_CH_B:
            drv_ymz294_set_reg(YMZ294_REG_CH_B_TONE_FREQ_2_ADDR, upper);
            drv_ymz294_set_reg(YMZ294_REG_CH_B_TONE_FREQ_ADDR,   lower);
            break;

        case YMZ294_TONE_CH_C:
            drv_ymz294_set_reg(YMZ294_REG_CH_C_TONE_FREQ_2_ADDR, upper);
            drv_ymz294_set_reg(YMZ294_REG_CH_C_TONE_FREQ_ADDR,   lower);
            break;

        case YMZ294_TONE_CH_ALL:
            drv_ymz294_set_reg(YMZ294_REG_CH_A_TONE_FREQ_2_ADDR, upper);
            drv_ymz294_set_reg(YMZ294_REG_CH_A_TONE_FREQ_ADDR,   lower);

            drv_ymz294_set_reg(YMZ294_REG_CH_B_TONE_FREQ_2_ADDR, upper);
            drv_ymz294_set_reg(YMZ294_REG_CH_B_TONE_FREQ_ADDR,   lower);

            drv_ymz294_set_reg(YMZ294_REG_CH_C_TONE_FREQ_2_ADDR, upper);
            drv_ymz294_set_reg(YMZ294_REG_CH_C_TONE_FREQ_ADDR,   lower);
            break;
    }
}

static void set_3_chord_tone_tp(tone_3_chord_data_t *p_tone_tbl)
{
    uint8_t i;
    uint8_t upper = 0, lower = 0;
    uint8_t data_buf[3] = {p_tone_tbl->data_a, p_tone_tbl->data_b, p_tone_tbl->data_c};

    for (i = 0; i < 3; i++)
    {
        if(data_buf[i] > 0) {
            upper = ((g_tone_tp_tbl[data_buf[i]] & 0x0F00) >> 8);
            lower = (g_tone_tp_tbl[data_buf[i]] & 0x00FF);
        }

        drv_ymz294_set_reg(g_tone_addr_tbl[i].upper_addr, upper);
        drv_ymz294_set_reg(g_tone_addr_tbl[i].lower_addr, lower);
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

    Delay_Us(1);

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
 * @brief YMZ294のトーンの周波数設定関数
 * 
 * @param ch Ch A~C
 * @param freq 設定したい音階
 */
void drv_ymz294_set_tone_freq(uint8_t ch, uint16_t tone)
{
    uint8_t upper = 0;
    uint8_t lower = 0;

    if(tone > 0) {
        upper = ((g_tone_tp_tbl[tone] & 0x0F00) >> 8);
        lower = (g_tone_tp_tbl[tone] & 0x00FF);
    }

    set_tone_tp(ch, upper, lower);
}

/**
 * @brief YMZ294のトーンの3和音の周波数設定関数
 * 
 * @param ch Ch A~C
 * @param freq 設定したい音階
 */
void drv_ymz294_set_3_chord_tone_freq(tone_3_chord_data_t *p_tone_tbl)
{
    set_3_chord_tone_tp(p_tone_tbl);
}

/**
 * @brief ３和音すべての周波数を0にする関数
 * 
 * @param ch 
 */
void drv_ymz294_set_tone_off(uint8_t ch)
{
    set_tone_tp(ch, 0, 0);
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
    *p_reg[YMZ294_REG_MIXER_ADDR] = tmp;

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

#ifdef YMZ294_DRV_DEBUG
    ymz294_test();
#endif // YMZ294_DRV_DEBUG

    drv_ymz294_set_tone_off(YMZ294_TONE_CH_ALL);
    drv_ymz294_mixser_config(MIXSER_CONFIG_TONE, MIXSER_OUTPUT_TONE_CH_A_B_C);
    drv_ymz294_set_volume(YMZ294_TONE_CH_A, 0x0F);
    drv_ymz294_set_volume(YMZ294_TONE_CH_B, 0x0F);
    drv_ymz294_set_volume(YMZ294_TONE_CH_C, 0x0F);
}

/**
 * @brief 単音演奏用
 * 
 * @param p_tone_tbl 楽譜テーブル
 * @param size 曲長
 */
void drv_ymz294_play_music_tone(const uint8_t *p_tone_tbl, uint16_t size)
{
    uint8_t i;
    uint8_t *p_tbl;

    p_tbl = (uint8_t *)p_tone_tbl;

    drv_ymz294_set_tone_off(YMZ294_TONE_CH_ALL);
    drv_ymz294_mixser_config(MIXSER_CONFIG_TONE, MIXSER_OUTPUT_TONE_CH_A);
    drv_ymz294_set_volume(YMZ294_TONE_CH_A, 0x0F);

    for (i = 0; i < size; i++)
    {
        drv_ymz294_set_tone_freq(YMZ294_TONE_CH_A, *p_tbl);
        p_tbl++;
        Delay_Ms(350);
    }

    drv_ymz294_set_volume(YMZ294_TONE_CH_A, 0x00);
    drv_ymz294_set_tone_off(YMZ294_TONE_CH_ALL);
    drv_ymz294_mixser_config(MIXSER_OUTPUT_MUTE, 0);
}

/**
 * @brief 3和音演奏用
 * 
 * @param p_tone_tbl 楽譜テーブル
 * @param size 曲長
 */
void drv_ymz294_play_music_chord_tone(const tone_3_chord_data_t *p_tone_tbl, uint16_t size)
{
    uint8_t i;

    drv_ymz294_set_tone_off(YMZ294_TONE_CH_ALL);
    drv_ymz294_mixser_config(MIXSER_CONFIG_TONE, MIXSER_OUTPUT_TONE_CH_A_B_C);
    drv_ymz294_set_volume(YMZ294_TONE_CH_A, 0x0F);
    drv_ymz294_set_volume(YMZ294_TONE_CH_B, 0x0F);
    drv_ymz294_set_volume(YMZ294_TONE_CH_C, 0x0F);

    for (i = 0; i < size; i++)
    {
        drv_ymz294_set_tone_freq(YMZ294_TONE_CH_A, p_tone_tbl->data_a);
        drv_ymz294_set_tone_freq(YMZ294_TONE_CH_B, p_tone_tbl->data_b);
        drv_ymz294_set_tone_freq(YMZ294_TONE_CH_C, p_tone_tbl->data_c);
        p_tone_tbl++;
        Delay_Ms(350);
    }

    drv_ymz294_set_volume(YMZ294_TONE_CH_A, 0x00);
    drv_ymz294_set_volume(YMZ294_TONE_CH_B, 0x00);
    drv_ymz294_set_volume(YMZ294_TONE_CH_C, 0x00);
    drv_ymz294_set_tone_off(YMZ294_TONE_CH_ALL);
    drv_ymz294_mixser_config(MIXSER_OUTPUT_MUTE, 0);
}

#ifdef YMZ294_DRV_DEBUG
void ymz294_test(void)
{
    // NOP
}
#endif // YMZ294_DRV_DEBUG