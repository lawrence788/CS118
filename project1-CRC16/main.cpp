#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <bitset>

typedef uint16_t u16;
typedef uint8_t u8;
typedef uint64_t u64;
typedef uint32_t u32;

using namespace std;
//Generator Testers: "10001"
//string GENERATOR = "10001000001010001";//"10001000001010001"; //0x11051

string getGen(){
	return "10001000001010001";
}

u64 covertBitString(string msg){
	int len = msg.length();
	int msf = len - 1; 
	uint64_t result = 0;
	for(int i = 0; i< len; i++){
		if(msg[i] == '1'){
			//cout << "1 \n";
			result = result + pow(2,msf);
			msf--;
			// cout << "curr : "; 
			// cout << result << endl;; 
		} else {
			//cout << "0";
			msf--;
		}
		
	}

	return result;
}
/*
// void printBits(vector<bool>* msg){
// 	for(int i =0; i< msg->size();i++){
// 		cout << (*msg)[i];
// 	}
// 	cout<<endl;
// }

u16 getTotalBits(u64 num){
	u16 totalBits = (u16)log2(num)+1;
	return totalBits;
}

u16 shifOpt(u64* gen, u64* msg){
	u16 shifts = 0;
	//get the total of the bits of the given msg.
	//cout << "shifOpt: \n"; 
	u16 totalGenBits = getTotalBits(*gen);
	// cout << "gen bits: ";
	// cout << totalGenBits << endl;

	u16 totalMsgBits = getTotalBits(*msg);
	// cout << "remainder bits: ";
	// cout << totalMsgBits << endl;

	if(totalGenBits > totalMsgBits){
		shifts = totalGenBits - totalMsgBits;
		*msg = (*msg << shifts);
		
	} else {
		shifts = totalMsgBits - totalGenBits;
		*gen = (*gen << shifts);
		
	}

	return shifts;

}

void stoper(int* i){
	if(*i == 5){
		cout<<"failed"<<endl;
		exit(2);
	}

}

u16 getCRC_g(int totCrc, u64 gen, u64 msg){
	// cout<< "THE totCRC ";
	// cout<< totCrc << endl;
	u64 remainder;
	u16 shifted = shifOpt(&gen, &msg);
	remainder = (msg ^= gen);
	cout << remainder << endl;
	if(remainder == 0){
		return remainder;
	}
	// cout << "shifted: ";
	// cout << shifted << endl;
	totCrc = totCrc - shifted;
	// cout<< "second totCRC ";
	// cout<< totCrc << endl;
	while(totCrc > 0){
		
		totCrc = totCrc - shifOpt(&gen, &remainder); 
		// cout << "current totCRC -> ";
		// cout << totCrc << endl; 
		remainder = (remainder ^= gen);

		if(remainder == 0)
			return remainder;

		if(totCrc < 0){
			u16 shift = 0 - totCrc;
			remainder = (remainder << shift);
		}
		// cout << "cur_ remainder ";
		// cout << remainder << endl;
	}

	return remainder;

}



u16 getCRC_m(int totCrc, u64 gen, u64 msg){
	u64 remainder;
	int i =0;
	u64 new_gen = gen;
	u16 shifts = shifOpt(&new_gen,&msg);
	// cout<<"SHIFTED ";
	// cout<<shifts<<endl;
	remainder = (new_gen ^= msg);
	// cout<<"REAINDER ";
	// cout<< remainder << endl;
	//cout<<gen<<end
	while(remainder > gen){
		u64 new_gen = gen;
		shifts = shifOpt(&remainder, &new_gen);
		if(shifts == 0){
			remainder = (new_gen ^= remainder);
			return remainder;
		}


		//totCrc = totCrc - shifts;
		remainder = (new_gen ^= remainder);
		if(remainder == 0)
			return 0;
		// stoper(&i);
		// i++;
	}

	return remainder;

}

u64 addCrc(u64 msg, u16 crc, int totCrcBits){
	u64 new_msg = (msg << totCrcBits);
	new_msg = new_msg | crc;

	return new_msg;
}

string convertBitToString(u64 newMsg){
	string str;
	if(newMsg > (2^32)){
		str = bitset<64>(newMsg).to_string();
		return str;
	}else{
		str = bitset<32>(newMsg).to_string();
		return str;
	}

}

string reformating(string bit_string){

	size_t pos = bit_string.find("1");
	bit_string = bit_string.substr(pos);
	return bit_string;
}


u16 calCRC(string s_msg){

	// vector<bool> msg = covertBitString(s_msg);
	// vector<bool> gen = covertBitString(GENERATOR);
	// addZeros(&msg, GENERATOR.length() - 1);

	u64 msg = covertBitString(s_msg);
	cout<<"before any shifts, the msg is ";
	cout<<msg<<endl;
	u64 gen = covertBitString(getGen());
	// u64 msg = 9;
	// u64 gen = 17;

	//shift the bit so that it can align with each other 
	
	int totCrcBits;
	// cout << "tot crc bits: ";
	// cout << totCrcBits << endl;
	u16 crc;
	if(gen >= msg){
		cout<<"call getCRC_g" << endl;
		totCrcBits = getTotalBits(gen) - 1;
		crc = getCRC_g(totCrcBits,gen,msg);
	}else{
		cout<<"call getCRC_m" << endl;
		int genBits;
		u64 msg_shifted;
		genBits = getTotalBits(gen)  - 1;
		msg_shifted = msg << genBits;
		totCrcBits = getTotalBits(msg_shifted);
		totCrcBits = totCrcBits - genBits + 1;
		//cout << msg << endl;
		crc = getCRC_m(totCrcBits,gen,msg_shifted);
	}

	cout << "CRC is : ";
	cout << crc << endl;
	totCrcBits = getTotalBits(gen) - 1;
	cout << "Current msg is ";
	cout << msg << endl;
	u64 newMsg = addCrc(msg, crc, totCrcBits);
	string bit_string = convertBitToString(newMsg);
	bit_string = reformating(bit_string);
	cout<<bit_string<<endl;

	//cout << msg <<endl;
	//cout << gen << endl;
	return crc;

	//printBits(&msg);	
}

string parser(string crcMsg, string* crc){
	int len = crcMsg.length() - (getGen().length() - 1);
	// cout << "the len is ";
	// cout << len << endl;
	string msg = crcMsg.substr(0, len);
	*crc = crcMsg.substr(len, crcMsg.length()-1);

	return msg;

}

bool crcValidator(string crcMsg){
	if(crcMsg.length() < getGen().length())
		return false;
	string crc; 
	string parsedMsg = parser(crcMsg, &crc);
	u16 crc_v = calCRC(parsedMsg);
	cout << parsedMsg <<endl;
	u64 crc_t = covertBitString(crc);
	
	if(crc_t == crc_v)
		return true;

	return false;
}
*/
int main(int argc, char *argv[]){
	//G is 1 0001 0000 0101 0001
	cout << "Hello World\n";

	int opt;
	extern int optind;
	extern char *optarg;

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
				//calCRC(optarg);
				break;
			case 'v':
				// if(crcValidator(optarg))
				// 	cout << "CRC is consistent " << endl;
				// else 
				// 	cout << "CRC is inconsistent" << endl;
					
				break;
			case 'f':
				cout << "Undetected 4 bit errors\n";
				cout << optarg;
				cout <<"\n";
				break;
			case 't':
				cout << "fraction of Undetected 2 biterrors \n";
				cout << optarg;
				cout <<"\n";
				break;
			case '?': 
				//cout << "???? \n";
				exit(2);
				break;
		}

	}
	
	
	return 0;
}