// Filename: dnaTrans.cxx
// Created by:  drose (07Dec04)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001 - 2004, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://etc.cmu.edu/panda3d/docs/license/ .
//
// To contact the maintainers of this program write to
// panda3d-general@lists.sourceforge.net .
//
////////////////////////////////////////////////////////////////////

#include "load_dna_file.h"
#include "filename.h"
#include "config_linmath.h"

#include "panda_getopt_long.h"

Filename output_filename;

// Short command-line options.
static const char *short_options = "o:h";

// Long command-line options.
enum CommandOptions {
  CO_hpr = 256,
  CO_nhpr,
  CO_help,
};

static struct option long_options[] = {
  { "hpr", no_argument, NULL, CO_hpr },
  { "nhpr", no_argument, NULL, CO_nhpr },
  { "help", no_argument, NULL, CO_help },
  { NULL }
};

void
show_usage() {
  std::cerr
    << "\nUsage:\n"
    << "  dna-trans [opts] -o output.dna input.dna\n"
    << "  dna-trans -h\n\n";
}

void show_help() {
  show_usage();
  std::cerr
    << "dna-trans can be used to read a Toontown DNA file, check it for valid\n"
    << "syntax, and output an essentially equivalent DNA file.\n\n"

    << "Options:\n\n"

    << "  -hpr\n"
    << "        Use the old-style 'hpr' syntax when writing the output DNA file.\n"
    << "        The resulting file will be backward-compatible with old DNA versions\n"
    << "        of the DNA loader.\n\n"

    << "  -nhpr\n"
    << "        Use the new-style 'nhpr' syntax when writing the output DNA file.\n\n"
    << "  If neither -hpr nor -nhpr is specified, the default is based on the\n"
    << "  current value of temp-hpr-fix.\n\n"

    << "  -o output_name\n"
    << "        Write the output DNA file to the indicated filename.\n\n";
}

int
main(int argc, char *argv[]) {
  extern char *optarg;
  extern int optind;
  int flag;

  bool temp_hpr_fix = true;

  flag = getopt_long_only(argc, argv, short_options, long_options, NULL);
  while (flag != EOF) {
    switch (flag) {
    case 'o':
      output_filename = Filename::from_os_specific(optarg);
      break;

    case CO_hpr:
      // Force old-style hpr's.
      temp_hpr_fix = false;
      break;

    case CO_nhpr:
      // Force new-style hpr's.
      temp_hpr_fix = true;
      break;

    case 'h':
    case CO_help:
      show_help();
      exit(0);

    default:
      exit(1);
    }
    flag = getopt_long_only(argc, argv, short_options, long_options, NULL);
  }

  argc -= (optind-1);
  argv += (optind-1);

  if (argc != 2 || output_filename.empty()) {
    show_usage();
    exit(1);
  }

  Filename input_filename = Filename::from_os_specific(argv[1]);

  DNAStorage dna_store;
  PT(DNAData) dna_data = load_DNA_file_AI(&dna_store, input_filename);

  if (!dna_data->write_dna(output_filename, std::cerr, &dna_store)) {
    exit(1);
  }

  return (0);
}
