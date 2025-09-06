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

// YMZ294音階テーブルインデックス定義
typedef enum {
    TONE_TBL_C4 = 0,    // ド  (C4) 261.63 Hz (中央)
    TONE_TBL_D4,        // レ  (D4) 293.66 Hz
    TONE_TBL_E4,        // ミ  (E4) 329.63 Hz
    TONE_TBL_F4,        // ファ(F4) 349.23 Hz
    TONE_TBL_G4,        // ソ  (G4) 392.00 Hz
    TONE_TBL_A4,        // ラ  (A4) 440.00 Hz (基準音)
    TONE_TBL_B4,        // シ  (B4) 493.88 Hz

    TONE_TBL_C5,        // ド  (C5) 523.00 Hz
    TONE_TBL_D5,        // レ  (D5) 587.33 Hz
    TONE_TBL_E5,        // ミ  (E5) 659.26 Hz
    TONE_TBL_F5,        // ファ(F5) 698.46 Hz
    TONE_TBL_G5,        // ソ  (G5) 783.99 Hz
    TONE_TBL_A5,        // ラ  (A5) 880.00 Hz
    TONE_TBL_B5,        // シ  (B5) 987.77 Hz

    TONE_TBL_C6,        // ド  (C6) 1046.50 Hz
    TONE_TBL_D6,        // レ  (D6) 1174.66 Hz
    TONE_TBL_E6,        // ミ  (E6) 1318.51 Hz
    TONE_TBL_F6,        // ファ(F6) 1396.91 Hz
    TONE_TBL_G6,        // ソ  (G6) 1567.98 Hz
    TONE_TBL_A6,        // ラ  (A6) 1760.00 Hz
    TONE_TBL_B6,        // シ  (B6) 1975.53 Hz
    TONE_TBL_C7,        // ド  (C7) 2093.00 Hz
} e_tone_tbl_idx;

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

static void reg_init_all(void);
static void data_pin_set_byte(uint8_t val);
static void set_tone_tp(uint8_t ch, uint8_t upper, uint8_t lower);

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

static void set_tone_tp(uint8_t ch, uint8_t upper, uint8_t lower)
{
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
 * @brief YMZ294のトーンの周波数設定関数
 * 
 * @param ch Ch A~C
 * @param freq 設定したい音階
 */
void drv_ymz294_set_tone_freq(uint8_t ch, uint16_t tone)
{
    uint16_t upper, lower;

    upper = ((g_tone_tp_tbl[tone] & 0x0F00) >> 8);
    lower = (g_tone_tp_tbl[tone] & 0x00FF);

    set_tone_tp(ch, upper, lower);
}

void drv_ymz294_set_tone_off(uint8_t ch)
{
    switch (ch)
    {
        case YMZ294_TONE_CH_A:
            drv_ymz294_set_reg(YMZ294_REG_CH_A_SOUND_FREQ_2_ADDR, 0);
            drv_ymz294_set_reg(YMZ294_REG_CH_A_SOUND_FREQ_ADDR,   0);
            break;

        case YMZ294_TONE_CH_B:
            drv_ymz294_set_reg(YMZ294_REG_CH_B_SOUND_FREQ_2_ADDR, 0);
            drv_ymz294_set_reg(YMZ294_REG_CH_B_SOUND_FREQ_ADDR,   0);
            break;

        case YMZ294_TONE_CH_C:
            drv_ymz294_set_reg(YMZ294_REG_CH_C_SOUND_FREQ_2_ADDR, 0);
            drv_ymz294_set_reg(YMZ294_REG_CH_C_SOUND_FREQ_ADDR,   0);
            break;

        case YMZ294_TONE_CH_ALL:
            drv_ymz294_set_reg(YMZ294_REG_CH_A_SOUND_FREQ_2_ADDR, 0);
            drv_ymz294_set_reg(YMZ294_REG_CH_A_SOUND_FREQ_ADDR,   0);

            drv_ymz294_set_reg(YMZ294_REG_CH_B_SOUND_FREQ_2_ADDR, 0);
            drv_ymz294_set_reg(YMZ294_REG_CH_B_SOUND_FREQ_ADDR,   0);

            drv_ymz294_set_reg(YMZ294_REG_CH_C_SOUND_FREQ_2_ADDR, 0);
            drv_ymz294_set_reg(YMZ294_REG_CH_C_SOUND_FREQ_ADDR,   0);
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

#if 0
    ymz294_test();
#endif

    drv_ymz294_mixser_config(MIXSER_CONFIG_TONE, MIXSER_OUTPUT_TONE_CH_A_B_C);
    drv_ymz294_set_volume(YMZ294_TONE_CH_A, 0x0F);
    drv_ymz294_set_volume(YMZ294_TONE_CH_B, 0x0F);
    drv_ymz294_set_volume(YMZ294_TONE_CH_C, 0x0F);
}

void ymz294_test(void)
{
    uint8_t i;

    drv_ymz294_mixser_config(MIXSER_CONFIG_TONE, MIXSER_OUTPUT_TONE_CH_A_B_C);
    drv_ymz294_set_volume(YMZ294_TONE_CH_A, 0x0F);
    drv_ymz294_set_volume(YMZ294_TONE_CH_B, 0x0F);
    drv_ymz294_set_volume(YMZ294_TONE_CH_C, 0x0F);

    for (i = 0; i < 8; i++)
    {
        drv_ymz294_set_tone_freq(YMZ294_TONE_CH_A, i);
        drv_ymz294_set_tone_freq(YMZ294_TONE_CH_B, i+7);
        drv_ymz294_set_tone_freq(YMZ294_TONE_CH_C, i+14);
        Delay_Ms(1000);
    }

    drv_ymz294_set_tone_off(YMZ294_TONE_CH_ALL);
    drv_ymz294_mixser_config(MIXSER_OUTPUT_MUTE, 0);
}