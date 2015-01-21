/*<one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <2015>  <Amir Hosein>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include "nfa.h"
#include <list>
#include <math.h>

int main()
{

	std::cout<<"Hi, Welcome! ..."<<std::endl;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1100, 630), "NFA To GNFA!",sf::Style::Default, settings);

	sf::RectangleShape box(sf::Vector2f(800,550));
	box.setPosition(25,25);
	box.setFillColor(sf::Color(220,220,220,100));

	sf::Font font;
	if (!font.loadFromFile("PRISTINA.TTF")){
		return 0;
	}

	//phases : 
	bool inQ = true;
	bool inDelta = false;
	bool inDeltaChoosingFirst = false;
	bool inDeltaChoosingSecond = false;
	bool inDeltaValue = false;
	bool inStart = false;
	bool inLast = false;

	sf::Text discr;        //description text
	discr.setFont(font);
	discr.setCharacterSize(30);
	discr.setPosition(25,575);
	discr.setString("Please enter NFA's modes");
	discr.setColor(sf::Color::White);

	sf::VertexArray qCenter(sf::Points,0);         //center of circles
	sf::CircleShape circ(30,500);            //circles
	circ.setOrigin(30,30);
	sf::Text circT;            //text inside circles
	circT.setFont(font);
	circT.setOrigin(20,30);
	circT.setColor(sf::Color::White);

	sf::RectangleShape firstBut(sf::Vector2f(225,60));          //first button
	firstBut.setFillColor(sf::Color(100,150,200));
	firstBut.setPosition(850,25);
	sf::Text firstButText;
	firstButText.setFont(font);
	firstButText.setString("Next");     
	firstButText.setCharacterSize(50);
	firstButText.setPosition(925,20);
	firstButText.setColor(sf::Color::White);

	sf::RectangleShape secBut(sf::Vector2f(225,60));          //second button
	secBut.setFillColor(sf::Color(100,150,200));
	secBut.setPosition(850,95);
	sf::Text secButText;
	secButText.setFont(font);
	secButText.setString("Edit");  
	secButText.setCharacterSize(50);
	secButText.setPosition(925,90);
	secButText.setColor(sf::Color::White);

	sf::RectangleShape thirdBut(sf::Vector2f(225,60));          //third button
	thirdBut.setFillColor(sf::Color(100,150,200));
	thirdBut.setPosition(850,165);
	sf::Text thirdButText;
	thirdButText.setFont(font);
	thirdButText.setString("Submit"); 
	thirdButText.setCharacterSize(50);
	thirdButText.setPosition(900,160);
	thirdButText.setColor(sf::Color::White);

	int firstChosenCircle = -1;        //input delta first mode
	int secondChosenCircle = -1;        //input delta second mode

	sf::CircleShape Inpcircle(10,1000);       //input combobox
	Inpcircle.setFillColor(sf::Color::White);
	sf::CircleShape InpChoscircle(8,1000);       //input combobox
	InpChoscircle.setFillColor(sf::Color::Black);
	InpChoscircle.setOrigin(8,8);
	int choosenInpCircle = -1;      //chosen circle
	sf::Text InpcircleText;
	InpcircleText.setFont(font);
	InpcircleText.setCharacterSize(20);
	InpcircleText.setColor(sf::Color::Black);

	std::list<ms> delta; //delta function

	sf::Text inLineTexts;
	inLineTexts.setFont(font);
	inLineTexts.setCharacterSize(15);
	inLineTexts.setColor(sf::Color::Black);

	int startPoint = -1;

	bool * lastPoints ; 

	sf::CircleShape lastPointCirc(25,2000);
	lastPointCirc.setOutlineThickness(2);
	lastPointCirc.setOutlineColor(sf::Color::Black);
	lastPointCirc.setFillColor(sf::Color::Transparent);
	lastPointCirc.setOrigin(25,25);

	sf::RectangleShape resBut(sf::Vector2f(225,60));          //reset button
	resBut.setFillColor(sf::Color(100,150,200));
	resBut.setPosition(850,515);
	sf::Text resButText;
	resButText.setFont(font);
	resButText.setString("Reset");        
	resButText.setCharacterSize(50);
	resButText.setPosition(910,510);
	resButText.setColor(sf::Color::White);

	std::cout<<"Everything Loaded Successfully ..."<<std::endl;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
			}
			if(event.type == sf::Event::MouseButtonReleased){         //mouse click
				sf::Vector2i mPos = sf::Mouse::getPosition(window);
				if(mPos.x>850 && mPos.x < 1075 && mPos.y>515 && mPos.y<575){   //reset
					inQ = true;
					inDelta = false;
					inDeltaChoosingFirst = false;
					inDeltaChoosingSecond = false;
					inDeltaValue = false;
					inStart = false;
					inLast = false;
					discr.setString("Please enter NFA's modes");
					qCenter.clear();
					firstButText.setString("Next");
					secButText.setString("Edit"); 
					thirdButText.setString("Submit"); 
					firstChosenCircle = -1;
					secondChosenCircle = -1;
					startPoint = -1;
					lastPoints = new bool[0];
					delta.clear();
					std::cout<<"Reset ..."<<std::endl;
				}
				if(inQ){
					if(mPos.x>55 && mPos.x < 795 && mPos.y>55 && mPos.y<545){   //in box
						sf::Vertex vert;
						vert.position = sf::Vector2f(mPos);
						qCenter.append(vert);
						std::cout<<"new mode added succesfully"<<std::endl;
					}
					else if(mPos.x>850 && mPos.x < 1075 && mPos.y>25 && mPos.y<85 && qCenter.getVertexCount()>0){  //next button
						inQ = false;
						inDelta = true;
						discr.setString("Please enter all the connections");
						inDeltaChoosingFirst = true;
						inDeltaChoosingSecond = false;
						inDeltaValue = false;
						firstChosenCircle = -1;
						secondChosenCircle = -1;
					}
				}
				else if(inDelta){
					if(mPos.x>850 && mPos.x < 1075 && mPos.y>25 && mPos.y<85){  //next button
						inDelta = false;
						discr.setString("Please show us the start mode");
						inDeltaChoosingFirst = false;
						inDeltaChoosingSecond = false;
						inDeltaValue = false;
						inStart = true;
						firstChosenCircle = -1;
						secondChosenCircle = -1;
					}
					if(!inDeltaChoosingFirst){
						if(mPos.x>850 && mPos.x < 1075 && mPos.y>95 && mPos.y<155){  //Edit button
							inDelta = true;
							discr.setString("Please enter all the connections");
							inDeltaChoosingFirst = true;
							inDeltaChoosingSecond = false;
							inDeltaValue = false;
							firstChosenCircle = -1;
							secondChosenCircle = -1;
						}
					}
					if(inDeltaChoosingFirst){
						if(mPos.x>55 && mPos.x < 795 && mPos.y>55 && mPos.y<545){   //in box
							for(int i=qCenter.getVertexCount()-1;i>=0;i--){
								if(((mPos.x-qCenter[i].position.x)*(mPos.x-qCenter[i].position.x) +
									(mPos.y-qCenter[i].position.y)*(mPos.y-qCenter[i].position.y)) <900){     //in i'th circle
										firstChosenCircle = i;
										inDeltaChoosingFirst = false;
										discr.setString("Please enter all the connections - the other side");
										inDeltaChoosingSecond = true;
										break;
								}
							}
						}
					}
					else if(inDeltaChoosingSecond){
						if(mPos.x>55 && mPos.x < 795 && mPos.y>55 && mPos.y<545){   //in box
							for(int i=qCenter.getVertexCount()-1;i>=0;i--){
								if(((mPos.x-qCenter[i].position.x)*(mPos.x-qCenter[i].position.x) +
									(mPos.y-qCenter[i].position.y)*(mPos.y-qCenter[i].position.y)) <900){     //in i'th circle
										secondChosenCircle = i;
										inDeltaChoosingSecond = false;
										discr.setString("Please enter all the connections - the value");
										inDeltaValue = true;
										break;
								}
							}
						}
					}
					else if(inDeltaValue){
						if(mPos.x>850 && mPos.x < 1075 && mPos.y>165 && mPos.y<225){  //Submit button
							ms newMs;
							newMs.q1 = firstChosenCircle;
							newMs.q2 = secondChosenCircle;
							newMs.s = choosenInpCircle;
							bool isAlreadyExisted = false;
							std::list<ms>::iterator it;
							for (it=delta.begin(); it!=delta.end(); ++it){
								if((*it).q1==newMs.q1 && (*it).q2==newMs.q2&&(*it).s==newMs.s){
									std::cout<<"Input Not allowed : already existed ..." <<std::endl;
									isAlreadyExisted = true;
								}
							}
							if(!isAlreadyExisted){
								delta.push_back(newMs);
								std::cout<<"new arrow added from "<<newMs.q1<<" to "<<newMs.q2<<" with "<<newMs.s<<" delta size : "<<delta.size()<<std::endl;
							}
							discr.setString("Please enter all the connections");
							firstChosenCircle = -1;
							secondChosenCircle = -1;
							inDeltaValue = false;
							inDeltaChoosingFirst = true;
						}
						else if(mPos.x>850 && mPos.x < 870 && mPos.y>240 && mPos.y<260){   //E
							choosenInpCircle = -1;
						}
						else if(mPos.x>900 && mPos.x < 920 && mPos.y>240 && mPos.y<260){   //0
							choosenInpCircle = 0;
						}
						else if(mPos.x>950 && mPos.x < 970 && mPos.y>240 && mPos.y<260){   //1
							choosenInpCircle = 1;
						}				
					}
				}
				else if(inStart){
					if(mPos.x>55 && mPos.x < 795 && mPos.y>55 && mPos.y<545){   //in box
						for(int i=qCenter.getVertexCount()-1;i>=0;i--){
							if(((mPos.x-qCenter[i].position.x)*(mPos.x-qCenter[i].position.x) +
								(mPos.y-qCenter[i].position.y)*(mPos.y-qCenter[i].position.y)) <900){     //in i'th circle
									startPoint = i;
							}

						}
					}
					else if(mPos.x>850 && mPos.x < 1075 && mPos.y>25 && mPos.y<85 && startPoint!=-1){  //next button
						inStart = false;
						discr.setString("Please show us last modes");
						inLast = true;
						lastPoints = new bool[qCenter.getVertexCount()];
						for(int i=0;i<qCenter.getVertexCount();i++)
							lastPoints[i]=false;
						firstButText.setString("Calculate");
						firstButText.setPosition(900,20);
					}
				}
				else if(inLast){
					if(mPos.x>55 && mPos.x < 795 && mPos.y>55 && mPos.y<545){   //in box
						for(int i=qCenter.getVertexCount()-1;i>=0;i--){
							if(((mPos.x-qCenter[i].position.x)*(mPos.x-qCenter[i].position.x) +
								(mPos.y-qCenter[i].position.y)*(mPos.y-qCenter[i].position.y)) <900){     //in i'th circle
									if(lastPoints[i])
										lastPoints[i]=false;
									else
										lastPoints[i] = true;
							}

						}
					}
					else if(mPos.x>850 && mPos.x < 1075 && mPos.y>25 && mPos.y<85 && startPoint!=-1){  //calc button
						int n = qCenter.getVertexCount();
						nfa NFA(n,&delta,startPoint,lastPoints);
						gnfa * GNFA = NFA.ntg();
						std::cout<<"GNFA Successfully Built : "<<std::endl;
						std::cout<<"nq = "<<GNFA->nq<<std::endl;
						GNFA->printL();
						std::cout<<"Starting Proccess"<<std::endl;
						while(GNFA->nq>0){
							GNFA = GNFA->lgnfa();
						}
						GNFA->printL();
					}
				}
			}
        }

		window.clear(sf::Color(200,150,150));
		window.draw(box);
		std::list<ms>::iterator it;
		for (it=delta.begin(); it!=delta.end(); ++it){
			if((*it).q1 != (*it).q2){
				sf::Vertex line[] = {
					sf::Vertex(qCenter[(*it).q1].position),
					sf::Vertex(qCenter[(*it).q2].position,sf::Color::Black)
				};

				window.draw(line, 2, sf::Lines);

				char num[20];
				sprintf(num,"%d%c",(*it).s,0);
				inLineTexts.setString(num);
				inLineTexts.setPosition((line[0].position.x+line[1].position.x)/2,(line[0].position.y+line[1].position.y)/2);

				window.draw(inLineTexts);
			}
			else{
				char num[20];
				sprintf(num,"%d%c",(*it).s,0);
				inLineTexts.setString(num);
				inLineTexts.setPosition(qCenter[(*it).q1].position.x+20,qCenter[(*it).q1].position.y+20);

				window.draw(inLineTexts);
			}
		}
		for(int i=0;i<qCenter.getVertexCount();i++){
			circ.setPosition(qCenter[i].position);
			if(i==firstChosenCircle && i ==secondChosenCircle)
				circ.setFillColor(sf::Color(100,100,0,200));
			else if(i==firstChosenCircle)
				circ.setFillColor(sf::Color(100,0,0,200));
			else if(i==secondChosenCircle)
				circ.setFillColor(sf::Color(0,100,0,200));
			else if(i==startPoint)
				circ.setFillColor(sf::Color(100,0,100,200));
			else
				circ.setFillColor(sf::Color(0,0,100,200));
			window.draw(circ);

			if(inLast)
				if(lastPoints[i]){
					lastPointCirc.setPosition(qCenter[i].position);
					window.draw(lastPointCirc);
				}

			char num[20];
			sprintf(num,"q%d%c",i,0);
			circT.setString(num);
			circT.setPosition(qCenter[i].position);
			window.draw(circT);
		}
		
		window.draw(firstBut);
		window.draw(firstButText);

		if(inDelta && !inDeltaChoosingFirst){
			window.draw(secBut);
			window.draw(secButText);
		}

		if(inDeltaValue){
			Inpcircle.setPosition(850 , 240);
			window.draw(Inpcircle);
			Inpcircle.setPosition(900 , 240);
			window.draw(Inpcircle);
			Inpcircle.setPosition(950 , 240);
			window.draw(Inpcircle);

			InpcircleText.setString("E");
			InpcircleText.setPosition(875,250);
			window.draw(InpcircleText);
			InpcircleText.setString("0");
			InpcircleText.setPosition(925,250);
			window.draw(InpcircleText);
			InpcircleText.setString("1");
			InpcircleText.setPosition(975,250);
			window.draw(InpcircleText);

			if(choosenInpCircle == -1)
				InpChoscircle.setPosition(860,250);
			if(choosenInpCircle == 0)
				InpChoscircle.setPosition(910,250);
			if(choosenInpCircle == 1)
				InpChoscircle.setPosition(960,250);
			window.draw(InpChoscircle);

			window.draw(thirdBut);
			window.draw(thirdButText);
		}
		window.draw(resBut);
		window.draw(resButText);
		window.draw(discr);
        window.display();
    }

    return 0;
}
