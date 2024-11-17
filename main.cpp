#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <fmt/core.h>
using namespace std;
using namespace fmt;


struct Password{
    string name, login, password;
    vector<string> category;
    Password(string name,string login, string password, vector<string> category)
            :name(name),login(login),password(password),category(category){

    }
    Password() =default;
    Password(const Password & other)
            :name(other.name),login(other.login),password(other.password),category(other.category){}
    ~Password(){}

};
void menu(){
    println("Wybierz funkcje:");
    println("1. Wyszykaj haslo");
    println("2. Sortować hasla");
    println("3. Dodac haslo");
    println("4. Edytowac haslo");
    println("5. Usunac haslo");
    println("6. Dodać kategorie");
    println("7. Usunąć kategorie");
    println("8. Wyjść z menu");
    println("");
}

void addTimestamp(const string& filename) {
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cerr << "Unable to open file for appending timestamp: " << filename << endl;
        return;
    }

    time_t now = time(0);
    tm* ltm = localtime(&now);

    file<< (ltm->tm_hour < 10 ? "0" : "") << ltm->tm_hour << ":"
         << (ltm->tm_min < 10 ? "0" : "") << ltm->tm_min << ":"
         << (ltm->tm_sec < 10 ? "0" : "") << ltm->tm_sec << "\n";

    file.close();
}

string encrypt(const string& input) {
    vector<char> word(input.begin(), input.end());
    string alf_lower = "abcdefghijklmnopqrstuvwxyz";
    string alf_upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 

    for (int i = 0; i < (int)input.length(); ++i) {
        if (isalpha(word[i])) {
            string& alf = islower(word[i]) ? alf_lower : alf_upper;
            for (int j = 0; j < (int)alf.length(); ++j) {
                if (word[i] == alf[j]) {
                    word[i] = alf[(j + 3) % 26];  
                    break;
                }
            }
        }
    }

    string str(word.begin(), word.end());
    return str;
}

void encryptPasswords(const vector<Password>& passwords, const string& filename) {
    ofstream file(filename,ios::in);
    if (!file.is_open()) {
        cerr << "Unable to open file for writing: " << filename << endl;
        return;
    }

    for (const auto& password : passwords) {
        file <<"name:"<< encrypt(password.name) << '\n';
        file <<"login:"<< encrypt(password.login) << '\n';
        file <<"password:"<< encrypt(password.password) << '\n';
        file<< "category:";

        for (const auto& category : password.category) {
            file << encrypt(category) << ',';
        }
        file << "\n\n";
    }

    file.close();
    cout << "Passwords have been encrypted and saved to " << filename << endl;
}


string decrypt(const string& input) {
    vector<char> word(input.begin(), input.end());
    string alf_lower = "abcdefghijklmnopqrstuvwxyz";  
    string alf_upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 

    for (int i = 0; i < (int)input.length(); ++i) {
        if (isalpha(word[i])) {
            string& alf = islower(word[i]) ? alf_lower : alf_upper;
            for (int j = 0; j < (int)alf.length(); ++j) {
                if (word[i] == alf[j]) {
                    word[i] = alf[(j -3) % 26];  // Сдвиг на 5 влево
                    break;
                }
            }
        }
    }

    string str(word.begin(), word.end());
    return str;
}

void decryptPasswords(vector<Password>& passwords, const string& filename) {
    ofstream file(filename,ios::in);
    if (!file.is_open()) {
        cerr << "Unable to open file for reading: " << filename << endl;
        return;
    }

    for (const auto& password : passwords) {
        file <<"name:"<< decrypt(password.name) << '\n';
        file <<"login:"<< decrypt(password.login) << '\n';
        file <<"password:"<< decrypt(password.password) << '\n';
        file<< "category:";

        for (const auto& category : password.category) {
            file << decrypt(category) << ',';
        }
        file << "\n\n";
    }
    file.close();
    addTimestamp(filename);
    cout << "Passwords have been decrypted from " << filename << endl;
}

void securePassword(const string& password){
    int lvl=0;
    bool digit= false, lower= false, upper= false, alnum= false;

    for (const char& p:password) {
        if (isdigit(p)){
            digit= true;
        }else if (islower(p)){
            lower=true;
        } else if (isupper(p)){
            upper=true;
        } else if (isalnum(p)){
            alnum=true;
        }
    }
    if (password.length()>=8){
        lvl +=6;
    }else if (password.length()>=5){
        lvl +=3;
    }

    if (digit){
        lvl +=1;
    }
    if (lower){
        lvl +=1;
    }
    if (upper){
        lvl +=1;
    }
    if (alnum){
        lvl +=1;
    }

    if (lvl> 7){
        println("Your password is strong: {}",lvl);
    } else if (lvl >=5){
        println("Your password is of medium complexity: {}",lvl);
    } else{
        println("Uour password is weak");
    }
}
string createPassword() {
    int size;
    bool digit, lower, upper, alnum;
    string cond;
    string comp="";
    string newpass;

    println("Enter password length:");
    cin >> size;
    if (cin.fail() || size<=0) {
        cin.clear();
        cin.ignore();
        println("Invalid input");
        return "";
    }

    println("Should the password include uppercase letters? (y/n):");
    cin >> cond;
    upper = cond =="y";

    println("Should the password include digits? (y/n):");
    cin >> cond;
    digit = cond == "y";

    println("Should the password include lowercase letters? (y/n):");
    cin >> cond;
    lower = cond == "y";

    println("Should the password include special characters? (y/n):");
    cin >> cond;
    alnum = cond == "y";

    if (upper) {
        comp += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }
    if (lower) {
        comp += "abcdefghijklmnopqrstuvwxyz";
    }
    if (alnum) {
        comp += "!@#$%^&*()_+'.?";
    }
    if (digit) {
        comp += "0123456789";
    }
    if (comp.empty()) {
        println("no character sets selected");
        return "";
    }

    for (int i = 0; i < size; ++i) {
        newpass += comp[rand() % comp.length()];
    }
    return newpass;
}
void addPassword(vector<Password>& passwords,const string& filename) {
    string name, login, password;
    vector<string> category;
    println("Enter name");
    getline(cin,name);
    println("Enter login");
    getline(cin,login);
    char choice;
    println("Do you want to input your password (i) or generate it automatically (a)? (i/a):");
    cin >> choice;
    cin.ignore();

    if (choice == 'a') {
        password = createPassword();
        if (password.empty()) {
            println("Password generation failed. Aborting operation.");
            return;
        }
        char mess;
        do {
            println("Generated password: {}", password);
            println("Add this generated password? (y/n)");
            cin >> mess;
            cin.ignore();
            switch (mess) {
                case 'y':
                    println("Password added successfully");
                    break;
                case 'n':
                    password = createPassword();
                    break;
                default:
                    println("Invalid operation");
                    break;
            }
        }
        while(mess != 'y');
    } else if (choice == 'i') {
        println("Enter password:");
        cin.ignore();
        getline(cin, password);
        securePassword(password);
    } else {
        println("Invalid option. Aborting operation.");
        return;
    }

    string categoryInp;
    println("Enter categories (, - separated)");
    cin.ignore();
    getline(cin,categoryInp);

    size_t start = 0, end;
    while ((end = categoryInp.find(',', start)) != string::npos) {
        category.push_back(categoryInp.substr(start, end - start));
        start = end + 1;
    }
    if (start < categoryInp.size()) {
        category.push_back(categoryInp.substr(start));
    }
    passwords.emplace_back(name, login, password, category);


    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cerr << "Unable to open file: " << filename << "\n";
        return;
    }
    file  << "name:" << name << "\n"
         << "login:" << login << "\n"
         << "password:" << password << "\n"
         << "category:";
    for (size_t i = 0; i < category.size(); ++i) {
        file << category[i];
        if (i < category.size() - 1) file << ",";
    }
    file << "\n";
    file.close();

    println("Password added successfully.");
}
void loadPasswords(const string& filename, vector<Password>& passwords) {
        ifstream file(filename, ios::in);

        if (!file.is_open()) {
            cerr << "Unable to open file: " << filename << "\n";
            return;
        }

        string line;
        string name, login, password, category;

        while (getline(file, line)) {
            if (line.substr(0, 5) == "name:") {
                if (!name.empty()) {
                    passwords.emplace_back(name, login, password, vector<string>{category});
                }
                name = line.substr(5);
                login.clear();
                password.clear();
                category.clear();
            } else if (line.substr(0, 6) == "login:") {
                login = line.substr(6);
            } else if (line.substr(0, 9) == "password:") {
                password = line.substr(9);
            } else if (line.substr(0, 9) == "category:") {
                category = line.substr(9);
            }
        }

        if (!name.empty()) {
            passwords.emplace_back(name, login, password, vector<string>{category});
        }

        file.close();
        println("Passwords loaded successfully from {}", filename);
}
string joinCategories(const vector<string>& categories) {
    string result;
    for (size_t i = 0; i < categories.size(); ++i) {
        result += categories[i];
        if (i < categories.size() - 1) {
            result += ", ";
        }
    }
    return result;
}
void findPassword(vector<Password>& passwords) {
    println("Do you want to find by:");
    println("1. name");
    println("2. login");
    println("3. category");

    string name, login, category;
    int answer;
    bool foundCategory = false;
    cin >> answer;
    cin.ignore();

    if (cin.fail()) {
        cin.clear();
        cin.ignore();
        println("Invalid answer");
        return;
    }

    switch (answer) {
        case 1:
            println("Enter name:");
            cin >> name;
            for (const Password &pass: passwords) {
                if (pass.name == name) {
                    println("{}:\nlogin: {}\npassword: {}\ncategory: {}", pass.name, pass.login, pass.password,
                            joinCategories(pass.category));
                    foundCategory= true;
                }
            }
            break;
        case 2:
            println("Enter login:");
            cin >> login;
            for (const Password &pass: passwords) {
                if (pass.login == login) {
                    println("{}:\nlogin: {}\npassword: {}\ncategory: {}", pass.name, pass.login, pass.password,
                            joinCategories(pass.category));
                    foundCategory= true;
                }
            }
            break;
        case 3:
            println("Enter category:");
            cin >> category;
            for (const Password &pass: passwords) {
                for (const string &cat: pass.category) {
                    if (cat == category) {
                        println("{}:\nlogin: {}\npassword: {}\ncategory: {}", pass.name, pass.login, pass.password,
                                joinCategories(pass.category));
                        foundCategory = true;
                    }
                }
            }
            break;
        default:
            println("Invalid operation");
            break;
    }
    if (!foundCategory){
        println("No passwords found for they selected category");
    }
}
void sortedByTwo(vector<Password>& passwords){
    if (passwords.empty()){
        println("The password list is empty");
        return;
    }
    println("Sorting by name and login");

    sort(passwords.begin(),passwords.end(),[](const Password &p1,const Password& p2){
        if (p1.name!=p2.name) {
            return p1.name < p2.name;
        }
        return p1.login<p2.login;
    });

    for (Password pass: passwords) {
        println("{}:\nlogin: {}\npassword: {}\ncategory: {}", pass.name, pass.login, pass.password, joinCategories(pass.category));
    }
}

void removePassword(vector<Password>& passwords, const string& filename) {
    string name;
    println("Enter the name of the password to remove or type 'all': ");
    cin >> name;
    if (name == "all") {
        println("Are you sure you want to remove all passwords? (y/n)");
        cin >> name;

        if (name == "y") {
            passwords.clear();
            ofstream output(filename, ios::trunc);
            if (output.is_open()) {
                output.close();
                println("All passwords have been removed");
            }
            output.close();
        }else{
            println("Passwords remain unchanged");
        }
    }

    else {
        ifstream input(filename);
        auto it = find_if(passwords.begin(), passwords.end(), [&name](const Password &p1) {
            return p1.name == name;
        });
        if (input.is_open()) {
            input.close();
            ofstream output(filename);
            if (output.is_open()) {
                if (it != passwords.end()) {
                    passwords.erase(it);
                    cout << "Password \"" << name << "\" has been removed." <<endl;
                } else {
                    cout << "Password \"" << name << "\" not found." << endl;
                }
            }
            for (const Password &pass: passwords) {
                output      << "name: " << pass.name << '\n'
                            << "login: " << pass.login << '\n'
                            << "password: " << pass.password << '\n'
                            << "category: ";
                for (size_t i = 0; i < pass.category.size(); ++i) {
                    output << pass.category[i];
                    if (i < pass.category.size() - 1) {
                        output << ", ";
                    }
                }
                output << '\n' << '\n';
            }
            output.close();
        }
    }
}
void editPassword(vector<Password>& passwords,  const string& filename) {
    string name, login, password, category;
    println("Enter the name of the password you want to edit: ");
    cin >> name;

    auto it = find_if(passwords.begin(), passwords.end(), [&](const Password& pass) {
        return pass.name == name;
    });

    if (it == passwords.end()) {
        cout << "Password with name \"" << name << "\" not found.\n";
        return;
    }
    cout << "Enter new login: ";
    cin.ignore();
    getline(cin, login);

    cout << "Enter new password: ";
    getline(cin, password);

    cout << "Enter new category: ";
    getline(cin, category);

    if (!login.empty()) it->login = login;
    if (!password.empty()) it->password = password;
    if (!category.empty()) it->category = {category};

    ofstream file(filename);
            if (!file.is_open()) {
                cerr << "Error opening file \"" << filename << "\" for writing.\n";
                return;
            }
            for (const auto& pass : passwords) {
                file << "name: " << pass.name << '\n'
                     << "login: " << pass.login << '\n'
                     << "password: " << pass.password << '\n'
                     << "category: ";
                for (size_t i = 0; i < pass.category.size(); ++i) {
                    file << pass.category[i];
                    if (i < pass.category.size() - 1) file << ", ";
                }
                file << "\n\n";
            }
        println("Passwords have been updated in the file.");
        file.close();
}
void deleteCategory(vector<Password> &passwords,const string& filename) {
    string category;
    println("Enter the name of the category you want to delete: ");
    cin >> category;

    bool categoryFound = false;

    for (Password &password : passwords) {
        auto it = remove(password.category.begin(), password.category.end(), category);
        if (it != password.category.end()) {
            categoryFound = true;
            password.category.erase(it, password.category.end());
        }
    }

    ofstream file(filename);
    if (file.is_open()) {
        for (const Password &password: passwords) {
            file << "name:" << password.name << '\n'
                 << "login:" << password.login << '\n'
                 << "password:" << password.password << '\n'
                 << "category:";
            for (size_t i = 0; i < password.category.size(); ++i) {
                file << password.category[i];
                if (i < password.category.size() - 1) {
                    file << ", ";
                }
            }
            file << "\n\n";
        }
        file.close();
        if (categoryFound) {
            cout << "Category \"" << category << "\" and associated data have been deleted.\n";
        } else {
            cout << "Category \"" << category << "\" not found.\n";
        }
    }
}

void addCategory(vector<Password> &passwords, const string& filename) {
    string category, name, answer;
    bool found;

    println("Enter the name of the password you want to add a new category to:");
    cin >> name;

    println("Enter new category:");
    cin >> category;

    for (Password& password : passwords) {
        if (password.name == name) {
            password.category.push_back(category);
            found=true;
            break;
        }
    }
    if (found) {
        ofstream file(filename);
        if (file.is_open()) {
            for (const Password& password : passwords) {
                file << "name:" << password.name << '\n'
                     << "login:" << password.login << '\n'
                     << "password: " << password.password << '\n'
                     << "category:";
                for (size_t i = 0; i < password.category.size(); ++i) {
                    file << password.category[i];
                    if (i < password.category.size() - 1) {
                        file << ", ";
                    }
                }
                file << "\n\n";
            }
            println("Category has been added");
            file.close();
        }
    } else {
        println("No password found with the specified name");
    }
}

int main() {
    vector<Password> passwords;
    string filePath;
    println("Enter the file name: ");
    getline(cin, filePath);

    println("File selected:{} ", filePath);
    if (!filesystem::exists(filePath)) {
        cerr << "The specified file does not exist. Program terminated\n";
        return 1;
    }
    loadPasswords(filePath,passwords);

    encryptPasswords(passwords, filePath);

    string pass;
    int count = 0;

    while (count < 3) {
        println("Enter password for Menager:");
        cin >> pass;

        if (pass.length() < 8) {
                decryptPasswords(passwords, filePath);
                println("Correct password!");
                int answer;

                do {
                    menu();
                    cin >> answer;
                    switch (answer) {
                        case 1:
                            findPassword(passwords);
                            break;
                        case 2:
                            sortedByTwo(passwords);
                            break;
                        case 3:
                            addPassword(passwords, filePath);
                            break;
                        case 4:
                            editPassword(passwords, filePath);
                            break;
                        case 5:
                            removePassword(passwords, filePath);
                            break;
                        case 6:
                            addCategory(passwords, filePath);
                            break;
                        case 7:
                            deleteCategory(passwords, filePath);
                            break;
                        case 8:
                            println("Good bye");
                            break;
                        default:
                            println("Invalid operation");
                            break;
                    }
                } while (answer != 9);
                break;

            } else {
                encryptPasswords(passwords, filePath);
                println("Incorrect password. Left", 2 - count, " attempts");
                count++;
            }
            if (count >= 3) {

                println("Attempt limit exceeded. Program will be closed");
                encryptPasswords(passwords, filePath);
            }
        }
}
