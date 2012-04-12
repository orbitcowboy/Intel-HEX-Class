/*******************************************************************************
*
* INTEL HEX UTILITY - IHDIFF
*
*******************************************************************************/

/*******************************************************************************
* ihDiff.cpp
*
* This utility is designed to compare the content of two Intel HEX format files.
* The utility generates a list of differences between the two chosen files,
* including:
*   - Addresses where two bytes differ from one another
*   - Addresses where one file has content, but the other does not.
* The utility also checks the source file for issues. Regardless of the issues
* potentially found, the utility will do its best to generate a differences
* list.
*
* For more information and the latest release, please visit this projects home
* page at http://codinghead.github.com/Intel-HEX-Class
* To participate in the project or for other enquiries, please contact Stuart
* Cording at codinghead@gmail.com
*
* Licensing Information
* Copyright (c) 2012 Stuart Cording
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Author: Stuart Cording aka CODINGHEAD
*
********************************************************************************
* Notes:
* No notes to date (12th April 2012)
*******************************************************************************/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>

#include "intelhexclass.h"

using namespace std;

char *program_name;

// Usage for this program
void usage()
{
    cerr << "Usage is " << program_name <<
                 " [input_file_a] [input_file_b]" << endl;
    exit (EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    // Create an input stream
	ifstream intelHexInputA;
	ifstream intelHexInputB;
	
	// Create an output stream
	ofstream intelHexOutput;
	
	// Create a variable for the intel hex data
	intelhex ihDiffA;
	intelhex ihDiffB;
	
	// Note if there were issues with decoding
	bool decodingIssues = false;
	
	// Storage for the address being analysed for both files
	unsigned long diffAAddress = 0UL;
	unsinged long diffBAddress = 0UL;
		
	// The program name is the first argument - save for later use
	program_name = argv[0];

	// Make sure there are only <command> and 2 x <file> arguments	
	if(argc != 3) {
    	usage();
    }
    	
	intelHexInputA.open(argv[1], ifstream::in);
	
	if(!intelHexInputA.good())
	{
    	std::cerr << "Error: couldn't open " << argv[1] << std::endl;
    	usage();
	}
	
	intelHexInputB.open(argv[2], ifstream::in);
	
	if(!intelHexInputB.good())
	{
    	std::cerr << "Error: couldn't open " << argv[2] << std::endl;
    	usage();
	}
 
    /* Decode both files                                                      */
    intelHexInputA >> ihDiffA;
    intelHexInputB >> ihDiffB;
    
    /* Check for errors or warnings                                           */
    if (ihDiffA.getNoWarnings() != 0)
    {
        std::cerr << "File " << argv[1] << " contained warnings." << std::endl;
        decodingIssues = true;
    }
    if (ihDiffA.getNoErrors() != 0)
    {
        std::cerr << "File " << argv[1] << " contained errors." << std::endl;
        decodingIssues = true;
    }
    if (ihDiffB.getNoWarnings() != 0)
    {
        std::cerr << "File " << argv[2] << " contained warnings." << std::endl;
        decodingIssues = true;
    }
    if (ihDiffB.getNoErrors() != 0)
    {
        std::cerr << "File " << argv[2] << " contained errors." << std::endl;
        decodingIssues = true;
    }
    
    if (decodingIssues == true)
    {
        std::cerr << "Continuing with diff despite issues listed." << std::endl;
    }
    
    /* Get start addresses for both files                                     */
    ihDiffA.begin();
    ihDiffB.begin();
    
    bool complete = false;
    do
    {
        /* Get address of data we are pointing to                             */
        diffAAddress = ihDiffA.currentAddress();
        diffBAddress = ihDiffB.currentAddress();
        
        /* Get the data at two current addresses                              */
        diffAData = ihDiffA.getData();
        diffBData = ihDiffB.getData();
        
        /* If addresses are the same, compare data values                     */
        if (diffAAddress == diffBAddress)
        {
            if (diffAData != diffBData)
            {
                std::cout << "Address 0x" << diffAAddress << " A = 0x" << \
                               diffAData << " B = 0x" << diffBData << std::endl;
            }
        }
        
        /* If addresses are different, find out which one is lower and output */
        /* that this address has data where the other does not have data      */
        if (diffAAddress < diffBAddress)
        {
            std::cout << "Address 0x" << diffAAddress << " A = 0x" << \
                                          diffAData << " B = ----" << std::endl;
            
        }
        
        
    } while (complete != true);
    
    
    
    if (ihRefactor.getNoErrors() > 0)
    {
        cout << "Issues found with " << argv[1] << endl;
        
        if (ihRefactor.getNoErrors() > 0)
            cout << "Errors:" << endl;
        while (ihRefactor.getNoErrors() > 0)
        {
            string message;
            
            ihRefactor.popNextError(message);
            
            cout << message << endl;
        }
        
        cout << "No output file generated." << endl;
    }
    else
    {
        intelHexOutput.open(argv[2], ofstream::out);
        
        if(!intelHexOutput.good())
	    {
        	std::cerr << "Error: couldn't open " << argv[2] << std::endl;
        	usage();
	    }
	    
	    intelHexOutput << ihRefactor;
	    
	    if (ihRefactor.getNoWarnings() > 0)
        {
            cout << "Warnings generated during decoding:" << endl;
            
            while (ihRefactor.getNoWarnings() > 0)
            {
                string message;
                
                ihRefactor.popNextWarning(message);
                
                cout << message << endl;
            }
        }
        
        cout << "File " << argv[2] << " created successfully." << endl;
    }
       
    return(0);	
}
