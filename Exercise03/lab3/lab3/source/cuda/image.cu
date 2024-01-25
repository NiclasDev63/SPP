#include "image.cuh"

#include <cooperative_groups.h>
#include <cuda.h>
#include <cuda_runtime_api.h>
#include <device_launch_parameters.h>

//TODO kompiliert nocht nicht vollständig Error:"calling a host function("Pixel<unsigned char> ::get_red_channel() const") from a global function("grayscale_kernel") is not allowed"
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

BitmapImage get_grayscale_cuda(const BitmapImage& source) {
    const size_t num_pixels = source.get_width() * source.get_height();
    Pixel<std::uint8_t>* output;
    Pixel<std::uint8_t>* device_input;
    Pixel<std::uint8_t>* device_output;

    // Allocate memory on the device
    cudaMalloc((void**)&device_input, num_pixels * sizeof(Pixel<std::uint8_t>));
    cudaMalloc((void**)&device_output, num_pixels * sizeof(Pixel<std::uint8_t>));

    // Copy input data to the device
    cudaMemcpy(device_input, source.get_data(), num_pixels * sizeof(Pixel<std::uint8_t>), cudaMemcpyHostToDevice);

    dim3 blockDim(16, 16);
    dim3 gridDim(divup(source.get_width(), blockDim.x), divup(source.get_height(), blockDim.y));

    // Call the kernel
    grayscale_kernel<<<gridDim, blockDim>>>(device_input, device_output, source.get_width(), source.get_height());

    // Wait for the kernel to finish
    cudaDeviceSynchronize();

    // Copy the result back to the host
    output = new Pixel<std::uint8_t>[num_pixels];
    cudaMemcpy(output, device_output, num_pixels * sizeof(Pixel<std::uint8_t>), cudaMemcpyDeviceToHost);

    BitmapImage result{source.get_width(), source.get_height()};
    
    for (unsigned int x = 0; x < source.get_width(); x++) {
        for (unsigned int y = 0; y < source.get_height(); y++) {
            const auto pixel = output[y * source.get_width() + x];
            result.set_pixel(x, y, pixel);
        }
    }

    // Free the device memory
    cudaFree(device_input);
    cudaFree(device_output);

    // Free the host memory
    delete[] output;

    return result;
}