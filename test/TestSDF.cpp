#include <sdf/SDFCryptoProvider.h>
#include<string>
#include<iostream>

using namespace std;
using namespace dev;
using namespace crypto;
using namespace sdf;


int main(int, const char* argv[]){
    // Make hash
    cout << "**************Make SM3 Hash************************"<<endl;
    unsigned char bHashData[64] = {0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,
                                0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,
                                0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,
                                0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64};

    unsigned char bHashStdResult[32] = {0xde,0xbe,0x9f,0xf9,0x22,0x75,0xb8,0xa1,0x38,0x60,0x48,0x89,0xc1,0x8e,0x5a,0x4d,
                                    0x6f,0xdb,0x70,0xe5,0x38,0x7e,0x57,0x65,0x29,0x3d,0xcb,0xa3,0x9c,0x0c,0x57,0x32};
    SDFCryptoResult result = Hash(nullptr,SM3,toHex(bHashData,64));
    if (result.sdfErrorMessage != nullptr){
        cout << "Get error : " << result.sdfErrorMessage <<endl;
    }else{
        cout << "Get Hash : " << result.hash << endl;
        cout << "Standard : " << toHex(bHashStdResult,32) <<endl;
    }
    
    result = KeyGen(SM2);
    cout << "****KeyGen****" << endl;
    if (result.sdfErrorMessage != nullptr){
        cout << "Get error : " << result.sdfErrorMessage <<endl;
    }else{
        cout << "Get public key : " << result.publicKey << endl;
        cout << "Get private key : " << result.privateKey << endl;
    }

    SDFCryptoResult signResult = Sign(result.privateKey,SM2,toHex(bHashStdResult,32));
    cout << "****Sign****" << endl;
    if (signResult.sdfErrorMessage != nullptr){
        cout << "Get error : " << signResult.sdfErrorMessage <<endl;
    }else{
        cout << "Get signature: " << signResult.signature << endl;
    }

    cout << "****Verify****" << endl;
    SDFCryptoResult verifyResult = Verify(result.publicKey,SM2,toHex(bHashStdResult,32),signResult.signature);
    if (verifyResult.sdfErrorMessage != nullptr){
        cout << "Get error : " << verifyResult.sdfErrorMessage <<endl;
    }else{
        cout << "Get verify result: " << verifyResult.result << endl;
    }

    signResult =
        SignWithInternalKey(1, "123456", SM2,(const char*) toHex(bHashStdResult, 32));
    cout << "****SignInternalKey****" << endl;
    if (signResult.sdfErrorMessage != nullptr){
        cout << "Get error : " << signResult.sdfErrorMessage <<endl;
    }else{
        cout << "Get signature: " << signResult.signature << endl;
    }

    cout << "****VerifyInternalKey****" << endl;
    verifyResult = VerifyWithInternalKey(1, SM2, (const char *)toHex(bHashStdResult, 32),
                                         signResult.signature);
    if (verifyResult.sdfErrorMessage != nullptr){
        cout << "Get error : " << verifyResult.sdfErrorMessage <<endl;
    }else{
        cout << "Get verify result: " << verifyResult.result << endl;
    }

    cout << "*****ExportInternalPublicKey****" << endl;
    SDFCryptoResult exportResult = ExportInternalPublicKey(2, SM2);
    cout << "Export public key: " << exportResult.publicKey << endl;


    cout << "*****SM4 Encrypt****" << endl;
    unsigned char pbKeyValue[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	unsigned char pbIV[16] = {0xeb,0xee,0xc5,0x68,0x58,0xe6,0x04,0xd8,0x32,0x7b,0x9b,0x3c,0x10,0xc9,0x0c,0xa7};
	unsigned char pbPlainText[32] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10,0x29,0xbe,0xe1,0xd6,0x52,0x49,0xf1,0xe9,0xb3,0xdb,0x87,0x3e,0x24,0x0d,0x06,0x47};
	unsigned char pbCipherText[32] = {0x3f,0x1e,0x73,0xc3,0xdf,0xd5,0xa1,0x32,0x88,0x2f,0xe6,0x9d,0x99,0x6c,0xde,0x93,0x54,0x99,0x09,0x5d,0xde,0x68,0x99,0x5b,0x4d,0x70,0xf2,0x30,0x9f,0x2e,0xf1,0xb7};
    SDFCryptoProvider &provider = SDFCryptoProvider::GetInstance();
    Key key = Key();
    key.setSymmetricKey(pbKeyValue,16);
    unsigned int cypherLen;
    unsigned char cypher[32] = {0};
    unsigned int encryptCode  = provider.Encrypt(key, SM4_CBC ,pbIV, pbPlainText,
        32, cypher, &cypherLen);
    if (encryptCode != SDR_OK) {
      cout << "Failed!!" <<endl;
      cout << provider.GetErrorMessage(encryptCode) <<endl;
    } else {
      cout << "Result: "<< toHex(cypher, (int)cypherLen) << endl;
      cout << "Stand : "<< toHex(pbCipherText, 32) << endl;
    }


    cout << "*****SM4 Decrypt****" << endl;
    unsigned char iv2[16] = {0};
    unsigned int plainlen;
    unsigned char plain[32] = {0};
    unsigned int decryptoCode  = provider.Decrypt(key, SM4_CBC,iv2,pbCipherText,32,plain,&plainlen);
    if (decryptoCode != SDR_OK) {
      cout << "Failed!!" <<endl;
      cout << provider.GetErrorMessage(decryptoCode) <<endl;
    } else {
      cout << "Result: "<< toHex(plain, (int)plainlen)<< endl;
      cout << "Stand : "<< toHex(pbPlainText, 32) << endl;
      cout << "Vi    : "<< toHex(iv2, 8) << endl;
      cout << "Std Vi: "<< toHex(pbIV, 8) << endl;
    }
    return 0;
}

