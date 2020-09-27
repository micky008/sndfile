package com.mycompany.tmp;

import java.io.*;

public class NewMain {

    public static void main(String[] args) {
        try {

            // Open the wav file specified as the first argument
            WavFile wavFile = WavFile.openWavFile(new File("C:\\ffmpeg\\carpenter-brut-trilogy.wav"));

            long nsc2 = wavFile.getNumFrames();            // 1 frame = value gauche + droite combiné.
            System.out.println("nombe de frames = " + nsc2);
            System.out.println("temps en seconde = " + nsc2 / 44100);

            double[] buffer = new double[nsc2 * 2 > Integer.MAX_VALUE ? Integer.MAX_VALUE : (int) nsc2 * 2]; //x2 pour stocker la valeure gauche et droite décodé
            final int SAMPLE = buffer.length / 2;

            int sampleCount = 1;

            double gauche;
            double droite;
            int nbSong = 1;
            while (sampleCount > 0) {
                sampleCount = wavFile.readFrames(buffer, SAMPLE);
                System.out.println("sampleCount lu=" + sampleCount);
                for (int i = 0; i < sampleCount; i++) {
                    gauche = buffer[i * 2];
                    droite = buffer[i * 2 + 1];
                    if (gauche == 0f && droite == 0f) {
                        int nbBlank = 0;
                        for (int j = i; j < sampleCount; j++, i++) {
                            gauche = buffer[j * 2];
                            droite = buffer[j * 2 + 1];
                            if (gauche == 0f && droite == 0f) {
                                nbBlank++;
                            } else {
                                break;
                            }
                        }
                        if (nbBlank > 21000) { //21000 < 500ms                       
                            nbSong++;
                        }
                    }
                }
            }
            wavFile.close();
            System.out.println("Nb songs : " + nbSong);

        } catch (Exception e) {
            System.err.println(e);
        }
    }

    private static boolean have0after(int i, double buffer[], int limit) {
        if (i >= limit) {
            return false;
        }
        int j = i + 1;
        double gauche = buffer[j * 2];
        double droite = buffer[j * 2 + 1];
        return gauche == 0f && droite == 0f;

    }
}
