#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <string.h>
#include <stdarg.h>
#include "svm.h"
typedef float Qfloat;
typedef signed char schar;
#ifndef min
template <class T> inline T min(T x,T y) { return (x<y)?x:y; }
#endif
#ifndef max
template <class T> inline T max(T x,T y) { return (x>y)?x:y; }
#endif
template <class T> inline void swap(T& x, T& y) { T t=x; x=y; y=t; }
template <class S, class T> inline void clone(T*& dst, S* src, int n)
{
  dst = new T[n];
  memcpy((void *)dst,(void *)src,sizeof(T)*n);
}
#define INF HUGE_VAL
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))
#if 0
void info(const char *fmt,...)
{
  va_list ap;
  va_start(ap,fmt);
  vprintf(fmt,ap);
  va_end(ap);
}
void info_flush()
{
  fflush(stdout);
}
#else
void info(const char *fmt,...) {}
void info_flush() {}
#endif


//
// Kernel Cache
//
// l is the number of total data items
// size is the cache size limit in bytes
//
class Cache
{
public:
  Cache(int l,int size);
  ~Cache();

  // request data [0,len)
  // return some position p where [p,len) need to be filled
  // (p >= len if nothing needs to be filled)
  int get_data(const int index, Qfloat **data, int len);
  void swap_index(int i, int j);	// future_option
private:
  int l;
  int size;
  struct head_t
  {
    head_t *prev, *next;	// a cicular list
    Qfloat *data;
    int len;		// data[0,len) is cached in this entry
  };

  head_t* head;
  head_t lru_head;
  void lru_delete(head_t *h);
  void lru_insert(head_t *h);
};

Cache::Cache(int l_,int size_):l(l_),size(size_)
{
  head = (head_t *)calloc(l,sizeof(head_t));	// initialized to 0
  size /= sizeof(Qfloat);
  size -= l * sizeof(head_t) / sizeof(Qfloat);
  lru_head.next = lru_head.prev = &lru_head;
}

Cache::~Cache()
{
  for(head_t *h = lru_head.next; h != &lru_head; h=h->next)
    free(h->data);
  free(head);
}

void Cache::lru_delete(head_t *h)
{
  // delete from current location
  h->prev->next = h->next;
  h->next->prev = h->prev;
}

void Cache::lru_insert(head_t *h)
{
  // insert to last position
  h->next = &lru_head;
  h->prev = lru_head.prev;
  h->prev->next = h;
  h->next->prev = h;
}

int Cache::get_data(const int index, Qfloat **data, int len)
{
  head_t *h = &head[index];
  if(h->len) lru_delete(h);
  int more = len - h->len;

  if(more > 0)
  {
    // free old space
    while(size < more)
    {
      head_t *old = lru_head.next;
      lru_delete(old);
      free(old->data);
      size += old->len;
      old->data = 0;
      old->len = 0;
    }

    // allocate new space
    h->data = (Qfloat *)realloc(h->data,sizeof(Qfloat)*len);
    size -= more;
    swap(h->len,len);
  }

  lru_insert(h);
  *data = h->data;
  return len;
}

void Cache::swap_index(int i, int j)
{
  if(i==j) return;

  if(head[i].len) lru_delete(&head[i]);
  if(head[j].len) lru_delete(&head[j]);
  swap(head[i].data,head[j].data);
  swap(head[i].len,head[j].len);
  if(head[i].len) lru_insert(&head[i]);
  if(head[j].len) lru_insert(&head[j]);

  if(i>j) swap(i,j);
  for(head_t *h = lru_head.next; h!=&lru_head; h=h->next)
  {
    if(h->len > i)
    {
      if(h->len > j)
        swap(h->data[i],h->data[j]);
      else
      {
        // give up
        lru_delete(h);
        free(h->data);
        size += h->len;
        h->data = 0;
        h->len = 0;
      }
    }
  }
}

//
// Kernel evaluation
//
// the static method k_function is for doing single kernel evaluation
// the constructor of Kernel prepares to calculate the l*l kernel matrix
// the member function get_Q is for getting one column from the Q Matrix
//
class Kernel {
public:
  Kernel(int l, svm_node * const * x, const svm_parameter& param);
  virtual ~Kernel();

  static double k_function(const svm_node *x, const svm_node *y,
                           const svm_parameter& param);
  virtual Qfloat *get_Q(int column, int len) const = 0;
  virtual void swap_index(int i, int j) const	// no so const...
  {
    swap(x[i],x[j]);
    if(x_square) swap(x_square[i],x_square[j]);
  }
protected:

  double (Kernel::*kernel_function)(int i, int j) const;
  const svm_node **x;
private:
	
  double *x_square;

  // svm_parameter
  const int kernel_type;
  const double degree;
  const double gamma;
  const double coef0;

  static double dot(const svm_node *px, const svm_node *py);
  double kernel_linear(int i, int j) const
  {
    return dot(x[i],x[j]);
  }
  double kernel_poly(int i, int j) const
  {
    return pow(gamma*dot(x[i],x[j])+coef0,degree);
  }
  double kernel_rbf(int i, int j) const
  {
    return exp(-gamma*(x_square[i]+x_square[j]-2*dot(x[i],x[j])));
  }
  double kernel_sigmoid(int i, int j) const
  {
    return tanh(gamma*dot(x[i],x[j])+coef0);
  }
};

Kernel::Kernel(int l, svm_node * const * x_, const svm_parameter& param)
 :kernel_type(param.kernel_type), degree(param.degree),
  gamma(param.gamma), coef0(param.coef0)
{
  switch(kernel_type)
  {
  case LINEAR:
    kernel_function = &Kernel::kernel_linear;
    break;
  case POLY:
    kernel_function = &Kernel::kernel_poly;
    break;
  case RBF:
    kernel_function = &Kernel::kernel_rbf;
    break;
  case SIGMOID:
    kernel_function = &Kernel::kernel_sigmoid;
    break;
  }

  clone(x,x_,l);

  if(kernel_type == RBF)
  {
    x_square = new double[l];
    for(int i=0;i<l;i++)
      x_square[i] = dot(x[i],x[i]);
  }
  else
    x_square = 0;
}

Kernel::~Kernel()
{
  delete[] x;
  delete[] x_square;
}

double Kernel::dot(const svm_node *px, const svm_node *py)
{
  double sum = 0;
  while(px->index != -1 && py->index != -1)
  {
    if(px->index == py->index)
    {
      sum += px->value * py->value;
      ++px;
      ++py;
    }
    else
    {
      if(px->index > py->index)
        ++py;
      else
        ++px;
    }			
  }
  return sum;
}

double Kernel::k_function(const svm_node *x, const svm_node *y,
			  const svm_parameter& param)
{
  switch(param.kernel_type)
  {
  case LINEAR:
    return dot(x,y);
  case POLY:
    return pow(param.gamma*dot(x,y)+param.coef0,param.degree);
  case RBF:
  {
    double sum = 0;
    while(x->index != -1 && y->index !=-1)
    {
      if(x->index == y->index)
      {
        double d = x->value - y->value;
        sum += d*d;
        ++x;
        ++y;
      }
      else
      {
        if(x->index > y->index)
        {	
          sum += y->value * y->value;
          ++y;
        }
        else
        {
          sum += x->value * x->value;
          ++x;
        }
      }
    }

    while(x->index != -1)
    {
      sum += x->value * x->value;
      ++x;
    }

    while(y->index != -1)
    {
      sum += y->value * y->value;
      ++y;
    }
			
    return exp(-param.gamma*sum);
  }
  case SIGMOID:
    return tanh(param.gamma*dot(x,y)+param.coef0);
  case MATRIX:
    x += (int)(y->value);
    return x->value;
  default:
    return 0;	/* Unreachable */
  }
}

// Generalized SMO+SVMlight algorithm
// Solves:
//
//	min 0.5(\alpha^T Q \alpha) + b^T \alpha
//
//		y^T \alpha = \delta
//		y_i = +1 or -1
//		0 <= alpha_i <= Cp for y_i = 1
//		0 <= alpha_i <= Cn for y_i = -1
//
// Given:
//
//	Q, b, y, Cp, Cn, and an initial feasible point \alpha
//	l is the size of vectors and matrices
//	eps is the stopping criterion
//
// solution will be put in \alpha, objective value will be put in obj
//
class Solver {
public:
  Solver() {};
  virtual ~Solver() {};

  struct SolutionInfo {
    double obj;
    double rho;
    double upper_bound_p;
    double upper_bound_n;
    double r;	// for Solver_NU
  };

  void Solve(int l, const Kernel& Q, const double *b_, const schar *y_,
             double *alpha_, double Cp, double Cn, double eps,
             SolutionInfo* si, int shrinking);
protected:
  int active_size;
  schar *y;
  double *G;		// gradient of objective function
  enum { LOWER_BOUND, UPPER_BOUND, FREE };
  char *alpha_status;	// LOWER_BOUND, UPPER_BOUND, FREE
  double *alpha;
  const Kernel *Q;
  double eps;
  double Cp,Cn;
  double *b;
  int *active_set;
  double *G_bar;		// gradient, if we treat free variables as 0
  int l;
  bool unshrinked;	// XXX

  double get_C(int i)
  {
    return (y[i] > 0)? Cp : Cn;
  }
  void update_alpha_status(int i)
  {
    if(alpha[i] >= get_C(i))
      alpha_status[i] = UPPER_BOUND;
    else if(alpha[i] <= 0)
      alpha_status[i] = LOWER_BOUND;
    else alpha_status[i] = FREE;
  }
  bool is_upper_bound(int i) { return alpha_status[i] == UPPER_BOUND; }
  bool is_lower_bound(int i) { return alpha_status[i] == LOWER_BOUND; }
  bool is_free(int i) { return alpha_status[i] == FREE; }
  void swap_index(int i, int j);
  void reconstruct_gradient();
  virtual int select_working_set(int &i, int &j);
  virtual double calculate_rho();
  virtual void do_shrinking();
};

void Solver::swap_index(int i, int j)
{
  Q->swap_index(i,j);
  swap(y[i],y[j]);
  swap(G[i],G[j]);
  swap(alpha_status[i],alpha_status[j]);
  swap(alpha[i],alpha[j]);
  swap(b[i],b[j]);
  swap(active_set[i],active_set[j]);
  swap(G_bar[i],G_bar[j]);
}

void Solver::reconstruct_gradient()
{
  // reconstruct inactive elements of G from G_bar and free variables

  if(active_size == l) return;

  int i;
  for(i=active_size;i<l;i++)
    G[i] = G_bar[i] + b[i];
	
  for(i=0;i<active_size;i++)
    if(is_free(i))
    {
      const Qfloat *Q_i = Q->get_Q(i,l);
      double alpha_i = alpha[i];
      for(int j=active_size;j<l;j++)
        G[j] += alpha_i * Q_i[j];
    }
}

void Solver::Solve(int l, const Kernel& Q, const double *b_, const schar *y_,
		   double *alpha_, double Cp, double Cn, double eps,
		   SolutionInfo* si, int shrinking)
{
  this->l = l;
  this->Q = &Q;
  clone(b, b_,l);
  clone(y, y_,l);
  clone(alpha,alpha_,l);
  this->Cp = Cp;
  this->Cn = Cn;
  this->eps = eps;
  unshrinked = false;

  // initialize alpha_status
  {
    alpha_status = new char[l];
    for(int i=0;i<l;i++)
      update_alpha_status(i);
  }

  // initialize active set (for shrinking)
  {
    active_set = new int[l];
    for(int i=0;i<l;i++)
      active_set[i] = i;
    active_size = l;
  }

  // initialize gradient
  {
    G = new double[l];
    G_bar = new double[l];
    int i;
    for(i=0;i<l;i++)
    {
      G[i] = b[i];
      G_bar[i] = 0;
    }
    for(i=0;i<l;i++)
      if(!is_lower_bound(i))
      {
        Qfloat *Q_i = Q.get_Q(i,l);
        double alpha_i = alpha[i];
        int j;
        for(j=0;j<l;j++)
          G[j] += alpha_i*Q_i[j];
        if(is_upper_bound(i))
          for(j=0;j<l;j++)
            G_bar[j] += get_C(i) * Q_i[j];
      }
  }

  // optimization step

  int iter = 0;
  int counter = min(l,1000)+1;

  while(1)
  {
    // show progress and do shrinking

    if(--counter == 0)
    {
      counter = min(l,1000);
      if(shrinking) do_shrinking();
      info("."); info_flush();
    }

    int i,j;
    if(select_working_set(i,j)!=0)
    {
      // reconstruct the whole gradient
      reconstruct_gradient();
      // reset active set size and check
      active_size = l;
      info("*"); info_flush();
      if(select_working_set(i,j)!=0)
        break;
      else
        counter = 1;	// do shrinking next iteration
    }
		
    ++iter;

    // update alpha[i] and alpha[j], handle bounds carefully
		
    const Qfloat *Q_i = Q.get_Q(i,active_size);
    const Qfloat *Q_j = Q.get_Q(j,active_size);

    double C_i = get_C(i);
    double C_j = get_C(j);

    double old_alpha_i = alpha[i];
    double old_alpha_j = alpha[j];

    if(y[i]!=y[j])
    {
      double delta = (-G[i]-G[j])/max(Q_i[i]+Q_j[j]+2*Q_i[j],(Qfloat)0);
      double diff = alpha[i] - alpha[j];
      alpha[i] += delta;
      alpha[j] += delta;
			
      if(diff > 0)
      {
        if(alpha[j] < 0)
        {
          alpha[j] = 0;
          alpha[i] = diff;
        }
      }
      else
      {
        if(alpha[i] < 0)
        {
          alpha[i] = 0;
          alpha[j] = -diff;
        }
      }
      if(diff > C_i - C_j)
      {
        if(alpha[i] > C_i)
        {
          alpha[i] = C_i;
          alpha[j] = C_i - diff;
        }
      }
      else
      {
        if(alpha[j] > C_j)
        {
          alpha[j] = C_j;
          alpha[i] = C_j + diff;
        }
      }
    }
    else
    {
      double delta = (G[i]-G[j])/max(Q_i[i]+Q_j[j]-2*Q_i[j],(Qfloat)0);
      double sum = alpha[i] + alpha[j];
      alpha[i] -= delta;
      alpha[j] += delta;
      if(sum > C_i)
      {
        if(alpha[i] > C_i)
        {
          alpha[i] = C_i;
          alpha[j] = sum - C_i;
        }
      }
      else
      {
        if(alpha[j] < 0)
        {
          alpha[j] = 0;
          alpha[i] = sum;
        }
      }
      if(sum > C_j)
      {
        if(alpha[j] > C_j)
        {
          alpha[j] = C_j;
          alpha[i] = sum - C_j;
        }
      }
      else
      {
        if(alpha[i] < 0)
        {
          alpha[i] = 0;
          alpha[j] = sum;
        }
      }
    }

    // update G

    double delta_alpha_i = alpha[i] - old_alpha_i;
    double delta_alpha_j = alpha[j] - old_alpha_j;
		
    for(int k=0;k<active_size;k++)
    {
      G[k] += Q_i[k]*delta_alpha_i + Q_j[k]*delta_alpha_j;
    }

    // update alpha_status and G_bar

    {
      bool ui = is_upper_bound(i);
      bool uj = is_upper_bound(j);
      update_alpha_status(i);
      update_alpha_status(j);
      int k;
      if(ui != is_upper_bound(i))
      {
        Q_i = Q.get_Q(i,l);
        if(ui)
          for(k=0;k<l;k++)
            G_bar[k] -= C_i * Q_i[k];
        else
          for(k=0;k<l;k++)
            G_bar[k] += C_i * Q_i[k];
      }

      if(uj != is_upper_bound(j))
      {
        Q_j = Q.get_Q(j,l);
        if(uj)
          for(k=0;k<l;k++)
            G_bar[k] -= C_j * Q_j[k];
        else
          for(k=0;k<l;k++)
            G_bar[k] += C_j * Q_j[k];
      }
    }
  }

  // calculate rho

  si->rho = calculate_rho();

  // calculate objective value
  {
    double v = 0;
    int i;
    for(i=0;i<l;i++)
      v += alpha[i] * (G[i] + b[i]);

    si->obj = v/2;
  }

  // put back the solution
  {
    for(int i=0;i<l;i++)
      alpha_[active_set[i]] = alpha[i];
  }

  // juggle everything back
  /*{
    for(int i=0;i<l;i++)
    while(active_set[i] != i)
    swap_index(i,active_set[i]);
    // or Q.swap_index(i,active_set[i]);
    }*/

  si->upper_bound_p = Cp;
  si->upper_bound_n = Cn;

  info("\noptimization finished, #iter = %d\n",iter);

  delete[] b;
  delete[] y;
  delete[] alpha;
  delete[] alpha_status;
  delete[] active_set;
  delete[] G;
  delete[] G_bar;
}

// return 1 if already optimal, return 0 otherwise
int Solver::select_working_set(int &out_i, int &out_j)
{
  // return i,j which maximize -grad(f)^T d , under constraint
  // if alpha_i == C, d != +1
  // if alpha_i == 0, d != -1

  double Gmax1 = -INF;		// max { -grad(f)_i * d | y_i*d = +1 }
  int Gmax1_idx = -1;

  double Gmax2 = -INF;		// max { -grad(f)_i * d | y_i*d = -1 }
  int Gmax2_idx = -1;

  for(int i=0;i<active_size;i++)
  {
    if(y[i]==+1)	// y = +1
    {
      if(!is_upper_bound(i))	// d = +1
      {
        if(-G[i] > Gmax1)
        {
          Gmax1 = -G[i];
          Gmax1_idx = i;
        }
      }
      if(!is_lower_bound(i))	// d = -1
      {
        if(G[i] > Gmax2)
        {
          Gmax2 = G[i];
          Gmax2_idx = i;
        }
      }
    }
    else		// y = -1
    {
      if(!is_upper_bound(i))	// d = +1
      {
        if(-G[i] > Gmax2)
        {
          Gmax2 = -G[i];
          Gmax2_idx = i;
        }
      }
      if(!is_lower_bound(i))	// d = -1
      {
        if(G[i] > Gmax1)
        {
          Gmax1 = G[i];
          Gmax1_idx = i;
        }
      }
    }
  }

  if(Gmax1+Gmax2 < eps)
    return 1;

  out_i = Gmax1_idx;
  out_j = Gmax2_idx;
  return 0;
}

void Solver::do_shrinking()
{
  int i,j,k;
  if(select_working_set(i,j)!=0) return;
  double Gm1 = -y[j]*G[j];
  double Gm2 = y[i]*G[i];

  // shrink
	
  for(k=0;k<active_size;k++)
  {
    if(is_lower_bound(k))
    {
      if(y[k]==+1)
      {
        if(-G[k] >= Gm1) continue;
      }
      else	if(-G[k] >= Gm2) continue;
    }
    else if(is_upper_bound(k))
    {
      if(y[k]==+1)
      {
        if(G[k] >= Gm2) continue;
      }
      else	if(G[k] >= Gm1) continue;
    }
    else continue;

    --active_size;
    swap_index(k,active_size);
    --k;	// look at the newcomer
  }

  // unshrink, check all variables again before final iterations

  if(unshrinked || -(Gm1 + Gm2) > eps*10) return;
	
  unshrinked = true;
  reconstruct_gradient();

  for(k=l-1;k>=active_size;k--)
  {
    if(is_lower_bound(k))
    {
      if(y[k]==+1)
      {
        if(-G[k] < Gm1) continue;
      }
      else	if(-G[k] < Gm2) continue;
    }
    else if(is_upper_bound(k))
    {
      if(y[k]==+1)
      {
        if(G[k] < Gm2) continue;
      }
      else	if(G[k] < Gm1) continue;
    }
    else continue;

    swap_index(k,active_size);
    active_size++;
    ++k;	// look at the newcomer
  }
}

double Solver::calculate_rho()
{
  double r;
  int nr_free = 0;
  double ub = INF, lb = -INF, sum_free = 0;
  for(int i=0;i<active_size;i++)
  {
    double yG = y[i]*G[i];

    if(is_lower_bound(i))
    {
      if(y[i] > 0)
        ub = min(ub,yG);
      else
        lb = max(lb,yG);
    }
    else if(is_upper_bound(i))
    {
      if(y[i] < 0)
        ub = min(ub,yG);
      else
        lb = max(lb,yG);
    }
    else
    {
      ++nr_free;
      sum_free += yG;
    }
  }

  if(nr_free>0)
    r = sum_free/nr_free;
  else
    r = (ub+lb)/2;

  return r;
}

//
// Solver for nu-svm classification and regression
//
// additional constraint: e^T \alpha = constant
//
class Solver_NU : public Solver
{
public:
  Solver_NU() {}
  void Solve(int l, const Kernel& Q, const double *b, const schar *y,
             double *alpha, double Cp, double Cn, double eps,
             SolutionInfo* si, int shrinking)
  {
    this->si = si;
    Solver::Solve(l,Q,b,y,alpha,Cp,Cn,eps,si,shrinking);
  }
private:
  SolutionInfo *si;
  int select_working_set(int &i, int &j);
  double calculate_rho();
  void do_shrinking();
};

int Solver_NU::select_working_set(int &out_i, int &out_j)
{
  // return i,j which maximize -grad(f)^T d , under constraint
  // if alpha_i == C, d != +1
  // if alpha_i == 0, d != -1

  double Gmax1 = -INF;	// max { -grad(f)_i * d | y_i = +1, d = +1 }
  int Gmax1_idx = -1;

  double Gmax2 = -INF;	// max { -grad(f)_i * d | y_i = +1, d = -1 }
  int Gmax2_idx = -1;

  double Gmax3 = -INF;	// max { -grad(f)_i * d | y_i = -1, d = +1 }
  int Gmax3_idx = -1;

  double Gmax4 = -INF;	// max { -grad(f)_i * d | y_i = -1, d = -1 }
  int Gmax4_idx = -1;

  for(int i=0;i<active_size;i++)
  {
    if(y[i]==+1)	// y == +1
    {
      if(!is_upper_bound(i))	// d = +1
      {
        if(-G[i] > Gmax1)
        {
          Gmax1 = -G[i];
          Gmax1_idx = i;
        }
      }
      if(!is_lower_bound(i))	// d = -1
      {
        if(G[i] > Gmax2)
        {
          Gmax2 = G[i];
          Gmax2_idx = i;
        }
      }
    }
    else		// y == -1
    {
      if(!is_upper_bound(i))	// d = +1
      {
        if(-G[i] > Gmax3)
        {
          Gmax3 = -G[i];
          Gmax3_idx = i;
        }
      }
      if(!is_lower_bound(i))	// d = -1
      {
        if(G[i] > Gmax4)
        {
          Gmax4 = G[i];
          Gmax4_idx = i;
        }
      }
    }
  }

  if(max(Gmax1+Gmax2,Gmax3+Gmax4) < eps)
    return 1;

  if(Gmax1+Gmax2 > Gmax3+Gmax4)
  {
    out_i = Gmax1_idx;
    out_j = Gmax2_idx;
  }
  else
  {
    out_i = Gmax3_idx;
    out_j = Gmax4_idx;
  }
  return 0;
}

void Solver_NU::do_shrinking()
{
  double Gmax1 = -INF;	// max { -grad(f)_i * d | y_i = +1, d = +1 }
  double Gmax2 = -INF;	// max { -grad(f)_i * d | y_i = +1, d = -1 }
  double Gmax3 = -INF;	// max { -grad(f)_i * d | y_i = -1, d = +1 }
  double Gmax4 = -INF;	// max { -grad(f)_i * d | y_i = -1, d = -1 }

  int k;
  for(k=0;k<active_size;k++)
  {
    if(!is_upper_bound(k))
    {
      if(y[k]==+1)
      {
        if(-G[k] > Gmax1) Gmax1 = -G[k];
      }
      else	if(-G[k] > Gmax3) Gmax3 = -G[k];
    }
    if(!is_lower_bound(k))
    {
      if(y[k]==+1)
      {	
        if(G[k] > Gmax2) Gmax2 = G[k];
      }
      else	if(G[k] > Gmax4) Gmax4 = G[k];
    }
  }

  double Gm1 = -Gmax2;
  double Gm2 = -Gmax1;
  double Gm3 = -Gmax4;
  double Gm4 = -Gmax3;

  for(k=0;k<active_size;k++)
  {
    if(is_lower_bound(k))
    {
      if(y[k]==+1)
      {
        if(-G[k] >= Gm1) continue;
      }
      else	if(-G[k] >= Gm3) continue;
    }
    else if(is_upper_bound(k))
    {
      if(y[k]==+1)
      {
        if(G[k] >= Gm2) continue;
      }
      else	if(G[k] >= Gm4) continue;
    }
    else continue;

    --active_size;
    swap_index(k,active_size);
    --k;	// look at the newcomer
  }

  // unshrink, check all variables again before final iterations

  if(unshrinked || max(-(Gm1+Gm2),-(Gm3+Gm4)) > eps*10) return;
	
  unshrinked = true;
  reconstruct_gradient();

  for(k=l-1;k>=active_size;k--)
  {
    if(is_lower_bound(k))
    {
      if(y[k]==+1)
      {
        if(-G[k] < Gm1) continue;
      }
      else	if(-G[k] < Gm3) continue;
    }
    else if(is_upper_bound(k))
    {
      if(y[k]==+1)
      {
        if(G[k] < Gm2) continue;
      }
      else	if(G[k] < Gm4) continue;
    }
    else continue;

    swap_index(k,active_size);
    active_size++;
    ++k;	// look at the newcomer
  }
}

double Solver_NU::calculate_rho()
{
  int nr_free1 = 0,nr_free2 = 0;
  double ub1 = INF, ub2 = INF;
  double lb1 = -INF, lb2 = -INF;
  double sum_free1 = 0, sum_free2 = 0;

  for(int i=0;i<active_size;i++)
  {
    if(y[i]==+1)
    {
      if(is_lower_bound(i))
        ub1 = min(ub1,G[i]);
      else if(is_upper_bound(i))
        lb1 = max(lb1,G[i]);
      else
      {
        ++nr_free1;
        sum_free1 += G[i];
      }
    }
    else
    {
      if(is_lower_bound(i))
        ub2 = min(ub2,G[i]);
      else if(is_upper_bound(i))
        lb2 = max(lb2,G[i]);
      else
      {
        ++nr_free2;
        sum_free2 += G[i];
      }
    }
  }

  double r1,r2;
  if(nr_free1 > 0)
    r1 = sum_free1/nr_free1;
  else
    r1 = (ub1+lb1)/2;
	
  if(nr_free2 > 0)
    r2 = sum_free2/nr_free2;
  else
    r2 = (ub2+lb2)/2;
	
  si->r = (r1+r2)/2;
  return (r1-r2)/2;
}

//
// Q matrices for various formulations
//
class SVC_Q: public Kernel
{ 
public:
  SVC_Q(const svm_problem& prob, const svm_parameter& param, const schar *y_)
    :Kernel(prob.l, prob.x, param)
  {
    clone(y,y_,prob.l);
    this->kernel_type = param.kernel_type;
    cache = new Cache(prob.l,(int)(param.cache_size*(1<<20)));
  }
	
  Qfloat *get_Q(int i, int len) const
  {
    Qfloat *data;
    int start;
    if((start = cache->get_data(i,&data,len)) < len)
    {
      if( kernel_type == MATRIX)
      {
        for(int j=start; j<len; j++)
          data[j] = (Qfloat)(y[i]*y[j]*(x[i][(int)(x[j][0].value)].value));
      }
      else
      {
                      
        for(int j=start;j<len;j++)
          data[j] = (Qfloat)(y[i]*y[j]*(this->*kernel_function)(i,j));
      }
    }
    return data;
  }

  void swap_index(int i, int j) const
  {
    cache->swap_index(i,j);
    Kernel::swap_index(i,j);
    swap(y[i],y[j]);
  }

  ~SVC_Q()
  {
    delete[] y;
    delete cache;
  }
private:
  schar *y;
  Cache *cache;
  int kernel_type;
};

class ONE_CLASS_Q: public Kernel
{
public:
  ONE_CLASS_Q(const svm_problem& prob, const svm_parameter& param)
    :Kernel(prob.l, prob.x, param)
  {
    this->kernel_type = param.kernel_type;
    cache = new Cache(prob.l,(int)(param.cache_size*(1<<20)));
  }
	
  Qfloat *get_Q(int i, int len) const
  {
    Qfloat *data;
    int start;
    if((start = cache->get_data(i,&data,len)) < len)
    {
      if(kernel_type == MATRIX)
      {
        for(int j=start;j<len;j++)
          data[j] = (Qfloat)((x[i][(int)(x[j][0].value)].value));
      }
      else
      {
        for(int j=start;j<len;j++)
          data[j] = (Qfloat)(this->*kernel_function)(i,j);
      }
    }
    return data;
  }

  void swap_index(int i, int j) const
  {
    cache->swap_index(i,j);
    Kernel::swap_index(i,j);
  }

  ~ONE_CLASS_Q()
  {
    delete cache;
  }
private:
  Cache *cache;
  int kernel_type;
};

class SVR_Q: public Kernel
{ 
public:
  SVR_Q(const svm_problem& prob, const svm_parameter& param)
    :Kernel(prob.l, prob.x, param)
  {
    l = prob.l;
    cache = new Cache(l,(int)(param.cache_size*(1<<20)));
    sign = new schar[2*l];
    index = new int[2*l];
    for(int k=0;k<l;k++)
    {
      sign[k] = 1;
      sign[k+l] = -1;
      index[k] = k;
      index[k+l] = k;
    }
    buffer[0] = new Qfloat[2*l];
    buffer[1] = new Qfloat[2*l];
    next_buffer = 0;
    this->kernel_type = param.kernel_type;
  }

  void swap_index(int i, int j) const
  {
    swap(sign[i],sign[j]);
    swap(index[i],index[j]);
  }
	
  Qfloat *get_Q(int i, int len) const
  {
    Qfloat *data;
    int real_i = index[i];
    if(cache->get_data(real_i,&data,l) < l)
    {
      if(kernel_type == MATRIX)
      {
        for(int j=0; j<l; j++)
          data[j] = (Qfloat)((x[real_i][(int)(x[j][0].value)].value));
      }
      else
      {
        for(int j=0;j<l;j++)
          data[j] = (Qfloat)(this->*kernel_function)(real_i,j);
      }
    }

    // reorder and copy
    Qfloat *buf = buffer[next_buffer];
    next_buffer = 1 - next_buffer;
    schar si = sign[i];
    for(int j=0;j<len;j++)
      buf[j] = si * sign[j] * data[index[j]];
    return buf;
  }

  ~SVR_Q()
  {
    delete cache;
    delete[] sign;
    delete[] index;
    delete[] buffer[0];
    delete[] buffer[1];
  }
private:
  int l;
  Cache *cache;
  schar *sign;
  int *index;
  mutable int next_buffer;
  Qfloat* buffer[2];
  int kernel_type;
};

//
// construct and solve various formulations
//
static void solve_c_svc(
  const svm_problem *prob, const svm_parameter* param,
  double *alpha, Solver::SolutionInfo* si, double Cp, double Cn)
{
  int l = prob->l;
  double *minus_ones = new double[l];
  schar *y = new schar[l];

  int i;

  for(i=0;i<l;i++)
  {
    alpha[i] = 0;
    minus_ones[i] = -1;
    if(prob->y[i] > 0) y[i] = +1; else y[i]=-1;
  }

  Solver s;
  s.Solve(l, SVC_Q(*prob,*param,y), minus_ones, y,
          alpha, Cp, Cn, param->eps, si, param->shrinking);

  double sum_alpha=0;
  for(i=0;i<l;i++)
    sum_alpha += alpha[i];

  if (Cp==Cn)
    info("nu = %f\n", sum_alpha/(Cp*prob->l));

  for(i=0;i<l;i++)
    alpha[i] *= y[i];
        
  delete[] minus_ones;
  delete[] y;
}

static void solve_nu_svc(
  const svm_problem *prob, const svm_parameter *param,
  double *alpha, Solver::SolutionInfo* si)
{
  int i;
  int l = prob->l;
  double nu = param->nu;

  schar *y = new schar[l];

  for(i=0;i<l;i++)
    if(prob->y[i]>0)
      y[i] = +1;
    else
      y[i] = -1;

  double sum_pos = nu*l/2;
  double sum_neg = nu*l/2;

  for(i=0;i<l;i++)
    if(y[i] == +1)
    {
      alpha[i] = min(1.0,sum_pos);
      sum_pos -= alpha[i];
    }
    else
    {
      alpha[i] = min(1.0,sum_neg);
      sum_neg -= alpha[i];
    }

  double *zeros = new double[l];

  for(i=0;i<l;i++)
    zeros[i] = 0;

  Solver_NU s;
  s.Solve(l, SVC_Q(*prob,*param,y), zeros, y,
          alpha, 1.0, 1.0, param->eps, si,  param->shrinking);
  double r = si->r;

  info("C = %f\n",1/r);

  for(i=0;i<l;i++)
    alpha[i] *= y[i]/r;

  si->rho /= r;
  si->obj /= (r*r);
  si->upper_bound_p = 1/r;
  si->upper_bound_n = 1/r;

  delete[] y;
  delete[] zeros;
}

static void solve_one_class(
  const svm_problem *prob, const svm_parameter *param,
  double *alpha, Solver::SolutionInfo* si)
{
  int l = prob->l;
  double *zeros = new double[l];
  schar *ones = new schar[l];
  int i;

  int n = (int)(param->nu*prob->l);	// # of alpha's at upper bound

  for(i=0;i<n;i++)
    alpha[i] = 1;
  alpha[n] = param->nu * prob->l - n;
  for(i=n+1;i<l;i++)
    alpha[i] = 0;

  for(i=0;i<l;i++)
  {
    zeros[i] = 0;
    ones[i] = 1;
  }

  Solver s;
  s.Solve(l, ONE_CLASS_Q(*prob,*param), zeros, ones,
          alpha, 1.0, 1.0, param->eps, si, param->shrinking);

  delete[] zeros;
  delete[] ones;
}

static void solve_epsilon_svr(
  const svm_problem *prob, const svm_parameter *param,
  double *alpha, Solver::SolutionInfo* si)
{
  int l = prob->l;
  double *alpha2 = new double[2*l];
  double *linear_term = new double[2*l];
  schar *y = new schar[2*l];
  int i;

  for(i=0;i<l;i++)
  {
    alpha2[i] = 0;
    linear_term[i] = param->p - prob->y[i];
    y[i] = 1;

    alpha2[i+l] = 0;
    linear_term[i+l] = param->p + prob->y[i];
    y[i+l] = -1;
  }

  Solver s;
  s.Solve(2*l, SVR_Q(*prob,*param), linear_term, y,
          alpha2, param->C, param->C, param->eps, si, param->shrinking);

  double sum_alpha = 0;
  for(i=0;i<l;i++)
  {
    alpha[i] = alpha2[i] - alpha2[i+l];
    sum_alpha += fabs(alpha[i]);
  }
  info("nu = %f\n",sum_alpha/(param->C*l));

  delete[] alpha2;
  delete[] linear_term;
  delete[] y;
}

static void solve_nu_svr(
  const svm_problem *prob, const svm_parameter *param,
  double *alpha, Solver::SolutionInfo* si)
{
  int l = prob->l;
  double C = param->C;
  double *alpha2 = new double[2*l];
  double *linear_term = new double[2*l];
  schar *y = new schar[2*l];
  int i;

  double sum = C * param->nu * l / 2;
  for(i=0;i<l;i++)
  {
    alpha2[i] = alpha2[i+l] = min(sum,C);
    sum -= alpha2[i];

    linear_term[i] = - prob->y[i];
    y[i] = 1;

    linear_term[i+l] = prob->y[i];
    y[i+l] = -1;
  }

  Solver_NU s;
  s.Solve(2*l, SVR_Q(*prob,*param), linear_term, y,
          alpha2, C, C, param->eps, si, param->shrinking);

  info("epsilon = %f\n",-si->r);

  for(i=0;i<l;i++)
    alpha[i] = alpha2[i] - alpha2[i+l];

  delete[] alpha2;
  delete[] linear_term;
  delete[] y;
}

//
// decision_function
//
struct decision_function
{
  double *alpha;
  double rho;	
  int nSV;
};

decision_function svm_train_one(
  const svm_problem *prob, const svm_parameter *param,
  double Cp, double Cn)
{
  double *alpha = Malloc(double,prob->l);
  Solver::SolutionInfo si;
  switch(param->svm_type)
  {
  case C_SVC:
    solve_c_svc(prob,param,alpha,&si,Cp,Cn);
    break;
  case NU_SVC:
    solve_nu_svc(prob,param,alpha,&si);
    break;
  case ONE_CLASS:
    solve_one_class(prob,param,alpha,&si);
    break;
  case EPSILON_SVR:
    solve_epsilon_svr(prob,param,alpha,&si);
    break;
  case NU_SVR:
    solve_nu_svr(prob,param,alpha,&si);
    break;
  }

  info("obj = %f, rho = %f\n",si.obj,si.rho);

  // output SVs

  int nSV = 0;
  int nBSV = 0;
  for(int i=0;i<prob->l;i++)
  {
    if(fabs(alpha[i]) > 0)
    {
      ++nSV;
      if(prob->y[i] > 0)
      {
        if(fabs(alpha[i]) >= si.upper_bound_p)
          ++nBSV;
      }
      else
      {
        if(fabs(alpha[i]) >= si.upper_bound_n)
          ++nBSV;
      }
    }
  }

  info("nSV = %d, nBSV = %d\n",nSV,nBSV);

  decision_function f;
  f.alpha = alpha;
  f.rho = si.rho;
  f.nSV = nSV;
  return f;
}



// Platt's binary SVM Probablistic Output: an improvement from Lin et al.
void sigmoid_train(
  int l, const double *dec_values, const double *labels, 
  double& A, double& B)
{
  double prior1=0, prior0 = 0;
  int i;

  for (i=0;i<l;i++)
    if (labels[i] > 0) prior1+=1;
    else prior0+=1;
	
  int max_iter=100; 	// Maximal number of iterations
  double min_step=1e-10;	// Minimal step taken in line search
  double sigma=1e-3;	// For numerically strict PD of Hessian
  double eps=1e-5;
  double hiTarget=(prior1+1.0)/(prior1+2.0);
  double loTarget=1/(prior0+2.0);
  double *t=Malloc(double,l);
  double fApB,p,q,h11,h22,h21,g1,g2,det,dA,dB,gd,stepsize;
  double newA,newB,newf,d1,d2;
  int iter; 
	
  // Initial Point and Initial Fun Value
  A=0.0; B=log((prior0+1.0)/(prior1+1.0));
  double fval = 0.0;

  for (i=0;i<l;i++)
  {
    if (labels[i]>0) t[i]=hiTarget;
    else t[i]=loTarget;
    fApB = dec_values[i]*A+B;
    if (fApB>=0)
      fval += t[i]*fApB + log(1+exp(-fApB));
    else
      fval += (t[i] - 1)*fApB +log(1+exp(fApB));
  }
  for (iter=0;iter<max_iter;iter++)
  {
    // Update Gradient and Hessian (use H' = H + sigma I)
    h11=sigma; // numerically ensures strict PD
    h22=sigma;
    h21=0.0;g1=0.0;g2=0.0;
    for (i=0;i<l;i++)
    {
      fApB = dec_values[i]*A+B;
      if (fApB >= 0)
      {
        p=exp(-fApB)/(1.0+exp(-fApB));
        q=1.0/(1.0+exp(-fApB));
      }
      else
      {
        p=1.0/(1.0+exp(fApB));
        q=exp(fApB)/(1.0+exp(fApB));
      }
      d2=p*q;
      h11+=dec_values[i]*dec_values[i]*d2;
      h22+=d2;
      h21+=dec_values[i]*d2;
      d1=t[i]-p;
      g1+=dec_values[i]*d1;
      g2+=d1;
    }

    // Stopping Criteria
    if (fabs(g1)<eps && fabs(g2)<eps)
      break;

    // Finding Newton direction: -inv(H') * g
    det=h11*h22-h21*h21;
    dA=-(h22*g1 - h21 * g2) / det;
    dB=-(-h21*g1+ h11 * g2) / det;
    gd=g1*dA+g2*dB;


    stepsize = 1; 		// Line Search
    while (stepsize >= min_step)
    {
      newA = A + stepsize * dA;
      newB = B + stepsize * dB;

      // New function value
      newf = 0.0;
      for (i=0;i<l;i++)
      {
        fApB = dec_values[i]*newA+newB;
        if (fApB >= 0)
          newf += t[i]*fApB + log(1+exp(-fApB));
        else
          newf += (t[i] - 1)*fApB +log(1+exp(fApB));
      }
      // Check sufficient decrease
      if (newf<fval+0.0001*stepsize*gd)
      {
        A=newA;B=newB;fval=newf;
        break;
      }
      else
        stepsize = stepsize / 2.0;
    }

    if (stepsize < min_step)
    {
      info("Line search fails in two-class probability estimates\n");
      break;
    }
  }

  if (iter>=max_iter)
    info("Reaching maximal iterations in two-class probability estimates\n");
  free(t);
}

double sigmoid_predict(double decision_value, double A, double B)
{
  double fApB = decision_value*A+B;
  if (fApB >= 0)
    return exp(-fApB)/(1.0+exp(-fApB));
  else
    return 1.0/(1+exp(fApB)) ;
}

// Method 2 from the multiclass_prob paper by Wu, Lin, and Weng
void multiclass_probability(int k, double **r, double *p)
{
  int t;
  int iter = 0, max_iter=100;
  double **Q=Malloc(double *,k);
  double *Qp=Malloc(double,k);
  double pQp, eps=0.001;
	
  for (t=0;t<k;t++)
  {
    p[t]=1.0/k;  // Valid if k = 1
    Q[t]=Malloc(double,k);
    Q[t][t]=0;
    for (int j=0;j<t;j++)
    {
      Q[t][t]+=r[j][t]*r[j][t];
      Q[t][j]=Q[j][t];
    }
    for (int j=t+1;j<k;j++)
    {
      Q[t][t]+=r[j][t]*r[j][t];
      Q[t][j]=-r[j][t]*r[t][j];
    }
  }
  for (iter=0;iter<max_iter;iter++)
  {
    // stopping condition, recalculate QP,pQP for numerical accuracy
    pQp=0;
    for (t=0;t<k;t++)
    {
      Qp[t]=0;
      for (int j=0;j<k;j++)
        Qp[t]+=Q[t][j]*p[j];
      pQp+=p[t]*Qp[t];
    }
    double max_error=0;
    for (t=0;t<k;t++)
    {
      double error=fabs(Qp[t]-pQp);
      if (error>max_error)
        max_error=error;
    }
    if (max_error<eps) break;
		
    for (t=0;t<k;t++)
    {
      double diff=(-Qp[t]+pQp)/Q[t][t];
      p[t]+=diff;
      pQp=(pQp+diff*(diff*Q[t][t]+2*Qp[t]))/(1+diff)/(1+diff);
      for (int j=0;j<k;j++)
      {
        Qp[j]=(Qp[j]+diff*Q[t][j])/(1+diff);
        p[j]/=(1+diff);
      }
    }
  }
  if (iter>=max_iter)
    info("Exceeds max_iter in multiclass_prob\n");
  for(t=0;t<k;t++) free(Q[t]);
  free(Q);
  free(Qp);
}

// Generalized Bradley-Terry Model
void GBT_multiclass_probability(int multiclass_type, int m, int k, double *rp, double *p, int **I)
{
  int iter=0,max_iter=1000;
  double mu=0.0, eps=0.001,
    *qp=Malloc(double, m),
    *qn=Malloc(double, m);
	
  if(multiclass_type == DENSE || multiclass_type == SPARSE)
    mu = 0.001;

  // initialize p
  for(int i=0; i<k; i++)
    p[i] = 1.0/k;

  // Algorithm 2
  double *delta=Malloc(double, k);
  for(iter=0; iter<max_iter; iter++)
  {
    for(int i=0; i<k; i++)
    {
      // update qp, qn
      for(int t=0; t<m; t++)
        qp[t] = qn[t] = 0.0;
      /*for(int t=0; t<m; t++)
        for(int j=0; j<k; j++)
        if(I[t][j]>0) qp[t] += p[j];
        else if(I[t][j]<0) qn[t] += p[j];*/
			
      for(int t=0; t<m; t++)
        if(I[t][i] != 0)
          for(int j=0; j<k; j++)
            if(I[t][j]>0) qp[t] += p[j];
            else if(I[t][j]<0) qn[t] += p[j];
					
      // Calculate update coefficients, check stopping condition
      double delta_d = 0.0, delta_n = 0.0; 	
      for(int j=0; j<m; j++)
      {
        if(I[j][i]>0)
        {
          delta_n += rp[j]/qp[j];
          delta_d += 1.0/(qp[j]+qn[j]);
        }
        else if(I[j][i]<0)
        {
          delta_n += (1.0-rp[j])/qn[j];
          delta_d += 1.0/(qp[j]+qn[j]);
        }
      }
      delta[i] = (delta_n+mu/p[i])/(delta_d+mu);
		
      p[i] = delta[i]*p[i];
      double sump = 0.0;
      for(int t=0; t<k; t++)
        sump += p[t];
      for(int t=0; t<k; t++)
        p[t] = p[t]/sump;
    }
		
    double max_error = 0.0;
    for(int i=0; i<k; i++)
      if(fabs(delta[i]-1.0)>max_error)
        max_error = fabs(delta[i]-1.0);
		
    if(max_error < eps) break;
  }
	
  if (iter>=max_iter)
    info("Exceeds max_iter in GBT_multiclass_prob\n");
	
  free(qp);
  free(qn);
  free(delta);
}


// Cross-validation decision values for probability estimates
void svm_binary_svc_probability(
  const svm_problem *prob, const svm_parameter *param,
  double Cp, double Cn, double& probA, double& probB)
{
  int i;
  int nr_fold = 5;
  int *perm = Malloc(int,prob->l);
  double *dec_values = Malloc(double,prob->l);

	
  // random shuffle
  for(i=0;i<prob->l;i++) perm[i]=i;
  for(i=0;i<prob->l;i++)
  {
    int j = i+rand()%(prob->l-i);
    swap(perm[i],perm[j]);
  }
  for(i=0;i<nr_fold;i++)
  {
    int begin = i*prob->l/nr_fold;
    int end = (i+1)*prob->l/nr_fold;
    int j,k;
    struct svm_problem subprob;

    subprob.l = prob->l-(end-begin);
    subprob.x = Malloc(struct svm_node*,subprob.l);
    subprob.y = Malloc(double,subprob.l);
			
    k=0;
    for(j=0;j<begin;j++)
    {
      subprob.x[k] = prob->x[perm[j]];
      subprob.y[k] = prob->y[perm[j]];
      ++k;
    }
    for(j=end;j<prob->l;j++)
    {
      subprob.x[k] = prob->x[perm[j]];
      subprob.y[k] = prob->y[perm[j]];
      ++k;
    }
    int p_count=0,n_count=0;
    for(j=0;j<k;j++)
      if(subprob.y[j]>0)
        p_count++;
      else
        n_count++;
		
    if(p_count==0 && n_count==0)
      for(j=begin;j<end;j++)
        dec_values[perm[j]] = 0;
    else if(p_count > 0 && n_count == 0)
      for(j=begin;j<end;j++)
        dec_values[perm[j]] = 1;
    else if(p_count == 0 && n_count > 0)
      for(j=begin;j<end;j++)
        dec_values[perm[j]] = -1;
    else
    {
      svm_parameter subparam = *param;
      subparam.probability=0;
      // For Generalized BT model, currently we do not consider weighted C
      subparam.C = param->C;
      subparam.nr_weight = 0;
			
      /*subparam.C=1.0;
        subparam.nr_weight=2;
        subparam.weight_label = Malloc(int,2);
        subparam.weight = Malloc(double,2);
        subparam.weight_label[0]=+1;
        subparam.weight_label[1]=-1;
        subparam.weight[0]=Cp;
        subparam.weight[1]=Cn;*/
      subprob.nr_binary = error_correcting_code(subparam.multiclass_type, svm_find_nr_class(&subprob), subprob.I);
      struct svm_model *submodel = svm_train(&subprob,&subparam);
      for(j=begin;j<end;j++)
      {
        svm_predict_values(submodel,prob->x[perm[j]],&(dec_values[perm[j]])); 
        // ensure +1 -1 order; reason not using CV subroutine
        dec_values[perm[j]] *= submodel->label[0];
      }		
      svm_destroy_model(submodel);
      svm_destroy_param(&subparam);
      free(subprob.x);
      free(subprob.y);
    }
  }		
  sigmoid_train(prob->l,dec_values,prob->y,probA,probB);
  free(dec_values);
  free(perm);
}

// Return parameter of a Laplace distribution 
double svm_svr_probability(
  const svm_problem *prob, const svm_parameter *param)
{
  int i;
  int nr_fold = 5;
  double *ymv = Malloc(double,prob->l);
  double mae = 0;

  svm_parameter newparam = *param;
  newparam.probability = 0;
  svm_cross_validation(prob,&newparam,nr_fold,ymv);
  for(i=0;i<prob->l;i++)
  {
    ymv[i]=prob->y[i]-ymv[i];
    mae += fabs(ymv[i]);
  }		
  mae /= prob->l;
  double std=sqrt(2*mae*mae);
  int count=0;
  mae=0;
  for(i=0;i<prob->l;i++)
    if (fabs(ymv[i]) > 5*std) 
      count=count+1;
    else 
      mae+=fabs(ymv[i]);
  mae /= (prob->l-count);
  info("Prob. model for test data: target value = predicted value + z,\nz: Laplace distribution e^(-|z|/sigma)/(2sigma),sigma= %g\n",mae);
  free(ymv);
  return mae;
}

// error-correcting code functions

// nr_binary for small number of class
static int dense[6] = {3,3,10,10,20,20};
static int sparse[3] = {3,10,30};

// check whether I is valid
// return 1 if invalid, 0 if valid
int check_I(int **In, int nr_binary, int nr_class){
  int c,i,j,k,s,z;

  // check for identical/complementary rows 
  for(i=0; i<nr_binary; i++)
    for(j=i+1; j<nr_binary; j++)
    {
      s = c = 0;
      for(k=0;k<nr_class;k++)
      {
        if(In[i][k] == In[j][k]) s++;
        if(In[i][k]+In[j][k] == 0) c++;
      }
			
      if(s==nr_class || c==nr_class)
        return 1;
    }
	
  // check for zero column
  for(i=0; i<nr_class; i++)
  {
    z = 0;
    for(j=0; j<nr_binary; j++)
      if(In[i][j] == 0) z++;
    if(z == nr_binary)
      return 1;
  }
	
  return 0;
}

int rho_score(int **In, int nr_binary, int nr_class){
  int i,j,k,rho,min = nr_binary;
	
  for(i=0; i<nr_class; i++)
    for(j=i+1; j<nr_class; j++)
    {	
      rho = 0;
      for(k=0; k<nr_binary; k++)
        rho += In[k][i] * In[k][j];
      if( nr_binary-rho < min) 
        min = nr_binary-rho;
    }
  return min;	
}

int error_correcting_code(int multiclass_type, int nr_class, int**& I)
{
  int nr_binary = 0;
  if(multiclass_type == ONE_ONE || nr_class <= 2)
  {
    nr_binary = nr_class*(nr_class-1)/2;
    I = Malloc(int *, nr_binary);
    int r,i,j=0,k=j+1;
    for (i=0; i<nr_binary;i++)
    {
      I[i]=Malloc(int, nr_class);
      for(r=0; r<nr_class;r++)
        I[i][r]=0;
      I[i][j]= +1; I[i][k]=-1;
      if (k==nr_class-1) 
      {
        j++; k=j+1;
      }
      else
        k++;
    }
  }
  else if(multiclass_type == ONE_ALL)
  {
    int i,j;
    nr_binary = nr_class;
    I = Malloc(int *, nr_binary);
    for(i=0; i<nr_binary; i++)
    {
      I[i] = Malloc(int, nr_class);
      for(j=0; j<nr_class; j++)
        I[i][j] = -1;
      I[i][i] = 1;
    }
  }
  else if(multiclass_type == DENSE)
  {
    int **In = NULL;
    int i,j,n,p,rho,max_rho=0;
		
    nr_binary = (int)round(10 * log(nr_class)/log(2.0));
    if(nr_class < 9)
      nr_binary = dense[nr_class-3];
		
    In = Malloc(int *, nr_binary);
    I = Malloc(int *, nr_binary);
    for(i=0; i<nr_binary; i++)
    {	
      In[i] = Malloc(int, nr_class);
      I[i] = Malloc(int, nr_class);
    }
		
    // Generate 100 I and select the best one 
    for(n = 0; n < 100; n++)
    {
      for(i=0; i<nr_binary; i++)
        for(j=0; j<nr_class; j++)
          In[i][j] = -1;
      for(i=0; i<nr_binary; i++)
      {
        for(j=0; j<nr_class/2; j++)
        {	
          p = rand() % nr_class;
          while(In[i][p] == 1)
            p = rand() % nr_class;
          In[i][p] = 1;
        }
      }
			
      if(check_I(In, nr_binary, nr_class))
      {
        n--;
        continue;
      }

      rho = rho_score(In, nr_binary, nr_class);
      if(rho > max_rho)
      {
        max_rho = rho;
        for(i=0; i<nr_binary; i++)
          memcpy(I[i], In[i], nr_class*sizeof(int));
      }
    }
		
    for(i=0; i<nr_binary; i++)
      free(In[i]);
    free(In);
  }
  else  // SPARSE
  {
    int **In = NULL;
    int i,j,n,f1,f2,rho,max_rho=0;
    float p;
		
    nr_binary = (int)round(15 * log(nr_class)/log(2.0));
    if(nr_class < 6)
      nr_binary = sparse[nr_class-3];
		
    In = Malloc(int *, nr_binary);
    I = Malloc(int *, nr_binary);
    for(i=0; i<nr_binary; i++)
    {	
      In[i] = Malloc(int, nr_class);
      I[i] = Malloc(int, nr_class);
    }
		
    // Generate 100 I and select the best one
    for(n=0; n<100; n++)
    {
      for(i=0; i<nr_binary; i++)
        do{
          f1=f2=0;
          for(j=0; j<nr_class; j++)
          {
            p = (rand()%1000)/1000.0;
            if(p > 0.75) f1=In[i][j]=1; 
            else if(p < 0.25) f2=In[i][j]=-1;
            else In[i][j]=0;
          }
        }while(!(f1*f2));
			
      if(check_I(In, nr_binary, nr_class))
      {
        n--;
        continue;
      }

      rho = rho_score(In, nr_binary, nr_class);
      if(rho > max_rho)
      {
        max_rho = rho;
        for(i=0; i<nr_binary; i++)
          memcpy(I[i], In[i], nr_class*sizeof(int));
      }
    }
    for(i=0; i<nr_binary; i++)
      free(In[i]);
    free(In);
  }
  return nr_binary;
}



//
// Interface functions
//
svm_model *svm_train(const svm_problem *prob, const svm_parameter *param)
{
  svm_model *model = Malloc(svm_model,1);
  model->param = *param;
  model->free_sv = 0;	// XXX

  if(param->svm_type == ONE_CLASS ||
     param->svm_type == EPSILON_SVR ||
     param->svm_type == NU_SVR)
  {
    // regression or one-class-svm
    model->nr_class = 2;
    model->label = NULL;
    model->nSV = NULL;
    model->probA = NULL; model->probB = NULL;
    model->sv_coef = Malloc(double *,1);

    if(param->probability && 
       (param->svm_type == EPSILON_SVR ||
        param->svm_type == NU_SVR))
    {
      model->probA = Malloc(double,1);
      model->probA[0] = svm_svr_probability(prob,param);
    }

    decision_function f = svm_train_one(prob,param,0,0);
    model->rho = Malloc(double,1);
    model->rho[0] = f.rho;

    int nSV = 0;
    int i;
    for(i=0;i<prob->l;i++)
      if(fabs(f.alpha[i]) > 0) ++nSV;
    model->l = nSV;
    model->SV = Malloc(svm_node *,nSV);
    model->sv_coef[0] = Malloc(double,nSV);
    int j = 0;
    for(i=0;i<prob->l;i++)
      if(fabs(f.alpha[i]) > 0)
      {
        model->SV[j] = prob->x[i];
        model->sv_coef[0][j] = f.alpha[i];
        ++j;
      }		

    free(f.alpha);
  }
  else
  {
    // classification
    // find out the number of classes
    int l = prob->l;
    int nr_binary = prob->nr_binary;
    int **I = prob->I;
    int max_nr_class = 16;
    int nr_class = 0;
    int *label = Malloc(int,max_nr_class);
    int *count = Malloc(int,max_nr_class);
    int *index = Malloc(int,l);
    int i;
		
    for(i=0;i<l;i++)
    {
      int this_label = (int)prob->y[i];
      int j;
      for(j=0;j<nr_class;j++)
        if(this_label == label[j])
        {
          ++count[j];
          break;
        }
      index[i] = j;
      if(j == nr_class)
      {
        if(nr_class == max_nr_class)
        {
          max_nr_class *= 2;
          label = (int *)realloc(label,max_nr_class*sizeof(int));
          count = (int *)realloc(count,max_nr_class*sizeof(int));
        }
        label[nr_class] = this_label;
        count[nr_class] = 1;
        ++nr_class;
      }
    }


    // group training data of the same class

    int *start = Malloc(int,nr_class);
    start[0] = 0;
    for(i=1;i<nr_class;i++)
      start[i] = start[i-1]+count[i-1];

    svm_node **x = Malloc(svm_node *,l);
		
    for(i=0;i<l;i++)
    {
      x[start[index[i]]] = prob->x[i];
      ++start[index[i]];
    }
		
    start[0] = 0;
    for(i=1;i<nr_class;i++)
      start[i] = start[i-1]+count[i-1];

    // calculate weighted C

    double *weighted_C = Malloc(double, nr_class);
    for(i=0;i<nr_class;i++)
      weighted_C[i] = param->C;
    for(i=0;i<param->nr_weight;i++)
    {	
      int j;
      for(j=0;j<nr_class;j++)
        if(param->weight_label[i] == label[j])
          break;
      if(j == nr_class)
        fprintf(stderr,"warning: class label %d specified in weight is not found\n", param->weight_label[i]);
      else
        weighted_C[j] *= param->weight[i];
    }

    // train k*(k-1)/2 models
		
    bool *nonzero = Malloc(bool,l);
    for(i=0;i<l;i++)
      nonzero[i] = false;
    decision_function *f = Malloc(decision_function,nr_binary);

    double *probA=NULL,*probB=NULL;
    if (param->probability)
    {
      probA=Malloc(double,nr_binary);
      probB=Malloc(double,nr_binary);
    }

    int p;
    for(i=0;i<nr_binary;i++)
    {
      svm_problem sub_prob;
      sub_prob.l = 0;
      for (int j=0; j < nr_class; j++)
        if (I[i][j]!=0)
          sub_prob.l += count[j];
      sub_prob.x = Malloc(svm_node *,sub_prob.l);
      sub_prob.y = Malloc(double,sub_prob.l);
			
      p=0;
      for(int j=0;j<nr_class;j++)
        if (I[i][j]!=0)
          for (int k=0;k<count[j];k++)
          {
            sub_prob.x[p] = x[start[j]+k];
            sub_prob.y[p] = I[i][j];
            p++;
          }
      if(param->probability)
        svm_binary_svc_probability(&sub_prob,param,param->C,param->C,probA[i],probB[i]);
			
      f[i] = svm_train_one(&sub_prob,param,param->C,param->C);
      free(sub_prob.x);
      free(sub_prob.y);
    }

    // build output
		
		
    model->nr_class = nr_class;
    model->nr_binary = nr_binary;
    model->I = Malloc(int *, nr_binary);
    for(int i=0; i<nr_binary; i++)
    {
      model->I[i] = Malloc(int, nr_class);
      memcpy(model->I[i], I[i], nr_class*sizeof(int));
    }
    model->label = Malloc(int,nr_class);
    for(i=0;i<nr_class;i++)
      model->label[i] = label[i];
		
    model->rho = Malloc(double,nr_binary);
    model->nSV_binary = Malloc(int,nr_binary);
    for(i=0;i<nr_binary;i++)
    {
      model->rho[i] = f[i].rho;
      model->nSV_binary[i] = f[i].nSV;
    }

    if(param->probability)
    {
      model->probA = Malloc(double,nr_binary);
      model->probB = Malloc(double,nr_binary);
      for(i=0;i<nr_binary;i++)
      {
        model->probA[i] = probA[i];
        model->probB[i] = probB[i];
      }
    }
    else
    {
      model->probA=NULL;
      model->probB=NULL;
    }

    for(i=0;i<nr_binary;i++)
    {
      p=0; 
      for(int j=0; j<nr_class; j++)
        if (I[i][j]!=0)
          for (int k=0;k<count[j];k++)
          {
            if (fabs(f[i].alpha[p]) > 0)
              nonzero[start[j]+k]=true;
            p++;
          }
    }

    int total_sv = 0;
    model->nSV = Malloc(int,nr_class);
    for(i=0;i<nr_class;i++)
    {
      int nSV = 0;
      for(int j=0;j<count[i];j++)
        if(nonzero[start[i]+j])
        {	
          ++nSV;
          ++total_sv;
        }
      model->nSV[i] = nSV;
    }
    info("Total nSV = %d\n",total_sv);

    model->l = total_sv;
    model->SV = Malloc(svm_node *,total_sv);

    int *xtosv=Malloc(int,l);
    p=0;
    for(i=0;i<l;i++)
      if (nonzero[i])
      {
        model->SV[p] = x[i];
        xtosv[i]=p;
        p++;
      }

    model->sv_coef = Malloc(double *,nr_binary);
    model->sv_ind = Malloc(int *,nr_binary);
		
    for(i=0;i<nr_binary;i++)
    {
      model->sv_ind[i] = Malloc(int,model->nSV_binary[i]);			
      model->sv_coef[i] = Malloc(double,model->nSV_binary[i]);			
      p=0; 
      int r=0;
      for(int j=0; j<nr_class; j++)
        if (I[i][j]!=0)
          for (int k=0;k<count[j];k++)
          {
            if (fabs(f[i].alpha[p]) > 0)
            {
              model->sv_ind[i][r]=xtosv[start[j]+k];
              model->sv_coef[i][r]=f[i].alpha[p];
              r++; 
            }
            p++;
          }
    }

    for(i=0;i<nr_binary;i++)
      free(I[i]);
    free(I);
    free(label);
    free(probA);
    free(probB);
    free(count);
    free(index);
    free(start);
    free(x);
    free(weighted_C);
    free(nonzero);
    free(xtosv);
    for(i=0;i<nr_binary;i++)
      free(f[i].alpha);
    free(f);
  }
  return model;
}


// stratified CV
void svm_cross_validation(const svm_problem *prob, const svm_parameter *param, int nr_fold, double *target)
{
  int i;
  int *perm = Malloc(int,prob->l);
  int *fold_start = Malloc(int,nr_fold+1);
  int l = prob->l;
  int max_nr_class = 16;
  int nr_class = 0;
  int **I = NULL;
  int nr_binary = 0;
	
  // random shuffle
  if(param->svm_type == C_SVC ||
     param->svm_type == NU_SVC)
  {
    int *label = Malloc(int,max_nr_class);
    int *count = Malloc(int,max_nr_class);
    int *index = Malloc(int,l);	
    int *fold_count = Malloc(int,nr_fold);
    int c;
    int min_count;
		
    for(i=0;i<l;i++)
    {
      int this_label = (int)prob->y[i];
      int j;
      for(j=0;j<nr_class;j++)
      {
        if(this_label == label[j])
        {
          ++count[j];
          break;
        }
      }
      index[i] = j;
      if(j == nr_class)
      {
        if(nr_class == max_nr_class)
        {
          max_nr_class *= 2;
          label = (int *)realloc(label,max_nr_class*sizeof(int));
          count = (int *)realloc(count,max_nr_class*sizeof(int));
        }
        label[nr_class] = this_label;
        count[nr_class] = 1;
        ++nr_class;
      }
    }

    min_count = count[0];
    for(i=0; i<nr_class; i++)
      if(count[i] < min_count)
        min_count = count[i];

    if(min_count >= nr_fold)
      nr_binary = error_correcting_code(param->multiclass_type, nr_class, I); 
    else
      nr_binary = 0;
		
		
		
    int *start = Malloc(int,nr_class);
    start[0] = 0;
    for(i=1;i<nr_class;i++)
      start[i] = start[i-1]+count[i-1];

    for(i=0;i<l;i++)
    {
      perm[start[index[i]]] = i;
      ++start[index[i]];
    }

    start[0] = 0;
    for(i=1;i<nr_class;i++)
      start[i] = start[i-1]+count[i-1];

    for(i=0;i<l;i++)
      index[i]=perm[i];
    for (c=0; c<nr_class; c++) 
      for(i=0;i<count[c];i++)
      {
        int j = i+rand()%(count[c]-i);
        swap(index[start[c]+j],index[start[c]+i]);
      }
    for(i=0;i<nr_fold;i++)
    {
      fold_count[i] = 0;
      for (c=0; c<nr_class;c++)
        fold_count[i]+=(i+1)*count[c]/nr_fold-i*count[c]/nr_fold;
    }
    fold_start[0]=0;
    for (i=1;i<=nr_fold;i++)
      fold_start[i] = fold_start[i-1]+fold_count[i-1];
    for (c=0; c<nr_class;c++)
      for(i=0;i<nr_fold;i++)
      {
        int begin = start[c]+i*count[c]/nr_fold;
        int end = start[c]+(i+1)*count[c]/nr_fold;
        for(int j=begin;j<end;j++)
        {
          perm[fold_start[i]] = index[j];
          fold_start[i]++;
        }
      }
    fold_start[0]=0;
    for (i=1;i<=nr_fold;i++)
      fold_start[i] = fold_start[i-1]+fold_count[i-1];
    free(index);
    free(start);	
    free(count);	
    free(fold_count);
  }
  else
  {
    for(i=0;i<prob->l;i++) perm[i]=i;
    for(i=0;i<prob->l;i++)
    {
      int j = i+rand()%(prob->l-i);
      swap(perm[i],perm[j]);
    }
    for(i=0;i<=nr_fold;i++)
      fold_start[i]=i*prob->l/nr_fold;
  }

  for(i=0;i<nr_fold;i++)
  {
    int begin = fold_start[i];
    int end = fold_start[i+1];
    int j,k;
    struct svm_problem subprob;

    subprob.l = prob->l-(end-begin);
    subprob.x = Malloc(struct svm_node*,subprob.l);
    subprob.y = Malloc(double,subprob.l);
			
    k=0;
    for(j=0;j<begin;j++)
    {
      subprob.x[k] = prob->x[perm[j]];
      subprob.y[k] = prob->y[perm[j]];
      ++k;
    }
    for(j=end;j<prob->l;j++)
    {
      subprob.x[k] = prob->x[perm[j]];
      subprob.y[k] = prob->y[perm[j]];
      ++k;
    }
		
    if(param->svm_type == C_SVC || param->svm_type == NU_SVC)
    {
      if(nr_binary > 0)
      {
        subprob.nr_binary = nr_binary;
        subprob.I = Malloc(int *, nr_binary);
        for(int i=0; i<nr_binary; i++)
        {
          subprob.I[i] = Malloc(int, nr_class);
          memcpy(subprob.I[i], I[i], nr_class*sizeof(int));
        }
      }
      else
        subprob.nr_binary = error_correcting_code(param->multiclass_type, svm_find_nr_class(&subprob), subprob.I);
    }
		
    struct svm_model *submodel = svm_train(&subprob,param);
    if(param->probability && 
       (param->svm_type == C_SVC || param->svm_type == NU_SVC))
    {
      double *prob_estimates=Malloc(double,svm_get_nr_class(submodel));
      for(j=begin;j<end;j++)
        target[perm[j]] = svm_predict_probability(submodel,prob->x[perm[j]],prob_estimates);
      free(prob_estimates);			
    }
    else
      for(j=begin;j<end;j++)
        target[perm[j]] = svm_predict(submodel,prob->x[perm[j]]);
    svm_destroy_model(submodel);
    free(subprob.x);
    free(subprob.y);
  }		
  if((param->svm_type == C_SVC || param->svm_type == NU_SVC) && nr_binary > 0)
  {
    for(int i=0; i<nr_binary; i++)
      free(I[i]);
    free(I);
  }
  free(perm);	
}

// non-stratified cross validation
/*
  void svm_cross_validation(const svm_problem *prob, const svm_parameter *param, int nr_fold, double *target)
  {
  int i;
  int *perm = Malloc(int,prob->l);

  // random shuffle
  for(i=0;i<prob->l;i++) perm[i]=i;
  for(i=0;i<prob->l;i++)
  {
  int j = i+rand()%(prob->l-i);
  swap(perm[i],perm[j]);
  }
  for(i=0;i<nr_fold;i++)
  {
  int begin = i*prob->l/nr_fold;
  int end = (i+1)*prob->l/nr_fold;
  int j,k;
  struct svm_problem subprob;

  subprob.l = prob->l-(end-begin);
  subprob.x = Malloc(struct svm_node*,subprob.l);
  subprob.y = Malloc(double,subprob.l);
			
  k=0;
  for(j=0;j<begin;j++)
  {
  subprob.x[k] = prob->x[perm[j]];
  subprob.y[k] = prob->y[perm[j]];
  ++k;
  }
  for(j=end;j<prob->l;j++)
  {
  subprob.x[k] = prob->x[perm[j]];
  subprob.y[k] = prob->y[perm[j]];
  ++k;
  }
  struct svm_model *submodel = svm_train(&subprob,param);
  if(param->probability && 
  (param->svm_type == C_SVC || param->svm_type == NU_SVC))
  {
  double *prob_estimates=Malloc(double,svm_get_nr_class(submodel));
  for(j=begin;j<end;j++)
  target[perm[j]] = svm_predict_probability(submodel,prob->x[perm[j]],prob_estimates);
  free(prob_estimates);			
  }
  else
  for(j=begin;j<end;j++)
  target[perm[j]] = svm_predict(submodel,prob->x[perm[j]]);
  svm_destroy_model(submodel);
  free(subprob.x);
  free(subprob.y);
  }		
  free(perm);	
  }
*/
int svm_get_svm_type(const svm_model *model)
{
  return model->param.svm_type;
}

int svm_get_nr_class(const svm_model *model)
{
  return model->nr_class;
}

int svm_find_nr_class(const svm_problem *prob)
{
  int max_nr_class = 16;
  int nr_class = 0;
  int *label = Malloc(int,max_nr_class);
  int i;
  for(i=0;i<prob->l;i++)
  {
    int this_label = (int)prob->y[i];
    int j;
    for(j=0;j<nr_class;j++)
      if(this_label == label[j])
        break;
		
    if(j == nr_class)
    {
      if(nr_class == max_nr_class)
      {
        max_nr_class *= 2;
        label = (int *)realloc(label,max_nr_class*sizeof(int));
      }
      label[nr_class] = this_label;
      ++nr_class;
    }
  }
  free(label);
  return nr_class;
}


void svm_get_labels(const svm_model *model, int* label)
{
  if (model->label != NULL)
    for(int i=0;i<model->nr_class;i++)
      label[i] = model->label[i];
}

double svm_get_svr_probability(const svm_model *model)
{
  if ((model->param.svm_type == EPSILON_SVR || model->param.svm_type == NU_SVR) &&
      model->probA!=NULL)
    return model->probA[0];
  else
  {
    info("Model doesn't contain information for SVR probability inference\n");
    return 0;
  }
}

void svm_predict_values(const svm_model *model, const svm_node *x, double* dec_values)
{
  if(model->param.svm_type == ONE_CLASS ||
     model->param.svm_type == EPSILON_SVR ||
     model->param.svm_type == NU_SVR)
  {
    double *sv_coef = model->sv_coef[0];
    double sum = 0;
    for(int i=0;i<model->l;i++)
      sum += sv_coef[i] * Kernel::k_function(x,model->SV[i],model->param);
    sum -= model->rho[0];
    *dec_values = sum;
  }
  else
  {
    int i;
    int nr_binary = model->nr_binary;
    int l = model->l;
		
    double *kvalue = Malloc(double,l);
    for(i=0;i<l;i++)
      kvalue[i] = Kernel::k_function(x,model->SV[i],model->param);

    for(i=0;i<nr_binary;i++)
    {
      double sum = 0;
      for (int j=0; j< model->nSV_binary[i];j++) {
        sum+= model->sv_coef[i][j]*kvalue[model->sv_ind[i][j]];
      }
      sum -= model->rho[i];
      dec_values[i] = sum;
    }
    free(kvalue);
  }
}

double svm_predict(const svm_model *model, const svm_node *x)
{
  if(model->param.svm_type == ONE_CLASS ||
     model->param.svm_type == EPSILON_SVR ||
     model->param.svm_type == NU_SVR)
  {
    double res;
    svm_predict_values(model, x, &res);
		
    if(model->param.svm_type == ONE_CLASS)
      return (res>0)?1:-1;
    else
      return res;
  }
  else
  {
    int i,j;
    int nr_class = model->nr_class;
    int nr_binary = model->nr_binary;
    double *dec_values = Malloc(double, nr_binary);
    svm_predict_values(model, x, dec_values);

    //for (int ii = 0; ii < nr_binary; ++ii) {
    //printf("%f ", dec_values[ii]);
    //}
    //printf("\n");
    double *vote = Malloc(double,nr_class);
    for(i=0;i<nr_class;i++)
      vote[i] = 0;
    //int pos=0;
    for(i=0;i<nr_class;i++)
      for(j=0;j<nr_binary;j++)
        vote[i] += exp(-model->I[j][i] * dec_values[j]);
    /*for(int j=i+1;j<nr_class;j++)
      {
      if(dec_values[pos++] > 0)
      ++vote[i];
      else
      ++vote[j];
      }*/

    int vote_min_idx = 0;
    for(i=1;i<nr_class;i++)
      if(vote[i] < vote[vote_min_idx])
        vote_min_idx = i;
		
    free(vote);
    free(dec_values);
    return model->label[vote_min_idx];
  }
}

double svm_predict_probability(
  const svm_model *model, const svm_node *x, double *prob_estimates)
{
  if ((model->param.svm_type == C_SVC || model->param.svm_type == NU_SVC) &&
      model->probA!=NULL && model->probB!=NULL)
  {
    int i;
    int nr_class = model->nr_class;
    int nr_binary = model->nr_binary;
    double *dec_values = Malloc(double, nr_binary);
    svm_predict_values(model, x, dec_values);

    double min_prob=1e-7;
    /*double **pairwise_prob=Malloc(double *,nr_class);
      for(i=0;i<nr_class;i++)
      pairwise_prob[i]=Malloc(double,nr_class);
      int k=0;
      for(i=0;i<nr_class;i++)
      for(int j=i+1;j<nr_class;j++)
      {
      pairwise_prob[i][j]=min(max(sigmoid_predict(dec_values[k],model->probA[k],model->probB[k]),min_prob),1-min_prob);
      pairwise_prob[j][i]=1-pairwise_prob[i][j];
      k++;
      }
    */
    double *rp=Malloc(double, nr_binary);
    for(int i=0; i<nr_binary; i++)
      rp[i]= min(max(sigmoid_predict(dec_values[i],model->probA[i],model->probB[i]),min_prob),1-min_prob);
		
    //multiclass_probability(nr_class,pairwise_prob,prob_estimates);
    GBT_multiclass_probability(model->param.multiclass_type, nr_binary, nr_class, rp, prob_estimates, model->I);

		
    int prob_max_idx = 0;
    for(i=1;i<nr_class;i++)
      if(prob_estimates[i] > prob_estimates[prob_max_idx])
        prob_max_idx = i;
    //for(i=0;i<nr_class;i++)
    //	free(pairwise_prob[i]);
    free(dec_values);
    //free(pairwise_prob);	     
    free(rp);
    return model->label[prob_max_idx];
  }
  else 
    return svm_predict(model, x);
}

const char *svm_type_table[] =
{
  "c_svc","nu_svc","one_class","epsilon_svr","nu_svr",NULL
};

const char *kernel_type_table[]=
{
  "linear","polynomial","rbf","sigmoid","matrix",NULL
};

int svm_save_model(const char *model_file_name, const svm_model *model)
{
  FILE *fp = fopen(model_file_name,"w");
  if(fp==NULL) return -1;

  const svm_parameter& param = model->param;

  fprintf(fp,"svm_type %s\n", svm_type_table[param.svm_type]);
  fprintf(fp,"kernel_type %s\n", kernel_type_table[param.kernel_type]);

  if(param.kernel_type == POLY)
    fprintf(fp,"degree %g\n", param.degree);

  if(param.kernel_type == POLY || param.kernel_type == RBF || param.kernel_type == SIGMOID)
    fprintf(fp,"gamma %g\n", param.gamma);

  if(param.kernel_type == POLY || param.kernel_type == SIGMOID)
    fprintf(fp,"coef0 %g\n", param.coef0);

  int nr_class = model->nr_class;
  int nr_binary=model->nr_binary;
  int l = model->l;
  fprintf(fp, "nr_class %d\n", nr_class);
  fprintf(fp, "nr_binary %d\n", nr_binary);
  fprintf(fp, "total_sv %d\n",l);
	
  {
    fprintf(fp, "rho");
    for(int i=0;i<nr_binary;i++)
      fprintf(fp," %g",model->rho[i]);
    fprintf(fp, "\n");
  }
	
  if(model->label)
  {
    fprintf(fp, "label");
    for(int i=0;i<nr_class;i++)
      fprintf(fp," %d",model->label[i]);
    fprintf(fp, "\n");
  }

  if(model->probA) // regression has probA only
  {
    fprintf(fp, "probA");
    for(int i=0;i<nr_binary;i++)
      fprintf(fp," %g",model->probA[i]);
    fprintf(fp, "\n");
  }
  if(model->probB)
  {
    fprintf(fp, "probB");
    for(int i=0;i<nr_binary;i++)
      fprintf(fp," %g",model->probB[i]);
    fprintf(fp, "\n");
  }

  if(model->nSV)
  {
    fprintf(fp, "nr_sv");
    for(int i=0;i<nr_class;i++)
      fprintf(fp," %d",model->nSV[i]);
    fprintf(fp, "\n");
  }

  if(model->nSV_binary)
  {
    fprintf(fp, "nr_sv_binary");
    for(int i=0;i<nr_binary;i++)
      fprintf(fp," %d",model->nSV_binary[i]);
    fprintf(fp, "\n");
  }
	
  if(model->I)
  {
    fprintf(fp, "I %d\n", param.multiclass_type);
    int **I = model->I;
		
    for(int i=0; i<nr_binary; ++i)
    {
      int nr_Ip = 0, nr_In = 0;
      for(int j=0; j<nr_class; j++)
        if(I[i][j]>0) nr_Ip++;
        else if(I[i][j]<0) nr_In++;
			
      fprintf(fp,"%d", nr_Ip);
      for(int j=0; j<nr_class; j++)
        if(I[i][j]>0) fprintf(fp," %d",j);
      fprintf(fp, "\n");
			
      fprintf(fp,"%d", nr_In);
      for(int j=0; j<nr_class; j++)
        if(I[i][j]<0) fprintf(fp," %d",j);
      fprintf(fp, "\n");
    }
  }

  fprintf(fp, "alpha\n");
  const int * const *sv_ind = model->sv_ind;
  const double * const *sv_coef = model->sv_coef;
  for(int i=0;i<nr_binary;i++)
  {
    for (int j=0;j<model->nSV_binary[i];j++)
      fprintf(fp, "%d ",sv_ind[i][j]);
    fprintf(fp, "\n");
    for (int j=0;j<model->nSV_binary[i];j++)
      fprintf(fp, "%.16g ",sv_coef[i][j]);
    fprintf(fp, "\n");
  }


  fprintf(fp, "SV\n");
  const svm_node * const *SV = model->SV;


  //when kernel_type is MATRX, saves the real index of SV only
  //otherwise, remain the same
  if(param.kernel_type == MATRIX)
  {
    for(int i=0;i<l;i++)
    {
      //for(int j=0;j<nr_class-1;j++)
      // fprintf(fp, "%.16g ",sv_coef[j][i]);

      const svm_node *p = SV[i];
      fprintf(fp,"0:%d\n",(int)((p->value)-1));
    }

  }else
  {
    for(int i=0;i<l;i++)
    {
      const svm_node *p = SV[i];
      while(p->index != -1)
      {
        fprintf(fp,"%d:%.8g ",p->index,p->value);
        p++;
      }
      fprintf(fp, "\n");
    }
  }

  fclose(fp);
  return 0;
}

svm_model *svm_load_model(const char *model_file_name)
{
  FILE *fp = fopen(model_file_name,"rb");
  if(fp==NULL) return NULL;

  int nr_binary=0;

  // read parameters

  svm_model *model = Malloc(svm_model,1);
  svm_parameter& param = model->param;
  model->rho = NULL;
  model->probA = NULL;
  model->probB = NULL;
  model->label = NULL;
  model->nSV = NULL;
  model->nSV_binary = NULL;

  char cmd[81];
  while(1)
  {
    assert(fscanf(fp,"%80s",cmd) != 0);

    if(strcmp(cmd,"svm_type")==0)
    {
      assert(fscanf(fp,"%80s",cmd) != 0);
      int i;
      for(i=0;svm_type_table[i];i++)
      {
        if(strcmp(svm_type_table[i],cmd)==0)
        {
          param.svm_type=i;
          break;
        }
      }
      if(svm_type_table[i] == NULL)
      {
        fprintf(stderr,"unknown svm type.\n");
        free(model->rho);
        free(model->label);
        free(model->nSV);
        free(model);
        return NULL;
      }
    }
    else if(strcmp(cmd,"kernel_type")==0)
    {		
      assert(fscanf(fp,"%80s",cmd) != 0);
      int i;
      for(i=0;kernel_type_table[i];i++)
      {
        if(strcmp(kernel_type_table[i],cmd)==0)
        {
          param.kernel_type=i;
          break;
        }
      }
      if(kernel_type_table[i] == NULL)
      {
        fprintf(stderr,"unknown kernel function.\n");
        free(model->rho);
        free(model->label);
        free(model->nSV);
        free(model);
        return NULL;
      }
    }
    else if(strcmp(cmd,"degree")==0)
      assert(fscanf(fp,"%lf",&param.degree) != 0);
    else if(strcmp(cmd,"gamma")==0)
      assert(fscanf(fp,"%lf",&param.gamma) != 0);
    else if(strcmp(cmd,"coef0")==0)
      assert(fscanf(fp,"%lf",&param.coef0) != 0);
    else if(strcmp(cmd,"nr_class")==0)
      assert(fscanf(fp,"%d",&model->nr_class) != 0);
    else if(strcmp(cmd,"nr_binary")==0)
    {
      assert(fscanf(fp,"%d",&model->nr_binary) != 0);
      nr_binary=model->nr_binary;
    }
    else if(strcmp(cmd,"total_sv")==0)
      assert(fscanf(fp,"%d",&model->l) != 0);
    else if(strcmp(cmd,"rho")==0)
    {
      int n = model->nr_binary;
      model->rho = Malloc(double,n);
      for(int i=0;i<n;i++)
        assert(fscanf(fp,"%lf",&model->rho[i]) != 0);
    }
    else if(strcmp(cmd,"label")==0)
    {
      int n = model->nr_class;
      model->label = Malloc(int,n);
      for(int i=0;i<n;i++)
        assert(fscanf(fp,"%d",&model->label[i]) != 0);
    }
    else if(strcmp(cmd,"probA")==0)
    {
      int n = model->nr_binary;
      model->probA = Malloc(double,n);
      for(int i=0;i<n;i++)
        assert(fscanf(fp,"%lf",&model->probA[i]) != 0);
    }
    else if(strcmp(cmd,"probB")==0)
    {
      int n = model->nr_binary;
      model->probB = Malloc(double,n);
      for(int i=0;i<n;i++)
        assert(fscanf(fp,"%lf",&model->probB[i]) != 0);
    }
    else if(strcmp(cmd,"nr_sv")==0)
    {
      int n = model->nr_class;
      model->nSV = Malloc(int,n);
      for(int i=0;i<n;i++)
        assert(fscanf(fp,"%d",&model->nSV[i]) != 0);
    }
    else if(strcmp(cmd,"nr_sv_binary")==0)
    {
      int n = model->nr_binary;
      model->nSV_binary = Malloc(int,n);
      for(int i=0;i<n;i++)
        assert(fscanf(fp,"%d",&model->nSV_binary[i]) != 0);
    }
    else if(strcmp(cmd,"I") == 0)
    {
      int nr_binary=model->nr_binary, 
        nr_class=model->nr_class,
        **I = NULL, nr_Ip=0, nr_In=0, ind=0;
      I=(int **)calloc(nr_binary, sizeof(int *));
      for(int i=0; i<nr_binary; i++)
        I[i]=(int *)calloc(nr_class, sizeof(int));

      assert(fscanf(fp,"%d", &(param.multiclass_type)) != 0);
      for(int i=0; i<nr_binary; i++)
      {
        assert(fscanf(fp, "%d", &nr_Ip) != 0);
        for(int j=0; j<nr_Ip; j++)
        {
          assert(fscanf(fp, "%d",&ind) != 0);
          I[i][ind]=1;
        }
        assert(fscanf(fp, "%d", &nr_In) != 0);
        for(int j=0; j<nr_In; j++)
        {
          assert(fscanf(fp, "%d",&ind) != 0);
          I[i][ind]= -1;
        }
      }
      model->I=I;
    }
    else if(strcmp(cmd,"alpha")==0)
    {
      while(1)
      {
        int c = getc(fp);
        if(c==EOF || c=='\n') break;	
      }
      break;
    }
    else
    {
      fprintf(stderr,"unknown text in model file\n");
      free(model->rho);
      free(model->label);
      free(model->nSV);
      free(model);
      return NULL;
    }
  }

  // read sv_coef
  model->sv_ind=Malloc(int *, nr_binary);
  model->sv_coef=Malloc(double *, nr_binary);
  for (int i=0;i<nr_binary;i++)
  {
    int bi=model->nSV_binary[i];
    model->sv_ind[i]=Malloc(int,bi);
    for(int j=0;j<bi;j++)
      assert(fscanf(fp,"%d",&model->sv_ind[i][j]) != 0);
    while(1)
    {
      int c = getc(fp);
      if(c==EOF || c=='\n') break;	
    }
    model->sv_coef[i]=Malloc(double,bi);
    for(int j=0;j<bi;j++)
      assert(fscanf(fp,"%lf",&model->sv_coef[i][j]) != 0);
    while(1)
    {
      int c = getc(fp);
      if(c==EOF || c=='\n') break;	
    }
  }

  // read SV
  assert(fscanf(fp,"%80s",cmd) != 0); // sv
  while(1)
  {
    int c = getc(fp);
    if(c==EOF || c=='\n') break;	
  }

  int elements = 0;
  long pos = ftell(fp);

  while(1)
  {
    int c = fgetc(fp);
    switch(c)
    {
    case '\n':
      // count the '-1' element
    case ':':
      ++elements;
      break;
    case EOF:
      goto out;
    default:
      ;
    }
  }
out:
  fseek(fp,pos,SEEK_SET);

  int l = model->l;
  int i;
  model->SV = Malloc(svm_node*,l);
  svm_node *x_space = Malloc(svm_node,elements);

  int j=0;
  for(i=0;i<l;i++)
  {
    model->SV[i] = &x_space[j];
    while(1)
    {
      int c;
      do {
        c = getc(fp);
        if(c=='\n') goto out2;
      } while(isspace(c));
      ungetc(c,fp);
      assert(fscanf(fp,"%d:%lf",&(x_space[j].index),&(x_space[j].value)) != 0);
      ++j;
    }	
  out2:
    x_space[j++].index = -1;
  }

  fclose(fp);

  model->free_sv = 1;	// XXX
  return model;
}

void svm_destroy_model(svm_model* model)
{
  if(model->free_sv)
    free((void *)(model->SV[0]));
  for(int i=0;i<model->nr_binary;i++)
  {
    free(model->sv_coef[i]);
    free(model->sv_ind[i]);
  }
  free(model->SV);
  free(model->sv_coef);
  free(model->sv_ind);
  free(model->rho);
  free(model->label);
  free(model->probA);
  free(model->probB);
  free(model->nSV);
  free(model->nSV_binary);
  free(model);
}

void svm_destroy_param(svm_parameter* param)
{
  free(param->weight_label);
  free(param->weight);
}

const char *svm_check_parameter(const svm_problem *prob, const svm_parameter *param)
{
  // svm_type

  int svm_type = param->svm_type;
  if(svm_type != C_SVC &&
     svm_type != NU_SVC &&
     svm_type != ONE_CLASS &&
     svm_type != EPSILON_SVR &&
     svm_type != NU_SVR)
    return "unknown svm type";
	
  // kernel_type
	
  int kernel_type = param->kernel_type;
  if(kernel_type != LINEAR &&
     kernel_type != POLY &&
     kernel_type != RBF &&
     kernel_type != SIGMOID &&
     kernel_type != MATRIX)
    return "unknown kernel type";

	
  // multiclass_type
	
  int multiclass_type = param->multiclass_type;
  if(multiclass_type != ONE_ONE &&
     multiclass_type != ONE_ALL &&
     multiclass_type != SPARSE &&
     multiclass_type != DENSE)
    return "unknown multiclass type";
	
  // cache_size,eps,C,nu,p,shrinking

  if(param->cache_size <= 0)
    return "cache_size <= 0";

  if(param->eps <= 0)
    return "eps <= 0";

  if(svm_type == C_SVC ||
     svm_type == EPSILON_SVR ||
     svm_type == NU_SVR)
    if(param->C <= 0)
      return "C <= 0";

  if(svm_type == NU_SVC ||
     svm_type == ONE_CLASS ||
     svm_type == NU_SVR)
    if(param->nu < 0 || param->nu > 1)
      return "nu < 0 or nu > 1";

  if(svm_type == EPSILON_SVR)
    if(param->p < 0)
      return "p < 0";

  if(param->shrinking != 0 &&
     param->shrinking != 1)
    return "shrinking != 0 and shrinking != 1";

  if(param->probability != 0 &&
     param->probability != 1)
    return "probability != 0 and probability != 1";

  if(param->probability == 1 &&
     svm_type == ONE_CLASS)
    return "one-class SVM probability output not supported yet";


  // check whether nu-svc is feasible
	
  if(svm_type == NU_SVC)
  {
    int l = prob->l;
    int max_nr_class = 16;
    int nr_class = 0;
    int *label = Malloc(int,max_nr_class);
    int *count = Malloc(int,max_nr_class);

    int i;
    for(i=0;i<l;i++)
    {
      int this_label = (int)prob->y[i];
      int j;
      for(j=0;j<nr_class;j++)
        if(this_label == label[j])
        {
          ++count[j];
          break;
        }
      if(j == nr_class)
      {
        if(nr_class == max_nr_class)
        {
          max_nr_class *= 2;
          label = (int *)realloc(label,max_nr_class*sizeof(int));
          count = (int *)realloc(count,max_nr_class*sizeof(int));
        }
        label[nr_class] = this_label;
        count[nr_class] = 1;
        ++nr_class;
      }
    }
	
    for(i=0;i<nr_class;i++)
    {
      int n1 = count[i];
      for(int j=i+1;j<nr_class;j++)
      {
        int n2 = count[j];
        if(param->nu*(n1+n2)/2 > min(n1,n2))
        {
          free(label);
          free(count);
          return "specified nu is infeasible";
        }
      }
    }
    free(label);
    free(count);
  }

  return NULL;
}

int svm_check_probability_model(const svm_model *model)
{
  return ((model->param.svm_type == C_SVC || model->param.svm_type == NU_SVC) &&
          model->probA!=NULL && model->probB!=NULL) ||
    ((model->param.svm_type == EPSILON_SVR || model->param.svm_type == NU_SVR) &&
     model->probA!=NULL);
}

int model_test(svm_model *model)
{

  printf("nr_binary = %d\n", model->nr_binary);
  for(int i=0; i<model->nr_binary; i++)
  {
    for(int j=0; j<model->nr_class; j++)
    {
      printf("%d ",model->I[i][j]);
    }
    printf("\n");
  }
  return 0;
}

int kernel_type_matrix(svm_model *model)
{
  if(model->param.kernel_type == MATRIX)
    return 1;
  else
    return 0;

}
