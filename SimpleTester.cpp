// Simple Tester v3.0
// Damian Strojek s184407
// Refactored this code in 04.2023
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <ctime>
#include <algorithm>
#include <random>

struct answer {
	std::string desc;
	bool isGood;
};

class Question {
private:
	std::string correctAnswer;
	std::string question;
	std::vector<answer> answers;
	int timesViewed;
	int numberOfCorrectAnswers;
public:
	Question(std::string _question) : question(_question), timesViewed(0), numberOfCorrectAnswers(0) {}
	
	std::string getCorrectAnswer() { return this->correctAnswer; }
	std::string getQuestion() { return this->question; }
	std::vector<answer> &getAnswers() { return this->answers; } 
	answer getAnswerIndex(const int index) { return this->answers[index]; }
	int getTimesViewed() { return this->timesViewed; }
	int getNumberCorrect() { return this->numberOfCorrectAnswers; }

	void setCorrectAnswerIndex(const int index) { 
		this->answers[index].isGood = true; 
		this->numberOfCorrectAnswers++;
	}

	void setQuestion(std::string _question) { this->question = _question; }

	void addAnswer(std::string _answer) {
		answer temp;
		temp.desc = _answer;
		temp.isGood = false;
		this->answers.push_back(temp);
	}

	void addView() { timesViewed++; }
};

int sortQuestions(Question _question1, Question _question2);
int invokeError(int status);
void helloMessage();
std::ifstream loadFile();
void loadQuestionsAnswers(std::ifstream &databaseFile, std::vector<Question> &questions,
							 std::string _question, int &numberOfQuestions);
void displayQuestions(int &correctAnswers, std::vector<Question> &questions);
void displayCorrectAnswers(const int index, std::vector<Question> &questions);

int main(int argc, char* argv[]) {

	helloMessage();
	std::ifstream databaseFile = loadFile();
	std::vector<Question> questions;
	std::string temp;
	int numberOfQuestions = 0;

	while(databaseFile.good()){
		std::getline(databaseFile, temp);
		if(!temp.empty()) loadQuestionsAnswers(databaseFile, questions, temp, numberOfQuestions);
	}

	std::cout << "\n\t\tNumber of questions loaded from the file: " << numberOfQuestions 
	<< "\n\t\tPress enter if you are ready for the first one. ";
	
	// Had to come up with something better than while(true)
	int correctAnswers = 0;
	srand(time(NULL));
	while(correctAnswers < 200){
		displayQuestions(correctAnswers, questions);
	}

	invokeError(1);
	return 0;
};

int sortQuestions(Question _question1, Question _question2) {
	return _question1.getTimesViewed() < _question2.getTimesViewed();
};

int invokeError(int status) {
	if (!status) std::cout << "\n\t[ERROR] Loading of the file failed.\n";
	else if (status) std::cout << "\n\t[GOODBYE] Thank you for using SimpleTester.\n";
	else if (status == 2) std::cout << "\n\t[ERROR] Loading of an answer failed.\n";
	
	// Pause for end user
	std::string stop;
	getline(std::cin, stop);

	exit(1);
	return status;
};

void helloMessage(){
	system("cls");
	std::cout << "\n\tSimpleTester release 2.0\n\n\tContributors: damianStrojek@2023" << 
	", Lemm@2012, Pitya@2022\n\n\tHave fun learning. Fuck GUT.\n\n";

	return;
};

std::ifstream loadFile(){
	std::cout << "\t\tInsert name of the database file [ANSI encoded]: ";
	std::string databaseName;
	std::cin >> databaseName;

	std::ifstream inputFile;
	inputFile.open(databaseName, std::ios::in);
	if(!inputFile.good()) invokeError(0);

	return inputFile;
};

void loadQuestionsAnswers(std::ifstream &databaseFile, std::vector<Question> &questions, 
							std::string _question, int &numberOfQuestions){
	std::string temp;
	std::getline(databaseFile, temp);
	// Check if the first symbol is a number
	// This while is to make sure that the "multiple line questions" will get processed too
	while (!((int)temp[0] > 48 && (int)temp[0] < 57)) { 
		if (!((int)temp[0] > 48 && (int)temp[0] < 57)) _question += '\n' + temp;
		std::getline(databaseFile, temp);
	}

	Question newQuestion(_question);
	int numberOfAnswers = temp[0] - '0', i, index;
	for(i = 0; i < numberOfAnswers && databaseFile.good(); i++){
		std::getline(databaseFile, temp);
		newQuestion.addAnswer(temp);
	}
	
	if(i == numberOfAnswers){
		std::getline(databaseFile, temp);
		for(int j = 0; j < temp.size(); j++){
			if(temp[i] > 96 && temp[i] < 105){
				index = (int)temp[i] - 97;
				newQuestion.setCorrectAnswerIndex(index);
			}
		}
		questions.push_back(newQuestion);
		numberOfQuestions++;
	}
};

void displayQuestions(int &correctAnswers, std::vector<Question> &questions){
	
	std::string answer;
	int counterOfQuestions = 0;
	std::cout << "\n\tYour actual score: " << (double)((double)correctAnswers / 
			(double)(counterOfQuestions == 0 ? 1 : counterOfQuestions)) * 100.0f << 
			"% (" << correctAnswers << "/" << counterOfQuestions << ")\n\n";

	// The program should choose questions with the lesser amount of successful answers 
	// or those with bad answers.

	// ---------------- TODO Create a logical sorting mechanism
	// BEFORE:
	/*
	if (nrpyt % lpytan == 0) std::shuffle(std::begin(pytania), std::end(pytania), rng);
		else sort(pytania.begin(), pytania.end(), sortQuestions);
		int rnd = rand() % (pytania.size()) / 4;
	*/

	// "rnd" doesn't make sense if the sorting mechanism is going to work properly
	int rnd = rand() % (questions.size()) / 4;
	std::cout << "\n\t" << questions[rnd].getQuestion() << "\n";
	int numberOfAnswers = questions[rnd].getAnswers().size();

	auto rng = std::default_random_engine{};
	std::shuffle(std::begin(questions[rnd].getAnswers()), std::end(questions[rnd].getAnswers()), rng);
	
	for(int i = 0; i < numberOfAnswers; i++) std::cout << "\t\t" << (char)(i + 'a') << ". " << 
												questions[rnd].getAnswers()[i].desc << "\n";
	
	std::cout << "\n\tAnswer: ";
	std::getline(std::cin, answer);
	bool ansIncorrectly  = false;

	for(int i = 0; i < answer.size(); i++){
		if(answer[i] > 96 && answer[i] < 105){
			int index = (int)answer[i] - 97;
			if(index >= questions[rnd].getAnswers().size()) invokeError(2);
			else if(questions[rnd].getAnswers()[index].isGood){
				correctAnswers++;
				continue;
			}
			else {
				displayCorrectAnswers(rnd, questions);
				ansIncorrectly = true;
				break;
			}
		}
	}

	if(questions[rnd].getNumberCorrect() != correctAnswers && ansIncorrectly)
		displayCorrectAnswers(rnd, questions);
	else if(!ansIncorrectly){
		std::cout << "\n\t\tWell done! This answer is correct.\n";
		correctAnswers++;
	}
	else 
		std::cout << "\n\t\t[DEBUG] Is this possible?\n";

	questions[rnd].addView();
	numberOfAnswers++;

	std::cout <<"\n\t[NEXT QUESTION]";
	std::getline(std::cin, answer);
	system("cls");
};

void displayCorrectAnswers(const int index, std::vector<Question> &questions){
	std::cout << "\n\t\tIncorrect answer. Correct is/are: \n";
	for(int j = 0; j < questions[index].getAnswers().size(); j++)
		if(questions[index].getAnswers()[j].isGood)
			std::cout << "\t\t\t" << questions[index].getAnswers()[j].desc << "\n";
};

/*
// OLD DEBUG
void debug(std::vector<Question> pytania) {
	for (int i = 0; i < pytania.size(); i++) {
		std::cout << pytania[i].question << "\n";
		for (int j = 0; j < pytania[i].answers.size(); j++) std::cout << (char)(j + 'a') << ". " << pytania[i].answers[j].desc << "   |   " << pytania[i].answers[j].isGood << "\n\n";
	}
};
*/