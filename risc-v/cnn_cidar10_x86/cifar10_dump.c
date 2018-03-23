#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

extern uint8_t _binary_cifar_10_batches_bin_data_batch_1_bin_start[];
extern uint8_t _binary_cifar_10_batches_bin_data_batch_1_bin_end[];

int main ()
{
  FILE *dump_fp;

  if ((dump_fp = fopen("cifar10.dump", "w")) == NULL) {
	perror ("cifar10.dump");
	exit (EXIT_FAILURE);
  }

  uint8_t *cifar10_data = _binary_cifar_10_batches_bin_data_batch_1_bin_start;
  while (cifar10_data != _binary_cifar_10_batches_bin_data_batch_1_bin_end) {
	uint8_t cifar10_label = cifar10_data[0];
	cifar10_data++;
	fprintf (dump_fp, "LABEL = %d\n", cifar10_label);

	for (int y = 0; y < 32; y++) {
	  for(int x = 0; x < 32; x++) {
		fprintf (dump_fp, "%02x ", cifar10_data[y * 32 + x]);
	  }
	  fprintf (dump_fp, "\n");
	}
	cifar10_data += 32 * 32 * 3;
  }

  fclose(dump_fp);
  
  return 0;
}

