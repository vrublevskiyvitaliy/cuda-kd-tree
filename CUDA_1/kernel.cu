#include "device_functions.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <iostream>
#include <stdio.h>
#include <chrono>

#include "KDTree.hpp"

const int numberOfPoints = 4;
const int numbersPerThread = 2;

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;

__device__ void swap(float &a, float &b)
{
	float c = a;
	a = b;
	b = c;
}

__device__ void swap(int &a, int &b)
{
	int c = a;
	a = b;
	b = c;
}

__device__ void printArray(int * a, int size)
{
	for (int i = 0; i < size; i++) {
		printf("%d ", a[i]);
	}
	printf("\n ");
}


__device__ void prefixScan(int * in_data, int * out_data, int n)
{

	// extern __shared__ float temp[];
	
	int thid = threadIdx.x;
	int offset = 1;

	for (int j = 0; j < numbersPerThread; j++) {
		out_data[j + thid * numbersPerThread] = in_data[j + thid * numbersPerThread];
	}



	for (int d = n >> 1; d > 0; d >>= 1) // build sum in place up the tree
	{
		__syncthreads();
		if (thid < d)
		{
			int ai = offset*(2 * thid + 1) - 1;
			int bi = offset*(2 * thid + 2) - 1;
			out_data[bi] += out_data[ai];
		}
		offset *= 2;
	}
	if (thid == 0) { out_data[n - 1] = 0; }

	for (int d = 1; d < n; d *= 2) // traverse down tree & build scan
	{
		offset >>= 1;
		__syncthreads();
		if (thid < d)
		{
			int ai = offset*(2 * thid + 1) - 1;
			int bi = offset*(2 * thid + 2) - 1;
			float t = out_data[ai];
			out_data[ai] = out_data[bi];
			out_data[bi] += t;
		}
	}
	__syncthreads();


	//printf("Prefix scan %d ", threadIdx.x);	

}


__global__ void medianSplit(float *x, float *y, float * x_n, float * y_n, int * d_l, int * d_r)
{
    int i = threadIdx.x;
	int amountOfThreads = blockDim.x;

	int l = *d_l;
	int r = *d_r;
	//printf("R = %d \n" , r);
	//printf("L = %d \n", l);


	__shared__ float x_shared[numberOfPoints];
	__shared__ float y_shared[numberOfPoints];

	__shared__ float x_shared_tmp[numberOfPoints];
	__shared__ float y_shared_tmp[numberOfPoints];


	//__shared__ int position[numberOfPoints];

	__shared__ int pointsLessThanPivot;

	__shared__ int prefixBlock[numberOfPoints];
	__shared__ int prefixSumBlock[numberOfPoints];

	__shared__ int prefixBlockReverse[numberOfPoints];
	__shared__ int prefixSumBlockReverse[numberOfPoints];

	int prefixPerThread[numbersPerThread];

	for (int j = 0; j < numbersPerThread; j++) {
		x_shared[j + i * numbersPerThread] = x[j + i * numbersPerThread];
		y_shared[j + i * numbersPerThread] = y[j + i * numbersPerThread];
	//	position[j + i * numbersPerThread] = j + i * numbersPerThread;
	}
	
	int k_th = (l + r) >> 1;

	int pivot = (l + r) >> 1;

	//printf("pivot = %d \n", pivot);

	__syncthreads();
	while (k_th > 0) {

		if (!i) {
			//printf("pivot = [%.f, %.f]\n", x_shared[pivot], y_shared[pivot]);

			swap(x_shared[pivot], x_shared[r]);
			swap(y_shared[pivot], y_shared[r]);

			//swap(position[pivot], position[r]);

			//printf("pivot = [%.f, %.f]\n", x_shared[r], y_shared[r]);
		}
		__syncthreads();
		

		for (int j = 0; j < numbersPerThread; j++) {
			int i_index = j + i * numbersPerThread;
			//prefixPerThread[j] = x[i_index] < x[r] ? 1 : 0;
			prefixBlock[i_index] = x_shared[i_index] < x_shared[r] ? 1 : 0;
			prefixBlockReverse[i_index] = (prefixBlock[i_index] + 1) % 2;

			//printf("COMPARE x[i_index] < x[r] ? 1 : 0  = %f < %f = %d \n", x_shared[i_index], x_shared[r], prefixBlock[i_index]);
		}

		__syncthreads();
		
		if (!i) {
			printArray(prefixBlock, numberOfPoints);
		}

		prefixScan(prefixBlock, prefixSumBlock, numberOfPoints);
		prefixScan(prefixBlockReverse, prefixSumBlockReverse, numberOfPoints);


		if (!i) {
			pointsLessThanPivot = prefixSumBlock[numberOfPoints - 1];

			printArray(prefixSumBlock, numberOfPoints);
			printArray(prefixSumBlockReverse, numberOfPoints);
		}
		__syncthreads();

		for (int j = 0; j < numbersPerThread; j++) {
			int i_index = j + i * numbersPerThread;

			int new_index = prefixBlock[i_index] ? prefixSumBlock[i_index] : prefixSumBlockReverse[i_index] + pointsLessThanPivot;

			x_shared_tmp[new_index] = x_shared[i_index];
			y_shared_tmp[new_index] = y_shared[i_index];
		}

		__syncthreads();

		for (int j = 0; j < numbersPerThread; j++) {
			int i_index = j + i * numbersPerThread;

			x_shared[i_index] = x_shared_tmp[i_index];
			y_shared[i_index] = y_shared_tmp[i_index];
		}

		if (!i) {
			swap(x_shared[r], x_shared[pointsLessThanPivot]);
			swap(y_shared[r], y_shared[pointsLessThanPivot]);
		}

		k_th = 0;
	}

	__syncthreads();

	for (int j = 0; j < numbersPerThread; j++) {
		//x_n[position[j + i * numbersPerThread]] = x_shared[j + i * numbersPerThread];
		//y_n[position[j + i * numbersPerThread]] = y_shared[j + i * numbersPerThread];
		x_n[j + i * numbersPerThread] = x_shared[j + i * numbersPerThread];
		y_n[j + i * numbersPerThread] = y_shared[j + i * numbersPerThread];
	}

}

void printArray(float * x, float * y)
{
	for (int i = 0; i < numberOfPoints; i++) {
		printf("Point %d : [%.0f, %.0f]\n", i, x[i], y[i]);
	}
}

int main()
{
	
	KDTree tree(numberOfPoints, KDTree::BUILD_RECURSIVE_FAST, NULL, false);

	std::vector<glm::vec2> points = tree.p->getPointsPositions();

	float * h_x = new float[numberOfPoints];
	float * h_y = new float[numberOfPoints];
	float * h_x_n = new float[numberOfPoints];
	float * h_y_n = new float[numberOfPoints];


	for (int i = 0; i < numberOfPoints; i++) {
		h_x[i] = points[i].x;
		h_y[i] = points[i].y;
	}

	tree.setData();

	tree.printTree();


	// CUDA CODE

	int size = numberOfPoints;

	float *d_x = 0;
	float *d_y = 0;
	float *d_x_n = 0;
	float *d_y_n = 0;

	int * d_l = 0;
	int * d_r = 0;

	int * h_l = new int;
	int * h_r = new int;
	*h_r = numberOfPoints - 1;
	*h_l = 0;



	// Choose which GPU to run on, change this on a multi-GPU system.
	cudaSetDevice(0);

	// Allocate GPU buffers for three vectors (two input, one output)    .
	cudaMalloc((void**)&d_x, size * sizeof(float));
	cudaMalloc((void**)&d_y, size * sizeof(float));
	cudaMalloc((void**)&d_x_n, size * sizeof(float));
	cudaMalloc((void**)&d_y_n, size * sizeof(float));
	cudaMalloc((void**)&d_l, sizeof(int));
	cudaMalloc((void**)&d_r, sizeof(int));


	auto t0 = Time::now();
	
	// Copy input vectors from host memory to GPU buffers.
	cudaMemcpy(d_x, h_x, size * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_y, h_y, size * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(d_l, h_l, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_r, h_r, sizeof(int), cudaMemcpyHostToDevice);

	// Launch a kernel on the GPU with one thread for each element.
	medianSplit << <1, size/2 >> >(d_x, d_y, d_x_n, d_y_n, d_l, d_r);

	// Check for any errors launching the kernel
	cudaGetLastError();

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	cudaDeviceSynchronize();

	// Copy output vector from GPU buffer to host memory.
	cudaMemcpy(h_x_n, d_x_n, size * sizeof(float), cudaMemcpyDeviceToHost);
	cudaMemcpy(h_y_n, d_y_n, size * sizeof(float), cudaMemcpyDeviceToHost);

	auto t1 = Time::now();

	fsec fs = t1 - t0;
	ms d = std::chrono::duration_cast<ms>(fs);

	std::cout << "Time " << d.count() << "ms\n";

	cudaFree(d_x);
	cudaFree(d_y);
	cudaFree(d_x_n);
	cudaFree(d_y_n);

	printArray(h_x, h_y);
	std::cout << "==============\n";
	printArray(h_x_n, h_y_n);
	/*

    // Add vectors in parallel.
    cudaError_t cudaStatus = addWithCuda(c, a, b, arraySize);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addWithCuda failed!");
        return 1;
    }

    printf("{1,2,3,4,5} + {10,20,30,40,50} = {%d,%d,%d,%d,%d}\n",
        c[0], c[1], c[2], c[3], c[4]);

    // cudaDeviceReset must be called before exiting in order for profiling and
    // tracing tools such as Nsight and Visual Profiler to show complete traces.
    cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceReset failed!");
        return 1;
    }

	*/


	delete []h_x;
	delete []h_y;
	delete []h_x_n;
	delete []h_y_n;


	int tmp;
	std::cin >> tmp;
    
	return 0;
}
