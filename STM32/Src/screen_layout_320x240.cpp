#include "fonts.h"
#include "lcd_driver.h"
#include "screen_layout.h"

#if (defined(LAY_320x240))

extern "C" constexpr STRUCT_LAYOUT_THEME LAYOUT_THEMES[LAYOUT_THEMES_COUNT] = {
    // Default
    {
        // Top row of status buttons
        .TOPBUTTONS_X1 = 0,
        .TOPBUTTONS_X2 = (LCD_WIDTH - 1),
        .TOPBUTTONS_Y1 = 1,
        .TOPBUTTONS_Y2 = 56,
        .TOPBUTTONS_WIDTH = 62,
        .TOPBUTTONS_HEIGHT = 15,
        .TOPBUTTONS_TB_MARGIN = 2,
        .TOPBUTTONS_LR_MARGIN = 2,
        .TOPBUTTONS_PRE_X = (float32_t)(LAYOUT_THEMES[0].TOPBUTTONS_X1 + LAYOUT_THEMES[0].TOPBUTTONS_LR_MARGIN),
        .TOPBUTTONS_PRE_Y = LAYOUT_THEMES[0].TOPBUTTONS_Y1,
        .TOPBUTTONS_ATT_X = (float32_t)(LAYOUT_THEMES[0].TOPBUTTONS_PRE_X + LAYOUT_THEMES[0].TOPBUTTONS_WIDTH + LAYOUT_THEMES[0].TOPBUTTONS_LR_MARGIN),
        .TOPBUTTONS_ATT_Y = LAYOUT_THEMES[0].TOPBUTTONS_Y1,
        .TOPBUTTONS_PGA_X = (float32_t)(LAYOUT_THEMES[0].TOPBUTTONS_ATT_X + LAYOUT_THEMES[0].TOPBUTTONS_WIDTH + LAYOUT_THEMES[0].TOPBUTTONS_LR_MARGIN),
        .TOPBUTTONS_PGA_Y = LAYOUT_THEMES[0].TOPBUTTONS_Y1,
        .TOPBUTTONS_DRV_X = (float32_t)(LAYOUT_THEMES[0].TOPBUTTONS_PGA_X + LAYOUT_THEMES[0].TOPBUTTONS_WIDTH + LAYOUT_THEMES[0].TOPBUTTONS_LR_MARGIN),
        .TOPBUTTONS_DRV_Y = LAYOUT_THEMES[0].TOPBUTTONS_Y1,
        .TOPBUTTONS_AGC_X = (float32_t)(LAYOUT_THEMES[0].TOPBUTTONS_DRV_X + LAYOUT_THEMES[0].TOPBUTTONS_WIDTH + LAYOUT_THEMES[0].TOPBUTTONS_LR_MARGIN),
        .TOPBUTTONS_AGC_Y = LAYOUT_THEMES[0].TOPBUTTONS_Y1,
        .TOPBUTTONS_SECOND_LINE_Y = (uint16_t)(LAYOUT_THEMES[0].TOPBUTTONS_Y1 + LAYOUT_THEMES[0].TOPBUTTONS_HEIGHT + LAYOUT_THEMES[0].TOPBUTTONS_TB_MARGIN),
        .TOPBUTTONS_DNR_X = (float32_t)(LAYOUT_THEMES[0].TOPBUTTONS_X1 + LAYOUT_THEMES[0].TOPBUTTONS_LR_MARGIN),
        .TOPBUTTONS_DNR_Y = LAYOUT_THEMES[0].TOPBUTTONS_SECOND_LINE_Y,
        .TOPBUTTONS_NB_X = (float32_t)(LAYOUT_THEMES[0].TOPBUTTONS_DNR_X + LAYOUT_THEMES[0].TOPBUTTONS_WIDTH + LAYOUT_THEMES[0].TOPBUTTONS_LR_MARGIN),
        .TOPBUTTONS_NB_Y = LAYOUT_THEMES[0].TOPBUTTONS_SECOND_LINE_Y,
        .TOPBUTTONS_FAST_X = (float32_t)(LAYOUT_THEMES[0].TOPBUTTONS_NB_X + LAYOUT_THEMES[0].TOPBUTTONS_WIDTH + LAYOUT_THEMES[0].TOPBUTTONS_LR_MARGIN),
        .TOPBUTTONS_FAST_Y = LAYOUT_THEMES[0].TOPBUTTONS_SECOND_LINE_Y,
        .TOPBUTTONS_MUTE_X = (float32_t)(LAYOUT_THEMES[0].TOPBUTTONS_FAST_X + LAYOUT_THEMES[0].TOPBUTTONS_WIDTH + LAYOUT_THEMES[0].TOPBUTTONS_LR_MARGIN),
        .TOPBUTTONS_MUTE_Y = LAYOUT_THEMES[0].TOPBUTTONS_SECOND_LINE_Y,
        .TOPBUTTONS_LOCK_X = (float32_t)(LAYOUT_THEMES[0].TOPBUTTONS_MUTE_X + LAYOUT_THEMES[0].TOPBUTTONS_WIDTH + LAYOUT_THEMES[0].TOPBUTTONS_LR_MARGIN),
        .TOPBUTTONS_LOCK_Y = LAYOUT_THEMES[0].TOPBUTTONS_SECOND_LINE_Y,
        // Clock
        .CLOCK_POS_Y = 17,
        .CLOCK_POS_HRS_X = (LCD_WIDTH - 75),
        .CLOCK_POS_MIN_X = (uint16_t)(LAYOUT_THEMES[0].CLOCK_POS_HRS_X + 25),
        .CLOCK_POS_SEC_X = (uint16_t)(LAYOUT_THEMES[0].CLOCK_POS_MIN_X + 25),
        .CLOCK_FONT = &FreeSans9pt7b,
        // WIFI
        .STATUS_WIFI_ICON_X = 0,
        .STATUS_WIFI_ICON_Y = 0,
        // SD
        .STATUS_SD_ICON_X = 0,
        .STATUS_SD_ICON_Y = 0,
        // FAN
        .STATUS_FAN_ICON_X = 0,
        .STATUS_FAN_ICON_Y = 0,
        // Frequency output
        .FREQ_A_LEFT = 0,
        .FREQ_X_OFFSET_100 = 60,
        .FREQ_X_OFFSET_10 = 80,
        .FREQ_X_OFFSET_1 = 100,
        .FREQ_X_OFFSET_KHZ = 130,
        .FREQ_X_OFFSET_HZ = 200,
        .FREQ_HEIGHT = 20,
        .FREQ_WIDTH = 220,
        .FREQ_TOP_OFFSET = 0,
        .FREQ_LEFT_MARGIN = 37,
        .FREQ_RIGHT_MARGIN = (uint16_t)(LCD_WIDTH - LAYOUT_THEMES[0].FREQ_LEFT_MARGIN - LAYOUT_THEMES[0].FREQ_WIDTH),
        .FREQ_BOTTOM_OFFSET = 8,
        .FREQ_BLOCK_HEIGHT = (uint16_t)(LAYOUT_THEMES[0].FREQ_HEIGHT + LAYOUT_THEMES[0].FREQ_TOP_OFFSET + LAYOUT_THEMES[0].FREQ_BOTTOM_OFFSET),
        .FREQ_Y_TOP = LAYOUT_THEMES[0].TOPBUTTONS_Y2,
        .FREQ_Y_BASELINE = (uint16_t)(LAYOUT_THEMES[0].TOPBUTTONS_Y2 + LAYOUT_THEMES[0].FREQ_HEIGHT + LAYOUT_THEMES[0].FREQ_TOP_OFFSET - 3),
        .FREQ_Y_BASELINE_SMALL = (uint16_t)(LAYOUT_THEMES[0].FREQ_Y_BASELINE - 2),
        .FREQ_FONT = &FreeSans18pt7b,
        .FREQ_SMALL_FONT = &FreeSans12pt7b,
        .FREQ_CH_FONT = &FreeSans12pt7b,
        .FREQ_CH_B_FONT = &FreeSans12pt7b,
        .FREQ_DELIMITER_Y_OFFSET = 0,
        .FREQ_DELIMITER_X1_OFFSET = 120,
        .FREQ_DELIMITER_X2_OFFSET = 190,
        // Display statuses under frequency
        .STATUS_Y_OFFSET = (uint16_t)(LAYOUT_THEMES[0].FREQ_Y_TOP + LAYOUT_THEMES[0].FREQ_BLOCK_HEIGHT + 1),
        .STATUS_HEIGHT = 30,
        .STATUS_BAR_X_OFFSET = 60,
        .STATUS_BAR_Y_OFFSET = 16,
        .STATUS_BAR_HEIGHT = 10,
        .STATUS_TXRX_X_OFFSET = 3,
        .STATUS_TXRX_Y_OFFSET = -50,
        .STATUS_TXRX_FONT = &FreeSans9pt7b,
        .STATUS_VFO_X_OFFSET = 0,
        .STATUS_VFO_Y_OFFSET = -43,
        .STATUS_VFO_BLOCK_WIDTH = 37,
        .STATUS_VFO_BLOCK_HEIGHT = 22,
        .STATUS_ANT_X_OFFSET = 0,
        .STATUS_ANT_Y_OFFSET = -23,
        .STATUS_ANT_BLOCK_WIDTH = 37,
        .STATUS_ANT_BLOCK_HEIGHT = 22,
        .STATUS_TX_LABELS_OFFSET_X = 5,
        .STATUS_TX_LABELS_MARGIN_X = 55,
        .STATUS_SMETER_ANALOG = false,
        .STATUS_SMETER_TOP_OFFSET = 0,
        .STATUS_SMETER_ANALOG_HEIGHT = 0,
        .STATUS_SMETER_WIDTH = 240,
        .STATUS_SMETER_MARKER_HEIGHT = 30,
        .STATUS_PMETER_WIDTH = 140,
        .STATUS_AMETER_WIDTH = 50,
        .STATUS_ALC_BAR_X_OFFSET = 10,
        .STATUS_LABELS_OFFSET_Y = 0,
        .STATUS_LABELS_FONT_SIZE = 1,
        .STATUS_LABEL_S_VAL_X_OFFSET = 12,
        .STATUS_LABEL_S_VAL_Y_OFFSET = 25,
        .STATUS_LABEL_S_VAL_FONT = &FreeSans7pt7b,
        .STATUS_LABEL_DBM_X_OFFSET = 5,
        .STATUS_LABEL_DBM_Y_OFFSET = 0,
        .STATUS_LABEL_BW_X_OFFSET = 0,
        .STATUS_LABEL_BW_Y_OFFSET = -65,
        .STATUS_LABEL_RIT_X_OFFSET = 160,
        .STATUS_LABEL_RIT_Y_OFFSET = 36,
        .STATUS_LABEL_THERM_X_OFFSET = 0,
        .STATUS_LABEL_THERM_Y_OFFSET = 36,
        .STATUS_LABEL_NOTCH_X_OFFSET = 0,
        .STATUS_LABEL_NOTCH_Y_OFFSET = 36,
        .STATUS_LABEL_FFT_BW_X_OFFSET = 0,
        .STATUS_LABEL_FFT_BW_Y_OFFSET = 36,
        // Information panel
        .STATUS_INFOA_X_OFFSET = 1,
        .STATUS_INFOB_X_OFFSET = 2,
        .STATUS_INFOC_X_OFFSET = 65,
        .STATUS_INFOD_X_OFFSET = 128,
        .STATUS_INFOE_X_OFFSET = 191,
        .STATUS_INFOF_X_OFFSET = 254,
        .STATUS_INFO_Y_OFFSET = 120,
        .STATUS_INFO_WIDTH = 63,
        .STATUS_INFO_HEIGHT = 12,
        .STATUS_LABEL_REC_X_OFFSET = 130,
        .STATUS_LABEL_REC_Y_OFFSET = 36,
        .STATUS_SMETER_PEAK_HOLDTIME = 1000,
        .STATUS_SMETER_TXLABELS_MARGIN = 55,
        .STATUS_SMETER_TXLABELS_PADDING = 23,
        .STATUS_TX_LABELS_VAL_WIDTH = 25,
        .STATUS_TX_LABELS_VAL_HEIGHT = 8,
        .STATUS_TX_LABELS_SWR_X = (uint16_t)(LAYOUT_THEMES[0].STATUS_BAR_X_OFFSET + LAYOUT_THEMES[0].STATUS_TX_LABELS_OFFSET_X + LAYOUT_THEMES[0].STATUS_SMETER_TXLABELS_PADDING),
        .STATUS_TX_LABELS_FWD_X = (uint16_t)(LAYOUT_THEMES[0].STATUS_BAR_X_OFFSET + LAYOUT_THEMES[0].STATUS_TX_LABELS_OFFSET_X + LAYOUT_THEMES[0].STATUS_SMETER_TXLABELS_MARGIN +
                                             LAYOUT_THEMES[0].STATUS_SMETER_TXLABELS_PADDING),
        .STATUS_TX_LABELS_REF_X = (uint16_t)(LAYOUT_THEMES[0].STATUS_BAR_X_OFFSET + LAYOUT_THEMES[0].STATUS_TX_LABELS_OFFSET_X + LAYOUT_THEMES[0].STATUS_SMETER_TXLABELS_MARGIN * 2 +
                                             LAYOUT_THEMES[0].STATUS_SMETER_TXLABELS_PADDING),
        .STATUS_TX_ALC_X_OFFSET = 40,
        .STATUS_MODE_X_OFFSET = (uint16_t)(LCD_WIDTH - LAYOUT_THEMES[0].FREQ_RIGHT_MARGIN + 10),
        .STATUS_MODE_Y_OFFSET = -38,
        .STATUS_MODE_BLOCK_WIDTH = 48,
        .STATUS_MODE_BLOCK_HEIGHT = 22,
        .STATUS_ERR_OFFSET_X = 290,
        .STATUS_ERR_OFFSET_Y = 65,
        .STATUS_ERR_WIDTH = 20,
        .STATUS_ERR_HEIGHT = 8,
        .TEXTBAR_FONT = 2,
        // bottom buttons
        .BOTTOM_BUTTONS_BLOCK_HEIGHT = 18,
        .BOTTOM_BUTTONS_BLOCK_TOP = (uint16_t)(LCD_HEIGHT - LAYOUT_THEMES[0].BOTTOM_BUTTONS_BLOCK_HEIGHT),
        .BOTTOM_BUTTONS_COUNT = FUNCBUTTONS_ON_PAGE,
        .BOTTOM_BUTTONS_ONE_WIDTH = (uint16_t)(LCD_WIDTH / LAYOUT_THEMES[0].BOTTOM_BUTTONS_COUNT),
        // FFT and waterfall CHECK MAX_FFT_HEIGHT AND MAX_WTF_HEIGHT defines !!!
        .FFT_HEIGHT_STYLE1 = 19,
        .WTF_HEIGHT_STYLE1 = 59,
        .FFT_HEIGHT_STYLE2 = 29,
        .WTF_HEIGHT_STYLE2 = 49,
        .FFT_HEIGHT_STYLE3 = 39,
        .WTF_HEIGHT_STYLE3 = 39,
        .FFT_HEIGHT_STYLE4 = 49,
        .WTF_HEIGHT_STYLE4 = 29,
        .FFT_HEIGHT_STYLE5 = 59,
        .WTF_HEIGHT_STYLE5 = 19,
        .FFT_PRINT_SIZE = LCD_WIDTH,
        .FFT_CWDECODER_OFFSET = 17,
        .FFT_FFTWTF_POS_Y = (uint16_t)(LCD_HEIGHT - LAYOUT_THEMES[0].FFT_HEIGHT_STYLE1 - LAYOUT_THEMES[0].WTF_HEIGHT_STYLE1 - LAYOUT_THEMES[0].BOTTOM_BUTTONS_BLOCK_HEIGHT),
        .FFT_FFTWTF_BOTTOM = (uint16_t)(LAYOUT_THEMES[0].FFT_FFTWTF_POS_Y + LAYOUT_THEMES[0].FFT_HEIGHT_STYLE1 + LAYOUT_THEMES[0].WTF_HEIGHT_STYLE1),
        .FFT_FREQLABELS_HEIGHT = 0,
        // System menu
        .SYSMENU_FONT_SIZE = 2,
        .SYSMENU_X1 = 5,
        .SYSMENU_X2 = 318,
        .SYSMENU_W = 340,
        .SYSMENU_ITEM_HEIGHT = 20,
        .SYSMENU_MAX_ITEMS_ON_PAGE = (uint16_t)(LCD_HEIGHT / LAYOUT_THEMES[0].SYSMENU_ITEM_HEIGHT),
        // Stuff
        .GREETINGS_X = (LCD_WIDTH / 2 - 5),
        .GREETINGS_Y = 25,
        // Tooltip
        .TOOLTIP_FONT = (const GFXfont *)&FreeSans9pt7b,
        .TOOLTIP_TIMEOUT = 1000,
        .TOOLTIP_MARGIN = 5,
        .TOOLTIP_POS_X = (LCD_WIDTH / 2),
        .TOOLTIP_POS_Y = 50,
        // BW Trapezoid
        .BW_TRAPEZ_POS_X = 0,
        .BW_TRAPEZ_POS_Y = 0,
        .BW_TRAPEZ_HEIGHT = 0,
        .BW_TRAPEZ_WIDTH = 0,
        // Touch buttons layout
        .BUTTON_PADDING = 1,
        .BUTTON_LIGHTER_WIDTH = 0.4f,
        .BUTTON_LIGHTER_HEIGHT = 4,
    },
};

#endif
