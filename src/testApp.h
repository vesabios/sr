#pragma once

#include "ofMain.h"
#include <vector>



class testApp : public ofBaseApp{

	struct obj {
		ofRectangle		rect;
		bool			clicked;
		int				type;
		int				depth;
		ofVec2f			vel;
		bool			velset;
		bool			stacked;

		bool operator < (const obj& s) const
		{
	   		return (depth < s.depth);
		}

		void setvel (const ofVec2f v)
		{
			vel = v;
			velset = true;
		}

		obj()
			: velset(false)
			, clicked(false)
		{}		
	};

	struct group {
		ofRectangle		rect;
		bool			active;
		vector<int>		objectIndices;
	};

	struct stack {
		ofRectangle		rect;
		int				number;
		stack()
			: number(1)
		{}			
	};


	public:

		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		bool rectInside(const ofRectangle A, const ofRectangle B);
		void processGroups();

		int objhitidx;
		int grouphitidx;

		int depthIdx;

		ofPoint oldMousePos;
		ofPoint mousePos;

		int margin;
		int snap;
		vector<obj> 			objects;
		vector<group>			groups;
		vector<stack>			stacks;


};
