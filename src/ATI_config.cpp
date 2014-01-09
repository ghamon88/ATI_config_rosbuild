/****                              ATI_config.cpp                                          ****/
/****                         author: Guillaume Hamon                                      ****/
/**** Get config values from CGI.xml and configure the F/T net sensor via HTTP get request ****/

 
#include <tinyxml.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <limits>
#include <typeinfo>

using namespace std;

void sendRequest(char* page, int serverSocket)
{
	char* request;
	size_t bWritten, bTotal;
	request=(char*)malloc((16+22+strlen(page))*sizeof(char));
	sprintf(request, "GET %s HTTP/1.1\r\nHOST:192.168.1.1\r\n\r\n",page);
	std::cout << "request " << request << std::endl;
    	for (bTotal = 0; bTotal < strlen(request); bTotal += bWritten)
    	{
        	bWritten = write(serverSocket, &request[bTotal], strlen(request) - bTotal);
        	if (bWritten == 0)
            		break;
    	}
	free(request);
}

int main(){
	int serverSocket;
	struct sockaddr_in serverAddr;
	char ip[] = "192.168.1.1";
	char* page;

	// To modify values --> CGI.xml
	// Settings
        char* setcfgsel; // sets the active configuration
	char* setuserfilter; // sets cutoff frequency of the low pass filter
	char* setpke; //enable peak logging
	char* setbias0; 
	char* setbias1; 
	char* setbias2;
	char* setbias3;
	char* setbias4;
	char* setbias5; //offset values for strain gages

	// Thresholding
        char* setmce; //enable threshold processing

	// Configurations
        char* cfgid; // config index
	char* cfgnam;// config name
	char* cfgcalsel; // calibration
	char* cfgtu; // Torque units, 3=Nm
	char* cfgfu; // force units, 2=Newton
		// + other variables about tool transformation


	// Communications
        char* comnetdhcp; // 0 use DHCP if available, 1 use software defined ip address
	char* comnetip; // staic IP 
	char* comnetmsk; //subnet mask ip
	char* comnetgw; //gateway ip
	char* comrdte; // enable RDT interface
	char* comrdtrate; // RDT rate
	char* comrdtbsiz; // RDT buffer size

        char docName[]="CGI2.xml";
	// Load CGI.xml

        TiXmlDocument doc(docName);
	
	std::cout << "LOAD " << docName << std::endl;

        if(!doc.LoadFile()) return 0;
	
	std::cout << "LOAD OK" << std::endl;

        TiXmlHandle docHandle(&doc );
	// Settings
        TiXmlElement* child = docHandle.FirstChild("Settings").ToElement();
	if(!child) return 0;
	setcfgsel = (char*)child->Attribute("setcfgsel");
	std::cout << setcfgsel << std::endl;
        setuserfilter = (char*)child->Attribute("setuserfilter");
        setpke = (char*)child->Attribute("setpke");
        setbias0 = (char*)child->Attribute("setbias0");
        setbias1 = (char*)child->Attribute("setbias1");
        setbias2 = (char*)child->Attribute("setbias2");
        setbias3 = (char*)child->Attribute("setbias3");
        setbias4 = (char*)child->Attribute("setbias4");
        setbias5 = (char*)child->Attribute("setbias5");
	std::cout << setpke << std::endl;
	// Thresholding
        child = docHandle.FirstChild("Thresholdings").ToElement();
        setmce =(char*)child->Attribute( "setmce");
	
	// Configurations
        child = docHandle.FirstChild("Configurations").ToElement();
        cfgid = (char*)child->Attribute( "cfgid");
	cfgnam = (char*)child->Attribute( "cfgnam");
	cfgcalsel = (char*)child->Attribute( "cfgcalsel");
	cfgtu = (char*)child->Attribute( "cfgtu");
	cfgfu = (char*)child->Attribute( "cfgfu");

	// Communications
        child = docHandle.FirstChild("Communications").ToElement();
        comnetdhcp = (char*)child->Attribute( "comnetdhcp");
        comnetip = (char*)child->Attribute( "comnetip");
        comnetmsk = (char*)child->Attribute( "comnetmsk");
        comnetgw = (char*)child->Attribute( "comnetgw");
        comrdte = (char*)child->Attribute( "comrdte");
        comrdtrate = (char*)child->Attribute( "comrdtrate");
        comrdtbsiz = (char*)child->Attribute( "comrdtbsiz");

	std::cout << "START SERVER" << std::endl;

	serverAddr.sin_family = AF_INET;
    	serverAddr.sin_port = htons(80);
    	serverAddr.sin_addr.s_addr = inet_addr(ip);
	
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
        	std::cout << "error socket" << std::endl;
    	if (connect(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        	std::cout << "error connect" << std::endl;

	page=(char*)malloc((1+23+strlen(setcfgsel)+15+strlen(setuserfilter)+8+strlen(setpke)+6*10+strlen(setbias0)+strlen(setbias1)+strlen(setbias2)+strlen(setbias3)+strlen(setbias4)+strlen(setbias5))*sizeof(char));
	sprintf(page,"/setting.cgi?setcfgsel=%s&setuserfilter=%s&setpke=%s&setbias0=%s&setbias1=%s&setbias2=%s&setbias3=%s&setbias4=%s&setbias5=%s",setcfgsel,setuserfilter,setpke,setbias0,setbias1,setbias2,setbias3,setbias4,setbias5);
	sendRequest(page,serverSocket);	

	page=(char*)malloc((1+19+strlen(setmce))*sizeof(char));
	sprintf(page,"/moncon.cgi?setmce=%s",setmce);
	sendRequest(page,serverSocket);

	page=(char*)malloc((1+18+strlen(cfgid)+8+strlen(cfgnam)+11+strlen(cfgcalsel)+7+strlen(cfgtu)+7+strlen(cfgfu))*sizeof(char));
	sprintf(page,"/config.cgi?cfgid=%s&cfgnam=%s&cfgcalsel=%s&cfgtu=%s&cfgfu=%s",cfgid,cfgnam,cfgcalsel,cfgtu,cfgfu);
	sendRequest(page,serverSocket);

	page=(char*)malloc((1+21+strlen(comnetdhcp)+10+strlen(comnetip)+11+strlen(comnetmsk)+10+strlen(comnetgw)+9+strlen(comrdte)+12+strlen(comrdtrate)+12+strlen(comrdtbsiz))*sizeof(char));
	sprintf(page,"/comm.cgi?comnetdhcp=%s&comnetip=%s&comnetmsk=%s&comnetgw=%s&comrdte=%s&comrdtrate=%s&comrdtbsiz=%s",comnetdhcp,comnetip,comnetmsk,comnetgw,comrdte,comrdtrate,comrdtbsiz);
	sendRequest(page,serverSocket);
	
}



