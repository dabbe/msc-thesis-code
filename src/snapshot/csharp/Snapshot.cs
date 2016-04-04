using System;
using System.Net;
using System.IO;

public class Snapshot {

	public void TakeSnapshot()
	{
		int start = Environment.TickCount;

		WebClient client = new WebClient();
		client.Credentials = new NetworkCredential("root", "pass");
		// Stream s = client.OpenRead("http://" + args[0] + "/jpg/1/image.jpg");
		client.DownloadFile("http://localhost/jpg/1/image.jpg", "image.jpg");

		Console.WriteLine("Screenshot saved to disk at " + (Environment.TickCount - start) + "ms");
	}

	public void UploadToFTP(string host, string user, string pass) 
	{
		using (WebClient client = new WebClient())
		{
    		client.Credentials = new NetworkCredential(user, pass);
    		client.UploadFile("ftp://" + host + "/image.jpg", "image.jpg");
		}
		Console.WriteLine("FTP Uploaded");
	}

	public static void Main(string[] args) 
	{

		for(int i = 0; i < 10; i++){	
			Snapshot snapshot = new Snapshot();

			snapshot.TakeSnapshot();
			snapshot.UploadToFTP(args[0], args[1], args[2]);
		}

	}
}
