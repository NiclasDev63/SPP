#include "image.cuh"

#include <cooperative_groups.h>
#include <cuda.h>
#include <cuda_runtime_api.h>
#include <device_launch_parameters.h>

__global__ void grayscale_kernel(const Pixel<std::uint8_t>* const input, Pixel<std::uint8_t>* const output, const unsigned int width, const unsigned int height) {
    //Calculate the global threadID
    unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x < width && y < height) {
        unsigned int index = y * width + x;
        Pixel<std::uint8_t> pixel = input[index];
        Pixel<std::uint8_t>* pixelPtr = &pixel;
        auto* r = reinterpret_cast<std::uint8_t*>(pixelPtr);
        auto* g = reinterpret_cast<std::uint8_t*>(pixelPtr + 1);
        auto* b = reinterpret_cast<std::uint8_t*>(pixelPtr + 2);

        // Calculate the grayscale value
        const auto gray = *r * 0.2989 + *g * 0.5870 + *b * 0.1140;
        auto gray_converted = static_cast<std::uint8_t>(gray);

        output[index] = reinterpret_cast<Pixel<std::uint8_t>&>(gray_converted, gray_converted, gray_converted);
    }
}

BitmapImage get_grayscale_cuda(const BitmapImage& source) {
    const size_t num_pixels = source.get_width() * source.get_height();
    const auto raw_size = num_pixels * sizeof(Pixel<std::uint8_t>);
    Pixel<std::uint8_t>* output;
    Pixel<std::uint8_t>* device_input;
    Pixel<std::uint8_t>* device_output;

    // Allocate memory on the device
    cudaMalloc((void**)&device_input, raw_size);
    cudaMalloc((void**)&device_output, raw_size);

    // Copy input data to the device
    cudaMemcpy(device_input, source.get_data(), raw_size, cudaMemcpyHostToDevice);

    dim3 thread_dim(32, 32);
    dim3 block_dim(divup(source.get_width(), thread_dim.x), divup(source.get_height(), thread_dim.y));

    // Call the kernel
    grayscale_kernel<<<block_dim, thread_dim>>>(device_input, device_output, source.get_width(), source.get_height());

    // Copy the result back to the host
    output = new Pixel<std::uint8_t>[num_pixels];
    cudaMemcpy(output, device_output, raw_size, cudaMemcpyDeviceToHost);


    // Free the device memory
    cudaFree(device_input);
    cudaFree(device_output);

    // Wait for the kernel to finish
    cudaDeviceSynchronize();

    BitmapImage result{source.get_width(), source.get_height()};
    
    for (unsigned int x = 0; x < source.get_width(); x++) {
        for (unsigned int y = 0; y < source.get_height(); y++) {
            const auto pixel = output[y * source.get_width() + x];
            result.set_pixel(x, y, pixel);
        }
    }


    // Free the host memory
    delete[] output;

    return result;
}