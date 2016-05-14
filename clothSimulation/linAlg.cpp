#include "linAlg.h"

void linAlg::matrixMult(float* M1, float* M2, float* Mout) {
	// Compute result in a local variable to avoid conflicts
	// with overwriting if Mout is the same variable as either
	// M1 or M2.
	float Mtemp[16];
	int i, j;
	// Perform the multiplication M3 = M1*M2
	// (i is row index, j is column index)
	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++) {
			Mtemp[i + j * 4] = M1[i] * M2[j * 4] + M1[i + 4] * M2[1 + j * 4]
				+ M1[i + 8] * M2[2 + j * 4] + M1[i + 12] * M2[3 + j * 4];
		}
	}
	// Copy the result to the output variable
	for (i = 0; i<16; i++) {
		Mout[i] = Mtemp[i];
	}
}

void linAlg::matrixMultTHREE(float* M1, float* M2, float* Mout) {
	// Compute result in a local variable to avoid conflicts
	// with overwriting if Mout is the same variable as either
	// M1 or M2.
	float Mtemp[9];
	int i, j;
	// Perform the multiplication M3 = M1*M2
	// (i is row index, j is column index)
	for (i = 0; i<3; i++) {
		for (j = 0; j<3; j++) {
			Mtemp[3*i + j] = M1[i*3] * M2[j] + 
							   M1[i*3 + 1] * M2[3 + j] + 
							   M1[i*3 + 2] * M2[6 + j];
		}
	}
	// Copy the result to the output variable
	for (i = 0; i<9; i++) {
		Mout[i] = Mtemp[i];
	}
}

void linAlg::vectorMatrixMult(float* M1, float* V, float* Vout) {
	//Vout[0] = M1[0] * V[0] + M1[1] * V[1] + M1[2] * V[2] + M1[3] * V[3];
	//Vout[1] = M1[4] * V[0] + M1[5] * V[1] + M1[6] * V[2] + M1[7] * V[3];
	//Vout[2] = M1[8] * V[0] + M1[9] * V[1] + M1[10] * V[2] + M1[11] * V[3];
	//Vout[3] = M1[12] * V[0] + M1[13] * V[1] + M1[14] * V[2] + M1[15] * V[3];

	Vout[0] = M1[0] * V[0] + M1[4] * V[1] + M1[8] * V[2] + M1[12] * V[3];
	Vout[1] = M1[1] * V[0] + M1[5] * V[1] + M1[9] * V[2] + M1[13] * V[3];
	Vout[2] = M1[2] * V[0] + M1[6] * V[1] + M1[10] * V[2] + M1[14] * V[3];
	Vout[3] = M1[3] * V[0] + M1[7] * V[1] + M1[11] * V[2] + M1[15] * V[3];
}

void linAlg::transpose(float* m)
{
	float temp[16];

	temp[0] = m[0]; temp[1] = m[4]; temp[2] = m[8]; temp[3] = m[12];
	temp[4] = m[1]; temp[5] = m[5]; temp[6] = m[9]; temp[7] = m[13];
	temp[8] = m[2]; temp[9] = m[6]; temp[10] = m[10]; temp[11] = m[14];
	temp[12] = m[3]; temp[13] = m[7]; temp[14] = m[11]; temp[15] = m[15];

	for (int i = 0; i < 16; i++)
		m[i] = temp[i];
}

void linAlg::makeUniform(float* m)
{
	for (int i = 0; i < 16; i++) {
		if (i == 0 || i == 5 || i == 10 || i == 15)
			m[i] = 1.0f;
		else 
			m[i] = 0.0f;
	}
}

void linAlg::invertMatrix(float* m, float* invOut)
{
	float inv[16], det;
	int i;

	inv[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0) {
		std::cout << "inverseMatrix: Determinant = 0. Cannot inverse." << std::endl;
	}
	else {
		det = 1.0 / det;
		for (i = 0; i < 16; i++)
			invOut[i] = inv[i] * det;
	}
}

void linAlg::calculateVec(float* a, float* b, float* newVec) {
	newVec[0] = a[0] - b[0];
	newVec[1] = a[1] - b[1];
	newVec[2] = a[2] - b[2];
}

void linAlg::normVec(float* vec) {
	float length = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);

	if (length != 0) {
		vec[0] = vec[0] / length;
		vec[1] = vec[1] / length;
		vec[2] = vec[2] / length;
	}
}

void linAlg::crossProd(float normal[3], float* vec1, float* vec2) {
	normal[0] = (vec1[1] * vec2[2] - vec1[2] * vec2[1]);
	normal[1] = -(vec1[0] * vec2[2] - vec1[2] * vec2[0]);
	normal[2] = (vec1[0] * vec2[1] - vec1[1] * vec2[0]);
}

void linAlg::rotAxis(float axis[3], float a, float* mOut)
{
	float u = axis[0];
	float v = axis[1];
	float w = axis[2];

	mOut[0] = pow(u, 2) + (1 - pow(u, 2))*cos(a);/**/ mOut[1] = u*v*(1 - cos(a)) - w*sin(a);/**/ mOut[2] = u*w*(1 - cos(a)) + v*sin(a);/**/ mOut[3] = 0;
	mOut[4] = u*v*(1.f - cos(a)) + w*sin(a);/**/mOut[5] = pow(v, 2) + (1 - pow(v, 2))*cos(a); /**/mOut[6] = v*w*(1 - cos(a)) - u*sin(a); /**/mOut[7] = 0;
	mOut[8] = u*w*(1.f - cos(a)) - v*sin(a);/**/ mOut[9] = v*w*(1.f - cos(a)) + u*sin(a);/**/ mOut[10] = pow(w, 2) + (1.f - pow(w, 2))*cos(a);/**/ mOut[11] = 0;
	mOut[12] = 0.f; mOut[13] = 0.f; mOut[14] = 0.f; mOut[15] = 1.f;
}

float linAlg::vecLength(float vec[3]) {
	return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

float linAlg::dotProd(float vec1[3], float vec2[3]){
	return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
}

float linAlg::lengthToSurface(float v[3], float w[3], float rad)
{
	//float t;
	return (-(v[0]*w[0] + v[1]*w[1] + v[2]*w[2])/(w[0]*w[0] + w[1]*w[1] + w[2]*w[2])) 
		+ sqrt((rad*rad + (v[0]*w[0] + v[1]*w[1] + v[2]*w[2]) - (v[0]*v[0] + v[1]*v[1] + v[2]*v[2]))/(w[0]*w[0] + w[1]*w[1] + w[2]*w[2]) );
}

void linAlg::rotX(float angle, float* resultMat) {
	resultMat[0] = 1.0f;	resultMat[1] = 0.0f;			resultMat[2] = 0.0f;			resultMat[3] = 0.0f;
	resultMat[4] = 0.0f;	resultMat[5] = cosf(angle);		resultMat[6] = sinf(angle);		resultMat[7] = 0.0f;
	resultMat[8] = 0.0f;	resultMat[9] = -sinf(angle);	resultMat[10] = cosf(angle);	resultMat[11] = 0.0f;
	resultMat[12] = 0.0f;	resultMat[13] = 0.0f;			resultMat[14] = 0.0f;			resultMat[15] = 1.0f;
};

void linAlg::rotY(float angle, float* resultMat) {
	resultMat[0] = cosf(angle);		resultMat[1] = 0.0f;	resultMat[2] = -sinf(angle);	resultMat[3] = 0.0f;
	resultMat[4] = 0.0f;			resultMat[5] = 1.0f;	resultMat[6] = 0.0f;			resultMat[7] = 0.0f;
	resultMat[8] = sinf(angle);		resultMat[9] = 0.0f;	resultMat[10] = cosf(angle);	resultMat[11] = 0.0f;
	resultMat[12] = 0.0f;			resultMat[13] = 0.0f;	resultMat[14] = 0.0f;			resultMat[15] = 1.0f;
};

void linAlg::rotZ(float angle, float* resultMat) {
	resultMat[0] = cosf(angle);		resultMat[1] = sinf(angle);		resultMat[2] = 0.0f;	resultMat[3] = 0.0f;
	resultMat[4] = -sinf(angle);	resultMat[5] = cosf(angle);		resultMat[6] = 0.0f;	resultMat[7] = 0.0f;
	resultMat[8] = 0.0f;			resultMat[9] = 0.0f;			resultMat[10] = 1.0f;	resultMat[11] = 0.0f;
	resultMat[12] = 0.0f;			resultMat[13] = 0.0f;		resultMat[14] = 0.0f;		resultMat[15] = 1.0f;
};