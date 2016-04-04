using System;


namespace xmlparsing {
	public class MainClass {
		public static void Main(string[] args)
		{
			if(args.Length != 1){
				Console.Error.WriteLine("USAGE: mono Main [file]");
				Environment.Exit(1);
			}


			XMLParser xmlParser = new XMLParser();
			xmlParser.Parse(args[0]);
			xmlParser.Print();	
		}
	}
}