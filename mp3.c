#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <mpg123.h>
#include "list.h"

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

float *myMemcpy(short *src, int debut, int cmb)
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
    char *help = "-o outputFolder(without slash at end)\n-i inputFile(zzz.mp3)\n[-p prefix (yyy) (final result => 01-yyy.mp3)) [by default AudioTrack]]\n[-b time in ms for silence between 2 songs (408 by default)] [-n nb sample blank 18000 by default]\n ";
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
    opt->outSuffix = "mp3";
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
        else if (strcmp(argv[i], "-p") == 0)
        {
            i++;
            opt->outPrefix = argv[i];
        }
        else if (strcmp(argv[i], "-n") == 0)
        {
            i++;
            opt->nbBlank = atoi(argv[i]);
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
    short gauche = 0;
    short droite = 0;
    int previous = 0;
    int nbBlank = 0;
    int ret = 1;
    options *opts = readOpt(argc, argv);
    if (opts == NULL)
    {
        return 1;
    }
    mpg123_init();
    mpg123_handle *m = mpg123_new(NULL, &ret);
    if (m == NULL)
    {
        fprintf(stderr, " 1 Unable to create mpg123 handle: %s\n", mpg123_plain_strerror(ret));
        free(opts);
        return -1;
    }
    mpg123_open(m, opts->inputFile);
    if (m == NULL)
    {
        printf("impossible to read: %s", opts->inputFile);
        free(opts);
        return -1;
    }

    ret = 1;
    long rate;
    int channels, enc;
    mpg123_getformat(m, &rate, &channels, &enc);
    size_t buffer_size = mpg123_outblock(m);
    short *minibuf = calloc(buffer_size, sizeof(short) + 1);
    mpg123_scan(m);
    off_t nsc2 = mpg123_length(m);
    printf("nb de Sample %d\n", nsc2);
    printf("rate %d\n", rate);
    printf("temps (en sec) %d\n", nsc2 / rate);
    short *buf = calloc(nsc2 * 2, sizeof(short) + 1);
    size_t sampleCount = 1;
    Liste *liste = newList();
    unsigned long bufPos = 0;
    while (ret != MPG123_DONE)
    {
        ret = mpg123_read(m, minibuf, buffer_size, &sampleCount);
        if (ret == MPG123_DONE)
        {
            continue;
        }
        for (int i = 0; i < sampleCount / 2; i++)
        {
            buf[i + bufPos] = minibuf[i];
        }
        bufPos += sampleCount / 2 + 1;
    }
    free(minibuf);
    printf("sample count lu = %d\n", bufPos);
    previous = 0;
    for (unsigned long i = 0; i < bufPos; i += 2)
    {
        gauche = buf[i];
        droite = buf[i + 1];
        if (gauche == 0 && droite == 0 || (i + 2 > bufPos))
        {
            nbBlank = 0;
            for (; i < bufPos; i += 2)
            {
                gauche = buf[i];
                droite = buf[i + 1];
                if (gauche == 0 && droite == 0)
                {
                    nbBlank++;
                }
                else
                {
                    break;
                }
            }
            if (nbBlank > opts->nbBlank || (i + 2 > bufPos))
            {
                insertList(liste, previous, i);
                previous = i;
            }
        }
    }

    printf("I've detect %d songs to write\n", liste->len);

    Element *el = liste->premier;
    mpg123_seek(m, 0, SEEK_SET);
    long count = 0;
    int nbSong = 1;
    while (el != NULL)
    {
        char *fileName = getOutTrack(opts, nbSong);
        printf("try to write %s\n", fileName);
        FILE *myfile = fopen(fileName, "wb");
        long max = (el->fin - el->debut) / 2;
        count = 0;

        while (count < max)
        {
            if ((ret = mpg123_framebyframe_next(m)) == MPG123_OK || ret == MPG123_NEW_FORMAT)
            {
                unsigned long header;
                unsigned char *bodydata;
                size_t bodybytes;
                if (mpg123_framedata(m, &header, &bodydata, &bodybytes) == MPG123_OK)
                {
                    /* Need to extract the 4 header bytes from the native storage in the correct order. */
                    unsigned char hbuf[4];
                    for (int i = 0; i < 4; ++i)
                    {
                        hbuf[i] = (unsigned char)((header >> ((3 - i) * 8)) & 0xff);
                    }

                    /* Now write out both header and data, fire and forget. */
                    fwrite(hbuf, sizeof(unsigned char), 4, myfile);
                    fwrite(bodydata, sizeof(unsigned char), bodybytes, myfile);
                }
            }
            count += mpg123_spf(m);
        }
        printf("%s writed\n", fileName);
        free(fileName);
        fclose(myfile);
        nbSong++;
        el = el->suivant;
    }

    free(opts);
    free(buf);
    deleteList(liste);
    mpg123_delete(m);
    mpg123_exit();
    return 0;
}