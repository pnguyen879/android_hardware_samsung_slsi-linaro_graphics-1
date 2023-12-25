#ifndef EXYNOS_SECONDARY_DISPLAY_MODULE_H
#define EXYNOS_SECONDARY_DISPLAY_MODULE_H

#include "ExynosSecondaryDisplay.h"

class ExynosSecondaryDisplayModule : public ExynosSecondaryDisplay {
    public:
        ExynosSecondaryDisplayModule(struct exynos5_hwc_composer_device_1_t *pdev);
        ~ExynosSecondaryDisplayModule();
    protected:
        virtual bool isOverlaySupported(hwc_layer_1_t &layer, size_t index, bool useVPPOverlay, ExynosMPPModule** supportedInternalMPP, ExynosMPPModule** supportedExternalMPP);
        virtual void doPreProcessing(hwc_display_contents_1_t* contents);
};

#endif
