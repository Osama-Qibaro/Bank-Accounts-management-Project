
#include <iostream>
#include <string>
#include <limits>
using namespace std;
namespace MyInputLib
{
    void ReadString(string msg, string &var){
    bool valid  = false;

        cout << msg;
    while(!valid){  
        cin >> var;

		if(cin.fail()){
			valid = false;
			cout << endl << "Input exceeds variable size. Please try again." << endl;
			cin.clear();
			// cin.ignore(numeric_limits<streamsize>::max, '\n');
		} 
		else valid = true;
	}

    }

    void ReadNumber(string msg, int &var){
        cout << msg;
        cin>>var;
    }

    void ReadNumber(string msg, float &var){
        cout << msg;
        cin>>var;
    }

    bool YesOrNo(string msg){
        char answer = 'm';
        do{
            cin >> answer;
        }while(answer != 'y' || answer != 'Y' || answer != 'n' || answer != 'N');

        return answer == 'y';
    }

    void ReadShortNumBetween(string msg, short &var, short start, short end){
        cout << msg;
        do{
            cin >> var;
        }while(var < start || var > end);
    }
}


