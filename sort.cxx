#include <rocprim/rocprim.hpp>
#include <cstdio>

int main()
{

// Prepare input and output (declare pointers, allocate device memory etc.)
size_t input_size=12800;          // e.g., 8
unsigned int * keys_input;  // e.g., [ 6, 3,  5, 4,  1,  8,  2, 7]
double * values_input;      // e.g., [-5, 2, -4, 3, -1, -8, -2, 7]
unsigned int * keys_output; // empty array of 8 elements
double * values_output;     // empty array of 8 elements

size_t tmp_nbytes;
void * tmp_ptr = nullptr;

values_input  = new double[input_size];
keys_input    = new unsigned int[input_size];
values_output = new double[input_size];
keys_output   = new unsigned int[input_size];
for(int i=0;i<input_size;i++)
{
    keys_input[i]=i;
    values_input[i]=(rand()/((double)RAND_MAX))*input_size;
}

// Get required size of the temporary storage
rocprim::merge_sort(
    tmp_ptr,
    tmp_nbytes,
    values_input, 
    values_output,
    keys_input,
    keys_output,
    input_size
);
printf("tmp_nbytes=%zu\n",tmp_nbytes);


// allocate temporary storage
hipMalloc(&tmp_ptr, tmp_nbytes);

// perform sort
rocprim::merge_sort(
    tmp_ptr,
    tmp_nbytes,
    values_input,
    values_output,
    keys_input, 
    keys_output,
    input_size
);
hipDeviceSynchronize();

int report_size=8;//input_size;
for(int i=0;i<report_size;i++) printf("%u ",keys_input[i]);
printf("\n");
for(int i=0;i<report_size;i++) printf("%u ",keys_output[i]);
printf("\n");
for(int i=0;i<report_size;i++) printf("%.2f ",values_input[i]);
printf("\n");
for(int i=0;i<report_size;i++) printf("%.2f ",values_output[i]);
printf("\n");

// keys_output:   [ 1,  2, 3, 4,  5,  6, 7,  8]
// values_output: [-1, -2, 2, 3, -4, -5, 7, -8]
}
