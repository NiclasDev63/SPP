#include "image.cuh"

#include <cooperative_groups.h>
#include <cuda.h>
#include <cuda_runtime_api.h>
#include <device_launch_parameters.h>

//TODO kompiliert nocht nicht vollständig
__global__ void grayscale_kernel(const Pixel<std::uint8_t>* const input, Pixel<std::uint8_t>* const output, const unsigned int width, const unsigned int height) {
    //Calculate the global threadID
    unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x < width && y < height) {
        unsigned int index = y * width + x;
        Pixel<std::uint8_t> pixel = input[index];
        const auto r = pixel.get_red_channel();
        const auto g = pixel.get_green_channel();
        const auto b = pixel.get_blue_channel();

        // Calculate the grayscale value
        const auto gray = r * 0.2989 + g * 0.5870 + b * 0.1140;
        const auto gray_converted = static_cast<std::uint8_t>(gray);

        const auto gray_pixel = Pixel{ gray_converted , gray_converted,  gray_converted };

        output[index] = gray_pixel;
    }
}