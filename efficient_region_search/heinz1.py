#!/usr/bin/python
"""converts Wuerzburg data to PCST data, calls heinz and does everything 

Author:     Gunnar W. Klau
Version:    1.11
History:     7 Dec 07, 1.16 changed options to heuristics on again. works much better
                            for our instances. 
            30 Nov 07, 1.15 added option -a and changed default settings for dhea
                            now, heuristics are turned off (see e-mail Ivana)
            21 Sep 07, 1.10 better option support (guwek)
    	    18 Sep 07, 1.00 created (guwek)
"""

### schoener machen:
## * environment-var ILM_ACCESS_FILE ueberpruefen
## * check whether sol dir exists


import sys
import re
import os
import glob
from optparse import OptionParser

#And now: The program.
parser = OptionParser()
parser.add_option("-n", "--nodefile", dest="nodeFile", help="WUE file with node information", metavar="FILE")
parser.add_option("-e", "--edgefile", dest="edgeFile", help="WUE file with edge information", metavar="FILE")
parser.add_option("-s", "--no_subopt", dest="no_subopt", help="number of suboptimal solutions", default=1)
parser.add_option("-d", "--subopt_diff", dest="subopt_diff", help="difference between sol (Ham. dist in % or -1)", default=-1)
parser.add_option("-t", "--tolerance", dest="tolerance", help="solutions may be this percentage off optimum (default: 0)", default=0)
parser.add_option("-H", "--heinztmp", dest="heinztmp", help="heinz tmp dir", default="heinztmp")
parser.add_option("-g", "--generate", dest="generate_instances", help="do first step (generate the transformed", default=True)
parser.add_option("-r", "--run", dest="run_heinz", help="run PCST code on transformed instances", default=True)
parser.add_option("-c", "--collect", dest="collect_solutions", help="collect solutions and write heinz outputfile", default=True)
parser.add_option("-a", "--additional", dest="additional_dhea_options", help="additional options for dhea", default="")
parser.add_option("-v", "--verbose", dest="verbose", help="verbose output", default=False)

(options, args) = parser.parse_args()

nodeFile = options.nodeFile
edgeFile = options.edgeFile
no_subopt = options.no_subopt
subopt_diff = options.subopt_diff
tolerance = options.tolerance
heinztmp = options.heinztmp
generate_instances = options.generate_instances
run_heinz = options.run_heinz
collect_solutions = options.collect_solutions
additional_dhea_options = options.additional_dhea_options
verbose = options.verbose

if generate_instances == True:
  if os.path.exists(heinztmp): 
    print "deleting old tmp dir... ",
    os.popen("rm -rf " + heinztmp)
    print "done."
  os.popen("mkdir " + heinztmp)

  cmd_str = "wue2stein.py " + nodeFile + " " + edgeFile + " " + heinztmp + "/heinz.tmp"

  print "running " + cmd_str + "... ",
  sys.stdout.flush() 
  os.popen(cmd_str)
  print "done."

if run_heinz == True:
  if verbose: verb_string = ""
  else: verb_string = "2>/dev/null"
  for heinz in glob.glob(heinztmp + "/heinz*"):
    cmd_str = "dhea ifile " + heinz + " solutiondir " + heinztmp + " odir " + heinztmp + " tolerance " + str(tolerance) + \
    " no_subopt " + str(no_subopt) + " subopt_diff " + str(subopt_diff) + " " + additional_dhea_options + verb_string

    print "running " + cmd_str + "... ",
    sys.stdout.flush() 
    os.popen(cmd_str)
    print "done."

if collect_solutions==True: 
  if os.path.exists(heinztmp + "/heinz.tmp.permuted.2.0.solution"): # we deal with permutations
    solfilelist = glob.glob(heinztmp + "/*.solution")
    no_permut = len(solfilelist)
    print "I have found " + str(no_permut) + " solutions of permuted instances"
    nodeSolFile = nodeFile + ".hnz"
    edgeSolFile = edgeFile + ".hnz"
    cmd_str = "heinz2wue.py " + nodeFile + " " + edgeFile + " " + nodeSolFile + " " + edgeSolFile
    for solfile in solfilelist: cmd_str += " " + solfile
    print "running " + cmd_str + " ... ",
    sys.stdout.flush()   
    os.popen(cmd_str)
    print "done."
  else:  
    for k in range (0, int(no_subopt)):
      solFile = heinztmp + "/heinz.tmp.permuted.1." + str(k) + ".solution"
      nodeSolFile = nodeFile + "." + str(k) + ".hnz"
      edgeSolFile = edgeFile + "." + str(k) + ".hnz"
      cmd_str = "heinz2wue.py " + nodeFile + " " + edgeFile + " " + nodeSolFile + " " + edgeSolFile + " " + solFile
      print "running " + cmd_str + " ... ",
      sys.stdout.flush() 
      os.popen(cmd_str)
      print "done."
