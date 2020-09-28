#include <stdlib.h>
#include <sndfile.h>
#include <string.h>

float *myMemcpy(float *src, int debut, int cmb)
{
	float *res = malloc(cmb * 2 * sizeof(float));
	memset(res, 0, cmb * 2);
	for (int i = 0; i < cmb; i++)
	{
		float g = src[(i + debut) * 2];
		float d = src[(i + debut) * 2 + 1];
		res[i * 2] = g;
		res[i * 2 + 1] = d;
	}
	return res;
}

int main(int argc, char *argv[])
{
	SNDFILE *sndFile;
	SNDFILE *sndFile2;
	long nsc2;
	int BUFFER;
	int SAMPLE;
	long simpleCount = 1;
	float gauche = 0;
	float droite = 0;
	int nbSong = 0;
	int previous = 0;
	int nbBlank = 0;
	char *wavFile = "C:/ffmpeg/carpenter-brut-trilogy.wav";
	printf("%s\n", wavFile);
	SF_INFO sfInfo;
	sndFile = sf_open(wavFile, SFM_READ, &sfInfo);
	nsc2 = sfInfo.frames;
	printf("nbFrame %d\n", nsc2);
	printf("temps (en sec) %d\n", nsc2 / 44100);
	BUFFER = nsc2 * 2 > 2147483647 ? 2147483647 : nsc2 * 2;
	SAMPLE = BUFFER / 2;
	float *farray = malloc(BUFFER * sizeof(float));
	memset(farray, 0, BUFFER);
	while (simpleCount > 0)
	{
		simpleCount = sf_readf_float(sndFile, farray, SAMPLE);
		printf("sample count lu = %d\n", simpleCount);
		previous = 0;
		for (int i = 0; i < simpleCount; i++)
		{
			gauche = farray[i * 2];
			droite = farray[i * 2 + 1];
			if (gauche == 0 && droite == 0 || (i + 1 > simpleCount))
			{
				nbBlank = 0;
				for (; i < simpleCount; i++)
				{
					gauche = farray[i * 2];
					droite = farray[i * 2 + 1];
					if (gauche == 0 && droite == 0)
					{
						nbBlank++;
					}
					else
					{
						break;
					}
				}
				if (nbBlank > 18000 || (i + 1 > simpleCount))
				{
					nbSong++;
					char file[40];
					memset(file, 0, 40);
					sprintf(file, "C:/ffmpeg/test//cb-%d.wav", nbSong);
					SF_INFO sfI2;
					memcpy(&sfI2, &sfInfo, sizeof(SF_INFO));
					sfI2.frames = i - previous;
					float *outBuf = myMemcpy(farray, previous, i - previous);
					sndFile2 = sf_open(file, SFM_WRITE, &sfI2);
					sf_write_float(sndFile2, outBuf, (i - previous) * 2);
					sf_close(sndFile2);
					free(outBuf);
					previous = i;
				}
			}
		}
	}

	free(farray);
	sf_close(sndFile);
	return 0;
}