#include "bayesian.h"
namespace baysian{

class naivebayesian : public bayesian
{
protected:
	void classifier(long double**  , int* , double* , int *, char*);
	//calculate the probability of each choice and choose the greatest one as our prediction
public:
	naivebayesian(char*, char*, char*); 
	//initialize all the information we need from training data
};

}// end of namespace bayesian
