#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <CL/cl.h>
#else // __APPLE__
#include <CL/cl.h>
#endif // __APPLE__

#define MAX_PLATFORM (4)
#define MAX_SOURCE_SIZE (0x10000)

int main(void)
{
    cl_int ret;
    cl_platform_id platform_id[MAX_PLATFORM];
    cl_uint ret_num_platforms;
    ret = clGetPlatformIDs(MAX_PLATFORM, platform_id, &ret_num_platforms);

    cl_device_id device_id;
    for (cl_uint i_pl = 0; i_pl < ret_num_platforms; i_pl++) {
        const size_t param_size = 100;
        size_t ret_param_size;
        char *name = new char[param_size];
        cl_int ret = clGetPlatformInfo(platform_id[i_pl], CL_PLATFORM_NAME, param_size, name, &ret_param_size);
        fprintf(stderr, "%s\n", name);

        cl_uint ret_num_devices;
        ret = clGetDeviceIDs(platform_id[i_pl], CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
        for (cl_uint i_dv = 0; i_dv < ret_num_devices; i_dv++) {
            const size_t device_name_size = 100;
            size_t ret_device_name_size;
            char *device_name = new char[device_name_size];
            clGetDeviceInfo(device_id, CL_DEVICE_NAME, device_name_size, device_name, &ret_device_name_size);
            fprintf(stderr, " %s\n", device_name);
        }
    }

    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
    
    FILE *fp;
    if ((fp = fopen("./kernel.cl", "r")) == NULL) {
        perror("kernel.cl");
        exit(EXIT_FAILURE);
    }
    char *source_str = (char *)malloc(MAX_SOURCE_SIZE);
    size_t source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);

    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
    ret = clBuildProgram(program, 1, &device_id, "-create-library", NULL, NULL);
    cl_kernel kernel = clCreateKernel(program, "vecAdd", &ret);

    const size_t binary_size = MAX_SOURCE_SIZE;
    char *binary_code = new char [binary_size];
    size_t ret_binary_size;
    clGetProgramInfo(program, CL_PROGRAM_BINARIES, binary_size, binary_code, &ret_binary_size);

    fprintf(stderr, "%d %s", ret_binary_size, binary_code);

    clFlush(command_queue);
    clFinish(command_queue);
    clReleaseKernel(kernel);


}

