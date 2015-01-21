#include <iostream>
#include <list>
#include "gnfa.h"

struct ms {   //member of movment function of nfa
	int q1, q2, s;
};

class nfa {
public:
	int nq; //number of q
	std::list<ms>* delta; //delta function
	int q0;
	bool *f;
	nfa(int NQ, std::list<ms>* DELTA, int Q0, bool *F){
		nq = NQ;
		delta = DELTA;
		q0 = Q0;
		f = F;
	}
	
	gnfa* ntg(){   //nfa to gnfa
		std::list<ms2> *deltat=new std::list<ms2>;
		for (std::list<ms>::iterator it = (*delta).begin(); it != (*delta).end(); it++){
			ms2 t; t.q1 = (*it).q1; t.q2 = (*it).q2;
			if ((*it).s == -1){
				t.s = "E";
			}
			else if ((*it).s == 0){
				t.s = "0";
			}
			else {
				t.s = "1";
			}
			bool found = false;
			for(std::list<ms2>::iterator it = (*deltat).begin(); it !=(*deltat).end();it++){
				if((*it).q1 == t.q1 && (*it).q2 == t.q2){
					(*it).s = "(("+(*it).s+")U("+ t.s +"))";
				}
			}
			if(!found)
				(*deltat).push_back(t);
		}
		for (int i = 0; i < nq; i++){
			if (f[i]){
				ms2 t2; t2.q1 = i; t2.q2 = nq; t2.s = "E";
				(*deltat).push_back(t2);
			}
		}
		ms2 t3; t3.q1 = -1; t3.q2 = q0; t3.s = "E";
		(*deltat).push_back(t3);
		gnfa* gn = new gnfa(nq, deltat);
		return gn;
	 }
	
};
