#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cpu_bitmap.h"
#include "julia_param.h"

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else // __APPLE__
#include <CL/cl.h>
#endif // __APPLE__


#define MEM_SIZE (DIM*DIM*4)
#define MAX_SOURCE_SIZE (0x100000)



struct DataBlock{
	uint8_t *dev_bitmap;
};

int main()
{
	cl_platform_id   platform_id   = NULL;
	cl_device_id     device_id     = NULL;
	cl_context       context       = NULL;
	cl_command_queue command_queue = NULL;

	cl_program program = NULL;
	cl_kernel  kernel = NULL;
	cl_uint    ret_num_devices;
	cl_uint    ret_num_platforms;
	cl_int     ret;

	const char fileName[] = "./julia_kernel.cpp";
	size_t source_size;
	char *source_str;

	FILE *fp;
	if ((fp = fopen(fileName, "r")) == NULL) {
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);

	clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	fprintf(stderr, "platform-num = %d\n", ret_num_platforms);
	clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1,  &device_id, &ret_num_devices);
	fprintf(stderr, "device_id = %08x\n", device_id);

	/* Create Context */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	/* Create Command Queue */
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	/* Create memory buffer */
	cl_mem memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, MEM_SIZE, NULL, &ret);

	/* Generate Program */
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	kernel = clCreateKernel(program, "julia_kernel", &ret);
	printf("err:%d\n", ret);

	if (ret != CL_SUCCESS) {
		size_t len;
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, NULL, NULL, &len);
		char *log = new char[len]; //or whatever you use
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, len, log, NULL);
		printf("Error was occurred.%d\n", len);
		printf("%s\n", log);
		exit(EXIT_FAILURE);
	}

	ret = clSetKernelArg(kernel, 0, sizeof(memobj), (void *)&memobj);

	size_t global_work_size[3] = { DIM * DIM, 0 };
	size_t local_work_size[3] = { DIM, 0 };

	printf("Kernel Start.\n");
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);
	printf("Kernel End.\n");

	CPUBitmap bitmap(DIM, DIM);
	ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0, DIM * DIM * 4, bitmap.get_ptr(), 0, NULL, NULL);
	uint8_t *mem = bitmap.get_ptr();

	// for (int i = 0; i < 100; i++){
	// 	printf("%02d", mem[i]);
	// 	if (i % 16 == 15) {
	// 		printf("\n");
	// 	}
	// }

	bitmap.display_and_exit();

	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(memobj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	return 0;
}
