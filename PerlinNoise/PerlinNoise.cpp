#include "olcConsoleGameEngine.h"
using namespace std;
#include <fstream>
#include <strstream>


class PerlinNoise : public olcConsoleGameEngine
{
public:
	PerlinNoise() {
		m_sAppName = L"Perlin Noise";
	}

private:

	//1D vars
	float* fNoiseSeed1D = nullptr;
	float* fPerlinNoise1D = nullptr;
	int nOutputSize = 256;
	int nOctaveCount = 1;

	//2D vars
	float* fNoiseSeed2D = nullptr;
	float* fPerlinNoise2D = nullptr;
	int nOutputWidth = 256;
	int nOutputHeight = 256;

	float fScalingBias = 2.0f; 
	int nMode = 1;

	virtual bool OnUserCreate() {
		nOutputSize = ScreenWidth();
		fNoiseSeed1D = new float[nOutputSize];
		fPerlinNoise1D = new float[nOutputSize];

		nOutputWidth = ScreenWidth();
		nOutputHeight = ScreenHeight();

		fNoiseSeed2D = new float[nOutputHeight * nOutputWidth];
		fPerlinNoise2D = new float[nOutputHeight * nOutputWidth];

		for (int i = 0; i < nOutputSize; i++) {
			fNoiseSeed1D[i] = (float)rand() / (float)RAND_MAX;
		}

		for (int i = 0; i < nOutputHeight * nOutputWidth; i++) {
			fNoiseSeed2D[i] = (float)rand() / (float)RAND_MAX;
		}

		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime) {
		Fill(0, 0, ScreenHeight(), ScreenHeight(), L' ');
		if (m_keys[VK_SPACE].bPressed)
			nOctaveCount++;

		if (m_keys[L'Z'].bReleased) {
			for (int i = 0; i < nOutputSize; i++) {
				fNoiseSeed1D[i] = (float)rand() / (float)RAND_MAX;
			}
		}

		if (m_keys[L'W'].bReleased) fScalingBias += 0.2f;
		if (m_keys[L'S'].bReleased) fScalingBias -= 0.2f;

		if (m_keys[L'1'].bReleased)
			nMode = 1;

		if (m_keys[L'2'].bReleased)
			nMode = 2;

		if (m_keys[L'3'].bReleased)
			nMode = 3;

		if (fScalingBias < 0.2f) {
			fScalingBias = 0.2f;
		}

		if (nOctaveCount == 9)
			nOctaveCount = 1;

		if (nMode == 1) {

			if (m_keys[L'Z'].bReleased) {
				for (int i = 0; i < nOutputSize; i++) {
					fNoiseSeed1D[i] = (float)rand() / (float)RAND_MAX;
				}
			}

			PerlinNoise1D(nOutputSize, fNoiseSeed1D, fPerlinNoise1D, nOctaveCount, fScalingBias);

			for (int x = 0; x < nOutputSize; x++) {
				int y = -(fPerlinNoise1D[x] * (float)ScreenHeight() / 2.0f) + (float)ScreenHeight() / 2.0f; // scaling into the middle

				for (int f = y; f < ScreenHeight() / 2; f++) {
					Draw(x, f, PIXEL_SOLID, FG_GREEN);
				}
			}
		}

		if (nMode == 2) {
			if (m_keys[L'Z'].bReleased) {
				for (int i = 0; i < nOutputHeight * nOutputWidth; i++) {
					fNoiseSeed2D[i] = (float)rand() / (float)RAND_MAX;
				}
			}
			PerlinNoise2D(nOutputWidth, nOutputHeight, fNoiseSeed2D, fPerlinNoise2D, nOctaveCount, fScalingBias);


			for (int x = 0; x < nOutputWidth; x++)
			{
				for (int y = 0; y < nOutputHeight; y++)
				{
					short bg_col, fg_col;
					wchar_t sym;
					int pixel_bw = (int)(fPerlinNoise2D[y * nOutputWidth + x] * 12.0f);
					switch (pixel_bw)
					{
					case 0: bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID; break;

					case 1: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_QUARTER; break;
					case 2: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_HALF; break;
					case 3: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_THREEQUARTERS; break;
					case 4: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_SOLID; break;

					case 5: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_QUARTER; break;
					case 6: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_HALF; break;
					case 7: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_THREEQUARTERS; break;
					case 8: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_SOLID; break;

					case 9:  bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_QUARTER; break;
					case 10: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_HALF; break;
					case 11: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_THREEQUARTERS; break;
					case 12: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_SOLID; break;
					}

					Draw(x, y, sym, fg_col | bg_col);
				}
			}
		}

		if (nMode == 3) // 2D Noise - colourised
		{
			if (m_keys[L'Z'].bReleased) // Noise Between 0 and +1
				for (int i = 0; i < nOutputWidth * nOutputHeight; i++) fNoiseSeed2D[i] = (float)rand() / (float)RAND_MAX;


			PerlinNoise2D(nOutputWidth, nOutputHeight, fNoiseSeed2D, fPerlinNoise2D, nOctaveCount, fScalingBias);

			for (int x = 0; x < nOutputWidth; x++)
			{
				for (int y = 0; y < nOutputHeight; y++)
				{
					short bg_col, fg_col;
					wchar_t sym;
					int pixel_bw = (int)(fPerlinNoise2D[y * nOutputWidth + x] * 16.0f);
					switch (pixel_bw)
					{
					case 0: bg_col = BG_DARK_BLUE; fg_col = FG_DARK_BLUE; sym = PIXEL_SOLID; break;

					case 1: bg_col = BG_DARK_BLUE; fg_col = FG_BLUE; sym = PIXEL_QUARTER; break;
					case 2: bg_col = BG_DARK_BLUE; fg_col = FG_BLUE; sym = PIXEL_HALF; break;
					case 3: bg_col = BG_DARK_BLUE; fg_col = FG_BLUE; sym = PIXEL_THREEQUARTERS; break;
					case 4: bg_col = BG_DARK_BLUE; fg_col = FG_BLUE; sym = PIXEL_SOLID; break;

					case 5: bg_col = BG_BLUE; fg_col = FG_GREEN; sym = PIXEL_QUARTER; break;
					case 6: bg_col = BG_BLUE; fg_col = FG_GREEN; sym = PIXEL_HALF; break;
					case 7: bg_col = BG_BLUE; fg_col = FG_GREEN; sym = PIXEL_THREEQUARTERS; break;
					case 8: bg_col = BG_BLUE; fg_col = FG_GREEN; sym = PIXEL_SOLID; break;

					case 9:  bg_col = BG_GREEN; fg_col = FG_DARK_GREY; sym = PIXEL_QUARTER; break;
					case 10: bg_col = BG_GREEN; fg_col = FG_DARK_GREY; sym = PIXEL_HALF; break;
					case 11: bg_col = BG_GREEN; fg_col = FG_DARK_GREY; sym = PIXEL_THREEQUARTERS; break;
					case 12: bg_col = BG_GREEN; fg_col = FG_DARK_GREY; sym = PIXEL_SOLID; break;

					case 13: bg_col = BG_DARK_GREY; fg_col = FG_WHITE; sym = PIXEL_QUARTER; break;
					case 14: bg_col = BG_DARK_GREY; fg_col = FG_WHITE; sym = PIXEL_HALF; break;
					case 15: bg_col = BG_DARK_GREY; fg_col = FG_WHITE; sym = PIXEL_THREEQUARTERS; break;
					case 16: bg_col = BG_DARK_GREY; fg_col = FG_WHITE; sym = PIXEL_SOLID; break;
					}

					Draw(x, y, sym, fg_col | bg_col);
				}
			}
		}
		return true;
	}

	void PerlinNoise1D(int nCount, float* fSeed, float* fOutput, int nOctaves, float fBias) {

		for (int x = 0; x < nCount; x++) {

			float fNoise = 0.0f;
			float fScale = 1.0f;

			//accumilate scale values in order to get fNoise to be between 0 and 1
			float fScaleAcc = 0.0f;
			// For every point we take two samples depending on the octive
			// the higher the octave the closer the two samples generated by the rand function
			// we interpolate between the two samples to get our sample for x
			
			for (int o = 0; o < nOctaves; o++) {
				int nPitch = nCount >> o;	//nPitch is the point we linearly interpolate between
				int nSample1 = (x / nPitch) * nPitch;
				int nSample2 = (nSample1 + nPitch) % nCount;

				float fBlend = (float)(x - nSample1) / (float)nPitch; // how far alont the pitch we are
				float fSample = (1.0f - fBlend) * fSeed[nSample1] + fBlend * fSeed[nSample2]; // linear interpolation

				fNoise += fSample * fScale;
				fScaleAcc += fScale;
				fScale /= fBias;
			}

			fOutput[x] = fNoise / fScaleAcc;
		}
	}

	void PerlinNoise2D(int nWidth, int nHeight, float* fSeed, float* fOutput, int nOctaves, float fBias) {

		for (int x = 0; x < nWidth; x++)
			for (int y = 0; y < nHeight; y++) {

			float fNoise = 0.0f;
			float fScale = 1.0f;

			//accumilate scale values in order to get fNoise to be between 0 and 1
			float fScaleAcc = 0.0f;
			// For every point we take two samples depending on the octive
			// the higher the octave the closer the two samples generated by the rand function
			// we interpolate between the two samples to get our sample for x

			for (int o = 0; o < nOctaves; o++) {
				int nPitch = nWidth >> o;	//nPitch is the point we linearly interpolate between

				int nSampleX1= (x / nPitch) * nPitch;
				int nSampleY1 = (y / nPitch) * nPitch;

				int nSampleX2 = (nSampleX1 + nPitch) % nWidth;
				int nSampleY2 = (nSampleY1 + nPitch) % nWidth;

				float fBlendX = (float)(x - nSampleX1) / (float)nPitch; // how far alont the pitch we are
				float fBlendY = (float)(y - nSampleY1) / (float)nPitch; // how far alont the pitch we are

				float fSampleT = (1.0f - fBlendX) * fSeed[nSampleY1 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY1 * nWidth + nSampleX2]; // linear interpolation
				float fSampleB = (1.0f - fBlendX) * fSeed[nSampleY2 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY2 * nWidth + nSampleX2]; // linear interpolation

				fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale; //interpolate between the y
				fScaleAcc += fScale;
				fScale /= fBias;
			}

			fOutput[y * nWidth + x] = fNoise / fScaleAcc;
		}
	}

};

int main() {
	PerlinNoise game;
	game.ConstructConsole(256, 256, 3, 3);
	game.Start();
	return 0;
}