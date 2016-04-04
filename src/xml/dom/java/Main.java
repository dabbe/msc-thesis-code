import org.xml.sax.SAXException;

import javax.xml.parsers.ParserConfigurationException;
import java.io.File;
import java.io.IOException;

/**
 * Code taken from:
 * http://www.tutorialspoint.com/java_xml/java_dom_parse_document.htm
 *
 *
 * Code takes in an xml file and parses it.
 * It also prints it recursively
 */
public class Main {

    public static void main(String[] args) {
        if (args.length != 1) {
            System.err.println("USAGE: java XMLParser [file]");
            System.exit(1);
        }

        try {
            XMLParser xmlParser = new XMLParser();
            xmlParser.parse(new File(args[0]));
            xmlParser.print();
        } catch (Exception e) {
            e.printStackTrace();
        }

    }
}
