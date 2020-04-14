/*************************************************************************
 *  Compilation:  javac In.java
 *  Execution:    java In
 *  
 *  Reads in data of various types from: stdin, file, URL.
 *
 *  Reads in a string, rest of line, or rest of file and returns
 *  it as a String or null if there is no more data.
 *
 *  The client can use Integer.parseInt(in.readString()) to read
 *  in an int.
 *
 *  Typically this class is used with another client, but main()
 *  provides a testing routine.
 *
 *  % java In
 *  Test if In.java works.
 *  Second line.
 *  
 *  Test 
 *  if
 *  In.java 
 *  works.
 *  Second
 *  line.
 *
 *  Test if In.java works.
 *  Second line.
 *
 *************************************************************************/


import java.net.URLConnection;
import java.net.URL;
import java.net.Socket;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;


class In {
   private BufferedReader br;

   // system independent
   private final static String NEWLINE = System.getProperty("line.separator");

   // for stdin
   public In() {
      InputStreamReader isr = new InputStreamReader(System.in);
      br = new BufferedReader(isr);
   }

   // for stdin
   public In(Socket socket) {
      try {
         InputStream is        = socket.getInputStream();
         InputStreamReader isr = new InputStreamReader(is);
         br                    = new BufferedReader(isr);
      } catch (IOException ioe) { ioe.printStackTrace(); }
   }
 
   // for URLs
   public In(URL url) {
      try {
         URLConnection site    = url.openConnection();
         InputStream is        = site.getInputStream();
         InputStreamReader isr = new InputStreamReader(is);
         br                    = new BufferedReader(isr);
      } catch (IOException ioe) { ioe.printStackTrace(); }
   }

   // for files and web pages
   public In(String s) {

      // try to read file from local file system
      try {
         URL url = getClass().getResource(s);       // for files, even if included in a jar
         if (url == null) url = new URL(s);         // no file found, try a URL

         URLConnection site    = url.openConnection();
         InputStream is        = site.getInputStream();
         InputStreamReader isr = new InputStreamReader(is);
         br = new BufferedReader(isr);
      } catch(IOException ioe) { ioe.printStackTrace(); }
   }


   // note read() returns -1 if EOF
   private int readChar() {
      int c = -1;
      try { c = br.read(); }
      catch(IOException ioe) { ioe.printStackTrace(); }
      return c;
   }

   // read a token - delete preceding whitespace and one trailing whitespace character
   public String readString() {
       int c;
       while ((c = readChar()) != -1)
          if (!Character.isWhitespace((char) c)) break;

       if (c == -1) return null;
 
       String s = "" + (char) c;
       while ((c = readChar()) != -1)
          if (Character.isWhitespace((char) c)) break;
          else s += (char) c;

       return s;
   }

   // return rest of line as string and return it, not including newline 
   public String readLine() {
       String s = null;
       try { s = br.readLine(); }
       catch(IOException ioe) { ioe.printStackTrace(); }
       return s;
   }

   // return rest of input as string, use StringBuffer to avoid quadratic run time
   public String readAll() {
       StringBuffer sb = new StringBuffer();
       String s;
       while ((s = readLine()) != null)
          sb.append(s).append(NEWLINE);
       return sb.toString();
   }

   public void close() { 
       try { br.close(); }
       catch (IOException e) { e.printStackTrace(); }
   }


   // This method is just here to test the class
   public static void main (String args[]) {
      In in;
      String s;

      // read from a URL
      in = new In("http://www.cs.princeton.edu/IntroCS/24inout/InTest.txt");
      System.out.println(in.readAll());

      // read one string at a time from URL
      in = new In("http://www.cs.princeton.edu/IntroCS/24inout/InTest.txt");
      while ((s = in.readString()) != null)
         System.out.println(s);
      System.out.println();

      // read one line at a time from file
      in = new In("InTest.txt");
      while ((s = in.readLine()) != null)
         System.out.println(s);
      System.out.println();
   }

}
