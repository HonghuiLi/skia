/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrMtlGpu_DEFINED
#define GrMtlGpu_DEFINED

#include "GrGpu.h"
#include "GrRenderTarget.h"
#include "GrSemaphore.h"
#include "GrTexture.h"

#import <Metal/Metal.h>

class GrSemaphore;

class GrMtlGpu : public GrGpu {
public:
    static GrGpu* Create(GrBackendContext backendContext, const GrContextOptions& options,
                         GrContext* context);

    ~GrMtlGpu() override {}

    bool onGetReadPixelsInfo(GrSurface* srcSurface, int readWidth, int readHeight, size_t rowBytes,
                             GrPixelConfig readConfig, DrawPreference*,
                             ReadPixelTempDrawInfo*) override { return false; }

    bool onGetWritePixelsInfo(GrSurface* dstSurface, int width, int height,
                              GrPixelConfig srcConfig, DrawPreference*,
                              WritePixelTempDrawInfo*) override { return false; }

    bool onCopySurface(GrSurface* dst,
                       GrSurface* src,
                       const SkIRect& srcRect,
                       const SkIPoint& dstPoint) override { return false; }

    void onQueryMultisampleSpecs(GrRenderTarget* rt, const GrStencilSettings&,
                                 int* effectiveSampleCnt, SamplePattern*) override {}

    GrGpuCommandBuffer* createCommandBuffer(const GrGpuCommandBuffer::LoadAndStoreInfo&,
                                            const GrGpuCommandBuffer::LoadAndStoreInfo&) override {
        return nullptr;
    }

    GrFence SK_WARN_UNUSED_RESULT insertFence() override { return 0; }
    bool waitFence(GrFence, uint64_t) override { return true; }
    void deleteFence(GrFence) const override {}

    sk_sp<GrSemaphore> SK_WARN_UNUSED_RESULT makeSemaphore(bool isOwned) override {
        return nullptr;
    }
    sk_sp<GrSemaphore> wrapBackendSemaphore(const GrBackendSemaphore& semaphore,
                                            GrWrapOwnership ownership) override { return nullptr; }
    void insertSemaphore(sk_sp<GrSemaphore> semaphore, bool flush) override {}
    void waitSemaphore(sk_sp<GrSemaphore> semaphore) override {}
    sk_sp<GrSemaphore> prepareTextureForCrossContextUsage(GrTexture*) override { return nullptr; }

private:
    GrMtlGpu(GrContext* context, const GrContextOptions& options) : INHERITED(context) {
        id<MTLDevice> device = MTLCreateSystemDefaultDevice();

        MTLTextureDescriptor* txDesc = [[MTLTextureDescriptor alloc] init];
        txDesc.textureType = MTLTextureType3D;
        txDesc.height = 64;
        txDesc.width = 64;
        txDesc.depth = 64;
        txDesc.pixelFormat = MTLPixelFormatBGRA8Unorm;
        txDesc.arrayLength = 1;
        txDesc.mipmapLevelCount = 1;
        fTestHeaderTexture = [device newTextureWithDescriptor:txDesc];
    }

    void onResetContext(uint32_t resetBits) override {}

    void xferBarrier(GrRenderTarget*, GrXferBarrierType) override {}

    sk_sp<GrTexture> onCreateTexture(const GrSurfaceDesc& desc, SkBudgeted budgeted,
                                     const SkTArray<GrMipLevel>& texels) override {
        return nullptr;
    }

    sk_sp<GrTexture> onWrapBackendTexture(const GrBackendTexture&,
                                          GrSurfaceOrigin,
                                          GrBackendTextureFlags,
                                          int sampleCnt,
                                          GrWrapOwnership) override {
        return nullptr;
    }

    sk_sp<GrRenderTarget> onWrapBackendRenderTarget(const GrBackendRenderTarget&,
                                                    GrSurfaceOrigin) override {
        return nullptr;
    }

    sk_sp<GrRenderTarget> onWrapBackendTextureAsRenderTarget(const GrBackendTexture&,
                                                             GrSurfaceOrigin,
                                                             int sampleCnt) override {
        return nullptr;
    }

    GrBuffer* onCreateBuffer(size_t, GrBufferType, GrAccessPattern, const void*) override {
        return nullptr;
    }

    gr_instanced::InstancedRendering* onCreateInstancedRendering() override { return nullptr; }

    bool onReadPixels(GrSurface* surface,
                      int left, int top, int width, int height,
                      GrPixelConfig,
                      void* buffer,
                      size_t rowBytes) override {
        return false;
    }

    bool onWritePixels(GrSurface* surface,
                       int left, int top, int width, int height,
                       GrPixelConfig config, const SkTArray<GrMipLevel>& texels) override {
        return false;
    }

    bool onTransferPixels(GrTexture* texture,
                          int left, int top, int width, int height,
                          GrPixelConfig config, GrBuffer* transferBuffer,
                          size_t offset, size_t rowBytes) override {
        return false;
    }

    void onResolveRenderTarget(GrRenderTarget* target) override { return; }

    GrStencilAttachment* createStencilAttachmentForRenderTarget(const GrRenderTarget*,
                                                                int width,
                                                                int height) override {
        return nullptr;
    }

    void clearStencil(GrRenderTarget* target) override  {}

    GrBackendObject createTestingOnlyBackendTexture(void* pixels, int w, int h,
                                                    GrPixelConfig config, bool isRT) override {
        return 0;
    }
    bool isTestingOnlyBackendTexture(GrBackendObject ) const override { return false; }
    void deleteTestingOnlyBackendTexture(GrBackendObject, bool abandonTexture) override {}

    id<MTLTexture> fTestHeaderTexture;

    typedef GrGpu INHERITED;
};

#endif

