#!/usr/bin/python
"""Converter program for PCST solutions to Wuerzburg graph data (to be imported in Excel)

I am 

* reading in the input graph and the solution
* outputting the solution to a file

The three arguments are files holding the nodes and edge lists and the solution, resp.
Author:     Gunnar W. Klau
Version:    1.10
History:    20 Sep 07, 1.10 support of permuted instances, different output style (guwek)
	    10 Jun 07, 1.00 created (guwek)
"""
import sys
import re    
n = 0 # number of nodces
min_weight = {} # the smallest negative weight 
sum_weights = {} # sum of all transformed weights
z = 0 # objective function value
m = 0 # number of edges
nodes = {}
edges = {}

origweights = {}
weights = {}
nodeidx = {}

#Some functions:

       
def parse_nodes(nodeFile):
    """Parse the nodes from the file containing the nodes
    """
    global n # this function modifies the global variable n
    global min_weight # and min_weight
    global sum_weights # and this one
    global weights
    global origweights
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
	        origweights[(j, i)] = weights[(j, i)]
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
            i = i + 1
            line = edgeFile.readline()
        m = i - 1
    except:
        print 'Failed parsing edges'
        return

def parse_sol(solFile):
    """Parse the solution
    """
    try:
        z = {}
        no_permut = len(solFile)
        print "analyzing " + str(no_permut) + " solution files"
        outputFile_node = open(sys.argv[3], 'w')
        outputFile_node.write("label\t")
        for j in range(1, no_permut + 1): outputFile_node.write('\tscore' + str(j))
        outputFile_node.write("\n")
       
        in_solution = {} 
       
        for j in range(1, no_permut + 1): 
            z[j] = 0
	    for i in range(1, n+1): in_solution[(j, i)] = False
	    line = solFile[j].readline()
            while line[0:3] != '===': # we are in the node section
                L = line.split()
	        in_solution[(j, eval(L[0]))] = True
	        z[j] += origweights[(j, eval(L[0]))]
		line = solFile[j].readline()
	    print "z = ", z[j]
	 
        for i in range(1, n+1):
            outputFile_node.write(nodes[i])
            for j in range(1, no_permut + 1):
                outputFile_node.write("\t")
	        if in_solution[(j, i)]: outputFile_node.write(str(origweights[(j, i)]))
                else: outputFile_node.write("n/a          ")
            outputFile_node.write("\n")
        outputFile_node.write("#subnet score")
        for j in range(1, no_permut + 1): outputFile_node.write("\t" + str(z[j]))
        outputFile_node.write("\n")
        outputFile_node.close()
    except:
       print 'Failed parsing solution'
       return	


def readandwrite():
    """Read the input files."""
    try:
        #Open and parse input files.
        nodeFile = open(sys.argv[1], 'r')
        edgeFile = open(sys.argv[2], 'r')
	solFile = {}
	for i in range(5, len(sys.argv)): solFile[i-4]  = open(sys.argv[i], 'r')
        parse_nodes(nodeFile)
        parse_edges(edgeFile)
        parse_sol(solFile)
        nodeFile.close()
    	edgeFile.close()
        for i in range(5, len(sys.argv)): solFile[i-4].close()
	return 
    except:
        print 'problem parsing input'
        #Put here some more information - usage...

def printUsage():
    """Print list of arguments."""
    print 'Usage: heinz2wue.py nodeFile edgeFile solFile nodeSolFile edgeSolFile'


#And now: The program.

if len(sys.argv) < 5:
    printUsage()
else:
    readandwrite()
