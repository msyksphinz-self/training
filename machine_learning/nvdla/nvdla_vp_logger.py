#!/usr/bin/python3.6

import numpy as np
import sys

import nvdlav1_reg_list as reglist

argvs = sys.argv

def read_without_empty (fp):
    while True:
        line = fp.readline()
        if not line:
            print("Inside Hit!")
            return ""
        elif line.strip() != '':
            return line.strip()


if len(argvs) != 2 :
  print("usage: nvdal_vp_logger.py sc.log")
  exit()

print("<Start Reading %s...>" % (argvs[1]))

with open(argvs[1]) as vp_fp:
    while True:
        sc_line = read_without_empty (vp_fp)
        if sc_line == "":
            break

        # CSB reqest
        if sc_line.find('csb req to') > -1:
    	    # Address
    	    sc_line = read_without_empty(vp_fp)
    	    addr = int(sc_line.split(' ')[5], 16)
    	    # Data
    	    sc_line = read_without_empty(vp_fp)
    	    data = int(sc_line.split(' ')[5], 16)
    	    # Is Write
    	    sc_line = read_without_empty(vp_fp)
    	    is_write = True if int(sc_line.split(' ')[6], 16) == 0x1 else False
    	    # NPosted
    	    sc_line = read_without_empty(vp_fp)
    	    nposted = True if int(sc_line.split(' ')[5], 16) == 0x1 else False

    	    if is_write :
    	        print("CSB[%04x(%-40s)] <= 0x%08x" % (addr, reglist.name_reglist(addr), data))
    	    else:
    	        # Read Data
    	        sc_line = read_without_empty(vp_fp)
    	        rdata = int(sc_line.split(' ')[9], 16)
    	        print("CSB[%04x(%-40s)] => 0x%08x" % (addr, reglist.name_reglist(addr), rdata))

vp_fp.close()
