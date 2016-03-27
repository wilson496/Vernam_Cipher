//Vernam Cipher implementation, by Cameron Wilson
/*
This implementation of Vernam Cipher converts
each character of a string to a 7-bit bitset and placed within a bitset array.
The bitset array will have the XOR operation applied to each bit of each bitset
with an arbitrarily chosen key.

*/
#include <string>
#include <bitset>
#include <iostream>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstddef>
#include <fstream>

using namespace std;



//Array of non-printable control characters that will printed to the output file according to
//ascii value that each bitset after XOR
string controlChar[128] = {"NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL", "BS", "TAB",
"LF", "VT", "FF", "CR", "SO", "SI", "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB", "CAN",
"EM", "SUB", "ESC", "FS", "GS", "RS", "US"};

//controller class to store binary value and the string from control characters or
//a printable ascii character
class controller {

    private:

      string controlField;
      char ascii;
      bitset<7> binary;

    public:

      void set_char(char newChar){
        ascii = newChar;
      }
      void set_string(string field){
        controlField = field;
      }
      void set_bin(bitset<7> bit){
        binary = bit;
      }
      string get_string(){
        return controlField;
      }
      char get_char(){
        return ascii;
      }
      bitset<7> get_binary(){
        return binary;
      }

};

//Decrypt will decrypt the ciphertext IF the user chooses yes. Otherwise, readFromFile
//will convert the ciphertext to the original plaintext.
void decrypt(controller chr[], int size){

    bitset<7> finalBit[size];

    //cipher key that will XOR with each bitset in the bitset array
    string cipherkey = "1011011";

    bitset<7> e;
    string final;

    for (int i = 0; i < size; i++){
      finalBit[i] = chr[i].get_binary();
      cout << "WE WILL DECRYPT " << finalBit[i] << endl;

      e = finalBit[i];
      for (int j = 0; j < sizeof(finalBit[i]); j++){


          //cout << sizeof(str[i]) << endl;
          if (cipherkey[j] == '1' && e.test(j) == true) {
            e[j] = false;
          }

          else if (cipherkey[j] == '1' && e.test(j) == false) {
            e[j] = true;
          }

          else if (cipherkey[j] == '0' && e.test(j) == true) {
            e[j] = true;
          }

          else {
            continue;
          }

      }
      finalBit[i] = e;

      unsigned char r = finalBit[i].to_ulong();
      cout << "Converted to ascii char " << r << " from bitset " << finalBit[i] << endl;

      final += r;
    }



    cout << "FINAL STRING! " << final << endl;
    ofstream myfile;
    myfile.open ("plain.txt");
    myfile << final << endl;
    myfile.close();
}

//This will convert the ciphertext to plaintext if the user chooses not to convert immediately.

//Note: The user will have had to encrypt before using decrypt
//(i.e. binary values separated on each line with the ciphertext on the last line,
//although this won't be read)




void readFromFile() {


  ifstream infile;
  infile.open("cipher.txt");

  int numlines = 0;
  string line;


  while (std::getline(infile, line))
  ++numlines;

  infile.close();



  infile.open("cipher.txt");

  string str;
  controller readControl[numlines];
  int index = 0;
  while (getline(infile, str) && index < numlines - 1)
  {
    bitset<7> newB = bitset<7>(str);
    readControl[index].set_bin(newB);
    cout << readControl[index].get_binary() << endl;
    index++;
  }

  infile.close();

  decrypt(readControl, numlines - 1);


}

//After the XOR operation is applied, convertToString will convert the bitset array's bitsets
//into ascii characters to reproduce a ciphertext to be immediately converted, or write to a file
//to convert later

//Note: the file write will happen regardless.

void convertToString(bitset<7> bit[], int size){

  string final;
  unsigned char d;
  string control;
  controller controls[size];

  ofstream myfile;
  myfile.open ("cipher.txt");


  for (int i = 0; i < size; i++){

    myfile << bit[i] << endl; //writes binary values to file to read from if converting later
    long t = bit[i].to_ulong();
    if (iscntrl(t)){
      cout << "CONTROL CHAR DETECTED!" << endl;
      if (t < 32) {

          control = controlChar[t];
          final += control;
          controls[i].set_string(control);
          controls[i].set_bin(bit[i]);
          //controls[i] = c;
          cout << "Substituting string " << controls[i].get_string() <<
          " for control character with ascii value "  << t << endl;
          continue;
      }
      else {
        control = "DEL";
        final += control;
        controls[i].set_string(control);
        controls[i].set_bin(bit[i]);
        //controls[i] = c;
        cout << "Substituting string " << controls[i].get_string() <<
        " for control character with ascii value "  << t << endl;
        continue;
      }
    }



    d = bit[i].to_ulong();
    controls[i].set_bin(bit[i]);
    controls[i].set_char(d);
    cout << "BIT IN ASCII? " << d << endl;
    final += d;


  }

  cout << "FINAl STRING IN FILE? " << final << endl;


  //put encrypted string into file to read from later

  myfile << "Ciphertext: " << final << endl;
  myfile.close();

  cmd: //return here if people put in a different answer than yes or no
  string decryptAns = "";
  cout << "Would you like to decrypt the cipher? (yes/no)" << endl;
  getline(cin, decryptAns);

  if (decryptAns == "yes"){
      decrypt(controls, size);
  }
  else if (decryptAns == "no"){

      exit(0);
  }
  else {
      goto cmd; // return to the point where you ask if you want to decrypt the cipher.
  }

}



void operationXOR(bitset<7>  bit[], int size){

  string cipherkey = "1011011";

  bitset<7> currBitset;
  //For each bit for each bitset element, we must create a key which we can apply XOR to
  for (size_t i = 0; i < size; i++){

      currBitset = bit[i];
      //cout << "prepare to enter loop" << endl;
      for (int j = 0; j < sizeof(bit[i]); j++){


          //cout << sizeof(str[i]) << endl;
          if (cipherkey[j] == '1' && currBitset.test(j) == true) {
            currBitset[j] = false;
          }

          else if (cipherkey[j] == '1' && currBitset.test(j) == false) {
            currBitset[j] = true;
          }

          else if (cipherkey[j] == '0' && currBitset.test(j) == true) {
            currBitset[j] = true;
          }

          else {
            continue;
          }

      }
      cout << bit[i] << " with XOR applied is " << currBitset << endl;
      bit[i] = currBitset;
  }

//PASS BITSET ARRAY AS PARAMETER WITH SIZE OF STRING
  convertToString(bit, size);

}



string convertToBinary(string plaintext){

  int n = plaintext.size();

  bitset<7>  bit[n];
  for (size_t i = 0; i < n; ++i)
  {
    bit[i] = bitset<7>(plaintext[i]);
    cout << "Original bitset " << bit[i] << " converted from ascii char " << plaintext[i] << endl;

  }

  operationXOR(bit, n);

  return "";
}

int main(){

    string answer = "";
    cout << "What may I do for you today?(encrypt/decrypt)" << endl;
    getline(cin, answer);

    if (answer == "encrypt") {
          cout << "ENCRYPT!" << endl;
          string input = "";
      	  cout<<"Enter your key"<<endl;
          getline(cin, input);
          cout<<"You've put " << input << endl;
          convertToBinary(input);
          return 0;
    }

    else if (answer == "decrypt"){
      string input = "";
      readFromFile();
      return 0;

    }



    else {
      cout << "Enter a valid command (encrypt/decrypt)" << endl;
      exit(0);
    }

}
