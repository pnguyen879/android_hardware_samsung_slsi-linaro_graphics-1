#define LOG_TAG "displaymodule"
#include "ExynosPrimaryDisplay.h"
#include "ExynosHWCModule.h"
#include "ExynosHWCUtils.h"
#include "ExynosMPPModule.h"
#include "ExynosHWCDebug.h"

ExynosPrimaryDisplay::ExynosPrimaryDisplay(int __unused numMPPs, struct exynos5_hwc_composer_device_1_t *pdev)
    :   ExynosOverlayDisplay(numMPPs, pdev)
{
}

ExynosPrimaryDisplay::~ExynosPrimaryDisplay()
{
}
