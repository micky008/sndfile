#include <stdlib.h>
#include <sndfile.h>
#include <string.h>

#define true 1
#define false 0

struct options
{
	char *inputFile;
	char *folderOutput;
	int nbBlank;
	char *outSuffix;
	char *outPrefix;

} typedef options;

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

options *readOpt(int argc, char *argv[])
{
	char *help = "-o outputFolder(without slash at end)\n-i inputFile(zzz.wav,zzz.flac,etc...)\n[-p prefix (yyy) (final result => 01-yyy.wav))]\n[-b time in ms for silence between 2 songs (408 by default)]\n[-s suffix(wav by default)]\n ";
	if (argc < 5)
	{
		puts(help);
		return NULL;
	}
	int optObligatoires = 0;
	//check si les args obligatoire sont la
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-o") == 0)
		{
			optObligatoires++;
		}
		else if (strcmp(argv[i], "-i") == 0)
		{
			optObligatoires++;
		}
	}
	if (optObligatoires < 2)
	{
		puts(help);
		return NULL;
	}
	//init struct
	//nbBlank default = 18000
	//outSuffix default = "wav"
	options *opt = malloc(sizeof(options));
	opt->nbBlank = 18000;
	opt->outSuffix = "wav";
	opt->outPrefix = "AudioTrack";
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-o") == 0)
		{
			i++;
			opt->folderOutput = argv[i];
		}
		else if (strcmp(argv[i], "-i") == 0)
		{
			i++;
			opt->inputFile = argv[i];
		}
		else if (strcmp(argv[i], "-b") == 0)
		{
			i++;
			opt->nbBlank = (atoi(argv[i]) * 44100) / 1000;
		}
		else if (strcmp(argv[i], "-s") == 0)
		{
			i++;
			opt->outSuffix = argv[i];
		}
		else if (strcmp(argv[i], "-p") == 0)
		{
			i++;
			opt->outPrefix = argv[i];
		}
		else
		{
			free(opt);
			puts(help);
			return NULL;
		}
	}
	return opt;
}

char *getOutTrack(options *opts, int nbSong)
{
	int taille = strlen(opts->folderOutput) + 5 + strlen(opts->outPrefix) + strlen(opts->outSuffix); //5 => '000-' and '.'   // c:/folder/000-zik.wav
	char *res = malloc(taille * sizeof(char));
	memset(res, 0, taille);
	sprintf(res, "%s/%00d-%s.%s", opts->folderOutput, nbSong, opts->outPrefix, opts->outSuffix);
	return res;
}

int main(int argc, char *argv[])
{
	long sampleCount = 1;
	float gauche = 0;
	float droite = 0;
	int nbSong = 0;
	int previous = 0;
	int nbBlank = 0;
	options *opts = readOpt(argc, argv);
	if (opts == NULL)
	{
		return 1;
	}
	SF_INFO sfInfo;
	SNDFILE *sndFile = sf_open(opts->inputFile, SFM_READ, &sfInfo);
	long nsc2 = sfInfo.frames;
	printf("nbFrame %d\n", nsc2);
	printf("temps (en sec) %d\n", nsc2 / 44100);
	int BUFFER = nsc2 * 2 > 2147483647 ? 2147483647 : nsc2 * 2;
	int SAMPLE = BUFFER / 2;
	float *farray = malloc(BUFFER * sizeof(float));
	memset(farray, 0, BUFFER);
	while (sampleCount > 0)
	{
		sampleCount = sf_readf_float(sndFile, farray, SAMPLE);
		printf("sample count lu = %d\n", sampleCount);
		previous = 0;
		for (int i = 0; i < sampleCount; i++)
		{
			gauche = farray[i * 2];
			droite = farray[i * 2 + 1];
			if (gauche == 0 && droite == 0 || (i + 1 > sampleCount))
			{
				nbBlank = 0;
				for (; i < sampleCount; i++)
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
				if (nbBlank > opts->nbBlank || (i + 1 > sampleCount))
				{
					nbSong++;
					char *file = getOutTrack(opts, nbSong);
					SF_INFO sfI2;
					memcpy(&sfI2, &sfInfo, sizeof(SF_INFO));
					sfI2.frames = i - previous;
					float *outBuf = myMemcpy(farray, previous, i - previous);
					SNDFILE *sndFile2 = sf_open(file, SFM_WRITE, &sfI2);
					sf_write_float(sndFile2, outBuf, (i - previous) * 2);
					sf_close(sndFile2);
					free(file);
					free(outBuf);
					previous = i;
				}
			}
		}
	}
	free(opts);
	free(farray);
	sf_close(sndFile);
	return 0;
}