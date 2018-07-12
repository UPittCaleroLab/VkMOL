/*
  Copyright 2018, Dylan Lukes

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef VKMOL_RENDERER_RENDERER_H
#define VKMOL_RENDERER_RENDERER_H

#include "Buffer.h"
#include "Resource.h"

#include <functional>
#include <unordered_set>

#include <vulkan/vulkan.hpp>

namespace vkmol {
namespace renderer {

struct RendererWSIDelegate {
    using InstanceExtensionsCallback =
        std::function<std::vector<const char *>()>;

    std::function<std::vector<const char *>()>  getInstanceExtensions;
    std::function<vk::SurfaceKHR(vk::Instance)> getSurface;
    std::function<std::tuple<int, int>()>       getWindowSize;
    std::function<std::tuple<int, int>()>       getFramebufferSize;
};

struct RendererInfo {
    bool debug;
    bool trace;

    std::string               appName;
    std::tuple<int, int, int> appVersion;

    RendererWSIDelegate delegate;

    RendererInfo()
    : debug(false), trace(false), appName("untitled"), appVersion(1, 0, 0) {}
};

typedef ResourceHandle<Buffer> BufferHandle;

class Renderer {
private:
    // todo: std::vector<Frame> frames;

    RendererWSIDelegate delegate;

    ResourceContainer<Buffer> buffers;
    // todo: ... other resource containers

    vk::Instance                       instance;
    vk::DebugReportCallbackEXT         debugCallback;
    vk::PhysicalDevice                 physicalDevice;
    vk::PhysicalDeviceProperties       deviceProperties;
    vk::PhysicalDeviceFeatures         deviceFeatures;
    vk::Device                         device;
    vk::SurfaceKHR                     surface;
    vk::PhysicalDeviceMemoryProperties memoryProperties;

    // These resources are cleaned out every frame.
    std::unordered_set<Resource> graveyard;

    bool isSwapchainDirty;

    unsigned long uboAlignment;
    unsigned long ssboAlignment;

    struct ResourceDeleter final {

        Renderer *renderer;

        ResourceDeleter(Renderer *renderer) : renderer(renderer) {}

        ResourceDeleter(const ResourceDeleter &) = default;

        ResourceDeleter(ResourceDeleter &&) = default;

        ResourceDeleter &operator=(const ResourceDeleter &) = default;

        ResourceDeleter &operator=(ResourceDeleter &&) = default;

        ~ResourceDeleter() = default;

        void operator()(Buffer &b) const { renderer->deleteBufferInternal(b); }
    };

    void deleteBufferInternal(Buffer &b);

public:
#pragma mark - Lifecycle

    explicit Renderer(const RendererInfo &rendererInfo);

    Renderer(const Renderer &) = delete;

    Renderer &operator=(const Renderer &) = delete;

    Renderer(Renderer &&other) = default;

    Renderer &operator=(Renderer &&other) = default;

    ~Renderer();

#pragma mark - Resource Management

    BufferHandle
    createBuffer(BufferType type, uint32_t size, const void *contents);

    void deleteBuffer(BufferHandle handle);
};

}; // namespace renderer
}; // namespace vkmol

#endif // VKMOL_RENDERER_RENDERER_H
