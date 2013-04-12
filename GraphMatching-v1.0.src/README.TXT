GRAPH MATCHING, version 1.0.
Implementation of a dual decomposition method for the graph matching
optimization problem described in 

	Feature Correspondence via Graph Matching: Models and Global Optimization
	Lorenzo Torresani, Vladimir Kolmogorov and Carsten Rother.
	In European Conference on Computer Vision (ECCV), October 2008. 

If you use this software for research purposes, you should cite the paper
above in any resulting publication.

Please send comments to vnk@adastral.ucl.ac.uk.

##################################################################

License & disclaimer:

    Copyright 2008 Vladimir Kolmogorov (vnk@adastral.ucl.ac.uk)

    This software can be used for research and evaluation purposes only. Commercial use is prohibited.
    Public redistribution of the code or its derivatives is prohibited.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


##################################################################

The code has two additional features not described in the paper:

- Local subproblems are solved via a branch-and-bound technique rather than
  exhaustive search. (This is controlled by GraphMatching::Options::use_branch_and_bound).

- An option of adding tree subproblems has been added.
  (A tree subproblem is defined be a tree over points in one of the images, say in the left image.
  An edge between assignments <p,p'> and <q,q'> is included in the subproblem if and only
  edge (p,q) belongs to the tree. The uniqueness constraint in the other image is ignored,
  with one exception: assignments <p,p'> and <q,p'> cannot be both active if edge (p,q)
  belongs to the tree.

##################################################################

For interface functions, see GraphMatching.h. Alternatively, you can load
a problem from a file as shown in example.cpp. Specification of file format:

// Angular parentheses mean that it should be replaced with an integer number,
// curly parentheses mean a floating point number.
// Point and assignment id's are integers starting from 0.

c comment line
p <N0> <N1> <A> <E>     // # points in the left image, # points in the right image, # assignments, # edges
a <a> <i0> <i1> {cost}  // specify assignment
e <a> <b> {cost}        // specify edge

i0 <id> {xi} {yi}       // optional - specify coordinate of a point in the left image
i1 <id> {xi} {yi}       // optional - specify coordinate of a point in the left image
n0 <i> <j>              // optional - specify that points <i> and <j> in the left image are neighbors
n1 <i> <j>              // optional - specify that points <i> and <j> in the right image are neighbors

