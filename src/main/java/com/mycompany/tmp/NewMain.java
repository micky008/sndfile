package com.mycompany.tmp;

import java.io.*;

public class NewMain {

    public static void main(String[] args) {
        try {
            
            final int BUFFER = 65535;
            
            // Open the wav file specified as the first argument
            WavFile wavFile = WavFile.openWavFile(new File("C:\\ffmpeg\\carpenter-brut-trilogy.wav"));

            // Get the number of audio channels in the wav file
            //int numChannels = wavFile.getNumChannels();

            // Create a buffer of 100 frames
            long nsc2 = wavFile.getNumFrames();
            
            double[] buffer = new double[nsc2 > Integer.MAX_VALUE ? Integer.MAX_VALUE : (int)nsc2];
            final int SAMPLE = buffer.length / 2;

            int sampleCount;

            double gauche;
            double droite;
            boolean endWrite = false;
            boolean previous0 = false;
            int nbSong = 0;
            do {
                sampleCount = wavFile.readFrames(buffer, SAMPLE);                
                endWrite = false;
                for (int i = 0; i < sampleCount; i++) {
                    gauche = buffer[i * 2];

                    droite = buffer[i * 2 + 1];

                    //System.out.println(gauche + " " + droite);
                    if (gauche == 0f && droite == 0f) {
                        if (!endWrite && !previous0) {
                            endWrite = true;
                            previous0 = true;
                            nbSong++;
                        }
                        continue;
                    }
                    previous0 = false;

                }
            } while (sampleCount > 0);
            // Close the wavFile
            wavFile.close();
            System.out.println("Nb songs : " + nbSong);

            // Output the minimum and maximum value
            //  System.out.printf("Min: %f, Max: %f\n", min, max);
        } catch (Exception e) {
            System.err.println(e);
        }
    }
}
