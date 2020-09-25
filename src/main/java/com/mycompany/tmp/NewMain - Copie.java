package com.mycompany.tmp;

import com.meganerd.CArrayFloat;
import com.meganerd.SF_INFO;
import com.meganerd.SWIGTYPE_p_SNDFILE_tag;
import com.meganerd.libsndfile;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

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
        boolean have0 = false;
        int nbSong = 1;
        int nbframe = BUFFER / sndInfo.getChannels();

        SWIGTYPE_p_SNDFILE_tag sndOutFile = null;
        int outcount = 0;
        long debut = 0;
        long fin = 0;
        List<Pair<Long, Long>> liste = new ArrayList();
        Map<Integer, List> map = new LinkedHashMap();
        map.put(1, liste);
        do {
            sampleCount = libsndfile.sf_readf_float(sndFile, farray.cast(), nbframe);
            have0 = false;
            for (int i = 0; i < sampleCount; i++) {
                gauche = farray.getitem(i * sndInfo.getChannels());
                if (sndInfo.getChannels() > 1) {
                    droite = farray.getitem(i * sndInfo.getChannels() + 1);
                }

                 System.out.println(gauche + " " + droite);
                if (gauche == 0f && droite == 0f) {
                    have0 = true;
                    fin = i;
                    liste.add(Pair.of(debut, fin));
                    nbSong++;
                    liste = new ArrayList<>();
                    map.put(new Integer(nbSong), liste);
                    while ((gauche == 0f && droite == 0f) && (i < sampleCount)) {
                        ++i;
                        gauche = farray.getitem(i * sndInfo.getChannels());
                        if (sndInfo.getChannels() > 1) {
                            droite = farray.getitem(i * sndInfo.getChannels() + 1);
                        }
                    }
                    --i;
                    if (i >= sampleCount) {
                        fin = sampleCount;
                        liste.add(Pair.of(debut, fin));
                    } else {
                        debut = i;
                    }
                }
            }
            if (!have0){
                liste.add(Pair.of(0l, sampleCount));
            }
        } while (sampleCount > 0);

        libsndfile.sf_close(sndFile);

        System.out.println("nb song=" + nbSong);
        System.out.println(map.size());
    }

    private static CArrayFloat memcpy(CArrayFloat src, int nbChannel, int debut, int fin) {
        CArrayFloat dest = new CArrayFloat(fin - debut);
        for (int i = debut, j = 0; i < fin; i++, j++) {
            dest.setitem(j, src.getitem(i));
        }
        return dest;
    }

}
