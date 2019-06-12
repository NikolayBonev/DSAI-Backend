#include <iostream>
#include <fstream>
#include "InfoProcessing.hpp"

class DSAIFileOperator
{
public:
    DSAIFileOperator();
    ~DSAIFileOperator();
	bool Init(const std::string& strBackUpFilename);

    void writeDataToFile(std::string data);
    std::string readDataFromFile();
	
	std::string m_strBackUpFilename;
};
