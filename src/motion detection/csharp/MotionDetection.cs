#define MONO

using System;
using System.Drawing;
using AForge;
using System.IO;
using AForge.Video;
using AForge.Imaging;

using System.Drawing.Imaging;

public class Test {
	public static void Main(string[] args){
		try{
			new Test();
		} catch(Exception e){}
	}

	MJPEGStream stream;
	MotionDetector detector;

	public Test(){

        detector = new MotionDetector(new TwoFramesDifferenceDetector());

		try{
		    	stream = new MJPEGStream("http://localhost/axis-cgi/mjpg/video.cgi?resolution=320x240");
			stream.Login = "root";
	    		stream.Password = "pass";

			stream.NewFrame += new Dennis.NewFrameEventHandler(video_NewFrame);

			stream.Start();

		}catch(Exception e){}
	}


	private void video_NewFrame( object sender, NewFrameEventArgs eventArgs )
		{
		Bitmap bitmap = (Bitmap) eventArgs.Frame.Clone();
		detector.ProcessFrame(bitmap);

		bitmap.Dispose();
		bitmap = null;

	}
}
