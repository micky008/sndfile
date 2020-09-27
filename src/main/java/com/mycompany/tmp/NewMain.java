package com.mycompany.tmp;

import com.meganerd.CArrayFloat;
import com.meganerd.SF_INFO;
import com.meganerd.SWIGTYPE_p_SNDFILE_tag;
import com.meganerd.libsndfile;
import java.io.*;
import static jdk.internal.jshell.tool.StopDetectingInputStream.State.BUFFER;
import static jdk.nashorn.internal.objects.ArrayBufferView.buffer;

public class NewMain {

    static {
        System.loadLibrary("libsndfile_wrap");
        System.loadLibrary("libsndfile-1");
    }

    public static void main(String[] args) {
        try {
            File wavFile = new File("C:\\ffmpeg\\carpenter-brut-trilogy.wav");
            //File wavFile = new File("C:\\ffmpeg\\test.wav");
            SF_INFO sndInfo = new SF_INFO();
            SWIGTYPE_p_SNDFILE_tag sndFile = libsndfile.sf_open(wavFile.getAbsolutePath(), libsndfile.SFM_READ, sndInfo);
            // Open the wav file specified as the first argument
            //WavFile wavFile = WavFile.openWavFile();

            long nsc2 = sndInfo.getFrames();            // 1 frame = value gauche + droite combiné.
            System.out.println("nombe de frames = " + nsc2);
            System.out.println("temps en seconde = " + nsc2 / 44100);

            //double[] buffer = new double[nsc2 * wavFile.getNumChannels() > Integer.MAX_VALUE ? Integer.MAX_VALUE : (int) nsc2 * wavFile.getNumChannels()]; //*wavFile.getNumChannels() pour stocker la valeure gauche et droite décodé
            int BUFFER = nsc2 * 2 > Integer.MAX_VALUE ? Integer.MAX_VALUE : (int) nsc2 * 2;
            CArrayFloat farray = new CArrayFloat(BUFFER);
            final long SAMPLE = BUFFER / 2;

            long sampleCount = 1;

            float gauche = 0;
            float droite = 0;
            int nbSong = 0;

            while (sampleCount > 0) {
                //sampleCount = wavFile.readFrames(buffer, SAMPLE);
                sampleCount = libsndfile.sf_readf_float(sndFile, farray.cast(), SAMPLE);
                System.out.println("sampleCount lu=" + sampleCount);
                int previous = 0;
                for (int i = 0; i < sampleCount; i++) {
                    gauche = farray.getitem(i * 2);
                    if (sndInfo.getChannels() == 2) {
                        droite = farray.getitem(i * 2 + 1);
                    }
                    if ((gauche == 0 && droite == 0) || (i + 1 >= sampleCount)) { // (i + 1 == sampleCount) => fin de chanson
                        int nbBlank = 0;
                        for (; i < sampleCount; i++) { //detecter le nombre de silance
                            gauche = farray.getitem(i * 2);
                            if (sndInfo.getChannels() == 2) {
                                droite = farray.getitem(i * 2 + 1);
                            }
                            if (gauche == 0 && droite == 0) {
                                nbBlank++;
                            } else {
                                break;
                            }
                        }
                        if (nbBlank > 18000 || (i + 1 >= sampleCount)) { //21000 < 500ms  || fin de chanson
                            nbSong++;
                            File file = new File("C:\\ffmpeg\\test\\carpenter-brut-trilogy-" + nbSong + ".wav");
                            file.createNewFile();
                            SF_INFO sndInfo2 = getNewSfInfo(sndInfo, i-previous );
                            CArrayFloat outbuf = memcpy(farray, previous, i-previous);
                            SWIGTYPE_p_SNDFILE_tag sndFile2 = libsndfile.sf_open(file.getAbsolutePath(), libsndfile.SFM_WRITE, sndInfo2);
                            libsndfile.sf_write_float(sndFile2, outbuf.cast(), (i-previous)*2 );                            
                            libsndfile.sf_close(sndFile2);
                            outbuf.delete();
                            sndInfo2.delete();
                            //WavFile wf = WavFile.newWavFile(file, wavFile.getNumChannels(), i - previous, wavFile.getValidBits(), wavFile.getSampleRate());
                            //wf.writeFrames(buffer, previous, i - previous);
                            //wf.close();
                            previous = i;
                        }
                    }
                }
            }
            //wavFile.close();
            libsndfile.sf_close(sndFile);
            sndInfo.delete();
            System.out.println("Nb songs : " + nbSong);

        } catch (Exception e) {
            System.err.println(e);
        }
    }

    private static CArrayFloat memcpy(CArrayFloat farray, int debut, int combien) {
        CArrayFloat res = new CArrayFloat(combien*2);
        for (int i = 0; i < combien; i++) {
            float f1 = farray.getitem((debut+i)*2);
            float f2 = farray.getitem((debut+i)*2+1);
            res.setitem(i*2, f1);
            res.setitem(i*2+1, f2);
        }
        return res;
    }

    private static SF_INFO getNewSfInfo(SF_INFO sfi, long nbFrame) {
        SF_INFO sndInfo2 = new SF_INFO();
        sndInfo2.setChannels(sfi.getChannels());
        sndInfo2.setFormat(sfi.getFormat());
        sndInfo2.setFrames(nbFrame);
        sndInfo2.setSamplerate(sfi.getSamplerate());
        sndInfo2.setSections(sfi.getSections());
        sndInfo2.setSeekable(sfi.getSeekable());
        return sndInfo2;
    }

}
