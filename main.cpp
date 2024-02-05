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
};
std::map<std::string,User*> users;
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
        ll id;
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
                std::cin>>body;
                Answer* my_answer = new Answer(id,body);
                users[from]->addAnswerOrThread(my_answer->getId());
                users[from]->removeFromNotAnswered(my_answer->getId());
                users[from]-> // Continue
                break;
            }
        }
    }
    else
    {
        ll id;
        std::cout<<"Enter the name of the user";
        std::string to;
        while(true)
        {
            std::cin>>to;
            if(users.contains(to))
                break;
        }
        for(const auto &x:users[from]->getAllAnswers()
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
                Answer* my_answer = new Answer(<#initializer#>, <#initializer#>, <#initializer#>, body, from);
                users[from]->addAnswerOrThread(my_answer->getId());
                users[from]->removeFromNotAnswered(my_answer->getId());
                break;
            }
        }
    }
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
    std::cin>>theQuestion;
    Question* question = new Question(theQuestion,from,to);
    users[to]->addQuestion(question->getId());
}


void serviceUser(std::string username)
{
    std::cout<<"select a choice\n(ask) a question\n(answer) a question\n(read) all answers of a user";
    std::string choice;
    while(1)
    {
        std::cout<<"\nchoice: ";
        std::cin>>choice;
        if(choice == "ask" || choice == "answer" || choice == "read")
            break;
        std::cout<<"\nWrong choice, try again";
    }
    if(choice == "ask")
        ask(username);
}
int main()
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
    }
}
// TODO use lamda instead of while(1)....etc