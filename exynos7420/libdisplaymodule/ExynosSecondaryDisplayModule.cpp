#include "ExynosSecondaryDisplayModule.h"
#include "ExynosHWCModule.h"
#include "ExynosHWCUtils.h"
#include "ExynosMPPModule.h"
#include "ExynosHWCDebug.h"

ExynosSecondaryDisplayModule::ExynosSecondaryDisplayModule(struct exynos5_hwc_composer_device_1_t *pdev) :
    ExynosSecondaryDisplay(pdev)
{
}

ExynosSecondaryDisplayModule::~ExynosSecondaryDisplayModule()
{
}

bool ExynosSecondaryDisplayModule::isOverlaySupported(hwc_layer_1_t &layer, size_t index, bool useVPPOverlay, ExynosMPPModule** supportedInternalMPP, ExynosMPPModule** supportedExternalMPP)
{
    private_handle_t *handle = private_handle_t::dynamicCast(layer.handle);
    if (hasPlaneAlpha(layer) && hasAlpha(handle->format) && (layer.blending == HWC_BLENDING_PREMULT)) {
        DISPLAY_LOGD(eDebugOverlaySupported, "\tlayer %u: planeAlpha + preMultiplied blending is not supported", index);
        mLayerInfos[index]->mCheckOverlayFlag |= eUnsupportedBlending;
        return false;
    }

    return ExynosDisplay::isOverlaySupported(layer, index, useVPPOverlay, supportedInternalMPP, supportedExternalMPP);
}

void ExynosSecondaryDisplayModule::doPreProcessing(hwc_display_contents_1_t* contents)
{
    mInternalDMAs.clear();
    ExynosDisplay::doPreProcessing(contents);
#if defined(USES_SINGLE_DECON)	
    for (size_t i = 0; i < contents->numHwLayers; i++) {
        hwc_layer_1_t &layer = contents->hwLayers[i];
        layer.displayFrame.left += (mXres/2);
        layer.displayFrame.right += (mXres/2);
    }
#endif	
}
