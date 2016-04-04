import javax.xml.parsers.*;
import org.xml.sax.*;
import org.xml.sax.helpers.*;

import java.util.*;
import java.io.*;

public class XMLParser extends DefaultHandler {

	private static int currentNode = 0;
	private static int deepestNode = -1;

	public static void main(String[] args) throws Exception {
		SAXParserFactory factory = SAXParserFactory.newInstance();
		factory.setNamespaceAware(true);
		SAXParser parser = factory.newSAXParser();	

		XMLReader xmlReader = parser.getXMLReader();
		xmlReader.setContentHandler(new XMLParser());
		
		xmlReader.parse(args[0]);
		System.out.printf("Deepest node: %d\n", deepestNode);
	}
	
	@Override
	public void startElement(String namespaceURI,
              	          		String localName,
             		            String qName, 
             		            Attributes atts) throws SAXException {
		++currentNode;
		deepestNode = Math.max(deepestNode, currentNode);
	}
	@Override
	public void endElement(String namespaceURI,
                     		    String localName,
                 		        String qName) throws SAXException {
		--currentNode;
	}			

	@Override
	public void startDocument() throws SAXException {}

	@Override
	public void endDocument() throws SAXException {}

}