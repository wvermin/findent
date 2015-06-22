/* $Id: Jfindent.java 65 2015-06-22 10:01:15Z willem_vermin $
 *
 * jfindent: a graphical wrapper for findent, my Fortran indenting program.
 * Take care that jfindent can find the findent executable.
 *
 * This is my first significant java project, I am glad to receive 
 * suggestions for improvements.
 *
 * This program needs java >= 1.7
 *
 * Willem Vermin
 * wvermin@gmail.com
 *
 * Many thanks for all information I found on many places on the WWW
 */
/*
 * Since this source contains code copied from and inspired from Oracle, I
 * inserted the following:
 *
 * Copyright (c) 1995, 2014, Oracle and/or its affiliates. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *   - Neither the name of Oracle or the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

import javax.swing.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.filechooser.*;
import java.io.*;
import java.text.*;
import java.lang.ProcessBuilder.Redirect;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import java.beans.*;

import javax.swing.*;


public class Jfindent {
   static final String VERSION = "1.0";
   static final int DOHELP     = 1;
   static final int DOVERSION  = 2;

   public static final class OsUtils {
      private static String OS             = null;
      private static String configFileName = null;
      private static String newLine        = null;

      public static String getOsName() {
	 if(OS == null) { 
	    OS = System.getProperty("os.name"); }
	 return OS;
      }
      public static boolean isWindows() {
	 return getOsName().startsWith("Windows");
      }
      public static String getConfigFileName() {
	 if(configFileName == null) {
	    if (isWindows()) {
	       configFileName = System.getProperty("user.home")+".\\jfindent.data";
	    } else {
	       configFileName = System.getProperty("user.home")+"/.jfindentrc";
	    }
	 }
	 return configFileName;
      }
      public static String getNewLine() {
	 if(newLine == null) {
	    newLine = System.getProperty("line.separator");
	 }
	 return newLine;
      }
   }

   class IndentOptions extends JPanel implements ChangeListener {
      static final int minIndent = 0;
      static final int maxIndent = 8;

      JSpinner spinner;
      public IndentOptions() {
	 JLabel indentLabel = new JLabel("Indent:");
	 indentLabel.setToolTipText("specify indentation");

	 if (indentParm < minIndent)
	    indentParm = minIndent;
	 if (indentParm >maxIndent)
	    indentParm = maxIndent;

	 SpinnerModel spm = new SpinnerNumberModel(indentParm,
	       minIndent, maxIndent,1);
	 spinner = new JSpinner(spm);
	 spinner.setEditor(new JSpinner.DefaultEditor(spinner));

	 spinner.addChangeListener(this);
	 add(indentLabel);
	 add(spinner);
      }

      public void stateChanged(ChangeEvent e) {

	 JSpinner source = (JSpinner)e.getSource();
	 indentParm = (int)source.getValue();
	 callFindentPreview();
	 writeConfig();
      }
   }

   class FormatOptions extends JPanel implements ActionListener {

      public FormatOptions() {

	 JLabel formatLabel = new JLabel("Input form:");
	 JRadioButton fixedButton = new JRadioButton("fixed");
	 fixedButton.setToolTipText("assume input is in fixed form");
	 JRadioButton freeButton  = new JRadioButton("free");
	 freeButton.setToolTipText("assume input is in free form");
	 JRadioButton autoButton  = new JRadioButton("auto");
	 autoButton.setToolTipText("let findent find out the form");

	 fixedButton.setActionCommand("fixed");
	 freeButton.setActionCommand("free");
	 autoButton.setActionCommand("auto");

	 fixedButton.addActionListener(this);
	 freeButton.addActionListener(this);
	 autoButton.addActionListener(this);

	 ButtonGroup fixfreeGroup = new ButtonGroup();
	 fixfreeGroup.add(autoButton);
	 fixfreeGroup.add(fixedButton);
	 fixfreeGroup.add(freeButton);

	 autoButton.setSelected(true);

	 switch (fixedfreeParm) {
	    case "fixed": fixedButton.setSelected(true); break;
	    case "free":  freeButton.setSelected(true); break;
	 }

	 add(formatLabel);
	 add(autoButton);
	 add(freeButton);
	 add(fixedButton);
      }

      public void actionPerformed(ActionEvent e) {
	 String s = e.getActionCommand();
	 switch (s) {
	    case "auto":  fixedfreeParm = "auto";  break;
	    case "free":  fixedfreeParm = "free";  break;
	    case "fixed": fixedfreeParm = "fixed"; break;
	 }
	 callFindentPreview();
	 writeConfig();
      }
   }

   class ConvertOption extends JPanel implements ActionListener {
      public ConvertOption() {

	 JLabel convertLabel = new JLabel("Fixed->free:");
	 JRadioButton yesButton = new JRadioButton("yes");
	 JRadioButton noButton  = new JRadioButton("no");

	 yesButton.setActionCommand("yes");
	 yesButton.setToolTipText("convert from fixed to free form");
	 noButton.setActionCommand("no");
	 noButton.setToolTipText("do not convert from fixed to free form");

	 yesButton.addActionListener(this);
	 noButton.addActionListener(this);

	 ButtonGroup yesnoGroup = new ButtonGroup();
	 yesnoGroup.add(noButton);
	 yesnoGroup.add(yesButton);

	 noButton.setSelected(true);

	 if (convertParm) {
	    yesButton.setSelected(true);
	 }

	 add(convertLabel);
	 add(noButton);
	 add(yesButton);
      }
      public void actionPerformed(ActionEvent e) {
	 String s = e.getActionCommand();
	 switch (s) {
	    case "yes": convertParm = true;  break;
	    case "no":  convertParm = false; break;
	 }
	 callFindentPreview();
	 writeConfig();
      }
   }

   class ExtraOptions extends JPanel implements ActionListener {
      JTextField extraText;
      public ExtraOptions() {

	 JLabel extraLabel = new JLabel("Extra options:");
	 extraText         = new JTextField(extraParm,40);
	 extraText.setToolTipText("type here extra options for findent");
	 extraText.setActionCommand("extra");
	 extraText.addActionListener(this);
	 extraText.setFont(new Font(Font.MONOSPACED,Font.BOLD,14));

	 JButton enterButton   = new JButton("enter");
	 JButton clearButton   = new JButton("clear extra options");
	 enterButton.addActionListener(this);
	 enterButton.setActionCommand("enter");
	 clearButton.addActionListener(this);
	 clearButton.setActionCommand("clear");
	 add(extraLabel);
	 add(extraText);
	 add(enterButton);
	 add(clearButton);
      }

      public void actionPerformed(ActionEvent e) {
	 switch (e.getActionCommand()){
	    case "clear":   extraParm = "";
			    extraText.setText(extraParm); 
			    break;
	    case "enter": 
	    case "extra":   extraParm = extraText.getText();
			    break;
	 }
	 callFindentPreview();
	 writeConfig();
      }
   }

   class PreviewOption extends JPanel implements ActionListener {
      public PreviewOption() {

	 JLabel previewLabel    = new JLabel("Preview:");
	 JRadioButton yesButton = new JRadioButton("yes");
	 yesButton.setToolTipText("show preview of indented selected file");
	 JRadioButton noButton  = new JRadioButton("no");
	 noButton.setToolTipText("do not show preview of indented selected file");

	 yesButton.setActionCommand("yes");
	 noButton.setActionCommand("no");

	 yesButton.addActionListener(this);
	 noButton.addActionListener(this);

	 ButtonGroup yesnoGroup = new ButtonGroup();
	 yesnoGroup.add(yesButton);
	 yesnoGroup.add(noButton);

	 yesButton.setSelected(true);

	 if (!previewParm) {
	    noButton.setSelected(true);
	 }

	 add(previewLabel);
	 add(yesButton);
	 add(noButton);
      }

      public void actionPerformed(ActionEvent e) {
	 String s = e.getActionCommand();
	 String eoln = OsUtils.getNewLine();
	 switch (s) {
	    case "yes": previewParm = true;  break;
	    case "no":  log.setText("Preview disabled"+eoln);
			previewParm = false; break;
	 }
	 writeConfig();
	 callFindentPreview();
      }
   }

   class Preview implements PropertyChangeListener {
      public Preview() {
      }
      public void propertyChange(PropertyChangeEvent evt) {
	 if (JFileChooser.SELECTED_FILE_CHANGED_PROPERTY == evt.getPropertyName()) {
	    if (evt.getNewValue() != null) {
	       inFile = new File(evt.getNewValue().toString());
	       if (inFile != null) {
		  callFindentPreview();
		  fcfolderParm = fc.getCurrentDirectory().getAbsolutePath();
		  writeConfig();
	       }
	    }
	 }
      }
   }


   static class PipeFromFile implements Runnable {

      BufferedReader inputFile;
      int            maxCount;
      int            counterIn;
      BufferedWriter bw;
      boolean        canRun = true;
      String         errmsg;
      static         String newline = System.getProperty("line.separator");

      public PipeFromFile(String fileName, BufferedWriter bw, int maxCount){

	 errmsg        = null;
	 this.maxCount = maxCount;
	 this.bw       = bw;
	 counterIn     = 0;

	 try{
	    inputFile = new BufferedReader(
		  new InputStreamReader(new FileInputStream(fileName)));
	 } catch (Exception e) {
	    errmsg += newline + "Not found: " + fileName;
	    canRun = false;
	 }
      }

      public void run(){

	 String currInputLine;
	 if (! canRun){
	    errmsg += newline + "Cannot run";
	    try{
	       bw.close();
	    } catch (Exception e) {
	    }
	    return;
	 }
	 try{
	    while((currInputLine = inputFile.readLine()) != null) {
	       counterIn++;
	       if (maxCount > 0 && counterIn > maxCount){
		  counterIn--;
		  break;
	       }
	       bw.write(currInputLine);
	       bw.newLine();
	    }
	    bw.close();
	    inputFile.close();
	    return;
	 } catch (Exception e) {
	    errmsg += newline + "Something wrong";
	    try{
	       bw.close();
	       inputFile.close();
	    } catch (Exception e1){
	    }
	 }
      }

      public int getCounterIn(){
	 return counterIn;
      }

      public String getErrmsg(){
	 return errmsg;
      }
   }

   public static class FileFormat {
      public enum FileType { WINDOWS, UNIX, MAC, UNKNOWN }

      private static final char CR = '\r';
      private static final char LF = '\n';

      public static FileType discover(String fileName) throws IOException {    

	 Reader reader = new BufferedReader(new FileReader(fileName));
	 FileType result = discover(reader);
	 reader.close();
	 return result;
      }

      private static FileType discover(Reader reader) throws IOException {
	 int c;
	 while ((c = reader.read()) != -1) {
	    switch(c) {        
	       case LF: return FileType.UNIX;
	       case CR: {
			   if (reader.read() == LF) {
			      return FileType.WINDOWS;
			   }
			   return FileType.MAC;
	       }
	       default: continue;
	    }
	 }
	 return FileType.UNKNOWN;
      }
   }

   JFileChooser     fc;
   static JTextArea log;
   static JScrollPane      logScrollPane;
   FormatOptions    formatPanel;
   IndentOptions    indentPanel;
   ConvertOption    convertPanel;
   PreviewOption    previewPanel;
   ExtraOptions     extraPanel;
   static File      inFile     = null;
   static String    prevInFile = null;

   final static boolean MULTICOLORED = false;

   //Specify the look and feel to use.  Valid values:
   //null (use the default), "Metal", "System", "Motif", "GTK+"
   final static String LOOKANDFEEL = "Metal";

   JPanel mainPane;

   static String  extraParm;         // extra parameters for findent
   static String  fixedfreeParm;     // -ifixed -ifree -iauto
   static String  fcfolderParm;      // last used folder
   static String  findentParm;       // path to findent
   static boolean convertParm;       // -ofree
   static boolean previewParm;       // preview on or off
   static int     indentParm;        // -i<n>

   public Jfindent() {
      UIManager.put("FileChooser.readOnly", true);
      readConfig();
      log = new JTextArea(25,130);
      log.setMargin(new Insets(5,5,5,5));
      log.setEditable(false);
      log.setFont(new Font(Font.MONOSPACED,Font.BOLD,12));
      logScrollPane = new JScrollPane(log);
      fc = new JFileChooser(fcfolderParm) {
	 @Override
	 public void approveSelection() {
	    fcfolderParm = getCurrentDirectory().getAbsolutePath();
	    writeConfig();
	    File[] ff = getSelectedFiles();
	    log.setText(null);
	    for ( File f : ff ) {
	       callFindent(f,log,f);
	    }
	 }
	 @Override
	 public void cancelSelection() {
	    System.exit(0);
	 }
      };

      fc.setFont(new Font(Font.MONOSPACED,Font.BOLD,14));
      fc.setAcceptAllFileFilterUsed(true);
      FileNameExtensionFilter filter = new FileNameExtensionFilter(
	    ".f .f90 f95 f03 f08 for ftn", 
	    "f","f90","f95","f03","f08","for","ftn");
      fc.setFileFilter(filter);
      fc.setMultiSelectionEnabled(true);
      fc.setApproveButtonText("indent selected files");
      fc.setApproveButtonToolTipText("indent selected files");
      Preview preview = new Preview();
      fc.addPropertyChangeListener(preview);

      formatPanel  = new FormatOptions();
      indentPanel  = new IndentOptions();
      convertPanel = new ConvertOption();
      extraPanel   = new ExtraOptions();
      previewPanel = new PreviewOption();

      JPanel optionsPanel = new JPanel();
      optionsPanel.add(previewPanel);
      optionsPanel.add(formatPanel);
      optionsPanel.add(convertPanel);
      optionsPanel.add(indentPanel);

      mainPane = new JPanel();
      mainPane.setLayout(new BoxLayout(mainPane, BoxLayout.PAGE_AXIS));
      if (MULTICOLORED) {
	 mainPane.setOpaque(true);
	 mainPane.setBackground(new Color(255, 0, 0));
      }
      mainPane.setBorder(BorderFactory.createEmptyBorder(5,5,5,5));
      mainPane.add(fc);
      mainPane.add(optionsPanel);
      mainPane.add(extraPanel);
      mainPane.add(logScrollPane);
      mainPane.add(Box.createGlue());
   }

   private static void initLookAndFeel() {
      String lookAndFeel = null;

      if (LOOKANDFEEL != null) {
	 if (LOOKANDFEEL.equals("Metal")) {
	    lookAndFeel = UIManager.getCrossPlatformLookAndFeelClassName();
	 } else if (LOOKANDFEEL.equals("System")) {
	    lookAndFeel = UIManager.getSystemLookAndFeelClassName();
	 } else if (LOOKANDFEEL.equals("Motif")) {
	    lookAndFeel = "com.sun.java.swing.plaf.motif.MotifLookAndFeel";
	 } else if (LOOKANDFEEL.equals("GTK+")) { //new in 1.4.2
	    lookAndFeel = "com.sun.java.swing.plaf.gtk.GTKLookAndFeel";
	 } else {
	    System.err.println("Unexpected value of LOOKANDFEEL specified: "
		  + LOOKANDFEEL);
	    lookAndFeel = UIManager.getCrossPlatformLookAndFeelClassName();
	 }

	 try {
	    UIManager.setLookAndFeel(lookAndFeel);
	 } catch (ClassNotFoundException e) {
	    System.err.println("Couldn't find class for specified look and feel:"
		  + lookAndFeel);
	    System.err.println("Did you include the L&F library in the class path?");
	    System.err.println("Using the default look and feel.");
	 } catch (UnsupportedLookAndFeelException e) {
	    System.err.println("Can't use the specified look and feel ("
		  + lookAndFeel
		  + ") on this platform.");
	    System.err.println("Using the default look and feel.");
	 } catch (Exception e) {
	    System.err.println("Couldn't get specified look and feel ("
		  + lookAndFeel
		  + "), for some reason.");
	    System.err.println("Using the default look and feel.");
	    e.printStackTrace();
	 }
      }
   }

   void readConfig() {
      extraParm     = "";
      convertParm   = false;
      fixedfreeParm = "auto";
      indentParm    = 3;
      previewParm   = true;
      findentParm   = "findent";
      fcfolderParm  = System.getProperty("user.home");

      try {
	 DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
	 DocumentBuilder builder        = factory.newDocumentBuilder();

	 Document document = builder.parse(new File(OsUtils.getConfigFileName()));
	 NodeList nodeList = document.getDocumentElement().getChildNodes();

	 for (int i = 0; i < nodeList.getLength(); i++) {
	    Node node = nodeList.item(i);

	    if (node.getNodeType() == Node.ELEMENT_NODE) {
	       Element elem = (Element) node;

	       try {
		  extraParm = elem.getElementsByTagName("extra")
		     .item(0).getChildNodes().item(0).getNodeValue();
	       } catch (Exception e) {}

	       try {
		  convertParm = Boolean.parseBoolean(elem.getElementsByTagName("convert")
			.item(0).getChildNodes().item(0).getNodeValue());
	       } catch (Exception e) {}

	       try {
		  previewParm = Boolean.parseBoolean(elem.getElementsByTagName("preview")
			.item(0).getChildNodes().item(0).getNodeValue());
	       } catch (Exception e) {}

	       try {
		  fixedfreeParm = elem.getElementsByTagName("fixedfree").item(0)
		     .getChildNodes().item(0).getNodeValue();
	       } catch (Exception e) {}

	       try {
		  indentParm = Integer.parseInt(elem.getElementsByTagName("indent")
			.item(0).getChildNodes().item(0).getNodeValue());
	       } catch (Exception e) {}

	       try {
		  fcfolderParm = elem.getElementsByTagName("fcfolder").item(0)
		     .getChildNodes().item(0).getNodeValue();
	       } catch (Exception e) {}

	       try {
		  findentParm = elem.getElementsByTagName("findent").item(0)
		     .getChildNodes().item(0).getNodeValue();
	       } catch (Exception e) {}
	    }
	 }
      }
      catch (Exception e) {
      }
   }

   static void writeConfig() {
      try {
	 PrintWriter writer = new PrintWriter(OsUtils.getConfigFileName(),"UTF-8");
	 writer.println("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	 writer.println("<jfindent>");
	 writer.println("  <parms>");
	 writer.println("    <convert>"   + convertParm    +"</convert>");
	 writer.println("    <fixedfree>" + fixedfreeParm  +"</fixedfree>");
	 writer.println("    <indent>"    + indentParm     +"</indent>");
	 writer.println("    <extra>"     + extraParm      +"</extra>");
	 writer.println("    <fcfolder>"  + fcfolderParm   +"</fcfolder>");
	 writer.println("    <preview>"   + previewParm    +"</preview>");
	 writer.println("    <findent>"   + findentParm    +"</findent>");
	 writer.println("  </parms>");
	 writer.println("</jfindent>");
	 writer.close();
      }
      catch (Exception e) {
      }
   }

   static void callFindentPreview(){
      if (!previewParm){
	 return;
      }
      if (inFile == null){
	 return;
      }

      // try to put the scrollbar postion on the same place if
      // one is looking at the same file, using different
      // indent options
      
      final Point vp; 
      
      if (prevInFile == null || !prevInFile.equals(inFile.getAbsolutePath())){
	 vp = new Point(0,0);
      } else {
	 vp  = logScrollPane.getViewport().getViewPosition();
      }

      callFindent(inFile,log,null);

      SwingUtilities.invokeLater(new Runnable() {
	 public void run() {
	    logScrollPane.getViewport().setViewPosition(vp);
	 }
      });

      prevInFile = inFile.getAbsolutePath();
   }

   static void callFindent(File inFile, JTextArea log, File outFile, Integer... what) {
      // what == DOHELP:    runs findent -h
      // what -- DOVERSION: runs findent -v
      // outFile = null: output to log, else output to outFile
      // outFile can be the same file as inFile

      // ok, this code got a bit hairy, suggestions are welcome

      boolean doHelp    = false;
      boolean doVersion = false;

      if (what.length == 1){
	 switch (what[0]){
	    case DOHELP:    doHelp = true;
			    break;
	    case DOVERSION: doVersion = true;
			    break;
	 }
      }

      boolean doFile = (outFile != null);
      if (doHelp || doVersion){
	 doFile = false;
      }

      String findentExe = findentParm;

      if (findentExe == null){
	 findentExe = "findent";
      }

      if (findentExe.trim().isEmpty()){
	 findentExe = "findent";
      }

      //if (!(doHelp || doVersion)){
//	 if (!doFile && !previewParm){
//	    return;
//	 }
 //     }

      String endl  = OsUtils.getNewLine();
      String fendl = "\n";

      if (!(doHelp || doVersion)){
	 if(inFile == null){
	    if (doFile) {
	       log.append("No inputfile ..."+endl);
	       log.setCaretPosition(log.getDocument().getLength());
	    }
	    return;
	 }
	 try{
	    switch(FileFormat.discover(inFile.getAbsolutePath())){
	       case WINDOWS: fendl = "\r\n"; break;
	       case MAC:     fendl = "\r";   break;
	       default:      fendl = "\n";   break;
	    }
	 } catch (Exception e) { 
	    fendl = "\n"; }
      }

      java.util.List<String> parms = new ArrayList<String>();
      parms.add(findentExe);

      if (doHelp){
	 parms.add("-h");
      } else if (doVersion){
	 parms.add("-v");
      } else {
	 String s;
	 s = "-i"+fixedfreeParm;
	 parms.add(s);

	 if (convertParm) {
	    s = "-ofree";
	    parms.add(s);
	 }
	 s = "-i"+indentParm;
	 parms.add(s);

	 String[] extraparms = extraParm.split("[ \t]+");
	 for ( String sp : extraparms ){
	    parms.add(sp);
	 }
      }

      ProcessBuilder pb = new ProcessBuilder(parms);
      pb.redirectErrorStream(true);

      Process process = null;
      try {
	 process = pb.start();
      } catch (IOException e) {
	 log.append("Couldn't start findent."+endl);
	 log.append("The command was '"+findentExe+"'"+endl);
	 log.append("Fix this in the 'file->location of findent' menu"+endl);
	 log.setCaretPosition(log.getDocument().getLength());
	 return;
      }

      BufferedReader br = new BufferedReader(new InputStreamReader(process.getInputStream()));
      BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(process.getOutputStream()));

      int mcount = 1000;
      if (doFile || doHelp || doVersion){
	 mcount = 0;
      }

      PipeFromFile pipe = null;
      Thread thread = null;
      if (!(doHelp || doVersion)){
	 pipe = new PipeFromFile(inFile.getAbsolutePath(),bw,mcount);
	 thread = new Thread(pipe);
	 thread.start();
      }

      int counterIn = 0;
      int counterOut = 0;

      if (doHelp || doVersion){
	 //log.setText("Findent options:"+endl);
      } else {
	 if (!doFile){
	    log.setText("Preview (max "+mcount+" lines)"+endl);
	 }
      }

      log.append("Running: ");
      for (String s1: parms){
	 log.append(s1+" ");
      }

      if (!(doHelp || doVersion)){
	 log.append(" < "+inFile.getName());
      }

      if (doFile){
	 log.append(" -- ");
      } else {
	 log.append(endl);
      }

      File temp     = null;
      Writer writer = null;

      if (doFile){
	 try{
	    temp = File.createTempFile("jfindent", ".tmp");
	 } catch (Exception e) {
	    log.append("Cannot create temp file");
	    log.setCaretPosition(log.getDocument().getLength());
	    return;
	 }

	 try {
	    writer = new BufferedWriter(new OutputStreamWriter(
		     new FileOutputStream(temp), "utf-8"));
	 } catch (Exception e) {
	    log.append("Cannot write to temp file "+temp.getAbsolutePath());
	    log.setCaretPosition(log.getDocument().getLength());
	    try{
	       temp.delete();
	    } catch(Exception ee) {}
	    return;
	 }
      }

      String currLine;
      try {
	 while((currLine = br.readLine()) != null) {
	    counterOut++;
	    if (doFile){
	       writer.write(currLine+fendl);
	    } else {
	       if (doHelp || doVersion){
		  log.append("    ");
	       } else {
		  log.append(String.format("%06d ",counterOut));
	       }
	       log.append(currLine+endl);
	    }
	 }
      } catch (IOException e) {
	 log.append(endl+"Couldn't read the output or write to temp file."+endl);
	 log.setCaretPosition(log.getDocument().getLength());
	 if (doFile){
	    try{
	       temp.delete();
	    } catch(Exception ee) {}
	 }
	 return;
      }

      if (doFile) {
	 try{
	    writer.close();
	 } catch (Exception e) {}
      }

      if(!(doHelp || doVersion)){
	 String errmsg = pipe.getErrmsg();

	 if (errmsg !=null){
	    log.append(endl+errmsg+endl);
	    log.setCaretPosition(log.getDocument().getLength());
	    if (doFile){
	       try{
		  temp.delete();
	       } catch(Exception e) {}
	    }
	    return;
	 }

	 try{
	    thread.join();
	 } catch (Exception e){}
	 counterIn = pipe.getCounterIn();
	 if (counterIn != counterOut){
	    log.append(endl + 
		  "Error: number of input lines("
		  +counterIn+") is not equal to number of output lines("
		  +counterOut+")"+endl);
	    log.setCaretPosition(log.getDocument().getLength());
	    if (doFile){
	       try{
		  temp.delete();
	       } catch(Exception e) {}
	    }
	    return;
	 }
      }

      if (doFile){
	 try{
	    Files.copy(temp.toPath(),outFile.toPath(),StandardCopyOption.REPLACE_EXISTING);
	 }
	 catch (Exception e){
	    log.append(e.toString());
	    log.append(endl+"Cannot copy from "+temp.getAbsolutePath()+" to "+outFile.getAbsolutePath()+endl);
	    log.setCaretPosition(log.getDocument().getLength());
	    try{
	       temp.delete();
	    } catch(Exception ee) {}
	    return;
	 }
	 log.append("Indented "+counterIn+" lines"+endl);
	 log.setCaretPosition(log.getDocument().getLength());
      } else {
	 if (doHelp){
	    log.append("---> end of options <---"+endl);
	    log.setCaretPosition(0);
	 } else if (doVersion){
	    log.setCaretPosition(0);
	 } else {
	    log.append("---> end of preview <---"+endl);
	 }
      }
   }

   static class JfindentMenu implements ActionListener, ItemListener {
      JFrame aboutFrame    = null;
      JFrame versionFrame  = null;
      JFrame generalHelpFrame     = null;
      JFrame extraOptionsFrame     = null;

      public JfindentMenu(JFrame frame){
	 JMenuBar menuBar = new JMenuBar();
	 JMenu fileMenu = new JMenu("file");

	 JMenu configMenu = new JMenu("config");

	 JMenuItem fcItem = new JMenuItem("choose location of findent ...");
	 fcItem.setActionCommand("fc");
	 fcItem.addActionListener(this);

	 JMenuItem defaultItem = new JMenuItem("find findent via PATH");
	 defaultItem.setActionCommand("defaultPath");
	 defaultItem.addActionListener(this);

	 JMenuItem testItem = new JMenuItem("test");
	 testItem.setActionCommand("test");
	 testItem.addActionListener(this);

	 configMenu.add(fcItem);
	 configMenu.addSeparator();
	 configMenu.add(defaultItem);
	 configMenu.addSeparator();
	 configMenu.add(testItem);

	 JMenuItem quitItem = new JMenuItem("quit");
	 quitItem.setActionCommand("quit");
	 quitItem.addActionListener(this);

	 fileMenu.add(quitItem);

	 JMenu infoMenu = new JMenu("info");

	 JMenuItem versionItem = new JMenuItem ("version");
	 versionItem.setActionCommand("version");
	 versionItem.addActionListener(this);

	 JMenuItem aboutItem = new JMenuItem("Whatis this?");
	 aboutItem.setActionCommand("about");
	 aboutItem.addActionListener(this);

	 infoMenu.add(aboutItem);
	 infoMenu.addSeparator();
	 infoMenu.add(versionItem);

	 JMenu helpMenu = new JMenu("help");

	 JMenuItem generalusageItem = new JMenuItem("general usage");
	 generalusageItem.setActionCommand("generalUsage");
	 generalusageItem.addActionListener(this);

	 JMenuItem extraoptionsItem = new JMenuItem("extra options");
	 extraoptionsItem.setActionCommand("extraOptions");
	 extraoptionsItem.addActionListener(this);

	 helpMenu.add(generalusageItem);
	 helpMenu.add(extraoptionsItem);

	 menuBar.add(fileMenu);
	 menuBar.add(configMenu);
	 menuBar.add(infoMenu);
	 menuBar.add(helpMenu);

	 frame.setJMenuBar(menuBar);
      }
      public void actionPerformed(ActionEvent e){
	 String endl = OsUtils.getNewLine();
	 switch(e.getActionCommand()){
	    case "fc":                setFindentLocation();
				      log.setText("Findent location: '"+findentParm+"'"+endl);
				      break;
	    case "defaultPath":       findentParm = "findent";
				      log.setText("Findent location: '"+findentParm+"'"+endl);
				      writeConfig();
				      break;
	    case "test":              log.setText("Test if findent runs. Expect 'findent version ...'"
					    +endl);
				      callFindent(null,log,null,DOVERSION);
				      break;
	    case "quit":              System.exit(0);
	    case "about":             showAbout();
				      break;
	    case "version":           showVersion();
				      break;
	    case "generalUsage":      showGeneralUsage();
				      break;
	    case "extraOptions":      showExtraOptions();
				      break;
	    case "doneAbout":         aboutFrame.dispose();
				      aboutFrame = null;
				      break;
	    case "doneVersion":       versionFrame.dispose();
				      versionFrame = null;
				      break;
	    case "doneGeneralHelp":   generalHelpFrame.dispose();
				      generalHelpFrame = null;
				      break;
	    case "doneExtraOptions":  extraOptionsFrame.dispose();
				      extraOptionsFrame = null;
				      break;
	 }
      }

      public void itemStateChanged(ItemEvent e){
      }

      void setFindentLocation(){
	 JFileChooser fileChooser = new JFileChooser();
	 int rc = fileChooser.showOpenDialog(null);
	 if (rc == fileChooser.APPROVE_OPTION){
	    File file = fileChooser.getSelectedFile();
	    findentParm = file.getAbsolutePath();
	    writeConfig();
	 }
	 if (rc == fileChooser.CANCEL_OPTION){
	    // keep current value
	 }
      }

      void showAbout(){
	 if (aboutFrame != null){
	    aboutFrame.setVisible(true);
	    aboutFrame.toFront();
	    return;
	 }
	 aboutFrame = new JFrame("What is this?");
	 JPanel aboutPanel = new JPanel();
	 aboutPanel.setLayout(new BoxLayout(aboutPanel, BoxLayout.PAGE_AXIS));
	 aboutPanel.setBorder(BorderFactory.createEmptyBorder(5,5,5,5));
	 JTextArea textArea = new JTextArea();
	 textArea.setEditable(false);
	 textArea.setFont(new Font(Font.MONOSPACED,Font.BOLD,14));
	 String endl = OsUtils.getNewLine();
	 textArea.append(endl+"jfindent is a graphical wrapper for findent"+endl);
	 textArea.append("findent indents Fortran sources"+endl);
	 aboutPanel.add(textArea);
	 JButton doneButton = new JButton("done");
	 doneButton.setActionCommand("doneAbout");
	 doneButton.addActionListener(this);
	 aboutPanel.add(doneButton);
	 aboutFrame.add(aboutPanel);
	 aboutFrame.pack();
	 aboutFrame.setLocationRelativeTo(null);
	 aboutFrame.setVisible(true);
      }

      void showGeneralUsage(){
	 if (generalHelpFrame != null){
	    generalHelpFrame.setVisible(true);
	    generalHelpFrame.toFront();
	    return;
	 }
	 String endl = OsUtils.getNewLine();

	 generalHelpFrame = new JFrame("general usage");

	 JPanel generalHelpPanel = new JPanel();
	 generalHelpPanel.setLayout(new BoxLayout(generalHelpPanel, BoxLayout.PAGE_AXIS));
	 generalHelpPanel.setBorder(BorderFactory.createEmptyBorder(5,5,5,5));

	 JTextArea textArea = new JTextArea();
	 textArea.setEditable(false);
	 textArea.setFont(new Font(Font.MONOSPACED,Font.BOLD,14));

	 textArea.append(endl+"Usage:"+endl);
	 textArea.append("Select one or more Fortran sources"+endl);
	 textArea.append("Have a look at the preview, adapt the options to your taste"+endl);
	 textArea.append("and click 'indent'"+endl);
	 textArea.append("NOTE: this will over-write the original files"+endl);
	 generalHelpPanel.add(textArea);

	 JButton doneButton = new JButton("done");
	 doneButton.setActionCommand("doneGeneralHelp");
	 doneButton.addActionListener(this);

	 generalHelpPanel.add(doneButton);
	 generalHelpFrame.add(generalHelpPanel);
	 generalHelpFrame.pack();
	 generalHelpFrame.setLocationRelativeTo(null);
	 generalHelpFrame.setVisible(true);
      }

      void showExtraOptions(){
	 if (extraOptionsFrame != null){
	    extraOptionsFrame.setVisible(true);
	    extraOptionsFrame.toFront();
	    return;
	 }
	 String endl = OsUtils.getNewLine();

	 extraOptionsFrame = new JFrame("extraOptions");
	 JPanel extraOptionsPanel = new JPanel();

	 JTextArea textArea = new JTextArea(25,80);
	 textArea.setEditable(false);
	 textArea.setFont(new Font(Font.MONOSPACED,Font.BOLD,14));

	 textArea.append("Extra options"+endl);
	 textArea.append("You can fill in extra options for findent in 'Extra options'"+endl);
	 textArea.append("When done, click 'enter'"+endl);
	 textArea.append("To remove extra options: click 'clear extra options'"+endl);
	 textArea.append("Below follow the possible options (the output of 'findent -h'):"+endl);
	 textArea.append("NOTE: not all options are really useful here,"+endl);
	 textArea.append(" please have a look at the preview first"+endl);
	 textArea.append("---------------------------"+endl);

	 JScrollPane textAreaScrollPane = new JScrollPane(textArea);

	 callFindent(null,textArea,null,DOHELP);

	 JButton doneButton = new JButton("done");
	 doneButton.setActionCommand("doneExtraOptions");
	 doneButton.addActionListener(this);

	 extraOptionsPanel.add(textAreaScrollPane);
	 extraOptionsFrame.add(extraOptionsPanel);
	 extraOptionsPanel.setLayout(new BoxLayout(extraOptionsPanel, BoxLayout.PAGE_AXIS));
	 extraOptionsPanel.setBorder(BorderFactory.createEmptyBorder(5,5,5,5));
	 extraOptionsPanel.add(doneButton);

	 extraOptionsFrame.pack();
	 extraOptionsFrame.setLocationRelativeTo(null);
	 extraOptionsFrame.setVisible(true);
      }

      void showVersion(){
	 if (versionFrame != null){
	    versionFrame.setVisible(true);
	    versionFrame.toFront();
	    return;
	 }
	 String endl = OsUtils.getNewLine();

	 versionFrame = new JFrame("version");

	 JPanel versionPanel = new JPanel();
	 versionPanel.setLayout(new BoxLayout(versionPanel, BoxLayout.PAGE_AXIS));
	 versionPanel.setBorder(BorderFactory.createEmptyBorder(5,5,5,5));

	 JTextArea textArea = new JTextArea();
	 textArea.setEditable(false);
	 textArea.setFont(new Font(Font.MONOSPACED,Font.BOLD,14));
	 textArea.append("jfindent version "+VERSION+endl);

	 callFindent(null,textArea,null,DOVERSION);

	 versionPanel.add(textArea);

	 JButton doneButton = new JButton("done");
	 doneButton.setActionCommand("doneVersion");
	 doneButton.addActionListener(this);

	 versionPanel.add(doneButton);
	 versionFrame.add(versionPanel);
	 versionFrame.pack();
	 versionFrame.setLocationRelativeTo(null);
	 versionFrame.setVisible(true);
      }

   }

   /**
    * Create the GUI and show it.  For thread safety,
    * this method should be invoked from the
    * event-dispatching thread.
    */
   private static void createAndShowGUI() {
      //Set the look and feel.
      initLookAndFeel();

      //Create and set up the window.
      JFrame frame = new JFrame("Jfindent: indent Fortran sources");
      frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

      //Create and set up the content pane.
      Jfindent jfindent = new Jfindent();
      jfindent.mainPane.setOpaque(true); //content panes must be opaque
      frame.setContentPane(jfindent.mainPane);

      JfindentMenu jfindentMenu = new JfindentMenu(frame);

      //Display the window.
      frame.pack();
      frame.setLocationRelativeTo(null);
      frame.setVisible(true);
   }

   public static void main(String[] args) {
      //Schedule a job for the event-dispatching thread:
      //creating and showing this application's GUI.
      javax.swing.SwingUtilities.invokeLater(new Runnable() {
	 public void run() {
	    createAndShowGUI();
	 }
      });
   }

}
