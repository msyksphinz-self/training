#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else // __APPLE__
#include <CL/cl.h>
#endif // __APPLE__

#define MEM_SIZE (128)

#define MAX_SOURCE_SIZE (0x100000)

int main()
{
	const char fileName[] = "./kernel_dot.cl";
	FILE *fp;
	if ((fp = fopen(fileName, "r")) == NULL) {
		perror(fileName);
		exit(EXIT_FAILURE);
	}
	char *source_str = (char *)malloc(MAX_SOURCE_SIZE);
	size_t source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	cl_uint ret_num_platform;
	cl_platform_id platform_id = NULL;
	clGetPlatformIDs(1, &platform_id, &ret_num_platform);
	cl_uint ret_num_devices;
	cl_device_id device_id = NULL;
	clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

	cl_int ret;
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	float mem_A[MEM_SIZE], mem_B[MEM_SIZE], mem_C[MEM_SIZE];
	for (int i = 0; i < MEM_SIZE; i++) {
		mem_A[i] = static_cast<float>(i);
		mem_B[i] = static_cast<float>(i * i);
	}

	cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	cl_mem memobj_A = clCreateBuffer(context, CL_MEM_READ_WRITE, MEM_SIZE *sizeof(float), NULL, &ret);
	cl_mem memobj_B = clCreateBuffer(context, CL_MEM_READ_WRITE, MEM_SIZE *sizeof(float), NULL, &ret);
	cl_mem memobj_C = clCreateBuffer(context, CL_MEM_READ_WRITE, MEM_SIZE *sizeof(float), NULL, &ret);

	ret = clEnqueueWriteBuffer(command_queue, memobj_A, CL_TRUE, 0, MEM_SIZE * sizeof(float), mem_A, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, memobj_B, CL_TRUE, 0, MEM_SIZE * sizeof(float), mem_B, 0, NULL, NULL);

	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

	/* Build kernel */
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	cl_kernel kernel = clCreateKernel(program, "vecDot", &ret);
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

	/* Setup kernel argument */

	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj_A);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&memobj_B);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&memobj_C);

	size_t global_work_size[3] = { MEM_SIZE, 0, 0 };
	size_t local_work_size[3] = { MEM_SIZE, 0, 0 };

	/* Execute kernel */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(command_queue, memobj_C, CL_TRUE, 0, MEM_SIZE*sizeof(float), mem_C, 0, NULL, NULL);

	for (int i = 0; i < MEM_SIZE; i++) {
		printf("mem_C[%d] : %f\n", i, mem_C[i]);
	}

	printf("Verification : ");
	float mem_Ans[MEM_SIZE];
	for (int i = 0; i < MEM_SIZE; i++) {
		mem_Ans[i] = mem_A[i] * mem_B[i];
	}
	for (int i = 1; i < MEM_SIZE; i++) {
		mem_Ans[0] += mem_Ans[i];
	}
	printf("Ans = %f\n", mem_Ans[0]);

	clFlush(command_queue);
	clFinish(command_queue);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseMemObject(memobj_A);
	clReleaseMemObject(memobj_B);
	clReleaseMemObject(memobj_C);
	clReleaseCommandQueue(command_queue);
	clReleaseContext(context);

	free(source_str);

	return 0;
}
