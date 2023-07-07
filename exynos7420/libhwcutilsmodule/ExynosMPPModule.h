#ifndef EXYNOS_GSC_MODULE_H
#define EXYNOS_GSC_MODULE_H

#include "ExynosMPPv2.h"

class ExynosDisplay;

enum {
    eMPPRot1    =       1,
    eMPPRot2    =       2,
    eMPPRot3    =       3,
    eMPPRot4    =       4,
    eMPPRot5    =       5,
};

class ExynosMPPModule : public ExynosMPP {
    public:
        ExynosMPPModule();
        ExynosMPPModule(ExynosDisplay *display, unsigned int mppType, unsigned int mppIndex);
        virtual int isProcessingSupported(hwc_layer_1_t &layer, int dst_format);
        virtual int getMaxDownscale(hwc_layer_1_t &layer);
};

#endif
