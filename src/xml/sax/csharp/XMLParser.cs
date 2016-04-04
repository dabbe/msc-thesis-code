using System;
using System.Xml;



public class XMLParser
{
	private static int deepestNode = -1;
	private static int currentNode = 0;
	

	public static void Main(string[] args){
		new XMLParser(args[0]);

		Console.WriteLine("Deepest Node: " + deepestNode);

	}	

	public XMLParser(string input){
		using(XmlReader reader = XmlReader.Create(input))
		{
			while(reader.Read()){
				switch(reader.NodeType)
				{
					case XmlNodeType.Element:
						if(!reader.IsEmptyElement)
							startElement();
						break;
					case XmlNodeType.EndElement:
						endElement();
						break;
				}
			}
		}


	}

	private void startElement(){
		++currentNode;
		deepestNode = Math.Max(currentNode, deepestNode);
	}

	private void endElement(){
		--currentNode;
	}
}