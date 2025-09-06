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
    data_pin_set_byte(addr);
    GPIO_WriteBit(GPIOD, YMZ294_WR_PIN, Bit_RESET);
    GPIO_WriteBit(GPIOD, YMZ294_CS_PIN, Bit_RESET);
    GPIO_WriteBit(GPIOC, YMZ294_A0_PIN, Bit_SET);

    // データ設定
    data_pin_set_byte(val);
    GPIO_WriteBit(GPIOD, YMZ294_WR_PIN, Bit_RESET);
    GPIO_WriteBit(GPIOD, YMZ294_CS_PIN, Bit_RESET);
    GPIO_WriteBit(GPIOC, YMZ294_A0_PIN, Bit_RESET);
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
 * @brief ミキサーからトーンとノイズの出力設定
 * 
 * @param type CONFIG_TONE ... トーン出力、CONFIG_NOISE ... ノイズ出力
 * @param val Ch A~Cの値(各ビットが0 ... 出力、1 ... NC)
 */
void drv_ymz294_mixser_config(uint8_t type, uint8_t config)
{
    switch (type)
    {
        case MIXSER_CONFIG_TONE:
            drv_ymz294_set_reg(YMZ294_REG_MIXER_ADDR, config);
            break;

        case MIXSER_CONFIG_NOISE:
            drv_ymz294_set_reg(YMZ294_REG_MIXER_ADDR, config << 3);
            break;

        case MIXSER_CONFIG_TONE_NOISE:
            drv_ymz294_set_reg(YMZ294_REG_MIXER_ADDR, MIXSER_OUTPUT_NOISE_CH_A_B_C);
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

    drv_ymz294_mixser_config(MIXSER_CONFIG_TONE_NOISE, 0);
}