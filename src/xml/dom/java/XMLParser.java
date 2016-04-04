import org.w3c.dom.*;
import org.xml.sax.EntityResolver;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.io.StringReader;

/**
 * Created by danielon on 12/7/15.
 */
public class XMLParser {

    private DocumentBuilder mBuilder;
    private Document document;

    private int deepestNode = -1;

    public XMLParser() throws ParserConfigurationException, IOException, SAXException {
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        mBuilder = factory.newDocumentBuilder();

        //ignore dtd
        mBuilder.setEntityResolver(new EntityResolver() {
            @Override
            public InputSource resolveEntity(String publicId, String systemId)
                    throws SAXException, IOException {
                if (systemId.contains(".dtd")) {
                    return new InputSource(new StringReader(""));
                } else {
                    return null;
                }
            }
        });
    }

    public void parse(File file) throws IOException, SAXException {
        document = mBuilder.parse(file);
        System.out.println("Done with parsing");
    }

    public void print() {
        Element root = document.getDocumentElement();
        traverseTree(root, 0);
        System.out.println("Deepest node: " + deepestNode);
    }

    private void traverseTree(Element element, int depth) {

        deepestNode = Math.max(deepestNode, depth);

        NodeList nodeList = element.getChildNodes();
        for (int i = 0; i < nodeList.getLength(); i++) {
            Node node = nodeList.item(i);

            switch (node.getNodeType()) {
                case Node.ELEMENT_NODE:
                    traverseTree((Element) nodeList.item(i), depth + 1);
                    break;
            }
        }
    }
}
