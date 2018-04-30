#include <iostream>
#include<fstream>
#include <string.h>
#include <bitset>

using namespace std;

typedef struct header_file
{
    char chunk_id[4];
    int chunk_size;
    char format[4];
    char subchunk1_id[4];
    int subchunk1_size;
    short int audio_format;
    short int num_channels;
    int sample_rate;			// sample_rate denotes the sampling rate.
    int byte_rate;
    short int block_align;
    short int bits_per_sample;
    char subchunk2_id[4];
    int subchunk2_size;			// subchunk2_size denotes the number of samples.
} header;

typedef struct header_file* header_p;

class StealthAudio{
    FILE * InFile;
    FILE * OutFile;
    string str;
public:
    StealthAudio();
    void Getdata(int);
    int Encode();
    string Decode();
};

StealthAudio::StealthAudio()
{
    str = "";
    InFile = NULL;
    OutFile = NULL;
}

void StealthAudio::Getdata(int i)
{
    char file[256];
    switch (i)
    {
        case 1:
            cout<<"Enter the input file name: ";
            cin>>file;
            InFile = fopen(file,"rb");
            cout<<"Enter the output file name: ";
            cin>>file;
            OutFile = fopen(file,"wb");
            break;
        case 2:
            cout<<"Enter the input file name: ";
            cin>>file;
            InFile = fopen(file,"rb");
            break;
        default:
            cout<<"Enter valid option.";
            break;
    }
}

int StealthAudio::Encode()
{
    int BUFSIZE = 16;
    int count = 0;
    bitset<8> message;
    bitset<16> buffer;
    short int buff16[BUFSIZE];
    int nb;
    cout<<"Enter a secret message: ";
    cin.ignore();
    getline(cin,str);
    if (InFile)
    {
        while (!feof(InFile))
        {
            nb = fread(buff16,1,BUFSIZE,InFile);
            /* Insert your processing code here*/
            if (count == 3)
            {
                message = str.length();
                
                buffer = buff16[1];
                for (int i = 0; i < 8; i++)
                {
                    buffer[i] = message[i];
                }
                buff16[1] = buffer.to_ulong();
                cout<<"message: "<<message<<"\t"<< buff16[1];
            }
            if (count<str.length()+5 && count > 4)
            {
                message = str[count-5];
                buffer = buff16[1];
                for (int i = 0; i < 8; i++)
                {
                    buffer[i] = message[i];
                }
                buff16[1] = buffer.to_ulong();
                char c = message.to_ullong();
                cout<< "\n"<<buffer<<"\t"<<message<<"\t"<<c;
            }
//            if (count == str.length()+1)
//            {
//                for (int i = 0; i < 16; i++)
//                {
//                    buffer[i] = 0;
//                }
//                cout<< "\n"<<buffer;
//            }
            
            fwrite(buff16,1,nb,OutFile);			// Writing read data into output file
            count++;
        }
    }
    return 0;
}

string StealthAudio::Decode()
{
    
    int BUFSIZE = 16;
    int count = 0;
    int flag;
    flag = 0;
    char c;
    string str1;
    bitset<8> message;
    bitset<16> buffer;
    short int buff16[BUFSIZE];
    int nb;
    long len;
    if (InFile)
    {
        while (!feof(InFile))
        {
            nb = fread(buff16,1,BUFSIZE,InFile);
            
            /* Insert your processing code here*/
            if (count == 3)
            {
                buffer = buff16[1];
                for (int i = 0; i < 8; i++)
                {
                    message[i] = buffer[i];
                }
                len = message.to_ulong();
                cout<<"Msg len: "<<len<<endl<<message;
            }

            if (count < len+5 && count > 4)
            {
                buffer = buff16[1];
                for (int i = 0; i < 8; i++)
                {
                    message[i] = buffer[i];
                }
                c = message.to_ullong();
                cout<<c<<"\t"<<message<<endl;
                str1 += c;
            }
            count++;
        }
    }
    return str1;
}
int main()
{
    StealthAudio s;
    int choice, res;
    choice = 0;
    while (choice!=3)
    {
        cout<<"\n**Stealth Audio**\n\t1.Encode\n\t2.Decode\n\t3.Exit\n\t\tEnter your choice: ";
        cin>>choice;
        s.Getdata(choice);
        if(choice == 1)
        {
            res = s.Encode();
        }
        else if(choice == 2)
        {
            cout<<"The secret message is: ";
            cout<<s.Decode();

        }
    }
   
    return 0;
}