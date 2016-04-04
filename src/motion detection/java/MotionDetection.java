import com.github.sarxos.webcam.Webcam;
import com.github.sarxos.webcam.ds.ipcam.IpCamAuth;
import com.github.sarxos.webcam.ds.ipcam.IpCamMode;

import javax.imageio.ImageIO;
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * Created by danielon on 2016-01-12.
 */
public class MotionDetection implements WebcamMotionListener {

    public static void main(String[] args) throws IOException {
        t = System.currentTimeMillis();
        new MotionDetection();
        System.out.printf("Total time: %d", System.currentTimeMillis() - t);

    }

    public MotionDetection() throws IOException {
        IpCamDevice ipcam = new IpCamDevice("M3045", "http://192.168.0.90/axis-cgi/mjpg/video.cgi?resolution=320x240", IpCamMode.PUSH, new IpCamAuth("root", "pass"));
        IpCamDriver driver = new IpCamDriver();
        driver.register(ipcam);

        Webcam.setDriver(driver);

        Webcam webcam = Webcam.getDefault();
        WebcamMotionDetector detector = new WebcamMotionDetector(webcam);
        detector.setInterval(0); // one check per 100 ms (10 FPS)
        detector.addMotionListener(this);
        detector.start();

        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        br.readLine();
        System.out.println("Exiting..");
    }

    int i = 0;

    static long t;

    @Override
    public void motionDetected(WebcamMotionEvent wme) {
        Webcam webcam = ((WebcamMotionDetector) wme.getSource()).getWebcam();
        IpCamDevice device = (IpCamDevice) webcam.getDevice();
    }
}
