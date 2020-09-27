package com.mycompany.tmp;

import java.io.File;
import java.io.FileInputStream;
import javazoom.jl.decoder.Bitstream;
import javazoom.jl.decoder.Decoder;
import javazoom.jl.decoder.Header;
import javazoom.jl.decoder.SampleBuffer;

/**
 *
 * @author Michael
 */
public class NewMain2 {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws Exception {
        File file = new File("c:/ffmpeg/carpenter-brut-trilogy.mp3");
        FileInputStream fis = new FileInputStream(file);
        Bitstream stream = new Bitstream(fis);
        Decoder decoder = new Decoder();
        SampleBuffer buf = null;
        boolean run = true;
        while (run) {

            Header h = stream.readFrame();
            
            buf = (SampleBuffer) decoder.decodeFrame(h, stream);

            short[] frames = buf.getBuffer();
            int t = buf.getBufferLength();
            System.out.println(buf.getBufferLength());
            buf.clear_buffer();
        }

//            for (int i = 0; i < t ; i++) {
//                System.out.println(frames[i] + " " + frames[i*2+1]);
//            }
        System.out.println("fin");
        stream.closeFrame();
        stream.close();
    }

}
