#!/usr/bin/python
"""Converter program for Wuerzburg graph data (exported from Excel)
to PCST format (where again did this come from and where is the documentation?)
whatever    

I am 

* reading in the graph
* deleting all self loops
* transforming the costs

The two arguments are files holding the nodes and edge lists, resp.
Author:     Gunnar W. Klau (guwek)
Version:    1.10
	    
History:    19 Sep 07, 1.10 support for permuted node weights (guwek)
	    14 Jun 07, 1.00 created (guwek)
"""
import sys
import re    
n = 0 # number of nodces
min_weight = {} # the smallest negative weight 
sum_weights = {} # sum of all transformed weights
m = 0 # number of edges
nodes = {}
edges = {}

weights = {}
edgeweights = {}
nodeidx = {}
verbose = False

no_permut = 1  # no of permuted scores (1: no permuations)

#Some functions:


def parse_nodes(nodeFile):
    """Parse the nodes from the file containing the nodes
    """
    global n # this function modifies the global variable n
    global min_weight # and min_weight
    global sum_weights # and this one
    global weights
    global no_permut

    try:
       i = 1	      
       line = nodeFile.readline() # read header line
       line = nodeFile.readline()
       while line:
	 L = line.split();
	 no_permut = len(L)-1
	 nodes[i] = L[0]
	 for j in range (1, no_permut+1):
           weights[(j, i)] = float(L[j].replace(",", "."))
           if not min_weight.has_key(j): min_weight[j] = 0
	   min_weight[j] = min(min_weight[j], weights[(j, i)])
	 nodeidx[L[0]] = i
         i = i + 1
         line = nodeFile.readline()
       n = i - 1
       # now transform the weights and compute their sum
       for j in range (1, no_permut+1): sum_weights[j] = 0
       for i in range(1, n+1):
          for j in range (1, no_permut+1):
            weights[(j, i)] -= min_weight[j]
	    sum_weights[j] += weights[(j, i)]
    except:
       print 'Failed parsing nodes'
       return

def parse_edges(edgeFile):
    """Parse the edges from the file containing the edges
    """
    global m # this function modifies the global variable m

    try:
       i = 1	
       line = edgeFile.readline() # read header line
       line = edgeFile.readline()
       while line:
	 L = line.split()
         if L[0] != L[1]: # we remove self loops here
  	   edges[i] = [nodeidx[L[0]], nodeidx[L[1]]]
	   edgeweights[i] = 0
	   if len(L) == 3:
	      edgeweights[i] = float(L[2])
           i = i + 1
         line = edgeFile.readline()
       m = i - 1
    except:
       print 'Failed parsing edges'
       return



def read():
    """Read the input files."""
    try:
        #Open and parse input files.
        nodeFile = open(sys.argv[1], 'r')
        edgeFile = open(sys.argv[2], 'r')
	
        parse_nodes(nodeFile)
        parse_edges(edgeFile)
        nodeFile.close()
	edgeFile.close()
	return 
    except:
        print 'problem parsing input'
        #Put here some more information - usage...

def write():
    try:
        for j in range (1, no_permut+1):
	  filename = sys.argv[3] + ".permuted." + str(j)
	  outputFile = open(filename, 'w')
	  #Write some comment
          outputFile.write('#created by wue2stein (permuted file #' + str(j) + '). ' + str(n) + ' nodes, ' + str(m) + ' edges\n')  
          outputFile.write('#sum of all node weights: ' + str(sum_weights[j]))
          outputFile.write('\n#min weight: ' + str(min_weight[j]))
          outputFile.write('\nnode\n')
	  for i in range(1,n+1):
	     outputFile.write(str(i) + "\t0\t0\t" + str(weights[(j,i)]) + "\n")
          outputFile.write('link\n')
	  for i in range(1,m+1):
    	     outputFile.write(str(i) + "\t" + str(edges[i][0]) + "\t" + str(edges[i][1]) + "\t" + str(-min_weight[j] + edgeweights[i]) + "\n")
	  outputFile.close()

    except:
	print 'problem writing output'



        

def printUsage():
    """Print list of arguments."""
    print 'Usage: wue2stein.py nodeFile edgeFile steinFile'
#And now: The program.

if len(sys.argv) < 3:
    printUsage()
else:
    if len(sys.argv) > 3:
        if sys.argv[1] == '-v':
            verbose = True
    read()
    #if len(graph)!=0:
    write()