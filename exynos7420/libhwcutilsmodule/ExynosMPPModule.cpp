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

int ExynosMPPModule::getMaxDownscale()
{
    if (
#ifdef MPP_VPP_G
            (mType == MPP_VPP_G) ||
#endif
            (mType == MPP_VG)
        )
        return 1;
    else
        return ExynosMPP::getMaxDownscale();
}

int ExynosMPPModule::getMaxDownscale(hwc_layer_1_t &layer)
{
    if (
#ifdef MPP_VPP_G
           (mType == MPP_VPP_G) ||
#endif
            (mType == MPP_VG)
        )
        return 1;
    else
        return ExynosMPP::getMaxDownscale(layer);
}
