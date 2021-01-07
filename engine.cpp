#include "olcConsoleGameEngineSDL.h" 
using namespace std;

struct vec3d {
	float x, y, z;
};

struct triangle {
	vec3d p[3];
};

struct mesh {
	vector<triangle> tris;
};

struct mat4x4 {
	float m[4][4] = { 0 };
};

class Engine: public olcConsoleGameEngine {
public:
	Engine() {
		m_sAppName = L"3D Demo";
	}

private:
	mesh meshCube;
	mat4x4 matProj;

	float fTheta = 0;

	void MultiplyMatrixVector(vec3d &i, vec3d &o, mat4x4 &m) {
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f) {
			o.x /= w;
			o.y /= w;
			o.z /= w;
		}
	}

public:
	bool OnUserCreate() override {
		meshCube.tris = {
			// SOUTH
			{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

			// EAST                                                      
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

			// NORTH                                                     
			{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

			// WEST                                                      
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

			// TOP                                                       
			{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

			// BOTTOM                                                    
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
		};

		//Projection Matrix
		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fFOV = 120.0f;
		float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
		float fFOVRad = 1.0f / tanf(fFOV * 0.5f / 180.0f * 3.14159f);

		matProj.m[0][0] = fAspectRatio * fFOVRad;
		matProj.m[1][1] = fFOVRad;
		matProj.m[2][2] = fFar / (fFar - fNear);
		matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matProj.m[2][3] = 1.0f;
		matProj.m[3][3] = 0.0f;
		printf("\n");
		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override {
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_DARK_BLUE);
		printf("\rElapsedTime: %f", fTheta);
		mat4x4 matRotZ, matRotX, matRotY;
		fTheta += .5f * fElapsedTime;
		printf("\rElapsedTime: %f", fTheta);

		// Rotation Z
		matRotZ.m[0][0] = cosf(.1f);
		matRotZ.m[0][1] = sinf(.1f);
		matRotZ.m[1][0] = -sinf(.1f);
		matRotZ.m[1][1] = cosf(.1f);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		// Rotation Y
		matRotY.m[0][0] = cosf(fTheta);
		matRotY.m[0][2] = sinf(fTheta);
		matRotY.m[2][0] = -sinf(fTheta);
		matRotY.m[2][2] = cosf(fTheta);
		matRotY.m[1][1] = 1;
		matRotY.m[3][3] = 1;


		for(auto tri: meshCube.tris) {
			triangle triProjected, triTranslated, triRotatedZ, triRotatedZY, triOriginShift, triOriginCorrected, triRotatedX;

			// MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
			// MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
			// MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

			// MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
			// MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
			// MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

			// triTranslated = triRotatedZX;
			// triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
			// triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
			// triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

			for(int i=0; i < 3; i++) {
				triOriginShift = tri;
				//Move Origin to centre of cube
				triOriginShift.p[i].x -= 0.5f;
				triOriginShift.p[i].y -= 0.5f;
				triOriginShift.p[i].z -= 0.5f;
				// Rotate around Z Axis
				MultiplyMatrixVector(triOriginShift.p[i], triRotatedZ.p[i], matRotZ);
				// Rotate around X Axis
				MultiplyMatrixVector(triRotatedZ.p[i], triRotatedZY.p[i], matRotY);
			}

			// //Scale into view
			// triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f; 
			// triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f; 
			// triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f; 

			// triProjected.p[0].x *= 0.5f * (float)ScreenWidth();
			// triProjected.p[0].y *= 0.5f * (float)ScreenHeight(); 
			// triProjected.p[1].x *= 0.5f * (float)ScreenWidth();
			// triProjected.p[1].y *= 0.5f * (float)ScreenHeight(); 
			// triProjected.p[2].x *= 0.5f * (float)ScreenWidth();
			// triProjected.p[2].y *= 0.5f * (float)ScreenHeight(); 

			for(int x=0; x < ScreenWidth(); x+=90) {
				for(int y=0; y < ScreenHeight(); y+=80) {
					for(int i=0; i < 3; i++) {
						// Rotation X
						matRotX.m[0][0] = 1;
						matRotX.m[1][1] = cosf((1+x)*(1+y));
						matRotX.m[1][2] = sinf((1+x)*(1+y));
						matRotX.m[2][1] = -sinf((1+x)*(1+y));
						matRotX.m[2][2] = cosf((1+x)*(1+y));
						matRotX.m[3][3] = 1;


						MultiplyMatrixVector(triRotatedZY.p[i], triRotatedX.p[i], matRotX);
						// Move Origin back
						triOriginCorrected = triRotatedX;
						triOriginCorrected.p[i].x += 0.5f;
						triOriginCorrected.p[i].y += 0.5f;
						triOriginCorrected.p[i].z += 0.5f;
						// Offset Z Axis 'into' the screen
						triTranslated = triOriginCorrected;
						triTranslated.p[i].z = triOriginCorrected.p[i].z + 3.0f;
						//Project 3d object into 2d field of view
						MultiplyMatrixVector(triTranslated.p[i], triProjected.p[i], matProj);
						// Scale unit cube to constraints of screen
						triProjected.p[i].x += .08f; triProjected.p[i].y += .08f; 
						triProjected.p[i].x *= 1.0f * (float)ScreenWidth();
						triProjected.p[i].y *= 1.0f * (float)ScreenHeight(); 
					}
					DrawTriangle(triProjected.p[0].x + x, triProjected.p[0].y + y,
						triProjected.p[1].x + x, triProjected.p[1].y + y,
						triProjected.p[2].x + x, triProjected.p[2].y + y);
				} 
			} 

		}
		return true;
	}
};

 int main() {
 	Engine engine;
 	if(engine.ConstructConsole(256, 240, 2, 2))
 		engine.Start();
 	return 0;
 }