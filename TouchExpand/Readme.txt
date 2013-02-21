This software implements the touch-expand algorithm and demonstrates how it can be applied for shape-from-oriented points reconstruction.
The algorithm and its application were suggested in:

		Victor Lempitsky and Yuri Boykov.
		Global Optimization for Shape Fitting.
		In proceedings of IEEE COnference on Computer Vision and Pattern Recognition (CVPR),
		2007

This software also contains mincut/maxflow code by Vladimir Kolmogorov and Yuri Boykov (version 2.2), which follows
		
		An Experimental Comparison of Min-Cut/Max-Flow Algorithms
		for Energy Minimization in Vision.
		Yuri Boykov and Vladimir Kolmogorov.
		In IEEE Transactions on Pattern Analysis and Machine Intelligence (PAMI), 
		September 2004

If you use this software in your research, you should cite
the two aforementioned papers in any resulting publication.

*******  LICENSE  ***********************************************
Copyright 2008 
	Victor Lempitsky (victorlempitsky@gmail.com), 
	Yuri Boykov (yuri@csd.uwo.ca), 
	Vladimir Kolmogorov (vnk@adastral.ucl.ac.uk),
	The University of Western Ontario

1) The maxflow subpart of the software is covered by the special license described in the MAXFLOW\README.TXT file. 
The rest of the software is covered by the following terms.

2) This software and its modifications can be used and distributed for research purposes only. 

3) The University of Western Ontario has the right to distribute this software to third parties for research purposes.

4) The research publications derived from the research using this software must cite the two publications given above.

5) The commercial use of this code, any of its parts, or its modifications is not permited.

6) The copyright notices must not be removed in case of any modifications.

7) This Licence commences on the date it is electronically or physically delivered to you and continues in effect unless you fail to comply with any of the terms of the License and fail to cure such breach within 30 days of becoming aware of the breach, in which case the Licence automatically terminates

8) This Licence is governed by the laws of Candaa and all disputes arising from or relating to this Licence must be brought in Toronto, Ontario.


9) THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
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



*******  FILES  ***********************************************

1) TouchExpand.h and TouchExpand.cpp

These files implement the touch-expand algorithm on 3D voxel grid graphs. Please see the comments in TouchExpand.h about the usage.

2) ShapeFitting.h, ShapeFitting.cpp, FitShape.cpp

These files implement the shape fitting procedure. Touch-expand algorithm is called from inside (see FitShape.cpp). The shape is output in a form of 3D binarized voxel grid (1=inside, 0=outside). Any isosurface extraction software can be used to turn this into surface representation.

3) TouchExpand.sln TouchExpand.vcproj

Project and solution files for MS Visual Studio 2005. 
The software was tested under Visual C 2005, Windows Vista 32-bit, but it should be usable in other environments as well. 
TouchExpand part does not use any system-specific code or formats.
ShapeFitting part uses input/output in binary format, so one should take care about little vs big-endian caveat.

3) maxflow\*.*

This folder contains maxflow software by Vladimir Kolmogorov and Yuri Boykov (version 2.2). It can be also obtained at the webpage of Vladimir Kolmogorov.

4) Stanford_Bunny\*.* 

This folder contains the test dataset for the shape fitting, created from the range scans obtained from Stanford 3D Scanning Repository. Please see the Stanford_Bunny\readme.txt for the description of input/output formats.

5) ShowSegmentation.m

This simple MATALB script allows to visualize the output of the algorithm (binary segmentation). Note, that this is a simple script that simply uses MATALB's isosurface to extract the surface from the binary segmentation (this is not what was used in the paper). The extracted surface will be jaggy and running out of memory is possible at high resolution.


