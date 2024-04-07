//The purpose of the project is to create a simple C++ program that performs an AES encryption and then decrypts using the CryptoPP
//P. Rao

#include <iostream>
//library to include the input and output file
#include <fstream>
//include the header libraries from the Crypto ++ library
#include "cryptopp/aes.h"
#include "cryptopp/modes.h"
#include "cryptopp/filters.h"
#include "cryptopp/files.h"

using namespace std;
using namespace CryptoPP;

//This is the main encryption function where CryptoPP function performs an AES encryption
void EncryptFile(const string& inputFile, const string& outputFile, const byte key[], const byte iv[]) {
    
    AES::Encryption aesEncryption(key, AES::DEFAULT_KEYLENGTH);
    CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);

    ifstream inFile(inputFile, ios::binary);
    ofstream outFile(outputFile, ios::binary);

    StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::FileSink(outFile));
    stfEncryptor.Put(byte(0x08), true); // Add padding

    inFile.seekg(0, inFile.end);
    unsigned int fileSize = inFile.tellg();
    inFile.seekg(0, inFile.beg);

    byte* fileBuffer = new byte[fileSize];
    inFile.read(reinterpret_cast<char*>(fileBuffer), fileSize);

    stfEncryptor.Put(fileBuffer, fileSize);
    stfEncryptor.MessageEnd();

    delete[] fileBuffer;
}

//Decryption function that is called after the encryyption has taken place
void DecryptFile(const string& inputFile, const string& outputFile, const byte key[], const byte iv[]) {
    AES::Decryption aesDecryption(key, AES::DEFAULT_KEYLENGTH);
    CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

    ifstream inFile(inputFile, ios::binary);
    ofstream outFile(outputFile, ios::binary);

    StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::FileSink(outFile));
    stfDecryptor.Put(byte(0x08), true); // Remove padding

    inFile.seekg(0, inFile.end);
    unsigned int fileSize = inFile.tellg();
    inFile.seekg(0, inFile.beg);

    byte* fileBuffer = new byte[fileSize];
    inFile.read(reinterpret_cast<char*>(fileBuffer), fileSize);

    stfDecryptor.Put(fileBuffer, fileSize);
    stfDecryptor.MessageEnd();

    delete[] fileBuffer;
}

//This is the main function where initation of the key size, AES cipher is 128 bit/16bytes
int main() {
    typedef unsigned char byte
    byte key[AES::DEFAULT_KEYLENGTH] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6'};
    byte iv[AES::BLOCKSIZE] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6'};

//Initialization of the input file where we store a message in sample.txt
    const string inputFile = "sample.txt";
//Initilizatoin of the output files one for the encryption and one for the decryption
    const string encryptedFile = "encrypted_sample.txt";
    const string decryptedFile = "decrypted_sample.txt";
//Call the Encrypted File Function to peform the permutations
    EncryptFile(inputFile, encryptedFile, key, iv);
    cout << "File encrypted successfully." << endl;
//Call the Decrypted file Function to perform the permuation
    DecryptFile(encryptedFile, decryptedFile, key, iv);
    cout << "File decrypted successfully." << endl;

    return 0;
