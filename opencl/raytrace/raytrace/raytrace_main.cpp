#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <stdint.h>

#include "./cpu_bitmap.h"

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else  // __APPLE__
#include <CL/cl.h>
#endif // __APPLE__

#include "./raytrace.h"

#define MAX_SOURCE_SIZE (0x100000)

struct DataBlock {
	unsigned char *dev_bitmap;
};

int main()
{
	/* Get Platform Device ID */
	cl_int ret;
	cl_platform_id platform_id;
	cl_device_id device_id;
	cl_uint ret_num_platforms;
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	cl_uint ret_num_devices;
	// ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

	/* Create OpenCL Context */
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	/* Create Command Queue */
	cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	/* Make Kernel Program */
	char fileName[] = "./ray_kernel.cl";
	FILE *fp;
	if ((fp = fopen(fileName, "r")) == NULL) {
		fprintf(stderr, "Failed to load kernel.\n");
		exit(EXIT_FAILURE);
	}
	char *source_str = (char *)malloc(MAX_SOURCE_SIZE);
	size_t source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	/* Make Kernel Program */
	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
		(const size_t *)&source_size, &ret);

	/* Build Kernel Program */
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	/* Make OpenCL kernel */
	cl_kernel kernel = clCreateKernel(program, "calc_sphere", &ret);
	printf("err:%d\n", ret);

	if (ret != CL_SUCCESS) {
		size_t len;
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, NULL, NULL, &len);
		char *log = new char[len]; //or whatever you use
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, len, log, NULL);
		printf("Error was occurred.%zd\n", len);
		printf("%s\n", log);

		switch (ret) {
		case CL_INVALID_PROGRAM:
			fprintf(stderr, "CL_INVALID_PROGRAM\n");
			break;
		case CL_INVALID_PROGRAM_EXECUTABLE:
			fprintf(stderr, "CL_INVALID_PROGRAM_EXECUTABLE\n");
			break;
		case CL_INVALID_KERNEL_NAME:
			fprintf(stderr, "CL_INVALID_KERNEL_NAME\n");
			break;
		case CL_INVALID_KERNEL_DEFINITION:
			fprintf(stderr, "CL_INVALID_KERNEL_DEFINITION\n");
			break;
		case CL_INVALID_VALUE :
			fprintf(stderr, "CL_INVALID_VALUE\n");
			break;
		case CL_OUT_OF_HOST_MEMORY :
			fprintf(stderr, "CL_OUT_OF_HOST_MEMORY\n");
			break;
		}

		exit(EXIT_FAILURE);
	}

	/* Make Ray-trace Sphere information */
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_real_distribution<float> score(0.0, 1.0);
	std::uniform_int_distribution<int> range(0, IMAGE_WIDTH);
	Sphere *temp_s = (Sphere *)malloc(sizeof(Sphere) * SPHERES);
	std::uniform_real_distribution<float> radius(10.0f, IMAGE_WIDTH / 10);
	for (int i = 0; i < SPHERES; i++) {
		temp_s[i].r = score(mt);
		temp_s[i].g = score(mt);
		temp_s[i].b = score(mt);

		temp_s[i].x = range(mt) - IMAGE_WIDTH/2;
		temp_s[i].y = range(mt) - IMAGE_WIDTH/2;
		temp_s[i].z = range(mt) - IMAGE_WIDTH/2;
		temp_s[i].radius = radius(mt);

//		printf("temp_s[i].r = %f, temp_s[i].g = %f, temp_s[i].b = %f, radius=%f\n",
//			temp_s[i].r, temp_s[i].g, temp_s[i].b, temp_s[i].radius);
	}
	
	cl_mem dev_temp_s = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(Sphere) * SPHERES, NULL, &ret);
	clEnqueueWriteBuffer(command_queue, dev_temp_s, CL_TRUE, 0, SPHERES * sizeof(Sphere), temp_s, 0, NULL, NULL);

	uint8_t *mem = new uint8_t[IMAGE_WIDTH * IMAGE_WIDTH * 4];
	for (int i = 0; i < IMAGE_WIDTH * IMAGE_WIDTH; i++) {
		mem[i] = 0;
	}
	cl_mem dev_mem = clCreateBuffer(context, CL_MEM_READ_WRITE, IMAGE_WIDTH*IMAGE_WIDTH * 4, NULL, &ret);

	/* Set OpenCL Arguments */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&dev_temp_s);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&dev_mem);
	/* Execute OpenCL kernel */
	size_t global_work_size[3] = { IMAGE_WIDTH * IMAGE_WIDTH, 0 };
	size_t local_work_size[3] = { IMAGE_WIDTH, 0 };
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);

	CPUBitmap bitmap(IMAGE_WIDTH, IMAGE_WIDTH);
	ret = clEnqueueReadBuffer(command_queue, dev_mem, CL_TRUE, 0, IMAGE_WIDTH * IMAGE_WIDTH * 4, bitmap.get_ptr(), 0, NULL, NULL);

	fclose(fp);

	/* Finish operation */
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	bitmap.display_and_exit();

	return 0;
}