#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){


	oldMousePos = ofPoint(0, 0);

	objhitidx = -1;
	grouphitidx = -1;
	margin = 20;
	snap = 20;
	depthIdx = 0;

	processGroups();

}


bool testApp::rectInside(const ofRectangle A, const ofRectangle B)
{

	if ((A.y+A.height) < B.y)		return FALSE;
	if (A.y > (B.y+B.height))		return FALSE;
	if ((A.x+A.width) < B.x)		return FALSE;
	if (A.x > (B.x+B.width))		return FALSE;
	
	return TRUE;

}

void testApp::processGroups() {
		groups.clear();

	// process groups

	// create initial set of groups;

	for (int i=0; i<objects.size(); i++) {

		group g;
		g.active = false;
		g.rect = objects[i].rect;
		
		// add margin
		g.rect.x -= margin;
		g.rect.y -= margin;
		g.rect.height += margin*2;
		g.rect.width += margin*2;

		groups.push_back(g);
	}

	bool finished = FALSE;


	while (!finished) {

		int hits = 0;

		for (int i=0; i<groups.size(); i++) {
			for (int j=i+1; j<groups.size(); j++) {

				if (rectInside(groups[i].rect,groups[j].rect)) {
					if ((i != grouphitidx) && j!=grouphitidx) {
						hits++;
						int Ax0 = groups[i].rect.x;
						int Ay0 = groups[i].rect.y;
						int Ax1 = Ax0+groups[i].rect.width;
						int Ay1 = Ay0+groups[i].rect.height;

						int Bx0 = groups[j].rect.x;
						int By0 = groups[j].rect.y;
						int Bx1 = Bx0+groups[j].rect.width;
						int By1 = By0+groups[j].rect.height;

						Ax0 = min(Ax0, Bx0);
						Ay0 = min(Ay0, By0);
						Ax1 = max(Ax1, Bx1);
						Ay1 = max(Ay1, By1);

						groups[i].rect.x = Ax0;
						groups[i].rect.y = Ay0;
						groups[i].rect.width = Ax1 - Ax0;
						groups[i].rect.height = Ay1 - Ay0;

						groups[i].active = true;

	 					groups.erase(groups.begin() + j);					
	 					j--;
 					}

				}
			}
		}

		if (hits==0) {
			finished = true;
		}

	}

	// assign objects to groups

	for (int i=0; i<groups.size(); i++) {
		groups[i].objectIndices.clear();

		for (int j=0; j<objects.size(); j++) {
			if (rectInside(objects[j].rect, groups[i].rect))
			{
				groups[i].objectIndices.push_back(j);
			}
		}
	}	

}


//--------------------------------------------------------------
void testApp::update(){

	ofBackground(90,90,90);

	for (int i=0; i<objects.size(); i++) {
		if (i != objhitidx) {
			objects[i].rect = objects[i].rect+objects[i].vel;
			objects[i].vel*=0.9;

		} else {
			if (!objects[i].velset) {
				objects[i].vel = ofVec2f(0,0);
			} else {
				objects[i].velset = false;
			}
		}

	}

	//processGroups();

}

//--------------------------------------------------------------
void testApp::draw() {

	ofNoFill();
	ofSetColor(80,80,80);
	for (int i=0; i<2000; i+=snap) {
		ofLine(i,0,i,2000);
		ofLine(0,i,2000,i);
	}

	//draw groups
	ofEnableAlphaBlending();
	for (int i=0; i<groups.size(); i++)
	{	
		if (groups[i].active) {
			ofFill();
			ofSetColor(50,50,50,50);
			ofRect(groups[i].rect);
		}
	}
	ofDisableAlphaBlending();

	// clear stacking info

	stacks.clear();
	for (int i=0; i<objects.size(); i++) {
		objects[i].stacked = false;
	}

	// draw the objects

	for (int i=0; i<objects.size(); i++)
	{	

		// let's calculate stacking here...
		bool stacked = false;
		for (int j=0; j<i; j++)
		{
			if (i!=j) {
				if (objects[i].rect == objects[j].rect) {
					stacked = true;

					bool hit = false;

					for (int k=0; k<stacks.size(); k++) {
						if (stacks[k].rect == objects[i].rect) {
							if (!objects[i].stacked) {
								stacks[k].number++;
							}
							objects[i].stacked = true;
							hit = true;
							
						}
					}

					if (!hit) {
						stack s;
						s.rect = objects[i].rect;
						objects[i].stacked = true;
						stacks.push_back(s);
					}

				}
			}
		}
		if (stacked) {

			ofPoint p0 = ofPoint(objects[i].rect.x + 2, objects[i].rect.y + 2);
			ofPoint p1 = p0 + ofPoint(objects[i].rect.width, objects[i].rect.height);

			ofPoint A = p0;
			ofPoint B = ofPoint(p1.x, p0.y);
			ofPoint C = ofPoint(p1.x, p1.y-20);
			ofPoint D = ofPoint(p1.x-20, p1.y);
			ofPoint E = ofPoint(p0.x, p1.y);
			ofPoint F = ofPoint(p1.x-20, p1.y-20);

			ofFill();
			switch (objects[i].type) {
				case 1:
					ofSetColor(255,251,176);
					break;
				case 2:
					ofSetColor(176,227,255);
					break;
				case 3:
					ofSetColor(255,211,176);
					break;
				default:
					ofSetColor(192,192,192);
					break;
			}
			

			ofBeginShape();
			ofVertex(A);
			ofVertex(B);
			ofVertex(C);
			ofVertex(D);
			ofVertex(E);
			ofVertex(A);
			ofEndShape();

			ofNoFill();
			ofSetColor(32,32,32);		
			
			ofBeginShape();
			ofVertex(A);
			ofVertex(B);
			ofVertex(C);
			ofVertex(D);
			ofVertex(E);
			ofVertex(A);
			ofEndShape();
			ofBeginShape();
			ofVertex(C);
			ofVertex(D);
			ofVertex(F);
			ofVertex(C);
			ofEndShape();

		} else {
			ofFill();
			switch (objects[i].type) {
				case 1:
					ofSetColor(255,251,176);
					break;
				case 2:
					ofSetColor(176,227,255);
					break;
				case 3:
					ofSetColor(255,211,176);
					break;
				default:
					ofSetColor(192,192,192);
					break;
			}
			ofRect(objects[i].rect);
			ofNoFill();
			ofSetColor(32,32,32);
			ofRect(objects[i].rect);
		}

	}

	// draw stack indicators

	for (int i=0; i<stacks.size(); i++) {

		if (stacks[i].number>1) {

			char buf[255];

			sprintf(buf, "%d", stacks[i].number);

			int x = stacks[i].rect.x + stacks[i].rect.width - 30;
			int y = stacks[i].rect.y + stacks[i].rect.height - 4;

			ofSetColor(0,0,0);
			ofDrawBitmapString(buf, x, y);

		}

	

	}

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

	if (key=='1') {
		obj o;
		o.rect.set(mousePos.x, mousePos.y, 95, 55);
		o.type = 1;
		o.depth = depthIdx++;
		objects.push_back(o);
	} else if (key=='2') {
		obj o;
		o.rect.set(mousePos.x, mousePos.y, 95, 55);
		o.type = 2;
		o.depth = depthIdx++;
		objects.push_back(o);
	} else if (key=='3') {
		obj o;
		o.rect.set(mousePos.x, mousePos.y, 95, 55);
		o.type = 3;
		o.depth = depthIdx++;
		objects.push_back(o);		
	}

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

	mousePos = ofPoint(x, y);

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

	ofPoint velocity = ofPoint(x,y) - oldMousePos;
	oldMousePos = ofPoint(x, y);

	if (objhitidx>=0) {

		objects[objhitidx].rect = objects[objhitidx].rect + velocity;
		objects[objhitidx].setvel(velocity);
		
		processGroups();

	} else if (grouphitidx>=0) {

		groups[grouphitidx].rect = groups[grouphitidx].rect + velocity;

		for (int i=0; i<groups[grouphitidx].objectIndices.size(); i++)
		{

			int idx = groups[grouphitidx].objectIndices[i];
			objects[idx].rect = objects[idx].rect + velocity;

		}

	}

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

	bool objhit = false;
	bool grouphit = false;
	oldMousePos = ofPoint(x, y);

	for (int i=objects.size()-1; i>=0; i--)
	{
		if (!objhit) {
			if (objects[i].rect.inside(x,y)) {
				objhit = true;
				objects[i].depth = depthIdx++;

			}
		}
	}

	if (objhit) {

		std::sort(objects.begin(), objects.end());			
		for (int i=0; i<objects.size(); i++)
		{
			if (objects[i].rect.inside(x,y)) {
				objhitidx = i;
			}
		}		
	}


	if (!objhit) {

		for (int i=0; i<groups.size(); i++) {
			groups[i].objectIndices.clear();

			for (int j=0; j<objects.size(); j++) {
				if (rectInside(objects[j].rect, groups[i].rect))
				{
					groups[i].objectIndices.push_back(j);
				}
			}
		}			

		for (int i=0; i<groups.size(); i++)
		{
			if (!grouphit) {
				if (groups[i].rect.inside(x,y)) {
					grouphit = true;
					grouphitidx = i;
				}
			}
		}		
	} 


}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {


	if (objhitidx>=0) {

		if (objects[objhitidx].vel.length()<3) {

			int nx = objects[objhitidx].rect.x;
			int ny = objects[objhitidx].rect.y;

			nx = int(round((float)nx/snap) * snap);
			ny = int(round((float)ny/snap) * snap);

			objects[objhitidx].rect.x = nx;
			objects[objhitidx].rect.y = ny;

			objects[objhitidx].vel = ofVec2f(0,0);
		}

	}

	

	if (grouphitidx>=0) {

		for (int i=0; i<groups[grouphitidx].objectIndices.size(); i++)
		{

			int idx = groups[grouphitidx].objectIndices[i];

			int nx = objects[idx].rect.x;
			int ny = objects[idx].rect.y;

			nx = int(round((float)nx/snap) * snap);
			ny = int(round((float)ny/snap) * snap);

			objects[idx].rect.x = nx;
			objects[idx].rect.y = ny;			

		}	
	}

	
	

	objhitidx = -1;
	grouphitidx = -1;

	processGroups();

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}