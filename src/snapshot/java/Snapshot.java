import java.io.*;
import java.net.*;
 
class Snapshot {
	public void takeSnapshot(String camIp) {
		try {
			Authenticator.setDefault (new Authenticator() {
			    protected PasswordAuthentication getPasswordAuthentication() {
			        return new PasswordAuthentication ("root", "pass".toCharArray());
			    }
			});

			URL url = new URL("http://" + camIp + "/jpg/1/image.jpg");
			HttpURLConnection conn = (HttpURLConnection) url.openConnection();
			InputStream is = conn.getInputStream();

			String imgFile = "image.jpg";

			FileOutputStream out = new FileOutputStream(imgFile);

			byte[] buf = new byte[1024];
			int len = is.read(buf);
			while (len != -1) {
			    out.write(buf, 0, len);
			    len = is.read(buf);
			}

			is.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void uploadToFTP(String hostIp, String user, String pass) {
		String ftpUrl = "ftp://" + user + ":" + pass + "@" + hostIp + "/image.jpg";
		String filePath = "/tmp/image.jpg";

		try {
		    URL url = new URL(ftpUrl);
		    URLConnection conn = url.openConnection();
		    OutputStream outputStream = conn.getOutputStream();
		    FileInputStream inputStream = new FileInputStream(filePath);

		    byte[] buffer = new byte[1024];
		    int bytesRead = -1;
		    while ((bytesRead = inputStream.read(buffer)) != -1) {
		        outputStream.write(buffer, 0, bytesRead);
		    }

		    inputStream.close();
		    outputStream.close();
		} catch (IOException ex) {
		    ex.printStackTrace();
		}
	}

	public static void main(String[] args) {
	
		for(int i = 0; i < 10; i++){
			long time = System.currentTimeMillis();
			Snapshot s = new Snapshot();
			s.takeSnapshot(args[0]);
			long snapTime = System.currentTimeMillis() - time;
			System.out.println("Snapshot saved to disk at " + snapTime + "ms");
			s.uploadToFTP(args[1], args[2], args[3]);
			long ftpTime = System.currentTimeMillis() - time;
			System.out.println("Snapshot transferred to FTP at " + ftpTime + "ms");
		}
	}
}
