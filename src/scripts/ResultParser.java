import java.io.*;
import java.util.*;

public class ResultParser {

	private List<TimedValue> cpu = new ArrayList<TimedValue>();
	private List<TimedValue> mem = new ArrayList<TimedValue>();
	private String filepath;

	public static void main(String[] args){
		if(args.length != 1){
			System.out.println("Usage: java ResultParser FILENAME");
			System.exit(1);
		}

		ResultParser resultParser = new ResultParser();
		resultParser.parse(args[0]);
		resultParser.saveToFile();
	}

	public void parse(String filepath){
		this.filepath = filepath;

		BufferedReader br = null;

		try{

			br = new BufferedReader(new FileReader(filepath));
			String line;
			while((line = br.readLine()) != null){
				readLine(line.substring(11));
			}

		} catch(IOException e){

			e.printStackTrace();

		} finally {
			try{
				br.close();
			} catch(IOException e){

			}
		}
	}

	private void readLine(String line){
		char c = line.charAt(13);

		TimedValue tv = new TimedValue();
		tv.time = getNicelyFormattedTime(line.substring(0, 12));
		tv.value = line.substring(25);
		

		switch(c){
			case 'c':
				cpu.add(tv);
				break;
			case 'm':
				mem.add(tv);
				break;
			default:
				System.out.println("THIS SHOULD REALLY NOT HAPPEN!");
				System.exit(-1);
		}
	}

	private void saveToFile(){
		try{
			printListToFile(cpu, filepath + "_cpu.output");
			printListToFile(mem, filepath + "_mem.output");			
		} catch(IOException e){
			e.printStackTrace();
		}

	}

	private String getNicelyFormattedTime(String time){

		long t = 0;

		// hour
		int hour = Integer.parseInt(time.substring(0, 2));
		t += hour * 60 * 60 * 1000;
		
		// minute
		int minute = Integer.parseInt(time.substring(3, 5));
		t += minute * 60 * 1000;

		
		// second
		int second = Integer.parseInt(time.substring(6, 8));
		t += second * 1000;

		
		//millisec
		int millisec = Integer.parseInt(time.substring(9, 12));
		t += millisec;
		

		return String.valueOf(t);
	}

	private void printListToFile(List<TimedValue> li, String outputPath) throws IOException{

		PrintWriter out = new PrintWriter(new FileWriter(outputPath));

		for(TimedValue tv : li){

			out.print(tv.time);
			out.print(",");
			out.println(tv.value);
		}

		out.close();
	}

	private class TimedValue {
		String time;
		String value;
	}
}
