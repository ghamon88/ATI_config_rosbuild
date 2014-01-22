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
	sprintf(request, "GET %s HTTP/1.0\r\nHOST:192.168.1.1\r\n\r\n",page);
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

	/* To modify values --> CGI.xml */

	/* Settings */
        char* setcfgsel; 		// sets the active configuration
	char* setuserfilter; 		// sets cutoff frequency of the low pass filter
	char* setpke; 			//enable peak logging
	char* setbias0; 
	char* setbias1; 
	char* setbias2;
	char* setbias3;
	char* setbias4;
	char* setbias5; 		//offset values for strain gages

	/* Thresholding */
        char* setmce; 			//enable threshold processing
	char* mce0;
	char* mce1;
	char* mce2;
	char* mce3;
	char* mce4;
	char* mce5;
	char* mce6;
	char* mce7;
	char* mce8;
	char* mce9;
	char* mce10;
	char* mce11;
	char* mce12;
	char* mce13;
	char* mce14;
	char* mce15; 
	char* mcx0;
	char* mcx1;
	char* mcx2;
	char* mcx3;
	char* mcx4;
	char* mcx5;
	char* mcx6;
	char* mcx7;
	char* mcx8;
	char* mcx9;
	char* mcx10;
	char* mcx11;
	char* mcx12;
	char* mcx13;
	char* mcx14;
	char* mcx15;
	char* mcc0;
	char* mcc1;
	char* mcc2;
	char* mcc3;
	char* mcc4;
	char* mcc5;
	char* mcc6;
	char* mcc7;
	char* mcc8;
	char* mcc9;
	char* mcc10;
	char* mcc11;
	char* mcc12;
	char* mcc13;
	char* mcc14;
	char* mcc15;
	char* mcv0;
	char* mcv1;
	char* mcv2;
	char* mcv3;
	char* mcv4;
	char* mcv5;
	char* mcv6;
	char* mcv7;
	char* mcv8;
	char* mcv9;
	char* mcv10;
	char* mcv11;
	char* mcv12;
	char* mcv13;
	char* mcv14;
	char* mcv15;
	char* mco0;
	char* mco1;
	char* mco2;	
	char* mco3;
	char* mco4;
	char* mco5;
	char* mco6;
	char* mco7;
	char* mco8;
	char* mco9;
	char* mco10;
	char* mco11;
	char* mco12;
	char* mco13;
	char* mco14;
	char* mco15;

	/* Configurations */
        char* cfgid; 			// config index
	char* cfgnam;			// config name
	char* cfgcalsel; 		// calibration
	char* cfgtu; 			// Torque units, 3=Nm
	char* cfgfu; 			// force units, 2=Newton
	char* cfgtdu; 
	char* cfgtau;
	char* cfgtfx0;
	char* cfgtfx1;
	char* cfgtfx2;
	char* cfgtfx3;
	char* cfgtfx4;
	char* cfgtfx5;
	char* cfgusra;
	char* cfgusrb;		// other variables about tool transformation


	/* Communications */
        char* comnetdhcp; 		// 0 use DHCP if available, 1 use software defined ip address
	char* comnetip; 		// staic IP 
	char* comnetmsk; 		//subnet mask ip
	char* comnetgw; 		//gateway ip
	char* comrdte; 			// enable RDT interface
	char* comrdtrate; 		// RDT rate
	char* comrdtbsiz; 		// RDT buffer size
	char* comeipe;
	char* comdnte;
	char* comrdtmsyn;
	char* comrdtmuid;

	/* Load CGI.xml */
        char docName[]="/home/kuka/src/groovy_workspace/ATI_config_rosbuild/src/CGI.xml";
        TiXmlDocument doc(docName);
	std::cout << "LOAD " << docName << std::endl;
        if(!doc.LoadFile()) return 0;
	std::cout << "LOAD OK" << std::endl;
        TiXmlHandle docHandle(&doc );

	/* Settings */
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

	/* Thresholding */
        child = docHandle.FirstChild("Thresholdings").ToElement();
	if(!child) return 0;
        setmce =(char*)child->Attribute( "setmce");
	mce0 = (char*)child->Attribute("mce0");
        mce1 = (char*)child->Attribute("mce1");
        mce2 = (char*)child->Attribute("mce2");
        mce3 = (char*)child->Attribute("mce3");
        mce4 = (char*)child->Attribute("mce4");
        mce5 = (char*)child->Attribute("mce5");
        mce6 = (char*)child->Attribute("mce6");
        mce7 = (char*)child->Attribute("mce7");
        mce8 = (char*)child->Attribute("mce8");
        mce9 = (char*)child->Attribute("mce9");
	mce10 = (char*)child->Attribute("mce10");
        mce11 = (char*)child->Attribute("mce11");
        mce12 = (char*)child->Attribute("mce12");
        mce13 = (char*)child->Attribute("mce13");
        mce14 = (char*)child->Attribute("mce14");
        mce15 = (char*)child->Attribute("mce15");
	mcx0 = (char*)child->Attribute("mcx0");
        mcx1 = (char*)child->Attribute("mcx1");
        mcx2 = (char*)child->Attribute("mcx2");
        mcx3 = (char*)child->Attribute("mcx3");
        mcx4 = (char*)child->Attribute("mcx4");
        mcx5 = (char*)child->Attribute("mcx5");
        mcx6 = (char*)child->Attribute("mcx6");
        mcx7 = (char*)child->Attribute("mcx7");
        mcx8 = (char*)child->Attribute("mcx8");
        mcx9 = (char*)child->Attribute("mcx9");
	mcx10 = (char*)child->Attribute("mcx10");
        mcx11 = (char*)child->Attribute("mcx11");
        mcx12 = (char*)child->Attribute("mcx12");
        mcx13 = (char*)child->Attribute("mcx13");
        mcx14 = (char*)child->Attribute("mcx14");
        mcx15 = (char*)child->Attribute("mcx15");
	mcc0 = (char*)child->Attribute("mcc0");
        mcc1 = (char*)child->Attribute("mcc1");
        mcc2 = (char*)child->Attribute("mcc2");
        mcc3 = (char*)child->Attribute("mcc3");
        mcc4 = (char*)child->Attribute("mcc4");
        mcc5 = (char*)child->Attribute("mcc5");
        mcc6 = (char*)child->Attribute("mcc6");
        mcc7 = (char*)child->Attribute("mcc7");
        mcc8 = (char*)child->Attribute("mcc8");
        mcc9 = (char*)child->Attribute("mcc9");
	mcc10 = (char*)child->Attribute("mcc10");
        mcc11 = (char*)child->Attribute("mcc11");
        mcc12 = (char*)child->Attribute("mcc12");
        mcc13 = (char*)child->Attribute("mcc13");
        mcc14 = (char*)child->Attribute("mcc14");
        mcc15 = (char*)child->Attribute("mcc15");
	mcv0 = (char*)child->Attribute("mcv0");
        mcv1 = (char*)child->Attribute("mcv1");
        mcv2 = (char*)child->Attribute("mcv2");
        mcv3 = (char*)child->Attribute("mcv3");
        mcv4 = (char*)child->Attribute("mcv4");
        mcv5 = (char*)child->Attribute("mcv5");
        mcv6 = (char*)child->Attribute("mcv6");
        mcv7 = (char*)child->Attribute("mcv7");
        mcv8 = (char*)child->Attribute("mcv8");
        mcv9 = (char*)child->Attribute("mcv9");
	mcv10 = (char*)child->Attribute("mcv10");
        mcv11 = (char*)child->Attribute("mcv11");
        mcv12 = (char*)child->Attribute("mcv12");
        mcv13 = (char*)child->Attribute("mcv13");
        mcv14 = (char*)child->Attribute("mcv14");
        mcv15 = (char*)child->Attribute("mcv15");
	mco0 = (char*)child->Attribute("mco0");
        mco1 = (char*)child->Attribute("mco1");
        mco2 = (char*)child->Attribute("mco2");
        mco3 = (char*)child->Attribute("mco3");
        mco4 = (char*)child->Attribute("mco4");
        mco5 = (char*)child->Attribute("mco5");
        mco6 = (char*)child->Attribute("mco6");
        mco7 = (char*)child->Attribute("mco7");
        mco8 = (char*)child->Attribute("mco8");
        mco9 = (char*)child->Attribute("mco9");
	mco10 = (char*)child->Attribute("mco10");
        mco11 = (char*)child->Attribute("mco11");
        mco12 = (char*)child->Attribute("mco12");
        mco13 = (char*)child->Attribute("mco13");
        mco14 = (char*)child->Attribute("mco14");
        mco15 = (char*)child->Attribute("mco15");
	
	/* Configurations */
        child = docHandle.FirstChild("Configurations").ToElement();
	if(!child) return 0;
        cfgid = (char*)child->Attribute( "cfgid");
	cfgnam = (char*)child->Attribute( "cfgnam");
	cfgcalsel = (char*)child->Attribute( "cfgcalsel");
	cfgtu = (char*)child->Attribute( "cfgtu");
	cfgfu = (char*)child->Attribute( "cfgfu");
	cfgtdu = (char*)child->Attribute( "cfgtdu");
	cfgtau = (char*)child->Attribute( "cfgtau");
	cfgtfx0 = (char*)child->Attribute( "cfgtfx0");
	cfgtfx1 = (char*)child->Attribute( "cfgtfx1");
	cfgtfx2 = (char*)child->Attribute( "cfgtfx2");
	cfgtfx3 = (char*)child->Attribute( "cfgtfx3");
	cfgtfx4 = (char*)child->Attribute( "cfgtfx4");
	cfgtfx5 = (char*)child->Attribute( "cfgtfx5");
	cfgusra = (char*)child->Attribute( "cfgusra");
	cfgusrb = (char*)child->Attribute( "cfgusrb");

	/* Communications */
        child = docHandle.FirstChild("Communications").ToElement();
	if(!child) return 0;
        comnetdhcp = (char*)child->Attribute( "comnetdhcp");
        comnetip = (char*)child->Attribute( "comnetip");
        comnetmsk = (char*)child->Attribute( "comnetmsk");
        comnetgw = (char*)child->Attribute( "comnetgw");
        comrdte = (char*)child->Attribute( "comrdte");
        comrdtrate = (char*)child->Attribute( "comrdtrate");
        comrdtbsiz = (char*)child->Attribute( "comrdtbsiz");
	comeipe = (char*)child->Attribute( "comeipe");
        comdnte = (char*)child->Attribute( "comdnte");
        comrdtmsyn = (char*)child->Attribute( "comrdtmsyn");
        comrdtmuid = (char*)child->Attribute( "comrdtmuid");

	/* Connect to server */
	std::cout << "START SERVER" << std::endl;
	serverAddr.sin_family = AF_INET;
    	serverAddr.sin_port = htons(80);
    	serverAddr.sin_addr.s_addr = inet_addr(ip);
	
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
        	std::cout << "error socket" << std::endl;
    	if (connect(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        	std::cout << "error connect" << std::endl;

	/* HTTP request */
	page=(char*)malloc((1+23+strlen(setcfgsel)+15+strlen(setuserfilter)+8+strlen(setpke)+6*10+strlen(setbias0)+strlen(setbias1)+strlen(setbias2)+strlen(setbias3)+strlen(setbias4)+strlen(setbias5))*sizeof(char));
	sprintf(page,"/setting.cgi?setcfgsel=%s&setuserfilter=%s&setpke=%s&setbias0=%s&setbias1=%s&setbias2=%s&setbias3=%s&setbias4=%s&setbias5=%s",setcfgsel,setuserfilter,setpke,setbias0,setbias1,setbias2,setbias3,setbias4,setbias5);
	sendRequest(page,serverSocket);	
	
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
        	std::cout << "error socket" << std::endl;
	if (connect(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
		std::cout << "error connect" << std::endl;

	page=(char*)malloc((1+19+strlen(setmce)+6*10*3+7*6*3+2*strlen(mce0)*16*3)*sizeof(char));
	sprintf(page,"/moncon.cgi?setmce=%s&mce0=%s&mce1=%s&mce2=%s&mce3=%s&mce4=%s&mce5=%s&mce6=%s&mce7=%s&mce8=%s&mce9=%s&mce10=%s&mce11=%s&mce12=%s&mce13=%s&mce14=%s&mce15=%s&mcx0=%s&mcx1=%s&mcx2=%s&mcx3=%s&mcx4=%s&mcx5=%s&mcx6=%s&mcx7=%s&mcx8=%s&mcx9=%s&mcx10=%s&mcx11=%s&mcx12=%s&mcx13=%s&mcx14=%s&mcx15=%s&mcc0=%s&mcc1=%s&mcc2=%s&mcc3=%s&mcc4=%s&mcc5=%s&mcc6=%s&mcc7=%s&mcc8=%s&mcc9=%s&mcc10=%s&mcc11=%s&mcc12=%s&mcc13=%s&mcc14=%s&mcc15=%s",setmce,mce0,mce1,mce2,mce3,mce4,mce5,mce6,mce7,mce8,mce9,mce10,mce11,mce12,mce13,mce14,mce15,mcx0,mcx1,mcx2,mcx3,mcx4,mcx5,mcx6,mcx7,mcx8,mcx9,mcx10,mcx11,mcx12,mcx13,mcx14,mcx15,mcc0,mcc1,mcc2,mcc3,mcc4,mcc5,mcc6,mcc7,mcc8,mcc9,mcc10,mcc11,mcc12,mcc13,mcc14,mcc15);
	sendRequest(page,serverSocket);
	
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
        	std::cout << "error socket" << std::endl;
	if (connect(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
		std::cout << "error connect" << std::endl;

	page=(char*)malloc((1+19+6*10*2+7*6*2+2*strlen(mcv0)*16*2+200)*sizeof(char));
	sprintf(page,"/moncon.cgi?mcv0=%s&mcv1=%s&mcv2=%s&mcv3=%s&mcv4=%s&mcv5=%s&mcv6=%s&mcv7=%s&mcv8=%s&mcv9=%s&mcv10=%s&mcv11=%s&mcv12=%s&mcv13=%s&mcv14=%s&mcv15=%s&mco0=%s&mco1=%s&mco2=%s&mco3=%s&mco4=%s&mco5=%s&mco6=%s&mco7=%s&mco8=%s&mco9=%s&mco10=%s&mco11=%s&mco12=%s&mco13=%s&mco14=%s&mco15=%s",mcv0,mcv1,mcv2,mcv3,mcv4,mcv5,mcv6,mcv7,mcv8,mcv9,mcv10,mcv11,mcv12,mcv13,mcv14,mcv15,mco0,mco1,mco2,mco3,mco4,mco5,mco6,mco7,mco8,mco9,mco10,mco11,mco12,mco13,mco14,mco15);
	sendRequest(page,serverSocket);
	
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
        	std::cout << "error socket" << std::endl;
	if (connect(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
		std::cout << "error connect" << std::endl;

	page=(char*)malloc((1+18+strlen(cfgid)+8+strlen(cfgnam)+11+strlen(cfgcalsel)+7+strlen(cfgtu)+7+strlen(cfgfu)+9*6+strlen(cfgtfx0)+strlen(cfgtfx1)+strlen(cfgtfx2)+strlen(cfgtfx3)+strlen(cfgtfx4)+strlen(cfgtfx5)+8+strlen(cfgtdu)+8+strlen(cfgtau)+9+strlen(cfgusra)+9+strlen(cfgusrb))*sizeof(char));
	sprintf(page,"/config.cgi?cfgid=%s&cfgnam=%s&cfgcalsel=%s&cfgtu=%s&cfgfu=%s&cfgtdu=%s&cfgtau=%s&cfgtfx0=%s&cfgtfx1=%s&cfgtfx2=%s&cfgtfx3=%s&cfgtfx4=%s&cfgtfx5=%s&cfgusra=%s&cfgusrb=%s",cfgid,cfgnam,cfgcalsel,cfgtu,cfgfu,cfgtdu,cfgtau,cfgtfx0,cfgtfx1,cfgtfx2,cfgtfx3,cfgtfx4,cfgtfx5,cfgusra,cfgusrb);
	sendRequest(page,serverSocket);
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
        	std::cout << "error socket" << std::endl;
	if (connect(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
		std::cout << "error connect" << std::endl;

	page=(char*)malloc((1+21+strlen(comnetdhcp)+10+strlen(comnetip)+11+strlen(comnetmsk)+10+strlen(comnetgw)+9+strlen(comrdte)+12+strlen(comrdtrate)+12+strlen(comrdtbsiz)+9+strlen(comeipe)+9+strlen(comdnte)+12+strlen(comrdtmsyn)+12+strlen(comrdtmuid))*sizeof(char));
	sprintf(page,"/comm.cgi?comnetdhcp=%s&comnetip=%s&comnetmsk=%s&comnetgw=%s&comrdte=%s&comrdtrate=%s&comrdtbsiz=%s&comeipe=%s&comdnte=%s&comrdtmsyn=%s&comrdtmuid=%s",comnetdhcp,comnetip,comnetmsk,comnetgw,comrdte,comrdtrate,comrdtbsiz,comeipe,comdnte,comrdtmsyn,comrdtmuid);
	sendRequest(page,serverSocket);
	
}


