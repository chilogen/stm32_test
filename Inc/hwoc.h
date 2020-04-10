//
// Created by a on 4/9/20.
//

#ifndef STM32_TEST_HWOC_H
#define STM32_TEST_HWOC_H

#define REPORTMSG 0
#define SETMSG 1
#define QUERTMSG 2

struct HWOCReportMSG {
    uint8_t msgId;
    uint8_t led0;
    uint8_t led1;
    uint8_t beep;
};

struct HWOCSetMSG {
    uint8_t msgId;
    uint8_t num;
    uint8_t state;
};

struct HWOCQueryMSG {
    uint8_t msgID;
};

typedef struct  {
    uint8_t msgId;
    uint8_t *data;
}HWOCMSG;

void msgEncode(HWOCMSG *src, char **dst);

void msgDecode(char *src, HWOCMSG *dst);

#endif //STM32_TEST_HWOC_H
