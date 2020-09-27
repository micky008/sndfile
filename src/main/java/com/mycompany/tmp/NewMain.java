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

            double[] buffer = new double[nsc2 * wavFile.getNumChannels() > Integer.MAX_VALUE ? Integer.MAX_VALUE : (int) nsc2 * wavFile.getNumChannels()]; //*wavFile.getNumChannels() pour stocker la valeure gauche et droite décodé
            final int SAMPLE = buffer.length / 2;

            int sampleCount = 1;

            double gauche = 0d;
            double droite = 0d;
            int nbSong = 0;

            while (sampleCount > 0) {
                sampleCount = wavFile.readFrames(buffer, SAMPLE);
                System.out.println("sampleCount lu=" + sampleCount);
                int previous = 0;
                for (int i = 0; i < sampleCount; i++) {
                    gauche = buffer[i * 2];
                    if (wavFile.getNumChannels() == 2) {
                        droite = buffer[i * 2 + 1];
                    }
                    if ((gauche == 0d && droite == 0d) || (i + 1 >= sampleCount)) { // (i + 1 == sampleCount) => fin de chanson
                        int nbBlank = 0;
                        for (int j = i; j < sampleCount; j++, i++) { //detecter le nombre de silance
                            gauche = buffer[j * wavFile.getNumChannels()];
                            if (wavFile.getNumChannels() == 2) {
                                droite = buffer[j * wavFile.getNumChannels() + 1];
                            }
                            if (gauche == 0d && droite == 0d) {
                                nbBlank++;
                            } else {
                                break;
                            }
                        }
                        if (nbBlank > 21000 || (i + 1 >= sampleCount)) { //21000 < 500ms  || fin de chanson
                            nbSong++;
                            File file = new File("C:\\ffmpeg\\test\\carpenter-brut-trilogy-" + nbSong + ".wav");
                            file.createNewFile();
                            WavFile wf = WavFile.newWavFile(file, wavFile.getNumChannels(), i - previous, wavFile.getValidBits(), wavFile.getSampleRate());
                            wf.writeFrames(buffer, previous, i - previous);
                            wf.close();
                            previous = i;
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
}
