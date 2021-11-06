#include <iostream>
#include <cstring>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

using namespace std;
//Generator Testers: "10001"
//string GENERATOR = "10001000001010001";//"10001000001010001"; //0x11051

// XOR logic, turning the reference bit into '1' or '0'; return bool 
bool exor(char a,char b, char* bit)                                      
{
	char result;
	if(a!=b){
		result ='1';
		*bit = result;
		return true;
	}else{
		result = '0';
		*bit = result;
		return false;
	}
	
}

//add |CRC| -  1 long zero bits to the msg 
char* addTail(char* msg, int crclen, int* codelen){
	int msglen = strlen(msg);
	// cout << "add tail : ";
	// cout << msglen << endl;
	// cout << "crclen ";
	// cout << crclen << endl;
	// cout << msg << endl;

	for(int i=0;i<crclen;i++){                
		msg[msglen+i]='0';
	}
	
	*codelen = msglen + crclen - 1;
	msg[*codelen]='\0';
	//cout << msg << endl;
	return msg;
}

//append msg with crc into MSGCRC
char* appendCRC(char* msg, char* crc){
	char* msgCrc = msg;
	// cout << "prints : ";
	// cout << msgCrc << endl;
	int genlen = strlen(crc);

	int originalDatalen = strlen(msg) - genlen;
	// cout << "originalDatalen is ";
	// cout << originalDatalen << endl;
	for(int i=0;i<genlen;i++)
		msgCrc[originalDatalen+i]=crc[i];                //replace n-1 zeros with n-1 bit CRC
	msgCrc[strlen(msg)]='\0';
	return msgCrc;
}
//setting bit operation; 
//option 'c' copy setter to receiver
//option 's' shift setter for 1 bit and copy to the receiver
void bitSetter(char* receiver, char* setter,int genlen, char op){
	//cout <<  "gen len is " << genlen << endl;
	if(op == 'c'){
		for(int i=0; i< genlen; i++){
			receiver[i] = setter[i]; 
			if(receiver[i] != '0' && receiver[i] != '1'){
				// cout << "got this set: " << setter[i] <<" at "<< i <<endl;
				// cout << "got this rec: " << receiver[i]<<" at "<< i <<endl;
				receiver[i] ='0';
				setter[i] ='0';
				// cout << "after fixed.: " << receiver[i]<< " at "<< i <<endl;
				// cout << "after fixed.: " << setter[i]<< " at "<< i <<endl;
			}
		}


		// cout << "aftcopy--->";
		// cout << receiver << endl;

	} else if(op == 's'){
		genlen-=1;
		for(int i=0; i< genlen; i++){
			receiver[i] = setter[i+1]; 
		}
	} else {
		cout << "no " << op << " option; exists with error" << endl;
		exit(2);
	}
}


char* calCrc(char* data, char* gen, char opt =' ')
{
	char* extendData = data;

	int datalen = strlen(extendData); //size of the original msg 
	// cout << "datalen is ";
	// cout << datalen<<endl;
	int genlen = strlen(gen); //size of the crc
	// cout << "genlen is  ";
	// cout << genlen << endl;
	// cout <<"curr msg";
	// cout << data << endl;;
	//construct the length of msg+crc 
	int codelen;                

	extendData = addTail(extendData, genlen, &codelen);
	//cout << "--------=====\n" << endl;
	//cout << extendData << endl;
	// cout << "cur data len: ";
	// cout << codelen << endl;
	// cout << "";
	char* temp = (char*)malloc(sizeof(char) * genlen);
	//temp[genlen] ='\0';
	char* remBits = (char*)malloc(sizeof(char) * codelen);
	//remBits = '\0';
	//cout << strlen(remBits) << "<--- size of rembit" << endl;
	//set remainder same as the msg that extends with n-1 crc size
	                   //considering n bits of data for each step of binary division/EXOR 
	bitSetter(remBits, extendData, genlen, 'c');
	//cout << remBits << endl;
	// cout << codelen << endl;
	// cout <<">>>>>>>>>>>>>>>>>>"<<endl;
	for(int i=genlen;i<=codelen;i++)
	{
		// record the previous remainder for the next divident
		bitSetter(temp,remBits, genlen, 'c');	                  
		//if 1's bit of remainder is not '0' then xor the divident with generator
		// cout << "current--->" << temp << endl; 
		// cout << "generat--->" << gen << endl;
	    if(remBits[0] !='0'){ 
	    	//cout << "curBits--->" << remBits << endl;
	    	for(int j=0;j<genlen-1;j++){
	    		//cout << j <<" : " <<temp[j+1] << " XOR " << gen[j+1] << " = ";
	            exor(temp[j+1],gen[j+1], &(remBits[j]));
	            // cout <<  remBits[j] << endl;
	            // cout << remBits << endl;
	        }
	        // remBits[genlen] ='0';
	        //cout << "fremBit--->" << remBits <<"[16]->" <<remBits[16] << endl;
	    }else{ 
	    //else if it's '0', then shift the string by 1 bit
	        bitSetter(remBits, temp, genlen,'s');     
	    }

	    if(i == codelen){
	    	remBits[genlen-1]='\0'; //when reach to the last bit, set \0 for end of string. 
	    }else{
	    	//appending next bit of the msg to remBit from division
	        remBits[genlen-1]=extendData[i];  
	    }  
	}
	
	char* msgCrc;
	// cout << "------" <<endl;
	// cout << extendData << endl;
	// cout << "data len before appendCRC ";
	// cout << strlen(extendData) <<endl;
	// cout << "crc len before appendCRC ";
	// cout << strlen(remBits) <<endl;
	// cout << remBits << endl;
	 //replace n-1 zeros with n-1 bit CRC
	msgCrc = appendCRC(extendData, remBits);   
	//cout << "====" << endl;
	if(opt == 'p'){
		cout<< msgCrc << endl;
	}
	//cout<<"CRC="<<remBits<<"\nDataword="<< msgCrc << endl;;
	if(opt == 'm'){
		return msgCrc;
	}

	return remBits;

}
char* parserCrc(char crcMsg[], char gen[]){
	
	int len = strlen(crcMsg) - (strlen(gen) - 1);
	int j =0;
	char* msg = (char*)malloc(sizeof(char) * len);
	msg[len] ='\0';
	for(int i = len; i < strlen(crcMsg); i++){
		
		char bit = crcMsg[i];
		msg[j] = bit;
		j++;
			//cout<<msg << endl;
	}

	//cout<< msg <<endl;
	return msg;
}

bool comp(char* msg, char* ogMsg){
	int count = 0;
	for(int i = 0; i < strlen(msg); i++){
		if(msg[i] != ogMsg[i]){
			return false;
		}
	}

	return true;

}

char* parserOgMsg(char crcMsg[], char gen[]){
	int len = strlen(crcMsg) - (strlen(gen) - 1);
	// cout << "parser : " ;
	// cout << len << endl;
	char* msg = (char*)malloc(sizeof(char) * len);
	msg[len] = '\0';
	// cout << "crcmsg----->"<<strlen(crcMsg) << endl;
	// cout << "the len is ";
	// cout << len << endl;
	int j = 0;
	for(int i = 0; i < len; i++){
		if(i < len){
			char bit = crcMsg[i];
			msg[i] = bit;
			//cout<<msg << endl;
		}
	}
	 // cout << "parser function: "; 
	 //cout << msg << endl;
	 //cout << "crc----->"<<strlen(crc) << endl;
	//msg[len+1]='\0';
	return msg;
}
char* fillZeros(char* gen){

	int len = strlen(gen)-1;
	char* zeros = (char*)malloc(sizeof(char) * len);
	//cout << strlen(zeros) << endl;
	zeros[len] = '\0';
	
	for(int i =0; i < len ; i++){
		zeros[i] = '0';
	}

	 // cout << "fillZeros: ";
	 // cout << zeros<<endl;
	// cout << "---"<<endl;
	return zeros;
}



bool crcValidator(char* crcMsg, char* gen ){
   if(strlen(crcMsg) < strlen(gen))
 		return false;
    
    //crc = parserCrc(crcMsg,gen);

 	if(strlen(crcMsg) > 32){
 		char* crc = fillZeros(gen);
 		char* crc_v = calCrc(crcMsg, gen);
 		if(comp(crc_v, crc))
			return true;
		return false;

 	}else{
	 	char* crc;
	 	int len = strlen(crcMsg) - (strlen(gen) - 1);
	 	// cout << "input : ";
	 	// cout << crcMsg << "----->";
	 	// cout << strlen(crcMsg) << endl;

	 	char* ogMsg = parserOgMsg(crcMsg,gen);

	 	// cout << "after calling parser ";
	 	// cout << ogMsg << "----->" ;
	 	// cout << strlen(ogMsg) << endl;

	 	char* crc_v = calCrc(ogMsg, gen);
	 // 	cout << "calling crc_v \n";
		// cout<<crc_v<< "----->";
		// cout << strlen(crc_v) << endl;

	 	crc = parserCrc(crcMsg,gen);
	 	// cout << "calling og crc ";
	 	// cout << crc << "----->";
	 	// cout << strlen(crc) << endl;

	 	// cout << "after calling parser ";
	 	// cout << ogMsg << endl;
	 	// cout << crc << endl;
		
		
		//cout<< crcMsg <<endl;
	 	if(comp(crc_v, crc))
			return true;
		return false;
	}
// 	cout << parsedMsg <<endl;
// 	u64 crc_t = covertBitString(crc);
	// if(hasRem(crc_v))
	// 	return false;
	// return true;
// 	if(crc_t == crc_v)
// 		return true;
 	//return false;
}


char* addGen(char* msg,char* gen){
	char* result = new char[strlen(msg)];
	result[strlen(msg)] = '\0';
	int temp = 0;
	int size = strlen(msg) - 1;
	// cout << msg << endl;
	// cout << size<<endl;
	int s =0;
    for(int i = size; i >= 0; i--){
    	if(msg[i] == '1'){
    		s++;
    	}

    	if(gen[i] == '1'){
    		s++;
    	}

    	if(s%2 == 1){
    		result[i] = '1';
    	} else {
    		result[i] = '0';
    	}
    	// s += msg[i] - '0';
    	// s += gen[i] - '0';
    	// result[i] = char(s%2 + '0');
    	// s += ((i >= 0)? msg[i] - '0': 0);
     //    s += ((i >= 0)? gen[i] - '0': 0);
     //    result[i] = char(s % 2 + '0');
        //cout << result[i];
        s /= 2;
    }

   // cout << result << endl;

    return result;
}

bool hasN(char* errors, int N){
	int count=0;
	int morethanN=N+1;
	for(int i=0; i < strlen(errors); i++){
		if(count > morethanN)
			return false;
		else{
			if(errors[i] == '1')
				count++;
		}
	}

	if(count == N)
		return true;
	return false;
}

char* produce_err(char* msgcrc, char* err){
	char* result = new char[strlen(msgcrc)];
	// cout << err << endl;
	// cout << msgcrc << endl;
	for(int i =0; i<strlen(msgcrc);i++){
		exor(msgcrc[i],err[i],&(result[i]));
	}

	return result;
}



double fact(int n) {
   if (n == 0 || n == 1)
   	return 1;
   else
   	return n * fact(n - 1);
}

char flipBit(char bit){
	if(bit == '1')
		return '0';
	else 
		return '1';
}
//print out 4 bit undetected errors 
void detectRandFour(char* msg, char* gen){
	int size = strlen(msg);
	char* crcErr = fillZeros(gen);
	// cout << "We have size " << size << " long codeword " << endl; 
	// cout << "We have this long crcErr " << crcErr << endl;
	for(int i = 0; i < size; i++){
		msg[i] = flipBit(msg[i]);
		for(int j = i+1; j < size; j++){
			msg[j] = flipBit(msg[j]);
			for(int k = j+1; k < size; k++){
				msg[k] = flipBit(msg[k]);
				for(int l = k+1; l < size; l++){
					msg[l] = flipBit(msg[l]);
					//char* crc = calCrc(msg, gen);
					char cp[size];
					cp[size] = '\0';
					bitSetter(cp,msg,size,'c');
					char* crc = calCrc(cp, gen);
					if(comp(crc, crcErr))
						cout << msg << endl;
					msg[l] = flipBit(msg[l]);
				}
				msg[k] = flipBit(msg[k]);
			}
			msg[j] = flipBit(msg[j]);
		}
		msg[i] = flipBit(msg[i]);
	}
	//cout<<"--" << endl;
}
//get the number of 2 bit undetected errors 
int detectRandTwo(char* msg, char* gen){
	int size = strlen(msg);
	//cout << "detectRandTwo size " << size << endl;
	char* crcErr = fillZeros(gen);
	//cout << "crcERR ::: "<<crcErr << endl;
	int count = 0;
	int tot_count =0;

	for(int i =0; i< size; i++){
		msg[i] = flipBit(msg[i]);
		for(int j=i+1; j < size; j++){
			msg[j] = flipBit(msg[j]);
			char cp[size];
			cp[size] = '\0';
			bitSetter(cp,msg,size,'c');
			//strncpy(cp, msg,size);
			// cout << "-----" << endl;
			// cout << cp << endl;
			char* crc = calCrc(cp, gen);
			// cout<< crc << endl;
			// cout << "-----" << endl;
			if(comp(crc, crcErr)){
				count++;
				//cout << cp << endl;
				//cout<< crc << endl;
			}	
			msg[j] = flipBit(msg[j]);
			tot_count++;

		}
		msg[i] = flipBit(msg[i]);
	}

	// cout << "tot is " <<endl;
	// cout << tot_count <<endl;
 
	return count;

}



int undetected_N_bits(char* msg,char* gen, int N, char opt = ' '){
	
	char* msgcrc = calCrc(msg,gen, 'm');
	//char msgcrc[] = "10011001";
	int count =0;
	// cout << msgcrc << endl;
	// cout << "------" <<endl;
	if(N == 4){
		detectRandFour(msgcrc, gen);
	}

	if(N == 2){
		count = detectRandTwo(msgcrc, gen);
		//cout << "tot undetected is " << count << endl;
	}

	return count;
}



int main(int argc, char *argv[]){
	//G is 1 0001 0000 0101 0001
	//cout << "Hello World\n";
	char gen[] = "10001000010100001";//"10001000010100001";
	int opt;
	char* msgcrc;
	char* crc = new char[strlen(gen)-1]; 
	int start = 0;
	
	double perm;
	int tot;
	int r;
	double toterror;
	double fraction;

	
	extern int optind;
	extern char *optarg;
	char* msg;

	//spec 5. if no argument exit with error 2
	if(argc == 1){
		//cout << "No arguments\n";
		exit(2);
	}
	
	//This loop detects the input flag, and call the relative function
	while((opt = getopt(argc, argv, "c:v:f:t:")) != -1){
		//cout << opt;
		switch(opt){
			case 'c':
			//Spec1 print the Msg + CRC 
				calCrc(optarg, gen, 'p');
				break;
			case 'v':
			//Spec2 verify the crc is consistent

				if(crcValidator(optarg,gen))
					cout << '1' << endl;
				else 
					cout << '0' << endl;
					
				break;
			case 'f':
			//Spec3 list all undetected 4 errors 
				//cout << "calling " << endl;
				undetected_N_bits(optarg, gen, 4, 'p');
				break;
			case 't':
				// cout << "the length of msgcrc is " <<endl;
				// cout << optarg << endl;
				// cout << "-----" << endl;
				tot = strlen(optarg)+strlen(gen)-1;
				r = 2;
				perm = fact(tot) / (2*fact(tot-2));
				//cout << "call undetected" << endl;
				//cout<<optarg<<endl;
				toterror = (double)undetected_N_bits(optarg, gen, 2);
				//cout << "----" << endl;
				//cout << perm << endl;
				fraction = toterror / perm;
				if(fraction == 0)
					break;
				cout << fraction << endl;
				//cout <<"\n";
				break;
			case '?': 
				//cout << "???? \n";
				exit(2);
				break;
		}

	}
	
	
	return 0;
}