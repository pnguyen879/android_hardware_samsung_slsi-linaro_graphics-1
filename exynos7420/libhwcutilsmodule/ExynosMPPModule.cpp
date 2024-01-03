#include "ExynosMPPModule.h"
#include "ExynosHWCUtils.h"

ExynosMPPModule::ExynosMPPModule()
    : ExynosMPP()
{
}

ExynosMPPModule::ExynosMPPModule(ExynosDisplay *display, unsigned int mppType, unsigned int mppIndex)
    : ExynosMPP(display, mppType, mppIndex)
{
}

int ExynosMPPModule::isProcessingSupported(hwc_layer_1_t &layer, int dst_format)
{
    private_handle_t *handle = private_handle_t::dynamicCast(layer.handle);
    
    if ((handle->size & 6) && (handle->format == HAL_PIXEL_FORMAT_EXYNOS_YCrCb_420_SP_M_FULL) && ((mType == MPP_VG) || (mType == MPP_VGR))) {
    	return -eMPPUnsupportedFormat;
    }
    return ExynosMPP::isProcessingSupported(layer, dst_format);
}

int ExynosMPPModule::getMaxDownscale(hwc_layer_1_t &layer)
{        
    if (mType == MPP_MSC)
        return 16;
    return ExynosMPP::getMaxDownscale(layer);
}
