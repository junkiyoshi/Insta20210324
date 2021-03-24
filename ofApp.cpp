#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofSetCircleResolution(60);

	ofColor color;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list.push_back(color);
	}

	this->box2d.init();
	this->box2d.setGravity(0, 50);
	this->box2d.createBounds();
	this->box2d.setFPS(60);
	this->box2d.registerGrabbing();
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = this->circle_list.size() - 1; i > -1; i--) {

		this->life_list[i] -= this->life_speed_list[i];

		if (this->life_list[i] < 0) {

			this->circle_list.erase(this->circle_list.begin() + i);
			this->radius_list.erase(this->radius_list.begin() + i);
			this->log_list.erase(this->log_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);
			this->life_speed_list.erase(this->life_speed_list.begin() + i);
		}
	}

	if (ofGetFrameNum() % 3 == 0) {

		auto radius = 10;

		auto circle = make_shared<ofxBox2dCircle>();
		circle->setPhysics(0.5, 0.83, 0.3);
		circle->setup(this->box2d.getWorld(), ofRandom(radius, ofGetWidth() - radius), radius, radius);
		circle->setRotation(ofRandom(360));
		this->circle_list.push_back(circle);
		this->radius_list.push_back(radius);
		vector<glm::vec2> log;
		this->log_list.push_back(log);

		int color_index = ofRandom(this->base_color_list.size());
		this->color_list.push_back(this->base_color_list[color_index]);
		this->life_list.push_back(128);
		this->life_speed_list.push_back(ofRandom(0.5, 1.2));
	}

	this->box2d.update();

	for (int i = 0; i < this->circle_list.size(); i++) {

		this->log_list[i].push_back(this->circle_list[i]->getPosition());

		while (this->log_list[i].size() > 30) { this->log_list[i].erase(this->log_list[i].begin()); }
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);
	ofTranslate(ofGetWidth() * -0.5, ofGetHeight() * -0.5);

	for (int k = 0; k < this->circle_list.size(); k++) {

		auto log = this->log_list[k];

		ofMesh mesh, frame;
		frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

		for (int i = 0; i < log.size(); i++) {

			auto right = glm::vec3(log[i], -15);
			auto left = glm::vec3(log[i], 15);

			mesh.addVertex(right);
			mesh.addVertex(left);

			frame.addVertex(right);
			frame.addVertex(left);

			if (i > 0) {

				mesh.addIndex(mesh.getNumVertices() - 1); mesh.addIndex(mesh.getNumVertices() - 2); mesh.addIndex(mesh.getNumVertices() - 4);
				mesh.addIndex(mesh.getNumVertices() - 1); mesh.addIndex(mesh.getNumVertices() - 3); mesh.addIndex(mesh.getNumVertices() - 4);

				frame.addIndex(frame.getNumVertices() - 1); frame.addIndex(frame.getNumVertices() - 3);
				frame.addIndex(frame.getNumVertices() - 2); frame.addIndex(frame.getNumVertices() - 4);
			}

			mesh.addColor(ofColor(this->color_list[k], ofMap(this->life_list[k], 128, 0, 255, 32)));
			mesh.addColor(ofColor(this->color_list[k], ofMap(this->life_list[k], 128, 0, 255, 32)));

			frame.addColor(ofColor(239));
			frame.addColor(ofColor(239));
		}

		frame.addIndex(0); frame.addIndex(1);
		frame.addIndex(frame.getNumVertices() - 1);	frame.addIndex(frame.getNumVertices() - 2);

		mesh.draw();
		frame.drawWireframe();
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}