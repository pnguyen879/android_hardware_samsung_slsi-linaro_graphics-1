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

int ExynosMPPModule::getMaxDownscale(hwc_layer_1_t &layer)
{        
    if (mType == MPP_MSC)
        return 16;
    return ExynosMPP::getMaxDownscale(layer);
}

int ExynosMPPModule::isProcessingSupported(hwc_layer_1_t &layer, int dst_format)
{
    private_handle_t *handle = private_handle_t::dynamicCast(layer.handle);
    
    if ((handle->size & 6) && (handle->format == HAL_PIXEL_FORMAT_EXYNOS_YCrCb_420_SP_M_FULL) && ((mType == MPP_VG) || (mType == MPP_VGR))) {
    	return -eMPPUnsupportedFormat;
    }
    
    if (isCompressed(layer)) {
        if (mType != MPP_VGR)
            return -eMPPUnsupportedCompression;
        if (layer.transform)
            return -eMPPUnsupportedRotation;
    }

    if ((mType == MPP_VG) || (mType == MPP_VGR))
        dst_format = INTERNAL_MPP_DST_FORMAT;

    int maxWidth = getMaxWidth(layer);
    int maxHeight = getMaxHeight(layer);
    int minWidth = getMinWidth(layer);
    int minHeight = getMinHeight(layer);
    int srcWidthAlign = getSrcWidthAlign(layer);
    int srcHeightAlign = getSrcHeightAlign(layer);

    int maxCropWidth = getMaxCropWidth(layer);
    int maxCropHeight = getMaxCropHeight(layer);
    int minCropWidth = getMinCropWidth(layer);
    int minCropHeight = getMinCropHeight(layer);
    int cropWidthAlign = getCropWidthAlign(layer);
    int cropHeightAlign = getCropHeightAlign(layer);
    int srcXOffsetAlign = getSrcXOffsetAlign(layer);
    int srcYOffsetAlign = getSrcYOffsetAlign(layer);

    int maxDstWidth = getMaxDstWidth(dst_format);
    int maxDstHeight = getMaxDstHeight(dst_format);
    int minDstWidth = getMinDstWidth(dst_format);
    int minDstHeight = getMinDstHeight(dst_format);
    int dstWidthAlign = getDstWidthAlign(dst_format);
    int dstHeightAlign = getDstHeightAlign(dst_format);

    int maxDownscale = getMaxDownscale(layer);
    if (((mType == MPP_VG) || (mType == MPP_VGR)) &&
        isRotated(layer) && isFormatRgb(handle->format))
        maxDownscale = 1;

    int maxUpscale = getMaxUpscale();

    bool isPerpendicular = !!(layer.transform & HAL_TRANSFORM_ROT_90);

    int srcW = WIDTH(layer.sourceCropf), srcH = HEIGHT(layer.sourceCropf);
    if (isFloat(layer.sourceCropf.right - layer.sourceCropf.left))
        srcW = ceilf(layer.sourceCropf.right - layer.sourceCropf.left);
    if (isFloat(layer.sourceCropf.bottom - layer.sourceCropf.top))
        srcH = ceilf(layer.sourceCropf.bottom - layer.sourceCropf.top);

    int xOffset = layer.sourceCropf.left;
    int yOffset = layer.sourceCropf.top;
    int dstW, dstH;
    if (isPerpendicular) {
        dstW = HEIGHT(layer.displayFrame);
        dstH = WIDTH(layer.displayFrame);
    } else {
        dstW = WIDTH(layer.displayFrame);
        dstH = HEIGHT(layer.displayFrame);
    }

    if (isFormatYUV420(handle->format)) {
        if ((mType == MPP_VG) || (mType == MPP_VGR)) {
            if (xOffset % srcXOffsetAlign != 0) {
                xOffset = ALIGN(xOffset, srcXOffsetAlign);
                srcW = (int)layer.sourceCropf.right - xOffset;
            }
            if (yOffset % srcYOffsetAlign != 0) {
                yOffset = ALIGN(yOffset, srcYOffsetAlign);
                srcH = (int)layer.sourceCropf.bottom - yOffset;
            }
        }
        if (srcW % cropWidthAlign != 0)
            srcW = ALIGN_DOWN(srcW, cropWidthAlign);
        if (srcH % cropHeightAlign != 0)
            srcH = ALIGN_DOWN(srcH, cropHeightAlign);
    }

    if (mType == MPP_MSC) {
        bool needDoubleOperation = false;
        if (getDrmMode(handle->flags) != NO_DRM) {
            needDoubleOperation = (dstW > srcW * maxUpscale) || (dstH > srcH * maxUpscale);
            if (needDoubleOperation)
                maxUpscale = maxUpscale * maxUpscale;

            needDoubleOperation = (srcW > dstW * maxDownscale) || (srcH > dstH * maxDownscale);
            if (needDoubleOperation)
                maxDownscale = maxDownscale * maxDownscale;
        }
    }

    if (!isFormatSupportedByMPP(handle->format))
        return -eMPPUnsupportedFormat;
    else if (!isFormatSupportedByMPP(dst_format))
        return -eMPPUnsupportedFormat;
#if 0
    else if (!isCSCSupportedByMPP(handle->format, dst_format, layer.dataSpace))
        return -eMPPUnsupportedCSC;
#endif
    else if (!mCanBlend &&
              (handle->format == HAL_PIXEL_FORMAT_RGBA_8888 || handle->format == HAL_PIXEL_FORMAT_BGRA_8888) &&
              layer.blending != HWC_BLENDING_NONE)
        return -eMPPUnsupportedBlending;
    else if (!mCanRotate && layer.transform)
        return -eMPPUnsupportedRotation;
    else if (srcW < minWidth)
        return -eMPPExeedMinSrcWidth;
    else if (srcH < minHeight)
        return -eMPPExeedMinSrcHeight;
    else if (srcW < minCropWidth)
        return -eMPPExeedSrcWCropMin;
    else if (srcH < minCropHeight)
        return -eMPPExeedSrcHCropMin;
    else if (dstW > maxDstWidth)
        return -eMPPExeedMaxDstWidth;
    else if (dstW > maxDstHeight)
        return -eMPPExeedMaxDstHeight;
    else if (dstW < minDstWidth)
        return -eMPPExeedMinDstWidth;
    else if (dstW < minDstHeight)
        return -eMPPExeedMinDstWidth;
    else if ((dstW % dstWidthAlign != 0) || (dstH % dstHeightAlign != 0))
        return -eMPPNotAlignedDstSize;
    else if (srcW > dstW * maxDownscale)
        return -eMPPExeedMaxDownScale;
    else if (dstW > srcW * maxUpscale)
        return -eMPPExeedMaxUpScale;
    else if (srcH > dstH * maxDownscale)
        return -eMPPExeedMaxDownScale;
    else if (dstH > srcH * maxUpscale)
        return -eMPPExeedMaxUpScale;

    if (getDrmMode(handle->flags) == NO_DRM) {
        if (handle->stride > maxWidth)
            return -eMPPExceedHStrideMaximum;
        else if (handle->vstride > maxHeight)
            return -eMPPExceedVStrideMaximum;
        else if (handle->stride % srcWidthAlign != 0)
            return -eMPPNotAlignedHStride;
        else if (handle->vstride % srcHeightAlign != 0)
            return -eMPPNotAlignedVStride;
        else if (srcW > maxCropWidth)
            return -eMPPExeedSrcWCropMax;
        else if (srcH > maxCropHeight)
            return -eMPPExeedSrcHCropMax;
        else if ((srcW % cropWidthAlign != 0) || (srcH % cropHeightAlign != 0))
            return -eMPPNotAlignedCrop;
        else if ((xOffset % srcXOffsetAlign != 0) || (yOffset % srcYOffsetAlign != 0))
            return -eMPPNotAlignedOffset;
    }

    return 1;
}
