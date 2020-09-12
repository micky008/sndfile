package com.mycompany.tmp;

import com.meganerd.CArrayFloat;
import com.meganerd.SF_INFO;
import com.meganerd.SWIGTYPE_p_SNDFILE_tag;
import com.meganerd.libsndfile;

/**
 *
 * @author Michael
 */
public class NewMain {

    private static final int BUFFER = 4096;

    static {
        System.loadLibrary("libsndfile_wrap");
        System.loadLibrary("libsndfile-1");
    }
// System.out.print(farray.getitem(i * sndInfo.getChannels() + c)); => formule pour convertir les sample en HR

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {

        String filePath = "C:/ffmpeg/test2.wav";
        String outFolder = "C:/ffmpeg/";

        SF_INFO sndInfo = new SF_INFO();
        SWIGTYPE_p_SNDFILE_tag sndFile = libsndfile.sf_open(filePath, libsndfile.SFM_READ, sndInfo);
        int err = libsndfile.sf_error(sndFile);
        System.out.println(libsndfile.sf_error_number(err));
        System.out.println(sndInfo.getChannels());
        System.out.println(sndInfo.getFormat());
        System.out.println(sndInfo.getFrames());
        System.out.println(sndInfo.getSamplerate());
        System.out.println(sndInfo.getSections());
        System.out.println(sndInfo.getSeekable());
        System.out.println();
        CArrayFloat farray = new CArrayFloat(BUFFER);
        long sampleCount = 0;
        float gauche = 0;
        float droite = 0;
        boolean endWrite = false;
        int nbSong = 1;
        int nbframe = BUFFER / sndInfo.getChannels();
        boolean previous0 = false;
        SWIGTYPE_p_SNDFILE_tag sndOutFile = null;
        int outcount = 0;
        do {
            sampleCount = libsndfile.sf_readf_float(sndFile, farray.cast(), nbframe);
            endWrite = false;
            for (int i = 0; i < sampleCount; i++) {
                gauche = farray.getitem(i * sndInfo.getChannels());
                if (sndInfo.getChannels() > 1) {
                    droite = farray.getitem(i * sndInfo.getChannels() + 1);
                }
                System.out.println(gauche + " " + droite);
                if (gauche == 0f && droite == 0f) {
                    if (!endWrite && !previous0) {
                        endWrite = true;
                        previous0 = true;
                        nbSong++;
                    }
                    continue;
                }
                previous0 = false;
                if (sndOutFile == null) {
                    sndOutFile = libsndfile.sf_open(outFolder + nbSong + "-track.wav", libsndfile.SFM_WRITE, sndInfo);
                }
                libsndfile.sf_writef_float(sndOutFile, ptr, i);

            }
        } while (sampleCount > 0);

        libsndfile.sf_close(sndFile);

        System.out.println("nb song=" + nbSong);
    }

}
