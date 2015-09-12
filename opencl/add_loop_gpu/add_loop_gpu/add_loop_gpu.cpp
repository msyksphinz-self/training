	#pragma warning (disable:4996)

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else // __APPLE__
#include <CL/cl.h>
#endif  // __APPLE__

#define MAX_PLATFORM_ID 5
#define MEM_SIZE (10)
#define MAX_SOURCE_SIZE (0x100000)

int main(void)
{
	cl_platform_id platform_id[MAX_PLATFORM_ID] = { NULL };
	cl_device_id   device_id = NULL;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;
	cl_mem memobj_a = NULL,
		memobj_b = NULL,
		memobj_c = NULL;
	cl_program program = NULL;
	cl_kernel kernel = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platform;
	cl_int ret;

	FILE *fp;
	char fileName[] = "./kernel.cl";
	size_t source_size;
	char *source_str;
	cl_int i;

	/* Load including Kernel */
	fp = fopen(fileName, "r");
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	int a[MEM_SIZE], b[MEM_SIZE], c[MEM_SIZE];

	for (int i = 0; i < MEM_SIZE; i++) {
		a[i] = -i;
		b[i] = i * i;
	}

	/* Obtain information of Platform Device */
	ret = clGetPlatformIDs(MAX_PLATFORM_ID, platform_id, &ret_num_platform);

	for (cl_uint pid = 0; pid < ret_num_platform; pid++) {
		ret = clGetDeviceIDs(platform_id[pid], CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
		cl_device_type device_type;
		size_t type_size;
		ret = clGetDeviceInfo(device_id, CL_DEVICE_TYPE, sizeof(device_type), &device_type, &type_size);
		if (type_size == sizeof(device_type)){
			fprintf(stderr, "Success to get device type. Device_Type=%d\n", device_type);
			if ((device_type & CL_DEVICE_TYPE_CPU) != 0) {
				fprintf(stderr, "Device_Type : CPU\n");
			}
			else if ((device_type & CL_DEVICE_TYPE_GPU) != 0) {
				fprintf(stderr, "Device Type : GPU\n");
			}
			else if ((device_type & CL_DEVICE_TYPE_ACCELERATOR) != 0) {
				fprintf(stderr, "Device Type : Accelerator\n");
			}
			else if ((device_type & CL_DEVICE_TYPE_DEFAULT) != 0) {
				fprintf(stderr, "Device Type : Default\n");
			}
		}
		else {
			fprintf(stderr, "Failed to get device type");
		}
	}

	/* Create OpenCL context */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	/* Create Command Queue */
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	/* Craete Memory Buffer */
	memobj_a = clCreateBuffer(context, CL_MEM_READ_WRITE, MEM_SIZE * sizeof(int), NULL, &ret);
	memobj_b = clCreateBuffer(context, CL_MEM_READ_WRITE, MEM_SIZE * sizeof(int), NULL, &ret);
	memobj_c = clCreateBuffer(context, CL_MEM_READ_WRITE, MEM_SIZE * sizeof(int), NULL, &ret);
	/* Translate Memory Buffer */
	ret = clEnqueueWriteBuffer(command_queue, memobj_a, CL_TRUE, 0, MEM_SIZE * sizeof(int), a, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, memobj_b, CL_TRUE, 0, MEM_SIZE * sizeof(int), b, 0, NULL, NULL);

	/* Make kernel program from read source */
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

	/* Build kernel program */
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	/* Build OpenCL kernel */
	kernel = clCreateKernel(program, "vecAdd", &ret);

	/* Setting OpenCL kernel arguments */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj_a);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&memobj_b);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&memobj_c);

	size_t global_work_size[3] = { MEM_SIZE, 0, 0 };
	size_t local_work_size[3] = { MEM_SIZE, 0, 0 };

	/* Execute OpenCL kernel */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);

	clEnqueueReadBuffer(command_queue, memobj_c, CL_TRUE, 0, MEM_SIZE * sizeof(int), c, 0, NULL, NULL);

	/* Show Results */
	for (i = 0; i < MEM_SIZE; i++) {
		printf("mem[%d] : %d %d %d\n", i, a[i], b[i], c[i]);
	}

	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(memobj_a);
	ret = clReleaseMemObject(memobj_b);
	ret = clReleaseMemObject(memobj_c);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	free(source_str);

	return 0;
}