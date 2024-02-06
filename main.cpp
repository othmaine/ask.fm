#include <iostream>
#include <map>
#include <string>
#include <set>

#define ll long long
class Message;
std::map<ll, Message*> messages;
std::ostream& operator<<(std::ostream &stream, Message &message);
class Message
{
private:
    ll my_id;
    static ll id;
    std::string my_body;
    std::string my_from,my_to;
    std::set<ll> threads;
protected:
    Message(const std::string &myBody, const std::string &myForm, const std::string &myTo)
    : my_body(myBody),
    my_from(myForm),
    my_to(myTo)
    {
        my_id = id;
        messages[this->getId()] = this;
        id++;
    }
    void setFrom(std::string& from)
    {
        my_from = from;
    }
    void setTo(std::string& to)
    {
        my_to = to;
    }

public:
    ll getId() const
    {
        return my_id;
    }
    std::string getBody() const
    {
        return my_body;
    }
    std::string getFrom() const
    {
        return my_from;
    }
    std::string getTo() const
    {
        return my_to;
    }
    std::set<ll> getTreadsId() const
    {
        return threads;
    }

    void addThread(ll id)
    {
        threads.insert(id);
    }
    void print()
    {
        std::cout<<*this;

    }

};
ll Message::id = 0;
class Question : public Message // TODO add threads
{
private:
    ll idOfAnswer;
public:
    Question(const std::string &myBody, const std::string &myForm, const std::string &myTo)
    : Message(myBody, myForm,myTo)
    {
        idOfAnswer = -1;
    }
    void setAnswer(ll i)
    {
        idOfAnswer = i;
    }
    ll getAnswer() const
    {
        return idOfAnswer;
    }
};
class Answer : public Message
{
private:
    ll my_idOfQuestion;
public:
    Answer(ll idOfQuestion,
           std::string &body) : Message(body, "", ""), my_idOfQuestion(idOfQuestion)
    {
        std::string to = messages[idOfQuestion]->getFrom();
        std::string from = messages[idOfQuestion]->getTo();
        setFrom(from);
        setTo(to);
    }
    void print()
    {
        messages[my_idOfQuestion]->print();
        ((Message*)this)->print();
    }
};

/*class Blog
{
private:
    ll idQuestion,idAnswer;
    std::string from, to;
public:
    Blog(long long int idQuestion, long long int idAnswer, const std::string &from, const std::string &to) : idQuestion(
            idQuestion), idAnswer(idAnswer), from(from), to(to) {}

    Blog() {}


};*/




std::ostream& operator<<(std::ostream &stream, Message &message)
{
    std::cout<<"\nTo: "<<message.getTo()<<"\n______________\n"<<message.getBody()<<"\n"<<"______________\n"<<"From: "
             <<message.getFrom()<<"\n___________________\n";
    return stream;
}

class Thread : Message
{
    ll idOfMessage;
public: // CONDITION : id Must be a valid && answered message
    Thread(const std::string &myBody, const std::string &myForm, const std::string &myTo, long long int idOfMessage)
            : Message(myBody, myForm, myTo), idOfMessage(idOfMessage){}
};

class User;
std::map<std::string,User*> users;
class User
{
private:
    std::string my_password,my_username;
    std::set<ll> questions,answers,notAnswered;

public:
    User(const std::string &myPassword, const std::string &myUsername) :
    my_password(myPassword),my_username(myUsername){}
    void addQuestion(ll id) // The question that the user got
    {
        questions.insert(id);
        notAnswered.insert(id);
    }
    void addAnswerOrThread(ll id)
    {
        answers.insert(id);
    }
    void removeFromNotAnswered(ll id)
    {
        notAnswered.erase(id);
    }
    std::set<ll> getAllQuestions()
    {
        return questions;
    }
    std::set<ll> getAllAnswers()
    {
        return answers;
    }
    std::set<ll> getAllUnAnswered()
    {
        return notAnswered;
    }
    bool checkPasswordMatch(const std::string& s)
    {
        return s == my_password;
    }
    static void addUser(std::string& password, std::string& username)
    {
        User* user = new User(password,username);
        users[username] = user;
    };
};
void createUser()
{
    std::string username;
    while(true)
    {
        std::cout<<"\nEnter the user name";
        std::cin>>username;
        if(!users.count(username))
            break;
        std::cout<<"\nthis name is already used, try again: ";
    }
    std::string password, confirm;
    std::cout<<"\nEnter the password:";
    std::cin>>password;
    while(true)
    {
        std::cout<<"\n confirm the password again\n";
        std::cin>>confirm;
        if(confirm == password)
            break;
        std::cout<<"doesn't match, try again\n";
    }
    users[username] = new User(username,password);
}
void answer(std::string &from)
{
    std::cout<<"\n (answer) or (thread)?(y)/(n)";
    std::string choice;
    while(true)
    {
        std::cout<<"\nchoice: ";
        std::cin>>choice;
        if(choice == "answer" || choice == "thread")
            break;
        std::cerr<<"\nanswer or thread ONLY !!";

    }
    if(choice == "answer")
    {
        for(const auto &x:users[from]->getAllUnAnswered())
        {
            std::cout<<"\nthis question ?(y/n)\n";
            messages[x]->print();
            while(true)
            {
                std::cin>>choice;
                if(choice == "y" || choice == "n")
                    break;
                std::cerr<<"\n(y\n) ONLY\n";
            }
            if(choice == "y")
            {
                std::cout<<"\nEnter the body\n";
                std::string body;
                std::getline(std::cin.ignore(),body);
                Answer* my_answer = new Answer(x,body);
                users[from]->addAnswerOrThread(my_answer->getId());
                users[from]->removeFromNotAnswered(my_answer->getId());
                break;
            }
        }
    }
    else
    {

    }
    /*{
        ll id;
        std::cout<<"Enter the name of the user: ";
        std::string to;
        while(true)
        {
            std::cin>>to;
            if(users.contains(to))
                break;
            std::cout<<"can't find such a user\n again: ";
        }
        for(const auto &x:users[from]->getAllAnswers())
        {
            std::cout<<"\nthis question ?(y/n)\n";
            messages[x]->print();
            while(true)
            {
                std::cin>>choice;
                if(choice == "y" || choice == "n")
                    break;
                std::cerr<<"\n(y\n) ONLY\n";
            }
            if(choice == "y")
            {
                std::cout<<"\nEnter the body\n";
                std::string body;
                std::cin>>body;
                Answer* my_answer = new Answer(body, from);
                users[from]->addAnswerOrThread(my_answer->getId());
                users[from]->removeFromNotAnswered(my_answer->getId());
                break;
            }
        }
    }*/
}
void ask(std::string &from)
{
    std::string to;
    std::cout<<"\nWho you want to ask?";
    while(true)
    {
        std::cout<<"\nName: ";
        std::cin>>to;
        if(users.contains(to))
            break;
        std::cout<<"\nNo user found";
    }
    std::string theQuestion;
    std::cout<<"Enter the question\n";
    std::getline(std::cin.ignore(),theQuestion);
    Question* question = new Question(theQuestion,from,to);
    users[to]->addQuestion(question->getId());
}

void readAllAnswers()
{
    std::cout<<"\nWho you want to read his answer?";
    std::string name;
    while(true)
    {
        std::cout<<"\nName: ";
        std::cin>>name;
        if(users.contains(name))
            break;
        std::cout<<"\nNo user found";
    }
    User* user = users[name];
    auto answers = user->getAllAnswers();
    for(auto x:answers)
    {
        Answer* message = static_cast<Answer *>(messages[x]);
        message->print();
    }

}

void serviceUser(std::string username)
{
    std::cout<<"select a choice\n(ask) a question\n(answer) a question\n(read) all answers of a user\n(log_out)";
    std::string choice;
    while(1)
    {
        std::cout<<"\nchoice: ";
        std::cin>>choice;
        if(choice == "ask" || choice == "answer" || choice == "read" || choice == "log_out")
            break;
        std::cout<<"\nWrong choice, try again";
    }
    if(choice == "ask")
        ask(username);
    else if(choice == "answer")
        answer(username);
    else if(choice == "read")
        readAllAnswers();
}
void login()
{
    std::cout<<"select a choice\n(sign) in\n(create) account\nchoice: ";
    std::string choice;
    while(1)
    {
        std::cin>>choice;
        if(choice == "sign" || choice == "create")
            break;
        std::cout<<"\nWrong choice, try again\n";
    }
    if(choice == "sign")
    {
        std::string userName;
        while(true)
        {
            std::cout<<"enter the userName: ";
            std::cin>>userName;
            if(users.contains(userName))
                break;
            std::cout<<"Can't find such a user\n";
        }
        std::string password;
        while(true)
        {
            std::cout<<"Enter the password: ";
            std::cin>>password;
            if(users[userName]->checkPasswordMatch(password))
                break;
            std::cout<<"Wrong password!\n ";
        }
        serviceUser(userName);
    }
    else if(choice == "create")
    {
        std::string userName;
        while(true)
        {
            std::cout<<"enter the userName: ";
            std::cin>>userName;
            if(!users.contains(userName))
                break;
            std::cout<<"already exists, try another\n";
        }

        std::string password;
        while(true)
        {
            std::cout<<"Enter the password: ";
            std::cin>>password;
            std::string confirm;
            std::cout<<"confirm: ";
            std::cin>>confirm;
            if(password == confirm)
                break;
            std::cout<<"doesn't match!\n ";
        }
        User::addUser(password,userName);
        serviceUser(userName);
    }
}
int main()
{
    std::string contin = "YES";
    while(contin == "YES")
    {
        login();
        std::cout<<"Continue?";
        std::cin>>contin;
        for(auto& x:contin)
            if(x > 'Z')
                x-= ('a' - 'A');
    }
}
// TODO
//  Threads
//  Use lamda instead of while(1)....etc
//  Don't miss  `back` option
//  Files, to save data
//  Memory management
//  Make sure that it can work in PARALLEL
//  Set anonymous future