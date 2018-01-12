#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <queue>
#include <string>
#include <sstream>

#include "bayesiannetwork.h"


namespace baysian{

struct data_compare 
{  
  bool operator()(const data<int> l, const data<int> r)  {  return l.key > r.key;  }  
};

void bayesiannetwork::printarray(double **array,int len1,int len2)
{
	
	for (int i = 0; i <len1; i++)
	{
		for (int j = 0; j < len2; j++)
			std::cout << array[i][j] << " ";
	}
	std::cout << std::endl;
}

//initialize all the information we need from training data
bayesiannetwork::bayesiannetwork(char* train_file, char* test_file, char* cfg_file)
{
	std::cout<<"Run Baysiannetwork"<<std::endl;
	std::ifstream configure;
	configure.open(cfg_file);
        if(!configure){std::cout<<"! Can't open configuration file!"<<std::endl;return;}
    
	configure>>traininstances>>testinstances>>attributes;// read the number of training instances and attributes

	std::cout << "the number of training instances is " << traininstances << std::endl;
	std::cout << "the number of testing instances is " << testinstances << std::endl;
	std::cout << "the attributes is " << attributes << std::endl;


	// TODO : implement handling continuous data. This is just a placeholder
	int *discrete = new int[attributes]; 
	//this array store the information about each attribute is continuous or not
	for(int z=0; z<attributes ; z++)     //  read the information about continuous or not
		configure>>discrete[z];

	int *numclass= new int[attributes+1]; 
	//this array store the number of classes of each attribute
	for(int v=0; v<=attributes; v++)   // read the number of classes
		configure>>numclass[v];
	std::cout << "the total number of each decision's class is " << numclass[attributes] << std::endl;

	double *count = new double[numclass[attributes]]; 
	//this array store the total number of each decision's class in training data
	for(int c=0; c<numclass[attributes]; c++)
		count[c]=0;


	configure.close();
	int combinations=1;
	for(int com=(attributes-1) ; com>1 ; com--)
		combinations+=com;  


	//std::cout<<"combinations is "<< combinations << std::endl<<std::endl; 

	std::ifstream trainingDataFile;
	std::string Buf;
        trainingDataFile.open(train_file);
        if(!trainingDataFile){std::cout<<"Can't open training data file!"<<std::endl;return;}

	int **rank= new int *[combinations];
	for(int zz=0 ; zz< combinations ; zz++)   
		rank[zz]= new int[2];
	int index=0;
	//std::cout << "initial rank[] is" << std::endl;
	for(int zzz=0 ; zzz< (attributes-1) ; zzz++)
	{
		for(int ppp=1 ; ppp<=(attributes-zzz-1) ; ppp++)
		{
			rank[index][0]=zzz ;
			rank[index][1]=(zzz+ppp) ;
			//std::cout << "rank [" << index << "]" << rank[index][0] << " " << rank[index][1] << " ";
			index++;
			
		}
		//std::cout << std::endl;
	}


	double **ccc= new double *[ attributes*numclass[attributes] ];
	for(int f=0 ; f< attributes ; f++)
	{
		for (int f1 = (f*numclass[attributes]); f1 < ((f + 1)*numclass[attributes]); f1++)
		{
			ccc[f1] = new double[numclass[f]];
			//std::cout << f1 << " " << numclass[f] << " ";
		}
	}
	for(int f2=0 ; f2< attributes ; f2++)
	{
		for(int f3=(f2*numclass[attributes]) ; f3 < ( (f2+1)*numclass[attributes] ) ; f3++)
		{
			for (int f4 = 0; f4 < numclass[f2]; f4++)
			{
				ccc[f3][f4] = 0;
				//f3��ʾ��f3��ccc���飬һ��numclass[attributes]*attributes�����飬f4��ʾ��f3�������ж�Ӧ��final class��attribute��class
				//std::cout << f3 << " " << f4 << " ";
			}
		}
	}


	double **aaa= new double *[ combinations*numclass[attributes] ];
	for(int ff=0 ; ff< combinations ; ff++) 
	{
		for(int ff1=(ff*numclass[attributes]) ; ff1 < ( (ff+1)*numclass[attributes] ) ; ff1++)
		{
			aaa[ff1]= new double[ numclass[ rank[ff][0] ]*numclass[ rank[ff][1] ] ];

			for(int ff2=0 ; ff2 < numclass[ rank[ff][0] ]*numclass[ rank[ff][1] ] ; ff2++)
				aaa[ff1][ff2]=0;
		}
	}
	


	double **bbb= new double *[combinations*numclass[attributes] ];
	for(int fff=0 ; fff< combinations ; fff++)   
	{
		for(int fff1=(fff*numclass[attributes]) ; fff1 < ( (fff+1)*numclass[attributes] ) ; fff1++)
		{
			bbb[fff1]= new double[ numclass[ rank[fff][0] ]*numclass[ rank[fff][1] ] ];

			for(int fff2=0 ; fff2<numclass[ rank[fff][0] ]*numclass[ rank[fff][1] ] ; fff2++)
				bbb[fff1][fff2]=0;
		}
	}


	int *oneLine = new int[attributes+1];

	for( int ii=1 ; ii<=traininstances; ii++)
	{
		
		getline(trainingDataFile, Buf);
		std::stringstream  lineStream(Buf);

		for (int bb = 0; bb <= attributes; bb++){
			getline(lineStream, Buf, ',');
			oneLine[bb] = stod(Buf);
		}
		for (int h=0 ; h< attributes ; h++)
			ccc[h*numclass[attributes]+oneLine[attributes]-1][oneLine[h]-1 ]++;

		for(int hh=0 ; hh < combinations ; hh++)
		{
			aaa[ hh*numclass[attributes]+oneLine[attributes]-1 ]
		    [ (oneLine[ rank[hh][0] ]-1)*numclass[rank[hh][1]] + oneLine[ rank[hh][1] ]-1 ]++;

			bbb[ hh*numclass[attributes]+oneLine[attributes]-1 ]
			[ (oneLine[ rank[hh][0] ]-1)*numclass[rank[hh][1]] + oneLine[ rank[hh][1] ]-1 ]++;
		}
		
	
		count[oneLine[attributes] -1 ] ++;
	}
	//std::cout << "ɾ����ֵ" << std :: endl;
	delete [] oneLine;
        trainingDataFile.close();

	
		
	for( int t=0 ; t< attributes ; t++)
	{	
		for ( int d=0 ; d<numclass[attributes] ; d++)
		{	
			int correction=0;

			for (int o=0 ; o < numclass[t] ; o++)
			{	
				//�����t��������finalΪd������£�ֵΪo���ܸ���Ϊ�㣬�ͽ������Ե���������ֵ�ĺͼ�1��
				if (ccc[(t*numclass[attributes]+d)][o]==0 )
				{
					correction=numclass[t];
					for(int p=0 ; p <numclass[t] ; p++)
					{
						ccc[(t*numclass[attributes]+d)][p]++;
					}
					break;
				}
			}
			//��finalΪd������£�����ÿ�����Ե�����ֵ����ռrecord��finalΪd���ı���,P(ai=x|Ci)
			for ( int w=0 ; w<numclass[t] ; w++)
				ccc[(t*numclass[attributes]+d)][w]/=(count[d]+correction);
		}		
	}




	for( int tt=0 ; tt< combinations ; tt++)
	{
		int correction1=0;

		for ( int dd=0 ; dd< numclass[attributes] ; dd++)
		{	
			for (int oo=0 ; oo < numclass[ rank[tt][0] ]*numclass[ rank[tt][1] ]  ; oo++)
			{
				//�����finalΪddʱ��combinationsΪttʱ��attribute[i]��attributes[j]֮���ĳ�����Ӧ��ֵΪ0
				//�ͽ������ϵ�е����ж�Ӧֵ��һ
				if (aaa[ tt*numclass[attributes]+dd ][oo]==0 )
				{
					for(int pp=0 ; pp < numclass[attributes] ; pp++)
					{
						for(int ppp=0 ; ppp < numclass[ rank[tt][0] ]*numclass[ rank[tt][1] ]  ; ppp++)
						{
							aaa[tt*numclass[attributes]+pp][ppp]++;
						}
					}

					correction1= numclass[attributes]*numclass[ rank[tt][0] ]*numclass[ rank[tt][1] ];

					break;
				}
			}          		
		}
        
		for(int w1=0 ; w1 < numclass[attributes] ; w1++)
		{
			for(int ww1=0 ; ww1 < numclass[ rank[tt][0] ]*numclass[ rank[tt][1] ]  ; ww1++)
			{
				//��finalΪw1������£�����ÿ������֮�����ظ�ֵռrecord�ı�����P(ai=x and aj=y and Ci)
				aaa[tt*numclass[attributes]+w1][ww1]/=( traininstances+correction1 );
			}
		}
	
	}

	



	for( int ttt=0 ; ttt< combinations ; ttt++)
	{
		for ( int ddd=0 ; ddd< numclass[attributes] ; ddd++)
		{	

			int correction2=0;

			for (int ooo=0 ; ooo < numclass[ rank[ttt][0] ]*numclass[ rank[ttt][1] ]  ; ooo++)
			{
				if (bbb[ ttt*numclass[attributes]+ddd ][ooo]==0 )
				{
					for(int ppp=0 ; ppp < numclass[ rank[ttt][0] ]*numclass[ rank[ttt][1] ]  ; ppp++)
					{
						bbb[ttt*numclass[attributes]+ddd][ppp]++;
					}

					correction2= numclass[ rank[ttt][0] ]*numclass[ rank[ttt][1] ];

					break;
				}
			} 

			for(int w2=0 ; w2 < numclass[ rank[ttt][0] ]*numclass[ rank[ttt][1] ]  ; w2++ )
				//��finalΪw1������£�����ÿ������֮�����ظ�ֵռrecord��finalΪd���ı�����P(ai=x and aj=y | Ci)
				bbb[ttt*numclass[attributes]+ddd][w2]/=( count[ddd]+correction2 );

		}
	}

	//printarray(bbb, 72, 100);
	//�������룬�����ȫ��0.0039**
	

	double *relation = new double[combinations];

	for( int ss=0 ; ss < combinations ; ss++)
	{
		double tempo=0;

		for( int s1=0 ; s1< numclass[attributes] ; s1++)
		{
			for( int s2=0 ; s2< numclass[ rank[ss][0] ] ; s2++)
			{
				for( int s3=0 ; s3< numclass[ rank[ss][1] ] ; s3++)
				{
					tempo+= 
					( 
						aaa[ ss*numclass[attributes]+s1 ][ s2*numclass[ rank[ss][1] ]+s3 ] * 
						log10
						( 
							bbb[ ss*numclass[attributes]+s1 ][ s2*numclass[ rank[ss][1] ]+s3 ] / 
							( ccc[ rank[ss][0]*numclass[attributes]+s1 ]
							[s2]*ccc[ rank[ss][1]*numclass[attributes]+s1 ][s3] ) 
						)   
					);
				}
			}
		}		
		relation[ss]=tempo;
	}

	/*for (int i = 0; i < combinations; i++)
	{
		std::cout << relation[i] << " ";
	}*/
		
	//printarray(bbb, 72, 100);
	//�������룬�����ȫ��0.0039**

	std::priority_queue< data<int>, std::vector< data<int> >, data_compare  >  maxweight;
	data<int> elen;

	for( int cast=0 ; cast< combinations ; cast++)
	{
          elen.value1 = rank[cast][0];
		  elen.value2 = rank[cast][1];
          elen.key = relation[cast] ;
	  maxweight.push(elen);
	}

	
	int *groups= new int[attributes];
	for(int vv=0; vv < attributes; vv++)   
		groups[vv]=0;

	int **graph= new int *[attributes];
	for(int zz1=0 ; zz1 < attributes ; zz1++)   
		graph[zz1]= new int[attributes];

	for(int k1=0; k1<attributes ; k1++)
	{
		for(int kk1=0 ; kk1<attributes  ; kk1++)	
			graph[k1][kk1]=0;
	}

	data<int> mmm;
	int base = 1;

	for(int combi=0 ; combi < combinations ; combi ++ )
	{
		if( !maxweight.empty() ){
			mmm = maxweight.top();
			maxweight.pop();
		}

		//�������mmm.value1������mmm.value2֮��ֵ����Ҷ���Ϊ��
		if( groups[mmm.value1] != 0 && groups[mmm.value2] != 0 && groups[mmm.value1] == groups[mmm.value2] )
		{}
		//�������mmm.value1������mmm.value2֮��ֵ���������mmm.value1Ϊ��
		else if( groups[mmm.value1] == 0 && groups[mmm.value1] == groups[mmm.value2] )
		{
			groups[mmm.value1]=base;
			groups[mmm.value2]=base;
			base++;

			graph[mmm.value1][mmm.value2]=1;
			graph[mmm.value2][mmm.value1]=1;
		}
		//�������mmm.value2��Ϊ��������mmm.value1Ϊ��
		else if( groups[mmm.value1] == 0 && groups[mmm.value2] != 0 )
		{
			groups[mmm.value1] = groups[mmm.value2];

			graph[mmm.value1][mmm.value2]=1;
			graph[mmm.value2][mmm.value1]=1;			
		}
		//�������mmm.value1��Ϊ��������mmm.value2Ϊ��
		else if( groups[mmm.value1] != 0 && groups[mmm.value2] == 0 )
		{
			groups[mmm.value2] = groups[mmm.value1];

			graph[mmm.value1][mmm.value2]=1;
			graph[mmm.value2][mmm.value1]=1;
		}
		else if( groups[mmm.value1] != 0 && groups[mmm.value2] != 0 && groups[mmm.value1] !=groups[mmm.value2] )
		{
			int boss,slave;

			if( groups[mmm.value1] <  groups[mmm.value2] )
			{
				boss=groups[mmm.value1];
				slave=groups[mmm.value2];
			}
			else
			{
				boss=groups[mmm.value2];
				slave=groups[mmm.value1];
			}

			for( int scan=0 ; scan < attributes ; scan++)
			{
				if ( groups[scan]==slave )
				{
					groups[scan]=boss;
				}
			}

			graph[mmm.value1][mmm.value2]=1;
			graph[mmm.value2][mmm.value1]=1;
		}
	}

	for( int atest=0 ; atest< attributes ; atest++)
	{
		for( int atest1=0 ; atest1< attributes ; atest1++)
			std::cout<<graph[atest][atest1]<<" " ;

		std::cout<<std::endl;
	}
	std::cout<<std::endl;



	int *transfer= new int[attributes];
	for(int vvv=0; vvv<attributes; vvv++)   
		transfer[vvv]=attributes;

	transfer[0]=0; 

	for(int redo=0 ; redo<attributes ; redo++)
	{
		int min=(attributes+1);
		int point=0;

		for(int redo1=0 ; redo1<attributes ; redo1++)
		{
			if( min > transfer[redo1] )
			{
				min=transfer[redo1];
				point=redo1;
			}
		}
		//minΪtransfer�е���Сֵ��pointΪtransfer�е���Сֵ�����������
		for(int redo2=0 ; redo2<attributes ; redo2++)
		{
			if( graph[point][redo2]==1)
			{
				graph[redo2][point]=0;
				transfer[redo2]=(min+1);
			}
		}
		
		transfer[point]=(attributes+1);
	}


	std::cout<<std::endl;
	for( int test=0 ; test< attributes ; test++)
	{
		for( int test1=0 ; test1< attributes ; test1++)
			std::cout<<graph[test][test1]<<" " ;

		std::cout<<std::endl;
	}
	std::cout<<std::endl;


//------------------------------------------------

	int **parent= new int *[attributes];
	//this 2-dimension array store each node's parents
	for(int z=0 ; z< attributes ; z++)   
		parent[z]= new int[attributes+1];

	for(int k=0; k<attributes ; k++)
	{
		for(int kk=0 ; kk<=attributes   ; kk++)	
			parent[k][kk]=0;
	}

	//read the information about everyone's parents
	for(int e=0; e<attributes ; e++)
	{
		int pama=1;
		int pamaindex=1;

		for(int ee=0 ; ee<attributes ; ee++)
		{		
			if( graph[ee][e]==1 )
			{
				pama++;
				parent[e][pamaindex]=ee;
				pamaindex++;
			}

		}

		parent[e][0]=pama;
		parent[e][pamaindex]=attributes;
	}

	for (int test = 0; test< attributes; test++)
	{
		for (int test1 = 0; test1<= attributes; test1++)
			std::cout << parent[test][test1] << " ";

		std::cout << std::endl;
	}
	std::cout << std::endl;
//-------------------------------------------------

	

	//cpt is a three dimention array
	//the first dimention is the attributes
	//the last two dimention is the "conditional probability table"
	// for each attribute
	long double ***cpt = new long double**[attributes]; 
	for(int j=0; j<attributes; j++)
	{

		cpt[j]=new long double*[ numclass[j] ];

		//calculate the appropriate length of the third dimention
		int reg=1;
		for(int jjj=1 ; jjj<=parent[j][0] ;jjj++) 
			reg*=numclass[ parent[j][jjj] ];


		for(int jj=0 ; jj<numclass[j] ;jj++)
		{
			cpt[j][jj]=new long double[reg+1];

			cpt[j][jj][0]=reg;       

			for(int jjjj=1 ; jjjj<=reg ; jjjj++) //initialize to zero
				cpt[j][jj][jjjj]=0;	
		}

	}


        trainingDataFile.open(train_file);
        if(!trainingDataFile){std::cout<<"Can't open training data file!"<<std::endl;return;}
	
	double *oneLine_double = new double[attributes+1];

	//store the counts of each possible conjunction into cpt
	for( int i=1 ; i<=traininstances; i++)
	{
		getline( trainingDataFile, Buf );
		std::stringstream  lineStream(Buf);

		for (int y=0 ; y<=attributes ; y++){
			getline( lineStream, Buf , ',' );
			oneLine_double[y]=stod(Buf);
		}

		for (int yy=0 ; yy<attributes ;yy++)
		{
			int reg_add=1;
			int reg_mul=1;

			for( int yyy=1 ; yyy<=parent[yy][0] ; yyy++)
			{
				reg_add+=(   reg_mul * (static_cast<int>(oneLine_double[ parent[yy][yyy] ]) -1) );
				reg_mul*=numclass[ parent[yy][yyy] ];
			}
			
			cpt[yy][(static_cast<int>(oneLine_double[yy])-1)][reg_add]++;
		}
	}


	delete [] oneLine_double;
        trainingDataFile.close();

    //processing the information in the protalbe to get the proabability of each conjunction
	for( int t1=0 ; t1< attributes ; t1++)
	{
		for ( int d=1 ; d<=cpt[t1][0][0] ; d++)
		{	
			for (int o=0 ; o < numclass[t1] ; o++)
			{
				//this loop judge weather there is zero occurence of some conjuction
				//if it dose, then do Laplacian correction
				if (cpt[t1][o][d]==0 )
				{
					for(int p=0 ; p <numclass[t1] ; p++)
					{
						cpt[t1][p][d]++;
					}
					break;
				}
			}

			int sum=0;

			for ( int w=0 ; w<numclass[t1] ; w++)
				sum+=cpt[t1][w][d];

			//claculate every conjuction's contribution of probability 
			for ( int ww=0 ; ww<numclass[t1] ; ww++)
				cpt[t1][ww][d]/=sum;
		}
	
	}

	//calculate the probability of each resulting class
	for ( int ppp=0 ; ppp<numclass[attributes] ; ppp++)
	{
		count[ppp]=count[ppp]/traininstances;
//		std::cout<<count[ppp]<<" ";
	}
//		std::cout<<std::endl;


	classifier(cpt , numclass ,  count , parent, test_file);
	//call function for classification


	//release the memory
	for( int x=0; x<attributes ; x++)
	{
		for( int xx=0 ; xx< numclass[x] ; xx++)
			delete [] cpt[x][xx];

		delete [] cpt[x];
	}
	
	for( int pa=0 ; pa< attributes ; pa++)
		delete [] parent[pa];

	delete [] parent;
	delete [] cpt;
	delete [] numclass;
	delete [] discrete;
	delete [] count;
}


//calculate the probability of each choice and choose the greatest one as our prediction
void bayesiannetwork::classifier(long double ***cpt ,int *numclass ,double *count ,int **parent,char* test_file)
{
	std::ifstream testInputFile(test_file);
	if(!testInputFile){std::cout<<"Can't open test data file!"<<std::endl;return;}
	std::string Buf;

	int *result= new int[testinstances]; //this array store the real result for comparison
	for(int w=0; w<testinstances; w++)
	{
		result[w]=0;
	}

	int *outcome=new int[testinstances]; //this array store our prediciton
	for(int f=0; f<testinstances; f++)
	{
		outcome[f]=0;
	}

	double *oneLine=new double [attributes+1];   //store each instance for processing

	long double *decision=new long double[numclass[attributes]]; 
	// store the probability of each choice

	for( int a=0 ; a<testinstances ; a++)
	{
		getline( testInputFile , Buf );
		std::stringstream  lineStream(Buf);
		//set the array's entries as 1 for each testing instance
		for ( int m=0 ; m<numclass[attributes]; m++)
		decision[m]=1;

		// read one instance for prediction
		for (int u=0 ; u<=attributes; u++){
			getline( lineStream, Buf , ',' );
			oneLine[u]=stod(Buf);
		}

		result[a]=oneLine[attributes];
		// store the result

		//calculate each choice's probability
		for( int x=0 ; x<numclass[attributes] ; x++)
		{
			for( int xx=0 ; xx<attributes ; xx++)
			{

				int reg_add=1;  //objective's position of the third dimention array
				int reg_mul=1;  //for calculating reg_add

				// the address of our objective is depend on this attribute's parent
				for( int xxx=1 ; xxx<parent[xx][0] ; xxx++)
				{
					reg_add+=(   reg_mul * (static_cast<int>(oneLine[ parent[xx][xxx] ]) -1) );
					reg_mul*=numclass[ parent[xx][xxx] ];					
				}
				reg_add+=(reg_mul*x);

				decision[x]*=cpt[xx][static_cast<int>(oneLine[xx])-1][reg_add];
			}					
			decision[x]*=count[x];
		}

	    //decide which choice has the highest probability
		int big=0;                                         
		long double hug=decision[0];
		for ( int v=1 ; v<numclass[attributes] ; v++)
		{
			if ( decision[v]>hug)
			{
				big=v;
				hug=decision[v];
			}
		}
		outcome[a]=(big+1);
	}
	accuracy ( outcome , result );
	//call function "accuracy" to calculate the accuracy 

	//release memory
	delete [] result;
	delete [] decision;
	delete [] oneLine;
	delete [] outcome;
}

}// end of namespace bayesian
