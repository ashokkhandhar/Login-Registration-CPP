#include<iostream>
#include<fstream>
#include<unordered_map>

using namespace std;

struct User
{
    string username;
    string password;
    string name;
    string age;
    string gender;
    string contect;
    string email;

    friend ostream &operator<<(ostream &os, User &user);
    friend ifstream &operator>>(ifstream &ifs, User &user);
    friend ofstream &operator<<(ofstream &ofs, User &user);
};
ifstream &operator>>(ifstream &ifs, User &user){
    ifs >> user.username;
    ifs >> user.password;
    ifs >> user.name;
    ifs >> user.age;
    ifs >> user.gender;
    ifs >> user.contect;
    ifs >> user.email;
    return ifs;
}
ofstream &operator<<(ofstream &ofs, User &user){
    ofs << user.username << endl;
    ofs << user.password << endl;
    ofs << user.name << endl;
    ofs << user.age << endl;
    ofs << user.gender << endl;
    ofs << user.contect << endl;
    ofs << user.email << endl;
    return ofs;
}
ostream &operator<<(ostream &os, User &user){
    os << "\n---Your Profile---" << endl;
    os << "Username: " << user.username << endl;
    os << "Password: " << user.password << endl;
    os << "Name: " << user.name << endl;
    os << "Age: " << user.age << endl;
    os << "Gender: " << user.gender << endl;
    os << "Contect: " << user.contect << endl;
    os << "Email: " << user.email << endl;
    return os;
}

bool usernameValidate(string &username, unordered_map<string, User> &data){
    if(data.find(username) != data.end()) {
        cout << "\nUsername already exists." << endl;
        return false;
    }
    if(username.length() < 3 or username.length() > 20) {
        cout << "\nUsername length must between 3-20." << endl;
        return false;
    }
    for(const auto& un : username){
        if(('a' <= un and un <= 'z') or ('A' <= un and un <= 'Z') or ('0' <= un and un <= '9') or un == '_') continue;
        else {
            cout << "\nUsername must contain only charactor of a-z, A-Z, _, 0-9." << endl;
            return false;
        }
    }
    return true;
}

bool passwordValidate(string &password){
    if(password.length() < 6 and password.length() > 12) return false;
    bool number = false, upper = false, lower = false, special = false;
    for(const char& p : password){
        if('0' <= p and p <= '9') number = true;
        else if('a' <= p and p <= 'z') lower = true;
        else if('A' <= p and p <= 'Z') upper = true;
        else if(p == '@' or p == '$' or p == '!' or p == '%' or p == '*' or p == '#' or p == '?' or p == '&') special = true;
    }
    return (number and upper and lower and special);
}

bool ageValidate(string &age){
    for(char &a : age){
        if(a < '0' or '9' < a) {
            cout << "\nEnter valid age..!" << endl;
            return false;
        }
    }
    return (age.length() <= 2);
}

bool emailValidate(string &email){
    // The email address must start with a letter (no numbers or symbols).
    if(!isalnum(email[0])) return false;
    // There must be one and only one @ somewhere in the string that is located before the dot.
    int at = -1, dot = -1;
    for(int i = 0; i < email.length(); i++){
        if(email[i] == '@') {
            if(at == -1) at = i;
            else return false;
        }
        else if(email[i] == '.'){
            dot = i;
        }
    }
    if(at == -1 or dot == -1) return false;
    // There must be text after the @ symbol but before the dot.
    if(dot < at or email[dot+1] == '.') return false;
    // There must be text after the dot.
    if(dot == email.length()-1) return false;
    return true;
}

int main(){
    unordered_map<string, User> dataLoader;

    User user;
    ifstream impot;
    impot.open("database.txt");
    while (impot >> user) {
        dataLoader.insert({user.username, user});
    }
    impot.close();
    while (true) {
        cout << "\n================" << endl;
        cout << "1 - Login" << endl;
        cout << "2 - Registration" << endl;
        cout << "3 - Deletion" << endl;
        cout << "4 - Exit" << endl;
        cout << "================" << endl;
        cout << "Enter: ";
        string choice;
        cin >> choice;

        if (choice == "1") {
            string username, password;
            cout << "Enter Username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            auto it = dataLoader.find(username);
            if (it == dataLoader.end())
                cout << endl << "Wrong username..!" << endl;
            else if (password != it->second.password)
                cout << endl << "Wrong password...!" << endl;
            else
                cout << it->second;
        } 
        else if (choice == "2") {
            User newUser;
            
            do{
                cout << "Enter username" << endl;
                cout << "(Should have length: 3-20." << endl;
                cout << "characters of a-z, A-Z, _, 0-9\n:";
                cin >> newUser.username;
            }while(!usernameValidate(newUser.username, dataLoader));

            do{
                cout << "Password: " << endl;
                cout << "(Should be between 6 to 20 characters long." << endl;
                cout << "At least one uppercase and one lowercase character." << endl;
                cout << "At least one special symbol (@$!%*#?&)." << endl;
                cout << "At least one number.)\n:";
                cin >> newUser.password;
            }while(!passwordValidate(newUser.password));

            cout << "Full Name: ";
            cin.ignore();
            getline(cin, newUser.name);

            do{
                cout << "Age: ";
                cin >> newUser.age;
            }while(!ageValidate(newUser.age));

            cout << "Gender: " << endl;
            cout << "1 - Male\n";
            cout << "2 - Female\n";
            cout << "3 - Other\n:";
            int option; cin >> option;
            switch (option)
            {
            case 1:
                newUser.gender = "Male";
                break;
            case 2:
                newUser.gender = "Female";
                break;
            default:
                newUser.gender = "Other";
                break;
            }

            cout << "Contact: ";
            cin.ignore(); // Clear the input buffer
            getline(cin, newUser.contect);

            do{
                cout << "Email (ashok.khandhar@example.com): ";
                cin >> newUser.email;
            }while(!emailValidate(newUser.email));

            if (dataLoader.find(newUser.username) != dataLoader.end()) {
                cout << "\nUsername already exists. Registration failed." << endl;
            } else {
                dataLoader.insert({newUser.username, newUser});
                ofstream expot("database.txt");
                for (auto &entry : dataLoader) {
                    expot << entry.second;
                }
                expot.close();
                cout << "\nRegistration Successful..!" << endl;
            }
        } 
        else if (choice == "3") {
            string username, password;
            cout << "Enter Username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            auto it = dataLoader.find(username);
            if (it == dataLoader.end())
                cout << endl << "Wrong username..!" << endl;
            else if (password != it->second.password)
                cout << endl << "Wrong password...!" << endl;
            else{
                dataLoader.erase(username);
                ofstream expot("database.txt");
                for (auto &entry : dataLoader) {
                    expot << entry.second;
                }
                expot.close();
                cout << "\nDeletion Successful..!" << endl;
            }
        } 
        else if(choice == "4") break;
        else {
            cout << "\nPlease enter a valid option..!" << endl;
        }
    }
    return 0;
}