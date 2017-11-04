#include <assert.h>
#include <bfd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include <stdint.h>


int bfd_show (char *filename)
{
  bfd *abfd = bfd_openr (filename, NULL);

  fprintf (stdout, "Filename = %s\n", abfd->filename);

  bfd_check_format (abfd, bfd_object);

  long storage_needed;
  asymbol **symbol_table;
  long number_of_symbols;
  long i;

  storage_needed = bfd_get_symtab_upper_bound (abfd);
  if (storage_needed < 0) {
    fprintf (stderr, "Error storage_needed < 0\n");
    return 0;
  }
  if (storage_needed == 0) {
    fprintf (stderr, "Error storage_needed == 0\n");
    return 0;
  }

  symbol_table = (asymbol **) malloc (storage_needed);

  number_of_symbols = bfd_canonicalize_symtab (abfd, symbol_table);

  fprintf (stdout, "number_of_symbols=%ld\n", number_of_symbols);

  if (number_of_symbols < 0) {
    fprintf (stderr, "Error: number_of_symbols < 0\n");
    return 0;
  }
  for (i = 0; i < number_of_symbols; i++) {
    fprintf (stdout, "SymbolName=%s : ", bfd_asymbol_name (symbol_table[i]));
    if ((symbol_table[i]->flags & BSF_FUNCTION) != 0x00) {
      fprintf (stdout, "BSF_Function ");
    } else if ((symbol_table[i]->flags & BSF_LOCAL) != 0x00) {
      fprintf (stdout, "BSF_Local ");
    } else if ((symbol_table[i]->flags & BSF_GLOBAL) != 0x00) {
      fprintf (stdout, "BSF_global ");
    } else {
      fprintf (stdout, "BSF_others ");
    }

    fprintf (stdout, "%08x %08x\n", bfd_asymbol_base (symbol_table[i]), bfd_asymbol_value (symbol_table[i]));
  }


  return 0;
}
