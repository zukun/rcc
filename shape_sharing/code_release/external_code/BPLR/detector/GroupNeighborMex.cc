// matlab include
#include "mex.h"
#include "matrix.h"

#include <stdlib.h>
#include <functional>
#include <numeric>
#include <algorithm>
#include <vector>
#include <math.h>
#include <list>

using namespace std;

class feat {
public:
	double ref_ptr[2];
	double ref_scale;
	//double center_ptr[2];
	vector<double> member_ptrs;
	vector<double> member_scales;
	vector<double> member_inds;
	double bbox[4];
	double bbox_size;
	double n_member;
	double magnif;
public:
	feat();
	feat(const feat& that);
	feat& operator = (const feat& that);
};

feat::feat()
: member_ptrs(0), 
member_scales(0),
member_inds(0),
ref_scale(0.0), 
bbox_size(0.0),
n_member(0.0),
magnif(0.0)
{
	ref_ptr[0] = 0;
	ref_ptr[1] = 0;
	//center_ptr[0] = 0.0;
	//center_ptr[1] = 0.0;
	bbox[0] = 0.0;
	bbox[1] = 0.0;
	bbox[2] = 0.0;
	bbox[3] = 0.0;
}
	
feat::feat(const feat &that)
: ref_scale(that.ref_scale), 
member_ptrs(that.member_ptrs), 
member_scales(that.member_scales),
member_inds(that.member_inds),
bbox_size(that.bbox_size),
n_member(that.n_member),
magnif(that.magnif)
{
	ref_ptr[0] = that.ref_ptr[0];
	ref_ptr[1] = that.ref_ptr[1];
	//center_ptr[0] = that.center_ptr[0];
	//center_ptr[1] = that.center_ptr[1];
	bbox[0] = that.bbox[0];
	bbox[1] = that.bbox[1];
	bbox[2] = that.bbox[2];
	bbox[3] = that.bbox[3];
}

feat& feat::operator =(const feat& that)
{
	if ( this == &that ) return *this;
	ref_scale = that.ref_scale; 
	member_ptrs = that.member_ptrs; 
	member_scales = that.member_scales;
	member_inds = that.member_inds;
	bbox_size = that.bbox_size;
	n_member = that.n_member;
	magnif = that.magnif;
	ref_ptr[0] = that.ref_ptr[0];
	ref_ptr[1] = that.ref_ptr[1];
	//center_ptr[0] = that.center_ptr[0];
	//center_ptr[1] = that.center_ptr[1];
	bbox[0] = that.bbox[0];
	bbox[1] = that.bbox[1];
	bbox[2] = that.bbox[2];
	bbox[3] = that.bbox[3];
	return *this;
}


//template<typename T>
//class MyLessThan
//{
//public:
//	bool operator() (const pair<T, unsigned int>& left, const pair<T, unsigned int>& right) 
//	{
//		bool ret(false);
//		if  ( left.first == right.first ) { // in-place sort
//			ret = left.second < right.second;
//		}
//		else {
//			ret = left.first < right.first;
//		}
//		return ret;
//		
//	}
//};

template<typename T>
class MyLessThan
{
public:
	bool operator() (const pair<T, unsigned int>& left, const pair<T, unsigned int>& right) 
	{
		return (left.first < right.first);
	
	}
};

void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
// rhs (input): 
// neighbor_inds (1xn cell)
// topology_dist (1xn cell)
// ptrs (2xn matrix)
// scales (1xn vector)
// topology_order_k (a scalar)
// euclidean_scale (a scalar)
// min_scale (a scalar)
// min_feat_size (a scalar)
// img_size (1x2 vector)
// lhs (output):
// feats (structure array)
// uids (1D cell)
{
 	if ( nrhs != 9 ) { 
		return;
	}
	
	// parameters
	const unsigned int n_ptr = (unsigned int)mxGetNumberOfElements(prhs[0]);
	const unsigned int topology_order_k = (unsigned int)mxGetScalar(prhs[4]);
	const double euc_scale_f = mxGetScalar(prhs[5]);
	const unsigned int max_member = topology_order_k*topology_order_k;
	const double min_member = 1;
	const double min_octave = euc_scale_f;
	const double PI = 2*acos(0.0);
	const double scale_bound_const = 64*15;
	const double min_feat_var = 1.5;
	const double max_feat_var = 16;
	const unsigned int max_iter = 1;
	const double min_scale = mxGetScalar(prhs[6]);
	const double min_feat_size = mxGetScalar(prhs[7]);
	const double* img_size = mxGetPr(prhs[8]);
	const double imh = img_size[0];
	const double imw = img_size[1];
	const double min_feat_bbox_size = 10.0*10.0; 
	
	// feature
	vector<feat> feats;
	double* ptrs = mxGetPr(prhs[2]);
	double* scales = mxGetPr(prhs[3]);
	for (unsigned int i=0 ; i < n_ptr ; i++ ) {
		
		for ( unsigned int j=0 ; j < max_iter ; j++) {
			double scale_factor = pow(2.0, (int)j);
			double feat_magnif = min_octave*scale_factor;

			// topology 
			const mxArray* mx_member_inds = mxGetCell(prhs[0],i);
			const mxArray* mx_member_td = mxGetCell(prhs[1],i);			
			unsigned int n_member = mxGetNumberOfElements(mx_member_inds);
			vector<double> member_inds(n_member);
			vector<double> member_ptrs(2*n_member);
			vector<double> member_scales(n_member);
			vector<pair<double, unsigned int> > member_td(n_member);
			
			double* member_inds_ = mxGetPr(mx_member_inds);
			double* member_td_ = mxGetPr(mx_member_td);
			double ref_ptr[2];
			ref_ptr[0] = ptrs[2*i];
			ref_ptr[1] = ptrs[2*i+1];
			double ref_scale = max(scales[i], min_scale);
			for ( unsigned int k=0 ; k < n_member ; k++) {
				unsigned int idx = member_inds_[k];
				idx--; // zero-based index
				member_inds[k] = idx;
				member_ptrs[2*k] = ptrs[2*idx];
				member_ptrs[2*k+1] = ptrs[2*idx+1];
				member_scales[k] = scales[idx];
				member_td[k].first = member_td_[k];
				member_td[k].second = k;
			}
			
			// euclidean
			vector<double> member_d(n_member);
			for ( unsigned int k=0 ; k < n_member; k++) {
				double d = sqrt( (member_ptrs[2*k]-ref_ptr[0])*(member_ptrs[2*k]-ref_ptr[0]) +
					(member_ptrs[2*k+1]-ref_ptr[1])*(member_ptrs[2*k+1]-ref_ptr[1]) );
				member_d[k] = d;
			}
			double feat_var = sqrt(1+scale_bound_const/(scales[i]*scales[i]));
			if ( feat_var < min_feat_var ) feat_var = min_feat_var;
			else if ( feat_var > max_feat_var ) feat_var = max_feat_var;
			// cut by euclidean scale
			vector<unsigned int> member_flag;
			for ( unsigned int k=0 ; k < n_member; k++) {
				if (  member_d[k] < feat_magnif*ref_scale && member_scales[k] <= feat_var*scales[i] ) 
					member_flag.push_back(k);					
			}
			n_member = member_flag.size();
			vector<double> member_inds__(n_member);
			vector<double> member_ptrs__(2*n_member);
			vector<double> member_scales__(n_member);	
			vector<pair<double, unsigned int> > member_td__(n_member);
			double feat_size(0.0);
			for ( unsigned int k=0 ; k < n_member; k++) {
				unsigned int idx = member_flag[k];
				member_inds__[k] = member_inds[idx];
				member_ptrs__[2*k] = member_ptrs[2*idx];
				member_ptrs__[2*k+1] = member_ptrs[2*idx+1];
				member_scales__[k] = member_scales[idx];
				member_td__[k].first = member_td[idx].first;
				member_td__[k].second = k;
				feat_size += PI*member_scales[idx]*member_scales[idx];
			}
			member_inds.swap(member_inds__);
			member_ptrs.swap(member_ptrs__);
			member_scales.swap(member_scales__);
			member_td.swap(member_td__);

			if ( feat_size >= min_feat_size ) {

				// cut discontinous jump in topological dist
				sort(member_td.begin(), member_td.end(), MyLessThan<double>());
				int td_discont(-1);
				for ( unsigned k=0 ; k < n_member-1; k++) {
					double diff = member_td[k+1].first - member_td[k].first;
					if ( diff > 2 ) {
						td_discont = k;
						break;
					}
				}
				if ( td_discont >= 0) {
					member_td.resize(td_discont+1);
				}				
				n_member = member_td.size();
				vector<double> member_ptrs_tmp(2*n_member);
				vector<double> member_scales_tmp(n_member);
				vector<double> member_inds_tmp(n_member);
				for ( unsigned k=0 ; k < n_member ; k++) {
					double idx = member_td[k].second;
					member_ptrs_tmp[2*k] = member_ptrs[2*idx];
					member_ptrs_tmp[2*k+1] = member_ptrs[2*idx+1];
					member_scales_tmp[k] = member_scales[idx];
					member_inds_tmp[k] = member_inds[idx];
				}
				member_inds.swap(member_inds_tmp);
				member_ptrs.swap(member_ptrs_tmp);
				member_scales.swap(member_scales_tmp);

				// pick max_member (for efficiency of descriptor computation)
				if ( n_member > max_member ) {
					member_ptrs.resize(2*max_member);
					member_scales.resize(max_member);
					member_inds.resize(max_member);
				}
				//else {
				//	member_inds.resize(max_member,-2); //later add by 1 --> -1
				//}
				
				// center ptr
				//n_member = member_scales.size();				
				//double cx(0.0), cy(0.0);
				//for (unsigned int k=0 ; k < n_member ; k++) {
				//	cx += member_ptrs[2*k];
				//	cy += member_ptrs[2*k+1];
				//}
				//cx /= n_member;
				//cy /= n_member;
				//double min_d(1e12);
				//unsigned int min_idx(0);
				//for (unsigned int k=0 ; k < n_member ; k++) {
				//	double d_tmp = (member_ptrs[2*k] - cx)*(member_ptrs[2*k] - cx) + 
				//		(member_ptrs[2*k+1] - cy)*(member_ptrs[2*k+1] - cy);
				//	if ( min_d > d_tmp ) min_idx = k;
				//}
				//double center_ptr[2];
				//center_ptr[0] = member_ptrs[2*min_idx];
				//center_ptr[1] = member_ptrs[2*min_idx+1];

				// compute boundbox of feat
				double lx(1e12), rx(-1.0), ty(1e12), dy(-1.0);
				n_member = member_scales.size();		
				for ( unsigned k=0 ; k < n_member ; k++ ) {
					double lx_k = member_ptrs[2*k] - member_scales[k];
					double rx_k = member_ptrs[2*k] + member_scales[k];
					double ty_k = member_ptrs[2*k+1] - member_scales[k];
					double dy_k = member_ptrs[2*k+1] + member_scales[k];

					if ( lx > lx_k ) lx = lx_k;
					if ( rx < rx_k ) rx = rx_k;
					if ( ty > ty_k ) ty = ty_k;
					if ( dy < dy_k ) dy = dy_k;
				}

				lx = max(1.0, lx);
				rx = min(imw, rx);
				ty = max(1.0, ty);
				dy = min(imh, dy);
				double bbox[4];
				bbox[0] = lx;
				bbox[1] = ty;
				bbox[2] = rx;
				bbox[3] = dy;

				feat f;
				f.bbox[0] = bbox[0];
				f.bbox[1] = bbox[1];
				f.bbox[2] = bbox[2];
				f.bbox[3] = bbox[3];
				f.bbox_size = (rx-lx+1)*(dy-ty+1);
				//f.center_ptr[0] = center_ptr[0];
				//f.center_ptr[1] = center_ptr[1];
				f.magnif = feat_magnif;
				f.member_ptrs = member_ptrs;
				f.member_scales = member_scales;
				f.member_inds = member_inds;
				f.n_member = n_member;
				f.ref_ptr[0] = ref_ptr[0];
				f.ref_ptr[1] = ref_ptr[1];
				f.ref_scale = ref_scale;
				feats.push_back(f);
			
			}
		}
	}

	// remove small features
	size_t n_feat = feats.size();
	vector<unsigned int> feat_flag;
	for ( unsigned int i=0 ; i < n_feat ; i++ ) {
		unsigned int n_member = feats[i].n_member;
		double feat_bbox_size = feats[i].bbox_size;
		double feat_size(0.0);
		for ( unsigned int j=0 ; j < n_member ; j++) {
			feat_size += PI*feats[i].member_scales[j]*feats[i].member_scales[j];
		}
		if ( (n_member >= min_member) && ((feat_size >= min_feat_size) || (feat_bbox_size >= min_feat_bbox_size)) )
			feat_flag.push_back(i);
	}
	
	// output
	n_feat = feat_flag.size();
	mwSize dims[2] = {1, n_feat };
	const char *field_names[] = {"bbox", "bbox_size", "magnif", "member_inds", "member_ptrs", "member_scales", "n_member", "ref_ptr", "ref_scale"};
	//const char *field_names[] = {"bbox", "bbox_size", "center_ptr", "magnif", "member_inds", "member_ptrs", "member_scales", "n_member", "ref_ptr", "ref_scale"};
	//const char *field_names[] = {"bbox", "bbox_size", "center_ptr", "magnif", "member_ptrs", "member_scales", "n_member", "ref_ptr", "ref_scale"};
	const int n_field = sizeof(field_names)/sizeof(*field_names);
	plhs[0] = mxCreateStructArray(2, dims, n_field, field_names);
	int bbox_field = mxGetFieldNumber(plhs[0],"bbox");
    int bbox_size_field = mxGetFieldNumber(plhs[0],"bbox_size");
	//int center_ptr_field = mxGetFieldNumber(plhs[0],"center_ptr");
    int magnif_field = mxGetFieldNumber(plhs[0],"magnif");
	int member_inds_field = mxGetFieldNumber(plhs[0],"member_inds");
	int member_ptrs_field = mxGetFieldNumber(plhs[0],"member_ptrs");
    int member_scales_field = mxGetFieldNumber(plhs[0],"member_scales");
	int n_member_field = mxGetFieldNumber(plhs[0],"n_member");
    int ref_ptr_field = mxGetFieldNumber(plhs[0],"ref_ptr");
	int ref_scale_field = mxGetFieldNumber(plhs[0],"ref_scale");
	
	unsigned int feat_cnt(0);
	double n_max_member(0);
	for (vector<unsigned int>::iterator it = feat_flag.begin() ; it != feat_flag.end() ; it++, feat_cnt++) {
		unsigned int i = *it;
		unsigned int n_member = feats[i].n_member;
		// bbox
		mxArray* mx_bbox = mxCreateDoubleMatrix(4,1,mxREAL);
		double* bbox = mxGetPr(mx_bbox);
		bbox[0] = feats[i].bbox[0];
		bbox[1] = feats[i].bbox[1];
		bbox[2] = feats[i].bbox[2];
		bbox[3] = feats[i].bbox[3];
		mxSetFieldByNumber(plhs[0],feat_cnt,bbox_field,mx_bbox);
		
		// bbox_size
		mxArray* mx_bbox_size = mxCreateDoubleScalar(feats[i].bbox_size);
		mxSetFieldByNumber(plhs[0],feat_cnt,bbox_size_field,mx_bbox_size);

		// center_ptr
		//mxArray* mx_center_ptr = mxCreateDoubleMatrix(2,1,mxREAL);
		//double* center_ptr = mxGetPr(mx_center_ptr);
		//center_ptr[0] = feats[i].center_ptr[0];
		//center_ptr[1] = feats[i].center_ptr[1];
		//mxSetFieldByNumber(plhs[0],feat_cnt,center_ptr_field,mx_center_ptr);

		// magnif
		mxArray* mx_magnif = mxCreateDoubleScalar(feats[i].magnif);
		mxSetFieldByNumber(plhs[0],feat_cnt,magnif_field,mx_magnif);

		// member inds
		unsigned int n_member_inds = feats[i].member_inds.size();
		mxArray* mx_member_inds = mxCreateDoubleMatrix(1,n_member_inds,mxREAL);
		double* member_inds = mxGetPr(mx_member_inds);
		for ( unsigned int k=0 ; k < n_member_inds ; k++) {
			member_inds[k] = feats[i].member_inds[k]+1; //one-based index
		}
		mxSetFieldByNumber(plhs[0],feat_cnt,member_inds_field,mx_member_inds);

		// member_ptrs
		mxArray* mx_member_ptrs = mxCreateDoubleMatrix(2,n_member,mxREAL);
		double* member_ptrs = mxGetPr(mx_member_ptrs);
		for ( unsigned int k=0 ; k < 2*n_member ; k++) {
			member_ptrs[k] = feats[i].member_ptrs[k];
		}
		mxSetFieldByNumber(plhs[0],feat_cnt,member_ptrs_field,mx_member_ptrs);

		// member scales
		mxArray* mx_member_scales = mxCreateDoubleMatrix(1,n_member,mxREAL);
		double* member_scales = mxGetPr(mx_member_scales);
		for ( unsigned int k=0 ; k < n_member ; k++) {
			member_scales[k] = feats[i].member_scales[k];
		}
		mxSetFieldByNumber(plhs[0],feat_cnt,member_scales_field,mx_member_scales);
		
		// n_member
		mxArray* mx_n_member = mxCreateDoubleScalar(feats[i].n_member);
		mxSetFieldByNumber(plhs[0],feat_cnt,n_member_field,mx_n_member);
		if ( n_max_member < feats[i].n_member ) n_max_member = feats[i].n_member;

		// ref_ptr
		mxArray* mx_ref_ptr = mxCreateDoubleMatrix(2,1,mxREAL);
		double* ref_ptr = mxGetPr(mx_ref_ptr);
		ref_ptr[0] = feats[i].ref_ptr[0];
		ref_ptr[1] = feats[i].ref_ptr[1];
		mxSetFieldByNumber(plhs[0],feat_cnt,ref_ptr_field,mx_ref_ptr);

		// ref_scale
		mxArray* mx_ref_scale = mxCreateDoubleScalar(feats[i].ref_scale);
		mxSetFieldByNumber(plhs[0],feat_cnt,ref_scale_field,mx_ref_scale);

    }

	// uid
	/*plhs[1] = mxCreateDoubleMatrix((unsigned int)n_max_member, n_feat, mxREAL);
	double* uid = (double*)mxGetData(plhs[1]);
	feat_cnt = 0;
	for (vector<unsigned int>::iterator it = feat_flag.begin() ; it != feat_flag.end() ; it++, feat_cnt++) {
		unsigned int i = *it;
		sort(feats[i].member_inds.begin(), feats[i].member_inds.end());
		unsigned int n_member_inds = feats[i].member_inds.size();
		unsigned int stride_i = feat_cnt*n_max_member;	
		for (unsigned int j=0 ; j < n_member_inds ; j++) {
			uid[stride_i + j] = feats[i].member_inds[j]+1;
		}
	}*/
	plhs[1] = mxCreateNumericMatrix((unsigned int)n_max_member, n_feat, mxUINT32_CLASS, mxREAL);
	unsigned int* uid = (unsigned int*)mxGetData(plhs[1]);
	feat_cnt = 0;
	for (vector<unsigned int>::iterator it = feat_flag.begin() ; it != feat_flag.end() ; it++, feat_cnt++) {
		unsigned int i = *it;
		sort(feats[i].member_inds.begin(), feats[i].member_inds.end());
		unsigned int n_member_inds = feats[i].member_inds.size();
		unsigned int stride_i = feat_cnt*n_max_member;	
		for (unsigned int j=0 ; j < n_member_inds ; j++) {
			uid[stride_i + j] = feats[i].member_inds[j]+1;
		}
	}

}