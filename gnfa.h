#include <iostream>
#include <list>


struct ms2 {  //member of movment function of gnfa
	int q1, q2;
	std::string s;
};

class gnfa{
public:
	int nq;
	int qs, qf;
	std::list<ms2>* delta;
	ms2 findq(int q1,int q2){  //peyda cardan yek yall khas
		ms2 t;
		t.s="";
		for (std::list<ms2>::iterator it = (*delta).begin(); it != (*delta).end(); it++){
			if (q1==(*it).q1&&q2==(*it).q2){
				t.q1 = (*it).q1; t.q2 = (*it).q2; t.s = (*it).s; break;
			}
		}
		return t;
	}
	gnfa* lgnfa(){   //kahesh dadan yek halat gnfa
		std::list<ms2>* deltat = new std::list<ms2>;
		if(nq==0)
			return this;
		for (int q1=-1;q1<nq-1;q1++){
			for(int q2=0;q2<nq+1;q2++){
				if(q2==nq-1)
					continue;
				ms2 t; t.q1 = q1;
				if(q2!=nq)
					t.q2 = q2;
				else
					t.q2 = nq-1;
				ms2 firstToOmited = findq(q1,nq-1);
				ms2 OmitedToOmited = findq(nq-1,nq-1);
				ms2 OmitedToSecond = findq(nq-1,q2);
				ms2 firstToSecond = findq(q1,q2);

				std::string byOmited ="";
				if(firstToOmited.s!="" && OmitedToSecond.s!=""){
					if(OmitedToOmited.s!=""&&OmitedToOmited.s!="E"){
						if(firstToOmited.s!="E" && OmitedToSecond.s=="E")
							byOmited=firstToOmited.s+"("+OmitedToOmited.s+")*";
						else if(firstToOmited.s=="E" && OmitedToSecond.s!="E")
							byOmited="("+OmitedToOmited.s+")*"+OmitedToSecond.s;
						else if(firstToOmited.s=="E" && OmitedToSecond.s=="E")
							byOmited = "("+OmitedToOmited.s+")*";
						else
							byOmited=firstToOmited.s+"("+OmitedToOmited.s+")*"+OmitedToSecond.s;
					}
					else if(firstToOmited.s!="E" && OmitedToSecond.s=="E")
						byOmited=firstToOmited.s;
					else if(firstToOmited.s=="E" && OmitedToSecond.s!="E")
						byOmited=OmitedToSecond.s;
					else if(firstToOmited.s=="E" && OmitedToSecond.s=="E")
						byOmited = "E";
					else
						byOmited = firstToOmited.s+OmitedToSecond.s;
				}

				if(byOmited!="" && firstToSecond.s!=""){
					t.s="(("+firstToSecond.s+")U("+byOmited+"))";
					(*deltat).push_back(t);
				}
				else if(byOmited=="" && firstToSecond.s!=""){
					t.s=firstToSecond.s;
					(*deltat).push_back(t);
				}
				else if(byOmited!="" && firstToSecond.s==""){
					t.s=byOmited;
					(*deltat).push_back(t);
				}
			}
		}
		gnfa* gn = new gnfa(nq-1, deltat);
		return gn;
	}

	gnfa(int NQ,std::list<ms2> * DELTA){     //constructor
		qf = NQ;
		qs = -1;
		nq = NQ;
		delta = DELTA;
	}

	void printL(){
		std::list<ms2>::iterator it;
		for (it=delta->begin(); it!=delta->end(); ++it){
			std::cout<<"Delta from "<<(*it).q1<<" to "<<(*it).q2<<" with "<<(*it).s<<std::endl;
		}
	}
};
