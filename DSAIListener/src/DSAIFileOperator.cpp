#include "DSAIFileOperator.h"
using namespace std;

DSAIFileOperator::DSAIFileOperator()
{
	
};

DSAIFileOperator::~DSAIFileOperator()
{
	
};

bool DSAIFileOperator::Init(const string& strBackUpFilename)
{
	m_strBackUpFilename = strBackUpFilename;
	
	return true;
}

void DSAIFileOperator::writeDataToFile(string data)
{

    fstream file;
	file.open(m_strBackUpFilename, ios::out|ios::binary);
	if(!file){
        std::cout << "File cannot be created.";
        return;
	}

	file<<data;
	file.close();
}


std::string DSAIFileOperator::readDataFromFile()
{

    ifstream file;

	 
  std::string data;
 

   	file.open(m_strBackUpFilename, ios::in|ios::binary);
	if(!file){
		 std::cout <<"Error in opening file";
		return "";
	}
 
	file >> data;
 
	file.close();

    return data;

}
