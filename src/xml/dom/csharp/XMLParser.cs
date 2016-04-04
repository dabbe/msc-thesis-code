using System;
using System.Xml;
using System.Collections;
using System.IO;

namespace xmlparsing {
	public class XMLParser {

		private XmlDocument document;
		private int deepestNode = -1;

		public XMLParser()
		{
			document = new XmlDocument();
			document.XmlResolver = null;
		}

		public void Parse(string filename)
		{
			document.Load(filename);
		}

		public void Print(){
			IEnumerator enumerator = document.GetEnumerator();

			while(enumerator.MoveNext()){
				traverseTree((XmlNode) enumerator.Current, 0);
			}

			Console.WriteLine("deepestNode: " + (deepestNode - 1));
		}

		private void traverseTree(XmlNode node, int depth){

			deepestNode = Math.Max(deepestNode, depth);

			if(node.NodeType == XmlNodeType.Element){
				foreach (XmlNode n in node.ChildNodes){
            		traverseTree(n, depth + 1);
				}
			}
		}
	}
}