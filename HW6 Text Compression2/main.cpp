//
//  main.cpp
//  assignment6
//
//  Created by rick gessner on 11/12/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include "ProcessController.hpp"
#include "Timer.hpp"


//set the path your local folder to get access to the story.txt file...
const char* getPath() {
  //STUDENT: set this path to the folder where your source code lives!!
  return "../story.txt";
}
     
std::ostream& getOutput() {
  return std::cout;
}

//this shows where your original and uncompressed version differ...
void showDivergence(const std::string &aS1, const std::string &anS2, std::ostream &anOutput) {
  size_t theLen1=aS1.length();
  size_t theLen2=anS2.length();
  size_t theMax = std::max(theLen1,theLen2);
  for(int i=0;i<theMax;i++) {
    if(aS1[i]!=anS2[i]) {
      // STUDENTS: EDIT ME IF YOU WANT MORE CONTEXT !!
      anOutput << "\n--MISMATCH:-----------------------------\n";
      anOutput << "string mismatch: " << aS1.substr(std::max(0,i-5), 10) << "\n";
      anOutput << "----------------------------------------\n";
      return;
    }
  }
}

int main(int argc, const char * argv[]) {


  std::ifstream theInput(getPath());

  if(!theInput.is_open()) {
    std::cout << "File did not open!" << std::endl;
    return 1;
  }

  std::string theInputString(
    (std::istreambuf_iterator<char>(theInput) ), (std::istreambuf_iterator<char>() ));

  //std::cout << theInputString << "\n"; //look at the input buffer...
  
  size_t theOrigLength = theInputString.length();
  
  std::string temp(theInputString);
  std::ostream &theOutput = getOutput();

  ECE17::Timer theTimer; //let's check how quickly you can do this!...
  theTimer.start();
  
  //run statistics, then compress and decompress the file...
  ECE17::ProcessController theController;
  theController.showStatistics(temp, theOutput);

  std::string theCompressedString;
  std::ostringstream theCompressedOutput(theCompressedString);
  std::string temp2(theInputString); //don't let them change our input string...
  theController.compress(temp, theCompressedOutput);
  std::string theTempC = theCompressedOutput.str();
  size_t theCompressedLength = theTempC.length();
  //std::cout << "compressed:\n" << theCompressedOutput.str() << "\n";

  std::string theDecompressedString;
  std::ostringstream theDecompressedOutput(theDecompressedString);
  theController.decompress(theTempC,theDecompressedOutput);
  std::string theFinalStr(theDecompressedOutput.str());
  size_t theDecompLength = theFinalStr.length();

  //std::cout << "decompressed:\n" << theDecompressedOutput.str() << "\n";

  theTimer.stop();
  theOutput << "\nElapsed time: " << theTimer.elapsed() << "\n";

  double theRate = (1-(1.0*theCompressedLength)/(1.0*theOrigLength))*100;
  theOutput << "Compression rate: "
            << theCompressedLength << "/" << theOrigLength << " = "
            << std::setprecision(2) << std::fixed << theRate << "%\n";

  if((theOrigLength!=theDecompLength) || (0!=theInputString.compare(theFinalStr))) {
    showDivergence(theInputString, theFinalStr, theOutput);
  }


  return 0;
}

