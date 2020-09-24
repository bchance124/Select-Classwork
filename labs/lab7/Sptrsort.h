
#include <vector>
 using namespace std;


template <typename T>
class sptr {
	public:    
		sptr(T *_ptr=NULL) { ptr = _ptr; }    
		bool operator< (const sptr &rhs) const {
	    	return *ptr < *rhs.ptr;    
		}    
		operator T * () const { return ptr; }  
		
		private:    
			T *ptr;
};

template <typename T>
void sptrsort(std::vector<T *> &A) {
  //USE std::sort on SPTR VERSION OF A
  //NOTE: A is vector of pointers (T *)

	vector < sptr<T> > Ap;
	
	Ap.resize(A.size());
	
	for (int i = 0; i < A.size(); i++) 
		Ap[i] = A[i];
	
	sort(Ap.begin(), Ap.end());

	for (int i = 0; i < A.size(); i++) 
		A[i] = Ap[i];

	return;
}
